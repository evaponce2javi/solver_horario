#pragma once

// ⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘
//  Paralelo.h — Un paralelo pertenece a una asignatura, puede tener varias
//               sesiones y cachea su máscara de ocupación (uint64_t).
// ⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘

#include "Sesion.h"
#include <vector>
#include <cstdint>

class Paralelo {
public:
    Paralelo() = default;
    explicit Paralelo(int numero);

    int                         numero()   const;
    const std::vector<Sesion>&  sesiones() const;
    uint64_t                    mascara()  const;   // máscara cacheada

    // Agrega una sesión. Retorna false si ya existía (no duplica).
    bool agregarSesion(const Sesion& sesion);

    // Unicidad = (día, clave), y SOLO para sesiones presenciales.
    // Las sesiones ONLINE no ocupan grid: se admiten varias por paralelo.
    bool existeSesion(int dia, int clave, bool online) const;

private:
    void recalcularMascara();

    int                 numero_ = 0;
    std::vector<Sesion> sesiones_;
    uint64_t            mascara_ = 0;
};
