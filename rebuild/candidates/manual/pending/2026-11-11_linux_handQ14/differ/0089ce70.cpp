// CGameScriptInterface::IsThingAwareOfOtherThingInAnyWay(const CScriptThing& thing, const CScriptThing& otherThing) const -- whether otherThing perceives thing in any way
// Grade: DIFFER(ours v retail) -- as-is SRC_FAIL here (needs a prior agent's FABLE_RETAIL_CTCPERCEIVETHING_SENSES overlay that is not in this batch; earlier graded 616v616); local model, not migrated; earlier residue: masked diff 22 lines, all x87: inlined Normalise multiplies f*X into a dup of the factor (`fld st0; fmul st3`) where retail does X*f in X's register (`fxch st2; fmul st2`), and the cone compare pops with `fcompp` where retail emits `fcomp st(1); fnstsw; fstp st(0)` and loads the Dot products Z,Y,X (ours Y,Z,X)
// Earlier grade note: DIFFER(616v616) -- masked diff 22 lines, all x87: inlined Normalise multiplies f*X into a dup of the factor (`fld st0; fmul st3`) where retail does X*f in X's register (`fxch st2; fmul st2`), and the cone compare pops with `fcompp` where retail emits `fcomp st(1); fnstsw; fstp st(0)` and loads the Dot products Z,Y,X (ours Y,Z,X)
// Leaf: ?IsThingAwareOfOtherThingInAnyWay@CGameScriptInterface@@UBE_NABVCScriptThing@@0@Z
// Retail: 0x0089CE70, 616 bytes, __thiscall (virtual, const)
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

bool CGameScriptInterface::IsThingAwareOfOtherThingInAnyWay(const CScriptThing& thing, const CScriptThing& otherThing) const
{
    if (!thing.IsAlive() || !otherThing.IsAlive())
        return false;
    CThing* perceiver = otherThing.GetPThing();
    CThing* perceived = thing.GetPThing();
    CTCPerceiveThing* perception;
    if (perceiver->GetTC(TCI_PERCEIVE_THING, &perception)) {
        const CThing* current = perception->GetPerceivedThing();
        if (current && current == perceived)
            return perception->IsAwareOfThing();
        float radius = std::max(perception->GetSmellRadius(), perception->GetSoundRadius());
        {
            C3DVector nearOffset = perceived->GetPos() - perceiver->GetPos();
            float nearSq = nearOffset.Z * nearOffset.Z;
            nearSq += nearOffset.Y * nearOffset.Y + nearOffset.X * nearOffset.X;
            if (nearSq <= radius * radius)
                return true;
        }
        radius = perception->GetExtendedSightRadius();
        C3DVector offset = perceived->GetPos() - perceiver->GetPos();
        float offsetSq = offset.Z * offset.Z;
        offsetSq += offset.X * offset.X + offset.Y * offset.Y;
        if (offsetSq <= radius * radius) {
            offset.Normalise();
            if (cos(perception->GetFOV() * (double)6.2831855f) < offset.Dot(perceiver->GetPhysicsTC()->GetFacingVector()))
                return true;
            else
                return false;
        }
        return false;
    }
    return false;
}
