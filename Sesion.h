#pragma once

// ⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘
//  Sesion.h — Núcleo del dominio: constantes de días/claves y la unidad
//             mínima de ocupación (Sesion).
// ⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘

#include <array>
#include <string>
#include <cstdint>

// ⫘⫘⫘ Constantes del grid (sin números mágicos) ⫘⫘⫘
constexpr int NUM_DIAS   = 5;                       // LUNES..VIERNES
constexpr int NUM_CLAVES = 7;                       // 1-2 .. 13-14
constexpr int NUM_CELDAS = NUM_DIAS * NUM_CLAVES;   // 35 celdas del grid

// Centinelas para sesiones ONLINE (día/clave irrelevantes).
constexpr int DIA_ONLINE   = -1;
constexpr int CLAVE_ONLINE = -1;

// ⫘⫘⫘ Etiquetas constantes (variables inline C++17) ⫘⫘⫘
inline const std::array<std::string, NUM_DIAS> ETIQUETAS_DIAS = {
    "LUNES", "MARTES", "MIÉRCOLES", "JUEVES", "VIERNES"
};

inline const std::array<std::string, NUM_CLAVES> ETIQUETAS_CLAVES = {
    "1-2", "3-4", "5-6", "7-8", "9-10", "11-12", "13-14"
};

// ⫘⫘⫘ Sesión: una celda (día, clave) o una marca online ⫘⫘⫘
struct Sesion {
    int  dia;     // 0..4  (o DIA_ONLINE si online)
    int  clave;   // 0..6  (o CLAVE_ONLINE si online)
    bool online;  // true => no ocupa grid, nunca choca
};

// Índice de bit en la máscara de 35 celdas: dia * 7 + clave.
inline int indiceBit(int dia, int clave) {
    return dia * NUM_CLAVES + clave;
}
