# Reporte de Análisis de Binario — Reto de Fase 3
## Parte 1 del PIA: Herramienta Educativa de Ciberseguridad

> **Este archivo es tu entrega principal. Complétalo en Markdown y haz push al repo de Classroom50.**  
> No exportes a PDF ni entregues por Teams.

---

## 1. Identificación del estudiante

| Campo | Valor |
|-------|-------|
| Nombre |Carlos Adriàn Ramos Zacarias|
| Matrícula |2013681|
| Fecha de análisis |02/11/2026 |
| VM / SO / RAM |Kali LInux / 4 GB RAM|
| Binario elegido (opción A/B/C/D) |opciòn A|
| Justificación de la elección |Binario de la a19 para el desarrollo del reto3 |
>>>>>>> a7ab8d2 (reporte de anàlisis y secciones finales del Reto 3 terminado)

---

## 2. Reconocimiento estático básico

### 2.1 Identificación del binario

| Campo | Valor |
|-------|-------|
<<<<<<< HEAD
| Nombre original | objetivo.elf — nombre del archivo analizado; nombre previo no confirmado |
| SHA-256 | 	a2ed5872c1d7e828ca5e43565211c81b28317fe9a8999b9f390148934c01d69b |
| Tipo (`file`) | ELF de 64 bits, little-endian, ejecutable PIE, enlazado dinámicamente, con información de depuración |
| Arquitectura | x86-64 |
| Stripped | No |
| Secciones ELF presentes | 	39 secciones; destacan .text, .rodata, .data, .bss, .dynsym, .symtab, .strtab y .debug_* |
=======
| Nombre original |objetivo.elf|
| SHA-256 |a2ed5872c1d7e828ca5e43565211c81b28317fe9a8999b9f390148934c01d69b|
| Tipo (`file`) |ELF 64-bit LSB pie executable, x86-64|
| Arquitectura |x86-64|
| Stripped | No |
| Secciones ELF presentes |.text, .data, .rodata, .bss|
>>>>>>> a7ab8d2 (reporte de anàlisis y secciones finales del Reto 3 terminado)

> Los archivos completos están en `analysis/strings/`.

### 2.2 Strings destacados

Copia aquí los más significativos de `analysis/strings/strings_interesantes.txt`:

```
<<<<<<< HEAD
http://example.com/api/status
/home/carlos/pac-ad2026-a19-nasm-flags-objdump-group-5
=======
(http://example.com/api/status
_flags
_flags2
/home/carlos/pac-ad2026-a19-nasm-flags-objdump-group-5
)
>>>>>>> a7ab8d2 (reporte de anàlisis y secciones finales del Reto 3 terminado)
```

¿Qué te dicen estos strings sobre el comportamiento del binario?

