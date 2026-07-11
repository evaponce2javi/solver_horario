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
bool Paralelo::existeSesion(int dia, int clave, bool online) const {
    for (const Sesion& s : sesiones_) {
        // Todas las marcas online colapsan a la misma clave lógica.
        if (online && s.online) return true;
        if (!online && !s.online && s.dia == dia && s.clave == clave) return true;
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
