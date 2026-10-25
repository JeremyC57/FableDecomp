// _global::AudioSystem_SetOutputFlags(unsigned long flags) -- libpk audio API: reports error -3 (invalid flags) for any bit in 0xFFF8C000, for 0x2000 combined with 0x80 and for 0x2000 combined with 0x1000, then hands the flags to the global audio system anyway
// Leaf: AudioSystem_SetOutputFlags
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C35EC0, 89 bytes, __stdcall `ret 4`. Descriptive name (no PDB covers libpk):
//   the catalog's `InvalidatePickupDisplay` is propagated. Twin of 0x00C33DC0 (mask 0xFE0FC000, setter 0x00C3DB80).
// Callees: the global audio system (0x0143BB58)'s SetError @ 0x00C3D090 (__fastcall, records the first error code
//   and calls the +0x7C error hook) and its flag setter @ 0x00C3DA60 (__fastcall, one word).
// Shape: `flags & 0x2000` is computed once into esi and reused by the third test; bit 7 is tested as `test bl,bl ; jns`.
struct CAudioSystem {
    void SetError(long code);              // 0x00C3D090
    void SetOutputFlags(unsigned long flags); // 0x00C3DA60
};
extern CAudioSystem g_audioSystem;          // 0x0143BB58

void __stdcall AudioSystem_SetOutputFlags(unsigned long flags)
{
    if (flags & 0xFFF8C000)
        g_audioSystem.SetError(-3);
    if ((flags & 0x2000) && (flags & 0x80))
        g_audioSystem.SetError(-3);
    if ((flags & 0x2000) && (flags & 0x1000))
        g_audioSystem.SetError(-3);
    g_audioSystem.SetOutputFlags(flags);
}