// GFFastArcCos(float value) -- arc cosine as a fraction of a full turn, linearly interpolated in the 1026-entry arc-cosine table
// Leaf: ?GFFastArcCos@@YIMM@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A22530, 88 bytes, __fastcall with the float on the stack, `ret 4`. No direct caller in
//   Fable.exe (expanded at its users); this is the out-of-line copy of the maths-header inline.
// Retail-asm: 0x00A2254D `fstp dword ptr [esp+4]` / 0x00A22551 `fld dword ptr [esp+4]` / 0x00A22555 `fistp dword ptr [esp]` -- the
//   engine's inline-assembly float-to-long helper, through the shared engine/GFFloatToLongNear.h (a bare dword fistp: `(long)x`
//   gives __ftol2, /QIfist `fistp qword`). This row carries no asm of its own.
// Identity: FableWin.pdb ?GFFastArcCos@@YIMM@Z (0x02F30710) / Ego_d.pdb GFFastArcCos (debug length 443, a header: file id 408).
// Shape: Ego_d.pdb's line table (lines 234..243) and locals (value; pos, index, interp, result), statement for statement: 235 pos
//   (24 debug bytes), 236 the nearest-integer index (27), 237 interp (9), 238 a debug-only assert (283), 239 the interpolation (35),
//   241 `result *= 1 / 2pi` (12), 242 the return (3). One text with the copies the animation rows expand (0x00A4D770, 0x00A4D920,
//   0x00A52760, 0x00A53860, 0x00A4C1F0).
// Inferred: the float cast in `float pos = (float)((value + 1.0f) * 512.5f);`. Ego_d's line 235 is 24 bytes where the uncast
//   statement is 18 at /Od, and row 0x00A4D770 CalcLocalTransformationsRelative needs it (measurements in that row's header).
//   Byte-neutral here: this row is RELOCATION_MATCH with and without the cast.
// Re-port (audit 2026-11-20, with row 0x00A4D770): the landed text was the October one-expression form (`cosine = (cosine + 1.0f)
//   * 512.5f; ... return (...) * 0.15915494f;`) on the made-up table name g_ArcCosTable; the table is Ego_d.pdb's public
//   ?GFastArcCosTable@@3PAMA.
// Globals: GFastArcCosTable 0x013CB530 (acos(k * 2 / 1025 - 1), filled at 0x00A0DBF8); 1.0f 0x0122DED8, 512.5f 0x0129CD1C, 0.5f
//   0x0122F59C, 1 / 2pi 0x01237F08.
#include "fable_compiler.h"
#include "engine/GFFloatToLongNear.h"
extern float GFastArcCosTable[1026];    // 0x013CB530 ?GFastArcCosTable@@3PAMA (acos(k * 2 / 1025 - 1), k = 0..1025)

float __fastcall GFFastArcCos(float value)
{
    float pos = (float)((value + 1.0f) * 512.5f);
    long index = GFFloatToLongNear(pos - 0.5f);
    float interp = pos - index;
    float result = (1.0f - interp) * GFastArcCosTable[index] + interp * GFastArcCosTable[index + 1];
    result *= 0.15915494f;
    return result;
}