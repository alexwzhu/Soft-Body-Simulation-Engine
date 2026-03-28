# Soft Body Simulation

A real-time soft-body physics simulation built with C++, GLFW, and OpenGL.

<img width="370" height="400" alt="Screen Shot 2025-09-16 at 6 29 16 PM" src="https://github.com/user-attachments/assets/38bfbb30-3cf2-447b-b1d4-44fdd4e302a3" />

## What it does

Simulates a 6x6 grid of particles connected by springs. The top two corners are pinned in place while the rest of the cloth falls under gravity, stretches, and oscillates. Particles are rendered as blue dots, springs as white lines.

## Dependencies

All dependencies are bundled in the repo — no installs needed.

- GLFW 3.4 (dylib + headers in `dependencies/`)
- GLAD (OpenGL 3.3 core loader — `glad.c` + `dependencies/include/glad/`)
- OpenGL 3.3+ (provided by macOS)

## Building and running

After cloning, compile from the project root:

```bash
gcc -c glad.c -I dependencies/include -o glad.o
g++ main.cpp glad.o -o app -I dependencies/include -L dependencies/library -lglfw.3.4 -framework OpenGL -Wl,-rpath,@executable_path/dependencies/library
```

Then run:

```bash
./app
```

Close the window to exit.
