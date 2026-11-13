# Ricochet

Proiect C++20 cu SDL3 3.4.0 si CMake 3.24+.

## Rulare din CLion

1. Deschide acest director ca proiect (sau fisierul `CMakeLists.txt`).
2. In **Settings > Build, Execution, Deployment > Toolchains**, selecteaza
   **Bundled MinGW**, CMake si Ninja incluse in CLion.
3. Lasa profilul CMake pe **Debug**, apoi executa **Reload CMake Project**.
   Prima configurare descarca SDL3 de pe GitHub si necesita internet.
4. Selecteaza tinta **Ricochet** si apasa **Run** sau **Debug**.

Aplicatia afiseaza o fereastra redimensionabila cu un dreptunghi verde.
Inchide fereastra sau apasa Escape pentru iesire.
SDL3 este compilat automat; `SDL3.dll` este copiat langa executabil pe Windows.
Nu trebuie instalat separat SDL si nu trebuie configurate manual include-uri.

## Din terminal

Cu CMake, Ninja si compilatorul C/C++ disponibile in PATH:

```sh
cmake -S . -B build -G Ninja -DCMAKE_BUILD_TYPE=Debug
cmake --build build --target Ricochet
./build/Ricochet
```

Pe Windows, executabilul este `build/Ricochet.exe`.
Argumentul `--smoke-test` inchide aplicatia automat dupa aproximativ 500 ms.

Integrarea foloseste tintele oficiale SDL:
https://wiki.libsdl.org/SDL3/README-cmake
