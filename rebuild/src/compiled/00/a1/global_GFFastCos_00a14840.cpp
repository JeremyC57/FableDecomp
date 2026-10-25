// GFFastCos(float angle) -- cosine of a fraction of a full turn, linearly interpolated in the 1024-step cosine table
// Grade: RELOCATION_MATCH (1 grade) -- rasm1 retail_asm: C++ body + a minimal 2-instruction __asm (no control flow) + the Retail-asm evidence line
// Retail-asm: 0x00A14857 `fstp [esp+4]` / 0x00A1485B `fld dword ptr [esp+4]` + 0x00A1485F `fistp dword ptr [esp]` -- the engine's inline-assembly float-to-long helper (the angle*1024 - 0.5 argument stored to its by-value copy in the frame and reloaded, a bare dword fistp into the result word; same helper as GFFastSin 0x00A147E0). VC7.1 C++ `(long)x` calls __ftol2 and /QIfist (RTM 3077 and QFE 4035, with /Op, /Og or /G5-/G7) emits `fistp qword` into an 8-byte slot, /arch:SSE2 a cvttss2si: no C/C++ form gives the bare dword fistp, so it is the original's `__asm`.
// Leaf: ?GFFastCos@@YIMM@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A14840, 81 bytes, `ret 4` (__fastcall; the float travels on the stack). No direct
//   callers (inlined everywhere it is used). Name from the FableWin PDB (`?GFFastCos@@YIMM@Z`); the catalog's
//   "Math_InterpolateSineLookup_Alt" is wrong in kind: the table holds cosines and this variant has no quarter-turn shift.
// Globals: cosine table 0x013CD550 (see 0x00A0DBB0), 1024.0f 0x01230010, 0.5f 0x0122F59C, 1.0f 0x0122DED8.
// Shape: identical to GFFastSin 0x00A147E0 without the `add eax,-256`.
extern float g_CosTable[1025];          // 0x013CD550: cos(2*pi*k/1024), k = 0..1024

// the engine's inline-assembly float-to-long helper (round to nearest, the FPU default); expanded in place as a __forceinline
__forceinline long FloatToLong(float value)
{
    long result;
    __asm fld value
    __asm fistp result
    return result;
}

float __fastcall GFFastCos(float angle)
{
    angle = angle * 1024.0f;
    long i = FloatToLong(angle - 0.5f);
    float frac = angle - (float)i;
    i &= 0x3ff;
    return (1.0f - frac) * g_CosTable[i] + frac * g_CosTable[i + 1];
}