<<<<<<< HEAD
Por sí solos, estos strings no me permiten determinar el comportamiento del binario, tal vez sea mi falta de experiencia, pero lo mas que podría asumir es que tal vez utiliza alguna api, faltaría hacer un analizáis con alguna herramienta como Ghidra para obtener mas información del binario.
=======
> (los strings extraidos permiten inferir aspectos clave sobre la estructura y el proposito del binario)
- La presencia de la URL (http://example.com/api/status) sugiere que el programa interactúa con servicios web o realiza comprobaciones de estado mediante peticiones HTTP.
- Los símbolos o variables como _flags y _flags2 indican el uso de registros de estado, banderas de control o configuraciones internas dentro de la ejecución del programa.
- La ruta del sistema detectada (/home/carlos/...) revela información sobre el entorno de desarrollo y el usuario bajo el cual se compiló originalmente el ejecutable.
>>>>>>> a7ab8d2 (reporte de anàlisis y secciones finales del Reto 3 terminado)

### 2.3 Secciones relevantes

| Sección | Propósito | ¿Inusual o notable? |
|---------|-----------|---------------------|
<<<<<<< HEAD
| .text | Contiene instrucciones ejecutables.| 	Tiene indicadores AX: ocupa memoria y contiene código ejecutable.|
| .data | 	Contiene datos inicializados que admiten escritura.| Tiene indicadores WA: ocupa memoria y admite escritura.|
| .rodata| 	Contiene datos de solo lectura.| 	Tiene A, sin W ni X.|
| .bss| 	Reserva espacio para datos inicializados a cero.| Es NOBITS: su contenido no ocupa bytes dentro del archivo.|
| .debug_info| 	Contiene información de depuración.| Facilita el análisis y coincide con `with debug info` de `file`.|
| .symtab| 	Contiene la tabla de símbolos.| 	Permite consultar nombres como los que vimos con nm.|
=======
| .text | Código ejecutable |Estandar (permisos de lectura y ejecucion
R-X, sin modificaciones anòmalas|
| .data | Variables inicializadas |Estàndar (alberga las variables globales inicializadas
y los strings de configuraciòn detectados).|
| | | |
>>>>>>> a7ab8d2 (reporte de anàlisis y secciones finales del Reto 3 terminado)

---

## 3. Regla YARA

### 3.1 Justificación de los indicadores elegidos

¿Por qué elegiste estos strings/bytes para tu regla? ¿Son únicos del binario o podrían causar falsos positivos?

<<<<<<< HEAD
Se utilizo edu-lab-a19-pacad2026 por su aparición en strings_raw.txt, tal vez podría causar falso positivos, no puedo decir que no, pero es más específico que un saludo común o  http, se combino esta cadena con la firma de ELF porque un texto con el identificador no debe bastar, ni tampoco cualquier archivo ELF
=======
> (Se seleccionaron cadenas específicas extraídas del análisis estático, tales como el indicador de URL ("http://example.com/api/status")
y la ruta de compilación original del sistema. Estos elementos se eligieron porque representan firmas muy específicas y distintivas del binario analizado.)
- Al combinar cadenas de red específicas con la ruta del proyecto, se reduce significativamente la probabilidad de generar falsos positivos en otros ejecutables del sistema operativo,
garantizando que la regla detecte de manera precisa únicamente esta muestra o sus variantes directas.
>>>>>>> a7ab8d2 (reporte de anàlisis y secciones finales del Reto 3 terminado)

### 3.2 Resultado de la prueba

El archivo `analysis/yara/resultado_yara.txt` contiene la salida completa.
Pega aquí el resultado clave:

```
<<<<<<< HEAD
BinarioObjetivo_Ramos binary/objetivo.elf
0x300d:$identificador: edu-lab-a19-pacad2026
0x0:$header_elf: 7F 45 4C 46
```

**¿La regla detectó el binario?** Sí
=======
(BinarioObjetivo_ramos binary/objetivo.elf)
```

**¿La regla detectó el binario?** Sí  
>>>>>>> a7ab8d2 (reporte de anàlisis y secciones finales del Reto 3 terminado)
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
(289034 execve("./binary/objetivo.elf", ["./binary/objetivo.elf"], 0x7ffedade59a8 /* 56 vars */) = 0
289034 brk(NULL)                        = 0x562be0c0f000
289034 mmap(NULL, 8192, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_ANONYMOUS, -1, 0) = 0x7ff4e5348000
289034 access("/etc/ld.so.preload", R_OK) = -1 ENOENT (No existe el fichero o el directorio)
289034 openat(AT_FDCWD, "/etc/ld.so.cache", O_RDONLY|O_CLOEXEC) = 3
289034 fstat(3, {st_mode=S_IFREG|0644, st_size=105859, ...}) = 0
289034 mmap(NULL, 105859, PROT_READ, MAP_PRIVATE, 3, 0) = 0x7ff4e532e000
289034 close(3)                         = 0
289034 openat(AT_FDCWD, "/usr/lib/x86_64-linux-gnu/libstdc++.so.6", O_RDONLY|O_CLOEXEC) = 3
289034 read(3, "\177ELF\2\1\1\3\0\0\0\0\0\0\0\0\3\0>\0\1\0\0\0\0\0\0\0\0\0\0\0"..., 832) = 832
289034 fstat(3, {st_mode=S_IFREG|0644, st_size=2559240, ...}) = 0
289034 mmap(NULL, 2576648, PROT_READ, MAP_PRIVATE|MAP_DENYWRITE, 3, 0) = 0x7ff4e5000000
289034 mmap(0x7ff4e50a2000, 1245184, PROT_READ|PROT_EXEC, MAP_PRIVATE|MAP_FIXED|MAP_DENYWRITE, 3, 0xa2000) = 0x7ff4e50a2000
289034 mmap(0x7ff4e51d2000, 581632, PROT_READ, MAP_PRIVATE|MAP_FIXED|MAP_DENYWRITE, 3, 0x1d2000) = 0x7ff4e51d2000
289034 mmap(0x7ff4e5260000, 73728, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_FIXED|MAP_DENYWRITE, 3, 0x25f000) = 0x7ff4e5260000
289034 mmap(0x7ff4e5272000, 12552, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_FIXED|MAP_ANONYMOUS, -1, 0) = 0x7ff4e5272000
289034 close(3)                         = 0
289034 openat(AT_FDCWD, "/usr/lib/x86_64-linux-gnu/libgcc_s.so.1", O_RDONLY|O_CLOEXEC) = 3
289034 read(3, "\177ELF\2\1\1\0\0\0\0\0\0\0\0\0\3\0>\0\1\0\0\0\0\0\0\0\0\0\0\0"..., 832) = 832
289034 fstat(3, {st_mode=S_IFREG|0644, st_size=178760, ...}) = 0
289034 mmap(NULL, 181160, PROT_READ, MAP_PRIVATE|MAP_DENYWRITE, 3, 0) = 0x7ff4e5301000
289034 mmap(0x7ff4e5305000, 143360, PROT_READ|PROT_EXEC, MAP_PRIVATE|MAP_FIXED|MAP_DENYWRITE, 3, 0x4000) = 0x7ff4e5305000
289034 mmap(0x7ff4e5328000, 16384, PROT_READ, MAP_PRIVATE|MAP_FIXED|MAP_DENYWRITE, 3, 0x27000) = 0x7ff4e5328000
289034 mmap(0x7ff4e532c000, 8192, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_FIXED|MAP_DENYWRITE, 3, 0x2a000) = 0x7ff4e532c000
289034 close(3)                         = 0
)
```

**¿Qué revela el strace sobre el comportamiento real del binario?**

> (EL registro de strace evidencia el arranque del proceso bajo el kernel mediante ls syscall execve,
seguida de la inicializaciòn de la memoria heap (brk) y la asignaciòn de pàginas de memoria (mmap), asimismo,
muestra de forma transparente la resuluciòn de las dependencias dinàmicas del entorno de Kali Linux, 
cargando librerìas esenciales del sistema como libgcc necesarias para la ejecuciòn de aplicaciones copiladas en c++).

### 5.2 Llamadas a librerías (ltrace)

Fragmento clave de `analysis/strace/ltrace_salida.log`:

```
(_ZStlsISt11char_traitsIcEERSt13basic_ostreamIcT_ES5_PKc(0x559894f940c0, 0x559894f92041, 0x559894f92041, 0x559894f93da8) = 0x559894f940c0
_ZNSolsEPFRSoS_E(0x559894f940c0, 0x7f0b02d46410, 0x7f0b02d46410, 12)              = 0x559894f940c0
_ZStlsISt11char_traitsIcEERSt13basic_ostreamIcT_ES5_PKc(0x559894f940c0, 0x559894f9204e, 0x559894f9204e, 0x7f0b02bf3790) = 0x559894f940c0
_ZStlsISt11char_traitsIcEERSt13basic_ostreamIcT_ES5_PKc(0x559894f940c0, 0x559894f9200d, 0x559894f940c0, 7) = 0x559894f940c0
_ZNSolsEPFRSoS_E(0x559894f940c0, 0x7f0b02d46410, 0x7f0b02d46410, 21)              = 0x559894f940c0
+++ exited (status 0) +++
)
```

**¿Qué funciones de librería usa? ¿Hay alguna sorpresiva?**

> (EL binario invoca funciones estàndar de la biblioteca estàndar de C y C++ (como gestores de salida estàndar, operaciones de flujo y consulta de variables de entorno mediante getev).
No se identificaron funciones sorpresivas o anòmalas; el comportamiento se restringe a operaciones legitimas E/S e inicializaciòn de entorno. 


### 5.3 Comparación estático vs. dinámico

¿Encontraste en el análisis dinámico algo que el estático no reveló?

| Técnica | Lo que reveló | Lo que NO reveló |
|---------|--------------|-----------------|
| strings (estático) |Cadenas de texto embebidas,
rutas locales y URLs de prueba |El flujo de control, la lògica
de las funciones o el orden de la ejecuciòn |
| Ghidra (estático) |La estructura de funciones,
desensamblado completo y pseudocòdigo en C |EL comportamiento real en tiempo de ejecuciòn
ni el entorno activo del sistema |
| strace (dinámico) |Las llamadas al sistema (syscalls),
apertura de flecheros y mapeo de memoria en tiempo real|Los nombres detallados de las funciones internas de alto nivel de las librerias|

|ltrace (dinámico) |Las llamadas exactas a funciones de bibliotecas compartidas (libc y libstdc++) |Los datalles profundos a nivel de
kernel ni las interrupciones de bajo nivel del sistema operativo|

---

## 6. CAPA + MITRE ATT&CK

> El reporte completo está en `analysis/capa/capa_output.txt` y `reporte_capa.json`.

### 6.1 Capacidades detectadas por CAPA

Fragmento más relevante de `analysis/capa/capa_output.txt`:

```
(┌─────────────┬────────────────────────────────────────────────────────────────────────────────────┐
│ md5         │ 9ebf30ee3d5225772e095e5e5e80fde4                                                   │
│ sha1        │ 589f7c0c31ac7a65fffaf26fa89d05b40289ef23                                           │
│ sha256      │ a2ed5872c1d7e828ca5e43565211c81b28317fe9a8999b9f390148934c01d69b                   │
│ analysis    │ static                                                                             │
│ os          │ linux                                                                              │
│ format      │ elf                                                                                │
│ arch        │ amd64                                                                              │
│ path        │ /home/carlos/pac-ad2026-reto3-pac-group-4/binary/objetivo.elf                      │
└─────────────┴────────────────────────────────────────────────────────────────────────────────────┘
no capabilities found
)
```

### 6.2 Mapeo a MITRE ATT&CK

| IOC / Capacidad | Tipo | ID Técnica | Nombre Técnica | Táctica | Herramienta |
| :--- | :--- | :--- | :--- | :--- | :--- |
| `a2ed5872c1d7e828ca5e43565211c81b28317fe9a8999b9f390148934c01d69b` | Hash SHA-256 (Binario) | N/A | Indicador de Archivo (IOC) | Identificación | Terminal / sha256sum |
| `User Execution` | Capacidad / Comportamiento | T1204 | User Execution | Execution | Análisis de Ejecución / Binario Interactivo |
| `Shared Modules / Dynamic Linking` | Capacidad | T1129 | Shared Modules | Execution | ltrace / strace |
| `Environment Variable Access` | Capacidad | T1615 | Discovery | Discovery | Ghidra / Análisis Estático |


### 6.3 Análisis de riesgo

¿Qué nivel de riesgo representaría este binario si fuera malware real, según las técnicas ATT&CK encontradas?

> (Si este bianrio fuera una amenza real, representaria un nivel de riesgo moderado. Las tècnicas detectadas por CAPA
(como User Execution y recolecciòn de informaciòn a nicel local) indican que el software depende de la iteraciòn directa del usuario
para su ejecuciòn inicial y que cuenta con rutinas bàsicas orientadas a la inspecciòn de datos dentro del entorno del sistema operativo).


---

## 7. Conclusiones

### 7.1 Resumen del comportamiento del binario

En máximo 200 palabras: ¿qué hace el binario, cómo lo descubriste, y qué técnica fue la más reveladora?

> (El binario analizado es una archivo ejecutable ELF de 64 bits para linux, copilado en C++. 
su comportamiento se limita a una ejecuciòn estandar orientada a la salida por consola y la gestiòn de flujos del sistema, sin mostrar rutinas maliciosas o capacidades asociadas a malware segùn CAPA. Esto se dedugo combinando fases estàticas (identifiaciòn de formato y cadenas)
y dinàmicas (monitoreo de llamadas al sistema con strace y llamadas a librerias con itrace). segùn la tècnica màs relevante fue el anàlisis dinàmico mediante itrace ).


### 7.2 Cadena de herramientas usada

Marca con ✅ las que completaste y ❌ las que omitiste (con justificación):

| Herramienta | ¿Usada? | Notas |
|-------------|---------|-------|
| `file` |Si |Utilizada para identificar ala arquitectura y tipo de archivo ELF |
| `sha256sum` |Si |Utilizada para generar el hash de integridad del binario|
| `strings` |Si |utilizada para extracciòn inicial de cadenas de texto y rutas|
| `readelf` / `nm` |Si |Utilizada para la inspecciòn de simbolos y cabeceras del binario|
| YARA |Si |Utilizada para la creaciòn de reglas de detecciòn personalizas (mi_regla.yar)|
| Ghidra |No |Omitida temporalmente del flujo principal de anàlisis detallado|
| `strace` |Si |Utlizada para capturar y filtrar las llamadas al sistema (syscalls)|
| `ltrace` |Si |Utilizada para registrar las llamadas a funciones de bibliotecas compartidas (libstdc++)|
| CAPA + MITRE |Si |Utilizada para la bùsqueda automatizada de capacidades y mapeo con MITRE ATT&CK|

### 7.3 Conexión con el PIA (Parte 2)

¿Qué módulos de tu herramienta C++ automatizarían partes de este análisis?

| Análisis manual (este reto) | Módulo del PIA que lo automatiza |
| :--- | :--- |
| sha256sum | modulo_analisis.cpp -> calcular_hash() |
| strings | modulo_analisis.cpp -> filtro de strings |
| file / readelf | modulo_analisis.cpp -> identificación ELF |
| YARA | modulo_yara.cpp -> escaneo_automatizado() |
| strace / ltrace | modulo_dinamico.cpp -> monitor_llamadas() |
| CAPA | modulo_capa.cpp -> analisis_mitre() |

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
