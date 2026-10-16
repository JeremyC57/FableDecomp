// CTCInventoryBase::GetAllSelectedItems(std::vector<long>&) const -- fills the vector with every category's highlighted item
// Leaf: GetAllSelectedItems
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005BEAF5, 98 bytes; frame-pointer size TU
// (`#pragma optimize("y", off)` + `optimize("s", on)`), `ret 4`.
// Callees: std::vector<long>::erase(iterator, iterator) @ 0x00440590 (the inline clear(), last pushed first),
//          NInventory::CCategory::GetHighlightedItem() @ 0x005BC99C, std::vector<long>::push_back(const long&)
//          @ 0x00485FF3.
// Shape: the out vector moves to edi, so the strength-reduced category byte offset lives in its dead argument slot
// (`and [ebp+8],ebx` with the index's zero, `add dword ptr [ebp+8],2Ch`); `for (unsigned i = 0; i < size(); ++i)`
// with size() recomputed (`cdq ; idiv`) each pass and the element as `_M_start[i]` (the `*(begin() + i)` form
// spills the index instead of the byte offset); the highlighted item is a temporary bound to push_back's const
// reference ([ebp-4]).
// Layout: retail CTCInventoryBase Categories {first, last} +0x20 (0x2C-byte NInventory::CCategory).
#pragma optimize("y", off)
#pragma optimize("s", on)

struct CLongVector {                                 // std::vector<long>
    long* _M_start;
    long* _M_finish;
    long* _M_end_of_storage;
    long* begin() { return _M_start; }
    long* end() { return _M_finish; }
    long* erase(long* first, long* last);            // 0x00440590
    void clear() { erase(begin(), end()); }
    void push_back(const long& value);               // 0x00485FF3
};

namespace NInventory {

struct CCategory {                                   // retail 0x2C bytes
    unsigned char _pad_0x00[0x2C];
    long GetHighlightedItem() const;                 // 0x005BC99C
};

template <class T> struct CRetailVector {            // STLport vector (12 bytes)
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    T* begin() const { return _M_start; }
    unsigned long size() const { return (unsigned long)(_M_finish - _M_start); }
    T& operator[](unsigned long n) const { return _M_start[n]; }
};

struct CTCInventoryBase {
    void*         __vftable;                         // +0x00
    void*         ParentThing;                       // +0x04 CTCBase
    unsigned char _pad_0x08[0x18];
    CRetailVector<CCategory> Categories;             // +0x20

    void GetAllSelectedItems(CLongVector& items) const;
};

void CTCInventoryBase::GetAllSelectedItems(CLongVector& items) const
{
    items.clear();
    for (unsigned long i = 0; i < Categories.size(); ++i)
        items.push_back(Categories[i].GetHighlightedItem());
}

}
