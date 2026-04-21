# XVKBD Modern

Una versión modernizada del clásico teclado virtual `xvkbd`, rediseñada desde cero para ofrecer una experiencia visual premium inspirada en el ecosistema Apple, con un motor de renderizado basado en Cairo y opciones de personalización avanzadas.

![XVKBD Modern Banner](https://via.placeholder.com/800x400.png?text=XVKBD+Modern+UI)

## ✨ Características Principales

- **Diseño Apple Style**: Interfaz elegante con bordes redondeados, tipografía moderna (Inter) y efectos de transparencia.
- **Tamaños Dinámicos (S/M/L)**: Cambia entre tres tamaños predefinidos (Small, Medium, Large) con un solo clic. El teclado se adapta inteligentemente al ancho de tu pantalla.
- **Panel de Personalización**: Menú integrado para cambiar colores de fondo, teclas, opacidad y niveles de redondeado en tiempo real.
- **Motor Cairo**: Renderizado vectorial de alta calidad con antialiasing y soporte para transparencia nativa (RGBA).
- **Multicapa**: Soporte completo para capas de Shift y Símbolos.

## 🚀 Instalación

### Requisitos

Asegúrate de tener instaladas las siguientes librerías de desarrollo:

- `libcairo2-dev`
- `libx11-dev`
- `libxtst-dev`
- `pkg-config`

En sistemas basados en Debian/Ubuntu:
```bash
sudo apt install libcairo2-dev libx11-dev libxtst-dev pkg-config
```

### Compilación

Clona el repositorio y compila usando el Makefile proporcionado:

```bash
make
```

Esto generará el binario `xvkbd_new`.

### Uso

Para ejecutar el teclado:
```bash
./xvkbd_new
```

- **Clic Izquierdo**: Enviar tecla.
- **Clic Derecho**: Ciclar entre tamaños S, M y L.
- **Botón "menu"**: Abrir el panel de configuración visual.
- **Arrastrar**: Haz clic en cualquier área vacía del teclado para mover la ventana.

## 🎨 Personalización

El teclado incluye un menú de personalización donde puedes ajustar:
- Paletas de colores (Carbón, Azul Noche, Vino, etc.)
- Opacidad de la ventana (efecto Glassmorphism).
- Radio de los bordes redondeados.
- Contornos de las teclas.

## 🛠️ Estructura del Proyecto

- `src/main.c`: Punto de entrada y lógica principal.
- `src/ui.c`: Motor de renderizado y gestión de eventos X11/Cairo.
- `src/menu.c`: Lógica y UI del panel de configuración.
- `src/engine.c`: Emulación de pulsaciones de teclas (Xtst).
- `src/layout.c`: Definición de la distribución de teclas.

## 📄 Licencia

Este proyecto se distribuye bajo la licencia MIT. Consulta el archivo `LICENSE` para más detalles.

---
Desarrollado con ❤️ por **ionet-cl** en la organización **iodevs**.
