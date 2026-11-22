// 00559A90 source-body test. The UI doubles record positions, actions, and event forwarding.
#include <cstdio>
#include <cmath>
#include <list>
#include <map>
#define FABLE_RETAIL_C2DVECTOR
#include "engine/C2DVector.h"

namespace NUISystem {
enum EEvent { UI_EVENT_MOUSE_MOVEMENT, UI_EVENT_CLICK };
enum EActionType { UI_ACTION_TYPE_SCROLL_ACTIVE_LIST_UP, UI_ACTION_TYPE_SCROLL_ACTIVE_LIST_DOWN, UI_ACTION_TYPE_OTHER };
struct CAction { EActionType ActionType; };
struct CUIState { C2DVector Position; CUIState() : Position(0, 0) {} };
struct CUIDef { C2DVector PositionOffset; CUIDef() : PositionOffset(0, 40) {} };

class CManager {
public:
    C2DVector MouseMovement, UIScale;
    CManager() : MouseMovement(0, 0), UIScale(1, 1) {}
    static CManager* GetInstance() { static CManager manager; return &manager; }
    const C2DVector& GetMouseMovement() const { return MouseMovement; }
    C2DVector GetUIScale() const { return UIScale; }
};

class CList {
public:
    C2DVector Offset;
    static std::list<CList*> GPreviousSelected;
    static std::list<CList*>& GetPreviousSelected() { return GPreviousSelected; }
    CList() : Offset(0, 0) {}
    void ChangeTargetPositionDelta(const C2DVector& delta, float, bool) { Offset += delta; }
};
std::list<CList*> CList::GPreviousSelected;

class CClickable {
public:
    unsigned ClickableEvents;
    EEvent LastEvent;
    CClickable() : ClickableEvents(0), LastEvent(UI_EVENT_CLICK) {}
    void ProcessEvent(EEvent event) { ++ClickableEvents; LastEvent = event; }
};

class CScrollBar : public CClickable {
public:
    bool WasLeftClicked, WasDragged;
    C2DVector Position, PositionBeforeDrag, DeltaBeforeAction, DeltaForActionUp, DeltaForActionDown;
    C2DVector TargetPosition, TargetZoom, LastDelta;
    unsigned long MaxAmountActionsUp, MaxAmountActionsDown, AmountActionsUp, AmountActionsDown;
    unsigned UpActions, DownActions, PositionCalls, DefReads;
    CUIDef Def;
    std::map<unsigned long, CUIState> States;
    std::list<CAction*> UpActionsList, DownActionsList;
    std::list<CAction*>* PActionsOnDraggedUp;
    std::list<CAction*>* PActionsOnDraggedDown;

    CScrollBar(float y, bool clicked, bool dragged)
        : WasLeftClicked(clicked), WasDragged(dragged), Position(0, y), PositionBeforeDrag(0, 0),
          DeltaBeforeAction(0, 0), DeltaForActionUp(0, 10), DeltaForActionDown(0, 0),
          TargetPosition(0, y), TargetZoom(1, 0.25f), LastDelta(0, 0), MaxAmountActionsUp(3),
          MaxAmountActionsDown(0), AmountActionsUp(0), AmountActionsDown(0), UpActions(0), DownActions(0),
          PositionCalls(0), DefReads(0), PActionsOnDraggedUp(&UpActionsList), PActionsOnDraggedDown(&DownActionsList)
    {}
    CUIDef* LoadAndReturnDef() { ++DefReads; return &Def; }
    std::map<unsigned long, CUIState>& GetStates() { return States; }
    void ChangePosition(const C2DVector& position, float, bool) {
        Position = position;
        TargetPosition = position;
        ++PositionCalls;
    }
    void ChangePositionDelta(const C2DVector& delta, float, bool) {
        LastDelta = delta;
        Position += delta;
        TargetPosition += delta;
    }
    void ExecuteActions(const std::list<CAction*>* actions) {
        if (actions == PActionsOnDraggedUp) ++UpActions;
        if (actions == PActionsOnDraggedDown) ++DownActions;
    }
    void SetAmountActionsUp(unsigned long count) { AmountActionsUp = count; }
    void SetAmountActionsDown(unsigned long count) { AmountActionsDown = count; }
    void SetMaxAmountActionsUp(unsigned long count) { MaxAmountActionsUp = count; }
    void SetMaxAmountActionsDown(unsigned long count) { MaxAmountActionsDown = count; }
    void SetDeltaForActionUp(const C2DVector& delta) { DeltaForActionUp = delta; }
    void SetDeltaForActionDown(const C2DVector& delta) { DeltaForActionDown = delta; }
    void ProcessEvent(EEvent event);
};
}

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

