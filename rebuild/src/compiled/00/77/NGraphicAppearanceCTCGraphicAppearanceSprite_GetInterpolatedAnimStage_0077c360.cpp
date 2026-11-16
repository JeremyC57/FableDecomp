// NGraphicAppearance::CTCGraphicAppearanceSprite::GetInterpolatedAnimStage(float t) const -- interpolate between LastAnimStage and AnimStage (wrapping through 1 when AnimStage lies below it) and clamp to [0, 1 - epsilon]
// Leaf: ?GetInterpolatedAnimStage@CTCGraphicAppearanceSprite@NGraphicAppearance@@IBEMM@Z
// Name-fix NF38: Ego_r.pdb / FableWin.pdb ?GetInterpolatedAnimStage@CTCGraphicAppearanceSprite@NGraphicAppearance@@IBEMM@Z (protected const),
// declared by engine/NGraphicAppearance_CTCGraphicAppearanceSprite.h (FABLE_RETAIL_CTCGRAPHICAPPEARANCESPRITE_PASS22): the two fractions
// are AnimStage +0x1C and LastAnimStage +0x20; the landed caller 0x0077C4D0 binds it. Was the descriptive stand-in CLoopedRange::GetFraction.
// Naming wave 18: the shared const float 1e-4f at 0x0129BA3C is ?DELTA_NOTIONAL_ZERO@@3MB (raw Ego_r public + typed data record; was EPSILON).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0077C360, 108 bytes (+ int3 padding), thiscall `ret 4`, result in st(0).
// Caller: NGraphicAppearance::CTCGraphicAppearanceSprite vtable slot 0 @ 0x0077C4D0 (`lea ecx,[esi-0Ch]`; the result
//   times an integer frame count picks the sprite frame).
// Constants: 1.0f @ 0x0122DED8, 0.0f @ 0x0122DEDC (literal pool), epsilon 0.0001f @ 0x0129BA3C (a global float: the
//   `1 - epsilon` bound is computed at run time and spilled to the dead `t` argument slot).
// Layout: the sprite's two fractions at +0x1C (AnimStage, was End) and +0x20 (LastAnimStage, was Start).
// Shape: `if (AnimStage > LastAnimStage) r = (AnimStage - LastAnimStage) * t + LastAnimStage; else { s = 1 - LastAnimStage; r = s + (AnimStage - s) * t; }` then the
//   clamp `if (r < 0) return 0; max = 1 - epsilon; if (r > max) return max; return r;`.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCGRAPHICAPPEARANCESPRITE_PASS22
#include "engine/NGraphicAppearance_CTCGraphicAppearanceSprite.h"
extern const float DELTA_NOTIONAL_ZERO;           // 0x0129BA3C (1e-4f)

float NGraphicAppearance::CTCGraphicAppearanceSprite::GetInterpolatedAnimStage(float t) const
{
    float r;
    if (AnimStage > LastAnimStage)
        r = (AnimStage - LastAnimStage) * t + LastAnimStage;
    else {
        float s = 1.0f - LastAnimStage;
        r = s + (AnimStage - s) * t;
    }
    if (r < 0.0f)
        return 0.0f;
    const float maxFraction = 1.0f - DELTA_NOTIONAL_ZERO;
    if (r > maxFraction)
        return maxFraction;
    return r;
}
