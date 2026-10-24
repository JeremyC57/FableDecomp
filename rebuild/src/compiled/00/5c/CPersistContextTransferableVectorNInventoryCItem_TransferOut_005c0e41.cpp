// CPersistContext_TransferableVector<NInventory::CItem>::TransferOut(CPersistContext* context, CCharString* out, long index) -- writes inventory item `index` as text through a default CPersistTraits<NInventory::CItem>
// Leaf: TransferOut
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005C0E41, 64 bytes, `leave ; ret 0Ch`.
// Callees: 0x005C0E81 CPersistTraits<NInventory::CItem>::TransferOut(CPersistContext*, CCharString*, CItem*)
// (thiscall, `ret 0Ch`); 0x005C04BF ~CCountedPointer<NInventory::CPerItemData> (manifest label `_Dest_val<...>` is
// propagated; the landed 0x005C2A5C uses it as the counted-pointer destructor too).
// Persist WRITE hook (vtable 0x01253CA4 slot 2; slot 3 is TransferIn 0x005C20D8, slot 1 Resize 0x005C2A5C, slot 0
// the count 0x005C0923 with its 20-byte divide) of CPersistContext_TransferableVector<NInventory::CItem>
// { vptr; vector<CItem>* TransVector; }. The traits object (PDB { CItem DefaultVal; }, 0x14 bytes at [ebp-14h]) is
// zeroed through one register by CItem's inline default constructor (ItemType, NumberOfItems, the counted pointer's
// Data/Info, AverageBoughtPrice); the element address is first + index*20 (`lea ; lea`), and the implicit traits
// destructor only runs the counted pointer's destructor at +8 ([ebp-0Ch]).
// Model: engine/CItem.h flattens PPerItemData into two raw words (no destructor), so the counted pointer is modelled
// locally to carry its out-of-line destructor; offsets agree with the header (Ego_r.pdb CItem 0x14 bytes).
// EBP frame + size TU (optimize("y", off), optimize("s", on)), as the landed 0x005C2A5C next to it.
#pragma optimize("y", off)
#pragma optimize("s", on)
struct CPersistContext;
struct CStringParser;
struct CCharString;
struct CPerItemData;
struct CCPPointerInfo;

struct CCountedPointer_CPerItemData {
    CPerItemData*   Data;                   // +0x0
    CCPPointerInfo* Info;                   // +0x4
    CCountedPointer_CPerItemData() : Data(0), Info(0) { }
    ~CCountedPointer_CPerItemData();        // 0x005C04BF
};

namespace NInventory {
struct CItem {                              // 0x14 bytes
    long                         ItemType;            // +0x00
    long                         NumberOfItems;       // +0x04
    CCountedPointer_CPerItemData PPerItemData;        // +0x08
    long                         AverageBoughtPrice;  // +0x10
    CItem() : ItemType(0), NumberOfItems(0), PPerItemData(), AverageBoughtPrice(0) { }
};
}

template <class T> struct CRetailVector {
    T* m_pFirst;
    T* m_pLast;
    T* m_pEnd;
    T& operator[](long index) { return *(m_pFirst + index); }
};

template <class T> struct CPersistTraits {
    T DefaultVal;
    void TransferOut(CPersistContext* context, CCharString* out, T* value);       // 0x005C0E81 for CItem
    void TransferIn(CPersistContext* context, CStringParser* parser, T* value);   // 0x005C2118 for CItem
};

struct CPersistContext_TransferableVector {
    virtual long GetCount();
    virtual void Resize(long count);
    virtual void TransferOut(CPersistContext* context, CCharString* out, long index);
    virtual void TransferIn(CPersistContext* context, CStringParser* parser, long index);
    CRetailVector<NInventory::CItem>* TransVector;     // +0x4
};

void CPersistContext_TransferableVector::TransferOut(CPersistContext* context, CCharString* out, long index)
{
    CPersistTraits<NInventory::CItem> traits;
    traits.TransferOut(context, out, &(*TransVector)[index]);
}