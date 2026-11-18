// NUISystem::CSlider::GetDataFromDef() -- read the slider's UI def: dimensions, range, step and default action, reset the values; install the def's on-selected action and an accept action, queue the default action, create the two arrows with their slider-left/right click actions, read the value from the profile, and give every queued action the slider value as a float parameter
// Leaf: ?GetDataFromDef@CSlider@NUISystem@@QAEXXZ
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x0054B4B0, 1381 bytes, plain `ret`.
// Identity: Ego_r.pdb + FableWin.pdb ?GetDataFromDef@CSlider@NUISystem@@QAEXXZ (engine/NUISystem_CSlider.h); the twin of the landed
//   CTextSlider::GetDataFromDef 0x00549B20 (NF76), whose recipe this row follows.
// Callees: LoadAndReturnDef (slot 108, +0x1B0), AddActionOnSelected (67, +0x10C), the arrows' AddActionOnLeftUnclicked (80, +0x140),
//   GetValueFromProfile (150, +0x258); operator new 0x00BFEA1A, CAction::CAction 0x0042BE50 and CCountedPointer<CAction>(CAction*)
//   0x0042AA29 (out of line at most sites, the accept action's CAction expanded: VC7.1's own inlining decision under
//   FABLE_RETAIL_CACTION_INLINE_CTOR, as in 0x00549B20), CCountedPointer<CAction>::DeleteData 0x00429F43, operator delete 0x00BFE9BC,
//   malloc 0x00BFEA0E, vector<CCountedPointer<CAction> >::_M_insert_overflow 0x0054A2A0, CManager::GetInstance 0x0041E5F2 /
//   CreateComponent(long) 0x0041D21B, CAction::AddParam(CActionParam*, CCountedPointer<CAction>&) 0x0053FDE0 (the slider's CActionParam
//   base, +0x18); vtables CActionParamSLONG 0x012366F4 and CActionParamFloat 0x01248CD0 (.?AVCActionParamFloat@NUISystem@@).
// Headers: engine/NUISystem_CSlider.h (Dimensions +0x184, Min / Max / Step / Value, Move +0x1AC, OldValue / StartValue, Actions +0x1C0,
//   PDefAction +0x1D4, PLeftArrow / PRightArrow, PreviousValue +0x1E0), engine/NUISystem_CUIDef.h (Action +0xC4, ActionOnSelected +0xCC,
//   Dimensions / Min / Max / Step +0x164.., SliderLeft +0x18C, SliderRight +0x190), engine/NUISystem_CAction.h (ADDPARAM_INLINE),
//   engine/NUISystem_CActionParamSLONG.h, engine/NUISystem_CActionParamFloat.h.
// NF79 (round 7, q10962 / q10963): ported onto the shared headers with NF76's 0x00549B20 macro block; every local model is gone
//   (C2DVector, CRetailList*, CActionParam*, SCountBlock, CAction / CInlineAction / CInlineActionPointer, CRetailVector + its stand-in
//   _M_insert_overflow body, CSliderDef / CDefPointer, CArrowComponent, CManager, CSlider) together with their offset casts.
//   0x0053FDE0 binds ?AddParam@CAction@NUISystem@@QAEXPAVCActionParam@2@AAV?$CCountedPointer@VCAction@NUISystem@@@@@Z (was the stand-in
//   AddUIActionParam), 0x0054A2A0 the STLport vector::_M_insert_overflow (was CRetailVector), 0x01248CD0 ??_7CActionParamFloat (was FLOAT).
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
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#include "engine/CDefPointer.h"
#include "engine/NUISystem_CUIDef.h"
#include "engine/NUISystem_CSlider.h"
#include "engine/NUISystem_CAction.h"
#include "engine/NUISystem_CActionParamSLONG.h"
#include "engine/NUISystem_CActionParamFloat.h"
#include "engine/NUISystem_CManager.h"

namespace NUISystem {
void CSlider::GetDataFromDef()
{
    CDefPointer<const CUIDef> pdefinition = LoadAndReturnDef();
    Dimensions = pdefinition->Dimensions;
    Min = pdefinition->Min;
    Max = pdefinition->Max;
    Step = pdefinition->Step;
    PreviousValue = Value = OldValue = StartValue = Min;
    Move = false;
    PDefAction = pdefinition->Action;
    if (pdefinition->ActionOnSelected) {
        CCountedPointer<CAction> paction(new CAction);
        paction->ActionType = pdefinition->ActionOnSelected;
        AddActionOnSelected(paction);
    }
    CCountedPointer<CAction> paction_accept(new CAction);
    paction_accept->ActionType = UI_ACTION_TYPE_ACCEPT_CHANGES;
    AddActionOnSelected(paction_accept);
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
    GetValueFromProfile();
    PreviousValue = Value;
    if (Max.X < Min.X)
        Max.X = Min.X;
    if (!Actions.empty()) {
        CActionParam* param = new CActionParamFloat(Value.X);
        for (unsigned long i = 0; i < Actions.size(); ++i)
            Actions[i]->AddParam(param);
    }
}
}
