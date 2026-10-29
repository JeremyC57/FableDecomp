// Behaviour test for NUISystem::CDraggable::OnLeftClicked (0x0055A5D0).
#include <cstdio>

struct C2DVector { float X, Y; };
struct CMouseCursor { int Closed; void TurnToClosedHand() { ++Closed; } };
struct CManager {
    CMouseCursor* PMouseCursor;
    static CManager* GetInstance() { static CManager m; return &m; }
    CMouseCursor* GetMouseCursor() const { return PMouseCursor; }
};
struct CClickable {
    C2DVector TargetPosition; char Layer; int Clicks;
    void OnLeftClicked() { ++Clicks; }
};
struct CDraggable : public CClickable {
    C2DVector PositionBeforeDrag; bool WasDragged;
    void OnLeftClicked()
    {
        CClickable::OnLeftClicked();
        if (!WasDragged) {
            WasDragged = true;
            PositionBeforeDrag = TargetPosition;
            --Layer;
        }
        CMouseCursor* cursor = CManager::GetInstance()->GetMouseCursor();
        if (cursor)
            cursor->TurnToClosedHand();
    }
};
int main()
{
    CMouseCursor cursor; cursor.Closed = 0;
    CManager::GetInstance()->PMouseCursor = 0;
    CDraggable d; d.TargetPosition.X = 3; d.TargetPosition.Y = 4; d.Layer = 5; d.Clicks = 0; d.WasDragged = false;
    d.OnLeftClicked();
    if (!d.WasDragged || d.PositionBeforeDrag.X != 3 || d.PositionBeforeDrag.Y != 4 || d.Layer != 4 || d.Clicks != 1) { std::printf("0055a5d0_TEST FAIL first\n"); return 1; }
    CManager::GetInstance()->PMouseCursor = &cursor;
    d.TargetPosition.X = 9;
    d.OnLeftClicked();
    if (d.PositionBeforeDrag.X != 3 || d.Layer != 4 || d.Clicks != 2 || cursor.Closed != 1) { std::printf("0055a5d0_TEST FAIL second\n"); return 1; }
    std::printf("0055a5d0_TEST PASS\n");
    return 0;
}
