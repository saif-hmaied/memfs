# memfs

An in-memory file system with a small Unix-like shell, written in C++20.
Every folder and file is a C++ object on the heap; nothing touches the disk.

I built it to practice object-oriented design (inheritance, virtual functions,
abstract classes) and manual memory management with raw pointers.

## Build and run

Requires a C++20 compiler and CMake (tested with g++ 13 on Ubuntu / WSL).

```bash
cmake -S . -B build -G Ninja
cmake --build build
./build/memfs
```

## Example

```
memfs:/$ mkdir projects
memfs:/$ cd projects
memfs:/projects$ touch notes.txt
memfs:/projects$ write notes.txt hello world
memfs:/projects$ cat notes.txt
hello world
memfs:/projects$ mkdir src
memfs:/projects$ ls
notes.txt
src/
memfs:/projects$ cd ..
memfs:/$ cp -r projects /projects/src
memfs:/$ stats
nodes in memory: 7
memfs:/$ rm projects
rm: cannot remove projects
memfs:/$ rm -r projects
memfs:/$ stats
nodes in memory: 1
```

## Commands

| Command | Description |
|---|---|
| `pwd` | print the current folder |
| `ls [path]` | list a folder |
| `cd <path>` | change folder |
| `mkdir <path>` | create a folder |
| `touch <path>` | create an empty file |
| `write <path> <text...>` | set a file's content |
| `cat <path>` | print a file's content |
| `rm [-r] <path>` | delete a file, or a folder with `-r` |
| `mv <src> <folder>` | move into a folder |
| `cp [-r] <src> <folder>` | deep copy into a folder |
| `stats` | number of nodes alive in memory |
| `help`, `exit` | |

Paths can be absolute (`/a/b`) or relative (`../a`), with `.` and `..`.

## Design

```
Node (abstract: name, parent, virtual size/is_directory/clone, virtual destructor)
├── File       (content)
└── Directory  (vector<Node*> children)
```

- **Ownership tree:** a `Directory` owns its children and deletes them in its
  destructor, so deleting a folder frees its whole subtree. The `parent`
  pointer only looks; it never owns.
- **`Filesystem`** owns the root `/` and keeps a non-owning `cwd` pointer.
  All path logic (`solve_path`) lives here; `main.cpp` only parses text.
- **Deep copy** uses a virtual `clone()`: each class knows how to copy itself,
  so `cp` works without knowing whether it copies a file or a folder.
- Copying a `Directory` or `Filesystem` object is disabled (`= delete`) to
  avoid two owners deleting the same memory.

## Memory safety

Raw pointers make some commands dangerous, so they check before acting:

- `rm` refuses to delete `/`, the current folder, or any folder above it
  (the `cwd` pointer would dangle).
- `mv` refuses to move a folder into itself (the subtree would be cut off
  from the tree and leak).
- New nodes are only allocated after every check passes, so a refused
  command never leaks.
- A static counter in `Node` tracks live objects (`stats`), and the code was
  tested with AddressSanitizer: no leaks, no double frees.

## Limitations

- No persistence: everything is lost on exit.
- `mv` moves into an existing folder but does not rename.
- `..` above the root is an error instead of staying at `/`.
