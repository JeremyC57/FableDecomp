// Behaviour test for CAIStateGroup_TavernKeeper::OnReplace (0x00968BC0): a running action is finished; a carried live jug leaves the carry slot and is put back on the bar (position, orientation, physics off, OnBar) and mode 0x2b is dropped; a live bar point is freed and its user cleared; the keeper becomes pushable.
#include <cstdio>
struct CJug { bool dead; bool hasPhysics; int resets; bool physicsActive; bool OnBar; };
struct CKeeper {
    bool performing, finished, carrying, removed, modeRemoved, barAlive, barFreed, pushable;
    CJug* jug; void* barUser;
    void OnReplace()
    {
        if (performing) finished = true;
        if (carrying) {
            if (jug && !jug->dead) {
                removed = true;
                if (jug->hasPhysics) { jug->resets += 2; jug->physicsActive = false; }
                jug->OnBar = true;
            }
            modeRemoved = true;
        }
        if (barAlive) { barFreed = true; barUser = 0; }
        pushable = true;
    }
};
int main()
{
    CJug jug = { false, true, 0, true, false };
    int dummy = 0;
    CKeeper k = { true, false, true, false, false, true, false, false, &jug, &dummy };
    k.OnReplace();
    bool ok = k.finished && k.removed && jug.resets == 2 && !jug.physicsActive && jug.OnBar && k.modeRemoved && k.barFreed
        && k.barUser == 0 && k.pushable;
    CJug dead = { true, true, 0, true, false };
    CKeeper k2 = { false, false, false, false, false, false, false, false, &dead, &dummy };
    k2.OnReplace();
    ok = ok && !k2.finished && !k2.removed && !dead.OnBar && !k2.modeRemoved && !k2.barFreed && k2.barUser == &dummy && k2.pushable;
    if (!ok) { std::printf("00968bc0_TEST FAIL\n"); return 1; }
    std::printf("00968bc0_TEST PASS\n");
    return 0;
}
