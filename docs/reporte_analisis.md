# Reporte de Análisis de Binario — Reto de Fase 3
## Parte 1 del PIA: Herramienta Educativa de Ciberseguridad

> **Este archivo es tu entrega principal. Complétalo en Markdown y haz push al repo de Classroom50.**  
> No exportes a PDF ni entregues por Teams.

---

## 1. Identificación del estudiante

| Campo | Valor |
|-------|-------|
| Nombre | Marcos Adrian Gonzalez Soto |
| Matrícula | 2037908 |
| Fecha de análisis | 02/11/2026 |
| VM / SO / RAM | VirtualBox/Ubuntu/6 GB |
| Binario elegido (opción A/B/C/D) | |
| Justificación de la elección | |

---

## 2. Reconocimiento estático básico

### 2.1 Identificación del binario

| Campo | Valor |
|-------|-------|
| Nombre original | objetivo.elf — nombre del archivo analizado; nombre previo no confirmado |
| SHA-256 | 	a2ed5872c1d7e828ca5e43565211c81b28317fe9a8999b9f390148934c01d69b |
| Tipo (`file`) | ELF de 64 bits, little-endian, ejecutable PIE, enlazado dinámicamente, con información de depuración |
| Arquitectura | x86-64 |
| Stripped | No |
| Secciones ELF presentes | 	39 secciones; destacan .text, .rodata, .data, .bss, .dynsym, .symtab, .strtab y .debug_* |

> Los archivos completos están en `analysis/strings/`.

### 2.2 Strings destacados

Copia aquí los más significativos de `analysis/strings/strings_interesantes.txt`:

```
http://example.com/api/status
/home/carlos/pac-ad2026-a19-nasm-flags-objdump-group-5
```

¿Qué te dicen estos strings sobre el comportamiento del binario?

Por sí solos, estos strings no me permiten determinar el comportamiento del binario, tal vez sea mi falta de experiencia, pero lo mas que podría asumir es que tal vez utiliza alguna api, faltaría hacer un analizáis con alguna herramienta como Ghidra para obtener mas información del binario.

### 2.3 Secciones relevantes

| Sección | Propósito | ¿Inusual o notable? |
|---------|-----------|---------------------|
| .text | Contiene instrucciones ejecutables.| 	Tiene indicadores AX: ocupa memoria y contiene código ejecutable.|
| .data | 	Contiene datos inicializados que admiten escritura.| Tiene indicadores WA: ocupa memoria y admite escritura.|
| .rodata| 	Contiene datos de solo lectura.| 	Tiene A, sin W ni X.|
| .bss| 	Reserva espacio para datos inicializados a cero.| Es NOBITS: su contenido no ocupa bytes dentro del archivo.|
| .debug_info| 	Contiene información de depuración.| Facilita el análisis y coincide con `with debug info` de `file`.|
| .symtab| 	Contiene la tabla de símbolos.| 	Permite consultar nombres como los que vimos con nm.|

---

## 3. Regla YARA

### 3.1 Justificación de los indicadores elegidos

¿Por qué elegiste estos strings/bytes para tu regla? ¿Son únicos del binario o podrían causar falsos positivos?

Se utilizo edu-lab-a19-pacad2026 por su aparición en strings_raw.txt, tal vez podría causar falso positivos, no puedo decir que no, pero es más específico que un saludo común o  http, se combino esta cadena con la firma de ELF porque un texto con el identificador no debe bastar, ni tampoco cualquier archivo ELF

### 3.2 Resultado de la prueba

El archivo `analysis/yara/resultado_yara.txt` contiene la salida completa.
Pega aquí el resultado clave:

```
Practica_Mags_Objetivo binary/objetivo.elf
0x300d:$identificador: edu-lab-a19-pacad2026
0x0:$header_elf: 7F 45 4C 46
```

**¿La regla detectó el binario?** Sí
**Falsos positivos encontrados:** Ninguno

---

## 4. Análisis con Ghidra

Las evidencias se organizan en `analysis/ghidra/`: listado de funciones, pseudocódigo y carpeta `screenshots/`.

### 4.1 Funciones identificadas

El script `src/listar_funciones.py`, ejecutado dentro de Ghidra, enumeró 90 funciones. El listado completo está en `analysis/ghidra/funciones.txt`. Incluye funciones del programa, rutinas de inicialización, funciones de biblioteca y entradas externas; el total no representa 90 funciones desarrolladas por el autor.

