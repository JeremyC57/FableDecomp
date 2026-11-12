// CAIStateGroup_MinionReactToCrime::Update()
// Shape: GetDef<CMessageEventDef> (0x007923C0) is instantiated in this TU with its real body (FABLE_RETAIL_CDEFINITIONMANAGER_GETDEF_NAME_BODY,
//   noinline): VC7.1 then knows the def handle is only written through `out`, so `*def` is read at its push and the release is in place.
// Run for help after a crime: hold the villager's home open (ForceOpenBuilding = home building, bump its force-open count), unless
// duplicates are allowed give up when someone else is already getting help; with a perceived thing, post the GOING_FOR_HELP message
// event from the villager, flag the opinion component as in find-a-guard, drop out of the hero's AI followers when the perceived thing
// is the (living) hero, find a guard and yell (retrying while it returns 4), clear the flag, set GuardTold on 2; Terminate.
// Retail: Fable.exe (TLC, VC7.1) @ 0x00958840, 724 bytes, virtual slot 12 (vtable 0x0128C558), plain `ret`. No pragmas / flags.
// Callees: CBaseIntelligentPointer ctor/GetPItem/SetPItem/dtor 0x00A01B10/0x00A01B50/0x00A01B90/0x00A01C10 (CIntelligentPointer<CThingBuilding>
//   temporary, vtable 0x01244344), GetSomeoneElseGettingHelpMessage 0x00958770, CVectorMap::LowerBound 0x004365B0 (const) / 0x0040F020,
//   CTCPerceiveThing::GetPerceivedThing 0x007446A0, CCharString ctor 0x0099EBF0 / dtor 0x0099EAE0, CAIStateGroupBase::PeekDefinitionManager
//   0x008FCFB0, CDefinitionManager::GetDef<CMessageEventDef>(const CCharString&, CDefPointer<const CMessageEventDef>*) 0x007923C0,
//   CAIStateGroupBase::GetWorld 0x008FCFC0, CThing::GetPos 0x004C73D0, CMessageEventManager::AddMessage 0x00687730,
//   CAIStateGroupBase::GetPlayerManager 0x008FCF90, CPlayerManager::GetMainPlayer 0x00449970, CPlayer::GetPControlledCreature 0x00487DC0,
//   CTCFollowed::IsAIFollower 0x006D4DB0 / QuitAIFollower 0x006D56E0, CAIStateGroupBase::AIAction_FindAGuardAndYellForHelp 0x0097FEA0,
//   CAIStateGroupBase::Terminate 0x008FCF40.
// Header overlay ($W/hdr): FABLE_RETAIL_CAISTATEGROUPBASE_FINDAGUARD_YELL, FABLE_RETAIL_CTHINGBUILDING_INCREMENTFORCEOPENCOUNT,
//   FABLE_RETAIL_CTCCREATUREOPINIONOFHERO_FINDAGUARD, FABLE_RETAIL_CMESSAGEEVENTMANAGER_ADDFROMDEF.
// Leaf: ?Update@CAIStateGroup_MinionReactToCrime@@UAEXXZ
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_STATTYPEFLAGS
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CTHINGAICREATURE_HOMEBUILDING
#define FABLE_RETAIL_CTHINGBUILDING_FORCEOPENCOUNT
#define FABLE_RETAIL_CTHINGBUILDING_INCREMENTFORCEOPENCOUNT
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN
#define FABLE_RETAIL_CINTELLIGENTPOINTER_GET_ORIGINAL_CHAIN
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COMPARE_NULL_INT
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CMESSAGEEVENT
#define FABLE_RETAIL_CMESSAGEEVENTMANAGER_REMOVE
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CAISTATEGROUPBASE_GETCREATURE
#define FABLE_RETAIL_CAISTATEGROUPBASE_GOTOBED_WORLD
#define FABLE_RETAIL_CAISTATEGROUPBASE_FINDAGUARD_YELL
#define FABLE_RETAIL_CAIBRAIN_GETCREATURE
#define FABLE_RETAIL_CAIBRAIN_CREATURE_REFERENCE
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CWORLD_MESSAGEEVENTMANAGER
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCCREATUREOPINIONOFHERO
#define FABLE_RETAIL_CTCCREATUREOPINIONOFHERO_GREET
#define FABLE_RETAIL_CTCCREATUREOPINIONOFHERO_FINDAGUARD
#define FABLE_RETAIL_CTCPERCEIVETHING
#define FABLE_RETAIL_CAISTATEGROUP_MINIONREACTTOCRIME
#define FABLE_RETAIL_CAISTATEGROUPBASE_PEEKDEFINITIONMANAGER
#define FABLE_RETAIL_CAISTATEGROUPBASE_AIACTIONS
#define FABLE_RETAIL_CAISTATEGROUPBASE_SPEECH
#define FABLE_RETAIL_CPLAYERMANAGER
#define FABLE_RETAIL_PLAYER
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CDEFINITIONMANAGER_GETDEF_NAME_BODY
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_STLP_LIST_SIZE_DISTANCE
#define FABLE_RETAIL_CMESSAGEEVENTMANAGER_ADDFROMDEF
#include "engine/CThingAICreature.h"
#include "engine/CThingBuilding.h"
#include "engine/CAIStateGroup_MinionReactToCrime.h"
#include "engine/CAIBrain.h"
#include "engine/CMessageEvent.h"
#include "engine/CMessageEventManager.h"
#include "engine/CWorld.h"
#include "engine/CTCCreatureOpinionOfHero.h"
#include "engine/CTCPerceiveThing.h"
#include "engine/CTCFollowed.h"
#include "engine/CPlayerManager.h"
#include "engine/CPlayer.h"
#include "engine/CGameDefinitionManager.h"
#include "engine/CDefPointer.h"
#include "engine/ThingAlive.h"
#include "engine/CThingPlayerCreature.h"

