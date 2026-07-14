# Solver de Horario Universitario (CSP en C++17)

Programa de consola que arma horarios universitarios modelados como un
**problema de satisfacción de restricciones (CSP)**, los resuelve por
**fuerza bruta completa** (backtracking con poda por restricción dura) y
ordena las soluciones según dos criterios de optimización independientes.

## Modelo

- **Variables**: las asignaturas.
- **Dominio** de cada asignatura: sus paralelos.
- **Asignación**: exactamente un paralelo por asignatura.
- **Restricción dura**: dos paralelos elegidos no pueden compartir ninguna
  celda `(día, clave)`. Se comprueba con un AND de máscaras `uint64_t`
  (cada sesión presencial prende el bit `dia * 7 + clave`).
- La única poda permitida es por violación de la restricción dura: se guardan
  **todas** las combinaciones válidas. La optimización solo **ordena**, nunca
  filtra.

### Criterios de optimización (ambos se minimizan, por separado)

1. **Ventanas** — por día, la corrida vacía más larga que tiene clase antes y
   después; se suma esa ventana máxima de los 5 días.
2. **Horario** — **promedio** de los índices de clave (0–6) de las sesiones
   presenciales. Menor promedio = clases más de mañana. Las online no cuentan.
   Se usa el promedio y no la suma porque la suma premiaba a los paralelos con
   *menos* sesiones en vez de a los que tienen clases *más temprano*: con suma,
   un paralelo de una sola sesión en la clave `11-12` (5) le ganaba a uno con
   dos sesiones en `1-2` y `13-14` (0 + 6 = 6). Con promedio, 5.0 vs 3.0, gana
   el correcto. La comparación es **exacta con enteros** (multiplicación
   cruzada `sumaA·nB` vs `sumaB·nA`), sin flotantes ni epsilons.

### Desempates y trade-off

- **Orden total determinista**: cada criterio desempata por el *otro* criterio y,
  si aún hay empate, por la **firma canónica** de la solución (`ICI4150-2|ICI4247-1`).
  El "mejor" ya no depende del orden en que el backtracking enumeró.
- **Frente de Pareto**: se listan las soluciones **no dominadas** — aquellas en las
  que no se puede mejorar un criterio sin empeorar el otro. Todo lo que queda fuera
  del frente está dominado por alguna otra solución igual o mejor en *ambos*
  criterios, así que no hay razón para elegirlo. Se calcula por barrido (skyline)
  en O(n log n), no comparando todos contra todos. Deliberadamente **no** se combinan
  los criterios en un score ponderado: eso obligaría a inventar un "tipo de cambio"
  arbitrario entre horas muertas y clases temprano.
- **Todo o nada**: si no existe una asignación completa (un paralelo por cada
  asignatura), no hay solución. No se devuelven horarios parciales.

### Días y claves

| Día | Índice | | Clave | Índice |
|-----|--------|-|-------|--------|
| LUNES | 0 | | 1-2 | 0 |
| MARTES | 1 | | 3-4 | 1 |
| MIÉRCOLES | 2 | | 5-6 | 2 |
| JUEVES | 3 | | 7-8 | 3 |
| VIERNES | 4 | | 9-10 | 4 |
| | | | 11-12 | 5 |
| | | | 13-14 | 6 |

`ONLINE` es una clave especial: no ocupa ninguna celda del grid y jamás choca.
Un paralelo puede tener **varias** sesiones ONLINE (la unicidad `(día, clave)`
solo rige para las presenciales).

## Estructura del proyecto

```
Sesion.h                     Constantes del dominio + struct Sesion
Paralelo.h / .cpp            Paralelo (multi-sesión) + máscara cacheada
Asignatura.h / .cpp          Asignatura con std::map<int, Paralelo>
Registro.h / .cpp            std::map<std::string, Asignatura> + CRUD
Solucion.h / .cpp            struct Solucion
Solver.h / .cpp              Backtracking del CSP + los dos scores
Tabla.h / .cpp               Render de la tabla (alineación UTF-8)
Utilidades.h / .cpp          Consola, validaciones, parseo de día/clave
Menu.h / .cpp                Despliegue y despacho del menú
main.cpp                     Punto de entrada / loop del menú
Makefile                     Compilación con un comando
CMakeLists.txt               Alternativa a Make
```

## Compilación y ejecución

Requisitos: un compilador con soporte C++17 (g++ ≥ 7, clang ≥ 5, o MSVC).
El programa compila **sin warnings** con `-std=c++17 -Wall -Wextra`.

### Opción A — Make (Linux / macOS / MSYS2 en Windows)

```bash
make          # compila ./horario
./horario     # ejecuta
# o directamente:
make run
make clean    # limpia binario, objetos y dependencias
```

### Opción B — CMake (multiplataforma)

```bash
mkdir build && cd build
cmake ..
cmake --build .
./horario          # en Windows: .\Release\horario.exe o .\horario.exe
```

### Compilación manual (sin build system)

```bash
g++ -std=c++17 -Wall -Wextra -O2 *.cpp -o horario
```

## Uso en VS Code

1. Instala la extensión **C/C++** (Microsoft) y, opcionalmente, **CMake Tools**.
2. Abre la carpeta del proyecto (`File → Open Folder`).
3. Compila desde la terminal integrada (`` Ctrl+` ``):
   - con Make: `make run`, o
   - con CMake Tools: `Ctrl+Shift+P → CMake: Build` y luego ejecuta el binario.
4. En Windows, ejecuta en una terminal con soporte UTF-8 (Windows Terminal,
   o `chcp 65001` en cmd) para que las tildes de `MIÉRCOLES` se vean bien.

## Menú

```
1. Añadir asignatura      (valida ^[A-Z]{3}[0-9]{4}$, ej. ICI4247)
2. Eliminar asignatura    (borra la asignatura y todos sus paralelos)
3. Añadir paralelo        (paralelo multi-sesión; acepta varias ONLINE)
4. Eliminar paralelo
5. Resolver horario       (CSP por fuerza bruta, guarda TODAS las soluciones)
6. Mostrar resultados     (mejor por ventanas, mejor por horario, frente de
                           Pareto, y el listado ordenado — máx. 50 tablas)
0. Salir
```

Cualquier mutación del registro (opciones 1–4) **invalida** las soluciones ya
calculadas, para que la opción 6 nunca muestre horarios con asignaturas o
paralelos que ya fueron eliminados.

## Notas de diseño

- **Máscara `uint64_t`**: 35 celdas caben de sobra en 64 bits; detectar choques
  es un simple `(a & b) != 0`, O(1).
- **`std::map`** para asignaturas y paralelos: da orden (lexicográfico /
  ascendente) gratis para todos los listados y unicidad de la clave.
- **Tope de impresión**: la opción 6 dibuja como máximo **50 tablas** (las mejores
  por ventanas y luego horario). Los criterios y el frente de Pareto se calculan
  siempre sobre *todas* las soluciones; el tope es solo de presentación.
- **Entrada numérica**: `parsearEntero` rechaza negativos y signo explícito — no hay
  ningún dato del dominio (opción de menú, número de paralelo) que pueda ser < 0.
- **Alineación UTF-8**: `std::setw` cuenta bytes, así que la `É` de `MIÉRCOLES`
  descuadraría la tabla. El render calcula el ancho **visible** (ignorando los
  bytes de continuación `10xxxxxx`) y rellena con espacios en consecuencia.

*Licenciado bajo PolyForm Noncommercial 1.0.0 — uso libre no comercial, ver LICENSE.*
