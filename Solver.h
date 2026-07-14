#pragma once

// ⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘
//  Solver.h — CSP por fuerza bruta (backtracking/DFS) + los dos scores.
//             Variables = asignaturas, dominio = paralelos, restricción
//             dura = no compartir celda (AND de máscaras). La optimización
//             SOLO ordena; nunca poda soluciones válidas.
//
//  Política "todo o nada": si alguna asignatura no puede encajar, NO se
//  devuelve una solución parcial; simplemente no hay solución.
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

    // ⫘⫘⫘ Funciones de optimización (ambas se minimizan) ⫘⫘⫘
    static int  calcularVentanas(const Solucion& solucion);
    static void calcularHorario(Solucion& solucion);   // llena suma/cantidad

    // ⫘⫘⫘ Órdenes totales y deterministas ⫘⫘⫘
    // Desempate lexicográfico: criterio principal → criterio secundario →
    // firma canónica. Así el "mejor" ya NO depende del orden de enumeración.
    static bool mejorPorVentanas(const Solucion& a, const Solucion& b);
    static bool mejorPorHorario (const Solucion& a, const Solucion& b);

    // ⫘⫘⫘ Trade-off entre los dos criterios ⫘⫘⫘
    // a domina a b  ⟺  a es ≤ en ambos criterios y < en al menos uno.
    static bool domina(const Solucion& a, const Solucion& b);

    // Soluciones NO dominadas (frente de Pareto): en ellas no se puede mejorar
    // un criterio sin empeorar el otro. Barrido O(n log n), no O(n²).
    // Los punteros apuntan a 'soluciones'; no sobrevivas al vector original.
    static std::vector<const Solucion*> frentePareto(const std::vector<Solucion>& soluciones);

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
