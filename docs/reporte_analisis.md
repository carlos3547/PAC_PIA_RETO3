# Reporte de Análisis de Binario — Reto de Fase 3
## Parte 1 del PIA: Herramienta Educativa de Ciberseguridad

## 1. Identificación del estudiante

| Campo | Valor |
|---|---|
| Nombre | Carlos Adriàn Ramos Zacarias |
| Matrícula | 2013681 |
| Fecha de análisis | 02/11/2026 |
| VM / SO / RAM | Kali Linux / 4 GB RAM |
| Binario elegido | Opción A |
| Justificación de la elección | Binario de A19 utilizado para desarrollar el reto. |

Los datos de identificación corresponden al entorno declarado en la versión de Carlos. La sección 4 integra el análisis con Ghidra realizado por el otro integrante del equipo. No se incorporan las pruebas de la práctica personal posterior.

---

## 2. Reconocimiento estático básico

### 2.1 Identificación del binario

| Campo | Valor |
|---|---|
| Nombre del archivo analizado | objetivo.elf; nombre previo no confirmado |
| SHA-256 | a2ed5872c1d7e828ca5e43565211c81b28317fe9a8999b9f390148934c01d69b |
| Tipo (`file`) | ELF 64-bit LSB PIE executable, x86-64 |
| Arquitectura | x86-64 |
| Stripped | No |
| Secciones destacadas declaradas | .text, .data, .rodata, .bss |

Los archivos completos están en `analysis/strings/`. La lista anterior recoge las secciones declaradas en el reporte del equipo; no es una enumeración exhaustiva.

### 2.2 Strings destacados

La versión del reporte atribuida a Carlos incluye las siguientes cadenas. Deben contrastarse con los TXT finales de `analysis/strings/` antes de la entrega:

```text
http://example.com/api/status
_flags
_flags2
/home/carlos/pac-ad2026-a19-nasm-flags-objdump-group-5
```

La URL es una cadena almacenada y podría ser una referencia de configuración o de prueba; por sí sola no demuestra peticiones HTTP. La ruta sugiere información del entorno de compilación, sin demostrar acceso a esa carpeta durante la ejecución ni identificar de forma concluyente al autor.

Los nombres `_flags` y `_flags2` no bastan para atribuir uso de registros de estado del procesador ni una capacidad particular. Su procedencia debe comprobarse en el contexto del binario. Son cadenas de menos de ocho caracteres, por lo que su presencia aislada en una extracción con `strings -n 8` requiere revisar el comando o la evidencia utilizada.

### 2.3 Secciones relevantes

| Sección | Propósito | Observación |
|---|---|---|
| .text | Contiene código ejecutable. | Su presencia es esperada en este ejecutable; no demuestra por sí sola anomalías ni su ausencia. |
| .data | Contiene datos inicializados que admiten escritura. | No se ha aportado evidencia suficiente para situar aquí específicamente las cadenas de configuración. |
| .rodata | Contiene datos de solo lectura. | Figura entre las secciones declaradas en el reporte. |
| .bss | Reserva espacio para datos inicializados a cero. | Figura entre las secciones declaradas en el reporte. |

---

## 3. Regla YARA

### 3.1 Justificación de los indicadores elegidos

La versión atribuida a Carlos declara haber utilizado la URL `http://example.com/api/status` y la ruta del proyecto como indicadores. Esta descripción queda pendiente de contrastar con la versión final de `analysis/yara/mi_regla.yar`, que no se adjuntó con este reporte.

La combinación puede acotar coincidencias según la condición utilizada, pero no garantiza identificar únicamente esta muestra ni todas sus variantes. Una ruta de compilación puede cambiar entre entornos o compartirse entre varios archivos.

### 3.2 Resultado de la prueba

El bloque conservado del lado de Carlos en el conflicto contiene:

```text
BinarioObjetivo_ramos binary/objetivo.elf
```

