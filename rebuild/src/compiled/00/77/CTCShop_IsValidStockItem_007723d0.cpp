// CTCShop::IsValidStockItem(long item) const -- true when the item is one of the shop's default stock items (FindDefaultStockItem finds it)
// Leaf: ?IsValidStockItem@CTCShop@@QBE_NJ@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x007723D0, 19 bytes, thiscall `ret 4`: `push item ; call FindDefaultStockItem ; neg eax ; sbb eax,eax ;
//   neg eax` (the found pointer as a bool).
// Identity: Ego_r.pdb / FableWin.pdb ?IsValidStockItem@CTCShop@@QBE_NJ@Z, the 19-byte member right after FindDefaultStockItem
//   0x007723A0 that it calls; the landed shop-trade rows (AddSellItemToList 0x0059ED42, ConstructSellActionsList 0x005A45F6, their PC
//   twins, and CTCInventoryClothing::CalculateArmorValues 0x005B4EBA) call it on the shop of the trader. Replaces the earlier
//   `_atexit` / HasDefaultStockItem labels (Codex pass 15 name fix).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCSHOP_VALIDSTOCK
#include "fable_string.h"
class CPersistContext;
#include "engine/CTCShop.h"

bool CTCShop::IsValidStockItem(long item) const
{
    return FindDefaultStockItem(item) != 0;
}
