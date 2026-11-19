// `anonymous namespace'::GetDamageMultiplierFromThingAtBuildUpTime(const CThing& thing, CDefPointer<const CHeroCombatDef> pdef, float buildup_time) -- the hero's ranged damage multiplier after building up a shot for buildup_time: (stat speed * hero stat parameter + 1) * (tanh(t * nominal speed) * nominal scale + (t * super speed)^2 * super scale)
// Leaf: ?GetDamageMultiplierFromThingAtBuildUpTime@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00789CE0, 175 bytes, __fastcall (thing in ecx; the CDefPointer by value and the float on
//   the stack, released by the callee), `ret 8`.
// Name: Ego_r.pdb tc_hero_ranged_attacks.obj #20 / FableWin.pdb #1 ?GetDamageMultiplierFromThingAtBuildUpTime@?A0x0524c104@@YIMABVCThing@@V?$CDefPointer@$$CBVCHeroCombatDef@@@@M@Z
//   (anonymous-namespace hash varies with the file path, so the Leaf is a prefix).
// Callees: CHeroAbilityStatWeight::GetParameter 0x00851BD0, the MSVCR71 _CItanh import thunk 0x00BFECE2, the def's virtual Delete (slot 1) on the last release;
//   1.0f 0x0122DED8.
// Source: Ego_d.pdb locals thing, pdef, buildup_time, ptc_hero_exp, damage_m, fractional_strength, comp2, comp1; the FableWin debug body
//   0x024F4D30 gives the statements (PeekTC<const CTCHeroExperience>, GetParameter, comp1 = GFTanh(..) * scale, comp2 = GFSqr(..) * scale,
//   damage_m = (stat * fractional + 1) * (comp1 + comp2); its debug-only null ASSERT is dropped). Shared headers: engine/CHeroCombatDef.h
//   (FABLE_RETAIL_FIRE_RANGED_WEAPON), CDefPointer.h, CTCHeroExperience.h, C3DVector.h's GFSqr `(n*n)` (FABLE_RETAIL_C3DVECTOR_NORMALISE_
//   SQUARE_HELPER + _SQUARE_GROUPED_PASS23: the plain GFWorldDistance.h text lets c2 reassociate (t*s)*t), and
//   FABLE_RETAIL_CDEFPOINTER_RELEASE_POINTEE (the may-alias edge keeps the count store after the last load through the def, 0x00789D6E).
#define FABLE_RETAIL_FIRE_RANGED_WEAPON
#define FABLE_RETAIL_CDEFPOINTER_RELEASE_POINTEE
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_NORMALISE_SQUARE_HELPER
#define FABLE_RETAIL_C3DVECTOR_SQUARE_GROUPED_PASS23
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CTCHEROEXPERIENCE
#include <math.h>
#include "engine/C3DVector.h"
#include "engine/CThing.h"
#include "engine/CDefPointer.h"
#include "engine/CHeroCombatDef.h"
#include "engine/CTCHeroExperience.h"

// FableWin.pdb ?GFTanh@@YINN@Z (debug 0x024F4F60; no shared header; the landed 0x008FCC60 row defines the same inline text); retail
// expands it to the MSVCR71 _CItanh import thunk 0x00BFECE2.
inline double FABLE_FASTCALL GFTanh(double n) { return tanh(n); }
namespace {
float FABLE_FASTCALL GetDamageMultiplierFromThingAtBuildUpTime(const CThing& thing, CDefPointer<const CHeroCombatDef> pdef, float buildup_time)
{
    float fractional_strength = 1.0f;
    const CTCHeroExperience* ptc_hero_exp;
    if (thing.PeekTC(&ptc_hero_exp))
        fractional_strength = pdef->RangedDamageMultiplierWeighting.GetParameter(*ptc_hero_exp);
    float comp1 = GFTanh(buildup_time * pdef->RangedDamageMultiplierSpeedNominalBuildup) * pdef->RangedDamageMultiplierScaleNominalBuildup;
    float comp2 = GFSqr(buildup_time * pdef->RangedDamageMultiplierSpeedSuperBuildup) * pdef->RangedDamageMultiplierScaleSuperBuildup;
    float damage_m = (pdef->RangedDamageMultiplierSpeedWithStatParameter * fractional_strength + 1.0f) * (comp1 + comp2);
    return damage_m;
}
}
// Emission: the file-static has no caller in this TU (retail callers CTCHeroRangedAttacks 0x0078A036 / 0x0078A883); a file-scope pointer keeps it.
float (FABLE_FASTCALL* keep_00789ce0)(const CThing&, CDefPointer<const CHeroCombatDef>, float) = GetDamageMultiplierFromThingAtBuildUpTime;