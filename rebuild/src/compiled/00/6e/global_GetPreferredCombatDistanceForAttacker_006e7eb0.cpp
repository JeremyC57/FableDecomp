// GetPreferredCombatDistanceForAttacker(const CThingCreatureBase& defender, const CThingCreatureBase& attacker) -- (anonymous namespace) the distance an attacker prefers to keep from a defender: the defender's physics radius plus the attacker's combat-def preferred combat distance, or the midpoint of its front middle / far limits when that is (near) zero
// Inline-budget: none needed: a leaf with no inline device. Its callers expand it through VC7.1's own /O2 auto-inlining (0x006F2DE0 at 0x006F2DE8..0x006F2E2D); the anonymous-namespace function keeps external linkage under VC7.1, so the compiler also emits this unreferenced out-of-line copy. GetPhysicsTC / GetRadius are plain inline header members.
// Leaf: ?GetPreferredCombatDistanceForAttacker@
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x006E7EB0, 57 bytes, `__fastcall` (ecx = defender, edx = attacker), `push ecx` slot for separation,
//   float in st0, plain `ret`. TU combat_manager.obj (anonymous namespace ?A0xd8b789b9 in Ego_r / Ego_d). Unreferenced: no E8/E9
//   caller and no absolute reference in retail; its callers (0x006F2DE0 and the 0x006E89B0 placement code) expand it.
// Identity: Ego_r.pdb / FableWin.pdb ?GetPreferredCombatDistanceForAttacker@?A0xd8b789b9@@YIMABVCThingCreatureBase@@0@Z (YI = __fastcall).
//   Ego_d.pdb parameters (defender, attacker), locals separation, distance (type 0x40 float), combat_def; block { far_range, middle_range }.
// Callees: CThingCreatureBase::GetCombatTypeDef 0x00666D10 (on the attacker); globals DELTA_NOTIONAL_ZERO 0x0129BA3C, 0.5f 0x0122F59C.
// Layout (retail): CThing PhysicsTC +0x60, CTCPhysicsBase Radius +0x34; CCombatTypeDef FrontFarLimit +0x6C, FrontMiddleLimit +0x70,
//   PreferredCombatDistance +0x80.
// Inferred: the text is the landed 0x006F2DE0 file's own helper (verbatim), itself the landed 0x006E89B0 helper's text with Ego_d's float
//   distance, Ego_d's parameter order and the PDB's __fastcall.
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGCREATUREBASE_GETCOMBATTYPEDEF
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CCOMBATTYPEDEF
#define FABLE_RETAIL_C3DVECTOR
#include "fable_compiler.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CTCPhysicsBase.h"
#include "engine/CCombatTypeDef.h"
namespace {
float __fastcall GetPreferredCombatDistanceForAttacker(const CThingCreatureBase& defender, const CThingCreatureBase& attacker)
{
    float separation = defender.GetPhysicsTC()->GetRadius();
    const CCombatTypeDef& combat_def = attacker.GetCombatTypeDef();
    float distance = combat_def.PreferredCombatDistance;
    if (distance < DELTA_NOTIONAL_ZERO) {
        float far_range = combat_def.FrontFarLimit;
        float middle_range = combat_def.FrontMiddleLimit;
        distance = (middle_range + far_range) / 2.0f;
    }
    separation += distance;
    return separation;
}
}