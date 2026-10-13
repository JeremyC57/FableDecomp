#include <cstdio>

struct CThing { unsigned char pad[0x8c]; unsigned short DefIndex; };
static CThing* g_t = 0; static int g_slot = -1;
struct CItem { long a, b; unsigned char rest[12]; void UpdateItemStats(CThing* t) { g_t = t; } };
struct Cat { unsigned char pad[8]; CItem* First;
    long GetSlotWithItemsOfType(long ty) { return ty == 7 ? 2 : -1; }
    void Update(CThing* t) { long s = GetSlotWithItemsOfType(*(unsigned short*)((char*)t + 0x8C)); if (s != -1) { CItem* items = First; items[s].UpdateItemStats(t); g_slot = s; } } };
int main()
{
    CItem items[3]; Cat c; c.First = items; CThing t; t.DefIndex = 9; c.Update(&t); if (g_t) { std::printf("005bf16e_TEST FAIL\n"); return 1; }
    t.DefIndex = 7; c.Update(&t); if (g_t != &t || g_slot != 2) { std::printf("005bf16e_TEST FAIL\n"); return 1; }
    std::printf("005bf16e_TEST PASS\n");
    return 0;
}
