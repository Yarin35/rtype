# ECS Network Game Project

This project is a simple C++ game using an Entity-Component-System (ECS) architecture with client-server networking. The game is a basic spaceship shooter inspired by R-Type, where clients control ships and the server manages the authoritative game state.

## 📁 **Project Structure**

```readme
📦 RTYPE
├─ 📂 src
│  ├─ 📂 ecs                # ECS Core (shared)
│  │  ├─ Entity.h           # Entity and Signature
│  │  ├─ ECSManager.h       # Entity management
│  │  └─ Init.cpp           # Game initialization
│  ├─ 📂 components         # Game components (shared)
│  │  ├─ Components.h       # Position, Velocity, Health
│  │  └─ Systems.h          # Systems for movement, collision
│  ├─ 📂 network            # Networking module (shared)
│  │  └─ Network.h          # Send/receive utilities
│  ├─ 📂 server             # Server-side logic
│  │  └─ ServerMain.cpp     # Server entry point
│  ├─ 📂 client             # Client-side logic
│  │  └─ ClientMain.cpp     # Client entry point
│  └─ 📂 common             # Shared utilities and logic
│     └─ Utils.h            # Optional: Helpers for serialization, etc.
├─ 📂 build                 # Build artifacts
├─ 📄 CMakeLists.txt        # CMake build setup
└─ 📄 README.md             # Project overview
```

## 🚀 **Setup and Build Instructions**

### **1. Prerequisites:**

- C++17 or later
- CMake 3.10+
- A C++ compiler (GCC, Clang, MSVC)

### **2. Build Steps:**

```bash
# Clone the repository
git clone https://github.com/yourusername/ECS_Network_Game.git
cd ECS_Network_Game

# Create build directory and compile
mkdir build
cd build
cmake ..
make
```

### **3. Running the Server and Client:**

Open two terminals:

- **Server:**

```bash
./server
```

- **Client:**

```bash
./client
```

## 🛠️ **Key Modules**

1. **ECS:** Core entity-component-system logic.
2. **Network:** Simple UDP-based communication.
3. **Game Logic:** Basic spaceship movement, shooting, and alien spawns.

## 📜 **License**

MIT License.

## 🤝 **Contributions**

Contributions are welcome! Submit a PR if you improve the code or add features.

---
🚀 *Built with C++, ECS, and Networking for a quick game prototype.*
