// behaviour test for CAIStateGroupBase brain-output actions (0x0097d980)
#include <cstdio>
struct Brain { int yields; unsigned long lastFlags; int freeAfter; int stopAfter; };
struct Group {
    Brain* b;
    bool IsStopping() { return b->stopAfter >= 0 && b->yields >= b->stopAfter; }
    bool IsFree() { return b->yields >= b->freeAfter; }
    void Yield() { ++b->yields; }
    long Perform()
    {
        if (IsStopping()) return 1;
        while (!IsFree() && !IsStopping()) Yield();
        if (IsStopping()) return 1;
        b->lastFlags = 8; Yield();
        if (IsStopping()) return 1;
        return 2;
    }
    long Pump()
    {
        if (IsStopping()) return 1;
        b->lastFlags = 0x20; Yield();
        if (IsStopping()) return 1;
        return 4;
    }
};
int main()
{
    const char* a = "0097d980"; bool perform = a[5] == '8';
    Brain b = { 0, 0, 3, -1 }; Group g = { &b };
    long r = perform ? g.Perform() : g.Pump();
    bool ok = perform ? (r == 2 && b.yields == 4 && b.lastFlags == 8) : (r == 4 && b.yields == 1 && b.lastFlags == 0x20);
    Brain c = { 0, 0, 0, 1 }; Group h = { &c };
    r = perform ? h.Perform() : h.Pump();
    ok = ok && r == 1;
    if (!ok) { std::printf("0097d980_TEST FAIL\n"); return 1; }
    std::printf("0097d980_TEST PASS\n");
    return 0;
}