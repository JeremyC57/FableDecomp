// NUISystem::CScrollableList::UpdateVisibleList(float, float) -- lay the list out for its current scroll offset: show or hide the up arrow, hide the children above the window, unhide and stack the visible window one offset step apart, show or hide the down arrow, hide the children below, and remember the scroll time
// Leaf: ?UpdateVisibleList@CScrollableList@NUISystem@@IAEXMM@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00555A90, 595 bytes, `ret 8`.
// Callees: the class's protected HideComponent 0x00555440 / UnhideComponent 0x005556F0 (Ego_r IAE, by-value C2DVector) and
//   CComponent::GetPosition() const (slot 16, +0x40). Members from engine/NUISystem_CScrollableList.h / NUISystem_CComponent.h.
// Shape: every child is read through CCountedPointer<CComponent>::Get() (FableWin.pdb ?Get@?$CCountedPointer@VCComponent@NUISystem@@@@QBEPAVCComponent@NUISystem@@XZ,
//   inline): the inline call's result is an expression temporary that reuses the `_M_start` base register (retail
//   `mov eax,[eax+edi*8]`); reading the `.Data` member straight made the base a separate candidate, which shifted the scratch
//   rotation in all three loops, the loop-2 position copy and the down-arrow test (12 hunks of register colouring).
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_CSCROLLABLELIST_VISIBLELIST
#include "rebuild_abi.h"
#include "engine/NUISystem_CScrollableList.h"
namespace NUISystem {
void CScrollableList::UpdateVisibleList(float move_time, float fade_time)
{
    C2DVector hidden = -PositionOffset;
    unsigned long i = 0;
    if (TopOfList == 0) {
        HideComponent(PUpArrow, 0, fade_time, PUpArrow->GetPosition(), 0);
        UpArrowActive = false;
    } else {
        UnhideComponent(PUpArrow, 0, fade_time, PUpArrow->GetPosition());
        UpArrowActive = true;
    }
    for (; i < TopOfList && i < Children.size(); ++i)
        HideComponent(Children[i].Get(), move_time, fade_time, hidden, 0);
    C2DVector pos;
    pos.X = hidden.X + PositionOffset.X;
    pos.Y = hidden.Y + PositionOffset.Y;
    unsigned long end = std::min<unsigned long>(TopOfList + VisibleListItems, Children.size());
    for (; i < end; ++i) {
        UnhideComponent(Children[i].Get(), move_time, fade_time, pos);
        pos += PositionOffset;
    }
    if (i < Children.size()) {
        UnhideComponent(PDownArrow, 0, fade_time, PDownArrow->GetPosition());
        DownArrowActive = true;
    } else {
        HideComponent(PDownArrow, 0, fade_time, PDownArrow->GetPosition(), 0);
        DownArrowActive = false;
    }
    for (; i < Children.size(); ++i)
        HideComponent(Children[i].Get(), move_time, fade_time, pos, 0);
    ScrollTimer = move_time;
}
}