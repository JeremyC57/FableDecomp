#include <cstdio>
struct Thing;
struct Perceive { Thing* perceived; float awareness; };
struct Thing { bool hasTC; Perceive* tc; };
struct F { Thing* PThingToBeAwareOf;
    bool operator()(Thing* t) const {
        if (t->hasTC) { Perceive* p = t->tc; return p->perceived == PThingToBeAwareOf && p->awareness != 0.0f; }
        return false; } };
int main()
{
    Thing target = { false, 0 };
    Perceive p = { &target, 1.0f };
    Thing t = { true, &p };
    F f = { &target };
    bool ok = f(&t);
    p.awareness = 0.0f; ok = ok && !f(&t);
    p.awareness = 2.0f; p.perceived = &t; ok = ok && !f(&t);
    t.hasTC = false; ok = ok && !f(&t);
    if (!ok) { std::printf("006a8630_TEST FAIL\n"); return 1; }
    std::printf("006a8630_TEST PASS\n");
    return 0;
}