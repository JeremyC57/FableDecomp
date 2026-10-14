// CItem::GetItemThings(CItemPairVector* out) -- when the slot holds items, push its (type, count) pair onto the vector (0x005947B6)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005BF03C, length 36 bytes.
// Shape: the EBP frame with the 8-byte pair at [ebp-8], `jle`, `leave`, `ret 4`.
#pragma optimize("y", off)
#pragma optimize("s", on)
#include "engine/CItem.h"
struct SItemPair_005bf03c { long m_type; long m_count; };
struct CItemPairVector_005bf03c {
    void PushBack(const SItemPair_005bf03c& pair);   // 0x005947B6 (labelled "Vector_PushBack_Pair")
};
struct CItem_Methods : CItem {
    void GetItemThings(CItemPairVector_005bf03c* out);
};

void CItem_Methods::GetItemThings(CItemPairVector_005bf03c* out)
{
    if (NumberOfItems > 0) {
        SItemPair_005bf03c pair;
        pair.m_type = ItemType;
        pair.m_count = NumberOfItems;
        out->PushBack(pair);
    }
}