#include <cstdio>

struct CThing { int x; };
static CThing g_a, g_b; static unsigned __int64 g_uid = 0;
struct Inv { void* vt; CThing* Owner; };
struct Entry { CThing* GetThing() { return &g_b; } };
static Entry g_entry;
Entry* GFFindThingMapEntryByUID(unsigned __int64 u) { g_uid = u; return &g_entry; }
struct Item { unsigned char pad[0x10]; Inv* PInventory; unsigned char p2[4]; unsigned __int64 UID;
    CThing* Get() { Inv* i = PInventory; if (i) return i->Owner; if (UID == 0) return 0; return GFFindThingMapEntryByUID(UID)->GetThing(); } };
int main()
{
    Inv inv; inv.Owner = &g_a; Item it; it.PInventory = &inv; it.UID = 0; if (it.Get() != &g_a) { std::printf("005d8e90_TEST FAIL\n"); return 1; }
    it.PInventory = 0; if (it.Get() != 0) { std::printf("005d8e90_TEST FAIL\n"); return 1; }
    it.UID = 0x100000005ULL; if (it.Get() != &g_b || g_uid != 0x100000005ULL) { std::printf("005d8e90_TEST FAIL\n"); return 1; }
    std::printf("005d8e90_TEST PASS\n");
    return 0;
}
