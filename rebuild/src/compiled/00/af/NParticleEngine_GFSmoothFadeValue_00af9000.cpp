// NParticleEngine::GFSmoothFadeValue(float t) -- cosine-table smooth fade: 0.5 - 0.5 * lerp(cos[i], cos[i+1]) over the 1024-entry table
// Leaf: ?GFSmoothFadeValue@NParticleEngine@@YIMM@Z
// Flags: /O2 /Oy /W3
// Retail-asm: 0x00AF9021 `fld dword ptr [esp+4]` + 0x00AF9025 `fistp dword ptr [esp]` -- the engine's inline-assembly float-to-long helper: the argument (t - 0.5) is stored to the helper's by-value parameter copy and reloaded for a bare dword fistp into a frame word; VC7.1 C++ `(long)x` calls __ftol2 and /QIfist emits `fistp qword` and folds the 0.5*1024 constant, so the dword fistp is the original's inline-asm.
// Helper (2026-11-03 asm twins): the retail inline-asm block is the shared engine/GFFloatToLongNear.h (two-path: the byte-matching asm, and a FABLE_PORTABLE C++ twin); this row carries no asm of its own.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AF9000, length 99 bytes (identical bodies at 0x00B05E10, 0x00B1AD90).
// Identity (naming wave 13, owner decision 2026-11-01): the free NParticleEngine::GFSmoothFadeValue (FableWin.pdb, between
//   the CParticleSystem accessors and CPSCRenderSprite, as retail 0x00AF9000 sits after ScaleParticleDimensions); the two
//   identical bodies are CPSCSingleSprite::SmoothFadeValue (IBE) and CPSCLight::SmoothFadeValue (QBE). Was catalogued as
//   CPSCLight::SmoothFadeValue; the body never reads `this`, so the __fastcall free form gives the same bytes.
// Globals: cosine table g_CosTable @ 0x013CD550 (1025 floats, built at start-up); constants 0.5f @ 0x0122F59C,
//          1024.0f @ 0x01230010, 1.0f @ 0x0122DED8.
// Shape: t scaled in place in its argument slot (`fst [esp+0Ch]`); `fild` of the rounded index gives the fraction;
// the index is masked to 10 bits for the two table reads.
namespace NParticleEngine {
float __fastcall GFSmoothFadeValue(float t);   // FableWin.pdb ?GFSmoothFadeValue@NParticleEngine@@YIMM@Z
}
extern float g_CosTable[1025];          // 0x013CD550
// the engine's inline-assembly float-to-long helper: VC7.1 expands a __forceinline function that contains inline-asm in place
#include "engine/GFFloatToLongNear.h"
float __fastcall NParticleEngine::GFSmoothFadeValue(float t)
{
    t = t * 0.5f;
    t = t * 1024.0f;
    long i = GFFloatToLongNear(t - 0.5f);
    float frac = t - (float)i;
    i &= 0x3ff;
    return 0.5f - ((1.0f - frac) * g_CosTable[i] + frac * g_CosTable[i + 1]) * 0.5f;
}