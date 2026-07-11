// ⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘
//  Asignatura.cpp
// ⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘

#include "Asignatura.h"
#include <utility>

Asignatura::Asignatura(std::string sigla) : sigla_(std::move(sigla)) {}

const std::string& Asignatura::sigla() const { return sigla_; }

const std::map<int, Paralelo>& Asignatura::paralelos() const { return paralelos_; }

std::map<int, Paralelo>& Asignatura::paralelos() { return paralelos_; }

bool Asignatura::tieneParalelo(int numero) const {
    return paralelos_.count(numero) > 0;
}

// Devuelve el paralelo existente o crea uno nuevo con ese número.
Paralelo& Asignatura::obtenerOCrearParalelo(int numero) {
    auto it = paralelos_.find(numero);
    if (it == paralelos_.end()) {
        it = paralelos_.emplace(numero, Paralelo(numero)).first;
    }
    return it->second;
}

bool Asignatura::eliminarParalelo(int numero) {
    return paralelos_.erase(numero) > 0;
}

std::size_t Asignatura::cantidadParalelos() const {
    return paralelos_.size();
}