| Nombre de función | Dirección | Descripción inferida |
|-------------------|-----------|----------------------|
| main | 0x0010226B | Coordina la salida por consola, conversión del argumento, validación y cálculo. |
| validar | 0x0010224A | Comprueba que el entero esté entre 1 y 99, inclusive. |
| compute | 0x00102209 | Combina multiplicación, suma y tres operaciones XOR para obtener el resultado. |

Las direcciones corresponden al programa cargado en Ghidra; no son offsets del archivo ni direcciones de ejecución verificadas dinámicamente.

### 4.2 Función de interés #1

**Nombre:** `main`  
**Dirección:** `0x0010226B`  
**¿Por qué es interesante?**

Permite reconstruir el flujo principal. Imprime `Hola, mundo!` y el contenido de `MAGIC`. Si existe un primer argumento, lo convierte a entero decimal con `std::stoi`, llama a `validar(n)` y decide entre calcular un resultado o imprimir un mensaje de error. Si no hay argumento, omite la validación y el cálculo. El flujo normal termina con `return 0`.

```c
// Ver también: analysis/ghidra/pseudocodigo_main.c
// Fragmento del pseudocódigo de Ghidra:
bVar1 = validar(n);
if (bVar1) {
  poVar3 = std::operator<<((ostream *)std::cout,"Resultado: ");
  iVar2 = compute(n,n * 2);
  this = (void *)std::ostream::operator<<(poVar3,iVar2);
  std::ostream::operator<<(this,std::endl<>);
}
else {
  poVar3 = std::operator<<((ostream *)std::cout,"Valor fuera de rango (0-100)");
  std::ostream::operator<<(poVar3,std::endl<>);
}
```

La conversión ocurre antes de la validación del rango. El pseudocódigo revisado no muestra un tratamiento explícito en `main` para las excepciones de conversión; las anotaciones de limpieza de objetos de C++ no demuestran por sí mismas que dichas excepciones se gestionen con un mensaje para el usuario.

### 4.3 Función de interés #2

**Nombre:** `validar`  
**Dirección:** `0x0010224A`  
**¿Por qué es interesante?**

Define el rango realmente admitido por el programa. Devuelve falso si `n < 1` o `n > 99`, y verdadero en los demás casos. Existe una discrepancia con el mensaje de `main`: aunque este anuncia `(0-100)`, los valores 0 y 100 se rechazan.

```c
// Ver también: analysis/ghidra/pseudocodigo_fn2.c
if ((n < 1) || (99 < n)) {
  bVar1 = false;
}
else {
  bVar1 = true;
}
return bVar1;
```

### 4.4 Función de interés #3

**Nombre:** `compute`  
**Dirección:** `0x00102209`  
**¿Por qué es interesante?**

Contiene la transformación que produce el resultado mostrado por `main`. Inicializa `x` con `b + a * 7` y realiza tres iteraciones de XOR con `a + i`. La suma tiene prioridad sobre XOR, por lo que `x ^ a + i` se interpreta como `x ^ (a + i)`.

```c
// Ver también: analysis/ghidra/pseudocodigo_fn3.c
x = b + a * 7;
for (i = 0; i < 3; i = i + 1) {
  x = x ^ a + i;
}
return x;
```

Como `main` llama a `compute(n, n * 2)`, el valor inicial es `9 * n`, al que se aplican sucesivamente XOR con `n`, `n + 1` y `n + 2`. Por ejemplo, para `n = 1`, el cálculo inferido es `9 → 8 → 10 → 9`. Este ejemplo se obtiene del pseudocódigo y no constituye una prueba de ejecución. El uso de XOR por sí solo no permite clasificar la función como cifrado.

### 4.5 Flujo de control general

El programa imprime un saludo y el identificador `MAGIC`. Después comprueba si recibió un argumento de línea de comandos. Si no lo recibió, termina normalmente. Si lo recibió, convierte `argv[1]` a entero decimal y verifica el rango mediante `validar`. Para valores del 1 al 99 llama a `compute(n, n * 2)` e imprime el resultado; para valores fuera de ese rango muestra `Valor fuera de rango (0-100)`. El flujo normal devuelve 0 tanto tras el cálculo como tras el rechazo del rango.

