#pragma once

// ⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘
//  Solucion.h — Una asignación completa del CSP: un paralelo por asignatura,
//               con la máscara combinada, los puntajes cacheados y una firma
//               canónica que permite desempates estables.
// ⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘

#include "Sesion.h"
#include <string>
#include <vector>
#include <cstdint>

// Elección de un paralelo concreto (con sus sesiones, para dibujar la tabla).
struct EleccionParalelo {
    std::string         sigla;
    int                 numero = 0;
    std::vector<Sesion> sesiones;
};

// Una solución válida del CSP.
struct Solucion {
    std::vector<EleccionParalelo> elecciones;
    uint64_t                      mascaraCombinada = 0;

    // ⫘ Criterio 1: VENTANAS (se minimiza) ⫘
    int scoreVentanas = 0;

    // ⫘ Criterio 2: HORARIO (se minimiza) ⫘
    // Se guarda como fracción (suma / cantidad) en vez de como suma cruda:
    // el PROMEDIO de clave evita el sesgo que premiaba a los paralelos con
    // MENOS sesiones en lugar de a los que tienen clases más temprano.
    int sumaClaves           = 0;   // numerador
    int sesionesPresenciales = 0;   // denominador (las ONLINE no cuentan)

    // Firma canónica "ICI4150-2|ICI4247-1": desempate estable, independiente
    // del orden en que el backtracking haya enumerado las soluciones.
    std::string firma;

    // Solo para mostrar. Nunca se usa para comparar (ver compararHorario).
    double promedioClave() const;
};

// Etiqueta legible "SIGLA-numero".
std::string etiquetaEleccion(const EleccionParalelo& eleccion);

// Firma canónica de una asignación completa.
std::string firmaSolucion(const std::vector<EleccionParalelo>& elecciones);

// Compara los promedios de clave SIN flotantes: cruza las fracciones
// (sumaA/nA vs sumaB/nB  ⟺  sumaA*nB vs sumaB*nA). Exacto, sin epsilons.
// Retorna <0 si a es mejor (más temprano), 0 si empatan, >0 si b es mejor.
int compararHorario(const Solucion& a, const Solucion& b);
