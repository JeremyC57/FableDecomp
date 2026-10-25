// IsWindows9x() -- whether the GameCODA platform probe found a Windows 95, 98, 98 SE or Me host (any of the four 9x flags it set from GetVersionExA)
// Leaf: ?IsWindows9x@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C5DA70, length 45 bytes; `ret` (__cdecl), 0/1 in eax.
// Callers: 0x00C5DDEE (the DirectSound device set-up). No callees.
// The flags are set by the version probe at 0x00C73680 (GetVersionExA, IAT 0x0143FE70): Win32s 0x0143CCC1,
// Windows 95 0x0143CCC2 (minor 0), 98 0x0143CCC3 (minor <= 10, build < 2222), 98 SE 0x0143CCC4 (build 2222), Me
// 0x0143CCC5, NT 3 / NT 4 / 2000 / XP 0x0143CCC6..0x0143CCC9. 0x00C5DA60 is the matching IsWindows95 getter.
// Label: the manifest's "Audio_IsDirectSoundInitialized" is propagated; GameCODA has no PDB, names are descriptive.
// Shape: the four flags must be SEPARATE globals: as an array or a struct VC7.1 merges the first two byte tests
// into one dword load (`mov eax,[g] ; test al,al ; test ah,ah`, 40 bytes). An `int` result gives the same bytes.
extern bool g_isWindows95;              // 0x0143CCC2
extern bool g_isWindows98;              // 0x0143CCC3
extern bool g_isWindows98SE;            // 0x0143CCC4
extern bool g_isWindowsMe;              // 0x0143CCC5

bool IsWindows9x()
{
    return g_isWindows95 || g_isWindows98 || g_isWindows98SE || g_isWindowsMe;
}