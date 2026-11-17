// CAIStateGroup_MinionGuardReactToBodyFoundMessage::Update() -- take the body-found message: when its originator (the body) is alive, remember the reported position, consume the message, unless already within 2.5 play the curiosity action and walk up to the body, look at it, walk to the reported position and order a security sweep; terminate in every case
// Leaf: ?Update@CAIStateGroup_MinionGuardReactToBodyFoundMessage@@UAEXXZ
// Retail: 0x00911300, 666 bytes (plain ret).
// Names: Ego_r.pdb class members; Ego_d.pdb locals pevent, and in its block poriginator (CIntelligentPointer<const CThing>), ret
//        (EAIActionReturn), pos (C3DVector) and two scoped bool nav_route_calculated. Actions: 0x00911180 CCreatureAction_Curiosity,
//        0x009110F0 CCreatureAction_OrderSecuritySweep (Ego_r order before Curiosity).
// FITTED (chosen by grading; no Ego_d local): the look speed spelled GetCreature().GetFOV() * (1.0f / 360.0f) (slot 92; 0x01238E00 holds
//        exactly 1/360 -- the original constant's name is unknown).
// GFWorld3DDistanceSmallerThan: the shared inline body (existing opt-in FABLE_RETAIL_GFWORLD3DDISTANCE_SMALLER); VC7.1 itself calls its
//        out-of-line copy (retail 0x009113DD `call 0x006614F0`), so no out-of-line device is needed.
#define FABLE_RETAIL_CTHING_ORIGINAL_VTABLE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_STATTYPEFLAGS
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGCREATUREBASE_STATTYPEFLAGS
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CTHINGAICREATURE_STATTYPEFLAGS
#define FABLE_RETAIL_GFWORLD3DDISTANCE_SMALLER
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN
#define FABLE_RETAIL_CINTELLIGENTPOINTER_GET_ORIGINAL_CHAIN
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COMPARE_NULL_INT
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COPY
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_CMESSAGEEVENT
#define FABLE_RETAIL_CMESSAGEEVENT_ORIGINATOR_ORIGINAL
#define FABLE_RETAIL_CMESSAGEEVENTMANAGER_REMOVE
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CAISTATEGROUPBASE_GETCREATURE
#define FABLE_RETAIL_CAISTATEGROUPBASE_AIACTIONS
#define FABLE_RETAIL_CAISTATEGROUPBASE_SPEECH
#define FABLE_RETAIL_CAISTATEGROUPBASE_PERFORMACTION
#define FABLE_RETAIL_CAISTATEGROUPBASE_MOVETOPOSPROXIMITY
#define FABLE_RETAIL_CAIBRAIN_GETCREATURE
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CWORLD_MESSAGEEVENTMANAGER
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CCREATUREACTION_CURIOSITY
#include "fable_string.h"
#include "engine/fable_ai_action_enums.h"
#include "engine/CThingAICreature.h"
#include "engine/CAIStateGroupBase.h"
#include "engine/CAIBrain.h"
#include "engine/CAIStateGroup_MinionGuardReactToBodyFoundMessage.h"
#include "engine/CMessageEvent.h"
#include "engine/CMessageEventManager.h"
#include "engine/CWorld.h"
#include "engine/GFWorldDistance.h"
#include "engine/CCreatureAction_Curiosity.h"
#include "engine/CCreatureAction_OrderSecuritySweep.h"
#include "engine/ThingAlive.h"

void CAIStateGroup_MinionGuardReactToBodyFoundMessage::Update()
{
    const CMessageEvent* pevent = FindBodyFoundEvent();
    if (pevent)
    {
        CIntelligentPointer<const CThing> poriginator(pevent->GetPOriginator<CThing>());
        EAIActionReturn ret;
        if (!IS_THING_ALIVE(poriginator))
        {
            Terminate();
            return;
        }
        C3DVector pos = pevent->Pos;
        GetWorld().GetMessageEventManager().RemoveMessage(pevent);
        if (!GFWorld3DDistanceSmallerThan(GetCreature().GetPos(), poriginator->GetPos(), 2.5f))
        {
            if (AIAction_PerformActionAndWait(CCreatureAction_Curiosity(GetCreature()), false) != AI_ACTION_FINISHED)
            {
                Terminate();
                return;
            }
            bool nav_route_calculated = false;
            do
            {
                if (!IS_THING_ALIVE(poriginator))
                {
                    Terminate();
                    return;
                }
                ret = AIAction_MoveToThingProximity(ACTION_REPEAT_CALLBACK, poriginator, 2.5f, nav_route_calculated, AI_MOVEMENT_RUNNING,
                                                    true, false, true, true, 0.0f);
            } while (ret == AI_ACTION_IN_PROGRESS);
            if (ret != AI_ACTION_FINISHED)
            {
                Terminate();
                return;
            }
        }
        if (IS_THING_ALIVE(poriginator))
        {
            if (AIAction_LookAtThing(ACTION_REPEAT_UNTIL_FINISHED, poriginator, 15, GetCreature().GetFOV() * (1.0f / 360.0f)) == AI_ACTION_FINISHED)
            {
                bool nav_route_calculated = false;
                if (AIAction_MoveToPosProximity(ACTION_REPEAT_UNTIL_FINISHED, pos, 1.5f, nav_route_calculated, AI_MOVEMENT_RUNNING,
                                                true, false, true, 0, true, 0.0f) != AI_ACTION_FINISHED)
                {
                    Terminate();
                    return;
                }
                if (AIAction_PerformActionAndWait(CCreatureAction_OrderSecuritySweep(GetCreature()), false) != AI_ACTION_FINISHED)
                {
                    Terminate();
                    return;
                }
            }
        }
    }
    Terminate();
}
