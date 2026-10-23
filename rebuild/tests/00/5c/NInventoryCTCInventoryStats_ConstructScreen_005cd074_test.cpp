// Behaviour test for NInventory::CTCInventoryStats::ConstructScreen @ 0x005CD074: action wiring
#include <cstdio>
struct Action { long type; long params[2]; int count; };
struct Comp { int selected; int back; long lastBackState; Comp() : selected(0), back(0), lastBackState(-1) {} };
static void Build(Comp* top, Comp* personality, Comp* experience, Action& action, Action& backAction)
{
    action.type = 3; action.count = 0;
    action.params[action.count++] = (long)(top != 0);
    action.params[action.count++] = 0;
    if (top) {
        backAction.type = 3; backAction.count = 0;
        backAction.params[backAction.count++] = 1;
        backAction.params[backAction.count++] = 1;
        if (personality) { personality->back++; personality->lastBackState = backAction.params[1]; experience->selected++; }
        if (experience) { experience->back++; experience->lastBackState = backAction.params[1]; experience->selected++; }
    }
}
int main()
{
    int ok = 1;
    Comp top, p, e;
    Action a, b;
    Build(&top, &p, &e, a, b);
    if (a.type != 3 || a.count != 2 || a.params[1] != 0) ok = 0;
    if (p.back != 1 || p.selected != 0 || e.selected != 2 || e.back != 1 || e.lastBackState != 1) ok = 0;
    Comp p2, e2;
    Build(0, &p2, &e2, a, b);
    if (p2.back != 0 || e2.selected != 0 || a.params[0] != 0) ok = 0;
    if (!ok) { std::printf("005cd074_TEST FAIL\n"); return 1; }
    std::printf("005cd074_TEST PASS\n");
    return 0;
}