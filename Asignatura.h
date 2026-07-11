#pragma once

// ⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘
//  Asignatura.h — Agrupa paralelos en un std::map<int, Paralelo>
//                 (orden ascendente y unicidad del número gratis).
// ⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘

#include "Paralelo.h"
#include <map>
#include <string>
#include <cstddef>

class Asignatura {
public:
    Asignatura() = default;
    explicit Asignatura(std::string sigla);

    const std::string&              sigla()     const;
    const std::map<int, Paralelo>&  paralelos() const;
    std::map<int, Paralelo>&        paralelos();

    bool        tieneParalelo(int numero) const;
    Paralelo&   obtenerOCrearParalelo(int numero);
    bool        eliminarParalelo(int numero);
    std::size_t cantidadParalelos() const;

private:
    std::string             sigla_;
    std::map<int, Paralelo> paralelos_;
};
