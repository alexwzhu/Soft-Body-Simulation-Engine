# Soft Body Simulation

A real-time soft-body physics simulation built with C++, GLFW, and OpenGL.

<img width="370" height="400" alt="Screen Shot 2025-09-16 at 6 29 16 PM" src="https://github.com/user-attachments/assets/38bfbb30-3cf2-447b-b1d4-44fdd4e302a3" />

## What it does

Simulates a 6x6 grid of particles connected by springs. The top two corners are pinned in place while the rest of the cloth falls under gravity, stretches, and bounces off the floor. Particles are rendered as blue dots, springs as white lines.

## Dependencies

- [GLFW](https://www.glfw.org/) — window and input handling
- [GLAD](https://glad.dav1d.de/) — OpenGL function loader
- OpenGL 3.3+

## Building and running

Make sure GLFW and GLAD are available, then compile with:

```bash
g++ main.cpp glad.c -o app -lglfw -framework OpenGL
```

Then run:

```bash
./app
```

Close the window to exit.
