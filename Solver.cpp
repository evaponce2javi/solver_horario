// ⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘
//  Solver.cpp
// ⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘

#include "Solver.h"
#include <array>
#include <algorithm>

// ⫘⫘⫘ Construcción de variables + arranque del backtracking ⫘⫘⫘
std::vector<Solucion> Solver::resolver(const Registro& registro,
                                       std::vector<std::string>& excluidas) {
    std::vector<Variable> variables;

    for (const auto& [sigla, asignatura] : registro.asignaturas()) {
        if (asignatura.paralelos().empty()) {
            excluidas.push_back(sigla);   // decisión fija #5: se excluye con aviso
            continue;
        }
        Variable var;
        var.sigla = sigla;
        for (const auto& [numero, paralelo] : asignatura.paralelos()) {
            (void)numero;
            var.dominio.push_back(&paralelo);
        }
        variables.push_back(std::move(var));
    }

    std::vector<Solucion> soluciones;
    if (variables.empty()) return soluciones;  // ninguna asignatura resoluble

    std::vector<EleccionParalelo> parcial;
    backtrack(variables, 0, parcial, 0, soluciones);
    return soluciones;
}

// ⫘⫘⫘ DFS: única poda permitida = violación de la restricción dura ⫘⫘⫘
// "Todo o nada": una solución solo se registra al asignar TODAS las variables.
void Solver::backtrack(const std::vector<Variable>& variables, std::size_t indice,
                       std::vector<EleccionParalelo>& parcial, uint64_t mascaraAcum,
                       std::vector<Solucion>& soluciones) {
    if (indice == variables.size()) {
        Solucion solucion;
        solucion.elecciones       = parcial;
        solucion.mascaraCombinada = mascaraAcum;
        solucion.firma            = firmaSolucion(parcial);
        solucion.scoreVentanas    = calcularVentanas(solucion);
        calcularHorario(solucion);
        soluciones.push_back(std::move(solucion));
        return;
    }

    const Variable& var = variables[indice];
    for (const Paralelo* paralelo : var.dominio) {
        // Poda dura: si comparte alguna celda con lo acumulado, jamás será válido.
        if ((mascaraAcum & paralelo->mascara()) != 0) continue;

        EleccionParalelo eleccion;
        eleccion.sigla    = var.sigla;
        eleccion.numero   = paralelo->numero();
        eleccion.sesiones = paralelo->sesiones();

        parcial.push_back(std::move(eleccion));
        backtrack(variables, indice + 1, parcial,
                  mascaraAcum | paralelo->mascara(), soluciones);
        parcial.pop_back();
    }
}

// ⫘⫘⫘ Score VENTANAS ⫘⫘⫘
// Por día: la corrida vacía máxima con clase antes y después. Suma los 5 días.
int Solver::calcularVentanas(const Solucion& solucion) {
    std::array<std::array<bool, NUM_CLAVES>, NUM_DIAS> ocupado{};

    for (const auto& eleccion : solucion.elecciones) {
        for (const Sesion& s : eleccion.sesiones) {
            if (!s.online) ocupado[s.dia][s.clave] = true;
        }
    }

    int total = 0;
    for (int d = 0; d < NUM_DIAS; ++d) {
        int previa      = -1;   // última clave ocupada vista
        int maxVentana  = 0;
        for (int c = 0; c < NUM_CLAVES; ++c) {
            if (ocupado[d][c]) {
                if (previa != -1) {
                    int hueco = c - previa - 1;      // vacías entre dos clases
                    if (hueco > maxVentana) maxVentana = hueco;
                }
                previa = c;
            }
        }
        total += maxVentana;
    }
    return total;
}

// ⫘⫘⫘ Score HORARIO ⫘⫘⫘
// Guarda suma y cantidad de claves presenciales: el criterio es el PROMEDIO
// (suma/cantidad), no la suma cruda. Menor promedio = clases más de mañana.
// Con la suma cruda, un paralelo con UNA sesión tardía (clave 5 ⇒ 5) le ganaba
// a uno con dos sesiones tempranas (claves 0 y 6 ⇒ 6), premiando "tener menos
// clases" en vez de "tenerlas temprano". El promedio (5.0 vs 3.0) lo corrige.
void Solver::calcularHorario(Solucion& solucion) {
    int suma     = 0;
    int cantidad = 0;
    for (const auto& eleccion : solucion.elecciones) {
        for (const Sesion& s : eleccion.sesiones) {
            if (!s.online) { suma += s.clave; ++cantidad; }   // las online no cuentan
        }
    }
    solucion.sumaClaves           = suma;
    solucion.sesionesPresenciales = cantidad;
}

// ⫘⫘⫘ Órdenes totales (desempate determinista) ⫘⫘⫘
bool Solver::mejorPorVentanas(const Solucion& a, const Solucion& b) {
    if (a.scoreVentanas != b.scoreVentanas) return a.scoreVentanas < b.scoreVentanas;
    const int c = compararHorario(a, b);           // desempate 1: el otro criterio
    if (c != 0) return c < 0;
    return a.firma < b.firma;                      // desempate 2: firma canónica
}

bool Solver::mejorPorHorario(const Solucion& a, const Solucion& b) {
    const int c = compararHorario(a, b);
    if (c != 0) return c < 0;
    if (a.scoreVentanas != b.scoreVentanas) return a.scoreVentanas < b.scoreVentanas;
    return a.firma < b.firma;
}

// ⫘⫘⫘ Dominancia de Pareto ⫘⫘⫘
bool Solver::domina(const Solucion& a, const Solucion& b) {
    const int cmpHorario  = compararHorario(a, b);
    const bool noPeorVent = a.scoreVentanas <= b.scoreVentanas;
    const bool noPeorHor  = cmpHorario <= 0;
    const bool mejorAlgo  = (a.scoreVentanas < b.scoreVentanas) || (cmpHorario < 0);
    return noPeorVent && noPeorHor && mejorAlgo;
}

// ⫘⫘⫘ Frente de Pareto por barrido (skyline) ⫘⫘⫘
// Ordenando por (ventanas ↑, horario ↑), una solución es no dominada si y solo
// si su horario mejora ESTRICTAMENTE al mejor horario visto hasta el momento.
// O(n log n) en vez de comparar todos contra todos, O(n²).
std::vector<const Solucion*> Solver::frentePareto(const std::vector<Solucion>& soluciones) {
    std::vector<const Solucion*> orden;
    orden.reserve(soluciones.size());
    for (const Solucion& s : soluciones) orden.push_back(&s);

    std::sort(orden.begin(), orden.end(),
              [](const Solucion* a, const Solucion* b) { return mejorPorVentanas(*a, *b); });

    std::vector<const Solucion*> frente;
    for (const Solucion* s : orden) {
        if (frente.empty() || compararHorario(*s, *frente.back()) < 0) {
            frente.push_back(s);   // nadie anterior la domina
        }
    }
    return frente;
}
