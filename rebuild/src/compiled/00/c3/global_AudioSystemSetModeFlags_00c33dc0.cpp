// _global::AudioSystem_SetModeFlags(unsigned long flags) -- libpk audio API: reports error -3 (invalid flags) for any bit in 0xFE0FC000, for 0x2000 combined with 0x80 and for 0x2000 combined with 0x1000, then hands the flags to the global audio system anyway
// Leaf: AudioSystem_SetModeFlags
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C33DC0, 89 bytes, __stdcall `ret 4`. Descriptive name (no PDB covers libpk):
//   the catalog's `PlayerGui_OnInventoryChange` is propagated. Twin of 0x00C35EC0 (mask 0xFFF8C000, setter 0x00C3DA60).
// Callees: the global audio system (0x0143BB58)'s SetError @ 0x00C3D090 (__fastcall, records the first error code
//   and calls the +0x7C error hook) and its flag setter @ 0x00C3DB80 (__fastcall, one word).
// Shape: `flags & 0x2000` is computed once into esi and reused by the third test; bit 7 is tested as `test bl,bl ; jns`.
struct CAudioSystem {
    void SetError(long code);              // 0x00C3D090
    void SetModeFlags(unsigned long flags); // 0x00C3DB80
};
extern CAudioSystem g_audioSystem;          // 0x0143BB58

void __stdcall AudioSystem_SetModeFlags(unsigned long flags)
{
    if (flags & 0xFE0FC000)
        g_audioSystem.SetError(-3);
    if ((flags & 0x2000) && (flags & 0x80))
        g_audioSystem.SetError(-3);
    if ((flags & 0x2000) && (flags & 0x1000))
        g_audioSystem.SetError(-3);
    g_audioSystem.SetModeFlags(flags);
}