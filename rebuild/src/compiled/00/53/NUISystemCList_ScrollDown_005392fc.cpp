// NUISystem::CList::ScrollDown(unsigned long, unsigned long, bool, bool, bool) -- shifts the scrolling children between first and last one place down the list
// Leaf: ?ScrollDown@CList@NUISystem@@MAEXKK_N00@Z
// 
// Retail 0x005392FC, 3493 bytes, CList vtable 0x012462E4 slot 167 (protected five-argument overload), `ret 14h`.
// The mirror of ScrollUp 0x005385AE: the selection moves one up (with wrap to the last child when it goes negative);
// the vacated slot takes ScrollingChildrenStates[first] / [last+1] (or the children's GetTarget* values); the loop
// walks i = first .. last-1 (an unsigned index sharing the `first` argument slot; `long` swaps two lea operands), giving
// child i child i+1's target position, alpha and zoom and copying the three state records; then the last child fades
// in at the tail (not wrapping and last == size-1: position plus PositionOffset.Y, alpha minus AlphaOffset*255 clamped
// at 0) or child `last` takes the saved values.
// Callees: as ScrollUp (CComponent virtuals +0xC0/+0x7C/+0x98/+0x8C/+0xDC/+0xE0/+0xE4/+0xD8, CList SetSelectedChild
// +0x284 / IsWrapping +0x2A8 / IsScrolling +0x2AC, map operator[] 0x0042D5B1, __ftol 0x00BFEA70).
// Pragma: optimize("s", on). Headers: CList retail branch, STLport map retail branch (in-class operator[] body).
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
using namespace NUISystem;
void CList::ScrollDown(unsigned long first, unsigned long last, bool useStates, bool moveSelection, bool force)
{
    if (ScrollingChildren.size() <= 1)
        return;
    if (moveSelection)
    {
        if (SelectedChild == 0 && !IsWrapping())
            return;
        if (SelectedChild >= first && SelectedChild <= last)
        {
            ScrollingChildren[SelectedChild]->ChangeState(UI_STATE_UNSELECTED);
            --SelectedChild;
            SetSelectedChild(SelectedChild < 0 ? ScrollingChildren.size() - 1 : SelectedChild);
            ScrollingChildren[SelectedChild]->ChangeState(UI_STATE_SELECTED);
        }
    }
    if (!IsScrolling() && !force)
        return;
    float time = (GetUpdateTime() != -1.0f) ? GetUpdateTime() : 0.0f;
    C2DVector position;
    C2DVector zoom;
    CRGBColour colour;
    if (last == ScrollingChildren.size() - 1)
    {
        if (useStates)
        {
            position = ScrollingChildrenStates[first][UI_STATE_LIST_ACTIVE].Position;
            zoom = ScrollingChildrenStates[first][UI_STATE_LIST_ACTIVE].Zoom;
            colour = ScrollingChildrenStates[first][UI_STATE_LIST_ACTIVE].Colour;
        }
        else
        {
            position = ScrollingChildren[first]->GetTargetPosition();
            zoom = ScrollingChildren[first]->GetTargetZoom();
            colour = ScrollingChildren[first]->GetTargetColour();
        }
    }
    else
    {
        if (useStates)
        {
            position = ScrollingChildrenStates[last + 1][UI_STATE_LIST_ACTIVE].Position;
            zoom = ScrollingChildrenStates[last + 1][UI_STATE_LIST_ACTIVE].Zoom;
            colour = ScrollingChildrenStates[last + 1][UI_STATE_LIST_ACTIVE].Colour;
        }
        else
        {
            position = ScrollingChildren[last + 1]->GetTargetPosition();
            zoom = ScrollingChildren[last + 1]->GetTargetZoom();
            colour = ScrollingChildren[last + 1]->GetTargetColour();
        }
    }
    for (unsigned long i = first; i < last; ++i)
    {
        ScrollingChildren[i]->ChangePosition(ScrollingChildren[i + 1]->GetTargetPosition(), time, false);
        CRGBColour childColour = ScrollingChildren[i]->GetTargetColour();
        childColour.A = ScrollingChildren[i + 1]->GetTargetColour().A;
        ScrollingChildren[i]->ChangeColour(childColour, time, false);
        ScrollingChildren[i]->ChangeZoom(ScrollingChildren[i + 1]->GetTargetZoom(), time, false);
        if (useStates)
        {
            ScrollingChildrenStates[i][UI_STATE_ACTIVE].Position = ScrollingChildrenStates[i + 1][UI_STATE_ACTIVE].Position;
            ScrollingChildrenStates[i][UI_STATE_UNSELECTED].Position = ScrollingChildrenStates[i + 1][UI_STATE_UNSELECTED].Position;
            ScrollingChildrenStates[i][UI_STATE_LIST_ACTIVE].Position = ScrollingChildrenStates[i + 1][UI_STATE_LIST_ACTIVE].Position;
            ScrollingChildrenStates[i][UI_STATE_ACTIVE].Colour.A = ScrollingChildrenStates[i + 1][UI_STATE_ACTIVE].Colour.A;
            ScrollingChildrenStates[i][UI_STATE_UNSELECTED].Colour.A = ScrollingChildrenStates[i + 1][UI_STATE_UNSELECTED].Colour.A;
            ScrollingChildrenStates[i][UI_STATE_LIST_ACTIVE].Colour.A = ScrollingChildrenStates[i + 1][UI_STATE_LIST_ACTIVE].Colour.A;
            ScrollingChildrenStates[i][UI_STATE_ACTIVE].Zoom = ScrollingChildrenStates[i + 1][UI_STATE_ACTIVE].Zoom;
            ScrollingChildrenStates[i][UI_STATE_UNSELECTED].Zoom = ScrollingChildrenStates[i + 1][UI_STATE_UNSELECTED].Zoom;
            ScrollingChildrenStates[i][UI_STATE_LIST_ACTIVE].Zoom = ScrollingChildrenStates[i + 1][UI_STATE_LIST_ACTIVE].Zoom;
        }
        else
        {
            ScrollingChildrenStates[i][UI_STATE_ACTIVE].Position = ScrollingChildren[i + 1]->GetTargetPosition();
            ScrollingChildrenStates[i][UI_STATE_UNSELECTED].Position = ScrollingChildren[i + 1]->GetTargetPosition();
            ScrollingChildrenStates[i][UI_STATE_LIST_ACTIVE].Position = ScrollingChildren[i + 1]->GetTargetPosition();
            ScrollingChildrenStates[i][UI_STATE_ACTIVE].Colour.A = ScrollingChildren[i + 1]->GetTargetColour().A;
            ScrollingChildrenStates[i][UI_STATE_UNSELECTED].Colour.A = ScrollingChildren[i + 1]->GetTargetColour().A;
            ScrollingChildrenStates[i][UI_STATE_LIST_ACTIVE].Colour.A = ScrollingChildren[i + 1]->GetTargetColour().A;
            ScrollingChildrenStates[i][UI_STATE_ACTIVE].Zoom = ScrollingChildren[i + 1]->GetTargetZoom();
            ScrollingChildrenStates[i][UI_STATE_UNSELECTED].Zoom = ScrollingChildren[i + 1]->GetTargetZoom();
            ScrollingChildrenStates[i][UI_STATE_LIST_ACTIVE].Zoom = ScrollingChildren[i + 1]->GetTargetZoom();
        }
    }
    if (!IsWrapping() && last == ScrollingChildren.size() - 1)
    {
        if (useStates)
        {
            position.Y = ScrollingChildrenStates[last][UI_STATE_LIST_ACTIVE].Position.Y + PositionOffset.Y;
            position.X = ScrollingChildrenStates[last][UI_STATE_LIST_ACTIVE].Position.X + PositionOffset.X;
            colour.A = (unsigned char)((ScrollingChildrenStates[last][UI_STATE_LIST_ACTIVE].Colour.A - AlphaOffset * 255.0f) < 0.0f ? 0.0f : (ScrollingChildrenStates[last][UI_STATE_LIST_ACTIVE].Colour.A - AlphaOffset * 255.0f));
            colour.R = ScrollingChildrenStates[last][UI_STATE_LIST_ACTIVE].Colour.R;
            colour.G = ScrollingChildrenStates[last][UI_STATE_LIST_ACTIVE].Colour.G;
            colour.B = ScrollingChildrenStates[last][UI_STATE_LIST_ACTIVE].Colour.B;
        }
        else
        {
            position.Y = ScrollingChildren[last]->GetTargetPosition().Y + PositionOffset.Y;
            position.X = PositionOffset.X + ScrollingChildren[last]->GetTargetPosition().X;
            colour.A = (unsigned char)((ScrollingChildren[last]->GetTargetColour().A - AlphaOffset * 255.0f) < 0.0f ? 0.0f : (ScrollingChildren[last]->GetTargetColour().A - AlphaOffset * 255.0f));
            colour.R = ScrollingChildren[last]->GetTargetColour().R;
            colour.G = ScrollingChildren[last]->GetTargetColour().G;
            colour.B = ScrollingChildren[last]->GetTargetColour().B;
        }
        ScrollingChildren[last]->ChangePosition(position, time, false);
        ScrollingChildren[last]->ChangeColour(colour, time, false);
        ScrollingChildrenStates[last][UI_STATE_ACTIVE].Position = position;
        ScrollingChildrenStates[last][UI_STATE_UNSELECTED].Position = position;
        ScrollingChildrenStates[last][UI_STATE_LIST_ACTIVE].Position = position;
        ScrollingChildrenStates[last][UI_STATE_ACTIVE].Colour.A = colour.A;
        ScrollingChildrenStates[last][UI_STATE_UNSELECTED].Colour.A = colour.A;
        ScrollingChildrenStates[last][UI_STATE_LIST_ACTIVE].Colour.A = colour.A;
    }
    else
    {
        ScrollingChildren[last]->ChangePosition(position, time, false);
        if (useStates)
        {
            colour.R = ScrollingChildrenStates[last][UI_STATE_LIST_ACTIVE].Colour.R;
            colour.G = ScrollingChildrenStates[last][UI_STATE_LIST_ACTIVE].Colour.G;
            colour.B = ScrollingChildrenStates[last][UI_STATE_LIST_ACTIVE].Colour.B;
        }
        else
        {
            colour.R = ScrollingChildren[last]->GetTargetColour().R;
            colour.G = ScrollingChildren[last]->GetTargetColour().G;
            colour.B = ScrollingChildren[last]->GetTargetColour().B;
        }
        ScrollingChildren[last]->ChangeColour(colour, time, false);
        ScrollingChildren[last]->ChangeZoom(zoom, time, false);
        ScrollingChildrenStates[last][UI_STATE_ACTIVE].Position = position;
        ScrollingChildrenStates[last][UI_STATE_UNSELECTED].Position = position;
        ScrollingChildrenStates[last][UI_STATE_LIST_ACTIVE].Position = position;
        ScrollingChildrenStates[last][UI_STATE_ACTIVE].Colour.A = colour.A;
        ScrollingChildrenStates[last][UI_STATE_UNSELECTED].Colour.A = colour.A;
        ScrollingChildrenStates[last][UI_STATE_LIST_ACTIVE].Colour.A = colour.A;
        ScrollingChildrenStates[last][UI_STATE_ACTIVE].Zoom = zoom;
        ScrollingChildrenStates[last][UI_STATE_UNSELECTED].Zoom = zoom;
        ScrollingChildrenStates[last][UI_STATE_LIST_ACTIVE].Zoom = zoom;
    }
}