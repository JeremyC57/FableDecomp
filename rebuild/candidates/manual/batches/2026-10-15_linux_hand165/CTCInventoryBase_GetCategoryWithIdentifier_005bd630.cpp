// CTCInventoryBase::GetCategoryWithIdentifier(long) -- the inventory category whose def carries the given identifier, or 0
// Leaf: GetCategoryWithIdentifier
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005BD630, 105 bytes; frame-pointer size TU
// (`#pragma optimize("y", off)` + `optimize("s", on)`), `ret 4`.
// Callee: the category def's release through vtable slot 1 (+0x04) when its count drops to zero.
// Shape: the category count is a signed local (`cdq ; idiv` by 0x2C) kept in the frame with the index (ebx, esi, edi
// hold the hit flag, this and the byte offset); each category's def pointer is copied by value through an inline
// GetDef() (add-ref null-tested, release not), the identifier compare is taken into bl (`sete`) before the release,
// and a hit returns `begin() + i` (`imul eax,eax,2Ch ; add eax,[esi+20h]`).
// Layout: retail NInventory::CCategory is 0x2C bytes (PDB 0x30 with a 16-byte item vector): PAssociatedThing +0x00,
// ItemsInCategory +0x08, PDef +0x14, PInventory +0x18, GlobalDefIndex +0x1C, SelectedItemIndex +0x20,
// HighlightedItemIndex +0x24, Selected +0x28, DisplayItems +0x29; retail CTCInventoryBase Categories +0x20 (PDB +0x24);
// CInventoryCategoryDef Identifier +0x50.
#pragma optimize("y", off)
#pragma optimize("s", on)

struct CInventoryCategoryDef {
    virtual void v00();
    virtual void Destroy();                          // slot 1 (+0x04)
    long          RefCount;                          // +0x04
    unsigned char _pad_0x08[0x48];
    long          Identifier;                        // +0x50
};

template <class T> struct CDefPointer {
    T* Object;
    __forceinline CDefPointer(const CDefPointer& other) : Object(other.Object) { if (Object) ++Object->RefCount; }
    __forceinline ~CDefPointer() { if (--Object->RefCount == 0) Object->Destroy(); }
    T* operator->() const { return Object; }
};

namespace NInventory {

struct CCategory {                                   // retail 0x2C bytes
    unsigned char _pad_0x00[0x14];                   // PAssociatedThing, ItemsInCategory
    CDefPointer<CInventoryCategoryDef> PDef;         // +0x14
    unsigned char _pad_0x18[0x10];                   // PInventory, GlobalDefIndex, SelectedItemIndex, HighlightedItemIndex
    bool          Selected;                          // +0x28
    bool          DisplayItems;                      // +0x29
    unsigned char _pad_0x2a[0x2];

    __forceinline CDefPointer<CInventoryCategoryDef> GetDef() const { return PDef; }
};

template <class T> struct CRetailVector {            // STLport vector (12 bytes)
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    T* begin() const { return _M_start; }
    long size() const { return _M_finish - _M_start; }
    T& operator[](long n) const { return *(begin() + n); }
};

struct CTCInventoryBase {
    void*         __vftable;                         // +0x00
    void*         ParentThing;                       // +0x04 CTCBase
    unsigned char _pad_0x08[0x18];
    CRetailVector<CCategory> Categories;             // +0x20

    CCategory* GetCategoryWithIdentifier(long identifier);
};

CCategory* CTCInventoryBase::GetCategoryWithIdentifier(long identifier)
{
    long count = Categories.size();
    for (long i = 0; i < count; ++i) {
        bool found = Categories[i].GetDef()->Identifier == identifier;
        if (found)
            return &Categories[i];
    }
    return 0;
}

}
