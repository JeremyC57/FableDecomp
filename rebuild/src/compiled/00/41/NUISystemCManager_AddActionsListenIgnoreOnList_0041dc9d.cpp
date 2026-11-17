// NUISystem::CManager::AddActionsListenIgnoreOnList(CComponent* list, CComponent* first, CComponent* second, EEvent event)
// Leaf: ?AddActionsListenIgnoreOnList@CManager@NUISystem@@UAEXPAVCComponent@2@00W4EEvent@2@@Z
// Retail: Fable.exe @ 0x0041DC9D, 565 bytes, `ret 10h`; /Os + optimize("y", off) TU (ebp frame closed with `leave`, as the
//   landed CreateLevelSlider 0x00427DC9). `this` is unused.
// Callees: operator new 0x00BFEA1A, CAction::CAction 0x0042BE50, CCountedPointer<CAction>(CAction*) 0x0042AA29 / its dtor
//   0x00429160, CAction::SetTarget 0x0053FDE0 (descriptive, as the landed CScrollableList::Initialise 0x00555EB0),
//   list<CActionParam*>::push_back 0x0042B646 (out of line here); the list's virtual slots 67 (+0x10c) and 70 (+0x118).
// Layout: CAction 0x10 bytes with the param list at +4 and the action-target subobject of a component at +0x18 (landed
//   CScrollableList::Initialise model); the event parameter is a 12-byte CActionParam (vtable 0x0122F844, type 5).
// NF51 (round 7): ported onto the shared NUISystem headers (were local CActionParam / CActionParamEvent / CActionParamList /
// CActionTarget / CAction / CActionPointer / CComponent / CManager models). The counted action is CCountedPointer<NUISystem::CAction>
// (engine/CCountedPointer.h: its 5-byte destructor jumps to ?ReleaseData@?$CCountedPointer@VCAction@NUISystem@@@@QAEXXZ 0x00429160 --
// the local CActionPointer bound a made-up ??1 there); the event parameter (vtable 0x0122F844, type 5) is NUISystem::CActionParamULONG;
// 0x0053FDE0 is ?AddParam@CAction@NUISystem@@QAEXPAVCActionParam@2@AAV?$CCountedPointer@VCAction@NUISystem@@@@@Z (was SetTarget, the
// component's CActionParam base at +0x18); the list's slots 67 / 70 are AddActionOnSelected / AddActionOnInactive (were descriptive
// AddListenAction / AddIgnoreAction); the function is the virtual ?AddActionsListenIgnoreOnList@CManager@NUISystem@@UAE..@Z
// (Ego_r.pdb; engine/NUISystem_CManager.h).
#pragma optimize("s", on)
#pragma optimize("y", off)
#define FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA
#define FABLE_RETAIL_CCOUNTEDPOINTER_CALLED_CTOR
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_CMANAGER
#define FABLE_RETAIL_CACTIONPARAMULONG
#include "stlport/fable_containers.h"
#include "stlport/fable_map.h"
#include "engine/CCountedPointer.h"
#include "engine/NUISystem_CManager.h"
#include "engine/NUISystem_CComponent.h"
#include "engine/NUISystem_CAction.h"
#include "engine/NUISystem_EActionType.h"
#include "engine/NUISystem_CActionParamULONG.h"
namespace NUISystem {

void CManager::AddActionsListenIgnoreOnList(CComponent* list, CComponent* first, CComponent* second, EEvent event)
{
    CCountedPointer<CAction> listenFirst(new CAction);
    listenFirst->ActionType = UI_ACTION_TYPE_OBSERVE_EVENT;
    listenFirst->AddParam(first, listenFirst);
    listenFirst->UIActionParams.push_back(new CActionParamULONG(event));
    CCountedPointer<CAction> ignoreFirst(new CAction);
    ignoreFirst->ActionType = UI_ACTION_TYPE_IGNORE_EVENT;
    ignoreFirst->AddParam(first, ignoreFirst);
    ignoreFirst->UIActionParams.push_back(new CActionParamULONG(event));
    list->AddActionOnSelected(listenFirst);
    list->AddActionOnInactive(ignoreFirst);
    CCountedPointer<CAction> listenSecond(new CAction);
    listenSecond->ActionType = UI_ACTION_TYPE_OBSERVE_EVENT;
    listenSecond->AddParam(second, listenSecond);
    listenSecond->UIActionParams.push_back(new CActionParamULONG(event));
    CCountedPointer<CAction> ignoreSecond(new CAction);
    ignoreSecond->ActionType = UI_ACTION_TYPE_IGNORE_EVENT;
    ignoreSecond->AddParam(second, ignoreSecond);
    ignoreSecond->UIActionParams.push_back(new CActionParamULONG(event));
    list->AddActionOnSelected(ignoreSecond);
    list->AddActionOnInactive(listenSecond);
}

}
