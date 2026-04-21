# Code Style and Conventions

- **SRP (Single Responsibility Principle)**: Each module should have a single responsibility.
- **Dependency Injection**: Pass dependencies (like renderers or window handles) to functions instead of using globals.
- **No Global State**: Avoid global variables to ensure predictability and testability.
- **Self-Documenting Code**: Prefer descriptive names over comments.
- **Module Size**: Aim for modules/functions to be short (ideally ≤ 100 lines for modules).
- **Constants**: Use `constants.h` for all magic numbers.
- **Naming**: Use `snake_case` for functions and variables. Use `PascalCase` or similar for types if defined via typedef (though the project seems to use `CamelCase` for structs in some places, let's verify).
- **DRY + LEAN + SOLID + KISS**: Strictly follow these principles to avoid "AI slop".
