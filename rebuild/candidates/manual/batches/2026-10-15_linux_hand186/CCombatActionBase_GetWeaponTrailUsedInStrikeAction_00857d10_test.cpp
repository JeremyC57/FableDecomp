#include <cstdio>
struct Trail { int id; };
struct Thing { bool dead; bool hasTrail; Trail* trail; };
struct Event { long start, end, Index; Thing* weapon; };
struct Action {
    long frame; Event* events; int n;
    Event* FindActive() const { for (int i = 0; i < n; ++i) if (frame >= events[i].start && frame <= events[i].end) return &events[i]; return 0; }
    Trail* GetWeaponTrailUsedInStrikeAction(long* idx) const
    {
        Event* e = FindActive();
        if (e) {
            Thing* w = e->weapon;
            if (w && !w->dead) {
                *idx = e->Index;
                if (!w->dead && w->hasTrail) return w->trail;
            }
        }
        return 0;
    }
};
int main()
{
    Trail t = { 7 };
    Thing sword = { false, true, &t }, fist = { false, false, 0 }, dead = { true, true, &t };
    Event ev[3] = { { 0, 10, 1, &sword }, { 11, 20, 2, &fist }, { 21, 30, 3, &dead } };
    Action a = { 5, ev, 3 };
    long idx = -1;
    Trail* r1 = a.GetWeaponTrailUsedInStrikeAction(&idx); long i1 = idx;
    a.frame = 15; idx = -1; Trail* r2 = a.GetWeaponTrailUsedInStrikeAction(&idx); long i2 = idx;
    a.frame = 25; idx = -1; Trail* r3 = a.GetWeaponTrailUsedInStrikeAction(&idx); long i3 = idx;
    a.frame = 99; Trail* r4 = a.GetWeaponTrailUsedInStrikeAction(&idx);
    if (r1 != &t || i1 != 1 || r2 != 0 || i2 != 2 || r3 != 0 || i3 != -1 || r4 != 0) { std::printf("00857d10_TEST FAIL\n"); return 1; }
    std::printf("00857d10_TEST PASS\n");
    return 0;
}
