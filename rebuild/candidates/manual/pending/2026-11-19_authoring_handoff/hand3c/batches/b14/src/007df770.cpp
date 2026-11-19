// CTCWallMountEffects::FrameUpdate() -- while waiting for the hero's answer to a wall-mount prompt, take the latest yes/no answer: yes to the effect prompt runs the effect, yes to the remove prompt starts the creature's use-wall-mount action; no to the effect prompt asks the remove question instead; otherwise the wait ends
// Leaf: ?FrameUpdate@CTCWallMountEffects@@UAEXXZ
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x007DF770, 437 bytes, thiscall, no frame pointer, `ret`. Vtable slot 9 of CTCWallMountEffects.
// Identity: FableWin.pdb ?FrameUpdate@CTCWallMountEffects@@UAEXXZ 0x02766430 (the catalog's vfunc_9).
// Shape: FableWin 0x02766430 statement for statement: GetWorld().GetMessageEventManager(), FindMostRecentMessage(
//   CMessageEventFilter_IsOfType(MESSAGE_EVENT_CONVERSATION_ANSWER_YES_OR_NO)); ExtraData == 1: CONFIRMATION_EFFECT -> DoEffect(),
//   CONFIRMATION_REMOVE with a live creature -> SetCurrentAction(CCreatureAction_UseWallMount(*creature, *ParentThing)), then the
//   creature pointer reset and the wait cleared; else CONFIRMATION_EFFECT -> the "Do you want to take the item?" Yes / No subtitle
//   (cut-scene main font, 4, 0.0f, false, true) and CONFIRMATION_REMOVE; else the reset; RemoveMessage in every answered case.
// Callees: CTCBase::GetWorld 0x00686A70, FindMostRecentMessage<CMessageEventFilter_IsOfType> 0x004AB8B0, DoEffect 0x007DF6A0,
//   IS_THING_ALIVE<CIntelligentPointer<CThingCreatureBase> > 0x006961F0, CBaseIntelligentPointer ctor / GetPItem / SetPItem / dtor
//   0x00A01B10 / 0x00A01B50 / 0x00A01B90 / 0x00A01C10, CCreatureAction_UseWallMount ctor 0x00843610 / dtor 0x007DF380,
//   CThingCreatureBase::SetCurrentAction 0x006644F0, CCharString 0x0099EBF0 / 0x0099EAE0, CSubtitleRenderer::SetText 0x00492FB4,
//   CMessageEventManager::RemoveMessage 0x00493B5B.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_DEREFERENCE
#define FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN
#define FABLE_RETAIL_CINTELLIGENTPOINTER_GET_ORIGINAL_CHAIN
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COMPARE_NULL_INT
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_IS_THING_ALIVE_NOINLINE
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CCREATUREACTION_USEWALLMOUNT_CLASS
#define FABLE_RETAIL_CGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT_TEXTBANK
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_CPLAYERGUI
#define FABLE_RETAIL_CSUBTITLERENDERER
#define FABLE_RETAIL_CSUBTITLERENDERER_SETTEXT_CHAR
#define FABLE_RETAIL_CSUBTITLERENDERER_SETTEXT_CHAR4
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CWORLD_MESSAGEEVENTMANAGER
#define FABLE_RETAIL_CMESSAGEEVENT
#define FABLE_RETAIL_CMESSAGEEVENTFILTERS
#define FABLE_RETAIL_CMESSAGEEVENTFILTERS_ISOFTYPE_CTOR
#define FABLE_RETAIL_CMESSAGEEVENTMANAGER_REMOVE
#define FABLE_RETAIL_CMESSAGEEVENTMANAGER_FINDRECENT_DECL
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_STLP_LIST
#include "stlport/fable_containers.h"
#include "fable_string.h"
#include "engine/EMessageEventType.h"
#include "engine/CThing.h"
#include "engine/CThingCreatureBase.h"
#include "engine/ThingAlive.h"
#include "engine/CCreatureActionBase.h"
#include "engine/CCreatureAction_UseWallMount.h"
#include "engine/CMessageEvent.h"
#include "engine/CMessageEventFilters.h"
#include "engine/CMessageEventManager.h"
#include "engine/CWorld.h"
#include "engine/CMainGameComponent.h"
#include "engine/CPlayerGui.h"
#include "engine/CSubtitleRenderer.h"
#include "engine/CTCWallMountEffects.h"

void CTCWallMountEffects::FrameUpdate()
{
    if (WaitingForConfirmation != CONFIRMATION_NONE) {
        CMessageEventManager& message_manager = GetWorld().GetMessageEventManager();
        const CMessageEvent* pmessage =
            message_manager.FindMostRecentMessage(CMessageEventFilter_IsOfType(MESSAGE_EVENT_CONVERSATION_ANSWER_YES_OR_NO));
        if (pmessage) {
            if (pmessage->ExtraData == 1) {
                if (WaitingForConfirmation == CONFIRMATION_EFFECT)
                    DoEffect();
                else if (WaitingForConfirmation == CONFIRMATION_REMOVE && IS_THING_ALIVE(CreaturePerformingAction))
                    CreaturePerformingAction->SetCurrentAction(CCreatureAction_UseWallMount(*CreaturePerformingAction, *ParentThing));
                CreaturePerformingAction = CIntelligentPointer<CThingCreatureBase>(0);
                WaitingForConfirmation = CONFIRMATION_NONE;
            } else if (WaitingForConfirmation == CONFIRMATION_EFFECT) {
                CPlayerGui::Get().GetSubtitleRenderer().SetText(CMainGameComponent::Get()->PeekCutSceneMainFont(),
                    "Do you want to take the item?", "", "Yes", "No", 4, 0.0f, false, true);
                WaitingForConfirmation = CONFIRMATION_REMOVE;
            } else {
                CreaturePerformingAction = CIntelligentPointer<CThingCreatureBase>(0);
                WaitingForConfirmation = CONFIRMATION_NONE;
            }
            message_manager.RemoveMessage(pmessage);
        }
    }
}
