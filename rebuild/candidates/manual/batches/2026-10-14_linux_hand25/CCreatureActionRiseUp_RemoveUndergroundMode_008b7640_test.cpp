#include <cstdio>
static unsigned long g_removed = 0; static int g_refresh = 0; static long g_which = -1;
struct Comp; struct Thing { unsigned long bits[9]; struct E { long type; Comp* comp; } entries[2]; unsigned char mapidx; template<class T> bool GetInterface(T** out, long t) { if (bits[t >> 5] & (1UL << (t & 31))) { *out = (T*)entries[0].comp; return true; } return false; } };
struct Ptr { virtual ~Ptr() {} Thing* item; Thing* GetPItem() const { return item; } };
struct Comp { void RemoveMode(unsigned long m) { g_removed = m; } };
struct Disp { void Refresh() { ++g_refresh; } }; static Disp g_d;
struct UI { Disp* GetMiniMapDisplay(long w) { g_which = w; return &g_d; } }; static UI g_ui; static UI* g_pUI = &g_ui;
struct Act { virtual void v00() {} unsigned char pad[4]; Ptr PCreature; void RemoveUndergroundMode() { Comp* m = 0; PCreature.GetPItem()->GetInterface(&m, 0x31); m->RemoveMode(30); g_pUI->GetMiniMapDisplay(0)->Refresh(); } };
int main()
{
    Comp c; Thing t = {}; t.entries[0].comp = &c; t.bits[1] = 1UL << 17; Act a; a.PCreature.item = &t; a.RemoveUndergroundMode();
    if (g_removed != 30 || g_refresh != 1 || g_which != 0) { std::printf("008b7640_TEST FAIL\n"); return 1; }
    std::printf("008b7640_TEST PASS\n");
    return 0;
}
