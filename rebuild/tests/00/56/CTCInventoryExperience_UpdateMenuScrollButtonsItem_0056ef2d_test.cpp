// Behaviour test for 0056ef2d: the body of src/0056ef2d.cpp over a minimal model of the members it reads/writes
// (InventorySelectionState +0x1C, the four MenuArrowHighlights* arrows, statIndicatorMax, the displayed stat index vector).
#include <cstdio>
#include <vector>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
enum EInventorySelectionState { INVENTORY_SELECTION_STATE_CATEGORIES = 0, INVENTORY_SELECTION_STATE_ITEMS = 1 };
enum ESelectionState { SELECTION_STATE_NONE = 0, SELECTION_STATE_OFF = 1, SELECTION_STATE_ON = 2, SELECTION_STATE_SEL = 3 };
class CTCInventoryExperience { public:
    EInventorySelectionState InventorySelectionState;
    ESelectionState MenuArrowHighlightsCategoryUp, MenuArrowHighlightsCategoryDn;
    ESelectionState MenuArrowHighlightsItemUp, MenuArrowHighlightsItemDn;
    long statIndicatorMax;
    std::vector<long> MenuDisplayListExperienceItem;
    void UpdateMenuScrollButtonsItem();
};
// ---- source body (verbatim) ----
void CTCInventoryExperience::UpdateMenuScrollButtonsItem()
{
    if (InventorySelectionState == INVENTORY_SELECTION_STATE_ITEMS) {
        long statCount = statIndicatorMax;
        MenuArrowHighlightsCategoryUp = SELECTION_STATE_OFF;
        MenuArrowHighlightsCategoryDn = SELECTION_STATE_OFF;
        if (MenuDisplayListExperienceItem[0] > 0) MenuArrowHighlightsItemUp = SELECTION_STATE_ON;
        else MenuArrowHighlightsItemUp = SELECTION_STATE_OFF;
        if (MenuDisplayListExperienceItem[MenuDisplayListExperienceItem.size() - 1] < statCount - 1) MenuArrowHighlightsItemDn = SELECTION_STATE_ON;
        else MenuArrowHighlightsItemDn = SELECTION_STATE_OFF;
    }
}
static void reset(CTCInventoryExperience& e, EInventorySelectionState s, long first, long last, long max)
{
    e.InventorySelectionState = s;
    e.MenuArrowHighlightsCategoryUp = e.MenuArrowHighlightsCategoryDn = SELECTION_STATE_SEL;
    e.MenuArrowHighlightsItemUp = e.MenuArrowHighlightsItemDn = SELECTION_STATE_NONE;
    e.statIndicatorMax = max;
    e.MenuDisplayListExperienceItem.clear();
    for (long i = first; i <= last; ++i) e.MenuDisplayListExperienceItem.push_back(i);
}
int main()
{
    CTCInventoryExperience e;
    // category state: nothing changes
    reset(e, INVENTORY_SELECTION_STATE_CATEGORIES, 0, 3, 10); e.UpdateMenuScrollButtonsItem();
    CHECK(e.MenuArrowHighlightsCategoryUp == SELECTION_STATE_SEL && e.MenuArrowHighlightsItemUp == SELECTION_STATE_NONE);
    CHECK(e.MenuArrowHighlightsItemDn == SELECTION_STATE_NONE);
    // top of the list: up off, down on
    reset(e, INVENTORY_SELECTION_STATE_ITEMS, 0, 3, 10); e.UpdateMenuScrollButtonsItem();
    CHECK(e.MenuArrowHighlightsCategoryUp == SELECTION_STATE_OFF && e.MenuArrowHighlightsCategoryDn == SELECTION_STATE_OFF);
    CHECK(e.MenuArrowHighlightsItemUp == SELECTION_STATE_OFF && e.MenuArrowHighlightsItemDn == SELECTION_STATE_ON);
    // middle: both on
    reset(e, INVENTORY_SELECTION_STATE_ITEMS, 2, 5, 10); e.UpdateMenuScrollButtonsItem();
    CHECK(e.MenuArrowHighlightsItemUp == SELECTION_STATE_ON && e.MenuArrowHighlightsItemDn == SELECTION_STATE_ON);
    // bottom: last index == max - 1 -> down off
    reset(e, INVENTORY_SELECTION_STATE_ITEMS, 6, 9, 10); e.UpdateMenuScrollButtonsItem();
    CHECK(e.MenuArrowHighlightsItemUp == SELECTION_STATE_ON && e.MenuArrowHighlightsItemDn == SELECTION_STATE_OFF);
    // last index == max - 2 -> down still on
    reset(e, INVENTORY_SELECTION_STATE_ITEMS, 5, 8, 10); e.UpdateMenuScrollButtonsItem();
    CHECK(e.MenuArrowHighlightsItemDn == SELECTION_STATE_ON);
    // whole list fits: both off
    reset(e, INVENTORY_SELECTION_STATE_ITEMS, 0, 2, 3); e.UpdateMenuScrollButtonsItem();
    CHECK(e.MenuArrowHighlightsItemUp == SELECTION_STATE_OFF && e.MenuArrowHighlightsItemDn == SELECTION_STATE_OFF);
    if (fails) { printf("%d failures\n", fails); return 1; }
    printf("0056ef2d_TEST PASS\n");
    return 0;
}