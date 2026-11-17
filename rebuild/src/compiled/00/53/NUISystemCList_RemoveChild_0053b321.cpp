// NUISystem::CList::RemoveChild(long index) -- remove child `index`: when it is a scrolling child, shift the rows after it,
//   keep the selection in range, drop its row, state map and visibility entry and re-arm the remaining children (states
//   0x19, 2-5 on the selection) before recomputing offsets; otherwise drop it from the non-scrolling children. Then the
//   base removal; an emptied list sends action 1, and when it heads the list registry it gives the head state 0 / 6,
//   pops it and wakes the next list (state 3); finally the up / down arrows get the new item counts.
// Leaf: ?RemoveChild@CList@NUISystem@@UAEXK@Z
// Retail: Fable.exe @ 0x0053B321, 795 bytes, `ret 4`; frame-pointer size TU (optimize "s" on, "y" off) as the landed
//   CList::AddScrollingChildDynamically 0x0053BEAC / ScrollToFirstChild 0x005363EA.
// Callees: CCountedPointer<CComponent> copy 0x00428928 / dtor 0x004291DE, std::find over CComponent* 0x0053C91C (LTCG
//   register convention: first ecx, last edx, value by const&), vector<CComponent*>::erase 0x00485468, the state-map
//   vector's erase 0x0053CD05, the visibility vector's erase 0x0053C7EF, CManager::GetInstance 0x0041E5F2 (slot 2
//   AddInputListener on the +4 base), CChangingStateComponent::RemoveChild 0x00533BC0 (direct base call), CDeletion ctor
//   0x0042BED4 / copy 0x0042CD84 / list dtor 0x0042ABCA, the list registry 0x013B8A98 with pop_front 0x0053C7D9.
//   Component slots 48 ChangeState, 62 SetDeletion (CDeletion by value), 108 LoadAndReturnDef, CListArrow 150 SetMaxToDraw, CList 161
//   SetSelectedChild / 165 ScrollUp(5 args) / 181 SetRecomputeOffsets; +4 CObserver slot 3 ObserveEvent.
// Layout: Ego_r.pdb CList, shifted to retail by the landed CList models: SelectedChild +0x15c, ScrollingChildren +0x164,
//   NonScrollingChildren +0x170, ScrollingChildrenStates +0x17c (12-byte maps), ScrollingChildrenVisibility +0x1c8,
//   PListUp +0x1e0, PListDown +0x1e4 (PDB 548 / 552 less the three 4-byte-smaller STLport vectors and the 12-byte
//   vector<bool>); the base child vector of counted pointers +0xb0 as the landed AddScrollingChildDynamically. Def fields
//   +0x13c / +0x221 are named descriptively (the retail def does not follow Ego_r.pdb CUIDef).
// Shape: the scrolling search caches the end in a local (compared in ebx); the non-scrolling one passes a local end but
//   compares a fresh end() (else VC keeps &finish, not &vector, in ebx); the arrows are cached in locals; the down count
//   reads a named def pointer (in the dead `index` slot) so the release follows the call; `if (restore) ...(0); else
//   ...(6);` keeps the push 0 / push 6 branches (a ?: becomes sbb/and/add).
// Name fix (agentNF 2026-11-10): 0x004291DE is CCountedPointer<NUISystem::CComponent>::ReleaseData (the destructor is the 5-byte
//   `jmp` COMDAT 0x00429C53; Ego_r.pdb ??1 is 5 bytes; likewise 0x00429160 = ReleaseData<CAction>, dtor jmp 0x00429B72). The local model's destructor is `{ ReleaseData(); }`
//   (engine/CCountedPointer.h FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA_DECL_ONLY shape).
// NF66 (round 7): ported onto engine/NUISystem_CList.h (FABLE_RETAIL_CLIST + CUIDef, the define block of the landed same-TU rows
//   0x0053A0A1 / 0x00549440): every local model deleted. Slot names from the header vtable (CList slots 142..: 161 SetSelectedChild,
//   165 the protected ScrollUp(unsigned long, unsigned long, bool, bool, bool), 181 SetRecomputeOffsets; CListArrow slot 150
//   SetMaxToDraw on the up / down arrows; CObserver slot 3 ObserveEvent; CObservable slot 2 AddObserver; CComponent slot 62
//   SetDeletion(CDeletion) with the ctor pair ??0CDeletion@NUISystem@@QAE@XZ 0x0042BED4 / ??0CDeletion@NUISystem@@QAE@ABV01@@Z
//   0x0042CD84 (the landed model called them CAction); the def fields CUIDef::DownLimit +0x13c and PCStyle +0x221; the registry
//   is CList::GPreviousSelected. Self: the PDB virtual ?RemoveChild@CList@NUISystem@@UAEXK@Z (was QAEXJ).
#include "fable_compiler.h"
#pragma optimize("y", off)
#pragma optimize("s", on)
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_C2DBOXF
#define FABLE_RETAIL_CUIDEF
#define FABLE_RETAIL_CLIST
#define FABLE_RETAIL_CMANAGER
#define FABLE_RETAIL_CDELETION
#define FABLE_RETAIL_CLISTARROW_VSLOTS
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_STLP_VECTOR_ERASE_NOINLINE
#define FABLE_RETAIL_STLP_FIND_NOINLINE_BODY_FORWARDER
#define FABLE_RETAIL_CCOUNTEDPOINTER_CALLED_CTOR
#define FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA_DECL_ONLY
#include "engine/CDefPointer.h"
#include "engine/NUISystem_CUIDef.h"
#include "engine/NUISystem_CList.h"
#include "engine/NUISystem_CManager.h"
#include "engine/NUISystem_CDeletion.h"
#include "engine/NUISystem_CListArrow.h"
#include "stlport/fable_algorithm.h"

