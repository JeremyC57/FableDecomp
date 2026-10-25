// _global::AudioSystem_Create(unsigned long flags, const void* desc) -- libpk audio API: initialises the global audio system from the caller's descriptor and returns the system, or 0 when the initialisation fails
// Leaf: AudioSystem_Create
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C334D0, 32 bytes, __stdcall `ret 8`. Descriptive name (no PDB covers libpk):
//   the catalog's `CTCHeroStats_GetVectorArraysIfInitialized` is propagated. Caller: the landed StartTimer 0x00C1C1A0
//   (`RegisterDescriptor(0, &desc)`), which builds the 28-byte descriptor first.
// Callee: the global audio system (0x0143BB58)'s initialiser @ 0x00C3DFD0 (__fastcall, two words, returns a bool).
// Shape: `ok ? &g_audioSystem : 0` -> `neg al ; sbb eax,eax ; and eax, offset g_audioSystem`.
struct CAudioSystem {
    bool Initialise(unsigned long flags, const void* desc);   // 0x00C3DFD0
};
extern CAudioSystem g_audioSystem;                            // 0x0143BB58

CAudioSystem* __stdcall AudioSystem_Create(unsigned long flags, const void* desc)
{
    return g_audioSystem.Initialise(flags, desc) ? &g_audioSystem : 0;
}