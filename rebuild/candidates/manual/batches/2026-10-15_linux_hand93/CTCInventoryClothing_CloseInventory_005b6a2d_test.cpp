#include <cstdio>
#include <cstring>
static char g_seq[32] = "";
struct CThing { bool killed; void Kill(bool immediate) { killed = true; std::strcat(g_seq, immediate ? "k!" : "k"); } };
struct Map { int n; void clear() { n = 0; std::strcat(g_seq, "M"); } };
struct Clothing {
    bool InventoryOpen; Map MorphPrimitiveHandles; CThing* PMannequin; bool UseMannequin; int entries;
    void ClearClothingMenuEntries() { entries = 0; std::strcat(g_seq, "C"); }
    void CloseInventory()
    {
        MorphPrimitiveHandles.clear();
        UseMannequin = false;
        if (PMannequin) { PMannequin->Kill(false); PMannequin = 0; }
        InventoryOpen = false;
        ClearClothingMenuEntries();
    }
};
int main()
{
    CThing m; m.killed = false;
    Clothing c; c.InventoryOpen = true; c.MorphPrimitiveHandles.n = 3; c.PMannequin = &m; c.UseMannequin = true; c.entries = 4;
    c.CloseInventory();
    if (std::strcmp(g_seq, "MkC") || !m.killed || c.PMannequin || c.UseMannequin || c.InventoryOpen || c.entries || c.MorphPrimitiveHandles.n) { std::printf("005b6a2d_TEST FAIL\n"); return 1; }
    g_seq[0] = 0; c.InventoryOpen = true; c.CloseInventory();
    if (std::strcmp(g_seq, "MC") || c.InventoryOpen) { std::printf("005b6a2d_TEST FAIL\n"); return 1; }
    std::printf("005b6a2d_TEST PASS\n");
    return 0;
}
