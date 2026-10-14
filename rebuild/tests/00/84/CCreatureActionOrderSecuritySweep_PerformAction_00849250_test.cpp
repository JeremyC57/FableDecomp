#include <cstdio>
static long g_recalc = -1; static void* g_vil = 0;
struct Comp; struct Thing { unsigned long bits[9]; struct E { long type; Comp* comp; } entries[2]; unsigned char mapidx; template<class T> bool GetInterface(T** out, long t) { if (bits[t >> 5] & (1UL << (t & 31))) { *out = (T*)entries[0].comp; return true; } return false; } };
struct Ptr { virtual ~Ptr() {} Thing* item; Thing* GetPItem() const { return item; } };
struct Comp { int x; };
struct Village { void RecalculateSecuritySweep(long f) { g_recalc = f; } };
static Village g_v; static bool g_have = true;
static Village* GFGetPVillageInterface(Comp* c) { g_vil = c; return g_have ? &g_v : 0; }
struct Act { virtual void PerformAction() { Comp* v = 0; PCreature.GetPItem()->GetInterface(&v, 0x23); Village* vl = GFGetPVillageInterface(v); if (vl) vl->RecalculateSecuritySweep(0); } unsigned char pad[4]; Ptr PCreature; };
int main()
{
    Comp c; Thing t = {}; t.entries[0].comp = &c; t.bits[1] = 8; Act a; a.PCreature.item = &t; a.PerformAction(); long r1 = g_recalc; g_have = false; g_recalc = -1; a.PerformAction();
    if (r1 != 0 || g_vil != &c || g_recalc != -1) { std::printf("00849250_TEST FAIL\n"); return 1; }
    std::printf("00849250_TEST PASS\n");
    return 0;
}