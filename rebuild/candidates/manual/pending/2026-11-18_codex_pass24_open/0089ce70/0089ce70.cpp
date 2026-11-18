// CGameScriptInterface::IsThingAwareOfOtherThingInAnyWay(const CScriptThing& thing, const CScriptThing& other_thing) const -- whether other_thing senses thing in any way (CanThingBe_Sensed_ByOtherThing with sense 5: the perceived thing's awareness, else within the larger of the smell / hearing radius, else within the extended sight radius and the field-of-view cone)
// Grade: DIFFER(614v616) -- The inlined sensory check emits a compare and pop sequence that differs at 0x0089D096.
// Leaf: ?IsThingAwareOfOtherThingInAnyWay@CGameScriptInterface@@UBE_NABVCScriptThing@@0@Z
// Flags: /d2inlS100
// Retail: Fable.exe (TLC, VC7.1) 0x0089CE70, 616 bytes, thiscall `ret 8`.
// Identity / shape: Ego_d game_script_interface.obj: a 31-byte debug body (like its four siblings CanThingBe_Seen / NearlySeen / Smelled /
//   Heard 0x0089CD10..0x0089CD70, landed as `return CanThingBe_Sensed_ByOtherThing(thing, other, N);`), parameters thing / other_thing,
//   no locals. Retail is that call with sense 5 EXPANDED: the same-TU private CanThingBe_Sensed_ByOtherThing (Ego_r 0x008DB0B0; retail
//   out of line at 0x0089C910, itself a parked row) is defined above the wrapper here (codex pass23 draft text) and VC7.1 inlines it
//   into this wrapper only, folding the switch -- as retail does. The /d2inlS100 flag is the 0x0089C910 draft's own (radius magnitude
//   sites); it must become that row's natural text or an Inline-budget line before landing.
// Tried (e05, 13 grades): GFCos for the cone (worse), operand order dot > cos (worse), Ego_d's `const CTCPhysicsBase* ptc_physics` block
//   local in both sight cases before / after Normalise (dot moves ahead of GetFOV, worse), if/return false form. Next: fit the Sensed
//   helper texts fwsize flags (Normalise 43i v FableWin 48i, GFSqr 6i v 10i, Dot / GetSquaredMagnitude named results, GetTC 45i v 27i),
//   jointly with 0x0089C910.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_DOT_ORIGINAL_SCOPE_PASS23
#define FABLE_RETAIL_C3DVECTOR_SQUARED_MAGNITUDE_ORIGINAL_EXPR_PASS23
#define FABLE_RETAIL_C3DVECTOR_NORMALISE_SQUARE_HELPER
#define FABLE_RETAIL_C3DVECTOR_SQUARE_ORIGINAL_FORMAL
#define FABLE_RETAIL_C3DVECTOR_PACKED
#define FABLE_RETAIL_C3DVECTOR_ORIGINAL_COMPONENT_CTOR
#define FABLE_RETAIL_C3DVECTOR_ORIGINAL_COPY_FORMAL
#define FABLE_RETAIL_GFFASTONEOVERSQRT_ORIGINAL_UNION

#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_CTCPERCEIVETHING
#define FABLE_RETAIL_CTCPERCEIVETHING_AWARENESS
#define FABLE_RETAIL_CTCPERCEIVETHING_AWARE_CALLED
#define FABLE_RETAIL_SENSED_PASS23
#include "fable_string.h"
#include "engine/CThing.h"
#include "engine/CTCPhysicsBase.h"
#include "engine/CTCPerceiveThing.h"
#include "stlport/fable_containers.h"
#include "engine/CCountedPointer.h"
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
#include "engine/CGameScriptInterface.h"
#include <math.h>
#ifdef _MSC_VER
#pragma intrinsic(cos)
#endif

bool CGameScriptInterface::CanThingBe_Sensed_ByOtherThing(const CScriptThing& thing, const CScriptThing& other_thing, long sense) const
{
    if (!thing.IsAlive() || !other_thing.IsAlive())
        return false;
    CThing* p_see_er = other_thing.GetPThing();
    CThing* p_see_ee = thing.GetPThing();
    CTCPerceiveThing* tc_perception;
    if (p_see_er->GetTC(TCI_PERCEIVE_THING, &tc_perception)) {
        const CThing* p_perceived_thing = tc_perception->GetPerceivedThing();
        if (p_perceived_thing && p_perceived_thing == p_see_ee) {
            switch (sense) {
            case 1:
                return tc_perception->IsThingLineOfSight();
            case 2:
                return tc_perception->IsThingExtendedLineOfSight();
            case 3:
                return tc_perception->IsThingSmelt();
            case 4:
                return tc_perception->IsThingHeard();
            case 5:
                return tc_perception->IsAwareOfThing();
            }
        } else {
            switch (sense) {
            case 5: {
                float sense_radius = std::max(tc_perception->GetSmellRadius(), tc_perception->GetSoundRadius());
                C3DVector from_see_er_to_see_ee=p_see_ee->GetPos() - p_see_er->GetPos();
                if (from_see_er_to_see_ee.GetSquaredMagnitude() <= sense_radius * sense_radius)
                    return true;
            }
            case 2: {
                float sight_radius = tc_perception->GetExtendedSightRadius();
                C3DVector from_see_er_to_see_ee = p_see_ee->GetPos() - p_see_er->GetPos();
                if (from_see_er_to_see_ee.GetSquaredMagnitude() <= sight_radius * sight_radius) {
                    from_see_er_to_see_ee.Normalise();
                    return cos(tc_perception->GetFOV() * (double)6.2831855f) < p_see_er->GetPhysicsTC()->GetFacingVector().Dot(from_see_er_to_see_ee);
                }
                return false;
            }
            case 1: {
                float sight_radius = tc_perception->GetSightRadius();
                C3DVector from_see_er_to_see_ee = p_see_ee->GetPos() - p_see_er->GetPos();
                if (from_see_er_to_see_ee.GetSquaredMagnitude() <= sight_radius * sight_radius) {
                    from_see_er_to_see_ee.Normalise();
                    return cos(tc_perception->GetFOV() * (double)6.2831855f) < p_see_er->GetPhysicsTC()->GetFacingVector().Dot(from_see_er_to_see_ee);
                }
                return false;
            }
            case 3: {
                float smell_radius = tc_perception->GetSmellRadius();
                C3DVector from_see_er_to_see_ee=p_see_ee->GetPos() - p_see_er->GetPos();
                return from_see_er_to_see_ee.GetSquaredMagnitude() <= smell_radius * smell_radius;
            }
            case 4: {
                float hearing_radius = tc_perception->GetSoundRadius();
                C3DVector from_see_er_to_see_ee=p_see_ee->GetPos() - p_see_er->GetPos();
                return from_see_er_to_see_ee.GetSquaredMagnitude() <= hearing_radius * hearing_radius;
            }
            }
        }
    }
    return false;
}

bool CGameScriptInterface::IsThingAwareOfOtherThingInAnyWay(const CScriptThing& thing, const CScriptThing& other_thing) const
{
    return CanThingBe_Sensed_ByOtherThing(thing, other_thing, 5);
}
