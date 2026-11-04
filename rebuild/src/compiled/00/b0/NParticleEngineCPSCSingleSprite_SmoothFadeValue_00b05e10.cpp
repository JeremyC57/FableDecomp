// NParticleEngine::CPSCSingleSprite::SmoothFadeValue(float t) -- cosine-table smooth fade: 0.5 - 0.5 * lerp(cos[i], cos[i+1]) over the 1024-entry table
// Leaf: ?SmoothFadeValue@CPSCSingleSprite@NParticleEngine@@IBEMM@Z
// Flags: none
// Retail-asm: 0x00B05E31 `fld dword ptr [esp+4]` + 0x00B05E35 `fistp dword ptr [esp]` -- the engine's inline-assembly float-to-long helper
//   GFFloatToLongNear (shared engine/GFFloatToLongNear.h): (t - 0.5) stored to the helper's by-value parameter copy and reloaded for a
//   bare dword fistp; VC7.1 C++ `(long)x` calls __ftol2 and /QIfist emits `fistp qword`. This row carries no asm of its own.
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B05E10, 99 bytes, thiscall `ret 4`; byte-identical body to the landed
//   NParticleEngine::GFSmoothFadeValue 0x00AF9000 and CPSCLight::SmoothFadeValue 0x00B1AD90 (per-TU copies; the body never reads `this`).
// Identity: FableWin.pdb ?SmoothFadeValue@CPSCSingleSprite@NParticleEngine@@IBEMM@Z (protected const); callers 0x00B08D77 / 0x00B08DA9 in the
//   CPSCSingleSprite TU.
// Globals: cosine table g_CosTable @ 0x013CD550 (1025 floats); constants 0.5f 0x0122F59C, 1024.0f 0x01230010, 1.0f 0x0122DED8.
#include "engine/GFFloatToLongNear.h"
extern float g_CosTable[1025];          // 0x013CD550
namespace NParticleEngine {
class CPSCSingleSprite {
protected:
    float SmoothFadeValue(float t) const;
};
float CPSCSingleSprite::SmoothFadeValue(float t) const
{
    t = t * 0.5f;
    t = t * 1024.0f;
    long i = GFFloatToLongNear(t - 0.5f);
    float frac = t - (float)i;
    i &= 0x3ff;
    return 0.5f - ((1.0f - frac) * g_CosTable[i] + frac * g_CosTable[i + 1]) * 0.5f;
}
}