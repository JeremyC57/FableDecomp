// NUISystem::CScrollBar::ProcessEvent -- process a drag and forward the event to the clickable control.
// Leaf: ?ProcessEvent@CScrollBar@NUISystem@@UAEXW4EEvent@2@@Z
// Flags: none
// Inline-budget: the visible map COMDAT preserves the nonescaping key. Keep its tree lower-bound walk out of line at 0x0042AD6C.
// Retail: Fable.exe @ 0x00559A90, 2163 bytes (`ret 4`). CObserver vtable 0x0124B7BC slot 1 (this = the CObserver subobject, full-4).
// Callees: CManager::GetInstance 0x0041E5F2, CManager::GetUIScale 0x0041CF47, std::map<unsigned long,CUIState>::operator[] 0x0042D5B1,
//   own slots 108 LoadAndReturnDef (+0x1B0), 82 GetStates (+0x148), 32 ChangePosition (+0x80), 31 ChangePositionDelta (+0x7C), 154..159
//   (SetDeltaForActionUp 0x00559650 .. SetMaxAmountActionsDown 0x005596C0), CClickable::ProcessEvent 0x0055AD60 (tail).
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_CUIDEF
#define FABLE_RETAIL_CLIST
#define FABLE_RETAIL_PASS22_SLIDER_PROCESS_EVENT
#define FABLE_RETAIL_CMANAGER
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_STLP_TREE_BOUNDS_OUTOFLINE
#define FABLE_RETAIL_STLP_MAP_INDEX_DECL_ONLY
#define FABLE_RETAIL_STLP_CUISTATE_MAP_OWNERS_PASS28
#include "engine/CDefPointer.h"
#include "engine/NUISystem_CUIDef.h"
#include "engine/NUISystem_CUIState.h"
#include "engine/NUISystem_CScrollBar.h"
#include "engine/NUISystem_CManager.h"
#include "engine/NUISystem_CList.h"
#include "stlport/fable_map.h"
namespace NUISystem {
void CScrollBar::ProcessEvent(EEvent event)
{
    switch (event) {
    case UI_EVENT_MOUSE_MOVEMENT:
        if (WasLeftClicked && !WasDragged) {
            WasDragged = true;
            PositionBeforeDrag = Position;
            DeltaBeforeAction.X = 0.0f;
            DeltaBeforeAction.Y = 0.0f;
        }
        if (WasDragged) {
            C2DVector delta_move = CManager::GetInstance()->GetMouseMovement();
            DeltaBeforeAction += delta_move;
            delta_move.X = 0.0f;
            if (DeltaBeforeAction.Y == 0.0f || delta_move.Y / DeltaBeforeAction.Y < 0.0f)
                DeltaBeforeAction = delta_move;
            if (delta_move.Y < 0.0f) {
                if (MaxAmountActionsUp > 0) {
                    float delta_size = LoadAndReturnDef()->PositionOffset.Y - LoadAndReturnDef()->PositionOffset.Y * TargetZoom.Y;
                    float delta_first_action = DeltaForActionUp.Y - (MaxAmountActionsUp * DeltaForActionUp.Y - (Position.Y - GetStates()[0].Position.Y));
                    if ((MaxAmountActionsUp - 1) * DeltaForActionUp.Y + delta_first_action < fabs(DeltaBeforeAction.Y)) {
                        delta_move.Y = -(((MaxAmountActionsUp - 1) * DeltaForActionUp.Y + delta_first_action) - (delta_move.Y - DeltaBeforeAction.Y));
                        SetAmountActionsUp(MaxAmountActionsUp);
                    } else if (fabs(DeltaBeforeAction.Y) >= delta_first_action)
                        SetAmountActionsUp((unsigned long)fabs((fabs(DeltaBeforeAction.Y) - delta_first_action) / DeltaForActionUp.Y) + 1);
                    else
                        SetAmountActionsUp(0);
                    delta_move.X /= CManager::GetInstance()->GetUIScale().X;
                    delta_move.Y /= CManager::GetInstance()->GetUIScale().Y;
                    ChangePositionDelta(delta_move, 0.0f, false);
                    if (TargetPosition.Y < GetStates()[0].Position.Y) {
                        ChangePosition(GetStates()[0].Position, 0.0f, false);
                        DeltaBeforeAction.Y = 0.0f;
                        DeltaBeforeAction.X = 0.0f;
                    }
                    PositionBeforeDrag = Position;
                    if (PActionsOnDraggedUp && !PActionsOnDraggedUp->empty()
                        && PActionsOnDraggedUp->front()->ActionType == UI_ACTION_TYPE_SCROLL_ACTIVE_LIST_DOWN) {
                        C2DVector where(0.0f, AmountActionsUp * 25.0f);
                        CList::GetPreviousSelected().front()->ChangeTargetPositionDelta(where, 0.0f, false);
                    } else {
                        for (unsigned long count = 0; count < AmountActionsUp; ++count)
                            ExecuteActions(PActionsOnDraggedUp);
                    }
                    if (AmountActionsUp > 0) {
                        if (MaxAmountActionsUp == AmountActionsUp)
                            DeltaBeforeAction.Y = 0.0f;
                        else
                            DeltaBeforeAction.Y = DeltaBeforeAction.Y + delta_first_action + AmountActionsUp * DeltaForActionUp.Y;
                        if (MaxAmountActionsDown == 0)
                            SetDeltaForActionDown(DeltaForActionUp);
                        SetMaxAmountActionsDown(MaxAmountActionsDown + AmountActionsUp);
                        SetMaxAmountActionsUp(MaxAmountActionsUp - AmountActionsUp);
                        SetAmountActionsUp(0);
                    }
                }
            } else if (delta_move.Y > 0.0f) {
                if (MaxAmountActionsDown > 0) {
                    float delta_first_action = DeltaForActionDown.Y - ((Position.Y - GetStates()[0].Position.Y) - MaxAmountActionsUp * DeltaForActionUp.Y);
                    if ((MaxAmountActionsDown - 1) * DeltaForActionDown.Y + delta_first_action < fabs(DeltaBeforeAction.Y)) {
                        delta_move.Y = ((MaxAmountActionsDown - 1) * DeltaForActionDown.Y + delta_first_action) - (DeltaBeforeAction.Y - delta_move.Y);
                        SetAmountActionsDown(MaxAmountActionsDown);
                    } else if (DeltaBeforeAction.Y >= delta_first_action)
                        SetAmountActionsDown((unsigned long)(fabs(DeltaBeforeAction.Y - delta_first_action) / DeltaForActionDown.Y) + 1);
                    else
                        SetAmountActionsDown(0);
                    delta_move.X /= CManager::GetInstance()->GetUIScale().X;
                    delta_move.Y /= CManager::GetInstance()->GetUIScale().Y;
                    ChangePositionDelta(delta_move, 0.0f, false);
                    if (TargetPosition.Y + LoadAndReturnDef()->PositionOffset.Y * TargetZoom.Y > GetStates()[0].Position.Y + LoadAndReturnDef()->PositionOffset.Y) {
                        C2DVector new_pos = GetStates()[0].Position + LoadAndReturnDef()->PositionOffset - LoadAndReturnDef()->PositionOffset * TargetZoom.Y;
                        ChangePosition(new_pos, 0.0f, false);
                        DeltaBeforeAction.Y = 0.0f;
                        DeltaBeforeAction.X = 0.0f;
                    }
                    PositionBeforeDrag = Position;
                    if (PActionsOnDraggedDown && !PActionsOnDraggedDown->empty()
                        && PActionsOnDraggedDown->front()->ActionType == UI_ACTION_TYPE_SCROLL_ACTIVE_LIST_UP) {
                        C2DVector where(0.0f, AmountActionsDown * -25.0f);
                        CList::GetPreviousSelected().front()->ChangeTargetPositionDelta(where, 0.0f, false);
                    } else {
                        for (unsigned long count = 0; count < AmountActionsDown; ++count)
                            ExecuteActions(PActionsOnDraggedDown);
                    }
                    if (AmountActionsDown != 0) {
                        if (MaxAmountActionsDown == AmountActionsDown)
                            DeltaBeforeAction.Y = 0.0f;
                        else
                            DeltaBeforeAction.Y = DeltaBeforeAction.Y - delta_first_action - AmountActionsDown * DeltaForActionDown.Y;
                        if (MaxAmountActionsUp == 0)
                            SetDeltaForActionUp(DeltaForActionDown);
                        SetMaxAmountActionsUp(MaxAmountActionsUp + AmountActionsDown);
                        SetMaxAmountActionsDown(MaxAmountActionsDown - AmountActionsDown);
                        SetAmountActionsDown(0);
                    }
                }
            }
        }
        break;
    }
    CClickable::ProcessEvent(event);
}
}