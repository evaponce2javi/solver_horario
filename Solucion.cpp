// ⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘
//  Solucion.cpp
// ⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘

#include "Solucion.h"

std::string etiquetaEleccion(const EleccionParalelo& eleccion) {
    return eleccion.sigla + "-" + std::to_string(eleccion.numero);
}

// Las elecciones vienen en orden de asignatura (el std::map del Registro),
// así que concatenarlas ya produce una clave canónica y comparable.
std::string firmaSolucion(const std::vector<EleccionParalelo>& elecciones) {
    std::string firma;
    for (std::size_t i = 0; i < elecciones.size(); ++i) {
        if (i > 0) firma += '|';
        firma += etiquetaEleccion(elecciones[i]);
    }
    return firma;
}

double Solucion::promedioClave() const {
    if (sesionesPresenciales == 0) return 0.0;   // todo online ⇒ nada que madrugar
    return static_cast<double>(sumaClaves) / sesionesPresenciales;
}

// Comparación exacta de promedios (sin división ni flotantes).
int compararHorario(const Solucion& a, const Solucion& b) {
    // Caso borde: sin sesiones presenciales el promedio se define como 0.
    const bool aVacia = (a.sesionesPresenciales == 0);
    const bool bVacia = (b.sesionesPresenciales == 0);
    if (aVacia && bVacia) return 0;
    if (aVacia)           return (b.sumaClaves > 0) ? -1 : 0;
    if (bVacia)           return (a.sumaClaves > 0) ?  1 : 0;

    // sumaA/nA  vs  sumaB/nB   ⟺   sumaA*nB  vs  sumaB*nA   (nA, nB > 0)
    const long long izquierda = static_cast<long long>(a.sumaClaves) * b.sesionesPresenciales;
    const long long derecha   = static_cast<long long>(b.sumaClaves) * a.sesionesPresenciales;

    if (izquierda < derecha) return -1;
    if (izquierda > derecha) return  1;
    return 0;
}
