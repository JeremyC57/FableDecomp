// Behaviour test for CAIStateGroup_GazeAtWork::IsRunnable (0x00918A40): a model of the row -- no workplace -> false; a shop keeper
// whose shop is missing or closed -> false; otherwise runnable exactly when a free gaze position exists.
#include <stdio.h>
struct Shop { bool open; bool IsOpeningHours() const { return open; } };
struct Keeper { Shop* shop; Shop* GetShopTC() const { return shop; } };
struct Group { void* owner; Keeper* keeper; void* freePos;
    bool IsRunnable() {
        if (owner) {
            if (keeper) { Shop* s = keeper->GetShopTC(); if (!s || !s->IsOpeningHours()) return false; }
            return freePos != 0;
        }
        return false;
    }
};
int main()
{
    int x; Shop open = { true }, shut = { false }; Keeper k1 = { &open }, k2 = { &shut }, k3 = { 0 };
    Group g0 = { 0, 0, &x }, g1 = { &x, 0, &x }, g2 = { &x, &k1, &x }, g3 = { &x, &k2, &x }, g4 = { &x, &k3, &x }, g5 = { &x, &k1, 0 };
    int ok = !g0.IsRunnable() && g1.IsRunnable() && g2.IsRunnable() && !g3.IsRunnable() && !g4.IsRunnable() && !g5.IsRunnable();
    printf(ok ? "00918a40_TEST PASS\n" : "00918a40_TEST FAIL\n");
    return ok ? 0 : 1;
}
