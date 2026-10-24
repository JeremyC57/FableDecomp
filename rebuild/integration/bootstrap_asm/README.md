# Bootstrap asm fixtures (not reconstruction)

`__asm`/`naked` bodies that `rebuild/build_bootstrap.ps1` compiles into the Windows GFMain runtime
fixtures. They call the fixture's `Fable*` shim symbols, so they are kept as they are; they are not
catalog rows. The landed, byte-exact reconstructions of the same addresses are genuine C++:

| address | fixture | landed source |
|---|---|---|
| 0x00403B10 | `src/CSystemManagerInit_Constructor_00403b10.cpp` | `rebuild/src/compiled/00/40/CSystemManagerInit_CSystemManagerInit_00403b10.cpp` |
| 0x0099A920 | `src/CDiskFileWin32_Close_0099A920.cpp` | `rebuild/src/compiled/00/99/CDiskFileWin32_Close_0099a920.cpp` |
| 0x0099AD80 | `src/CDiskFileWin32_Constructor_0099AD80.cpp` | `rebuild/src/compiled/00/99/CDiskFileWin32_CDiskFileWin32_0099ad80.cpp` |
| 0x009C9070 | `src/NGameText_CDataBank_GetPGroupEntry_009c9070.cpp` | `rebuild/src/compiled/00/9c/NGameTextCDataBank_GetPGroupEntry_009c9070.cpp` |

Moved here 2026-10-24 out of `rebuild/src/compiled` and `rebuild/tests`, where the 0x0099A920 pair
differed from the landed files only by letter case and collided on a Windows checkout. Retiring them
means pointing the fixture at the genuine sources and providing their real callees instead of the shims.
