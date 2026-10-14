#include <cstdio>
static int g_base = 0, g_trig = 0;
struct Comp; struct Thing { unsigned long bits[9]; struct E { long type; Comp* comp; } entries[2]; unsigned char mapidx; template<class T> bool GetInterface(T** out, long t) { if (bits[t >> 5] & (1UL << (t & 31))) { *out = (T*)entries[0].comp; return true; } return false; } };
struct Ptr { virtual ~Ptr() {} Thing* item; Thing* GetPItem() const { return item; } };
struct Comp { unsigned char pad[0xc]; unsigned char flag; void Trigger() { ++g_trig; } };
struct Base { virtual void FinishAction() { ++g_base; } unsigned char pad[4]; Ptr PCreature; };
struct Act : Base { void FinishAction() { Base::FinishAction(); Comp* s; if (PCreature.GetPItem()->GetInterface(&s, 0x95)) { if (!s->flag) s->Trigger(); } } };
int main()
{
    Comp c; c.flag = 0; Thing t = {}; t.entries[0].comp = &c; Act a; a.PCreature.item = &t; a.FinishAction(); int t1 = g_trig; t.bits[0x95 >> 5] = 1UL << (0x95 & 31); a.FinishAction(); int t2 = g_trig; c.flag = 1; a.FinishAction();
    if (g_base != 3 || t1 != 0 || t2 != 1 || g_trig != 1) { std::printf("0069f750_TEST FAIL\n"); return 1; }
    std::printf("0069f750_TEST PASS\n");
    return 0;
}
