#pragma once

// ⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘
//  Solucion.h — Una asignación completa del CSP: un paralelo por asignatura,
//               con la máscara combinada y los puntajes cacheados.
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
    int                           scoreVentanas    = 0;
    int                           scoreHorario     = 0;
};

// Etiqueta legible "SIGLA-numero".
std::string etiquetaEleccion(const EleccionParalelo& eleccion);
