// CAIStateGroup_MinionReactToCrime::OnReplace() -- when the state group is replaced: release the building it forced open (if that
// building is still alive), withdraw this creature's own "someone else is getting help" message when the guard was not told, clear
// GuardTold, and if this state group applied the opinion deed mask, restore the creature's default deed mask from its opinion def.
// Retail: Fable.exe (TLC, VC7.1) @ 0x00958B20, 224 bytes (+ int3 padding). __fastcall virtual (slot 10), `ret`.
// Leaf: ?OnReplace@CAIStateGroup_MinionReactToCrime@@UAEXXZ
// Identity: Ego_r.pdb / FableWin.pdb ?OnReplace@CAIStateGroup_MinionReactToCrime@@UAEXXZ; the address is slot 10 (+0x28) of the two
//   .rdata vtables that reference it (0x0128C580 CAIStateGroup_MinionReactToCrime, 0x0128C5BC its FromSleep subclass, which does
//   not override it); the members read are this class's Ego_r members (OpinionDeedHandle +0x14, GuardTold +0x18, DeedMaskApplied
//   +0x1A, ForceOpenBuilding +0x1B) and the callee 0x00958770 is its private GetSomeoneElseGettingHelpMessage(long).
// Callees: CBaseIntelligentPointer::GetPItem 0x00A01B50, CAIStateGroup_MinionReactToCrime::GetSomeoneElseGettingHelpMessage
//   0x00958770, CAIStateGroupBase::GetWorld 0x008FCFC0 (CWorld::GetMessageEventManager inline, +0x60), CMessageEventManager::
//   RemoveMessage 0x00493B5B, CVectorMap<ETCInterfaceType,CTCBase*,..>::LowerBound 0x0040F020 (CThing::GetTC<T> expanded),
//   CTCCreatureOpinionOfHero::SetOpinionDeedMask(long) 0x006821E0.
// Shape: IS_THING_ALIVE(ForceOpenBuilding) (null test + IsDead through operator->, two GetPItem calls) then the inline
//   CThingBuilding::DecrementForceOpenCount (third GetPItem, `dec [eax+0D0h]`); the sender is CMessageEvent::GetPOriginator<CThing>()
//   in its original form (CThing::StatGetTypeFlags() is 0, so the flag test folds to the constant `xor cl,cl ; jne` after the dead
//   things_flags store); GetTC<CTCCreatureOpinionOfHero> expanded (presence bit 0x4F at +0x28, key local, LowerBound, end-or-greater);
//   DeedMaskApplied is cleared only when the creature has the opinion component.
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
#define FABLE_RETAIL_CTHINGBUILDING_FORCEOPENCOUNT
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_GET_ORIGINAL_CHAIN
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COMPARE_NULL_INT
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CMESSAGEEVENT
#define FABLE_RETAIL_CMESSAGEEVENT_ORIGINATOR_ORIGINAL
#define FABLE_RETAIL_CMESSAGEEVENTMANAGER_REMOVE
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CAISTATEGROUPBASE_GETCREATURE
#define FABLE_RETAIL_CAISTATEGROUPBASE_GOTOBED_WORLD
#define FABLE_RETAIL_CAIBRAIN_GETCREATURE
#define FABLE_RETAIL_CAIBRAIN_CREATURE_REFERENCE
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CWORLD_MESSAGEEVENTMANAGER
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCCREATUREOPINIONOFHERO
#define FABLE_RETAIL_CTCCREATUREOPINIONOFHERO_GREET
#define FABLE_RETAIL_CTCCREATUREOPINIONOFHERO_SETDEEDMASK
#define FABLE_RETAIL_CAISTATEGROUP_MINIONREACTTOCRIME
#include "engine/CThingAICreature.h"
#include "engine/CThingBuilding.h"
#include "engine/CAIStateGroup_MinionReactToCrime.h"
#include "engine/CAIBrain.h"
#include "engine/CMessageEvent.h"
#include "engine/CMessageEventManager.h"
#include "engine/CWorld.h"
#include "engine/CTCCreatureOpinionOfHero.h"
#include "engine/ThingAlive.h"
#include "engine/CDefClassBase.h"

// COpinionOfHeroDef (Ego_r.pdb, 60 bytes; no shared header): a CDefClassBase with ThreatenGift +0x28, WillAttackCriminals +0x2C,
// Personality +0x30, ReactionMask +0x34, DeedMask +0x38.
class COpinionOfHeroDef : public CDefClassBase {
public:
    long ThreatenGift;                                                   // +0x28 CDefIndex
    bool WillAttackCriminals;                                            // +0x2c
    long Personality;                                                    // +0x30
    long ReactionMask;                                                   // +0x34
    long DeedMask;                                                       // +0x38
};

void CAIStateGroup_MinionReactToCrime::OnReplace()
{
    if (IS_THING_ALIVE(ForceOpenBuilding))
        ForceOpenBuilding->DecrementForceOpenCount();
    if (!GuardTold) {
        const CMessageEvent* pmessage = GetSomeoneElseGettingHelpMessage(OpinionDeedHandle);
        if (pmessage && pmessage->GetPOriginator<CThing>() == &GetCreature())
            GetWorld().GetMessageEventManager().RemoveMessage(pmessage);
    }
    GuardTold = false;
    if (DeedMaskApplied) {
        CTCCreatureOpinionOfHero* popinion;
        if (GetCreature().GetTC(&popinion)) {
            popinion->SetOpinionDeedMask(popinion->PDef->DeedMask);
            DeedMaskApplied = false;
        }
    }
}