// ⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘
//  Menu.cpp — Interacción de consola: las 6 acciones + salir.
// ⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘

#include "Menu.h"
#include "Utilidades.h"
#include "Solver.h"
#include "Tabla.h"
#include "Sesion.h"

#include <iostream>
#include <iomanip>
#include <algorithm>
#include <string>
#include <vector>

namespace {

// Tope de tablas impresas: con 5 ramos x 4 paralelos hay 1024 soluciones y
// escupirlas todas a consola es inservible. Los criterios y el frente de
// Pareto siguen calculándose sobre TODAS; el tope es solo de presentación.
constexpr std::size_t MAX_TABLAS_MOSTRADAS = 50;

// ⫘⫘⫘ Invalidación de resultados ⫘⫘⫘
// Toda mutación del registro deja obsoletas las soluciones ya calculadas.
// Sin esto, resolver → borrar una asignatura → "mostrar resultados" seguía
// dibujando horarios con la asignatura eliminada.
void invalidarSoluciones(EstadoApp& estado) {
    estado.soluciones.clear();
    estado.haResuelto = false;
}

// ⫘⫘⫘ Helpers de listado ⫘⫘⫘
void listarAsignaturasConConteo(const Registro& registro) {
    if (registro.vacio()) {
        std::cout << "(No hay asignaturas registradas)\n";
        return;
    }
    for (const auto& [sigla, asignatura] : registro.asignaturas()) {
        std::cout << "- " << sigla
                  << " (con " << asignatura.cantidadParalelos() << " paralelos)\n";
    }
}

void imprimirSesionesParalelo(const Paralelo& paralelo) {
    const auto& sesiones = paralelo.sesiones();
    if (sesiones.empty()) {
        std::cout << "(sin sesiones)";
        return;
    }
    for (std::size_t i = 0; i < sesiones.size(); ++i) {
        const Sesion& s = sesiones[i];
        if (s.online) std::cout << "ONLINE";
        else          std::cout << ETIQUETAS_DIAS[s.dia] << " " << ETIQUETAS_CLAVES[s.clave];
        if (i + 1 < sesiones.size()) std::cout << ", ";
    }
}

// ⫘⫘⫘ Opción 1 — Añadir asignatura ⫘⫘⫘
void opcionAnadirAsignatura(EstadoApp& estado) {
    std::string sigla;
    while (true) {
        std::string entrada = aMayusculas(
            trim(leerLinea("Ingrese la sigla de la asignatura (Ejemplo: ICI4247)")));
        if (esSiglaValida(entrada)) { sigla = entrada; break; }
        std::cout << "Error: Utilice el formato adecuado (Ejemplo: ICI4247)\n";
    }

    if (estado.registro.agregarAsignatura(sigla)) {
        invalidarSoluciones(estado);
        std::cout << "Asignatura guardada con éxito.\n";
    } else {
        std::cout << "Aviso: la asignatura " << sigla << " ya existe en el registro.\n";
    }
}

// ⫘⫘⫘ Opción 2 — Eliminar asignatura ⫘⫘⫘
void opcionEliminarAsignatura(EstadoApp& estado) {
    if (estado.registro.vacio()) {
        std::cout << "No hay asignaturas para eliminar.\n";
        return;
    }
    std::cout << "Asignaturas disponibles para eliminar:\n";
    listarAsignaturasConConteo(estado.registro);

    while (true) {
        std::string sigla = aMayusculas(
            trim(leerLinea("Ingrese la sigla de la asignatura a eliminar:")));
        int borrados = estado.registro.eliminarAsignatura(sigla);
        if (borrados >= 0) {
            invalidarSoluciones(estado);
            std::cout << sigla << " eliminado con éxito junto con sus "
                      << borrados << " paralelos.\n";
            return;
        }
        // No existe → submenú.
        std::cout << "Error: La asignatura no existe en el registro. Ingrese una opción:\n"
                  << "1. Eliminar asignatura\n"
                  << "2. Volver al menú\n";
        bool reintentar = false;
        while (!reintentar) {
            std::string opcion = trim(leerLinea(""));
            if (opcion == "1")      reintentar = true;   // vuelve a pedir sigla
            else if (opcion == "2") return;
            else std::cout << "Error: Escoja una opción válida.\n";
        }
    }
}

// ⫘⫘⫘ Opción 3 — Añadir paralelo ⫘⫘⫘
void opcionAnadirParalelo(EstadoApp& estado) {
    if (estado.registro.vacio()) {
        std::cout << "No hay asignaturas registradas. Cree una primero (opción 1).\n";
        return;
    }
    std::cout << "Asignaturas disponibles:\n";
    listarAsignaturasConConteo(estado.registro);

    std::string sigla = aMayusculas(trim(leerLinea("Ingrese la sigla de la asignatura:")));
    Asignatura* asignatura = estado.registro.buscar(sigla);
    if (!asignatura) {
        std::cout << "Error: La asignatura no existe en el registro.\n";
        return;
    }

    // 1) Número de paralelo (entero estricto).
    int numero = 0;
    while (true) {
        std::string entrada = leerLinea("Ingrese el número de paralelo (ejemplo: 1):");
        if (parsearEntero(entrada, numero)) break;
        std::cout << "Error: Ingrese un número válido.\n";
    }

    // 2) Día (o token ONLINE, que salta la clave).
    bool online = false;
    int  dia    = DIA_ONLINE;
    int  clave  = CLAVE_ONLINE;
    while (true) {
        std::string entrada = leerLinea("Ingrese el día (ejemplo: VIERNES, o ONLINE):");
        if (esTokenOnline(entrada)) { online = true; break; }
        int d = parsearDia(entrada);
        if (d >= 0) { dia = d; break; }
        std::cout << "Error: Ingrese un día válido.\n";
    }

    // 3) Clave horaria (o token ONLINE).
    if (!online) {
        while (true) {
            std::string entrada = leerLinea("Ingrese la clave horaria (ejemplo: 1-2, o ONLINE):");
            if (esTokenOnline(entrada)) {
                online = true; dia = DIA_ONLINE; clave = CLAVE_ONLINE; break;
            }
            int c = parsearClave(entrada);
            if (c >= 0) { clave = c; break; }
            std::cout << "Error: Ingrese una clave válida.\n";
        }
    }

    // 4) Unicidad (sigla, número, día, clave).
    if (asignatura->tieneParalelo(numero)) {
        const Paralelo& existente = asignatura->paralelos().at(numero);
        if (existente.existeSesion(dia, clave, online)) {
            std::cout << "Aviso: esa sesión ya existe para " << sigla << "-" << numero
                      << ". No se duplicó.\n";
            return;
        }
    }

    // 5) Resumen + confirmación (solo acepta 1 o 2).
    std::cout << "\n" << sigla << "-" << numero << " ";
    if (online) std::cout << "ONLINE\n";
    else        std::cout << ETIQUETAS_DIAS[dia] << " CLAVE " << ETIQUETAS_CLAVES[clave] << "\n";
    std::cout << "1. Deshacer datos\n2. Confirmar datos\n";

    while (true) {
        std::string opcion = trim(leerLinea("Escoja una opción (ejemplo: 2)"));
        if (opcion == "1") {
            std::cout << "Datos descartados.\n";
            return;
        }
        if (opcion == "2") {
            Sesion sesion{dia, clave, online};
            Paralelo& paralelo = asignatura->obtenerOCrearParalelo(numero);
            paralelo.agregarSesion(sesion);
            invalidarSoluciones(estado);
            std::cout << "Paralelo asignado con éxito: " << sigla << "-" << numero << "\n";
            return;
        }
        std::cout << "Error: Escoja una opción válida.\n";
    }
}

// ⫘⫘⫘ Opción 4 — Eliminar paralelo ⫘⫘⫘
void opcionEliminarParalelo(EstadoApp& estado) {
    if (estado.registro.vacio()) {
        std::cout << "No hay asignaturas registradas.\n";
        return;
    }
    std::cout << "Asignaturas disponibles:\n";
    listarAsignaturasConConteo(estado.registro);

    std::string sigla = aMayusculas(trim(leerLinea("Ingrese la sigla de la asignatura:")));
    Asignatura* asignatura = estado.registro.buscar(sigla);
    if (!asignatura) {
        std::cout << "Error: La asignatura no existe en el registro.\n";
        return;
    }
    if (asignatura->paralelos().empty()) {
        std::cout << "Esta asignatura no tiene paralelos.\n";
        return;
    }

    std::cout << "Paralelos disponibles:\n";
    for (const auto& [numero, paralelo] : asignatura->paralelos()) {
        std::cout << "- " << sigla << "-" << numero << " ";
        imprimirSesionesParalelo(paralelo);
        std::cout << "\n";
    }

    int numero = 0;
    while (true) {
        std::string entrada = leerLinea("Ingrese el número del paralelo a eliminar (ejemplo: 2):");
        if (parsearEntero(entrada, numero) && asignatura->tieneParalelo(numero)) break;
        std::cout << "Error: Ingrese un número válido.\n";
    }

    asignatura->eliminarParalelo(numero);
    invalidarSoluciones(estado);
    std::cout << "Paralelo " << numero << " eliminado con éxito.\n";
}

// ⫘⫘⫘ Opción 5 — Resolver horario (CSP por fuerza bruta) ⫘⫘⫘
void opcionResolver(EstadoApp& estado) {
    invalidarSoluciones(estado);

    if (estado.registro.vacio()) {
        // Ojo: NO se marca como "resuelto". Un registro vacío no es lo mismo
        // que un registro sin solución posible.
        std::cout << "No hay asignaturas registradas. No se puede resolver.\n";
        return;
    }

    std::vector<std::string> excluidas;
    Solver solver;
    estado.soluciones = solver.resolver(estado.registro, excluidas);
    estado.haResuelto = true;

    for (const auto& sigla : excluidas) {
        std::cout << "Aviso: la asignatura " << sigla
                  << " no tiene paralelos y fue excluida.\n";
    }

    if (estado.soluciones.empty()) {
        // Política "todo o nada": no se ofrecen asignaciones parciales.
        std::cout << "No se encontraron soluciones que cumplan las restricciones.\n";
    } else {
        std::cout << "Se encontraron " << estado.soluciones.size()
                  << " soluciones válidas.\n";
    }
}

// ⫘⫘⫘ Impresión de los puntajes de una solución ⫘⫘⫘
void imprimirScores(const Solucion& solucion) {
    std::cout << "[ventanas=" << solucion.scoreVentanas
              << ", horario=" << std::fixed << std::setprecision(2)
              << solucion.promedioClave() << std::defaultfloat << "]";
}

// ⫘⫘⫘ Opción 6 — Mostrar resultados ⫘⫘⫘
void opcionMostrarResultados(const EstadoApp& estado) {
    if (!estado.haResuelto) {
        std::cout << "Aún no ha resuelto el horario. Use la opción 5 primero.\n";
        return;
    }
    if (estado.soluciones.empty()) {
        std::cout << "No hay solución posible que cumpla con las restricciones\n";
        return;
    }

    const auto& sols = estado.soluciones;
    std::cout << "Se han encontrado un total de " << sols.size() << " soluciones.\n"
              << "Criterios (ambos se minimizan): ventanas = huecos; "
              << "horario = promedio de clave (0 = 1-2 ... 6 = 13-14).\n\n";

    // ⫘ Los dos óptimos, con desempate determinista (ver Solver::mejorPor*) ⫘
    const auto& mejorVentanas = *std::min_element(sols.begin(), sols.end(),
                                                  Solver::mejorPorVentanas);
    const auto& mejorHorario  = *std::min_element(sols.begin(), sols.end(),
                                                  Solver::mejorPorHorario);

    std::cout << "(1) M E J O R   S O L U C I Ó N   V E N T A N A S:  ";
    imprimirScores(mejorVentanas);
    std::cout << "\n";
    dibujarHorario(mejorVentanas);

    std::cout << "\n(2) M E J O R   S O L U C I Ó N   H O R A R I O:  ";
    imprimirScores(mejorHorario);
    std::cout << "\n";
    dibujarHorario(mejorHorario);

    // ⫘ Frente de Pareto: el trade-off explícito entre ambos criterios ⫘
    // Son las soluciones donde no se puede mejorar un criterio sin empeorar
    // el otro. Cualquier solución fuera del frente está dominada: existe otra
    // igual o mejor en ambos, así que no hay razón para elegirla.
    const std::vector<const Solucion*> frente = Solver::frentePareto(sols);
    std::cout << "\n(3) F R E N T E   D E   P A R E T O:  "
              << frente.size() << " soluciones no dominadas\n"
              << "    (no se puede mejorar un criterio sin empeorar el otro)\n";
    for (std::size_t i = 0; i < frente.size(); ++i) {
        std::cout << "    " << (i + 1) << ") ";
        imprimirScores(*frente[i]);
        std::cout << "  " << frente[i]->firma << "\n";
    }

    // ⫘ Listado completo, ordenado y acotado ⫘
    std::vector<const Solucion*> orden;
    orden.reserve(sols.size());
    for (const Solucion& s : sols) orden.push_back(&s);
    std::sort(orden.begin(), orden.end(),
              [](const Solucion* a, const Solucion* b) { return Solver::mejorPorVentanas(*a, *b); });

    const std::size_t aMostrar = std::min(orden.size(), MAX_TABLAS_MOSTRADAS);
    std::cout << "\n(4) Soluciones que cumplen restricciones "
              << "(ordenadas por ventanas, luego horario):\n";
    if (orden.size() > aMostrar) {
        std::cout << "    Mostrando las " << aMostrar << " mejores de "
                  << orden.size() << ".\n";
    }

    for (std::size_t i = 0; i < aMostrar; ++i) {
        std::cout << "\n( " << (i + 1) << " )  ";
        imprimirScores(*orden[i]);
        std::cout << "\n";
        dibujarHorario(*orden[i]);
    }

    if (orden.size() > aMostrar) {
        std::cout << "\n... y " << (orden.size() - aMostrar)
                  << " soluciones más (no mostradas). Agregue restricciones "
                  << "o quite paralelos poco atractivos para acotar la búsqueda.\n";
    }
}

} // namespace

// ⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘ API pública ⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘
int mostrarMenuYLeerOpcion() {
    std::cout << "==== SOLVER DE HORARIO UNIVERSITARIO ====\n"
              << "1. Añadir asignatura\n"
              << "2. Eliminar asignatura\n"
              << "3. Añadir paralelo\n"
              << "4. Eliminar paralelo\n"
              << "5. Resolver horario\n"
              << "6. Mostrar resultados\n"
              << "0. Salir\n";
    std::string linea = leerLinea("Seleccione una opción:");
    int opcion = 0;
    if (!parsearEntero(linea, opcion)) return -1;
    return opcion;
}

bool despachar(int opcion, EstadoApp& estado) {
    switch (opcion) {
        case 1: opcionAnadirAsignatura(estado);   break;
        case 2: opcionEliminarAsignatura(estado); break;
        case 3: opcionAnadirParalelo(estado);     break;
        case 4: opcionEliminarParalelo(estado);   break;
        case 5: opcionResolver(estado);           break;
        case 6: opcionMostrarResultados(estado);  break;
        case 0:
            std::cout << "Saliendo del programa. ¡Hasta pronto!\n";
            return false;
        default:
            std::cout << "Error: Opción no válida.\n";
            break;
    }
    presioneTeclaParaContinuar();
    limpiarPantalla();
    return true;
}
