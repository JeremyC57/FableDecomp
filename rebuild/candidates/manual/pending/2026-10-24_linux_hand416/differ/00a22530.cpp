// GFFastArcCos(float cosine) -- arc cosine as a fraction of a full turn, linearly interpolated in the 1025-step acos table
// Grade: DIFFER(72v88) -- retail_asm candidate: retail rounds with a bare `fld [esp+4] ; fistp dword [esp]` on the ((c+1)*512.5 - 0.5) copy (inline asm in the engine's float-to-long helper); C++ `(long)x` calls __ftol2 and keeps the scaled value in st(0) instead of storing it back to its argument slot. next: retail_asm with the landed float-to-long helper form (a __forceinline FloatToLong(float value) holding the bare fld/fistp; see rebuild/src/compiled/00/af/NParticleEngineCPSCLight_SmoothFadeValue_00af9000.cpp)
// Leaf: ?GFFastArcCos@@YIMM@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A22530, 88 bytes, `ret 4` (__fastcall). No direct callers (inlined at its users).
//   Name from the FableWin PDB (`?GFFastArcCos@@YIMM@Z`); the catalog's "Math_InterpolateTableLookup" is descriptive.
// Globals: arc-cosine table 0x013CB530 (1025 floats, acos(k*2/1025 - 1), filled at 0x00A0DBF8 through the _CIacos
//   import), 512.5f 0x0129CD1C, 1.0f 0x0122DED8, 0.5f 0x0122F59C, 1/(2pi) 0x01237F08 (the result is in turns).
// Shape: no index mask (the input is assumed within [-1, 1]); the lerp result is scaled by 1/(2pi).
extern float g_ArcCosTable[1026];       // 0x013CB530: acos(k*2/1025 - 1), k = 0..1024

float __fastcall GFFastArcCos(float cosine)
{
    cosine = (cosine + 1.0f) * 512.5f;
    long i = (long)(cosine - 0.5f);
    float frac = cosine - (float)i;
    return ((1.0f - frac) * g_ArcCosTable[i] + frac * g_ArcCosTable[i + 1]) * 0.15915494f;
}
