#include <cstdio>
static int g_closed = 0, g_hooks = 0;
struct CPlayerGuiCalls { void Close() { ++g_closed; } };
static CPlayerGuiCalls g_gui; CPlayerGuiCalls* g_pPlayerGui = &g_gui;
struct CCategory_Retail { unsigned char _body[0x2C]; void OnInventoryClosed() { ++g_hooks; } };
struct CCategoryVector { CCategory_Retail* first; CCategory_Retail* last; CCategory_Retail* end_of_storage; unsigned long size() const { return (unsigned long)(last - first); } CCategory_Retail& operator[](unsigned long n) { return first[n]; } };
struct CTCInventoryBase_Retail { unsigned char _pad_0x00[0x20]; CCategoryVector Categories; unsigned char _pad_0x2c[0x2C]; bool InventoryOpen; unsigned char _pad_0x59[0xCB]; long InventoryAnimThingDefIndex; long RHSetInvItemDefIndex;
    void OnInventoryClosed() { InventoryOpen = false; g_pPlayerGui->Close(); for (unsigned long i = 0; i < Categories.size(); ++i) Categories[i].OnInventoryClosed(); InventoryAnimThingDefIndex = 0; RHSetInvItemDefIndex = 0; } };

int main()
{
    CCategory_Retail cats[3]; CTCInventoryBase_Retail inv; inv.Categories.first = cats; inv.Categories.last = cats + 3; inv.InventoryOpen = true; inv.InventoryAnimThingDefIndex = 5; inv.RHSetInvItemDefIndex = 6;
    inv.OnInventoryClosed();
    if (inv.InventoryOpen || g_closed != 1 || g_hooks != 3 || inv.InventoryAnimThingDefIndex != 0 || inv.RHSetInvItemDefIndex != 0) { std::printf("005bd597_TEST FAIL\n"); return 1; }
    if ((unsigned char*)&inv.InventoryOpen - (unsigned char*)&inv != 0x58 || (unsigned char*)&inv.RHSetInvItemDefIndex - (unsigned char*)&inv != 0x128) { std::printf("005bd597_TEST FAIL\n"); return 1; }
    std::printf("005bd597_TEST PASS\n");
    return 0;
}
