// Behaviour test for NUISystem::CScrollBar::OnLeftClicked (0x005596D0): on the first click of a drag the bar remembers
// its position and clears the accumulated delta; the draggable handling runs in every case.
#include <cstdio>
static int g_base = 0;
class C2DVector { public: float X, Y; };
class CDraggable { public:
    C2DVector Position; C2DVector PositionBeforeDrag; bool WasDragged;
    virtual ~CDraggable() { }
    virtual void OnLeftClicked() { ++g_base; WasDragged = true; }
};
class CScrollBar : public CDraggable { public:
    C2DVector DeltaBeforeAction;
    virtual void OnLeftClicked()
    {
        if (!WasDragged) {
            PositionBeforeDrag = Position;
            DeltaBeforeAction.X = 0.0f;
            DeltaBeforeAction.Y = 0.0f;
        }
        CDraggable::OnLeftClicked();
    }
};
int main()
{
    CScrollBar bar; bar.WasDragged = false; bar.Position.X = 3.0f; bar.Position.Y = 4.0f;
    bar.PositionBeforeDrag.X = bar.PositionBeforeDrag.Y = -1.0f; bar.DeltaBeforeAction.X = 5.0f; bar.DeltaBeforeAction.Y = 6.0f;
    CDraggable& d = bar;
    d.OnLeftClicked();
    if (g_base != 1 || bar.PositionBeforeDrag.X != 3.0f || bar.PositionBeforeDrag.Y != 4.0f || bar.DeltaBeforeAction.X != 0.0f
        || bar.DeltaBeforeAction.Y != 0.0f) { std::printf("005596d0_TEST FAIL\n"); return 1; }
    bar.Position.X = 8.0f; bar.DeltaBeforeAction.X = 2.0f;
    d.OnLeftClicked();                      // already dragging: nothing reset
    if (g_base != 2 || bar.PositionBeforeDrag.X != 3.0f || bar.DeltaBeforeAction.X != 2.0f) { std::printf("005596d0_TEST FAIL\n"); return 1; }
    std::printf("005596d0_TEST PASS\n");
    return 0;
}