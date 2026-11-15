// 0053F1C0 NUISystem::CScrollingViewport::Update -- clip child scrolling and update the scrollbar's zoom, position and actions.
// Retail:0x0053F1C0,1601bytes,thiscall ret4.
// Leaf: ?Update@CScrollingViewport@NUISystem@@UAEXM@Z
// Flags: none
// Original iterator CFG has the constant-false arm before each scoped loop; macro spelling is unknown.
// Original GetExtents value API and natural grouped quotient retain the observed native x87 evaluation.
// Called STLport map/tree bodies select ordinary separately compiled library COMDATs; see LIBRARY_BUILD.json.
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_C2DBOXF
#define FABLE_RETAIL_C2DBOXF_GETEXTENTS
#define FABLE_RETAIL_CUIDEF
#define FABLE_RETAIL_CMANAGER
#define FABLE_RETAIL_STLP_MAP
#include <math.h>
#include "engine/CDefPointer.h"
#include "engine/NUISystem_CUIDef.h"
#include "engine/NUISystem_CScrollingViewport.h"
#include "engine/NUISystem_CScrollBar.h"
#include "engine/NUISystem_CManager.h"
#include "stlport/fable_map.h"
#include "engine/NUISystem_CUIState.h"
#include "engine/GFDebugPDynamicCast.h"
using namespace NUISystem;
void CScrollingViewport::Update(float tick)
{
    C2DBoxF window_to_use;
    window_to_use.TLX = Window.GetTLX() * CManager::GetInstance()->GetUIScale().X + GetRenderPosition().X;
    window_to_use.TLY = Window.GetTLY() * CManager::GetInstance()->GetUIScale().Y + GetRenderPosition().Y;
    window_to_use.BRX = Window.GetExtents().X * CManager::GetInstance()->GetUIScale().X + window_to_use.TLX;
    window_to_use.BRY = Window.GetExtents().Y * CManager::GetInstance()->GetUIScale().Y + window_to_use.TLY;
    float offset_to_use_above = 0.0f;
    float offset_to_use_under = 0.0f;
    bool viewport_inactive = true;
    if (false) {} else for (unsigned long count = 0; count < Children.size(); ++count)
    {
        if (Children[count]->IsAllowedToScroll() && Children[count].Get() != PScrollBar && Children[count].Get() != PScrollBarOutside)
        {
            C2DVector position = Children[count]->GetRenderPosition();
            float allowed_offset = Children[count]->CanScroll(true, window_to_use);
            if (fabs(offset_to_use_above) < fabs(allowed_offset))
                offset_to_use_above = allowed_offset;
            viewport_inactive = false;
        }
    }
    if (false) {} else for (unsigned long count = 0; count < FakeChildren.size(); ++count)
    {
        if (FakeChildren[count]->IsAllowedToScroll())
        {
            C2DVector position = FakeChildren[count]->GetRenderPosition();
            float allowed_offset = FakeChildren[count]->CanScroll(true, window_to_use);
            if (fabs(offset_to_use_above) < fabs(allowed_offset))
                offset_to_use_above = allowed_offset;
            viewport_inactive = false;
        }
    }
    if (false) {} else for (unsigned long count = 0; count < Children.size(); ++count)
    {
        if (Children[count]->IsAllowedToScroll() && Children[count].Get() != PScrollBar && Children[count].Get() != PScrollBarOutside)
        {
            C2DVector position = Children[count]->GetRenderPosition();
            float allowed_offset = Children[count]->CanScroll(false, window_to_use);
            if (fabs(offset_to_use_under) < fabs(allowed_offset))
                offset_to_use_under = allowed_offset;
        }
    }
    if (false) {} else for (unsigned long count = 0; count < FakeChildren.size(); ++count)
    {
        if (FakeChildren[count]->IsAllowedToScroll())
        {
            C2DVector position = FakeChildren[count]->GetRenderPosition();
            float allowed_offset = FakeChildren[count]->CanScroll(false, window_to_use);
            if (fabs(offset_to_use_under) < fabs(allowed_offset))
                offset_to_use_under = allowed_offset;
        }
    }
    if (OffsetToUseAbove != offset_to_use_above || OffsetToUseUnder != offset_to_use_under)
    {
        Changed = true;
        OffsetToUseUnder = offset_to_use_under;
        OffsetToUseAbove = offset_to_use_above;
    }
    if (PScrollBar)
    {
        C2DVector zoom(1.0f, 1.0f);
        float visible = window_to_use.GetBRY() - window_to_use.GetTLY();
        float invisible_above = offset_to_use_under;
        float invisible = -offset_to_use_above + offset_to_use_under;
        float invisible_under = offset_to_use_above;
        zoom.Y = visible / (visible + invisible);
        if (viewport_inactive)
            zoom.Y = 0.0f;
        PScrollBar->ChangeZoom(zoom, 0.0f, false);
        C2DVector position = PScrollBar->GetStates()[UI_STATE_INACTIVE].Position;
        float delta_size = PScrollBar->LoadAndReturnDef()->PositionOffset.Y - zoom.Y * PScrollBar->LoadAndReturnDef()->PositionOffset.Y;
        if (invisible > 0.0f)
            position.Y += (invisible_above / invisible) * delta_size;
        CScrollBar* pscroll_bar = GFDebugPDynamicCast(PScrollBar, (CScrollBar*)0);
        if (pscroll_bar && Changed)
        {
            if (offset_to_use_under == 0.0f || offset_to_use_above == 0.0f || !pscroll_bar->IsDragged())
            {
                PScrollBar->ChangePosition(position, 0.2f, false);
                pscroll_bar->SetPositionBeforeDrag(position);
            }
        }
        if (invisible > 0.0f)
        {
            C2DVector delta_action(0.0f, 1.0f * PScrollBar->LoadAndReturnDef()->PositionOffset.Y / (visible + invisible));
            C2DVector delta_action_null(-1.0f, -1.0f);
            CScrollBar* pscroll_bar2 = GFDebugPDynamicCast(PScrollBar, (CScrollBar*)0);
            if (pscroll_bar2)
            {
                if (offset_to_use_under > 0.0f)
                {
                    pscroll_bar2->SetDeltaForActionUp(delta_action);
                    pscroll_bar2->SetMaxAmountActionsUp((unsigned long)offset_to_use_under);
                }
                else
                {
                    pscroll_bar2->SetDeltaForActionUp(delta_action_null);
                    pscroll_bar2->SetMaxAmountActionsUp(0);
                }
                if (offset_to_use_above < 0.0f)
                {
                    pscroll_bar2->SetDeltaForActionDown(delta_action);
                    pscroll_bar2->SetMaxAmountActionsDown((unsigned long)-offset_to_use_above);
                }
                else
                {
                    pscroll_bar2->SetDeltaForActionDown(delta_action_null);
                    pscroll_bar2->SetMaxAmountActionsDown(0);
                }
            }
        }
    }
    Changed = false;
    CViewport::Update(tick);
}