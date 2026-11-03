// NUISystem::CScrollingViewport::Update(float) -- clips the children against the window, sets the scroll offsets and sizes the scroll bar
// Grade: DIFFER(1601v1601) -- one x87 choice: retail pops ratio to its frame word and multiplies the window height left on the stack by it (fstp [ratio] ; fmul [ratio]); VC7.1 here keeps ratio on the stack (fst ; fmul st(1)) and pops the height later (4 masked lines)
// Leaf: ?Update@CScrollingViewport@NUISystem@@UAEXM@Z
// 
// Retail 0x0053F1C0, 1601 bytes, CScrollingViewport vtable 0x012468D4 slot 1 (catalog "vfunc_1"), ret 4.
// Builds the scaled window box (CManager::GetUIScale 0x0041CF47, GetRenderPosition +0x64), takes the largest CanScroll
// (+0x160) offsets above/below over Children and FakeChildren, marks Changed when they move, then sizes and places the
// scroll bar (ChangeZoom +0x8C, ChangePosition +0x7C, CDraggable IsDragged/SetPositionBeforeDrag, CScrollBar delta /
// max-action setters) and ends with CChangingStateComponent::Update 0x0052C7E0.
// Headers: NUISystem_CScrollingViewport.h / NUISystem_CScrollBar.h / NUISystem_CViewport.h (new), C2DBoxF and CManager retail branches.
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_C2DBOXF
#define FABLE_RETAIL_CUIDEF
#define FABLE_RETAIL_CMANAGER
#include <math.h>
#include "engine/CDefPointer.h"
#include "engine/NUISystem_CUIDef.h"
#include "engine/NUISystem_CScrollingViewport.h"
#include "engine/NUISystem_CScrollBar.h"
#include "engine/NUISystem_CManager.h"
#include "stlport/fable_map.h"
#include "engine/NUISystem_CUIState.h"
using namespace NUISystem;
void CScrollingViewport::Update(float dt)
{
    C2DBoxF window;
    window.TLX = Window.GetTLX() * CManager::GetInstance()->GetUIScale().X + GetRenderPosition().X;
    window.TLY = Window.GetTLY() * CManager::GetInstance()->GetUIScale().Y + GetRenderPosition().Y;
    float width = Window.GetWidth();
    window.BRX = width * CManager::GetInstance()->GetUIScale().X + window.TLX;
    float height = Window.GetHeight();
    window.BRY = height * CManager::GetInstance()->GetUIScale().Y + window.TLY;
    float above = 0.0f;
    float under = 0.0f;
    bool noScrollingChild = true;
    unsigned long i;
    for (i = 0; i < Children.size(); ++i)
    {
        if (Children[i]->IsAllowedToScroll() && Children[i].Data != PScrollBar && Children[i].Data != PScrollBarOutside)
        {
            Children[i]->GetRenderPosition();
            float offset = Children[i]->CanScroll(true, window);
            if (fabs(above) < fabs(offset))
                above = offset;
            noScrollingChild = false;
        }
    }
    for (i = 0; i < FakeChildren.size(); ++i)
    {
        if (FakeChildren[i]->IsAllowedToScroll())
        {
            FakeChildren[i]->GetRenderPosition();
            float offset = FakeChildren[i]->CanScroll(true, window);
            if (fabs(above) < fabs(offset))
                above = offset;
            noScrollingChild = false;
        }
    }
    for (i = 0; i < Children.size(); ++i)
    {
        if (Children[i]->IsAllowedToScroll() && Children[i].Data != PScrollBar && Children[i].Data != PScrollBarOutside)
        {
            Children[i]->GetRenderPosition();
            float offset = Children[i]->CanScroll(false, window);
            if (fabs(under) < fabs(offset))
                under = offset;
        }
    }
    for (i = 0; i < FakeChildren.size(); ++i)
    {
        if (FakeChildren[i]->IsAllowedToScroll())
        {
            FakeChildren[i]->GetRenderPosition();
            float offset = FakeChildren[i]->CanScroll(false, window);
            if (fabs(under) < fabs(offset))
                under = offset;
        }
    }
    if (OffsetToUseAbove != above || OffsetToUseUnder != under)
    {
        Changed = true;
        OffsetToUseUnder = under;
        OffsetToUseAbove = above;
    }
    if (PScrollBar)
    {
        float range;
        float ratio;
        C2DVector zoom;
        zoom.X = 1.0f;
        {
            float windowHeight = window.GetHeight();
            range = under - above;
            ratio = 1.0f / (range + windowHeight);
            zoom.Y = windowHeight * ratio;
        }
        if (noScrollingChild)
            zoom.Y = 0.0f;
        PScrollBar->ChangeZoom(zoom, 0.0f, false);
        CComponent* component;
        component = PScrollBar;
        C2DVector position = component->GetStates()[UI_STATE_INACTIVE].Position;
        float length = PScrollBar->LoadAndReturnDef()->PositionOffset.Y - zoom.Y * PScrollBar->LoadAndReturnDef()->PositionOffset.Y;
        if (range > 0.0f)
            position.Y += under / range * length;
        CDraggable* draggable = static_cast<CDraggable*>(PScrollBar);
        if (draggable && Changed)
        {
            if (under == 0.0f || above == 0.0f || !draggable->IsDragged())
            {
                PScrollBar->ChangePosition(position, 0.2f, false);
                draggable->SetPositionBeforeDrag(position);
            }
        }
        if (range > 0.0f)
        {
            C2DVector delta(0.0f, ratio * PScrollBar->LoadAndReturnDef()->PositionOffset.Y);
            C2DVector noDelta(-1.0f, -1.0f);
            CScrollBar* scrollBar = static_cast<CScrollBar*>(PScrollBar);
            if (scrollBar)
            {
                if (under > 0.0f)
                {
                    scrollBar->SetDeltaForActionUp(delta);
                    scrollBar->SetMaxAmountActionsUp((unsigned long)under);
                }
                else
                {
                    scrollBar->SetDeltaForActionUp(noDelta);
                    scrollBar->SetMaxAmountActionsUp(0);
                }
                if (above < 0.0f)
                {
                    scrollBar->SetDeltaForActionDown(delta);
                    scrollBar->SetMaxAmountActionsDown((unsigned long)-above);
                }
                else
                {
                    scrollBar->SetDeltaForActionDown(noDelta);
                    scrollBar->SetMaxAmountActionsDown(0);
                }
            }
        }
    }
    Changed = false;
    CViewport::Update(dt);
}
