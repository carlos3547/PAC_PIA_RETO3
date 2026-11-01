# Reto de Fase 3 — Análisis Integrado de Binarios
## Parte 1 del PIA: Herramienta Educativa de Ciberseguridad

> **⚠️ SOLO EN VM CON INTERNAL NETWORK. Lee `ETHICS.md` antes de empezar.**

---

## Objetivo

Aplicar **de forma integrada** todas las técnicas de Fase 3 sobre un binario real:
análisis estático, YARA, desensamblado con Ghidra, análisis dinámico y mapeo
MITRE ATT&CK.

Todo tu trabajo queda en este repositorio. **No hay entrega por Teams ni PDF:**
el repo mismo es tu evidencia.

---

## Elige tu binario

Trabaja con **uno** de los siguientes (documenta tu elección en `docs/reporte_analisis.md`):

| Opción | Binario | Origen |
|--------|---------|--------|
| A | `cpp_debug` generado en A19 con `-g -O0` | Tu propio código |
| B | `cpp_prod` generado en A19 con `-O2` | Tu propio código (sin símbolos de debug) |
| C | Uno de los ELF de **Reversing ELF** (A23, TryHackMe) | TryHackMe |
| D | Otro binario ELF obtenido legalmente | Indica la fuente en el reporte |

Coloca el binario elegido en `binary/objetivo.elf`.

---

## Flujo de trabajo recomendado

```
Elige binario → Fase 1 → commit → Fase 2 → commit → Fase 3 → commit
→ Fase 4 → commit → Fase 5 → commit → Reporte final → push
```

**Haz un commit al terminar cada fase.** Mensajes sugeridos:

```bash
git commit -m "fase1: reconocimiento estático de objetivo.elf"
git commit -m "fase2: regla YARA creada y verificada"
git commit -m "fase3: análisis Ghidra — 3 funciones de interés"
git commit -m "fase4: análisis dinámico con strace y ltrace"
git commit -m "fase5: CAPA + mapeo MITRE ATT&CK"
git commit -m "reporte: reporte_analisis.md completo"
```

---

## Fases del análisis

### Fase 1 — Reconocimiento estático básico

```bash
cd binary/

# Identificación
file objetivo.elf
sha256sum objetivo.elf          # guarda este hash — es tu IOC principal

# Cabeceras ELF
readelf -h objetivo.elf
readelf -S objetivo.elf         # secciones
nm objetivo.elf 2>/dev/null | head -30

# Extracción de strings
strings -n 8 objetivo.elf > ../analysis/strings/strings_raw.txt
strings -n 8 objetivo.elf | grep -E "(http|pass|key|flag|secret|exec)" \
    > ../analysis/strings/strings_interesantes.txt

# Info general
objdump -f objetivo.elf > ../analysis/strings/objdump_info.txt
```

Agrega los resultados más relevantes a `docs/reporte_analisis.md` → **Sección 2**.  
Los archivos `.txt` generados ya son evidencia — quedan en `analysis/strings/`.

---

### Fase 2 — Regla YARA

Edita `analysis/yara/mi_regla.yar` con los strings y patrones que encontraste:

```yara
rule BinarioObjetivo_APELLIDO {
    meta:
        author      = "TU_NOMBRE TU_MATRICULA"
        date        = "YYYY-MM-DD"
        target      = "objetivo.elf"
        description = "Regla YARA — Reto Fase 3"
        ethics      = "Solo para análisis educativo en VM aislada"

    strings:
        $s1 = "TU_STRING_1" ascii
        $s2 = "TU_STRING_2" ascii
        $header_elf = { 7F 45 4C 46 }

    condition:
        uint16(0) == 0x457F and
        filesize < 50MB and
        $header_elf at 0 and
        ($s1 or $s2)
}
```

Guarda la salida de la prueba directamente en el repo:

```bash
yara -r analysis/yara/mi_regla.yar binary/ > analysis/yara/resultado_yara.txt
cat analysis/yara/resultado_yara.txt
```

