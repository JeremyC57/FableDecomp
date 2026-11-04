// NUISystem::CSlider::ProcessEvent(EEvent event) -- slider input: step the value left/right by Step.X (clamped to Min.X / Max.X) and, when it moved, push it into every action and play the step sound, or confirm (select) / cancel (back) through the manager and run the matching action list
// Grade: DIFFER(1454v1454) -- residue: in the LEFT case's action loop VC7.1 schedules `lea ebx,[edi*8]` before the first element access (ours `mov edx,[ebx+ecx]`, retail `mov edx,[ecx+edi*8]` with the lea after); the RIGHT case's identical loop matches; jump-table relocations equal; 26 diff lines, 45 grades
// Leaf: ?ProcessEvent@CSlider@NUISystem@@UAEXW4EEvent@2@@Z
// Flags: none
// Retail: Fable.exe @ 0x0054AC90, 1454 bytes (`ret 4`). CObserver vtable 0x01248CE0 slot 1 (this = the CObserver subobject, full-4).
// Jump table: SELECT(4) 0x0054B107, BACK(5) 0x0054AFF1, LEFT(20) 0x0054ACB6, RIGHT(21) 0x0054AE51, default 0x0054B20C.
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
        float value = Value.X - Step.X;
        Value.X = (value >= Min.X) ? value : Min.X;
        if (!(OldValue == Value)) {
            if (PAcceptButton && !PAcceptButton->IsObservingEvent(UI_EVENT_MOUSE_MOVEMENT)) {
                PAcceptButton->ObserveEvent(UI_EVENT_MOUSE_MOVEMENT);
                CRGBColour colour = PAcceptButton->GetColour();
                colour.A = 255;
                PAcceptButton->ChangeColour(colour, 0.2f, false);
            }
            for (unsigned long i = 0; i < Actions.size(); ++i) {
                static_cast<CActionParamFloat*>(Actions[i]->UIActionParams.front())->Value = Value.X;
                CManager::GetInstance()->Action(Actions[i]);
            }
            Move = true;
            CManager::GetInstance()->PlaySoundW(CManager::GetInstance()->GetMiscThingsDef()->SoundBack, 1);
        }
        break;
    }
    case UI_EVENT_SLIDER_RIGHT: {
        OldValue = Value;
        float value = Value.X + Step.X;
        Value.X = (value <= Max.X) ? value : Max.X;
        if (!(OldValue == Value)) {
            if (PAcceptButton && !PAcceptButton->IsObservingEvent(UI_EVENT_MOUSE_MOVEMENT)) {
                PAcceptButton->ObserveEvent(UI_EVENT_MOUSE_MOVEMENT);
                CRGBColour colour = PAcceptButton->GetColour();
                colour.A = 255;
                PAcceptButton->ChangeColour(colour, 0.2f, false);
            }
            for (unsigned long i = 0; i < Actions.size(); ++i) {
                static_cast<CActionParamFloat*>(Actions[i]->UIActionParams.front())->Value = Value.X;
                CManager::GetInstance()->Action(Actions[i]);
            }
            Move = true;
            CManager::GetInstance()->PlaySoundW(CManager::GetInstance()->GetMiscThingsDef()->SoundForward, 1);
        }
        break;
    }
    case UI_EVENT_BACK:
        if (CList::GPreviousSelected.size() > 1) {
            CList::GPreviousSelected.front()->ChangeState(6);
            CList::GPreviousSelected.pop_front();
            CList::GPreviousSelected.front()->ChangeState(3);
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
        if (CList::GPreviousSelected.size() > 1) {
            CList::GPreviousSelected.front()->ChangeState(6);
            CList::GPreviousSelected.pop_front();
            CList::GPreviousSelected.front()->ChangeState(3);
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
