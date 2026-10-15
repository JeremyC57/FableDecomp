#include <cstdio>
#include <cstring>
static char g_seq[32] = "";
struct Base {
    long InventorySelectionState; bool willSelect;
    virtual void AddCloseInventoryEvent() { std::strcat(g_seq, "E"); }
    void SelectHighlightedItem(bool& selected) { std::strcat(g_seq, selected ? "x" : "S"); selected = willSelect; }
    void ProcessButtonAReleased() { std::strcat(g_seq, "B"); }
};
struct CTCInventoryWeapons : Base {
    void ProcessButtonAReleased()
    {
        if (InventorySelectionState == 1) {
            bool selected = false;
            SelectHighlightedItem(selected);
            if (selected)
                AddCloseInventoryEvent();
        }
        Base::ProcessButtonAReleased();
    }
};
int main()
{
    CTCInventoryWeapons s; s.InventorySelectionState = 0; s.willSelect = true; s.ProcessButtonAReleased();
    if (std::strcmp(g_seq, "B")) { std::printf("005c2e2a_TEST FAIL\n"); return 1; }
    g_seq[0] = 0; s.InventorySelectionState = 1; s.willSelect = false; s.ProcessButtonAReleased();
    if (std::strcmp(g_seq, "SB")) { std::printf("005c2e2a_TEST FAIL\n"); return 1; }
    g_seq[0] = 0; s.willSelect = true; s.ProcessButtonAReleased();
    if (std::strcmp(g_seq, "SEB")) { std::printf("005c2e2a_TEST FAIL\n"); return 1; }
    std::printf("005c2e2a_TEST PASS\n");
    return 0;
}