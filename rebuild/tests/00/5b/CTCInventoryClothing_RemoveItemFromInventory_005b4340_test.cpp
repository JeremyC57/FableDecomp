// Behaviour test for 0x005B4340 CTCInventoryClothing::RemoveItemFromInventory
#include <cstdio>
static int g_selected = -1, g_dressed = 0; static bool g_flagDuringRemove = true;
static long GetReplacementItemForType(long t) { return t + 100; }
struct CInvBase {
    void* ParentThing; bool Dying; long sel;
    bool IsItemSelected(long t) { return t == sel; }
    void SelectItemWithDefIndex(long d) { g_selected = (int)d; }
};
struct CClothing : CInvBase {
    bool UseMannequin;
    void BaseRemove(long, long) { g_flagDuringRemove = UseMannequin; }
    void SetThingAsWearingSelectedClothes(void*) { ++g_dressed; }
    void RemoveItemFromInventory(long itemType, long count)
    {
        bool useMannequin = UseMannequin;
        UseMannequin = false;
        bool wasSelected = IsItemSelected(itemType);
        BaseRemove(itemType, count);
        if (!Dying) {
            if (wasSelected)
                SelectItemWithDefIndex(GetReplacementItemForType(itemType));
            SetThingAsWearingSelectedClothes(ParentThing);
        }
        UseMannequin = useMannequin;
    }
};
int main()
{
    CClothing c; c.ParentThing = 0; c.Dying = false; c.sel = 4; c.UseMannequin = true;
    c.RemoveItemFromInventory(4, 1);
    bool ok = g_selected == 104 && g_dressed == 1 && !g_flagDuringRemove && c.UseMannequin;
    g_selected = -1; c.RemoveItemFromInventory(5, 1);
    if (g_selected != -1 || g_dressed != 2) ok = false;
    c.Dying = true; c.RemoveItemFromInventory(4, 1);
    if (g_selected != -1 || g_dressed != 2) ok = false;
    if (!ok) { std::printf("005b4340_TEST FAIL\n"); return 1; }
    std::printf("005b4340_TEST PASS\n");
    return 0;
}