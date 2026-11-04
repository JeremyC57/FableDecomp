// NUISystem::CTextSlider::ProcessEvent(EEvent event) -- slider input: outside the input delay, run the component handler, then step the selection left/right (wrapping at 0 / MaxChild) and push it into every action, or confirm (select) / cancel (back) through the manager and run the matching action list
// Leaf: ?ProcessEvent@CTextSlider@NUISystem@@UAEXW4EEvent@2@@Z
// Flags: none
//
// Retail: Fable.exe (TLC, VC7.1) @ 0x00549440, 1174 bytes (incl. the 5-entry jump table + 18-byte index table), `ret 4`.
// Identity: CTextSlider's CObserver vtable 0x01248A68 slot 1 (CObserver::ProcessEvent override; `this` is the CObserver
//   subobject, full object - 4, so members read at PDB-retail offset - 4); Ego_r.pdb ?ProcessEvent@CTextSlider@NUISystem@@
//   UAEXW4EEvent@2@@Z. Events: SLIDER_LEFT 20 / SLIDER_RIGHT 21 / BACK 5 / SELECT 4 (engine/NUISystem_enums.h); the jump table
//   pairs were checked by hand (index bytes 00 01 04.. 02 03 -> SELECT 0x005497FE, BACK 0x0054974A, LEFT 0x00549486, RIGHT
//   0x005495EA, default 0x005498A5).
// Callees: CComponent::ProcessEvent 0x0052E830 (direct), CManager::GetInstance 0x0041E5F2 and its virtuals RemoveObserver
//   (slot 5), Action (14), SetChange (32), PlaySoundW (40), GetMiscThingsDef (42; CDefPointer<CUIMiscThingsDef> by hidden
//   pointer, SoundForward +0x1A0); the accept button's CObserver IsObservingEvent / ObserveEvent (slots 2 / 3) and its
//   GetColour (14) / ChangeColour (38); CList::GPreviousSelected 0x013B8A98 (list<CList*>) size() 0x0042B707 and
//   pop_front() 0x0053C7D9 (both out of line), the front list's ChangeState(unsigned long) (slot 48).
// Layout: engine/NUISystem_CTextSlider.h (new): SelectedChild +0x15C, PreviouslySelectedChild +0x160, Move +0x168, Actions
//   +0x16C, PActionsOnBack +0x178, PActionsOnSelect +0x17C, LastInputTime +0x180, InputDelay +0x184, PAcceptButton +0x198,
//   MaxChild +0x19C; each action's first UI parameter is a CActionParamSLONG (+8 Value).
// Shape: the four cases in source order LEFT, RIGHT, BACK, SELECT (VC7.1 keeps it; SELECT before BACK swaps the blocks);
//   the vector size() is re-read every pass; the action lists are walked with end() re-read through the member pointer.
// Headers: engine/NUISystem_CTextSlider.h (new); stlport/fable_containers.h (FABLE_RETAIL_STLP_LIST: STLport erase/pop_front).
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_C2DBOXF
#define FABLE_RETAIL_CUIDEF
#define FABLE_RETAIL_CLIST
#define FABLE_RETAIL_CMANAGER
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_CACTIONPARAMSLONG
#include "engine/CDefPointer.h"
#include "engine/NUISystem_CUIDef.h"
#include "engine/NUISystem_CTextSlider.h"
#include "engine/NUISystem_CClickable.h"
#include "engine/NUISystem_CActionParamSLONG.h"
#include "engine/NUISystem_CManager.h"
#include "engine/NUISystem_CList.h"
#include "engine/NUISystem_CUIMiscThingsDef.h"
namespace NUISystem {
void CTextSlider::ProcessEvent(EEvent event)
{
    if (Time - LastInputTime < InputDelay)
        return;
    CComponent::ProcessEvent(event);
    switch (event) {
    case UI_EVENT_SLIDER_LEFT:
        PreviouslySelectedChild = SelectedChild;
        if (SelectedChild == 0)
            SelectedChild = MaxChild;
        else
            --SelectedChild;
        if (PreviouslySelectedChild != SelectedChild) {
            if (PAcceptButton && !PAcceptButton->IsObservingEvent(UI_EVENT_MOUSE_MOVEMENT)) {
                PAcceptButton->ObserveEvent(UI_EVENT_MOUSE_MOVEMENT);
                CRGBColour colour = PAcceptButton->GetColour();
                colour.A = 255;
                PAcceptButton->ChangeColour(colour, 0.2f, false);
            }
            for (unsigned long i = 0; i < Actions.size(); ++i) {
                static_cast<CActionParamSLONG*>(Actions[i]->UIActionParams.front())->Value = SelectedChild;
                CManager::GetInstance()->Action(Actions[i]);
            }
            Move = true;
            LastInputTime = Time;
            CManager::GetInstance()->PlaySoundW(CManager::GetInstance()->GetMiscThingsDef()->SoundForward, 1);
        }
        break;
    case UI_EVENT_SLIDER_RIGHT:
        PreviouslySelectedChild = SelectedChild;
        if (SelectedChild == MaxChild)
            SelectedChild = 0;
        else
            ++SelectedChild;
        if (PreviouslySelectedChild != SelectedChild) {
            if (PAcceptButton && !PAcceptButton->IsObservingEvent(UI_EVENT_MOUSE_MOVEMENT)) {
                PAcceptButton->ObserveEvent(UI_EVENT_MOUSE_MOVEMENT);
                CRGBColour colour = PAcceptButton->GetColour();
                colour.A = 255;
                PAcceptButton->ChangeColour(colour, 0.2f, false);
            }
            for (unsigned long i = 0; i < Actions.size(); ++i) {
                static_cast<CActionParamSLONG*>(Actions[i]->UIActionParams.front())->Value = SelectedChild;
                CManager::GetInstance()->Action(Actions[i]);
            }
            Move = true;
            LastInputTime = Time;
            CManager::GetInstance()->PlaySoundW(CManager::GetInstance()->GetMiscThingsDef()->SoundForward, 1);
        }
        break;
    case UI_EVENT_BACK:
        if (CList::GPreviousSelected.size() > 1) {
            CList::GPreviousSelected.front()->ChangeState(6);
            CList::GPreviousSelected.pop_front();
            CList::GPreviousSelected.front()->ChangeState(3);
            CManager::GetInstance()->RemoveObserver(this);
        }
        CManager::GetInstance()->SetChange(CManager::UI_CHANGE_REJECT);
        if (PActionsOnBack) {
            for (std::list<CCountedPointer<CAction> >::iterator it = PActionsOnBack->begin(); it != PActionsOnBack->end(); ++it)
                CManager::GetInstance()->Action(*it);
        }
        LastInputTime = Time;
        break;
    case UI_EVENT_SELECT:
        if (CList::GPreviousSelected.size() > 1) {
            CList::GPreviousSelected.front()->ChangeState(6);
            CList::GPreviousSelected.pop_front();
            CList::GPreviousSelected.front()->ChangeState(3);
            CManager::GetInstance()->RemoveObserver(this);
        }
        CManager::GetInstance()->SetChange(CManager::UI_CHANGE_ACCEPT);
        if (PActionsOnSelect) {
            for (std::list<CCountedPointer<CAction> >::iterator it = PActionsOnSelect->begin(); it != PActionsOnSelect->end(); ++it)
                CManager::GetInstance()->Action(*it);
        }
        LastInputTime = Time;
        break;
    }
}
}