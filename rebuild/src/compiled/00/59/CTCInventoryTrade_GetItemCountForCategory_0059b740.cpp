// CTCInventoryTrade::GetItemCountForCategory() -- the buy or sell item count matching the selected category (identifier 11 / 12), else 0
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0059B740, 55 bytes.
// Callees: CTCInventoryBase::GetSelectedCategory @ 0x005BCACE, NInventory::CCategory::GetCategoryIdentifier @ 0x005BD1B3.
//
// Shape: both locals zeroed up front (`xor edi,edi ; xor ebx,ebx`), the identifier only fetched for a non-null category,
// then a two-case `switch` lowered to `sub ebx,0Bh ; je ; dec ebx ; jne` (GOTCHAS: `dec ; jne` = a switch case). Size TU
// (`#pragma optimize("s", on)`, as the rest of the trade module). Members: engine/CTCInventoryTrade.h names
// ItemSellCount (+0x190) / ItemBuyCount (+0x194); retail holds them at +0x164 / +0x168 (the header's PDB layout is
// 0x2C high here: CTCInventoryBase is 0x18 smaller, and the vector, list and two maps before them are 4/8 bytes
// narrower each), so the header is NOT included and a local model carries the real names. Which of 11/12 is "buy" is
// inferred from the member order (buy = 11 -> ItemBuyCount); the enum names are descriptive.
// The local view is named CTCInventoryTrade_Retail so it does not shadow the PDB header engine/CTCInventoryTrade.h.
#pragma optimize("s", on)
namespace NInventory {
class CCategory { public:
    long GetCategoryIdentifier();                   // 0x005BD1B3
};
}
enum { CATEGORY_TRADE_BUY = 11, CATEGORY_TRADE_SELL = 12 };
struct CTCInventoryBase_Retail {
    virtual void v00();
    unsigned char _pad_0x04[0x130 - 4];
    NInventory::CCategory* GetSelectedCategory();   // 0x005BCACE
};
struct CTCInventoryTrade_Retail : CTCInventoryBase_Retail {
    unsigned char _pad_0x130[0x164 - 0x130];
    long ItemSellCount;                             // +0x164
    long ItemBuyCount;                              // +0x168
    long GetItemCountForCategory();
};
long CTCInventoryTrade_Retail::GetItemCountForCategory()
{
    long count = 0;
    long ident = 0;
    NInventory::CCategory* category = GetSelectedCategory();
    if (category)
        ident = category->GetCategoryIdentifier();
    switch (ident) {
    case CATEGORY_TRADE_BUY:
        count = ItemBuyCount;
        break;
    case CATEGORY_TRADE_SELL:
        count = ItemSellCount;
        break;
    }
    return count;
}