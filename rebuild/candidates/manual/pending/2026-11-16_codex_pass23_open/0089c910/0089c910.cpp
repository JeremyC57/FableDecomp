// 0089c910 CGameScriptInterface::CanThingBe_Sensed_ByOtherThing
// Grade: DIFFER(1016v1016) -- 974 code bytes versus976; x87 compare/pop pair differs; shifted local targets
// Flags: /d2inlS100
// Inline-budget: /d2inlS100 expands the four radius magnitude sites (0089CA58, 0089CABB, 0089CB64, 0089CC94); plain inline leaves calls. Dot is ordinary inline.
// Leaf: ?CanThingBe_Sensed_ByOtherThing@CGameScriptInterface@@ABE_NABVCScriptThing@@0J@Z
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
