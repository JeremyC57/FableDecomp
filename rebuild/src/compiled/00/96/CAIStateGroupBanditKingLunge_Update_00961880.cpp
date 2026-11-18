// CAIStateGroup_BanditKingLunge::Update() -- wait for idle, face the player and perform a lunge, then advance the boss phase and record its time.
// Leaf: ?Update@CAIStateGroup_BanditKingLunge@@MAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00961880, 313 bytes. Callees: CThingCreatureBase::IsPerformingAnyAction 0x006622A0,
//   CAIStateGroupBase::AIAction_DoNothing 0x0097E200, CBaseIntelligentPointer ctor / GetPItem / SetPItem 0x00A01B10 / 0x00A01B50 /
//   0x00A01B90 (the by-value CIntelligentPointer<const CThing>, vtable 0x01237F1C), CAIStateGroupBase::AIAction_LookAtThing 0x0097D6F0,
//   CAIStateGroup_BanditKingLunge::CanLungeAtPlayer 0x009613A0, CVectorMap::LowerBound 0x0040F020 (inline GetTC, interface 173 CTCBoss),
//   CCreatureAction_BanditKingLunge ctor 0x009619C0, AIAction_CombatPerformActionAndWait 0x0097DA40, ~CCombatActionBase 0x0062E900,
//   CTCBoss::GetBossPhase 0x007AED10 / SetNextBossPhase 0x007AEDA0 / ResetStrikesSinceHitHero 0x007AED50, CAIStateGroupBase::GetWorld
//   0x008FCFC0 + static CWorld::GetFrame 0x0049D870, CAIStateGroupBase::Terminate 0x008FCF40.
// NF67 (round 7): ported onto the shared headers (the 0x00961640 / 0x0095BCF0 macro blocks). The BanditKingLungeMethods / CThingRetail /
//   CInterfaceMap / CConstThingPointer / CRetailBanditKingLungeAction stand-ins, the local CTCBoss / CBaseIntelligentPointer models and the
//   local ReadGameTime are gone; the retail classes come from overlay opt-ins FABLE_RETAIL_CAISTATEGROUP_BANDITKINGLUNGE and
//   FABLE_RETAIL_CCREATUREACTION_BANDITKINGLUNGE. 0x006622A0 now binds ?IsPerformingAnyAction@CThingCreatureBase@@QBE_NXZ (nf41:e106).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGCREATUREBASE_ACTIONQUERY
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CTHINGPLAYERCREATURE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_GET_ORIGINAL_CHAIN
#define FABLE_RETAIL_CINTELLIGENTPOINTER_CONVERT_MUTABLE
#define FABLE_RETAIL_CINTELLIGENTPOINTER_DEREFERENCE
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CAISTATEGROUPBASE_GETCREATURE
#define FABLE_RETAIL_CAISTATEGROUPBASE_GOTOBED_WORLD
#define FABLE_RETAIL_CAISTATEGROUPBASE_AIACTIONS
#define FABLE_RETAIL_CAISTATEGROUPBASE_COMBATPERFORMACTION
#define FABLE_RETAIL_CAIBRAIN_GETCREATURE
#define FABLE_RETAIL_CAIBRAIN_CREATURE_REFERENCE
#define FABLE_RETAIL_CTCBOSS_STRIKES
#define FABLE_RETAIL_CAISTATEGROUP_BANDITKINGLUNGE
#define FABLE_RETAIL_CCREATUREACTION_BANDITKINGLUNGE
#include "rebuild_abi.h"
#include "engine/fable_ai_action_enums.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CThingAICreature.h"
#include "engine/CThingPlayerCreature.h"
#include "engine/CTCBoss.h"
#include "engine/CWorld.h"
#include "engine/CAIStateGroupBase.h"
#include "engine/CAIBrain.h"
#include "engine/CCreatureAction_BanditKingLunge.h"
#include "engine/CAIStateGroup_BanditKingLunge.h"

void CAIStateGroup_BanditKingLunge::Update()
{
    while (GetCreature().IsPerformingAnyAction()) {
        EAIActionReturn ret = AIAction_DoNothing();
        if (ret != AI_ACTION_IN_PROGRESS && ret != AI_ACTION_FINISHED) {
            Terminate();
            return;
        }
    }
    if (AIAction_LookAtThing(ACTION_REPEAT_UNTIL_FINISHED, Player, 0x17, 0.0027777001f) == AI_ACTION_FINISHED
        && CanLungeAtPlayer(Player.Get())) {
        CTCBoss* ptcBoss;
        GetCreature().GetTC(&ptcBoss);
        AIAction_CombatPerformActionAndWait(CCreatureAction_BanditKingLunge(GetCreature(), *Player), false);
        ptcBoss->SetNextBossPhase(ptcBoss->GetBossPhase() + 1, false);
        TimeOfLastLunge = GetWorld().GetFrame();
        ptcBoss->ResetStrikesSinceHitHero();
    }
    Terminate();
}
