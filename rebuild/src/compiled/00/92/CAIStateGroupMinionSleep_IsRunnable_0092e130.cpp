// CAIStateGroup_MinionSleep::IsRunnable() -- runnable straight away on the sleep component's fast track; otherwise only when it is ready to sleep, the go-to-bed group is runnable and the creature already stands on the bed position facing the bed's direction
// Leaf: ?IsRunnable@CAIStateGroup_MinionSleep@@UAE_NXZ
// Retail: Fable.exe 0x0092E130, 229 bytes, thiscall (Ego_r.pdb ?IsRunnable@CAIStateGroup_MinionSleep@@UAE_NXZ).
// Callees: CThing::GetTC<CTCSleep> (expanded, interface 0xD4), CTCSleep::GetFastTrack 0x0092C3B0,
//   CAIStateGroup_GoToBed::IsRunnable 0x0092D6F0 (qualified), CThing::GetPos const 0x004C73D0, CTCPhysicsBase::GetRHSet() const (slot).
#define FABLE_RETAIL_CAISTATEGROUP_GOTOBED
#define FABLE_RETAIL_CAISTATEGROUP_MINIONSLEEP
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CAISTATEGROUPBASE_GETCREATURE
#define FABLE_RETAIL_CAIBRAIN_GETCREATURE
#define FABLE_RETAIL_CTCSLEEP
#define FABLE_RETAIL_CTCSLEEP_FASTTRACK
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_CTCPHYSICSBASE
#include "engine/CThingAICreature.h"
#include "engine/CAIStateGroupBase.h"
#include "engine/CAIBrain.h"
#include "engine/CTCSleep.h"
#include "engine/CTCPhysicsBase.h"
#include "engine/CRightHandedSet.h"
#include "engine/C2DVector.h"
#include "engine/CAIStateGroup_GoToBed.h"
#include "engine/CAIStateGroup_MinionSleep.h"

bool CAIStateGroup_MinionSleep::IsRunnable()
{
    CTCSleep* sleep;
    if (!GetCreature().GetTC(&sleep))
        return false;
    if (!sleep->GetFastTrack()) {
        // Two separate tests (not one `||`): retail keeps both return-false blocks inline after the readiness test (0x0092E183).
        if (!sleep->GetIsReadyToSleep())
            return false;
        if (!CAIStateGroup_GoToBed::IsRunnable())
            return false;
        const C3DVector& pos = GetCreature().GetPos();
        if (C2DVector(BedPos.X - pos.X, BedPos.Y - pos.Y).GetMagnitude() > 0.01f)
            return false;
        const C3DVector& facing = GetCreature().GetPhysicsTC()->GetRHSet().GetForward();
        if (C2DVector(Rhset.GetForward().X - facing.X, Rhset.GetForward().Y - facing.Y).GetMagnitude() > 0.1f)
            return false;
    }
    return true;
}