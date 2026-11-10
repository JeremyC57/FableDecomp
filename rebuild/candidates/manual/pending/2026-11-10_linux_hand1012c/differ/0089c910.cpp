// CGameScriptInterface::CanThingBe_Sensed_ByOtherThing(const CScriptThing& thing, const CScriptThing& otherThing, long senseType) const -- whether otherThing can see / nearly see / smell / hear thing, or is aware of it in any way (sense types 1..5)
// Grade: DIFFER(1016v1016) -- same x87 residue as 0x0089CE70 in the shared cone tail (Normalise f*X operand order; fcompp v retail fcomp st(1)/fstp st(0)); the rest of the masked diff is jump-table displacement desync from that size-neutral change
// Leaf: ?CanThingBe_Sensed_ByOtherThing@CGameScriptInterface@@ABE_NABVCScriptThing@@0J@Z
// Retail: 0x0089C910, 1016 bytes, __thiscall (private, const)
// Overlay: graded with FABLE_HDR_OVERLAY=$A/hdr (see hdr/README.md)
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_NORMALISE_SQUARE_HELPER

#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_CTCPERCEIVETHING
#define FABLE_RETAIL_CTCPERCEIVETHING_AWARENESS
#define FABLE_RETAIL_CTCPERCEIVETHING_SENSES
#define FABLE_RETAIL_CGAMESCRIPTINTERFACE_SENSED
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
#pragma intrinsic(cos)

bool CGameScriptInterface::CanThingBe_Sensed_ByOtherThing(const CScriptThing& thing, const CScriptThing& otherThing, long senseType) const
{
    if (!thing.IsAlive() || !otherThing.IsAlive())
        return false;
    CThing* perceiver = otherThing.GetPThing();
    CThing* perceived = thing.GetPThing();
    CTCPerceiveThing* perception;
    if (perceiver->GetTC(TCI_PERCEIVE_THING, &perception)) {
        const CThing* current = perception->GetPerceivedThing();
        if (current && current == perceived) {
            switch (senseType) {
            case 1:
                return perception->IsThingLineOfSight();
            case 2:
                return perception->IsThingExtendedLineOfSight();
            case 3:
                return perception->IsThingSmelt();
            case 4:
                return perception->IsThingHeard();
            case 5:
                return perception->IsAwareOfThing();
            }
        } else {
            switch (senseType) {
            case 5: {
                float radius = std::max(perception->GetSmellRadius(), perception->GetSoundRadius());
                if ((perceived->GetPos() - perceiver->GetPos()).GetSquaredMagnitude() <= radius * radius)
                    return true;
            }
            case 2: {
                float radius = perception->GetExtendedSightRadius();
                C3DVector offset = perceived->GetPos() - perceiver->GetPos();
                float offsetSq = offset.Z * offset.Z;
                offsetSq += offset.X * offset.X + offset.Y * offset.Y;
                if (offsetSq <= radius * radius) {
                    offset.Normalise();
                    return cos(perception->GetFOV() * (double)6.2831855f) < offset.Dot(perceiver->GetPhysicsTC()->GetFacingVector());
                }
                break;
            }
            case 1: {
                float radius = perception->GetSightRadius();
                C3DVector offset = perceived->GetPos() - perceiver->GetPos();
                float offsetSq = offset.Z * offset.Z;
                offsetSq += offset.X * offset.X + offset.Y * offset.Y;
                if (offsetSq <= radius * radius) {
                    offset.Normalise();
                    return cos(perception->GetFOV() * (double)6.2831855f) < offset.Dot(perceiver->GetPhysicsTC()->GetFacingVector());
                }
                break;
            }
            case 3: {
                float radius = perception->GetSmellRadius();
                return (perceived->GetPos() - perceiver->GetPos()).GetSquaredMagnitude() <= radius * radius;
            }
            case 4: {
                float radius = perception->GetSoundRadius();
                return (perceived->GetPos() - perceiver->GetPos()).GetSquaredMagnitude() <= radius * radius;
            }
            }
        }
    }
    return false;
}
