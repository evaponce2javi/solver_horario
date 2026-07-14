#pragma once

// ⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘
//  Utilidades.h — Portabilidad de consola, lectura/validación de entrada
//                 y helpers de texto UTF-8 (ancho visible, centrado).
// ⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘

#include <string>
#include <cstddef>

// ⫘⫘⫘ Consola ⫘⫘⫘
void configurarConsola();   // en Windows fija la consola en UTF-8 (no-op en otros SO)
void limpiarPantalla();
void presioneTeclaParaContinuar();

// ⫘⫘⫘ Texto ⫘⫘⫘
std::string aMayusculas(std::string texto);          // solo ASCII
std::string trim(const std::string& texto);
std::size_t anchoVisible(const std::string& texto);  // cuenta code points UTF-8
std::string centrar(const std::string& texto, std::size_t ancho);

// ⫘⫘⫘ Entrada ⫘⫘⫘
std::string leerLinea(const std::string& mensaje);   // imprime mensaje + "> "
bool        parsearEntero(const std::string& texto, int& salida);  // enteros NO negativos

// ⫘⫘⫘ Validaciones de dominio ⫘⫘⫘
bool esSiglaValida(const std::string& sigla);        // ^[A-Z]{3}[0-9]{4}$
int  parsearDia(const std::string& texto);           // 0..4 o -1
int  parsearClave(const std::string& texto);         // 0..6 o -1
bool esTokenOnline(const std::string& texto);