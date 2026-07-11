#pragma once

// ⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘
//  Registro.h — std::map<sigla, Asignatura>: orden lexicográfico gratuito
//               para los listados y búsqueda O(log n) por sigla.
// ⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘

#include "Asignatura.h"
#include <map>
#include <string>

class Registro {
public:
    const std::map<std::string, Asignatura>& asignaturas() const;

    bool existeAsignatura(const std::string& sigla) const;

    // Retorna false si la sigla ya existía.
    bool agregarAsignatura(const std::string& sigla);

    // Retorna el número de paralelos borrados, o -1 si no existía.
    int  eliminarAsignatura(const std::string& sigla);

    Asignatura*       buscar(const std::string& sigla);
    const Asignatura* buscar(const std::string& sigla) const;

    bool vacio() const;

private:
    std::map<std::string, Asignatura> asignaturas_;
};