En las tres funciones examinadas no se observan verificaciones de contraseña ni llamadas explícitas de comunicación de red. La URL encontrada durante la extracción de cadenas no demuestra por sí sola una conexión. La función `compute` realiza operaciones aritméticas y de bits, sin evidencia suficiente para atribuirle una finalidad criptográfica. Estas conclusiones corresponden al análisis estático de las funciones revisadas; el comportamiento de ejecución debe contrastarse con la fase de análisis dinámico.

Las capturas requeridas deben conservarse en `analysis/ghidra/screenshots/` con los nombres `01_symbol_tree.png`, `02_decompiler_main.png` y `03_decompiler_fn2.png`.

---

## 5. Análisis dinámico

> Los logs completos están en `analysis/strace/`.

### 5.1 Syscalls relevantes (strace)

Copia el fragmento más significativo de `analysis/strace/strace_filtrado.txt`:

```
(pegar aquí)
```

**¿Qué revela el strace sobre el comportamiento real del binario?**

> (Tu análisis)

### 5.2 Llamadas a librerías (ltrace)

Fragmento clave de `analysis/strace/ltrace_salida.log`:

```
(pegar aquí)
```

**¿Qué funciones de librería usa? ¿Hay alguna sorpresiva?**

> (Tu análisis)

### 5.3 Comparación estático vs. dinámico

¿Encontraste en el análisis dinámico algo que el estático no reveló?

| Técnica | Lo que reveló | Lo que NO reveló |
|---------|--------------|-----------------|
| strings (estático) | | |
| Ghidra (estático) | | |
| strace (dinámico) | | |
| ltrace (dinámico) | | |

---

## 6. CAPA + MITRE ATT&CK

> El reporte completo está en `analysis/capa/capa_output.txt` y `reporte_capa.json`.

### 6.1 Capacidades detectadas por CAPA

Fragmento más relevante de `analysis/capa/capa_output.txt`:

```
(pegar aquí)
```

### 6.2 Mapeo a MITRE ATT&CK

| IOC / Capacidad | Tipo | ID Técnica | Nombre Técnica | Táctica | Herramienta |
|-----------------|------|------------|----------------|---------|-------------|
| (hash SHA-256) | Archivo | T1204 | User Execution | Execution | CAPA |
| | | | | | |
| | | | | | |

### 6.3 Análisis de riesgo

¿Qué nivel de riesgo representaría este binario si fuera malware real, según las técnicas ATT&CK encontradas?

> (Tu análisis)

---

## 7. Conclusiones

### 7.1 Resumen del comportamiento del binario

En máximo 200 palabras: ¿qué hace el binario, cómo lo descubriste, y qué técnica fue la más reveladora?

> (Tu conclusión)

### 7.2 Cadena de herramientas usada

Marca con ✅ las que completaste y ❌ las que omitiste (con justificación):

| Herramienta | ¿Usada? | Notas |
|-------------|---------|-------|
| `file` | | |
| `sha256sum` | | |
| `strings` | | |
| `readelf` / `nm` | | |
| YARA | | |
| Ghidra | | |
| `strace` | | |
| `ltrace` | | |
| CAPA + MITRE | | |

### 7.3 Conexión con el PIA (Parte 2)

¿Qué módulos de tu herramienta C++ automatizarían partes de este análisis?

| Análisis manual (este reto) | Módulo del PIA que lo automatiza |
|-----------------------------|----------------------------------|
| `sha256sum` | `modulo_analisis.cpp` → `calcular_hash()` |
| `strings` | `modulo_analisis.cpp` → filtro de strings |
| `file` / `readelf` | `modulo_analisis.cpp` → identificación ELF |
| (agrega más) | |

---

## 8. Reflexión

1. **¿Qué herramienta fue más útil para entender el binario y por qué?**  
   >

2. **¿Qué encontraste en el análisis dinámico que el estático no mostraba?**  
   >

3. **¿Cómo usarías CAPA en un pipeline automatizado de análisis de malware?**  
   >

4. **Si este binario fuera malware, ¿qué técnica anti-análisis hubiera complicado más tu trabajo?**  
   >

---

*Programación para Ciberseguridad — FCFM-UANL — AD2026*
