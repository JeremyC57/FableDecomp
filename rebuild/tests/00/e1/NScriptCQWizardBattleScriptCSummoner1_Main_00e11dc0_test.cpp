#include <cstdio>
// Behaviour model of CSummoner1::Main's watch loop: frames until terminating; aware -> feed-out then attack; hit -> attack.
struct World { int frame; int awareAt; int hitAt; int termAt; int frames; bool fedOut; bool attacked; };
static bool Term(World& w) { return w.termAt >= 0 && w.frame >= w.termAt; }
static void Run(World& w)
{
    while (!Term(w)) {
        if (w.awareAt >= 0 && w.frame >= w.awareAt) {
            if (Term(w)) return;
            w.fedOut = true;
            goto attack;
        }
        if (w.hitAt >= 0 && w.frame >= w.hitAt)
            goto attack;
        ++w.frame; ++w.frames;
    }
    return;
attack:
    if (Term(w)) return;
    w.attacked = true;
}
int main()
{
    World a = { 0, 3, -1, -1, 0, false, false }; Run(a);
    World b = { 0, -1, 2, -1, 0, false, false }; Run(b);
    World c = { 0, -1, -1, 4, 0, false, false }; Run(c);
    if (!a.fedOut || !a.attacked || a.frames != 3 || b.fedOut || !b.attacked || b.frames != 2 || c.attacked || c.frames != 4)
    { std::printf("00e11dc0_TEST FAIL\n"); return 1; }
    std::printf("00e11dc0_TEST PASS\n");
    return 0;
}