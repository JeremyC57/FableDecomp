#include <cstdio>
#include <cstring>
static char g_seq[32] = "";
struct Inv {
    long InventorySelectionState;
    virtual void OnSelectedCategoryChanged() { std::strcat(g_seq, "c"); }
    virtual void OnHighlightedItemChanged() { std::strcat(g_seq, "h"); }
    void SelectNextCategory() { std::strcat(g_seq, "S"); }
    void HighlightNextItem() { std::strcat(g_seq, "H"); }
    void ProcessInventoryInputDown()
    {
        switch (InventorySelectionState) {
        case 0: SelectNextCategory(); OnSelectedCategoryChanged(); break;
        case 1: HighlightNextItem(); OnHighlightedItemChanged(); break;
        }
    }
};
int main()
{
    Inv inv;
    inv.InventorySelectionState = 0; inv.ProcessInventoryInputDown();
    inv.InventorySelectionState = 1; inv.ProcessInventoryInputDown();
    inv.InventorySelectionState = 2; inv.ProcessInventoryInputDown();
    if (std::strcmp(g_seq, "ScHh")) { std::printf("005bddb8_TEST FAIL\n"); return 1; }
    std::printf("005bddb8_TEST PASS\n");
    return 0;
}