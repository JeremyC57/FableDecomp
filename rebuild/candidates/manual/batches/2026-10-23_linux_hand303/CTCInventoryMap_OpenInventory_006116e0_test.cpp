// Behaviour test for CTCInventoryMap::OpenInventory @ 0x006116E0: closes when the map menu cannot be built, else keeps pointer + screen
#include <cstdio>
struct State { long previous; int components; int closed; int fades; int screenState; };
static void Open(State& s, int active, int soundSystem, int buildOk, long currentScreen)
{
    s.previous = -1;
    if (!active) s.previous = currentScreen;
    if (active) return;
    if (soundSystem) s.fades += 3;
    if (buildOk) { s.components += 2; s.screenState = 5; }
    else s.closed = 1;
}
int main()
{
    int ok = 1;
    State a = { 0, 0, 0, 0, 0 };
    Open(a, 0, 1, 1, 7);
    if (a.previous != 7 || a.components != 2 || a.closed || a.fades != 3 || a.screenState != 5) ok = 0;
    State b = { 0, 0, 0, 0, 0 };
    Open(b, 0, 0, 0, 3);
    if (b.components != 0 || !b.closed || b.fades != 0) ok = 0;
    State c = { 0, 0, 0, 0, 0 };
    Open(c, 1, 1, 1, 9);
    if (c.previous != -1 || c.components != 0 || c.fades != 0) ok = 0;
    if (!ok) { std::printf("006116e0_TEST FAIL\n"); return 1; }
    std::printf("006116e0_TEST PASS\n");
    return 0;
}
