// GFFastCos(float angle) -- cosine of a fraction of a full turn, linearly interpolated in the 1024-step cosine table
// Grade: DIFFER(65v81) -- retail_asm candidate: retail rounds with a bare `fld [esp+4] ; fistp dword [esp]` on the (angle*1024 - 0.5) copy (inline asm in the engine's float-to-long helper); C++ `(long)x` calls __ftol2 and keeps the scaled angle in st(0) instead of storing it back to its argument slot. next: retail_asm with the landed float-to-long helper form (a __forceinline FloatToLong(float value) holding the bare fld/fistp; see rebuild/src/compiled/00/af/NParticleEngineCPSCLight_SmoothFadeValue_00af9000.cpp)
// Leaf: ?GFFastCos@@YIMM@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A14840, 81 bytes, `ret 4` (__fastcall; the float travels on the stack). No direct
//   callers (inlined everywhere it is used). Name from the FableWin PDB (`?GFFastCos@@YIMM@Z`); the catalog's
//   "Math_InterpolateSineLookup_Alt" is wrong in kind: the table holds cosines and this variant has no quarter-turn shift.
// Globals: cosine table 0x013CD550 (see 0x00A0DBB0), 1024.0f 0x01230010, 0.5f 0x0122F59C, 1.0f 0x0122DED8.
// Shape: identical to GFFastSin 0x00A147E0 without the `add eax,-256`.
extern float g_CosTable[1025];          // 0x013CD550: cos(2*pi*k/1024), k = 0..1024

float __fastcall GFFastCos(float angle)
{
    angle = angle * 1024.0f;
    long i = (long)(angle - 0.5f);
    float frac = angle - (float)i;
    i &= 0x3ff;
    return (1.0f - frac) * g_CosTable[i] + frac * g_CosTable[i + 1];
}
