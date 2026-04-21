# 🛠️ Setup Guide — OS Lab Practicals

This guide will help you set up your environment to compile and run the Operating System lab practicals.

---

## 📋 Prerequisites

All practicals use Linux-only system calls (`fork`, `pipe`, `pthread`, `semaphore`, etc.), so they cannot run natively on Windows. You will need either:

- A Linux machine, or
- WSL (Windows Subsystem for Linux) on Windows

---

## 🪟 Setting Up WSL (Windows Users)

### Step 1 — Install WSL

Open PowerShell as Administrator and run:

```powershell
wsl --install
```

This installs WSL 2 with Ubuntu by default. Restart your PC when prompted.

### Step 2 — Initial Setup

After restart, Ubuntu will open automatically. Set up your username and password when prompted.

### Step 3 — Verify Installation

```bash
wsl --version
```

You should see WSL version 2.x with an Ubuntu distribution listed.

> Tip: You can always open WSL by typing `wsl` in PowerShell or searching "Ubuntu" in the Start Menu.

---

## 📦 Installing Essential Tools

Once inside your WSL terminal (or native Linux terminal), run:

```bash
sudo apt update && sudo apt install -y build-essential
```

This single command installs everything you need:

| Tool | Purpose |
|------|---------|
| `gcc` | C compiler |
| `g++` | C++ compiler |
| `make` | Build automation |
| `libc6-dev` | C library headers (`stdio.h`, `unistd.h`, etc.) |

### Verify Installation

```bash
gcc --version
```

---

## 📥 Downloading Practicals

You can download any practical directly from your terminal using our installer script:

```bash
bash <(curl -fsSL https://raw.githubusercontent.com/ashleyxdev/os-suite/main/install.sh)
```

Select a practical number from the menu, and the source files will be downloaded into a folder.

---

## 🔨 Compiling & Running

### Basic programs (no threading)

Practicals 1, 2, 3, 7, 8:

```bash
gcc -o output filename.c
./output
```

### Programs using pthreads / semaphores

Practicals 4, 5, 6 — link with `-lpthread`:

```bash
gcc -o output filename.c -lpthread
./output
```

### Shell script (Practical 1)

```bash
chmod +x library.sh
./library.sh
```

---

## 📁 Practical Overview

| # | Folder | Topic | Files |
|---|--------|-------|-------|
| 1 | `practical-1` | Library Management (Shell) | `library.sh` |
| 2 | `practical-2.1` | Fork, Zombie & Orphan Processes | `fork-wait-demo.c`, `orphan-demo.c`, `zombie-demo.c` |
| 3 | `practical-2.2` | Process Scheduling (FCFS, SJF, RR, Priority) | `main.c`, `search.c` |
| 4 | `practical-3` | Producer-Consumer Problem | `producer-consumer.c` |
| 5 | `practical-4` | Reader-Writer Problem | `reader-writer.c` |
| 6 | `practical-5` | Dining Philosophers Problem | `dining_philosophers.c` |
| 7 | `practical-6.1` | IPC using Pipes | `pipes.c`, `test.txt` |
| 8 | `practical-6.2` | IPC using Shared Memory | `process1.c`, `process2.c` |

---

Happy coding! 🚀
