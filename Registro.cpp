// ⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘
//  Registro.cpp — CRUD sobre el mapa de asignaturas.
// ⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘

#include "Registro.h"

const std::map<std::string, Asignatura>& Registro::asignaturas() const {
    return asignaturas_;
}

bool Registro::existeAsignatura(const std::string& sigla) const {
    return asignaturas_.count(sigla) > 0;
}

bool Registro::agregarAsignatura(const std::string& sigla) {
    if (existeAsignatura(sigla)) return false;
    asignaturas_.emplace(sigla, Asignatura(sigla));
    return true;
}

int Registro::eliminarAsignatura(const std::string& sigla) {
    auto it = asignaturas_.find(sigla);
    if (it == asignaturas_.end()) return -1;
    int borrados = static_cast<int>(it->second.cantidadParalelos());
    asignaturas_.erase(it);
    return borrados;
}

Asignatura* Registro::buscar(const std::string& sigla) {
    auto it = asignaturas_.find(sigla);
    return it == asignaturas_.end() ? nullptr : &it->second;
}

const Asignatura* Registro::buscar(const std::string& sigla) const {
    auto it = asignaturas_.find(sigla);
    return it == asignaturas_.end() ? nullptr : &it->second;
}

bool Registro::vacio() const {
    return asignaturas_.empty();
}
