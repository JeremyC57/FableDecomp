// NUISystem::CList::ComputeChildrenVisibility() -- marks which scrolling children lie inside the list view area and fades them in/out
// Grade: DIFFER(2126v2102) -- the C2DBoxF view-area box: retail fetches the eight LoadAndReturnDef() pointers right to left, then converts TLX..BRY left to right straight into the box (one frame word fewer); every constructor/accessor/aggregate spelling tried builds the components in argument order
// Leaf: ?ComputeChildrenVisibility@CList@NUISystem@@MAEXXZ
// 
// Retail 0x005379F5, 2102 bytes, CList vtable 0x012462E4 slot 185 (catalog "vfunc_185"). Biased frame (lea ebp,[esp-78h]).
// Per scrolling child parented to the list: target position and size (scaled by CManager::GetUIScale 0x0041CF47);
// the view area from the def ViewArea (CUIScreenUnitConverter::ConvertX 0x0052E470 / ConvertY 0x0052E4D0), moved to
// the initial-position dummy, grown by 1 and scaled; both corners inside (C2DBoxF::ContainsPoint 0x00535CD0) fade the
// child in (alpha 255) and set its visibility byte, else fade out; then re-observe MOUSE_MOVEMENT on selected children and
// repair the selection when the selected child left the view (IgnoreEvent/ObserveEvent 2..5, ten Update(0.2f) steps).
// Pragma: optimize("s", on). Headers: CList retail branch, C2DBoxF retail branch (ContainsPoint, accessors).
#pragma optimize("s", on)
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_C2DBOXF
#define FABLE_RETAIL_CUIDEF
#define FABLE_RETAIL_CLIST
#define FABLE_RETAIL_CMANAGER
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
void CList::ComputeChildrenVisibility()
{
    for (unsigned long i = 0; i < ScrollingChildren.size(); ++i)
    {
        if (ScrollingChildren[i]->GetParent() == this)
        {
            CComponent* child = ScrollingChildren[i];
            C2DVector position(child->GetTargetRelativeRenderPosition().X, child->GetTargetRelativeRenderPosition().Y);
            child = ScrollingChildren[i];
            C2DVector size(child->GetSize().X, child->GetSize().Y);
            size.X *= CManager::GetInstance()->GetUIScale().X;
            size.Y *= CManager::GetInstance()->GetUIScale().Y;
            C2DVector end(position.X + size.X, position.Y + size.Y);
            C2DBoxF window(CUIScreenUnitConverter::ConvertX((float)LoadAndReturnDef()->ViewArea.TLX),
                           CUIScreenUnitConverter::ConvertY((float)LoadAndReturnDef()->ViewArea.TLY),
                           CUIScreenUnitConverter::ConvertX((float)LoadAndReturnDef()->ViewArea.TLX) - LoadAndReturnDef()->ViewArea.TLX + LoadAndReturnDef()->ViewArea.BRX,
                           CUIScreenUnitConverter::ConvertY((float)LoadAndReturnDef()->ViewArea.TLY) - LoadAndReturnDef()->ViewArea.TLY + LoadAndReturnDef()->ViewArea.BRY);
            if (PInitialPositionDummy)
            {
                C2DVector dummy = PInitialPositionDummy->GetRelativeRenderPosition();
                window.TLY = dummy.Y;
                window.BRY = LoadAndReturnDef()->ViewArea.BRY + dummy.Y - LoadAndReturnDef()->ViewArea.TLY;
            }
            window.BRX += 1.0f;
            window.BRY += 1.0f;
            window.TLX -= 1.0f;
            window.TLY -= 1.0f;
            window.BRX = (window.BRX - window.TLX) * CManager::GetInstance()->GetUIScale().X + window.TLX;
            window.BRY = (window.BRY - window.TLY) * CManager::GetInstance()->GetUIScale().Y + window.TLY;
            if (window.ContainsPoint(C2DVector(position)) && window.ContainsPoint(C2DVector(end)))
            {
                if (ScrollingChildren[i]->GetTargetColour().A == 0)
                {
                    CRGBColour colour = ScrollingChildren[i]->GetColour();
                    CRGBColour newColour(colour.R, colour.G, colour.B, 255);
                    ScrollingChildren[i]->ChangeColour(newColour, 0.0f, false);
                    ScrollingChildrenVisibility[i] = true;
                }
                if (!ScrollingChildrenVisibility[i])
                    ScrollingChildrenVisibility[i] = true;
            }
            else
            {
                if (ScrollingChildren[i]->GetTargetColour().A != 0)
                {
                    CRGBColour colour = ScrollingChildren[i]->GetColour();
                    CRGBColour newColour(colour.R, colour.G, colour.B, 0);
                    ScrollingChildren[i]->ChangeColour(newColour, 0.0f, false);
                    ScrollingChildrenVisibility[i] = false;
                }
                if (ScrollingChildrenVisibility[i])
                    ScrollingChildrenVisibility[i] = false;
            }
        }
    }
    for (unsigned long j = 0; j < ScrollingChildren.size(); ++j)
    {
        if (ScrollingChildren[j]->GetParent() == this)
        {
            if (ScrollingChildrenVisibility[j])
            {
                if (GetCurrentStateIndex() == UI_STATE_SELECTED && GetTargetStateIndex() == UI_STATE_SELECTED && RequestedState == UI_STATE_SELECTED)
                    ScrollingChildren[j]->ObserveEvent(UI_EVENT_MOUSE_MOVEMENT);
            }
            else
            {
                if (GetCurrentStateIndex() == UI_STATE_SELECTED && GetTargetStateIndex() == UI_STATE_SELECTED && RequestedState == UI_STATE_SELECTED)
                    ScrollingChildren[j]->IgnoreEvent(UI_EVENT_MOUSE_MOVEMENT);
            }
        }
    }
    if (SelectedChild >= 0 && (unsigned long)SelectedChild < ScrollingChildrenVisibility.size() && !ScrollingChildrenVisibility[SelectedChild])
    {
        long first = -1;
        long last = -1;
        long newSelected = 0;
        for (unsigned long k = 0; k < ScrollingChildrenVisibility.size(); ++k)
        {
            if (first == -1 && ScrollingChildrenVisibility[k])
                first = k;
            if (first != -1 && !ScrollingChildrenVisibility[k])
            {
                last = k - 1;
                break;
            }
        }
        if (last == -1)
            last = ScrollingChildrenVisibility.size() - 1;
        if (SelectedChild > last)
            newSelected = last;
        else if (first != -1)
            newSelected = first;
        if (newSelected != -1)
        {
            ScrollingChildren[SelectedChild]->IgnoreEvent(UI_EVENT_LEFT);
            ScrollingChildren[SelectedChild]->IgnoreEvent(UI_EVENT_RIGHT);
            ScrollingChildren[SelectedChild]->IgnoreEvent(UI_EVENT_SELECT);
            ScrollingChildren[SelectedChild]->IgnoreEvent(UI_EVENT_BACK);
            ScrollingChildren[SelectedChild]->ChangeState(UI_STATE_UNSELECTED);
            for (int n = 0; n < 10; ++n)
                ScrollingChildren[SelectedChild]->Update(0.2f);
            SetSelectedChild(newSelected);
            ScrollingChildren[SelectedChild]->ChangeState(UI_STATE_SELECTED);
            for (int m = 0; m < 10; ++m)
                ScrollingChildren[SelectedChild]->Update(0.2f);
            ScrollingChildren[SelectedChild]->ObserveEvent(UI_EVENT_LEFT);
            ScrollingChildren[SelectedChild]->ObserveEvent(UI_EVENT_RIGHT);
            ScrollingChildren[SelectedChild]->ObserveEvent(UI_EVENT_SELECT);
            ScrollingChildren[SelectedChild]->ObserveEvent(UI_EVENT_BACK);
            for (unsigned long n2 = 0; n2 < NonScrollingChildren.size(); ++n2)
            {
                if (NonScrollingChildren[n2]->HasState(SelectedChild + UI_STATE_CUSTOM) || NonScrollingChildren[n2]->HasStateInChildren(SelectedChild + UI_STATE_CUSTOM))
                    NonScrollingChildren[n2]->ChangeState(SelectedChild + UI_STATE_CUSTOM);
            }
        }
    }
}
