# Recompiler handoff: where the Windows x64 build stopped (2026-10-03)

Goal: Fable TLC running as a native x64 Windows exe (then Android). The exe needs the
user's original game folder; no retail bytes or generated code are in git.

## Where things stand

- **Lifter** (`lifter/`): turns Fable.exe (x86) into C.
  - Now also adds entries for code that is only reached through pointers: relocations,
    data dwords, and `push offset` immediates. That gives 62,888 functions.
  - New options: `-` in place of `functions.tsv`, `--prefix NAME` (to lift a DLL) and
    `--no-refs`.
  - Direct calls to addresses that are not lifted go through `recomp_dispatch`.
- **Runtime** (`runtime/`):
  - `RECOMP_IDENTITY_MEMORY` mode: guest address == host address, accessed via `GP()`.
  - `recomp_register_table()` registers extra function tables, one per recompiled DLL.
- **Windows host** (`win/`): **builds, links (52 MB) and boots under Wine.** What runs so far:
  - The CRT starts up, and static constructors run.
  - The game creates threads, checks the EULA and opens files from the game folder.
  - It loads `strings.dll` as a data file.
  - It loads the **recompiled ConfigDetect.dll** at 0x10000000 and runs its DllMain.
- **Current blocker:** inside ConfigDetect.dll's CRT init, the MSVC `/GS` check fails with
  "Buffer overrun detected!" and shows a MessageBox that blocks under headless Wine.
  - Likely cause: a host import writes past a 32-bit guest struct, or a guest DLL CRT
    import is wrong.
  - Suspects: `GetVersionExA`, `GetStartupInfoA`, `GetCPInfo`, `GetModuleFileNameA`,
    `GetEnvironmentStrings*`, `LCMapString*`, `GetStringType*`, and the TLS functions.
  - **Next step:** run with `FABLE_RECOMP_LOG=2` and read the import calls just before
    `user32!MessageBoxA`. Also check `__security_init_cookie` and whether its stack frames
    agree.

## Files in `win/`

