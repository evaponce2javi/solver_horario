#pragma once

// ⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘
//  Menu.h — Estado de la aplicación + despliegue y despacho del menú.
// ⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘

#include "Registro.h"
#include "Solucion.h"
#include <vector>

// Estado compartido durante toda la sesión.
struct EstadoApp {
    Registro              registro;
    std::vector<Solucion> soluciones;
    bool                  haResuelto = false;
};

// Muestra el menú y lee la opción (retorna -1 si no es un entero válido).
int mostrarMenuYLeerOpcion();

// Ejecuta la opción. Retorna false SOLO cuando corresponde salir (opción 0).
bool despachar(int opcion, EstadoApp& estado);
