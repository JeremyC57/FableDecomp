#include <cstdio>
#include <cstring>
static char g_seq[32] = "";
struct Inv {
    long InventorySelectionState;
    virtual void OnSelectedCategoryChanged() { std::strcat(g_seq, "c"); }
    virtual void OnHighlightedItemChanged() { std::strcat(g_seq, "h"); }
    void SelectPreviousCategory() { std::strcat(g_seq, "S"); }
    void HighlightPreviousItem() { std::strcat(g_seq, "H"); }
    void ProcessInventoryInputUp()
    {
        switch (InventorySelectionState) {
        case 0: SelectPreviousCategory(); OnSelectedCategoryChanged(); break;
        case 1: HighlightPreviousItem(); OnHighlightedItemChanged(); break;
        }
    }
};
int main()
{
    Inv inv;
    inv.InventorySelectionState = 0; inv.ProcessInventoryInputUp();
    inv.InventorySelectionState = 1; inv.ProcessInventoryInputUp();
    inv.InventorySelectionState = 2; inv.ProcessInventoryInputUp();
    if (std::strcmp(g_seq, "ScHh")) { std::printf("005bdd8c_TEST FAIL\n"); return 1; }
    std::printf("005bdd8c_TEST PASS\n");
    return 0;
}
