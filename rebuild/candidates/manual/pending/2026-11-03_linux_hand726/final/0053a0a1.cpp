// NUISystem::CList::ChangeState(unsigned long) -- per-state observer registration, re-centring and state hand-over for a list
// Leaf: ?ChangeState@CList@NUISystem@@UAEXK@Z
// 
// Retail 0x0053A0A1, 3174 bytes, CList vtable slot 48 (CComponent::ChangeState override), `ret 4`.
// Shape: a switch laid out LIST_INACTIVE(6), LIST_ACTIVE(5), UNSELECTED(4), SELECTED(3), ACTIVE(1), INACTIVE(0) (VC7.1
// keeps source order; the jump chain is `sub eax,0 ; je ; dec ...`), default forwards to
// CChangingStateComponent::ChangeState 0x0052CF40, which every case ends with.
//  - 6: stop observing (CManager::RemoveObserver, vtable +0x14) self and both scroll bars (IgnoreEvent MOUSE_MOVEMENT
//    first); base state INACTIVE when CurrentState is INACTIVE, else LIST_INACTIVE.
//  - 5: when RecomputeOffsets is set, DoRecomputeOffsets()+SetRecomputeOffsets(false) and scroll the middle child to the
//    selection (ScrollUp/ScrollDown(true,false)); play the misc-things SoundForward (CManager GetMiscThingsDef +0xA8,
//    PlaySoundW +0xA0); scroll until DefaultChild is selected; when IsScrolling(), copy each child's three saved
//    state records into its own GetStates() map; for a list in the list tree (def PartOfListTree +0x20A) detach the
//    front of CList::GPreviousSelected (0x013B8A98) and push this list in front (push_front 0x0053CA3C); re-parent the
//    children, observe the scrolling children and the scroll bars, set the base state, send the previous list
//    LIST_UNSELECTED, select the selected child and SetNonScrollingChildrenStates().
//  - 4 / 0: stop observing self, the scrolling children (then OnObserverRemove) and the scroll bars.
//  - 3: observe self (UP/DOWN), the scrolling children, select the selected child, observe the scroll bars.
//  - 1: observe self (UP/DOWN).
// Levers: `bool recompute = RecomputeOffsets; if (RecomputeOffsets)` keeps retail's spilled flag and second test; the
// middle index as one conditional expression gives retail's register assignment (edi = zero, ebx = 1); the STLport
// map retail branch (in-class operator[] body) lets VC7.1 hoist the eighteen loop-invariant key stores out of the
// state-copy loop; the STLport list retail branch supplies size()/front()/push_front().
// Callees: CManager::GetInstance 0x0041E5F2; CChangingStateComponent::ChangeState 0x0052CF40; map operator[] 0x0042D5B1;
// list<CList*>::push_front 0x0053CA3C; virtuals as named.
// Pragma: optimize("s", on). Headers: CList retail branch (adds the static GPreviousSelected), CUIMiscThingsDef.
#pragma optimize("s", on)
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
#include "engine/CDefPointer.h"
#include "engine/NUISystem_CUIDef.h"
#include "engine/NUISystem_CList.h"
#include "engine/NUISystem_CManager.h"
namespace NUISystem {
class CUIScreenUnitConverter {
public:
    static float __fastcall ConvertX(float x);          // 0x0052E470
    static float __fastcall ConvertY(float y);          // 0x0052E4D0
};
}
#include "engine/NUISystem_CUIMiscThingsDef.h"
using namespace NUISystem;
void CList::ChangeState(unsigned long state)
{
    switch (state)
    {
    case UI_STATE_LIST_INACTIVE:
        CManager::GetInstance()->RemoveObserver(this);
        if (PScrollBar)
        {
            PScrollBar->IgnoreEvent(UI_EVENT_MOUSE_MOVEMENT);
            CManager::GetInstance()->RemoveObserver(PScrollBar);
        }
        if (PScrollBarOutside)
        {
            PScrollBarOutside->IgnoreEvent(UI_EVENT_MOUSE_MOVEMENT);
            CManager::GetInstance()->RemoveObserver(PScrollBarOutside);
        }
        if (CurrentState == UI_STATE_INACTIVE)
            CChangingStateComponent::ChangeState(UI_STATE_INACTIVE);
        else
            CChangingStateComponent::ChangeState(UI_STATE_LIST_INACTIVE);
        break;
    case UI_STATE_LIST_ACTIVE:
    {
        bool recompute = RecomputeOffsets;
        if (RecomputeOffsets)
        {
            DoRecomputeOffsets();
            SetRecomputeOffsets(false);
        }
        if (recompute && !ScrollingChildren.empty())
        {
            long middle = (ScrollingChildren.size() % 2 == 0) ? ScrollingChildren.size() / 2 - 1 : ScrollingChildren.size() / 2;
            if (middle < SelectedChild)
            {
                while (middle != SelectedChild)
                {
                    ScrollUp(true, false);
                    ++middle;
                }
            }
            else
            {
                while (middle != SelectedChild)
                {
                    ScrollDown(true, false);
                    --middle;
                }
            }
        }
        CManager::GetInstance()->PlaySoundW(CManager::GetInstance()->GetMiscThingsDef()->SoundForward, 1);
        if (DefaultChild >= 0 && DefaultChild < ScrollingChildren.size() && DefaultChild != SelectedChild)
        {
            if (DefaultChild < SelectedChild)
            {
                while (DefaultChild != SelectedChild)
                    ScrollDown(true, true);
            }
            else
            {
                while (DefaultChild != SelectedChild)
                    ScrollUp(true, true);
            }
        }
        if (IsScrolling())
        {
            for (unsigned long i = 0; i < ScrollingChildren.size(); ++i)
            {
                ScrollingChildren[i]->GetStates()[UI_STATE_ACTIVE].Position = ScrollingChildrenStates[i][UI_STATE_ACTIVE].Position;
                ScrollingChildren[i]->GetStates()[UI_STATE_UNSELECTED].Position = ScrollingChildrenStates[i][UI_STATE_UNSELECTED].Position;
                ScrollingChildren[i]->GetStates()[UI_STATE_LIST_ACTIVE].Position = ScrollingChildrenStates[i][UI_STATE_LIST_ACTIVE].Position;
                ScrollingChildren[i]->GetStates()[UI_STATE_ACTIVE].Colour.A = ScrollingChildrenStates[i][UI_STATE_ACTIVE].Colour.A;
                ScrollingChildren[i]->GetStates()[UI_STATE_UNSELECTED].Colour.A = ScrollingChildrenStates[i][UI_STATE_UNSELECTED].Colour.A;
                ScrollingChildren[i]->GetStates()[UI_STATE_LIST_ACTIVE].Colour.A = ScrollingChildrenStates[i][UI_STATE_LIST_ACTIVE].Colour.A;
                ScrollingChildren[i]->GetStates()[UI_STATE_ACTIVE].Zoom = ScrollingChildrenStates[i][UI_STATE_ACTIVE].Zoom;
                ScrollingChildren[i]->GetStates()[UI_STATE_UNSELECTED].Zoom = ScrollingChildrenStates[i][UI_STATE_UNSELECTED].Zoom;
                ScrollingChildren[i]->GetStates()[UI_STATE_LIST_ACTIVE].Zoom = ScrollingChildrenStates[i][UI_STATE_LIST_ACTIVE].Zoom;
            }
        }
        if (LoadAndReturnDef()->PartOfListTree)
        {
            if (GPreviousSelected.size() > 0)
            {
                CManager::GetInstance()->RemoveObserver(GPreviousSelected.front());
                if (GPreviousSelected.front()->PScrollBar)
                {
                    GPreviousSelected.front()->PScrollBar->IgnoreEvent(UI_EVENT_MOUSE_MOVEMENT);
                    CManager::GetInstance()->RemoveObserver(GPreviousSelected.front()->PScrollBar);
                }
                if (GPreviousSelected.front()->PScrollBarOutside)
                {
                    GPreviousSelected.front()->PScrollBarOutside->IgnoreEvent(UI_EVENT_MOUSE_MOVEMENT);
                    CManager::GetInstance()->RemoveObserver(GPreviousSelected.front()->PScrollBarOutside);
                }
                if (GPreviousSelected.front()->ScrollingChildren.size() > 0)
                {
                    for (unsigned long i = 0; i < GPreviousSelected.front()->ScrollingChildren.size(); ++i)
                    {
                        CManager::GetInstance()->RemoveObserver(GPreviousSelected.front()->ScrollingChildren[i]);
                        GPreviousSelected.front()->ScrollingChildren[i]->OnObserverRemove();
                    }
                }
            }
            GPreviousSelected.push_front(this);
        }
        for (unsigned long i = 0; i < Children.size(); ++i)
            Children[i]->SetParent(this);
        if (ScrollingChildren.size() > 0)
        {
            for (unsigned long i = 0; i < ScrollingChildren.size(); ++i)
            {
                CManager::GetInstance()->AddObserver(ScrollingChildren[i]);
                ScrollingChildren[i]->ObserveEvent(UI_EVENT_MOUSE_MOVEMENT);
            }
            ScrollingChildren[SelectedChild]->ObserveEvent(UI_EVENT_LEFT);
            ScrollingChildren[SelectedChild]->ObserveEvent(UI_EVENT_RIGHT);
            ScrollingChildren[SelectedChild]->ObserveEvent(UI_EVENT_SELECT);
            ScrollingChildren[SelectedChild]->ObserveEvent(UI_EVENT_BACK);
        }
        if (PScrollBar)
        {
            CManager::GetInstance()->AddObserver(PScrollBar);
            PScrollBar->ObserveEvent(UI_EVENT_MOUSE_MOVEMENT);
        }
        if (PScrollBarOutside)
        {
            CManager::GetInstance()->AddObserver(PScrollBarOutside);
            PScrollBarOutside->ObserveEvent(UI_EVENT_MOUSE_MOVEMENT);
        }
        CChangingStateComponent::ChangeState(UI_STATE_LIST_ACTIVE);
        if (LoadAndReturnDef()->PartOfListTree)
        {
            std::list<CList*>::iterator previous = GPreviousSelected.begin();
            ++previous;
            if (previous != GPreviousSelected.end())
                (*previous)->ChangeState(UI_STATE_LIST_UNSELECTED);
        }
        if (ScrollingChildren.size() > 0)
            ScrollingChildren[SelectedChild]->ChangeState(UI_STATE_SELECTED);
        SetNonScrollingChildrenStates();
        break;
    }
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
        if (PScrollBar)
        {
            PScrollBar->IgnoreEvent(UI_EVENT_MOUSE_MOVEMENT);
            CManager::GetInstance()->RemoveObserver(PScrollBar);
        }
        if (PScrollBarOutside)
        {
            PScrollBarOutside->IgnoreEvent(UI_EVENT_MOUSE_MOVEMENT);
            CManager::GetInstance()->RemoveObserver(PScrollBarOutside);
        }
        CChangingStateComponent::ChangeState(UI_STATE_UNSELECTED);
        break;
    case UI_STATE_SELECTED:
        CManager::GetInstance()->AddObserver(this);
        ObserveEvent(UI_EVENT_DOWN);
        ObserveEvent(UI_EVENT_UP);
        if (ScrollingChildren.size() > 0)
        {
            for (unsigned long i = 0; i < ScrollingChildren.size(); ++i)
            {
                CManager::GetInstance()->AddObserver(ScrollingChildren[i]);
                ScrollingChildren[i]->ObserveEvent(UI_EVENT_MOUSE_MOVEMENT);
            }
            ScrollingChildren[SelectedChild]->ObserveEvent(UI_EVENT_LEFT);
            ScrollingChildren[SelectedChild]->ObserveEvent(UI_EVENT_RIGHT);
            ScrollingChildren[SelectedChild]->ObserveEvent(UI_EVENT_SELECT);
            ScrollingChildren[SelectedChild]->ObserveEvent(UI_EVENT_BACK);
        }
        CChangingStateComponent::ChangeState(UI_STATE_SELECTED);
        if (ScrollingChildren.size() > 0)
            ScrollingChildren[SelectedChild]->ChangeState(UI_STATE_SELECTED);
        SetNonScrollingChildrenStates();
        if (PScrollBar)
        {
            CManager::GetInstance()->AddObserver(PScrollBar);
            PScrollBar->ObserveEvent(UI_EVENT_MOUSE_MOVEMENT);
        }
        if (PScrollBarOutside)
        {
            CManager::GetInstance()->AddObserver(PScrollBarOutside);
            PScrollBarOutside->ObserveEvent(UI_EVENT_MOUSE_MOVEMENT);
        }
        break;
    case UI_STATE_ACTIVE:
        CManager::GetInstance()->AddObserver(this);
        ObserveEvent(UI_EVENT_DOWN);
        ObserveEvent(UI_EVENT_UP);
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
        if (PScrollBar)
        {
            PScrollBar->IgnoreEvent(UI_EVENT_MOUSE_MOVEMENT);
            CManager::GetInstance()->RemoveObserver(PScrollBar);
        }
        if (PScrollBarOutside)
        {
            PScrollBarOutside->IgnoreEvent(UI_EVENT_MOUSE_MOVEMENT);
            CManager::GetInstance()->RemoveObserver(PScrollBarOutside);
        }
        CChangingStateComponent::ChangeState(UI_STATE_INACTIVE);
        break;
    default:
        CChangingStateComponent::ChangeState(state);
        break;
    }
}
