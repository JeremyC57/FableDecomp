// CAIStateGroup_BanditGossip::Update() -- a bandit gossips: take a pending gossip request from a valid gossipee in range (and pair
// Grade: DIFFER(1362v1367) 10 hunks: frame 0x168 v 0x170 (retail places the IsOfType/IntelligentPointer temporaries 4-8 bytes higher), the CTCSolidToThing lookup uses ecx/eax where ours uses eax/ecx/edx, and ours CSEs the creature before the AddTC CCharString temp (retail reloads it after the pushes) -- 5 bytes short
//   the two gossip components up), else post a request and wait up to four seconds for one; look at the gossipee, walk up to it,
//   sheathe a carried weapon, become non-solid to it and play one of three random bandit interactions with it, then unsheathe
//   and terminate
// Leaf: ?Update@CAIStateGroup_BanditGossip@@MAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0090E5D0, 1367 bytes, thiscall, plain `ret` (8-aligned frame, 0x170).
// Callees: CBaseIntelligentPointer ctor 0x00A01B10 / GetPItem 0x00A01B50 / SetPItem 0x00A01B90 / dtor 0x00A01C10,
//   CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound 0x0040F020, CThingFilter_IsValidGossipee ctor 0x0090C870, CThing::GetPos
//   0x004C73D0, CAIStateGroupBase::GetWorld 0x008FCFC0 / GetPWorldSeed 0x008FCFD0 / Terminate 0x008FCF40 / AIAction_DoNothing
//   0x0097E200 / AIAction_LookAtThing 0x0097D6F0 / AIAction_MoveToThingProximity 0x0097E380 / AIAction_PerformActionAndWait 0x0097DB70
//   / AIAction_InteractWith 0x00981410, CMessageEventManager::FindMostRecentMessage<...> 0x0090EBE0 / RemoveMessage 0x00493B5B /
//   AddMessage 0x00687540, CThingPhysical::PeekPos 0x006A4D00, CWorld::GetFrame 0x0049D870, CMainGameComponent::ConstantFPS
//   0x01375550, CTCCarrying::IsThingCarryingWeapon 0x006A1540, CCreatureAction_SheatheWeapons ctor 0x008CC370 /
//   CCreatureAction_UnsheatheWeapons ctor 0x008B8590 / ~CCombatActionBase 0x0062E900, IS_THING_DEAD COMDAT 0x007304C0, CThing::AddTC
//   0x004C9D60 / RemoveTC 0x004C9840, CTCSolidToThing::AddThingNotSolidTo 0x00770FA0, GFROR13 0x00497890, CCharString 0x0099EBF0 /
//   copy 0x0099EC30 / ~ 0x0099EAE0, CIntelligentPointer<CThingAICreature> copy 0x0050F410.
// Layout: overlay engine/CAIStateGroup_BanditGossip.h / CAIStateGroup_MinionGossip.h (FABLE_RETAIL_CAISTATEGROUP_GOSSIP: Critical +0x18,
//   WasCarryingWeapon +0x24); CTCGossip (Ego_r: Gossipee +0x0c, MyTurnToSpeak +0x14; no engine header, local model).
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CTHINGAICREATURE_STATTYPEFLAGS
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COMPARE_NULL_INT
#define FABLE_RETAIL_CINTELLIGENTPOINTER_CONVERT_MUTABLE
#define FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COPY_OUTOFLINE
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CCREATUREACTIONBASE_FULL_MEMBERS
#define FABLE_RETAIL_CCREATUREACTION_UNSHEATHEWEAPONS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CAISTATEGROUPBASE_GETCREATURE
#define FABLE_RETAIL_CAISTATEGROUPBASE_AIACTIONS
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEALACTIONS
#define FABLE_RETAIL_CAISTATEGROUPBASE_PERFORMACTION
#define FABLE_RETAIL_CAISTATEGROUPBASE_GOTOBED_WORLD
#define FABLE_RETAIL_CAISTATEGROUPBASE_INTERACTWITH
#define FABLE_RETAIL_CAIBRAIN_GETCREATURE
#define FABLE_RETAIL_CAIBRAIN_CREATURE_REFERENCE
#define FABLE_RETAIL_CAISTATEGROUP_GOSSIP
#define FABLE_RETAIL_CMESSAGEEVENT
#define FABLE_RETAIL_CMESSAGEEVENT_ORIGINATOR_ORIGINAL
#define FABLE_RETAIL_CMESSAGEEVENTMANAGER_FINDRECENT
#define FABLE_RETAIL_CMESSAGEEVENTMANAGER_REMOVE
#define FABLE_RETAIL_CMESSAGEEVENTFILTERS
#define FABLE_RETAIL_CMESSAGEEVENTFILTERS_PAYMENT
#define FABLE_RETAIL_CTHINGFILTER_IN2DRANGE_CTOR
#define FABLE_RETAIL_CTCSOLIDTOTHING
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CWORLD_MESSAGEEVENTMANAGER
#define FABLE_RETAIL_CINITBASECLASS
#define FABLE_RETAIL_CGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CTCCARRYING
#define FABLE_RETAIL_CTHING_STATEACCESS
#include "fable_string.h"
#include "engine/fable_ai_action_enums.h"
#include "engine/C3DVector.h"
#include "engine/CDefPointer.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CThingAICreature.h"
#include "engine/CTCCarrying.h"
#include "engine/CTCSolidToThing.h"
#include "engine/ThingAlive.h"
#include "engine/GFRandom.h"
#include "engine/CMainGameComponent.h"
#include "engine/CWorld.h"
#include "engine/CMessageEvent.h"
#include "engine/CMessageEventManager.h"
#include "engine/CMessageEventFilters.h"
#include "engine/CFilter_Bind3.h"
#include "engine/CThingFilter_IsValidGossipee.h"
#include "engine/CCombatActionBase.h"
#include "engine/CCreatureAction_UnsheatheWeapons.h"
#include "engine/CAIStateGroupBase.h"
#include "engine/CAIBrain.h"
#include "engine/CAIStateGroup_BanditGossip.h"

