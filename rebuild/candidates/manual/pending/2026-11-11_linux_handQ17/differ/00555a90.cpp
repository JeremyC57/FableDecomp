// NUISystem::CScrollableList::UpdateVisibleList(float, float) -- lay the list out for its current scroll offset: show or hide the up arrow, hide the children above the window, unhide and stack the visible window one offset step apart, show or hide the down arrow, hide the children below, and remember the scroll time
// Grade: DIFFER(ours v retail) -- DIFFER(595v595), 12 hunks / 42 lines, register colouring only: now ported onto engine/NUISystem_CScrollableList.h (+ opt-in FABLE_RETAIL_CSCROLLABLELIST_VISIBLELIST) with STLport std::min and Children[i]; retail loads Children._M_start into a register before the down-arrow size test (ours sub reg,[mem]) so its vtable loads stay per-branch (ours hoists mov edx,[ecx]), stores the by-value pos copy Y-first in loop 2 (ours X-first) and picks eax/edx for the child pointer in loops 1/3 (ours edx/eax); tried >/>= compare flips, a named count, break-form loop 1, while-form loop 3, ctor-form pos/hidden (worse) -- 1 Q17 grade
// Leaf: ?UpdateVisibleList@CScrollableList@NUISystem@@IAEXMM@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00555A90, 595 bytes, `ret 8`.
// Callees: the class's protected HideComponent 0x00555440 / UnhideComponent 0x005556F0 (Ego_r IAE, by-value C2DVector) and
//   CComponent::GetPosition() const (slot 16, +0x40). Members from engine/NUISystem_CScrollableList.h / NUISystem_CComponent.h.
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
        HideComponent(Children[i].Data, move_time, fade_time, hidden, 0);
    C2DVector pos;
    pos.X = hidden.X + PositionOffset.X;
    pos.Y = hidden.Y + PositionOffset.Y;
    unsigned long end = std::min<unsigned long>(TopOfList + VisibleListItems, Children.size());
    for (; i < end; ++i) {
        UnhideComponent(Children[i].Data, move_time, fade_time, pos);
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
        HideComponent(Children[i].Data, move_time, fade_time, pos, 0);
    ScrollTimer = move_time;
}
}
