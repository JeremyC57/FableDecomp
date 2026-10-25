// GFFastSin(float angle) -- sine of a fraction of a full turn, linearly interpolated in the 1024-step cosine table
// Grade: RELOCATION_MATCH (1 grade) -- rasm1 retail_asm: C++ body + a minimal 2-instruction __asm (no control flow) + the Retail-asm evidence line
// Retail-asm: 0x00A147F7 `fstp [esp+4]` / 0x00A147FB `fld dword ptr [esp+4]` + 0x00A147FF `fistp dword ptr [esp]` -- the engine's inline-assembly float-to-long helper (the angle*1024 - 0.5 argument stored to its by-value copy in the frame and reloaded, a bare dword fistp into the result word), the same helper as the landed retail_asm 0x00AF9000/0x00B10220; VC7.1 C++ `(long)x` calls __ftol2 and /QIfist (RTM 3077 and QFE 4035, with /Op, /Og or /G5-/G7) emits `fistp qword` into an 8-byte slot, /arch:SSE2 a cvttss2si: no C/C++ form gives the bare dword fistp, so it is the original's `__asm`.
// Leaf: ?GFFastSin@@YIMM@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A147E0, 86 bytes, `ret 4` (__fastcall; the float travels on the stack). No direct
//   callers (the body is inlined everywhere: GetRandomUnitVector 0x00A13EB0 and ~45 other table readers). Name from the
//   FableWin PDB (`?GFFastSin@@YIMM@Z`); the catalog's "Math_InterpolateSineLookup" is descriptive.
// Globals: cosine table 0x013CD550 (1025 floats, cos(k*pi/1024 * 2), filled by the start-up loop at 0x00A0DBB0 with fcos);
//   1024.0f 0x01230010, 0.5f 0x0122F59C, 1.0f 0x0122DED8.
// Shape: angle scaled in place in its argument slot (`fst [esp+0Ch]`), index rounded by fistp, fraction from fild, index
//   shifted a quarter turn back (-256: cos(a - 1/4 turn) = sin a) and masked to 10 bits; lerp of two table entries.
extern float g_CosTable[1025];          // 0x013CD550: cos(2*pi*k/1024), k = 0..1024

// the engine's inline-assembly float-to-long helper (round to nearest, the FPU default); expanded in place as a __forceinline
__forceinline long FloatToLong(float value)
{
    long result;
    __asm fld value
    __asm fistp result
    return result;
}

float __fastcall GFFastSin(float angle)
{
    angle = angle * 1024.0f;
    long i = FloatToLong(angle - 0.5f);
    float frac = angle - (float)i;
    i = (i - 256) & 0x3ff;
    return (1.0f - frac) * g_CosTable[i] + frac * g_CosTable[i + 1];
}