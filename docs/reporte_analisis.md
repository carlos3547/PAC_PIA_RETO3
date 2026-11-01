# Reporte de Análisis de Binario — Reto de Fase 3
## Parte 1 del PIA: Herramienta Educativa de Ciberseguridad

> **Este archivo es tu entrega principal. Complétalo en Markdown y haz push al repo de Classroom50.**  
> No exportes a PDF ni entregues por Teams.

---

## 1. Identificación del estudiante

| Campo | Valor |
|-------|-------|
| Nombre | |
| Matrícula | |
| Fecha de análisis | |
| VM / SO / RAM | |
| Binario elegido (opción A/B/C/D) | |
| Justificación de la elección | |

---

## 2. Reconocimiento estático básico

### 2.1 Identificación del binario

| Campo | Valor |
|-------|-------|
| Nombre original | |
| SHA-256 | |
| Tipo (`file`) | |
| Arquitectura | |
| Stripped | Sí / No |
| Secciones ELF presentes | |

> Los archivos completos están en `analysis/strings/`.

### 2.2 Strings destacados

Copia aquí los más significativos de `analysis/strings/strings_interesantes.txt`:

```
(pegar aquí)
```

¿Qué te dicen estos strings sobre el comportamiento del binario?

> (Tu análisis aquí)

### 2.3 Secciones relevantes

| Sección | Propósito | ¿Inusual o notable? |
|---------|-----------|---------------------|
| .text | Código ejecutable | |
| .data | Variables inicializadas | |
| | | |

---

## 3. Regla YARA

### 3.1 Justificación de los indicadores elegidos

¿Por qué elegiste estos strings/bytes para tu regla? ¿Son únicos del binario o podrían causar falsos positivos?

> (Tu análisis aquí)

### 3.2 Resultado de la prueba

El archivo `analysis/yara/resultado_yara.txt` contiene la salida completa.
Pega aquí el resultado clave:

```
(pegar salida de yara aquí)
```

**¿La regla detectó el binario?** Sí / No  
**Falsos positivos encontrados:** Ninguno / (describe cuáles)

---

## 4. Análisis con Ghidra

> Las capturas de pantalla están en `analysis/ghidra/screenshots/`.

### 4.1 Funciones identificadas

Copia las más relevantes de `analysis/ghidra/funciones.txt`:

| Nombre de función | Dirección | Descripción inferida |
|-------------------|-----------|----------------------|
| main | 0x... | Punto de entrada |
| | | |
| | | |

### 4.2 Función de interés #1

**Nombre:** `___________`  
**Dirección:** `0x___`  
**¿Por qué es interesante?**

> (Tu análisis)

```c
// Ver también: analysis/ghidra/pseudocodigo_main.c
// Fragmento más relevante:
(pegar aquí)
```

### 4.3 Función de interés #2

**Nombre:** `___________`  
**Dirección:** `0x___`  
**¿Por qué es interesante?**

> (Tu análisis)

```c
// Ver también: analysis/ghidra/pseudocodigo_fn2.c
(pegar aquí)
```

### 4.4 Función de interés #3

**Nombre:** `___________`  
**Dirección:** `0x___`  
**¿Por qué es interesante?**

> (Tu análisis)

```c
// Ver también: analysis/ghidra/pseudocodigo_fn3.c
(pegar aquí)
```

### 4.5 Flujo de control general

Describe en prosa el comportamiento del binario según Ghidra:

> (¿Qué hace el programa? ¿Hay verificaciones de contraseña, cifrado, acceso a red, etc.?)

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