Documenta en `docs/reporte_analisis.md` → **Sección 3**.

---

### Fase 3 — Desensamblado con Ghidra

1. Abre Ghidra → crea proyecto `Reto_Fase3_MATRICULA`.
2. Importa `binary/objetivo.elf` → analiza con parámetros default.
3. Corre el script `src/listar_funciones.py` desde Script Manager:

```bash
# Script Manager → Run → guarda la salida en:
analysis/ghidra/funciones.txt
```

4. Del Decompiler, copia el pseudocódigo de al menos **3 funciones de interés**:

```bash
analysis/ghidra/pseudocodigo_main.c    # función principal
analysis/ghidra/pseudocodigo_fn2.c    # función de interés #2
analysis/ghidra/pseudocodigo_fn3.c    # función de interés #3
```

5. Toma capturas de pantalla de las funciones en Ghidra y guárdalas:

```
analysis/ghidra/screenshots/
    01_symbol_tree.png
    02_decompiler_main.png
    03_decompiler_fn2.png
```

Documenta en `docs/reporte_analisis.md` → **Sección 4**.

---

### Fase 4 — Análisis dinámico

```bash
# strace — syscalls de archivo y red
strace -f -e trace=file,network ./binary/objetivo.elf \
    2>&1 | tee analysis/strace/strace_completo.log

# Filtrar lo relevante
grep -E "open|write|connect|execve" analysis/strace/strace_completo.log \
    > analysis/strace/strace_filtrado.txt

# ltrace — llamadas a librerías
ltrace -f ./binary/objetivo.elf 2>&1 | tee analysis/strace/ltrace_salida.log
```

> El log completo puede ser grande. `strace_filtrado.txt` es el que importa para el reporte.

Documenta en `docs/reporte_analisis.md` → **Sección 5**.

---

### Fase 5 — CAPA + MITRE ATT&CK

```bash
pip3 install capa
capa --update

capa binary/objetivo.elf | tee analysis/capa/capa_output.txt
capa -j binary/objetivo.elf > analysis/capa/reporte_capa.json
```

Para cada técnica que reporte CAPA, busca el ID en <https://attack.mitre.org> y completa
la tabla en `docs/reporte_analisis.md` → **Sección 6**.

---

## Estructura del repositorio al terminar

```
reto-fase3-analisis-binario/
├── binary/
│   └── objetivo.elf
├── analysis/
│   ├── strings/
│   │   ├── strings_raw.txt
│   │   ├── strings_interesantes.txt
│   │   └── objdump_info.txt
│   ├── yara/
│   │   ├── mi_regla.yar
│   │   └── resultado_yara.txt          ← evidencia de que la regla funciona
│   ├── ghidra/
│   │   ├── funciones.txt
│   │   ├── pseudocodigo_main.c
│   │   ├── pseudocodigo_fn2.c
│   │   ├── pseudocodigo_fn3.c
│   │   └── screenshots/                ← capturas de Ghidra (PNG)
│   ├── strace/
│   │   ├── strace_filtrado.txt
│   │   └── ltrace_salida.log
│   └── capa/
│       ├── capa_output.txt
│       └── reporte_capa.json
├── docs/
│   └── reporte_analisis.md             ← el reporte integrado (en Markdown)
├── src/
│   └── listar_funciones.py
├── ETHICS.md
└── README.md
```

**Tu entrega es el push a Classroom50** con todas las carpetas anteriores completas
y `docs/reporte_analisis.md` respondido.

---

## Conexión con el PIA (Parte 2)

El `reporte_analisis.md` que produces aquí es la **entrada** del PIA.
Cuando desarrolles tu herramienta C++ (`pia-herramienta-cyber`), el módulo
de análisis automatizará parte de lo que hiciste manualmente aquí.

Copia `docs/reporte_analisis.md` a `analysis/reporte_analisis.md` del repo del PIA
antes de empezar la Parte 2.

**No empieces el PIA sin haber completado primero este reto.**
