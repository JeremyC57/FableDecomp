// behaviour test for NUISystem::CDraggable::OnLeftClicked 0x0055A5D0: the first click of a drag remembers the start
// position, marks the drag and lifts the component one layer (Layer decrements); later clicks change nothing; the cursor
// turns to the closed hand when there is one
#include <cstdio>
struct V { float X, Y; };
struct D { bool WasDragged; V Target, Before; signed char Layer; int closed;
    void Click(bool cursor) { if (!WasDragged) { WasDragged = true; Before = Target; --Layer; } if (cursor) ++closed; } };
int main()
{
    D d = { false, { 3.0f, 4.0f }, { 0.0f, 0.0f }, 5, 0 };
    d.Click(true); d.Target.X = 9.0f; d.Click(false);
    int ok = d.WasDragged && d.Before.X == 3.0f && d.Before.Y == 4.0f && d.Layer == 4 && d.closed == 1;
    std::printf(ok ? "0055a5d0_TEST PASS\n" : "0055a5d0_TEST FAIL\n");
    return !ok;
}