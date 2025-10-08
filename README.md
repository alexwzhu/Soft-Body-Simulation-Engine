# Soft Body Simulation (Modern OpenGL)

A simple real-time **soft-body physics simulation** built with **C++**, **GLFW**, and **GLAD**.  
The program simulates a deformable grid of particles connected by springs, demonstrating basic soft-body physics and rendering via **Modern OpenGL**.

---

## 🎮 Features

- 🧠 Soft-body physics using a **mass-spring system**
- 🌍 **Gravity** and basic **collision** detection
- 📌 **Pinned anchor points** to fix edges or corners
- 🧱 Rendered with **Modern OpenGL (3.3 Core Profile)**
- 🎨 Minimal shader-based rendering (points + lines)
- 🔁 Real-time simulation loop using **GLFW**

---

## 🧱 Dependencies

You’ll need the following libraries:

- [GLFW](https://www.glfw.org/) — window and input handling  
- [GLAD](https://glad.dav1d.de/) — OpenGL function loader  
- [OpenGL 3.3+](https://www.opengl.org/) — rendering backend  

### Install (Linux / macOS)
```bash
sudo apt install libglfw3-dev libgl1-mesa-dev

<img width="350" height="250" alt="Screen Shot 2025-09-16 at 6 29 16 PM" src="https://github.com/user-attachments/assets/38bfbb30-3cf2-447b-b1d4-44fdd4e302a3" />
