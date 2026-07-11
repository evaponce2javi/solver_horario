// ⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘
//  Tabla.cpp
// ⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘

#include "Tabla.h"
#include "Sesion.h"
#include "Utilidades.h"

#include <array>
#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

namespace {

// Celda enmarcada: "| " + contenido centrado + " ".
std::string celda(const std::string& contenido, std::size_t ancho) {
    return "| " + centrar(contenido, ancho) + " ";
}

// Borde "+----+----+...": una sección de clave + 5 secciones de día.
std::string bordeFila(std::size_t anchoClave, std::size_t anchoDia) {
    std::string borde = "+" + std::string(anchoClave + 2, '-');
    for (int d = 0; d < NUM_DIAS; ++d) {
        borde += "+" + std::string(anchoDia + 2, '-');
    }
    borde += "+";
    return borde;
}

} // namespace

void dibujarHorario(const Solucion& solucion) {
    // ⫘⫘⫘ 1) Construir la grilla [clave][día] ⫘⫘⫘
    std::array<std::array<std::string, NUM_DIAS>, NUM_CLAVES> grilla;
    for (auto& fila : grilla) fila.fill("X");

    std::vector<std::string> online;
    for (const auto& eleccion : solucion.elecciones) {
        bool tieneOnline = false;
        for (const Sesion& s : eleccion.sesiones) {
            if (s.online) tieneOnline = true;
            else          grilla[s.clave][s.dia] = etiquetaEleccion(eleccion);
        }
        if (tieneOnline) online.push_back(etiquetaEleccion(eleccion));
    }

    // ⫘⫘⫘ 2) Calcular anchos (por ancho VISIBLE, no por bytes) ⫘⫘⫘
    std::size_t anchoClave = anchoVisible("CLAVE");
    for (const auto& c : ETIQUETAS_CLAVES) {
        anchoClave = std::max(anchoClave, anchoVisible(c));
    }

    std::size_t anchoDia = 0;
    for (const auto& d : ETIQUETAS_DIAS) {
        anchoDia = std::max(anchoDia, anchoVisible(d));
    }
    for (const auto& fila : grilla) {
        for (const auto& cel : fila) {
            anchoDia = std::max(anchoDia, anchoVisible(cel));
        }
    }

    // ⫘⫘⫘ 3) Render ⫘⫘⫘
    const std::string borde = bordeFila(anchoClave, anchoDia);

    std::cout << borde << "\n";

    std::string encabezado = celda("CLAVE", anchoClave);
    for (int d = 0; d < NUM_DIAS; ++d) {
        encabezado += celda(ETIQUETAS_DIAS[d], anchoDia);
    }
    encabezado += "|";
    std::cout << encabezado << "\n" << borde << "\n";

    for (int c = 0; c < NUM_CLAVES; ++c) {
        std::string fila = celda(ETIQUETAS_CLAVES[c], anchoClave);
        for (int d = 0; d < NUM_DIAS; ++d) {
            fila += celda(grilla[c][d], anchoDia);
        }
        fila += "|";
        std::cout << fila << "\n";
    }
    std::cout << borde << "\n";

    // ⫘⫘⫘ 4) Pie con sesiones online (no ocupan grid) ⫘⫘⫘
    if (!online.empty()) {
        std::cout << "Sesiones online: ";
        for (std::size_t i = 0; i < online.size(); ++i) {
            std::cout << online[i] << (i + 1 < online.size() ? ", " : "");
        }
        std::cout << "\n";
    }
}
