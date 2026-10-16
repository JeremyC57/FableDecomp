#include <cstdio>
// Behaviour model of CTCHeroRangedAttacks::OnCreate (0x00789C20).
struct Def { long refs; };
struct Thing { Def* combat; };
struct Ranged { Thing* parent; Thing* creature; Def* combat;
    void OnCreate() { creature = parent; Def* tmp = parent->combat; ++tmp->refs; if (combat != tmp) { combat = tmp; ++tmp->refs; } --tmp->refs; } };
int main()
{
    Def d = { 1 }; Thing t = { &d };
    Ranged r; r.parent = &t; r.creature = 0; r.combat = 0;
    r.OnCreate();
    if (r.creature != &t || r.combat != &d || d.refs != 2) { std::printf("00789c20_TEST FAIL\n"); return 1; }
    r.OnCreate();
    if (d.refs != 2) { std::printf("00789c20_TEST FAIL\n"); return 1; }
    std::printf("00789c20_TEST PASS\n");
    return 0;
}