**¿La regla detectó el binario?** Sí, según la salida consignada en esa versión del reporte.  
**Falsos positivos:** no se aportan controles de la ejecución del equipo que permitan evaluarlos.

**Pendiente de verificación:** cotejar el nombre exacto y las condiciones con `mi_regla.yar` y la salida original `resultado_yara.txt`. No se incorporan los offsets, el identificador ni los controles de la regla de práctica personal.

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

Las evidencias del equipo deben conservarse en `analysis/strace/`. Los siguientes fragmentos proceden de la versión del reporte atribuida a Carlos y corresponden a una ejecución sin argumentos adicionales. No se incluyen las pruebas personales con 0, 1, 99 o 100 ni sus logs.

### 5.1 Syscalls relevantes (strace)

Fragmento consignado por Carlos:

```text
289034 execve("./binary/objetivo.elf", ["./binary/objetivo.elf"], 0x7ffedade59a8 /* 56 vars */) = 0
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
```

El fragmento muestra el inicio del ejecutable mediante `execve`, consultas de memoria con `brk`, mapeos con `mmap` y apertura de bibliotecas por el cargador dinámico. El resultado ENOENT de `/etc/ld.so.preload` indica que ese archivo no existe; la secuencia continúa con la carga de dependencias.

**Límite de procedencia:** el reporte lo atribuye a `strace_filtrado.txt`, pero contiene llamadas que no pasarían el filtro `open|write|connect|execve`, como `brk`, `close` y `mmap`. Debe confirmarse el comando real y corregir el nombre de la evidencia o generar el filtrado desde el log original. El fragmento mostrado no contiene llamadas de red; no permite extrapolar esa ausencia a todas las entradas posibles.

### 5.2 Llamadas a bibliotecas (ltrace)

Fragmento consignado de `analysis/strace/ltrace_salida.log`:

```text
_ZStlsISt11char_traitsIcEERSt13basic_ostreamIcT_ES5_PKc(0x559894f940c0, 0x559894f92041, 0x559894f92041, 0x559894f93da8) = 0x559894f940c0
_ZNSolsEPFRSoS_E(0x559894f940c0, 0x7f0b02d46410, 0x7f0b02d46410, 12)              = 0x559894f940c0
_ZStlsISt11char_traitsIcEERSt13basic_ostreamIcT_ES5_PKc(0x559894f940c0, 0x559894f9204e, 0x559894f9204e, 0x7f0b02bf3790) = 0x559894f940c0
_ZStlsISt11char_traitsIcEERSt13basic_ostreamIcT_ES5_PKc(0x559894f940c0, 0x559894f9200d, 0x559894f940c0, 7) = 0x559894f940c0
_ZNSolsEPFRSoS_E(0x559894f940c0, 0x7f0b02d46410, 0x7f0b02d46410, 21)              = 0x559894f940c0
+++ exited (status 0) +++
```

Se observan funciones de salida de C++ con nombres codificados, relacionadas con `std::operator<<` y operaciones de flujo. Son coherentes con la impresión de mensajes descrita en `main`. El registro indica una salida con estado 0.

No aparece `getenv` en este fragmento, por lo que no se atribuye acceso a variables de entorno. Los argumentos mostrados por ltrace pueden requerir interpretación y no se consideran todos automáticamente parámetros correctamente decodificados. No se identifican llamadas inesperadas en el fragmento aportado.

### 5.3 Comparación estático vs. dinámico

| Técnica | Lo que reveló | Lo que NO reveló |
|---|---|---|
| strings | Texto almacenado, incluida una URL y una ruta. | No confirma el uso de las cadenas ni reconstruye el flujo de control. |
| Ghidra | Funciones principales, conversión de argumentos, rango 1–99 y cálculo con XOR. | No demuestra qué rutas se ejecutaron realmente en una prueba. |
| strace | Arranque, carga de bibliotecas y operaciones de memoria del recorrido registrado. | No reconstruye por sí solo la lógica de validar y compute. |
| ltrace | Llamadas de salida de la biblioteca de C++ durante el recorrido registrado. | No representa todas las instrucciones internas ni sustituye la traza de syscalls. |

