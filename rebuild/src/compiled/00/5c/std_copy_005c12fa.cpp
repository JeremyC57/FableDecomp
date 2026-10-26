// std::__copy(_RandomAccessIter __first, _RandomAccessIter __last, _OutputIter __result, const random_access_iterator_tag&, _Distance*) -- (NInventory::CItem*, NInventory::CItem*, int) STLport random-access copy: n = last - first assignments through the element's out-of-line operator=, return the result's end
// Flags: /Gr
// Leaf: ??$__copy@PAVCItem@NInventory@@PAV12@H@std@@YIPAVCItem@NInventory@@PAV12@00ABUrandom_access_iterator_tag@0@PAH@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005C12FA, 50 bytes (size-optimised TU), __fastcall (/Gr: first ecx, last edx;
//   result, the tag reference and the null distance pointer on the stack), `ret 0Ch`.
// Identity (proven): std::__copy<NInventory::CItem*, NInventory::CItem*, int>; evidence: its operator= callee 0x005C0C24 assigns that layout (dwords +0 / +4, the counted pointer's assignment 0x005C077F at +8, the dword +0x10) every 0x14 bytes; its callers are agent951's landed vector<NInventory::CItem>::erase 0x005C1ECC (CTCInventoryBase's vector +0xAC) and the __copy_ptrs forwarder 0x005C13F9 -- the mutable-source instance (its const-source twin is 0x005C132C)
// Label: the manifest's "_Copy_impl<CArray<CRandomAppearanceMorph::CBodyPartMesh>*,CArray<class_CRandomAppearanceMo" is a propagated / behavioural label (family J070: five 50-byte copy loops, each its own instantiation; retail has no identical-code folding).
// Callees: NInventory::CItem::operator=(const CItem&) 0x005C0C24 (thiscall, `ret 4`), once per element.
// Shape: STLport 4.6 _algobase.h random-access __copy verbatim; the count is the exact pointer difference (`idiv 0x14`, the
//   divisor by push/pop under /Os) and the result pointer is advanced in its own stack slot.
// Element: NInventory::CItem (Ego_r.pdb 0x14 bytes: ItemType, NumberOfItems, CCountedPointer<CPerItemData> PPerItemData, AverageBoughtPrice)
#pragma optimize("s", on)
template <class T> class CCountedPointer {               // 8 bytes: the object and its shared CCPPointerInfo count block
public:
    T*    Data;                                          // +0x0
    void* Info;                                          // +0x4 CCPPointerInfo*
};
namespace NInventory {
class CPerItemData;
class CItem {                                            // Ego_r.pdb 0x14 bytes
public:
    long                          ItemType;              // +0x00
    long                          NumberOfItems;         // +0x04
    CCountedPointer<CPerItemData> PPerItemData;          // +0x08
    long                          AverageBoughtPrice;    // +0x10
    CItem& operator=(const CItem& other);                // 0x005C0C24 (implicit; emitted out of line in retail)
};
}
namespace std {
struct random_access_iterator_tag { };
template <class _RandomAccessIter, class _OutputIter, class _Distance>
_OutputIter __copy(_RandomAccessIter __first, _RandomAccessIter __last, _OutputIter __result,
                   const random_access_iterator_tag&, _Distance*)
{
    for (_Distance __n = __last - __first; __n > 0; --__n) {
        *__result = *__first;
        ++__first;
        ++__result;
    }
    return __result;
}
template NInventory::CItem* __copy<NInventory::CItem*, NInventory::CItem*, int>(NInventory::CItem*, NInventory::CItem*, NInventory::CItem*, const random_access_iterator_tag&, int*);
}