// behaviour test for CAIStateGroup_StayNearSchool::Update (0x00916910): logic copy over stubs
#include <cstdio>
struct Thing { unsigned char flags91; };
struct Group {
    Thing* school; bool hours, villager, village, open;
    long results[4]; int moves, terminated; float lastDistance;
    long Move(Thing*, float d) { lastDistance = d; return results[moves++]; }
    void Update()
    {
        if (school && !(school->flags91 & 1) && hours) {
            long r;
            do {
                if (!school || (school->flags91 & 1) || !villager || !village || !open) { terminated++; return; }
                r = Move(school, 15.0f);
            } while (r == 4);
            return;
        }
        terminated++;
    }
};
int main()
{
    Thing s = { 0 };
    Group g = { &s, true, true, true, true, { 4, 4, 2, 0 }, 0, 0, 0.0f };
    g.Update();
    bool ok = g.moves == 3 && g.terminated == 0 && g.lastDistance == 15.0f;
    Group h = { &s, false, true, true, true, { 2, 0, 0, 0 }, 0, 0, 0.0f };
    h.Update();
    if (h.terminated != 1 || h.moves != 0) ok = false;
    Group k = { &s, true, true, true, false, { 2, 0, 0, 0 }, 0, 0, 0.0f };
    k.Update();
    if (k.terminated != 1 || k.moves != 0) ok = false;
    if (!ok) { std::printf("00916910_TEST FAIL\n"); return 1; }
    std::printf("00916910_TEST PASS\n");
    return 0;
}