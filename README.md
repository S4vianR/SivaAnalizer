# SivaAnalizer

SivaAnalizer es un analizador léxico para el lenguaje SIVA, escrito en C. Extrae tokens de archivos fuente `.siv` y los imprime en formato JSON, facilitando el análisis y procesamiento posterior.

## Características
- Reconoce palabras reservadas, operadores, delimitadores y comentarios según la gramática SIVA.
- Soporta tipos de datos: int, float, str, bool, undefined, num, array.
- Imprime los tokens con información de tipo, lexema, literal, línea y columna.
- Fácil de extender para análisis sintáctico.

## Estructura del proyecto
```
main.c           # Analizador léxico principal
syntax.bnf       # Gramática BNF del lenguaje SIVA
scripts/
  calculadora/   # Ejemplo: calculadora.siv
  helloWorld/    # Ejemplo: helloWorld.siv
```

## Uso
Compila el programa:

```bash
gcc main.c -o main
```

Ejecuta el analizador sobre un archivo `.siv`:

```bash
./main scripts/calculadora/calculadora.siv
```

El resultado será un listado de tokens en formato JSON.

## Gramática y tokens
- Consulta `syntax.bnf` para la gramática formal.
- Los tokens reconocidos están definidos en `main.c`.

## Ejemplo de salida
```json
[
  {
    "tipo": "VAR",
    "lexema": "var",
    "linea": 1,
    "columna": 1
  },
  {
    "tipo": "IDENTIFIER",
    "lexema": "x",
    "linea": 1,
    "columna": 5
  },
  ...
]
```

## Extensión
Puedes modificar la gramática en `syntax.bnf` y los tokens en `main.c` para adaptar el analizador a nuevas características del lenguaje.

## Autor
- Reuben

## Licencia
MIT
