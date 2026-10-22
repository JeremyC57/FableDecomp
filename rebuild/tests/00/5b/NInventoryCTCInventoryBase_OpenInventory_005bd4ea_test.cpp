// Behaviour test for CTCInventoryBase::OpenInventory @ 0x005BD4EA (self-contained model of the candidate logic).
#include <stdio.h>
#include <string.h>
static int g_time = 0;
static double GFGetTime() { return 1234.5; }
struct CDef { int refs; int destroyed; };
struct CPresentationIntervalDesc { bool PresentImmediate; bool PresentImmediateIfSyncMissed; long VSyncCount; };
struct CDisplayDevice { CPresentationIntervalDesc PresentationInterval; };
static CDisplayDevice g_device;
static int g_guiClosed = -1;
struct CCategory { bool selected; void SetAsSelected(bool s) { selected = s; } };
struct Model {
    bool FirstRenderFrame; long MenuArrowHighlightsCategoryUp, MenuArrowHighlightsCategoryDn, MenuArrowHighlightsItemUp, MenuArrowHighlightsItemDn;
    double LastRenderTime; bool InventoryOpen; CPresentationIntervalDesc OldIntervalDesc; bool OldIntervalDescInitialised;
    CDef* PDef; long SelectedCategoryIndex; long RHSetInvItemDefIndex;
    CCategory cats[3]; int ncats; long highlighted; long selectedItem; long resetItem;
    long GetHighlightedItem() { return highlighted; }
    void ResetInventoryAnim(long item) { resetItem = item; }
    void SelectItem(long item) { selectedItem = item; }
    void OpenInventory()
    {
        LastRenderTime = GFGetTime();
        FirstRenderFrame = true;
        CDef* inventoryDef = PDef; if (inventoryDef) ++inventoryDef->refs;
        if (!OldIntervalDescInitialised) { OldIntervalDesc = g_device.PresentationInterval; OldIntervalDescInitialised = true; }
        InventoryOpen = true;
        g_guiClosed = 1;
        SelectItem(0);
        if (ncats != 0) cats[SelectedCategoryIndex].SetAsSelected(true);
        long item = GetHighlightedItem();
        if (item > 0 && RHSetInvItemDefIndex == 0) ResetInventoryAnim(item);
        MenuArrowHighlightsCategoryUp = 1; MenuArrowHighlightsCategoryDn = 1; MenuArrowHighlightsItemUp = 1; MenuArrowHighlightsItemDn = 1;
        if (inventoryDef && --inventoryDef->refs == 0) inventoryDef->destroyed = 1;
    }
};
int main()
{
    bool ok = true;
    g_device.PresentationInterval.PresentImmediate = true; g_device.PresentationInterval.VSyncCount = 3;
    CDef def = { 1, 0 };
    Model m; memset(&m, 0, sizeof m);
    m.PDef = &def; m.ncats = 3; m.SelectedCategoryIndex = 2; m.highlighted = 7; m.selectedItem = 99;
    m.OpenInventory();
    ok = ok && m.LastRenderTime == 1234.5 && m.FirstRenderFrame && m.InventoryOpen && g_guiClosed == 1;
    ok = ok && m.OldIntervalDescInitialised && m.OldIntervalDesc.PresentImmediate && m.OldIntervalDesc.VSyncCount == 3;
    ok = ok && m.selectedItem == 0 && m.cats[2].selected && !m.cats[0].selected && m.resetItem == 7;
    ok = ok && m.MenuArrowHighlightsCategoryUp == 1 && m.MenuArrowHighlightsItemDn == 1;
    ok = ok && def.refs == 1 && def.destroyed == 0;          // addref/release balanced, def still owned by the member
    // second open: the interval is not re-read, no categories, highlighted item with an RH set in place -> no anim reset
    g_device.PresentationInterval.VSyncCount = 9; m.ncats = 0; m.resetItem = 0; m.RHSetInvItemDefIndex = 5;
    m.OpenInventory();
    ok = ok && m.OldIntervalDesc.VSyncCount == 3 && m.resetItem == 0;
    // a null definition is tolerated; a non-positive highlighted item never resets
    m.PDef = 0; m.RHSetInvItemDefIndex = 0; m.highlighted = 0;
    m.OpenInventory();
    ok = ok && m.resetItem == 0;
    if (!ok) { printf("005bd4ea_TEST FAIL\n"); return 1; }
    printf("005bd4ea_TEST PASS\n"); return 0;
}