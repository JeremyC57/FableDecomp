// CTCShop::GetStockQuantity(long item) -- look the item up in the Stock map and return its Quantity rounded to the nearest whole unit (0 when the shop has no such item)
// Leaf: GetStockQuantity
// Grade: DIFFER(93v101) -- lane gap: retail rounds with a bare dword `fistp [ebp+8]` (inline-asm FloatToLong helper writing the dead argument slot) and compares the float bit patterns as dwords; plain C++ gives __ftol2. One try.
// Retail: Fable.exe 0x007726B0, 99 bytes (`ret 4`, frame pointer: optimize("y", off)).
// Callees: map<long, CShopItem>::find(const long&) @ 0x00777170 (iterator through the hidden pointer).
// Layout: retail CTCShop: Stock (STLport map<long, CShopItem>; header node pointer at +0) +0x34; map node value
// {key +0x10, CShopItem +0x14}; CShopItem (Ego_r.pdb 0x1C): Quantity +0x00, PriceMultiplier +0x04, MaxStock +0x08
// (this row reads node+0x14 = Quantity).
// Shape: r = round(v + 0.5f); if (bits(v) == bits((float)r - 1.0f)) --r; the 0.5/1.0 constants live in frame locals.
#pragma optimize("y", off)
struct CShopItem {
    float Quantity;                                  // +0x00
    float PriceMultiplier;                           // +0x04
    float MaxStock;                                  // +0x08
    long  RestockPeriod;                             // +0x0c
    float RestockPeriodRandomness;                   // +0x10
    float SalesPerDay;                               // +0x14
    float SalesPerDayRandomness;                     // +0x18
};
struct SStockNode {
    unsigned char _rb[0x10];                         // color, parent, left, right
    long      Key;                                   // +0x10
    CShopItem Item;                                  // +0x14
};
struct CStockMap {                                   // STLport map<long, CShopItem>
    SStockNode* _M_header;
    struct iterator { SStockNode* _M_node; };
    iterator find(const long& key);                  // 0x00777170
};
inline long FloatToLong(float f) { return (long)f; }  // retail: inline-asm fistp
struct CTCShop {
    unsigned char _pad00[0x34];
    CStockMap Stock;                                 // +0x34
    long GetStockQuantity(long item);
};

long CTCShop::GetStockQuantity(long item)
{
    CStockMap::iterator it = Stock.find(item);
    if (it._M_node == Stock._M_header)
        return 0;
    float value = it._M_node->Item.Quantity;
    float half = 0.5f;
    float one = 1.0f;
    item = FloatToLong(value + half);
    float below = (float)item - one;
    if (*(long*)&value == *(long*)&below)
        --item;
    return item;
}
