// behaviour test for 0x0065EA1B CGetBestTaskForThing<CVillageTaskPickCrops >::operator(): an invalid task is skipped, a valid one replaces the best only when it is better for the thing
#include <cstdio>
struct Thing { int id; };
struct Task {
    bool valid; int score; const Thing* lastThing;
    bool IsValid() const { return valid; }
    bool IsBetterThan(const Task* other, const Thing& thing) const { const_cast<Task*>(this)->lastThing = &thing; return other == 0 || score > other->score; }
};
struct Functor {
    const Thing* thing; Task* best;
    void Call(Task* task)                                   // the function under test
    {
        if (task->IsValid() && task->IsBetterThan(best, *thing))
            best = task;
    }
};
int main()
{
    int fail = 0;
    Thing t; t.id = 1;
    Task a; a.valid = true; a.score = 5; a.lastThing = 0;
    Task b; b.valid = false; b.score = 9; b.lastThing = 0;
    Task c; c.valid = true; c.score = 3; c.lastThing = 0;
    Task d; d.valid = true; d.score = 8; d.lastThing = 0;
    Functor f; f.thing = &t; f.best = 0;
    f.Call(&a); if (f.best != &a || a.lastThing != &t) fail |= 1;
    f.Call(&b); if (f.best != &a || b.lastThing != 0) fail |= 2;
    f.Call(&c); if (f.best != &a) fail |= 4;
    f.Call(&d); if (f.best != &d) fail |= 8;
    if (fail) { std::printf("0065ea1b_TEST FAIL %d\n", fail); return 1; }
    std::printf("0065ea1b_TEST PASS\n");
    return 0;
}