// NUISystem::CTextSlider::GetDataFromDef() -- read the slider's UI def: input delay and default action, clear the selection state; install the def's on-selected action, queue its default action, create the right and left arrow components (def SliderRight / SliderLeft) whose left-unclick sends the slider an owned SLIDER_RIGHT / SLIDER_LEFT event; finally install the accept-changes action, record the last child index and read the value from the profile
// Leaf: ?GetDataFromDef@CTextSlider@NUISystem@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00549B20, 1079 bytes (+ int3 padding), plain `ret`; called by the CTextSlider ctor 0x00549F60.
// Identity: Ego_r.pdb ?GetDataFromDef@CTextSlider@NUISystem@@QAEXXZ (0x005C3860, 1369 bytes; also FableWin.pdb), declared on
//   engine/NUISystem_CTextSlider.h; Ego_d.pdb's record has exactly this function's locals (pdefinition, paction x2, paction_click
//   x2, paction_accept). The catalogue's "Initialize" was a forge autoname.
// Callees: LoadAndReturnDef (slot 108, +0x1B0), AddActionOnSelected (67, +0x10C), the arrows' AddActionOnLeftUnclicked (80,
//   +0x140), GetValueFromProfile (143, +0x23C, the slider's 0x00548790); operator new 0x00BFEA1A, CAction::CAction 0x0042BE50 and
//   CCountedPointer<CAction>(CAction*) 0x0042AA29 (out of line for the first four, expanded for the last: VC7.1's own inlining
//   decision), CCountedPointer<CAction>::DeleteData 0x00429F43, operator delete 0x00BFE9BC, malloc 0x00BFEA0E,
//   vector<CCountedPointer<CAction> >::_M_insert_overflow 0x0054A2A0, CManager::GetInstance 0x0041E5F2 / CreateComponent(long)
//   0x0041D21B, CAction::AddParam(CActionParam*, CCountedPointer<CAction>&) 0x0053FDE0 (the slider's CActionParam base, +0x18);
//   CActionParamSLONG vtable 0x012366F4 (??_7CActionParamSLONG@NUISystem@@6B@).
// Headers: engine/NUISystem_CTextSlider.h, engine/NUISystem_CUIDef.h (Action +0xC4, ActionOnSelected +0xCC, InputDelay +0x184,
//   SliderLeft +0x18C, SliderRight +0x190), engine/NUISystem_CAction.h (FABLE_RETAIL_CACTION_ADDPARAM_INLINE: AddParam(CActionParam*)
//   expanded here), engine/NUISystem_CActionParamSLONG.h, stlport/fable_containers.h (vector overflow visible, out of line).
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_C2DBOXF
#define FABLE_RETAIL_CUIDEF
#define FABLE_RETAIL_CMANAGER
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_STLP_VECTOR
#define FABLE_RETAIL_STLP_VECTOR_OVERFLOW_OUTOFLINE
#define FABLE_RETAIL_CACTIONPARAMSLONG
#define FABLE_RETAIL_CACTION_INLINE_CTOR
#define FABLE_RETAIL_CACTION_ADDPARAM_INLINE
#include "engine/CDefPointer.h"
#include "engine/NUISystem_CUIDef.h"
#include "engine/NUISystem_CTextSlider.h"
#include "engine/NUISystem_CAction.h"
#include "engine/NUISystem_CActionParamSLONG.h"
#include "engine/NUISystem_CManager.h"
namespace NUISystem {
void CTextSlider::GetDataFromDef()
{
    CDefPointer<const CUIDef> pdefinition = LoadAndReturnDef();
    InputDelay = pdefinition->InputDelay;
    PDefAction = pdefinition->Action;
    StartSelectedChild = 0;
    PreviouslySelectedChild = 0;
    SelectedChild = 0;
    OriginalSelectedChild = 0;
    Move = false;
    if (pdefinition->ActionOnSelected) {
        CCountedPointer<CAction> paction(new CAction);
        paction->ActionType = pdefinition->ActionOnSelected;
        AddActionOnSelected(paction);
    }
    if (pdefinition->Action) {
        CCountedPointer<CAction> paction(new CAction);
        paction->ActionType = pdefinition->Action;
        Actions.push_back(paction);
    }
    if (pdefinition->SliderRight) {
        PRightArrow = CManager::GetInstance()->CreateComponent(pdefinition->SliderRight);
        CCountedPointer<CAction> paction_click(new CAction);
        paction_click->ActionType = UI_ACTION_TYPE_SEND_OWNED_EVENT_FORCE_OBSERVATION;
        paction_click->AddParam(new CActionParamSLONG(UI_EVENT_SLIDER_RIGHT));
        paction_click->AddParam(this, paction_click);
        PRightArrow->AddActionOnLeftUnclicked(paction_click);
    }
    if (pdefinition->SliderLeft) {
        PLeftArrow = CManager::GetInstance()->CreateComponent(pdefinition->SliderLeft);
        CCountedPointer<CAction> paction_click(new CAction);
        paction_click->ActionType = UI_ACTION_TYPE_SEND_OWNED_EVENT_FORCE_OBSERVATION;
        paction_click->AddParam(new CActionParamSLONG(UI_EVENT_SLIDER_LEFT));
        paction_click->AddParam(this, paction_click);
        PLeftArrow->AddActionOnLeftUnclicked(paction_click);
    }
    CCountedPointer<CAction> paction_accept(new CAction);
    paction_accept->ActionType = UI_ACTION_TYPE_ACCEPT_CHANGES;
    AddActionOnSelected(paction_accept);
    MaxChild = Children.size() - 1;
    GetValueFromProfile();
}
}