namespace NUISystem {

void CList::RemoveChild(unsigned long index)
{
    CCountedPointer<CComponent> child = Children[index];
    std::vector<CComponent*>::iterator last = ScrollingChildren.end();
    std::vector<CComponent*>::iterator it = std::find(ScrollingChildren.begin(), last, child.Get());
    if (it != last) {
        long pos = it - ScrollingChildren.begin();
        if (SelectedChild == 0) {
            ScrollUp(pos, ScrollingChildren.size() - 1, false, false, true);
            SetSelectedChild(0);
        } else if (SelectedChild == ScrollingChildren.size() - 1) {
            ScrollUp(pos, ScrollingChildren.size() - 1, false, false, true);
            SetSelectedChild(ScrollingChildren.size() - 2);
        } else {
            ScrollUp(pos, ScrollingChildren.size() - 1, false, false, true);
        }
        ScrollingChildren.erase(it);
        ScrollingChildrenStates.erase(ScrollingChildrenStates.begin() + pos);
        ScrollingChildrenVisibility.erase(ScrollingChildrenVisibility.begin() + pos);
        if (!ScrollingChildren.empty()) {
            ScrollingChildren[SelectedChild]->ChangeState(3);
            for (unsigned long i = 0; i < ScrollingChildren.size(); ++i) {
                CManager::GetInstance()->AddObserver(ScrollingChildren[i]);
                ScrollingChildren[i]->ObserveEvent(UI_EVENT_MOUSE_MOVEMENT);
            }
            ScrollingChildren[SelectedChild]->ObserveEvent(UI_EVENT_LEFT);
            ScrollingChildren[SelectedChild]->ObserveEvent(UI_EVENT_RIGHT);
            ScrollingChildren[SelectedChild]->ObserveEvent(UI_EVENT_SELECT);
            ScrollingChildren[SelectedChild]->ObserveEvent(UI_EVENT_BACK);
        }
        SetRecomputeOffsets(true);
    } else {
        std::vector<CComponent*>::iterator otherLast = NonScrollingChildren.end();
        std::vector<CComponent*>::iterator found = std::find(NonScrollingChildren.begin(), otherLast, child.Get());
        if (found != NonScrollingChildren.end())
            NonScrollingChildren.erase(found);
    }
    CChangingStateComponent::RemoveChild(index);
    if (ScrollingChildren.empty()) {
        CDeletion deletion;
        deletion.Method = CDeletion::UI_DELETE_THIS_INSTANCE;
        SetDeletion(deletion);
        if (this == GPreviousSelected.front()) {
            bool restore = LoadAndReturnDef()->PCStyle;
            if (restore)
                GPreviousSelected.front()->ChangeState(0);
            else
                GPreviousSelected.front()->ChangeState(6);
            GPreviousSelected.pop_front();
            if (!GPreviousSelected.empty())
                GPreviousSelected.front()->ChangeState(3);
        }
    }
    CComponent* up = PListUp;
    if (up)
        static_cast<CListArrow*>(up)->SetMaxToDraw(ScrollingChildren.size());
    CComponent* down = PListDown;
    if (down) {
        CDefPointer<const CUIDef> def = LoadAndReturnDef();
        static_cast<CListArrow*>(down)->SetMaxToDraw(ScrollingChildren.size() - def->DownLimit - 1);
    }
}
}
