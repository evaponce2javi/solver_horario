// ⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘
//  Paralelo.cpp
// ⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘

#include "Paralelo.h"

// ⫘⫘⫘ Construcción y accesores ⫘⫘⫘
Paralelo::Paralelo(int numero) : numero_(numero) {}

int Paralelo::numero() const { return numero_; }

const std::vector<Sesion>& Paralelo::sesiones() const { return sesiones_; }

uint64_t Paralelo::mascara() const { return mascara_; }

// ⫘⫘⫘ Unicidad de sesión ⫘⫘⫘
// Solo las presenciales tienen unicidad: dos clases no pueden ocupar la misma
// celda (día, clave). Las ONLINE no ocupan grid y jamás chocan, así que un
// paralelo puede acumular tantas marcas ONLINE como sesiones remotas tenga.
bool Paralelo::existeSesion(int dia, int clave, bool online) const {
    if (online) return false;   // múltiples marcas online permitidas
    for (const Sesion& s : sesiones_) {
        if (!s.online && s.dia == dia && s.clave == clave) return true;
    }
    return false;
}

// ⫘⫘⫘ Alta de sesión (sin duplicados) ⫘⫘⫘
bool Paralelo::agregarSesion(const Sesion& sesion) {
    if (existeSesion(sesion.dia, sesion.clave, sesion.online)) return false;
    sesiones_.push_back(sesion);
    if (!sesion.online) recalcularMascara();
    return true;
}

// ⫘⫘⫘ Máscara: prende un bit por sesión presencial ⫘⫘⫘
void Paralelo::recalcularMascara() {
    mascara_ = 0;
    for (const Sesion& s : sesiones_) {
        if (!s.online) {
            mascara_ |= (static_cast<uint64_t>(1) << indiceBit(s.dia, s.clave));
        }
    }
}
