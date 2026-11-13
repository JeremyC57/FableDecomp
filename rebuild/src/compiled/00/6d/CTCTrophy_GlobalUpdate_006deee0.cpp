// CTCTrophy::GlobalUpdate(bool update_sounds) -- trophy dialog handling while a confirmation is pending: a yes/no answer starts the minigame (unsheathing the trophy for its owner first), shows the trophy (the show action plus the bandit-faction deeds) or opens the instructions; a click-past ends the game, clears the score wait, pages the instructions (then the play-game text with the witness counts) or unpauses
// Leaf: ?GlobalUpdate@CTCTrophy@@UAEX_N@Z
// Inline-budget: FABLE_RETAIL_CDEFSTRING_FORCEINLINE_CONVERSION -- the show action's animation name is CDefString's implicit conversion
//   temporary (0x006DF0BE..0x006DF0D1: `lea edx,<tmp> ; push edx ; add ecx,3Ch ; call 0x00415DD0` then `lea eax,<tmp>` for the
//   argument); retail expands the inline ??B forwarder and calls GetString 0x00415DD0 out of line, where VC7.1's plain-inline heuristic
//   calls the forwarder's own COMDAT ??B (0x00415D70). Same opt-in and evidence as row 0x008A8C60. An explicit `.GetString()` argument
//   pushes the result's eax instead and moves the frame (DIFFER 1433v1437).
// Retail: Fable.exe 0x006DEEE0, 1437 bytes (`ret 4`, five epilogue copies; the last at 0x006DF470). Catalogued as CTCTrophy::vfunc_26:
//   it is vtable 0x0125FC54 slot 26 (0x0125FCBC), CTCBase's slot 26 GlobalUpdate(bool) (engine/CTCBase.h), and Ego_r.pdb / FableWin.pdb
//   ?GlobalUpdate@CTCTrophy@@UAEX_N@Z is the class's only `(bool)` virtual (`mov al,[esp+4]` .. `ret 4`).
// Callees: UpdateGainedWitnessSounds 0x006DEE80; CTCBase::GetWorld 0x00686A70 / RemoveFromGlobalTCs 0x00686DF0;
//   CMessageEventManager::FindMostRecentMessage<CMessageEventFilter_IsOfType> 0x004AB8B0 / RemoveMessage 0x00493B5B;
//   CTCCarryable::GetPCarriedByThing 0x00705180; CTCInventoryItem::GetInventoryOwner 0x005D8E90; CThingCreatureBase::FinishCurrentAction
//   0x00662650 / SetCurrentAction 0x006644F0; CCreatureAction_UnsheatheItemFromInventory ctor 0x006A0150 / dtor 0x005CC091;
//   CCreatureAction_UnsheatheToShowToSheatheTrophy ctor 0x006DF800 / dtor 0x006DF860; CDefString::GetString 0x00415DD0;
//   StartMinigame 0x006DE080; CFactionManager::GetPFaction(unsigned long) 0x006B93A0 (CCharString::GetCRC 0x00404310);
//   CThing::GetPos 0x004C73D0; CTCHeroOpinionDeedLog::PostDeed 0x006C6990; CCharString::ToWideString 0x0099E480
//   (CTCTrophy::InstructionsText 0x013BAB84); StartDialogBox 0x006DD7E0; TerminateMinigame 0x006DE280; PlaySoundW 0x006DDCD0;
//   NGameText::CDataBank::GetTextBySymbol 0x009C95E0; list<CIntelligentPointer<CThingAICreature> >::size 0x0065F17B;
//   NGameText::ConstructString(CWideString&, long, long) 0x009C7B80; SendPauseEvent 0x006DD760; CCharString / CWideString ctors / dtors.
// Shape: one if / else-if chain over the answer's ExtraData (1, 0, 2) with the message removed after it (VC7.1 duplicates the
//   RemoveMessage tail into each arm); the carried-by-a-creature arm and the unsheathe arm each call StartMinigame (cross-jumped
//   into one call); the play-game texts sit in the `else` of the page test, so their frame words are shared with the page texts
//   (retail frame 0xD4: one word fewer than with the play-game block enclosing the page block).
// Interfaces: TCI 0x47 CTCCarryable (flag byte +0x28 bit 7), 0x1A CTCInventoryItem (+0x20 & 0x4000000), 0x51 CTCHeroOpinionDeedLog
//   (+0x28 & 0x20000). Message types 16 MESSAGE_EVENT_CONVERSATION_ANSWER_YES_OR_NO (ExtraData 1 / 0 / 2) and 17 ..._CLICK_PAST.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CDEFSTRING_FORCEINLINE_CONVERSION
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGCREATUREBASE_FINISHACTION
#define FABLE_RETAIL_CTHINGCREATUREBASE_ANIMATION
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CCREATUREACTIONBASE_INTERRUPTION
#define FABLE_RETAIL_CTCINVENTORYITEM
#define FABLE_RETAIL_CTCINVENTORYITEM_INVENTORYACCESS
#define FABLE_RETAIL_CTCCARRYABLE_NONCONST_CARRIER
#define FABLE_RETAIL_CTCHEROOPINIONDEEDLOG
#define FABLE_RETAIL_CGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CWORLD_FACTIONMANAGER
#define FABLE_RETAIL_CWORLD_MESSAGEEVENTMANAGER
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_PACKED
#define FABLE_RETAIL_CMESSAGEEVENT
#define FABLE_RETAIL_CMESSAGEEVENTFILTERS
#define FABLE_RETAIL_CMESSAGEEVENTFILTERS_ISOFTYPE_CTOR_B
#define FABLE_RETAIL_CMESSAGEEVENTMANAGER_REMOVE
#define FABLE_RETAIL_CMESSAGEEVENTMANAGER_FINDRECENT_DECL_B
#define FABLE_RETAIL_CFACTIONMANAGER_BYCRC
#define FABLE_RETAIL_NGAMETEXT_CONSTRUCTSTRING_LONGS
#define FABLE_RETAIL_CTROPHYDEF
#define FABLE_RETAIL_STLP_LIST
#include "stlport/fable_containers.h"
#include "fable_string.h"
#include "engine/C3DVector.h"
#include "engine/ETCInterfaceType.h"
#include "engine/EMessageEventType.h"
#include "engine/EOpinionDeedType.h"
#include "engine/CDefinitionManager.h"
#include "engine/CThing.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CCreatureActionBase.h"
#include "engine/CCreatureAction_UnsheatheItemFromInventory.h"
#include "engine/CCreatureAction_UnsheatheToShowToSheatheTrophy.h"
#include "engine/CTCCarryable.h"
#include "engine/CTCInventoryItem.h"
#include "engine/CTCHeroOpinionDeedLog.h"
#include "engine/CMessageEvent.h"
#include "engine/CMessageEventFilters.h"
#include "engine/CMessageEventManager.h"
#include "engine/CFactionManager.h"
#include "engine/CWorld.h"
#include "fable_game.h"
#include "engine/NGameText_CDataBank.h"
#include "engine/CTrophyDef.h"
#include "engine/CTCTrophy.h"

