// Behaviour test for CAIStateGroup_MinionBehaviourCrowdBase::Update (0x00957DB0), g++ model of the parked source.
#include <cstdio>
struct Perceive { float awareness; bool direct; bool lineOfSight; };
struct Group { Perceive* p; int moveResults[8]; int nMove; int lookResults[8]; int nLook; int moves, looks, terminated, reachedHook; };
static int Move(Group& g) { return g.moveResults[g.moves++]; }
static int Look(Group& g) { return g.lookResults[g.looks++]; }
static void Update(Group& g)
{
    int result;
    do {
        result = Move(g);
        if (g.p->awareness == 0.0f || !g.p->direct || !g.p->lineOfSight) break;
        if (result == 2) {
            do { result = Look(g); } while (result == 4);
            if (result == 2) ++g.reachedHook;
            return;
        }
    } while (result == 4);
    ++g.terminated;
}
int main()
{
    Perceive p = { 1.0f, true, true };
    Group g = { &p, { 4, 4, 2 }, 3, { 4, 2 }, 2, 0, 0, 0, 0 };
    Update(g);
    int ok = g.moves == 3 && g.looks == 2 && g.reachedHook == 1 && g.terminated == 0;
    Group h = { &p, { 4, 3 }, 2, { 0 }, 0, 0, 0, 0, 0 }; Update(h); ok &= h.terminated == 1 && h.moves == 2 && h.looks == 0;
    Perceive q = { 0.0f, true, true };
    Group k = { &q, { 2 }, 1, { 2 }, 1, 0, 0, 0, 0 }; Update(k); ok &= k.terminated == 1 && k.looks == 0;
    Group m = { &p, { 2 }, 1, { 3 }, 1, 0, 0, 0, 0 }; Update(m); ok &= m.terminated == 0 && m.reachedHook == 0 && m.looks == 1;
    std::printf(ok ? "00957db0_TEST PASS\n" : "00957db0_TEST FAIL\n");
    return ok ? 0 : 1;
}