| File | Purpose |
|---|---|
| `host.hpp` | Core API: guest memory helpers, traps/import registry, `Arg<T>` converters, `stdThunk`/`cdeclThunk`, `FWD_STD` / `IMPORT` / `IMPORTN` macros, guest threads, `guestCall` |
| `core.cpp` | Logging, low-memory allocator (guest heap: boundary tags over 64 MB segments below 2 GB), traps, import table, guest threads/TEB/PEB, host→guest calls, module handles |
| `loader.cpp` | Maps PE images at their base, binds imports to traps, loads recompiled DLLs (calls DllMain), export lookup |
| `main.cpp` | Finds the game folder (`--game`, `FABLE_DIR`, or the exe's own folder), verifies the Fable.exe SHA-256, maps it, runs entry 0x401067 on a 256 MB-stack thread |
| `crt.cpp` | msvcr71/msvcp71 (startup, heap, strings, printf family, math incl. `_CI*`, a VC7.1-exact qsort, `std::exception`, VC7.1 `std::string`) |
| `format.cpp/.hpp` | printf/scanf over guest varargs with MSVC semantics |
| `kernel32.cpp` | kernel32: critical sections (host pointer kept in the guest CS), async I/O (`ReadFileEx`), resources, `LoadLibrary`/`GetProcAddress`, TLS, etc. |
| `user32.cpp` | Window classes/procs (guest WndProc called through a host proc), MSG and `lParam` struct conversion, dialogs |
| `misc.cpp` | gdi32, advapi32 (registry in the 32-bit view), winmm, version, shell32, ole32/oleaut32, imm32, wsock32 (network reported as unavailable) |
| `com.hpp/.cpp`, `com_vtables.inc`, `gen_com.py` | COM proxies. Guest vtables of traps for D3D9 and DInput8 are generated from mingw headers. `OVERRIDES` in `gen_com.py` lists the hand-written methods |
| `d3d9.cpp` | Direct3DCreate9, CreateDevice/Reset (converts D3DPRESENT_PARAMETERS, applies the 24-bit FPU mode), all Lock/Unlock through guest staging buffers. D3DX math is native; D3DX texture helpers load any x64 `d3dx9_XX.dll` at run time |
| `dinput.cpp` | DirectInput8Create, SetDataFormat, GetDeviceData, EnumDevices, EnumObjects |
| `gamedlls.cpp` | Stand-ins for 32-bit game-folder DLLs (`eula.dll!EBUEula` returns accepted) |
| `CMakeLists.txt`, `cmake/llvm-mingw-x64.cmake` | Cross build. `FABLE_GEN_DIR` = lifter output. `FABLE_GUEST_DLLS="cfgdetect\|ConfigDetect.dll\|<dir>"` |

## How to rebuild

The original session used Linux with llvm-mingw, Wine 11.18 and Xvfb. On Windows you can
use llvm-mingw directly, or clang-cl after small tweaks.

```sh
# Lifter (needs Zydis): build rebuild/recomp, then
fable_recomp Fable.exe rebuild/manifest/functions.tsv gen_win/
fable_recomp ConfigDetect.dll - gen_cfg/ --prefix cfgdetect
python3 rebuild/recomp/win/gen_com.py <llvm-mingw>/generic-w64-mingw32/include rebuild/recomp/win/com_vtables.inc
cmake -S rebuild/recomp/win -B build/win -G Ninja \
  -DCMAKE_TOOLCHAIN_FILE=rebuild/recomp/win/cmake/llvm-mingw-x64.cmake -DLLVM_MINGW=<llvm-mingw> \
  -DFABLE_GEN_DIR=gen_win "-DFABLE_GUEST_DLLS=cfgdetect|ConfigDetect.dll|gen_cfg" -DCMAKE_BUILD_TYPE=Release
cmake --build build/win     # about 5 min; the generated C is ~250 MB
FableRecomp.exe --game "D:\SteamLibrary\steamapps\common\Fable The Lost Chapters"
```

The log goes to `<game>\FableRecomp.log`.

Environment variables:
- `FABLE_RECOMP_LOG=2` logs every import call.
- `FABLE_RECOMP_HEADLESS=1` turns off the fatal-error MessageBox.

Under Wine you must create `Documents\My Games\Fable` in the prefix.

## Known gaps, in rough order

1. The ConfigDetect `/GS` failure (above).
2. **C++ exceptions and SEH are not implemented.** `_CxxThrowException`, `__CxxFrameHandler`
   and `_except_handler3` all end in a fatal error.
   - The game throws `CBBBFileException` when a file is missing, so the full data folder
     is needed. It will also need real unwinding: a longjmp-style resume into the catch
     funclet's continuation, which needs lifter support.
3. Audio:
   - dsound.dll, OpenAL32.dll and wrap_oal.dll are reported as missing, and
     `CoCreateInstance` fails.
   - The game needs a DirectSound proxy (COM, like D3D9) or an OpenAL proxy.
   - WMV video (DirectShow via `CoCreateInstance`) is not supported either.
4. `mgspid.dll` / `PidGen.dll` (product ID) are reported as missing. This has not been
   checked for side effects.
5. The registry key `HKLM\Software\Microsoft\Microsoft Games\Fable\1.0` is missing under
   Wine. Steam creates it on a real install.
6. waveOut/mixer report no devices.
7. Testing:
   - The game data was only partly staged into the Linux session; the big `.big`, `.lut`,
     `.stb` and `.wad` files were not.
   - Real testing should now happen on the Windows PC with the full install.
8. Android:
   - Needs a non-identity memory layout, or a guest region reserved below 4 GB in the
     ARM64 process.
   - The Win32/D3D9 layer must be re-targeted to SDL/GLES or Vulkan.
