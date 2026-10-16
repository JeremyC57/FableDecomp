// behaviour test for 0x0074E460 CTCContainerRewardHero::GetDynamicResurrectionDefIndex
#include <cstdio>
#include <cstring>
struct Inventory { long phials; long GetNumberOfItemsOfTypeInInventory(long idx) { return idx == 42 ? phials : 0; } };
struct CThing {
    unsigned int bits[9]; Inventory* inv; unsigned char flags91;
    unsigned int HasInterface(long t) const { return bits[t >> 5] & (1u << (t & 31)); }
    Inventory* GetInterface(long) { return inv; }
};
static long DefIndexFromName(const char* n) { return std::strcmp(n, "OBJECT_RESURRECTION_PHIAL") == 0 ? 42 : -1; }
struct Reward {
    CThing* hero;
    long GetDynamicResurrectionDefIndex()
    {
        CThing* h = hero;
        if (h && !(h->flags91 & 1) && h->HasInterface(0x11)) {
            Inventory* inventory = h->GetInterface(0x11);
            long index = DefIndexFromName("OBJECT_RESURRECTION_PHIAL");
            if (inventory->GetNumberOfItemsOfTypeInInventory(index) == 0) return index;
        }
        return 0;
    }
};
int main()
{
    Inventory inv = { 0 };
    CThing h; for (int i = 0; i < 9; ++i) h.bits[i] = 0; h.inv = &inv; h.flags91 = 0;
    Reward r; r.hero = 0;
    bool ok = r.GetDynamicResurrectionDefIndex() == 0;
    r.hero = &h;
    ok = ok && r.GetDynamicResurrectionDefIndex() == 0;          // no inventory interface
    h.bits[0] = 0x20000;
    ok = ok && r.GetDynamicResurrectionDefIndex() == 42;
    inv.phials = 1;
    ok = ok && r.GetDynamicResurrectionDefIndex() == 0;
    inv.phials = 0; h.flags91 = 1;
    ok = ok && r.GetDynamicResurrectionDefIndex() == 0;
    if (!ok) { std::printf("0074e460_TEST FAIL\n"); return 1; }
    std::printf("0074e460_TEST PASS\n");
    return 0;
}
