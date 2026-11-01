// EnsureWindowsVersionDetected_00c73770(self) -- GameCODA one-time guard: run the Windows-version probe DetectWindowsVersion() 0x00C73660 unless its
//   done flag is set, then return the object handed in (ecx in, eax out: a libpk init helper that passes its object through)
// Leaf: ?EnsureWindowsVersionDetected_00c73770@@YIPAXPAX@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C73770, 21 bytes, __fastcall, plain `ret`. Descriptive name (libpk / GameCODA, no PDB): the landed
//   CDefString::SetDefaultString label is a propagation; the callee is the landed ?DetectWindowsVersion@@YAXXZ (no arguments: the ecx the
//   caller still holds is this helper's own argument). Naming wave 19: re-authored with the callee's real signature (byte-identical).
extern "C" unsigned char g_defStringInit;   // 0x0143CCC0: the probe's done flag (the landed 0x00C73660 row's bound spelling; really "version probe done")
void __cdecl DetectWindowsVersion();        // 0x00C73660

void* __fastcall EnsureWindowsVersionDetected_00c73770(void* self)
{
    if (!g_defStringInit)
        DetectWindowsVersion();
    return self;
}
