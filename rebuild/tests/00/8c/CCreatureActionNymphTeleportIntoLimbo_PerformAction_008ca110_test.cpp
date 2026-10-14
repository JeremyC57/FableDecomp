#include <cstdio>
static long g_fly = -1; static int g_effect = 0;
struct Comp; struct Thing { unsigned long bits[9]; struct E { long type; Comp* comp; } entries[2]; unsigned char mapidx; template<class T> bool GetInterface(T** out, long t) { if (bits[t >> 5] & (1UL << (t & 31))) { *out = (T*)entries[0].comp; return true; } return false; } };
struct Ptr { virtual ~Ptr() {} Thing* item; Thing* GetPItem() const { return item; } };
struct Comp { void StartFlyingAround(long d) { g_fly = d; } };
struct Base { virtual void PerformAction() {} unsigned char pad[4]; Ptr PCreature; long dur; unsigned char pad2[8]; long el; void DoEffect() { ++g_effect; } };
struct Act : Base { void PerformAction() { Comp* n; if (PCreature.GetPItem()->GetInterface(&n, 0xf5)) n->StartFlyingAround(dur - el); DoEffect(); } };
int main()
{
    Comp c; Thing t = {}; t.entries[0].comp = &c; Act a; a.PCreature.item = &t; a.dur = 10; a.el = 3;
    a.PerformAction(); long f1 = g_fly; t.bits[7] = 1UL << 21; a.PerformAction();
    if (f1 != -1 || g_fly != 7 || g_effect != 2) { std::printf("008ca110_TEST FAIL\n"); return 1; }
    std::printf("008ca110_TEST PASS\n");
    return 0;
}