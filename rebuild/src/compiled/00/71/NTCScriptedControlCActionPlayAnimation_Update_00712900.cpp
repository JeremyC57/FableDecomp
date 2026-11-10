// NTCScriptedControl::CActionPlayAnimation::Update -- start or wait for a creature animation.
// Fidelity: Original debug lifetime, cleanup placement and sound constructor are restored.
// Retail: 0x00712900,362 bytes,virtual thiscall.
// Leaf: ?Update@CActionPlayAnimation@NTCScriptedControl@@UAEXXZ
// Flags: none
#define FABLE_RETAIL_CSOUNDPAIR_ORIGINAL_CTORS
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCSCRIPTEDCONTROL_ACTIONBASE
#define FABLE_RETAIL_CTCSCRIPTEDCONTROL_CREATURE
#define FABLE_RETAIL_CCREATUREACTION_PLAYANIMATION
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGCREATUREBASE_ACTIONQUERY
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_STLP_LIST
#include "fable_string.h"
#include "engine/ETCInterfaceType.h"
#include "engine/CThingCreatureBase.h"
#include "engine/NTCScriptedControl_CActionPlayAnimation.h"
#include "engine/CSoundPair.h"
#include "engine/CCreatureAction_PlayAnimationWithLookTurning.h"
namespace NCreatureMode { bool FABLE_FASTCALL IsIncapacitated(const CThing&); }
inline CThingCreatureBase& CTCScriptedControl::GetParentCreature() { CThingCreatureBase& t=static_cast<CThingCreatureBase&>(GetParentThing()); return t; }
inline CThingCreatureBase& CTCScriptedControl::CActionBase::GetParentCreature() { return PTCScript->GetParentCreature(); }
inline void CTCScriptedControl::CActionBase::Terminate() { Terminated=true; }
namespace NTCScriptedControl {
void CActionPlayAnimation::Update()
{
    CCreatureActionBase* action=0;
    if (NCreatureMode::IsIncapacitated(GetParentCreature()))
        return;
    if (!PlayedAnim) {
        if (AllowLooking)
            action = new CCreatureAction_PlayAnimationWithLookTurning(GetParentCreature(),
                GetParentCreature().GetRandomAnimFromName(Animation, -1), StayOnLastFrame, Looping, NumLoops,
                CSoundPair(0), UseMovement, UsePhysics, AnimMayNeedCameraPositionUpdatedInSpeakAndWait, 0, Priority);
        else
            action = new CCreatureAction_PlayAnimation(GetParentCreature(),
                GetParentCreature().GetRandomAnimFromName(Animation, -1), StayOnLastFrame, Looping, NumLoops,
                CSoundPair(0), UseMovement, UsePhysics, AnimMayNeedCameraPositionUpdatedInSpeakAndWait, 0, Priority);
        if (AddAsQueuedAction) {
            if (GetParentCreature().IsPerformingAnyAction()) {
                GetParentCreature().AddQueuedAction(*action);
                PlayedAnim = true;
            } else
                PlayedAnim = GetParentCreature().SetCurrentAction(*action);
        } else
            PlayedAnim = GetParentCreature().SetCurrentAction(*action);
        if (!WaitForAnimationToFinish)
            Terminate();
    } else {
        if (!GetParentCreature().IsPerformingAnyAction())
            Terminate();
    }
    if(action)
        delete action;
}
}