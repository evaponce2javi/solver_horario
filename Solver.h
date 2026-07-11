#pragma once

// ⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘
//  Solver.h — CSP por fuerza bruta (backtracking/DFS) + los dos scores.
//             Variables = asignaturas, dominio = paralelos, restricción
//             dura = no compartir celda (AND de máscaras). La optimización
//             SOLO ordena; nunca poda soluciones válidas.
// ⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘

#include "Registro.h"
#include "Solucion.h"
#include <vector>
#include <string>
#include <cstddef>
#include <cstdint>

class Solver {
public:
    // Explora TODAS las combinaciones válidas. Las asignaturas sin paralelos
    // se acumulan en 'excluidas' y se omiten del producto.
    std::vector<Solucion> resolver(const Registro& registro,
                                   std::vector<std::string>& excluidas);

    // Funciones de optimización (se minimizan, se evalúan por separado).
    static int calcularVentanas(const Solucion& solucion);
    static int calcularHorario(const Solucion& solucion);

private:
    struct Variable {
        std::string                   sigla;
        std::vector<const Paralelo*>  dominio;
    };

    void backtrack(const std::vector<Variable>& variables,
                   std::size_t                  indice,
                   std::vector<EleccionParalelo>& parcial,
                   uint64_t                     mascaraAcum,
                   std::vector<Solucion>&       soluciones);
};
