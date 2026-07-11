// ⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘
//  Solver.cpp
// ⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘

#include "Solver.h"
#include <array>

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
void Solver::backtrack(const std::vector<Variable>& variables, std::size_t indice,
                       std::vector<EleccionParalelo>& parcial, uint64_t mascaraAcum,
                       std::vector<Solucion>& soluciones) {
    if (indice == variables.size()) {
        Solucion solucion;
        solucion.elecciones       = parcial;
        solucion.mascaraCombinada = mascaraAcum;
        solucion.scoreVentanas    = calcularVentanas(solucion);
        solucion.scoreHorario     = calcularHorario(solucion);
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
// Suma de índices de clave de las sesiones presenciales. Menor = más mañana.
int Solver::calcularHorario(const Solucion& solucion) {
    int total = 0;
    for (const auto& eleccion : solucion.elecciones) {
        for (const Sesion& s : eleccion.sesiones) {
            if (!s.online) total += s.clave;
        }
    }
    return total;
}
