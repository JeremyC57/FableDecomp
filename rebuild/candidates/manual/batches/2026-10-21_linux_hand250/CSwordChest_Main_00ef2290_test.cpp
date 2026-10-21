// Behaviour test for CSwordChest::Main (0x00EF2290): wait for the hero to use the chest, then hand over the sword.
#include <cstdio>
struct Thing { bool alive; bool carriedByHero; };
struct Chest {
    int frames, usedAt, doneAfter; Thing* sword; int carryCalls;
    bool IsDone() { return frames >= doneAfter; }
    void NewScriptFrame() { ++frames; }
    bool MsgIsUsedBy() { return frames >= usedAt; }
    bool IsAlive(Thing* t) { return t && t->alive; }
    bool IsBeingCarriedBy(Thing* t) { return t && t->carriedByHero; }
    void Main()
    {
        NewScriptFrame();
        if (IsDone()) return;
        while (!MsgIsUsedBy()) { NewScriptFrame(); if (IsDone()) return; }
        if (IsDone()) return;
        Thing* s = sword;
        if (IsAlive(s) && !IsBeingCarriedBy(s)) ++carryCalls;
    }
};
int main()
{
    Thing sw = { true, false };
    Chest a = { 0, 5, 100, &sw, 0 }; a.Main();
    Thing carried = { true, true };
    Chest b = { 0, 5, 100, &carried, 0 }; b.Main();
    Chest c = { 0, 5, 100, 0, 0 }; c.Main();
    Chest d = { 0, 50, 10, &sw, 0 }; d.Main();
    if (a.carryCalls != 1 || a.frames != 5 || b.carryCalls != 0 || c.carryCalls != 0 || d.carryCalls != 0 || d.frames != 10)
    { std::printf("00ef2290_TEST FAIL\n"); return 1; }
    std::printf("00ef2290_TEST PASS\n");
    return 0;
}
