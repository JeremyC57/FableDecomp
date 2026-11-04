// NUISystem::CScrollBar::ProcessEvent(EEvent event) -- scroll-bar thumb drag: on mouse movement while dragged, accumulate the vertical drag, move the thumb, fire the dragged-up / dragged-down actions once per step and rebalance the step budgets; then the CClickable handler
// Grade: DIFFER(2144v2163) -- residue: frame-slot sharing only: retail gives every LoadAndReturnDef CDefPointer temporary and every GetUIScale result its own frame slot (`sub esp,5Ch`) where VC7.1 shares them across statements and the UP / DOWN blocks (`sub esp,30h`), so three dead `mov [slot],0` def-pointer stores are dropped and one alignment pad moves; instruction stream otherwise equal; 78 diff lines, 11 grades
// Leaf: ?ProcessEvent@CScrollBar@NUISystem@@UAEXW4EEvent@2@@Z
// Flags: none
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
#define FABLE_RETAIL_CMANAGER
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_STLP_MAP
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
    if (event == UI_EVENT_MOUSE_MOVEMENT) {
        if (WasLeftClicked && !WasDragged) {
            WasDragged = true;
            PositionBeforeDrag = Position;
            DeltaBeforeAction.X = 0.0f;
            DeltaBeforeAction.Y = 0.0f;
        }
        if (WasDragged) {
            C2DVector movement = CManager::GetInstance()->GetMouseMovement();
            DeltaBeforeAction += movement;
            movement.X = 0.0f;
            if (DeltaBeforeAction.Y == 0.0f || movement.Y / DeltaBeforeAction.Y < 0.0f)
                DeltaBeforeAction = movement;
            if (movement.Y < 0.0f) {
                if (MaxAmountActionsUp > 0) {
                    float top = LoadAndReturnDef()->PositionOffset.Y + LoadAndReturnDef()->PositionOffset.Y;
                    float start = DeltaForActionUp.Y - (MaxAmountActionsUp * DeltaForActionUp.Y - (Position.Y - GetStates()[0].Position.Y));
                    float end = (MaxAmountActionsUp - 1) * DeltaForActionUp.Y + start;
                    float dragged = fabs(DeltaBeforeAction.Y);
                    if (end < dragged) {
                        movement.Y = -(end - (movement.Y - DeltaBeforeAction.Y));
                        SetAmountActionsUp(MaxAmountActionsUp);
                    } else if (dragged >= start)
                        SetAmountActionsUp((unsigned long)fabs((dragged - start) / DeltaForActionUp.Y) + 1);
                    else
                        SetAmountActionsUp(0);
                    movement.X /= CManager::GetInstance()->GetUIScale().X;
                    movement.Y /= CManager::GetInstance()->GetUIScale().Y;
                    ChangePositionDelta(movement, 0.0f, false);
                    if (TargetPosition.Y < GetStates()[0].Position.Y) {
                        ChangePosition(GetStates()[0].Position, 0.0f, false);
                        DeltaBeforeAction.Y = 0.0f;
                        DeltaBeforeAction.X = 0.0f;
                    }
                    PositionBeforeDrag = Position;
                    if (PActionsOnDraggedUp && !PActionsOnDraggedUp->empty()
                        && PActionsOnDraggedUp->front()->ActionType == UI_ACTION_TYPE_SCROLL_ACTIVE_LIST_DOWN) {
                        C2DVector scroll(0.0f, AmountActionsUp * 25.0f);
                        CList::GPreviousSelected.front()->ChangeTargetPositionDelta(scroll, 0.0f, false);
                    } else {
                        for (unsigned long i = 0; i < AmountActionsUp; ++i)
                            ExecuteActions(PActionsOnDraggedUp);
                    }
                    if (AmountActionsUp > 0) {
                        if (MaxAmountActionsUp == AmountActionsUp)
                            DeltaBeforeAction.Y = 0.0f;
                        else
                            DeltaBeforeAction.Y += AmountActionsUp * DeltaForActionUp.Y + start;
                        if (MaxAmountActionsDown == 0)
                            SetDeltaForActionDown(DeltaForActionUp);
                        SetMaxAmountActionsDown(MaxAmountActionsDown + AmountActionsUp);
                        SetMaxAmountActionsUp(MaxAmountActionsUp - AmountActionsUp);
                        SetAmountActionsUp(0);
                    }
                }
            } else if (movement.Y > 0.0f) {
                if (MaxAmountActionsDown > 0) {
                    float start = DeltaForActionDown.Y - ((Position.Y - GetStates()[0].Position.Y) - MaxAmountActionsUp * DeltaForActionUp.Y);
                    float end = (MaxAmountActionsDown - 1) * DeltaForActionDown.Y + start;
                    if (end < fabs(DeltaBeforeAction.Y)) {
                        movement.Y = end - (DeltaBeforeAction.Y - movement.Y);
                        SetAmountActionsDown(MaxAmountActionsDown);
                    } else if (DeltaBeforeAction.Y >= start)
                        SetAmountActionsDown((unsigned long)(fabs(DeltaBeforeAction.Y - start) / DeltaForActionDown.Y) + 1);
                    else
                        SetAmountActionsDown(0);
                    movement.X /= CManager::GetInstance()->GetUIScale().X;
                    movement.Y /= CManager::GetInstance()->GetUIScale().Y;
                    ChangePositionDelta(movement, 0.0f, false);
                    if (TargetPosition.Y + LoadAndReturnDef()->PositionOffset.Y * TargetZoom.Y > GetStates()[0].Position.Y + LoadAndReturnDef()->PositionOffset.Y) {
                        C2DVector position = GetStates()[0].Position + LoadAndReturnDef()->PositionOffset - LoadAndReturnDef()->PositionOffset * TargetZoom.Y;
                        ChangePosition(position, 0.0f, false);
                        DeltaBeforeAction.Y = 0.0f;
                        DeltaBeforeAction.X = 0.0f;
                    }
                    PositionBeforeDrag = Position;
                    if (PActionsOnDraggedDown && !PActionsOnDraggedDown->empty()
                        && PActionsOnDraggedDown->front()->ActionType == UI_ACTION_TYPE_SCROLL_ACTIVE_LIST_UP) {
                        C2DVector scroll(0.0f, AmountActionsDown * -25.0f);
                        CList::GPreviousSelected.front()->ChangeTargetPositionDelta(scroll, 0.0f, false);
                    } else {
                        for (unsigned long i = 0; i < AmountActionsDown; ++i)
                            ExecuteActions(PActionsOnDraggedDown);
                    }
                    if (AmountActionsDown != 0) {
                        if (MaxAmountActionsDown == AmountActionsDown)
                            DeltaBeforeAction.Y = 0.0f;
                        else
                            DeltaBeforeAction.Y = DeltaBeforeAction.Y - start - AmountActionsDown * DeltaForActionDown.Y;
                        if (MaxAmountActionsUp == 0)
                            SetDeltaForActionUp(DeltaForActionDown);
                        SetMaxAmountActionsUp(MaxAmountActionsUp + AmountActionsDown);
                        SetMaxAmountActionsDown(MaxAmountActionsDown - AmountActionsDown);
                        SetAmountActionsDown(0);
                    }
                }
            }
        }
    }
    CClickable::ProcessEvent(event);
}
}