La evidencia dinámica del equipo aquí conservada corresponde al arranque sin argumentos; no confirma dinámicamente la validación ni el resultado del cálculo. Esos comportamientos se describen como hallazgos estáticos de la sección 4.

---

## 6. CAPA + MITRE ATT&CK

Los archivos de evidencia son `analysis/capa/capa_output.txt` y `analysis/capa/reporte_capa.json`.

### 6.1 Capacidades detectadas por CAPA

Salida atribuida al entorno de Carlos:

```text
┌─────────────┬────────────────────────────────────────────────────────────────────────────────────┐
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
```

CAPA identifica la muestra como un ELF de Linux para amd64 y no reporta capacidades en su salida resumida. La ausencia de capacidades reportadas no significa que el programa no haga nada ni garantiza que sea seguro.

No se atribuyen a esta ejecución la versión, las reglas auxiliares ni otros detalles obtenidos del JSON de la práctica personal. El JSON final del equipo debe cotejarse por separado.

### 6.2 Mapeo a MITRE ATT&CK

| IOC / Capacidad | Tipo | ID Técnica | Nombre Técnica | Táctica | Herramienta |
|---|---|---|---|---|---|
| a2ed5872c1d7e828ca5e43565211c81b28317fe9a8999b9f390148934c01d69b | Identificador de archivo SHA-256 | No aplica | No aplica | No aplica | Hash consignado en la salida de CAPA |
| Sin capacidades con mapeo ATT&CK reportadas | Resultado del análisis | No identificado | No identificado | No identificada | CAPA |

El hash identifica la muestra, no una técnica de ataque. La ejecución manual de un programa y la carga normal de bibliotecas no aportan por sí solas contexto suficiente para atribuir T1204 o T1129 a esta muestra. Se elimina la asociación a T1615: esa técnica corresponde a Group Policy Discovery, no a acceso a variables de entorno.

Referencias para comprobar el alcance de las técnicas descartadas:

- https://attack.mitre.org/techniques/T1204/
- https://attack.mitre.org/techniques/T1129/
- https://attack.mitre.org/techniques/T1615/

### 6.3 Análisis de riesgo

No hay técnicas ATT&CK sustentadas en los resultados conservados que permitan asignar un nivel de riesgo basado en esas técnicas. El código revisado con Ghidra es consistente con un ejercicio de entrada numérica y salida por consola; las trazas del equipo muestran el recorrido de inicio sin argumentos. Estas evidencias no justifican clasificarlo como malware ni garantizan su inocuidad.

No se mantiene la calificación de riesgo moderado de la versión anterior, porque se apoyaba en técnicas atribuidas a CAPA que no aparecen en su salida. Una evaluación más amplia requeriría conocer el origen autorizado de la muestra y contrastar otras rutas de ejecución.

---

## 7. Conclusiones

### 7.1 Resumen del comportamiento del binario

El archivo analizado es un ELF de 64 bits para Linux con código C++. El análisis con Ghidra muestra que imprime un saludo y el identificador MAGIC. Si recibe un argumento, lo convierte a entero, admite valores del 1 al 99 y calcula un resultado mediante suma, multiplicación y tres operaciones XOR. Existe una discrepancia entre ese rango y el mensaje de rechazo, que anuncia 0–100. Las trazas del equipo conservadas en este reporte corresponden a la ejecución sin argumentos y muestran carga de bibliotecas y llamadas de salida, con terminación normal en ltrace. La salida resumida de CAPA no reporta capacidades. Ghidra permite explicar la lógica interna; las herramientas dinámicas aportan evidencia del recorrido efectivamente observado. No se atribuye comunicación HTTP únicamente por encontrar una URL ni se asignan técnicas ATT&CK sin evidencia suficiente.

