# ETHICS.md — Reto de Fase 3 / Parte 1 del PIA

## Declaración de uso ético

Este repositorio forma parte de un ejercicio académico de análisis de binarios con fines
educativos, en el marco del curso **Programación para Ciberseguridad** de la FCFM-UANL.

## Restricciones obligatorias

### 1. Entorno de ejecución
- **TODO el trabajo de este reto se realiza EXCLUSIVAMENTE en una VM con Internal Network.**
- **PROHIBIDO** ejecutar el binario objetivo directamente en tu máquina host.
- **PROHIBIDO** habilitar adaptador de red tipo NAT o Bridged durante el análisis dinámico.
- Configuración mínima requerida: VM con Internal Network únicamente.

### 2. Alcance del análisis
- Los binarios analizados son **benigno o de propósito educativo**.
- Si el binario elegido es de una fuente externa (opción C o D), debes tener **permiso explícito** para analizarlo.
- **PROHIBIDO** analizar binarios de sistemas en producción sin autorización escrita del propietario.

### 3. Uso de las técnicas aprendidas
- Las técnicas de reverse engineering de este reto son para **comprender** sistemas, no para comprometer sistemas ajenos.
- Cualquier aplicación de estas técnicas fuera del entorno académico sin autorización puede constituir:
  - **CPF Art. 211 Bis** — Acceso ilícito a sistemas y equipos de informática (hasta 8 años de prisión)
  - Violación a la Ley Federal de Telecomunicaciones y Radiodifusión

### 4. Datos y confidencialidad
- No incluyas en el repositorio: contraseñas reales, tokens, datos personales de terceros.
- Si el análisis dinámico captura tráfico de red con datos sensibles, **no lo subas al repo**.

## Contenido del triple confinamiento (triple containment)

```
VM aislada (Internal Network)
└── Binario de propósito educativo o con permiso
    └── Análisis sin exfiltración de datos reales
```

## Declaración del estudiante

Al hacer push a este repositorio, confirmas que:

- [ ] Leíste y entiendes este documento.
- [ ] Todo el análisis dinámico lo realizaste en una VM con Internal Network.
- [ ] No usaste estas técnicas en sistemas sin autorización.
- [ ] El binario analizado es propio, educativo o tienes permiso para analizarlo.
- [ ] No subiste datos sensibles al repositorio.

**Matrícula:** ________________  
**Fecha:** ________________

---

*"La seguridad informática es una responsabilidad, no un juego."*
