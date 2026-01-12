# Contributing to Teensy 4.1 Serial Router

Thank you for your interest in contributing! This project follows specific development practices to ensure quality and maintainability.

## Branching Strategy (GitHub Flow)

We use a simplified **GitHub Flow**:
1.  **Main Branch**: `main` is always deployable.
2.  **Feature Branches**: Create a new branch for every feature or fix.
    - Format: `type/short-description`
    - Examples: `feat/add-star-command`, `fix/buffer-overflow`, `docs/update-kpi`
3.  **Pull Requests**: Open a PR to merge your branch into `main`.
4.  **Merge**: Squash and merge after review and CI checks pass.

## Commit Convention

We follow **Conventional Commits** to automate changelog generation.
Format: `<type>(<scope>): <description>`

Types:
- `feat`: A new feature
- `fix`: A bug fix
- `docs`: Documentation only changes
- `style`: Changes that do not affect the meaning of the code (white-space, formatting, etc)
- `refactor`: A code change that neither fixes a bug nor adds a feature
- `test`: Adding missing tests or correcting existing tests
- `chore`: Changes to the build process or auxiliary tools

Example: `feat(router): add support for ADD_STAR command`

## Development Workflow

1.  **PlatformIO**: Ensure you have PlatformIO installed.
2.  **Unit Tests**: Run `pio test -e native` locally to verify logic before pushing.
3.  **Simulation**: Run the host simulation to verify protocol integration.

## Pull Request Process

1.  Ensure all local tests pass.
2.  Update documentation if you changed behavior.
3.  The PR title should follow the Conventional Commit format.
