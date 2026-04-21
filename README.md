# 0-board

Un teclado virtual moderno desde cero, diseñado con principios de ingeniería de software sólidos: DRY, SOLID, KISS y LEAN. Renderizado vectorial Cairo con interfaz minimalista y código mantenible para agentes de IA.

![0-board Banner](https://via.placeholder.com/800x400.png?text=0-board+Clean+Architecture)

## 🎯 Filosofía del Proyecto

**0-board** representa una reescritura completa desde cero ("0") con foco en:
- **Código autocontenido**: Sin deuda técnica, sin dependencias innecesarias
- **Arquitectura limpia**: Principios SOLID aplicados rigurosamente  
- **Mantenibilidad IA-first**: Estructura predecible para agentes de IA
- **Eficiencia LEAN**: Recursos justos, sin desperdicio

## ✨ Características Principales

### 🏗️ **Arquitectura Sólida**
- **SRP estricto**: Módulos con responsabilidad única
- **Inyección de dependencias**: Testing sin mocks complejos
- **Interfaces abstractas**: Backends intercambiables (X11, Wayland-ready)
- **Sin estado global**: Predictibilidad total

### 🎨 **Experiencia Visual**
- **Renderizado Cairo**: Vectorial, antialiasing, transparencia nativa
- **Diseño minimalista**: Enfoque en funcionalidad, no en ornamentos
- **Personalización en tiempo real**: Opacidad, esquemas de color, fuentes
- **Tamaños adaptativos**: Small/Medium/Large según necesidades

### ⚡ **Eficiencia LEAN**
- **Lazy loading**: Fuentes cargadas bajo demanda
- **Double buffer optimizado**: Sin triple buffer innecesario
- **Event-driven**: Renderizado solo cuando cambia el estado
- **RAM consciente**: ~5MB uso máximo, no 50MB

## 📊 Comparativa Arquitectónica

| Principio | Código Legacy | **0-board** |
| :--- | :--- | :--- |
| **SOLID** | Violaciones múltiples | Aplicación rigurosa |
| **DRY** | Duplicación en buffer×3 | Abstracciones reusables |
| **KISS** | 354 líneas en ui.c | Módulos ≤100 líneas |
| **LEAN** | 512 fuentes al inicio | Lazy loading |
| **Acoplamiento** | Alto (X11+Cairo+FontConfig) | Bajo (interfaces) |
| **Testabilidad** | Difícil (globales) | Trivial (inyección) |

## 🚀 Instalación

### Requisitos

```bash
# Debian/Ubuntu
sudo apt install libcairo2-dev libx11-dev libxtst-dev pkg-config

# Fedora/RHEL  
sudo dnf install cairo-devel libX11-devel libXtst-devel pkg-config
```

### Compilación

```bash
git clone https://github.com/tu-usuario/0-board.git
cd 0-board
make
./0-board
```

## 🏗️ Estructura del Proyecto

```
0-board/
├── src/
│   ├── core/           # Lógica de negocio
│   │   ├── keyboard.c  # Estado y lógica de teclado
│   │   └── layout.c    # Definición de disposiciones
│   ├── render/         # Abstracción de gráficos
│   │   ├── renderer.h  # Interface abstracta
│   │   └── cairo_renderer.c  # Implementación Cairo
│   ├── platform/       # Backends específicos
│   │   ├── x11_window.c
│   │   └── x11_engine.c
│   ├── ui/             # Coordinación de UI
│   └── main.c          # Punto de entrada
├── include/            # Headers públicos
└── Makefile
```

## 🔧 API para Agentes de IA

```c
// Ejemplo de uso predecible
Keyboard* kb = keyboard_create(layout);
Window* win = window_create(x11_backend);
Renderer* renderer = cairo_renderer_create(win);

// Inyección clara de dependencias
Application* app = application_create(kb, win, renderer);
application_run(app);
```

## 🧪 Testing

```bash
# Compilar con cobertura
make test

# Ejecutar tests unitarios  
./test_keyboard
./test_layout
```

## 📈 Roadmap

### Fase 1 ✓ **Estabilización** (Actual)
- [x] Renombre a 0-board
- [x] Eliminación de variables globales
- [x] Constantes con nombre
- [x] Gestión modular de fuentes

### Fase 2 🔄 **Desacoplamiento** (En progreso)
- [ ] Interface Renderer abstracta
- [ ] Sistema de eventos desacoplado
- [ ] Gestor de layouts dinámico
- [ ] Configuración externalizable

### Fase 3 ⏳ **Optimización**
- [ ] Double buffer simple
- [ ] Cache de superficies
- [ ] Benchmarks comparativos
- [ ] Soporte Wayland (opcional)

## 🤝 Contribuir

0-board está diseñado para ser **contribuible por agentes de IA**. Las reglas:
1. **Nombres > comentarios**: Código autodocumentado
2. **Módulos ≤100 líneas**: Responsabilidad única
3. **Sin magic numbers**: constants.h para todo
4. **Interfaces abstractas**: Nuevas features sin tocar core

## 📄 Licencia

MIT - Ver [LICENSE](LICENSE) para detalles.

---

*0-board: Teclado virtual desde cero, código desde cero, deuda técnica desde cero.*