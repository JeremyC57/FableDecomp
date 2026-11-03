// NUISystem::CList::ScrollUp(unsigned long, unsigned long, bool, bool, bool) -- shifts the scrolling children between first and last one place up the list
// Leaf: ?ScrollUp@CList@NUISystem@@MAEXKK_N00@Z
// 
// Retail 0x005385AE, 3406 bytes, CList vtable 0x012462E4 slot 165 (the protected five-argument overload; overloaded
// virtuals sit in reverse declaration order, so ScrollUp(bool,bool) is slot 166), `ret 14h`.
// Shape: with more than one scrolling child, optionally moves the selection one down (ChangeState UNSELECTED/SELECTED
// on the old/new child, SetSelectedChild with wrap to 0); returns unless IsScrolling() or `force`; the transition time
// is GetUpdateTime() unless that is -1.0f (0x0122DEE0). The new position/zoom/colour of the vacated slot come from
// ScrollingChildrenStates[last or first-1][LIST_ACTIVE] (useStates) or the child's GetTarget* getters. The loop walks
// i = last .. first+1 (the loop variable shares the `last` argument slot; the i*12 state offset lives in the dead
// `moveSelection` slot), giving child i child i-1's target position, alpha and zoom and copying the ACTIVE / UNSELECTED /
// LIST_ACTIVE state records. Then either the first child fades in at the list head (not wrapping and first == 0:
// position minus PositionOffset.Y, alpha minus AlphaOffset*255 clamped at 0) or child `first` takes the saved values.
// Callees: virtuals of CComponent (ChangeState +0xC0, ChangePosition +0x7C, ChangeColour +0x98, ChangeZoom +0x8C,
// GetTargetPosition +0xDC, GetTargetColour +0xE0, GetTargetZoom +0xE4, GetUpdateTime +0xD8) and of CList
// (SetSelectedChild +0x284, IsWrapping +0x2A8, IsScrolling +0x2AC); map<unsigned long,CUIState>::operator[] 0x0042D5B1;
// __ftol 0x00BFEA70 for the alpha byte.
// Pragma: optimize("s", on) (the CList TU). Headers: CList retail branch (0x1FC bytes), the STLport map retail branch:
// the in-class operator[] body is what lets VC7.1 schedule each key temporary's store right before its call.
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
void CList::ScrollUp(unsigned long first, unsigned long last, bool useStates, bool moveSelection, bool force)
{
    if (ScrollingChildren.size() <= 1)
        return;
    if (moveSelection)
    {
        if (SelectedChild == ScrollingChildren.size() - 1 && !IsWrapping())
            return;
        if (SelectedChild >= first && SelectedChild <= last)
        {
            ScrollingChildren[SelectedChild]->ChangeState(UI_STATE_UNSELECTED);
            ++SelectedChild;
            SetSelectedChild(SelectedChild < ScrollingChildren.size() ? SelectedChild : 0);
            ScrollingChildren[SelectedChild]->ChangeState(UI_STATE_SELECTED);
        }
    }
    if (!IsScrolling() && !force)
        return;
    float time = (GetUpdateTime() != -1.0f) ? GetUpdateTime() : 0.0f;
    C2DVector position;
    C2DVector zoom;
    CRGBColour colour;
    if (first == 0)
    {
        if (useStates)
        {
            position = ScrollingChildrenStates[last][UI_STATE_LIST_ACTIVE].Position;
            zoom = ScrollingChildrenStates[last][UI_STATE_LIST_ACTIVE].Zoom;
            colour = ScrollingChildrenStates[last][UI_STATE_LIST_ACTIVE].Colour;
        }
        else
        {
            position = ScrollingChildren[last]->GetTargetPosition();
            zoom = ScrollingChildren[last]->GetTargetZoom();
            colour = ScrollingChildren[last]->GetTargetColour();
        }
    }
    else
    {
        if (useStates)
        {
            position = ScrollingChildrenStates[first - 1][UI_STATE_LIST_ACTIVE].Position;
            zoom = ScrollingChildrenStates[first - 1][UI_STATE_LIST_ACTIVE].Zoom;
            colour = ScrollingChildrenStates[first - 1][UI_STATE_LIST_ACTIVE].Colour;
        }
        else
        {
            position = ScrollingChildren[first - 1]->GetTargetPosition();
            zoom = ScrollingChildren[first - 1]->GetTargetZoom();
            colour = ScrollingChildren[first - 1]->GetTargetColour();
        }
    }
    for (long i = last; i > first; --i)
    {
        ScrollingChildren[i]->ChangePosition(ScrollingChildren[i - 1]->GetTargetPosition(), time, false);
        CRGBColour childColour = ScrollingChildren[i]->GetTargetColour();
        childColour.A = ScrollingChildren[i - 1]->GetTargetColour().A;
        ScrollingChildren[i]->ChangeColour(childColour, time, false);
        ScrollingChildren[i]->ChangeZoom(ScrollingChildren[i - 1]->GetTargetZoom(), time, false);
        if (useStates)
        {
            ScrollingChildrenStates[i][UI_STATE_ACTIVE].Position = ScrollingChildrenStates[i - 1][UI_STATE_ACTIVE].Position;
            ScrollingChildrenStates[i][UI_STATE_UNSELECTED].Position = ScrollingChildrenStates[i - 1][UI_STATE_UNSELECTED].Position;
            ScrollingChildrenStates[i][UI_STATE_LIST_ACTIVE].Position = ScrollingChildrenStates[i - 1][UI_STATE_LIST_ACTIVE].Position;
            ScrollingChildrenStates[i][UI_STATE_ACTIVE].Colour.A = ScrollingChildrenStates[i - 1][UI_STATE_ACTIVE].Colour.A;
            ScrollingChildrenStates[i][UI_STATE_UNSELECTED].Colour.A = ScrollingChildrenStates[i - 1][UI_STATE_UNSELECTED].Colour.A;
            ScrollingChildrenStates[i][UI_STATE_LIST_ACTIVE].Colour.A = ScrollingChildrenStates[i - 1][UI_STATE_LIST_ACTIVE].Colour.A;
            ScrollingChildrenStates[i][UI_STATE_ACTIVE].Zoom = ScrollingChildrenStates[i - 1][UI_STATE_ACTIVE].Zoom;
            ScrollingChildrenStates[i][UI_STATE_UNSELECTED].Zoom = ScrollingChildrenStates[i - 1][UI_STATE_UNSELECTED].Zoom;
            ScrollingChildrenStates[i][UI_STATE_LIST_ACTIVE].Zoom = ScrollingChildrenStates[i - 1][UI_STATE_LIST_ACTIVE].Zoom;
        }
        else
        {
            ScrollingChildrenStates[i][UI_STATE_ACTIVE].Position = ScrollingChildren[i - 1]->GetTargetPosition();
            ScrollingChildrenStates[i][UI_STATE_UNSELECTED].Position = ScrollingChildren[i - 1]->GetTargetPosition();
            ScrollingChildrenStates[i][UI_STATE_LIST_ACTIVE].Position = ScrollingChildren[i - 1]->GetTargetPosition();
            ScrollingChildrenStates[i][UI_STATE_ACTIVE].Colour.A = ScrollingChildren[i - 1]->GetTargetColour().A;
            ScrollingChildrenStates[i][UI_STATE_UNSELECTED].Colour.A = ScrollingChildren[i - 1]->GetTargetColour().A;
            ScrollingChildrenStates[i][UI_STATE_LIST_ACTIVE].Colour.A = ScrollingChildren[i - 1]->GetTargetColour().A;
            ScrollingChildrenStates[i][UI_STATE_ACTIVE].Zoom = ScrollingChildren[i - 1]->GetTargetZoom();
            ScrollingChildrenStates[i][UI_STATE_UNSELECTED].Zoom = ScrollingChildren[i - 1]->GetTargetZoom();
            ScrollingChildrenStates[i][UI_STATE_LIST_ACTIVE].Zoom = ScrollingChildren[i - 1]->GetTargetZoom();
        }
    }
    if (!IsWrapping() && first == 0)
    {
        if (useStates)
        {
            position.Y = ScrollingChildrenStates[0][UI_STATE_LIST_ACTIVE].Position.Y - PositionOffset.Y;
            position.X = ScrollingChildrenStates[0][UI_STATE_LIST_ACTIVE].Position.X + PositionOffset.X;
            colour.A = (unsigned char)((ScrollingChildrenStates[0][UI_STATE_LIST_ACTIVE].Colour.A - AlphaOffset * 255.0f) < 0.0f ? 0.0f : (ScrollingChildrenStates[0][UI_STATE_LIST_ACTIVE].Colour.A - AlphaOffset * 255.0f));
            colour.R = ScrollingChildrenStates[0][UI_STATE_LIST_ACTIVE].Colour.R;
            colour.G = ScrollingChildrenStates[0][UI_STATE_LIST_ACTIVE].Colour.G;
            colour.B = ScrollingChildrenStates[0][UI_STATE_LIST_ACTIVE].Colour.B;
        }
        else
        {
            position.Y = ScrollingChildren[0]->GetTargetPosition().Y - PositionOffset.Y;
            position.X = PositionOffset.X + ScrollingChildren[0]->GetTargetPosition().X;
            colour.A = (unsigned char)((ScrollingChildren[0]->GetTargetColour().A - AlphaOffset * 255.0f) < 0.0f ? 0.0f : (ScrollingChildren[0]->GetTargetColour().A - AlphaOffset * 255.0f));
            colour.R = ScrollingChildren[0]->GetTargetColour().R;
            colour.G = ScrollingChildren[0]->GetTargetColour().G;
            colour.B = ScrollingChildren[0]->GetTargetColour().B;
        }
        ScrollingChildren[0]->ChangePosition(position, time, false);
        ScrollingChildren[0]->ChangeColour(colour, time, false);
        ScrollingChildrenStates[0][UI_STATE_ACTIVE].Position = position;
        ScrollingChildrenStates[0][UI_STATE_UNSELECTED].Position = position;
        ScrollingChildrenStates[0][UI_STATE_LIST_ACTIVE].Position = position;
        ScrollingChildrenStates[0][UI_STATE_ACTIVE].Colour.A = colour.A;
        ScrollingChildrenStates[0][UI_STATE_UNSELECTED].Colour.A = colour.A;
        ScrollingChildrenStates[0][UI_STATE_LIST_ACTIVE].Colour.A = colour.A;
    }
    else
    {
        ScrollingChildren[first]->ChangePosition(position, time, false);
        if (useStates)
        {
            colour.R = ScrollingChildrenStates[first][UI_STATE_LIST_ACTIVE].Colour.R;
            colour.G = ScrollingChildrenStates[first][UI_STATE_LIST_ACTIVE].Colour.G;
            colour.B = ScrollingChildrenStates[first][UI_STATE_LIST_ACTIVE].Colour.B;
        }
        else
        {
            colour.R = ScrollingChildren[first]->GetTargetColour().R;
            colour.G = ScrollingChildren[first]->GetTargetColour().G;
            colour.B = ScrollingChildren[first]->GetTargetColour().B;
        }
        ScrollingChildren[first]->ChangeColour(colour, time, false);
        ScrollingChildren[first]->ChangeZoom(zoom, time, false);
        ScrollingChildrenStates[first][UI_STATE_ACTIVE].Position = position;
        ScrollingChildrenStates[first][UI_STATE_UNSELECTED].Position = position;
        ScrollingChildrenStates[first][UI_STATE_LIST_ACTIVE].Position = position;
        ScrollingChildrenStates[first][UI_STATE_ACTIVE].Colour.A = colour.A;
        ScrollingChildrenStates[first][UI_STATE_UNSELECTED].Colour.A = colour.A;
        ScrollingChildrenStates[first][UI_STATE_LIST_ACTIVE].Colour.A = colour.A;
        ScrollingChildrenStates[first][UI_STATE_ACTIVE].Zoom = zoom;
        ScrollingChildrenStates[first][UI_STATE_UNSELECTED].Zoom = zoom;
        ScrollingChildrenStates[first][UI_STATE_LIST_ACTIVE].Zoom = zoom;
    }
}
