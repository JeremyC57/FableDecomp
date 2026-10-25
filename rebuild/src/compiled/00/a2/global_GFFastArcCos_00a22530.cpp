// GFFastArcCos(float cosine) -- arc cosine as a fraction of a full turn, linearly interpolated in the 1025-step acos table
// Grade: RELOCATION_MATCH (1 grade) -- rasm1 retail_asm: C++ body + a minimal 2-instruction __asm (no control flow) + the Retail-asm evidence line
// Retail-asm: 0x00A2254D `fstp [esp+4]` / 0x00A22551 `fld dword ptr [esp+4]` + 0x00A22555 `fistp dword ptr [esp]` -- the engine's inline-assembly float-to-long helper (the (c+1)*512.5 - 0.5 argument stored to its by-value copy in the frame and reloaded, a bare dword fistp into the result word; same helper as GFFastSin 0x00A147E0). VC7.1 C++ `(long)x` calls __ftol2 and /QIfist (RTM 3077 and QFE 4035, with /Op, /Og or /G5-/G7) emits `fistp qword` into an 8-byte slot, /arch:SSE2 a cvttss2si: no C/C++ form gives the bare dword fistp, so it is the original's `__asm`.
// Leaf: ?GFFastArcCos@@YIMM@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A22530, 88 bytes, `ret 4` (__fastcall). No direct callers (inlined at its users).
//   Name from the FableWin PDB (`?GFFastArcCos@@YIMM@Z`); the catalog's "Math_InterpolateTableLookup" is descriptive.
// Globals: arc-cosine table 0x013CB530 (1025 floats, acos(k*2/1025 - 1), filled at 0x00A0DBF8 through the _CIacos
//   import), 512.5f 0x0129CD1C, 1.0f 0x0122DED8, 0.5f 0x0122F59C, 1/(2pi) 0x01237F08 (the result is in turns).
// Shape: no index mask (the input is assumed within [-1, 1]); the lerp result is scaled by 1/(2pi).
extern float g_ArcCosTable[1026];       // 0x013CB530: acos(k*2/1025 - 1), k = 0..1024

// the engine's inline-assembly float-to-long helper (round to nearest, the FPU default); expanded in place as a __forceinline
__forceinline long FloatToLong(float value)
{
    long result;
    __asm fld value
    __asm fistp result
    return result;
}

float __fastcall GFFastArcCos(float cosine)
{
    cosine = (cosine + 1.0f) * 512.5f;
    long i = FloatToLong(cosine - 0.5f);
    float frac = cosine - (float)i;
    return ((1.0f - frac) * g_ArcCosTable[i] + frac * g_ArcCosTable[i + 1]) * 0.15915494f;
}