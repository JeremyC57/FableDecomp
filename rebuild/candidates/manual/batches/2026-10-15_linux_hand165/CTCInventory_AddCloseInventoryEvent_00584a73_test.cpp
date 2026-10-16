#include <cstdio>
// Behaviour model of CTCInventory::AddCloseInventoryEvent (0x00584A73): an active, not closing menu is asked to close,
// otherwise the base queues the close event; afterwards the live GUI is toggled when live mode is on.
struct Menu { bool active, closing; int closeRequests; };
struct State { Menu menu; int baseEvents; bool liveGui; int toggles; };
static void AddClose(State& s)
{
    if (s.menu.active && !s.menu.closing) ++s.menu.closeRequests;
    else ++s.baseEvents;
    if (s.liveGui) { ++s.toggles; s.liveGui = false; }
}
int main()
{
    State s = { { true, false, 0 }, 0, true, 0 };
    AddClose(s);
    if (s.menu.closeRequests != 1 || s.baseEvents || s.toggles != 1) { std::printf("00584a73_TEST FAIL\n"); return 1; }
    s.menu.closing = true; AddClose(s);
    if (s.menu.closeRequests != 1 || s.baseEvents != 1 || s.toggles != 1) { std::printf("00584a73_TEST FAIL\n"); return 1; }
    std::printf("00584a73_TEST PASS\n");
    return 0;
}
