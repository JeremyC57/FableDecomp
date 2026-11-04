// NUISystem::CFrontEndList::ChangeState(unsigned long state) -- per-state observer registration for a front-end list, then the CChangingStateComponent state change (CList's own ChangeState is bypassed)
// Leaf: ?ChangeState@CFrontEndList@NUISystem@@UAEXK@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x0054CBF0, 1408 bytes including the 7-entry case table at 0x0054D154, thiscall, `ret 4`.
// Identity: CFrontEndList vtable 0x01249224 slot 48 (catalog "vfunc_48"), the ChangeState(unsigned long) slot of CComponent's table
//   (engine/NUISystem_CComponent.h; CList's slot-48 override is CList::ChangeState 0x0053A0A1, landed); Ego_r.pdb / FableWin.pdb
//   ?ChangeState@CFrontEndList@NUISystem@@UAEXK@Z. Every case ends in CChangingStateComponent::ChangeState 0x0052CF40 (as CList's does).
// Shape: a jump-table switch whose case bodies VC7.1 lays out in source order; retail's order is ACTIVE(1), INACTIVE(0),
//   UNSELECTED(4), LIST_ACTIVE(5), LIST_INACTIVE(6), SELECTED(3), then default (DEAD 2 and > 6). The case-to-target pairing was
//   checked against the retail table (0 -> +0x58, 1 -> +0x1A, 2 -> +0x553, 3 -> +0x466, 4 -> +0xFE, 5 -> +0x19E, 6 -> +0x3A4).
//  - 1: observe self (UP, DOWN).  0 / 4: stop observing self and the scrolling children (then OnObserverRemove).
//  - 5: DoRecomputeOffsets() + SetRecomputeOffsets(false) when RecomputeOffsets is set; when there are scrolling children, find the
//    selected child among Children (the CCountedPointer vector at +0xB0) and put every non-scrolling child into the custom state
//    UI_STATE_CUSTOM + index when it has one (HasState / HasStateInChildren), else ACTIVE -- every non-scrolling child ACTIVE when the
//    selection is not found (a `found` flag VC7.1 threads away); then the base state, then observe the scrolling children
//    (MOUSE_MOVEMENT) and the selection (DELETE, SELECT; the last call is a tail jump).
//  - 6: as 0, then the base state INACTIVE when CurrentState is INACTIVE, else LIST_INACTIVE.
//  - 3: observe self (UP, DOWN), the scrolling children and the selection (DELETE, SELECT), then the base state.
// Callees: NUISystem::CManager::GetInstance 0x0041E5F2 (AddObserver slot 2, RemoveObserver slot 5),
//   NUISystem::CChangingStateComponent::ChangeState 0x0052CF40; virtuals: CObserver::ObserveEvent (+0x0C on the +4 subobject),
//   CComponent::OnObserverRemove (slot 109), HasState (44), HasStateInChildren (45), ChangeState (48), CList::DoRecomputeOffsets
//   (slot 184), CList::SetRecomputeOffsets (181).
// Model: the landed CList::ChangeState 0x0053A0A1 (same TU style and helpers; this class's TU is /O2, no optimize pragma).
// Headers: engine/NUISystem_CFrontEndList.h (new, chunk 753), engine/NUISystem_CList.h (FABLE_RETAIL_CLIST), engine/NUISystem_CManager.h.
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_CLIST
#define FABLE_RETAIL_CMANAGER
#include "engine/NUISystem_CFrontEndList.h"
#include "engine/NUISystem_CManager.h"
using namespace NUISystem;
void CFrontEndList::ChangeState(unsigned long state)
{
    switch (state)
    {
    case UI_STATE_ACTIVE:
        CManager::GetInstance()->AddObserver(this);
        ObserveEvent(UI_EVENT_UP);
        ObserveEvent(UI_EVENT_DOWN);
        CChangingStateComponent::ChangeState(UI_STATE_ACTIVE);
        break;
    case UI_STATE_INACTIVE:
        CManager::GetInstance()->RemoveObserver(this);
        if (ScrollingChildren.size() > 0)
        {
            for (unsigned long i = 0; i < ScrollingChildren.size(); ++i)
            {
                CManager::GetInstance()->RemoveObserver(ScrollingChildren[i]);
                ScrollingChildren[i]->OnObserverRemove();
            }
        }
        CChangingStateComponent::ChangeState(UI_STATE_INACTIVE);
        break;
    case UI_STATE_UNSELECTED:
        CManager::GetInstance()->RemoveObserver(this);
        if (ScrollingChildren.size() > 0)
        {
            for (unsigned long i = 0; i < ScrollingChildren.size(); ++i)
            {
                CManager::GetInstance()->RemoveObserver(ScrollingChildren[i]);
                ScrollingChildren[i]->OnObserverRemove();
            }
        }
        CChangingStateComponent::ChangeState(UI_STATE_UNSELECTED);
        break;
    case UI_STATE_LIST_ACTIVE:
        if (RecomputeOffsets)
        {
            DoRecomputeOffsets();
            SetRecomputeOffsets(false);
        }
        if (ScrollingChildren.size() > 0)
        {
            bool found = false;
            for (unsigned long j = 0; j < Children.size(); ++j)
            {
                if (Children[j].Data == ScrollingChildren[SelectedChild])
                {
                    for (unsigned long i = 0; i < NonScrollingChildren.size(); ++i)
                    {
                        if (NonScrollingChildren[i]->HasState(j + UI_STATE_CUSTOM) || NonScrollingChildren[i]->HasStateInChildren(j + UI_STATE_CUSTOM))
                            NonScrollingChildren[i]->ChangeState(j + UI_STATE_CUSTOM);
                        else
                            NonScrollingChildren[i]->ChangeState(UI_STATE_ACTIVE);
                    }
                    found = true;
                    break;
                }
            }
            if (!found)
            {
                for (unsigned long i = 0; i < NonScrollingChildren.size(); ++i)
                    NonScrollingChildren[i]->ChangeState(UI_STATE_ACTIVE);
            }
        }
        CChangingStateComponent::ChangeState(UI_STATE_LIST_ACTIVE);
        if (ScrollingChildren.size() > 0)
        {
            for (unsigned long i = 0; i < ScrollingChildren.size(); ++i)
            {
                CManager::GetInstance()->AddObserver(ScrollingChildren[i]);
                ScrollingChildren[i]->ObserveEvent(UI_EVENT_MOUSE_MOVEMENT);
            }
            ScrollingChildren[SelectedChild]->ObserveEvent(UI_EVENT_DELETE);
            ScrollingChildren[SelectedChild]->ObserveEvent(UI_EVENT_SELECT);
        }
        break;
    case UI_STATE_LIST_INACTIVE:
        CManager::GetInstance()->RemoveObserver(this);
        if (ScrollingChildren.size() > 0)
        {
            for (unsigned long i = 0; i < ScrollingChildren.size(); ++i)
            {
                CManager::GetInstance()->RemoveObserver(ScrollingChildren[i]);
                ScrollingChildren[i]->OnObserverRemove();
            }
        }
        if (CurrentState == UI_STATE_INACTIVE)
            CChangingStateComponent::ChangeState(UI_STATE_INACTIVE);
        else
            CChangingStateComponent::ChangeState(UI_STATE_LIST_INACTIVE);
        break;
    case UI_STATE_SELECTED:
        CManager::GetInstance()->AddObserver(this);
        ObserveEvent(UI_EVENT_UP);
        ObserveEvent(UI_EVENT_DOWN);
        if (ScrollingChildren.size() > 0)
        {
            for (unsigned long i = 0; i < ScrollingChildren.size(); ++i)
            {
                CManager::GetInstance()->AddObserver(ScrollingChildren[i]);
                ScrollingChildren[i]->ObserveEvent(UI_EVENT_MOUSE_MOVEMENT);
            }
            ScrollingChildren[SelectedChild]->ObserveEvent(UI_EVENT_DELETE);
            ScrollingChildren[SelectedChild]->ObserveEvent(UI_EVENT_SELECT);
        }
        CChangingStateComponent::ChangeState(UI_STATE_SELECTED);
        break;
    default:
        CChangingStateComponent::ChangeState(state);
        break;
    }
}