class CTCGossip : public CTCBase {                       // Ego_r.pdb CTCGossip (0x18); no engine header
public:
    static ETCInterfaceType __fastcall StatGetInterfaceType() { return TCI_GOSSIP; }
    CIntelligentPointer<CThingAICreature> Gossipee;     // +0x0c
    bool MyTurnToSpeak;                                 // +0x14
};
class CCreatureAction_SheatheWeapons : public CCombatActionBase {   // RTTI : CCombatActionBase; no engine header
public:
    CCreatureAction_SheatheWeapons(CThingCreatureBase& creature);   // 0x008CC370
};

typedef CFilter_Bind3<CMessageEventFilter_IsOfType, CMessageEventFilter_ThingSearchFilter<CThingFilter_In2DRange>,
                      CMessageEventFilter_ThingSearchFilter<CThingFilter_IsValidGossipee> > TGossipRequestFilter;


void CAIStateGroup_BanditGossip::Update()
{
    WasCarryingWeapon = false;
    Critical = false;
    CIntelligentPointer<CThingAICreature> gossipee = 0;
    CTCGossip* ptc_gossip = 0;
    CTCGossip* ptc_gossip_other = 0;
    CTCSolidToThing* ptc_solid = 0;
    GetCreature().GetTC(&ptc_gossip);
    bool my_turn;
    const CMessageEvent* message = GetWorld().GetMessageEventManager().FindMostRecentMessage(
        TGossipRequestFilter(CMessageEventFilter_IsOfType(MESSAGE_EVENT_REQUEST_GOSSIP),
                             CMessageEventFilter_ThingSearchFilter<CThingFilter_In2DRange>(CThingFilter_In2DRange(GetCreature().GetPos(), 16.0f)),
                             CMessageEventFilter_ThingSearchFilter<CThingFilter_IsValidGossipee>(CThingFilter_IsValidGossipee(GetCreature()))));
    if (message) {
        gossipee = message->GetPOriginator<CThingAICreature>();
        GetWorld().GetMessageEventManager().RemoveMessage(message);
        if (IS_THING_DEAD(gossipee)) {
            Terminate();
            return;
        }
        gossipee->GetTC(&ptc_gossip_other);
        ptc_gossip->Gossipee = gossipee;
        ptc_gossip->MyTurnToSpeak = true;
        ptc_gossip_other->Gossipee = &GetCreature();
        ptc_gossip_other->MyTurnToSpeak = false;
        my_turn = false;
    } else {
        GetWorld().GetMessageEventManager().AddMessage(MESSAGE_EVENT_REQUEST_GOSSIP, CMainGameComponent::GetConstantFPS() * 4,
            GetCreature().PeekPos(), &GetCreature(), 0, 16.0f, 16.0f, 0);
        long end_frame = CWorld::GetFrame() + CMainGameComponent::GetConstantFPS() * 4;
        for (;;) {
            if (IS_THING_ALIVE(ptc_gossip->Gossipee)) {
                gossipee = ptc_gossip->Gossipee;
                gossipee.Get();
                my_turn = true;
                break;
            }
            if (AIAction_DoNothing() == AI_ACTION_TERMINATE_PROCESS || CWorld::GetFrame() > end_frame) {
                Terminate();
                return;
            }
        }
    }
    if (gossipee == 0) {
        Terminate();
        return;
    }
    if (AIAction_LookAtThing(ACTION_REPEAT_UNTIL_FINISHED, gossipee, 17, 0.0027777f) != AI_ACTION_FINISHED) {
        Terminate();
        return;
    }
    EAIActionReturn ret;
    do {
        bool reached;
        ret = AIAction_MoveToThingProximity(ACTION_REPEAT_CALLBACK, gossipee, 2.0f, reached, (EMovementType)3, true, false, true, true, 0.0f);
    } while (ret == AI_ACTION_IN_PROGRESS);
    if (ret != AI_ACTION_FINISHED) {
        Terminate();
        return;
    }
    if (CTCCarrying::IsThingCarryingWeapon(GetCreature())) {
        if (AIAction_PerformActionAndWait(CCreatureAction_SheatheWeapons(GetCreature()), false) != AI_ACTION_FINISHED) {
            Terminate();
            return;
        }
        WasCarryingWeapon = true;
    }
    if (IS_THING_DEAD(gossipee)) {
        Terminate();
        return;
    }
    if (!GetCreature().GetTC(&ptc_solid))
        ptc_solid = static_cast<CTCSolidToThing*>(GetCreature().AddTC(CCharString("CTCSolidToThing"), false, 0));
    ptc_solid->AddThingNotSolidTo(gossipee.Get());
    const char* interactions[3] = { "VILLAGER_INTERACTION_BANDIT_PUSHING", "VILLAGER_INTERACTION_BANDIT_HOLDING",
                                    "VILLAGER_INTERACTION_BANDIT_TELLING_OFF" };
    CCharString interaction(interactions[GFRandom(3, GetPWorldSeed())]);
    Critical = true;
    AIAction_InteractWith(ACTION_REPEAT_UNTIL_FINISHED, gossipee, interaction, 100, my_turn);
    Critical = false;
    GetCreature().RemoveTC(TCI_SOLID_TO_THING);
    if (WasCarryingWeapon) {
        if (AIAction_PerformActionAndWait(CCreatureAction_UnsheatheWeapons(GetCreature()), false) != AI_ACTION_FINISHED) {
            Terminate();
            return;
        }
        WasCarryingWeapon = false;
    }
    Terminate();
}
