// ⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘
//  Utilidades.cpp
// ⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘

#include "Utilidades.h"
#include "Sesion.h"

#include <iostream>
#include <cstdlib>
#include <cctype>
#include <charconv>
#include <system_error>
#include <regex>

// ⫘⫘⫘ Normalización interna (mayúsculas ASCII + acentos → base) ⫘⫘⫘
namespace {

// Convierte a mayúsculas ASCII y colapsa vocales/ñ acentuadas UTF-8 a su
// letra base, para poder comparar "MIÉRCOLES", "MIERCOLES" y "miércoles".
std::string normalizarTexto(const std::string& entrada) {
    std::string salida;
    for (std::size_t i = 0; i < entrada.size(); ++i) {
        unsigned char c = static_cast<unsigned char>(entrada[i]);
        if (c == 0xC3 && i + 1 < entrada.size()) {
            unsigned char n = static_cast<unsigned char>(entrada[i + 1]);
            switch (n) {
                case 0xA1: case 0x81: salida += 'A'; ++i; continue; // á Á
                case 0xA9: case 0x89: salida += 'E'; ++i; continue; // é É
                case 0xAD: case 0x8D: salida += 'I'; ++i; continue; // í Í
                case 0xB3: case 0x93: salida += 'O'; ++i; continue; // ó Ó
                case 0xBA: case 0x9A: salida += 'U'; ++i; continue; // ú Ú
                case 0xB1: case 0x91: salida += 'N'; ++i; continue; // ñ Ñ
                default: break;
            }
        }
        if (c < 128) salida += static_cast<char>(std::toupper(c));
        // Otros bytes multibyte se descartan (irrelevantes para el matching).
    }
    return salida;
}

} // namespace

// ⫘⫘⫘ Consola portable ⫘⫘⫘
void limpiarPantalla() {
    // El 'if' consume el valor de retorno (evita -Wunused-result).
#ifdef _WIN32
    if (std::system("cls") != 0) { /* sin acción */ }
#else
    if (std::system("clear") != 0) { /* sin acción */ }
#endif
}

void presioneTeclaParaContinuar() {
    std::cout << "\nPresione enter para continuar...";
    std::string basura;
    std::getline(std::cin, basura);
}

// ⫘⫘⫘ Texto ⫘⫘⫘
std::string aMayusculas(std::string texto) {
    for (char& c : texto) {
        unsigned char uc = static_cast<unsigned char>(c);
        if (uc < 128) c = static_cast<char>(std::toupper(uc));
    }
    return texto;
}

std::string trim(const std::string& texto) {
    std::size_t ini = texto.find_first_not_of(" \t\r\n");
    if (ini == std::string::npos) return "";
    std::size_t fin = texto.find_last_not_of(" \t\r\n");
    return texto.substr(ini, fin - ini + 1);
}

// Cuenta caracteres visibles: ignora bytes de continuación UTF-8 (10xxxxxx).
std::size_t anchoVisible(const std::string& texto) {
    std::size_t n = 0;
    for (unsigned char c : texto) {
        if ((c & 0xC0) != 0x80) ++n;
    }
    return n;
}

// Centra según ancho VISIBLE (compensa multibyte para no descuadrar la tabla).
std::string centrar(const std::string& texto, std::size_t ancho) {
    std::size_t vis = anchoVisible(texto);
    if (vis >= ancho) return texto;
    std::size_t pad = ancho - vis;
    std::size_t izq = pad / 2;
    std::size_t der = pad - izq;
    return std::string(izq, ' ') + texto + std::string(der, ' ');
}

// ⫘⫘⫘ Entrada ⫘⫘⫘
std::string leerLinea(const std::string& mensaje) {
    if (!mensaje.empty()) std::cout << mensaje << "\n";
    std::cout << "> ";
    std::string linea;
    std::getline(std::cin, linea);
    return linea;
}

// Entero estricto: rechaza "4x", "ICA-1", "abc", vacío.
bool parsearEntero(const std::string& texto, int& salida) {
    std::string s = trim(texto);
    if (s.empty()) return false;
    int valor = 0;
    const char* ini = s.data();
    const char* fin = s.data() + s.size();
    auto res = std::from_chars(ini, fin, valor);
    if (res.ec != std::errc() || res.ptr != fin) return false;
    salida = valor;
    return true;
}

// ⫘⫘⫘ Validaciones de dominio ⫘⫘⫘
bool esSiglaValida(const std::string& sigla) {
    static const std::regex patron("^[A-Z]{3}[0-9]{4}$");
    return std::regex_match(sigla, patron);
}

int parsearDia(const std::string& texto) {
    std::string t = normalizarTexto(texto);
    if (t == "LUNES")     return 0;
    if (t == "MARTES")    return 1;
    if (t == "MIERCOLES") return 2;
    if (t == "JUEVES")    return 3;
    if (t == "VIERNES")   return 4;
    return -1;
}

int parsearClave(const std::string& texto) {
    std::string t = trim(texto);
    for (int i = 0; i < NUM_CLAVES; ++i) {
        if (t == ETIQUETAS_CLAVES[i]) return i;
    }
    return -1;
}

bool esTokenOnline(const std::string& texto) {
    return normalizarTexto(texto) == "ONLINE";
}
