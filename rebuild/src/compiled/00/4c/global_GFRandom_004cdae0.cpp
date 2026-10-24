// GFRandom(unsigned long range, unsigned long* seed) -- advance the caller's LCG seed (x * 0x24A1 + 0x24DF, then rotate right 13) and return it modulo range (0 for an empty range)
// Leaf: ?GFRandom@@YIKKPAK@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004CDAE0, length 48 bytes (47 + int3 padding), __fastcall (ecx = range,
// edx = seed), plain `ret`.
// Identity: Ego_r.pdb `?GFRandom@@YIKKPAK@Z` (unsigned long __fastcall GFRandom(unsigned long, unsigned long*)); 20+
// callers (0x004CEDD6, 0x0057B83D, 0x006D5638, ...). The catalog "GFRandom" is right.
// Callee: GFROR13(unsigned long) @ 0x00497890 (__fastcall, landed name).
// Shape: the stepped seed is stored before the rotate and the rotated value stored again; the modulo is unsigned (`div`)
// on the returned value, guarded by `range != 0` written as the taken arm (the `== 0` early return lays the zero
// exit out first).
unsigned long __fastcall GFROR13(unsigned long value);   // 0x00497890

unsigned long __fastcall GFRandom(unsigned long range, unsigned long* seed)
{
    *seed = *seed * 0x24a1 + 0x24df;
    *seed = GFROR13(*seed);
    if (range != 0)
        return *seed % range;
    return 0;
}