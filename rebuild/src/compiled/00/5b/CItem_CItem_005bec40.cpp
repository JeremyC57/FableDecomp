// CItem::CItem(long type, long count, const CPerItemData* data, long price) -- the item type, count and average price stored, the counted per-item data cleared and, when source data is given, a 0x24-byte CPerItemData copy (operator new @ 0x00BFEA1A, null-checked; copy constructor @ 0x005C14C4) handed to the counted pointer (Reset @ 0x005C1946)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005BEC40, length 82 bytes.
// Shape: the EBP frame, `and dword ptr [edi],0` pairs for the counted pointer, the null-checked `new`, `ret 10h`.
#pragma optimize("y", off)
#pragma optimize("s", on)
void* __cdecl operator new(unsigned int size);     // 0x00BFEA1A
struct CPerItemData {                              // 0x24 bytes
    unsigned char _body[0x24];
    CPerItemData(const CPerItemData& other);       // 0x005C14C4
};
struct CCountedPointer_PerItemData {               // 8 bytes: the object and its count block
    CPerItemData* m_pObject;
    void*         m_pCount;
    CCountedPointer_PerItemData() : m_pObject(0), m_pCount(0) { }
    void Reset(CPerItemData* object);              // 0x005C1946
};
struct CItem {
    long                        ItemType;          // +0x00
    long                        NumberOfItems;     // +0x04
    CCountedPointer_PerItemData PPerItemData;      // +0x08
    long                        AverageBoughtPrice;   // +0x10
    CItem(long type, long count, const CPerItemData* data, long price);
};

CItem::CItem(long type, long count, const CPerItemData* data, long price)
    : ItemType(type), NumberOfItems(count), PPerItemData(), AverageBoughtPrice(price)
{
    if (data)
        PPerItemData.Reset(new CPerItemData(*data));
}