// list<CIntelligentPointer<CThingAICreature> >::size is called out of line in retail (0x0065F17B, `lea ecx,[esi+34h] ; call`).
template <>
size_t std::list<CIntelligentPointer<CThingAICreature> >::size() const;

void CTCTrophy::GlobalUpdate(bool update_sounds)
{
    if (update_sounds)
        UpdateGainedWitnessSounds();
    if (!WaitingForConfirmation)
        return;
    CMessageEventManager& events = GetWorld().GetMessageEventManager();
    const CMessageEvent* answer =
        events.FindMostRecentMessage(CMessageEventFilter_IsOfType(MESSAGE_EVENT_CONVERSATION_ANSWER_YES_OR_NO));
    if (answer) {
        RemoveFromGlobalTCs();
        IsGlobalTC = false;
        WaitingForConfirmation = false;
        if (answer->ExtraData == 1) {
            CTCCarryable* carryable;
            ParentThing->GetTC(&carryable);
            CThing* carrier = carryable->GetPCarriedByThing();
            if (carrier && carrier->GetType() == 2) {
                StartMinigame();
            } else {
                const CTCInventoryItem* item;
                ParentThing->PeekTC(&item);
                CThingCreatureBase* owner = (CThingCreatureBase*)item->GetInventoryOwner();
                if (owner && owner->GetType() == 2) {
                    if (owner->PeekPCurrentAction() && owner->PeekPCurrentAction()->GetInterruptionGroupID() <= 0)
                        owner->FinishCurrentAction();
                    owner->SetCurrentAction(CCreatureAction_UnsheatheItemFromInventory(*owner, *ParentThing, 0, 150));
                    StartMinigame();
                }
            }
        } else if (answer->ExtraData == 0) {
            const CTCInventoryItem* item;
            ParentThing->PeekTC(&item);
            CThingCreatureBase* owner = (CThingCreatureBase*)item->GetInventoryOwner();
            if (owner && owner->GetType() == 2) {
                if (owner->PeekPCurrentAction() && owner->PeekPCurrentAction()->GetInterruptionGroupID() <= 0)
                    owner->FinishCurrentAction();
                owner->SetCurrentAction(CCreatureAction_UnsheatheToShowToSheatheTrophy(*owner, *ParentThing, PDef->ShowTrophyAnimName));
                CTCHeroOpinionDeedLog* deed_log;
                if (owner->GetTC(&deed_log)) {
                    CCharString faction_name("FACTION_BANDITS");
                    const CFaction* bandits = GetWorld().GetFactionManager().GetPFaction(faction_name.GetCRC());
                    deed_log->PostDeed(PDef->EvilTrophy ? OPINION_DEED_TYPE_SHOW_TROPHY_EVIL : OPINION_DEED_TYPE_SHOW_TROPHY_GOOD,
                                       owner->GetPos(), 0, 0, (EOpinionPostDeedType)2, false, false, bandits, true, 0);
                    deed_log->PostDeed(PDef->EvilTrophy ? OPINION_DEED_TYPE_SHOW_TROPHY_GOOD : OPINION_DEED_TYPE_SHOW_TROPHY_EVIL,
                                       owner->GetPos(), 0, 0, (EOpinionPostDeedType)2, false, false, bandits, false, 0);
                }
            }
        } else if (answer->ExtraData == 2) {
            CWideString text = InstructionsText[CurrentInstructionsPage].ToWideString();
            CWideString next(L"TEXT_GUI_NEXT");
            StartDialogBox(text, 10, next, L"", L"");
            WaitingForNextInstructionsPage = true;
        }
        events.RemoveMessage(answer);
        return;
    }
    const CMessageEvent* click =
        events.FindMostRecentMessage(CMessageEventFilter_IsOfType(MESSAGE_EVENT_CONVERSATION_CLICK_PAST));
    if (!click)
        return;
    RemoveFromGlobalTCs();
    IsGlobalTC = false;
    events.RemoveMessage(click);
    WaitingForConfirmation = false;
    if (WaitingForTerminate) {
        WaitingForTerminate = false;
        TerminateMinigame();
    } else if (WaitingForScore) {
        WaitingForScore = false;
    } else if (WaitingForNextInstructionsPage) {
        if (++CurrentInstructionsPage < 2) {
            CWideString text = InstructionsText[CurrentInstructionsPage].ToWideString();
            CWideString done(L"TEXT_GUI_DONE");
            CWideString next(L"TEXT_GUI_NEXT");
            StartDialogBox(text, 10, CurrentInstructionsPage + 1 == 2 ? done : next, L"", L"");
        } else {
            CurrentInstructionsPage = 0;
            WaitingForNextInstructionsPage = false;
            PlaySoundW(PDef->SoundStartGame);
            CWideString text = CMainGameComponent::Get()->GetTextBank().GetTextBySymbol("TEXT_GUI_TROPHY_PLAY_GAME");
            NGameText::ConstructString(text, AvailableWitnesses.size(), BestWitnessesAheadToDate);
            CWideString help(L"TEXT_GUI_HELP");
            StartDialogBox(text, 4, L"", L"", help);
        }
    } else {
        SendPauseEvent(false);
    }
}