### 7.2 Cadena de herramientas usada

| Herramienta | ¿Usada? | Notas |
|---|---|---|
| file | Sí, declarada | Identificación de formato y arquitectura. |
| sha256sum | Sí, declarada | Hash de la muestra, también consignado por CAPA. |
| strings | Sí, declarada | Cadenas; cotejar los TXT finales y el mínimo de longitud empleado. |
| readelf / nm | Sí, declarada | Cabeceras y símbolos; conservar salidas originales. |
| YARA | Sí, declarada | Pendiente cotejar la regla final con la descripción y la salida. |
| Ghidra | Sí | Análisis documentado en la sección 4, con tres funciones. |
| strace | Sí, según evidencia consignada | Confirmar filtrado y comando del fragmento. |
| ltrace | Sí, según evidencia consignada | Llamadas de salida de C++ y terminación con estado 0. |
| CAPA + MITRE ATT&CK | Sí, con alcance limitado | CAPA ejecutado; no se sustenta un mapeo a técnicas ATT&CK. |

### 7.3 Conexión con el PIA (Parte 2)

Los nombres siguientes son propuestas de módulos y funciones, no implementaciones verificadas del PIA.

| Análisis manual | Propuesta de automatización |
|---|---|
| sha256sum | modulo_analisis.cpp → calcular_hash() |
| strings | modulo_analisis.cpp → filtro de strings |
| file / readelf | modulo_analisis.cpp → identificación ELF |
| YARA | modulo_yara.cpp → escaneo_automatizado() |
| strace / ltrace | modulo_dinamico.cpp → monitor_llamadas(), dentro de un entorno aislado |
| CAPA | modulo_capa.cpp → ejecución y lectura de resultados; solo mapear técnicas sustentadas |

---

## 8. Reflexión

1. **¿Qué herramienta fue más útil para entender el binario y por qué?**  
   > Ghidra fue la herramienta más útil, ya que su descompilador de código intermedio (IL) a C permitió analizar con claridad la lógica interna, las llamadas a funciones y la estructura del binario ELF sin necesidad de interpretar directamente código ensamblador en crudo.
   > Esto facilitó identificar rápidamente los puntos de entrada y los flujos de control condicionales del objetivo.


2. **¿Qué encontraste en el análisis dinámico que el estático no mostraba?**  
   > El análisis dinámico (mediante herramientas de rastreo) permitió observar el comportamiento real en tiempo de ejecución, revelando llamadas a librerías compartidas, resolución dinámica de símbolos y argumentos que se cargan únicamente en memoria durante la ejecución.
   > El análisis estático por sí solo solo muestra la estructura estática del binario, mientras que la ejecución expone interacciones reales con el sistema operativo y el manejo dinámico de recursos.

3. **¿Cómo usarías CAPA en un pipeline automatizado de análisis de malware?**  
   > Se integraría CAPA como una etapa automatizada posterior a la ingesta de muestras (por ejemplo, en un script de Python dentro de un entorno aislado).
   > Esto permitiría etiquetar capacidades técnicas de forma masiva como técnicas de evasión, persistencia o inyección identificadas por reglas ATT&CK para priorizar rápidamente qué muestras sospechosas requieren una inversión de tiempo mayor en ingeniería inversa manual.

4. **Si este binario fuera malware, ¿qué técnica anti-análisis hubiera complicado más tu trabajo?**  
   > Técnicas de ofuscación avanzada de código, empaquetado (packing) o rutinas de anti-depuración (como la detección de trazas mediante ptrace o comprobaciones de temporizadores de ejecución).
   > Estas técnicas fuerzan al analista a realizar un desempaquetado manual en memoria o a implementar técnicas de evasión de parches (hooking/patching) antes de poder comprender la verdadera lógica del binario.

---

*Programación para Ciberseguridad — FCFM-UANL — AD2026*