#include "engine/CDefClassBase.h"
// CMessageEventDef (Ego_r.pdb, 84 bytes; no shared header): a CDefClassBase; no member is read here.
class CMessageEventDef : public CDefClassBase { };

// FableWin.pdb ?GetHomeBuilding@CThingAICreature@@QBEPAVCThingBuilding@@XZ, inline in retail (declared in engine/CThingAICreature.h):
//   `lea ecx,[eax+168h] ; call 0x00A01B50` at 0x0095884F; same body and justification as landed 0x00946400 CAIStateGroup_PrepareMeal::IsRunnable.
inline CThingBuilding* CThingAICreature::GetHomeBuilding() const { return HomeBuilding.Get(); }

void CAIStateGroup_MinionReactToCrime::Update()
{
    ForceOpenBuilding = GetCreature().GetHomeBuilding();
    if (IS_THING_ALIVE(ForceOpenBuilding))
        ForceOpenBuilding->IncrementForceOpenCount();
    if (AllowDuplicateReactToCrime || !GetSomeoneElseGettingHelpMessage(OpinionDeedHandle)) {
        const CTCPerceiveThing* pperceive;
        if (GetCreature().PeekTC(TCI_PERCEIVE_THING, &pperceive) && pperceive->GetPerceivedThing()) {
            CDefPointer<const CMessageEventDef> def;
            bool found;
            // The named string in its own block is what retail's order needs: the CCharString ctor (0x0095893D) runs before &def is
            // pushed (0x00958942 / 0x00958946) and ~CCharString (0x00958960) runs before `test bl,bl` (0x00958965). With a temporary in
            // the if condition VC7.1 pushes &def first (721 bytes instead of 724).
            {
                CCharString name("STANDARD_MESSAGE_EVENT_GOING_FOR_HELP");
                found = PeekDefinitionManager().GetDef(name, &def);
            }
            if (found)
                GetWorld().GetMessageEventManager().AddMessage(*def, GetCreature().GetPos(), &GetCreature(), 0, OpinionDeedHandle);
            C3DVector pos = pperceive->GetPerceivedThing()->GetPos();
            CTCCreatureOpinionOfHero* popinion;
            if (GetCreature().GetTC(&popinion))
                popinion->SetInFindAGuardAndYellForHelp(true);
            CThingPlayerCreature* hero = GetPlayerManager().GetMainPlayer().GetPControlledCreature();
            if (hero && !hero->IsDead() && pperceive->GetPerceivedThing() == hero) {
                CTCFollowed* pfollowed;
                if (hero->GetTC(&pfollowed) && pfollowed->IsAIFollower(&GetCreature()))
                    pfollowed->QuitAIFollower(GetCreature());
            }
            EAIActionReturn result;
            do {
                result = AIAction_FindAGuardAndYellForHelp(pos, OpinionDeedHandle, true);
            } while (result == 4);
            if (GetCreature().GetTC(&popinion))
                popinion->SetInFindAGuardAndYellForHelp(false);
            if (result == 2)
                GuardTold = true;
        }
    }
    Terminate();
}