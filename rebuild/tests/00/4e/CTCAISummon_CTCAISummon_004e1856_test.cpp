// behaviour test for 0x004E1856 CTCAISummon::~CTCAISummon() (model: members destroyed in reverse order, the def released, then the base)
#include <cstdio>
static int g_order = 0, g_member = 0, g_def = 0, g_base = 0;
struct Def { long rc; Def() : rc(1) { } void Destroy() { g_def = ++g_order; } };
struct DefPtr { Def* o; ~DefPtr() { if (o) { if (--o->rc == 0) o->Destroy(); o = 0; } } };
struct Member { ~Member() { g_member = ++g_order; } };
struct Base { virtual ~Base() { g_base = ++g_order; } };
struct C : Base { DefPtr pdef; Member m; virtual ~C() { } };
int main()
{
    Def d;
    { C c; c.pdef.o = &d; }
    int ok = g_member == 1 && g_def == 2 && g_base == 3;
    if (!ok) { std::printf("004e1856_TEST FAIL\n"); return 1; }
    std::printf("004e1856_TEST PASS\n");
    return 0;
}