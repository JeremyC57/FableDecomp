// 0054AC90 NUISystem::CSlider::ProcessEvent -- step, accept or reject slider input and dispatch its actions.
// Leaf: ?ProcessEvent@CSlider@NUISystem@@UAEXW4EEvent@2@@Z
// Flags: /d2inlT256
// Inline-budget: /d2inlT256 -- the CSlider string-equality helper: T64 and T128 leave two out-of-line equality calls (1374 bytes); T256 expands both as retail (rows/0054ac90/INLINE_BUDGET_PROBES.json in the Codex pass 22 hand-back).
// Retail: Fable.exe TLC VC7.1 @0x0054AC90,1454-byte assigned envelope including switch tables; CObserver this=full+4, ret4.
// Existing native entry consumes the CObserver subobject; raw PDB method this_adjust=4. Full C++ owner is retained.
#define FABLE_RETAIL_PASS22_SLIDER_PROCESS_EVENT
#define FABLE_RETAIL_CCLICKABLE_SHARED_ACTION
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_CUIDEF
#define FABLE_RETAIL_CLIST
#define FABLE_RETAIL_CMANAGER
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#include "engine/CDefPointer.h"
#include "engine/NUISystem_CUIDef.h"
#include "engine/NUISystem_CSlider.h"
#include "engine/NUISystem_CClickable.h"
#include "engine/NUISystem_CActionParamFloat.h"
#include "engine/NUISystem_CManager.h"
#include "engine/NUISystem_CList.h"
#include "engine/NUISystem_CUIMiscThingsDef.h"
namespace NUISystem {
void CSlider::ProcessEvent(EEvent event)
{
    switch (event) {
    case UI_EVENT_SLIDER_LEFT: {
        OldValue = Value;
        Value.X = (Value.X-Step.X >= Min.X) ? Value.X-Step.X : Min.X;
        if (OldValue != Value) {
            if (PAcceptButton && !PAcceptButton->IsObservingEvent(UI_EVENT_MOUSE_MOVEMENT)) {
                PAcceptButton->ObserveEvent(UI_EVENT_MOUSE_MOVEMENT);
                CRGBColour colour = PAcceptButton->GetColour();
                colour.A = 255;
                PAcceptButton->ChangeColour(colour, 0.2f, false);
            }
            for (unsigned long count = 0; count < Actions.size(); ++count) {
                CActionParamFloat* pparam = static_cast<CActionParamFloat*>(Actions[count]->UIActionParams.front());
                pparam->Value = Value.X;
                CManager::GetInstance()->Action(Actions[count]);
            }
            Move = true;
            CManager::GetInstance()->PlaySoundW(CManager::GetInstance()->GetMiscThingsDef()->SoundBack, 1);
        }
        break;
    }
    case UI_EVENT_SLIDER_RIGHT: {
        OldValue = Value;
        Value.X = (Value.X+Step.X <= Max.X) ? Value.X+Step.X : Max.X;
        if (OldValue != Value) {
            if (PAcceptButton && !PAcceptButton->IsObservingEvent(UI_EVENT_MOUSE_MOVEMENT)) {
                PAcceptButton->ObserveEvent(UI_EVENT_MOUSE_MOVEMENT);
                CRGBColour colour = PAcceptButton->GetColour();
                colour.A = 255;
                PAcceptButton->ChangeColour(colour, 0.2f, false);
            }
            for (unsigned long count = 0; count < Actions.size(); ++count) {
                CActionParamFloat* pparam = static_cast<CActionParamFloat*>(Actions[count]->UIActionParams.front());
                pparam->Value = Value.X;
                CManager::GetInstance()->Action(Actions[count]);
            }
            Move = true;
            CManager::GetInstance()->PlaySoundW(CManager::GetInstance()->GetMiscThingsDef()->SoundForward, 1);
        }
        break;
    }
    case UI_EVENT_BACK:
        if (CList::GetPreviousSelected().size() > 1) {
            CList::GetPreviousSelected().front()->ChangeState(6);
            CList::GetPreviousSelected().pop_front();
            CList::GetPreviousSelected().front()->ChangeState(3);
            CManager::GetInstance()->RemoveObserver(this);
        }
        CManager::GetInstance()->PlaySoundW(CManager::GetInstance()->GetMiscThingsDef()->SoundBack, 1);
        CManager::GetInstance()->SetChange(CManager::UI_CHANGE_REJECT);
        if (PActionsOnBack) {
            for (std::list<CCountedPointer<CAction> >::iterator it = PActionsOnBack->begin(); it != PActionsOnBack->end(); ++it)
                CManager::GetInstance()->Action(*it);
        }
        break;
    case UI_EVENT_SELECT:
        if (CList::GetPreviousSelected().size() > 1) {
            CList::GetPreviousSelected().front()->ChangeState(6);
            CList::GetPreviousSelected().pop_front();
            CList::GetPreviousSelected().front()->ChangeState(3);
            CManager::GetInstance()->RemoveObserver(this);
        }
        CManager::GetInstance()->PlaySoundW(CManager::GetInstance()->GetMiscThingsDef()->SoundForward, 1);
        CManager::GetInstance()->SetChange(CManager::UI_CHANGE_ACCEPT);
        if (PActionsOnSelect) {
            for (std::list<CCountedPointer<CAction> >::iterator it = PActionsOnSelect->begin(); it != PActionsOnSelect->end(); ++it)
                CManager::GetInstance()->Action(*it);
        }
        break;
    }
}
}