using namespace NUISystem;

static void Move(CScrollBar& bar, float x, float y)
{
    CManager::GetInstance()->MouseMovement = C2DVector(x, y);
    bar.ProcessEvent(UI_EVENT_MOUSE_MOVEMENT);
}

static int Check()
{
    CScrollBar idle(30, false, false);
    Move(idle, 9, -14);
    if (idle.WasDragged || idle.LastDelta.Y != 0 || idle.ClickableEvents != 1)
        return 1;

    CScrollBar fresh(100, true, false);
    fresh.DeltaBeforeAction.Y = 99;
    Move(fresh, 12, -4);
    if (!fresh.WasDragged || fresh.UpActions || fresh.Position.Y != 96 || fresh.PositionBeforeDrag.Y != 96 ||
        fresh.DeltaBeforeAction.Y != -4 || fresh.LastDelta.X != 0 || fresh.DefReads != 2)
        return 2;

    CScrollBar capped(30, true, true);
    Move(capped, 0, -50);
    if (capped.UpActions != 3 || capped.MaxAmountActionsUp || capped.MaxAmountActionsDown != 3 ||
        capped.LastDelta.Y != -30 || capped.DeltaBeforeAction.Y != 0 || capped.DeltaForActionDown.Y != 10)
        return 3;
    Move(capped, 0, 25);
    if (capped.DownActions != 2 || capped.MaxAmountActionsDown != 1 || capped.MaxAmountActionsUp != 2 ||
        capped.DeltaBeforeAction.Y != -5)
        return 4;
    capped.ProcessEvent(UI_EVENT_CLICK);
    if (capped.ClickableEvents != 3 || capped.LastEvent != UI_EVENT_CLICK)
        return 5;

    CScrollBar listBar(30, true, true);
    CAction up = { UI_ACTION_TYPE_SCROLL_ACTIVE_LIST_DOWN };
    CAction down = { UI_ACTION_TYPE_SCROLL_ACTIVE_LIST_UP };
    CList active;
    CList::GetPreviousSelected().push_back(&active);
    listBar.UpActionsList.push_back(&up);
    listBar.DownActionsList.push_back(&down);
    Move(listBar, 0, -25);
    if (active.Offset.Y != 50 || listBar.UpActions || listBar.MaxAmountActionsDown != 2)
        return 6;
    Move(listBar, 0, 50);
    if (active.Offset.Y != 0 || listBar.DownActions || listBar.MaxAmountActionsUp != 3)
        return 7;

    CScrollBar scaled(30, true, true);
    CManager::GetInstance()->UIScale.Y = 0.5f;
    Move(scaled, 0, -30);
    if (scaled.LastDelta.Y != -60 || scaled.Position.Y != 0 || scaled.PositionCalls != 1)
        return 8;
    Move(scaled, 0, 30);
    if (scaled.LastDelta.Y != 60 || scaled.Position.Y != 30 || scaled.PositionCalls != 2)
        return 9;
    return 0;
}

int main()
{
    const int result = Check();
    if (result) {
        std::printf("00559a90_TEST FAIL %d\n", result);
        return 1;
    }
    std::printf("00559a90_TEST PASS\n");
    return 0;
}