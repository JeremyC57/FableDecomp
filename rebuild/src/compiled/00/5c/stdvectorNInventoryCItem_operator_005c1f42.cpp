// std::vector<NInventory::CItem>::operator=(const vector& x) -- STLport vector assignment for inventory items: reallocate and copy when x outgrows the capacity, else assign over the live items and destroy the surplus, or assign the overlap and copy-construct the rest
// Leaf: ??4
// Retail: Fable.exe @ 0x005C1F42, 209 bytes, `ret 4` (thiscall, returns this). Label correction: the catalog's
// "CActiveFile_CopyAssign" is descriptive/propagated (the 0x005C1F1F neighbour is landed as the vector's _M_clear under the
// propagated "CActiveFile::OnReadFinished"); the body is the STLport vector<T>::operator= for the 0x14-byte NInventory::CItem.
// Callees: _M_allocate_and_copy(n, first, last) @ 0x005C144A (thiscall, out of line here; catalog "fill_n<...>"),
// _M_clear @ 0x005C1F1F, __copy(first, last, result, const random_access_iterator_tag&, int*) @ 0x005C132C (`__fastcall`,
// CItem::operator= @ 0x005C0C24 per element), __destroy_aux(first, last, const __false_type&) @ 0x005C18C6 (`__fastcall`),
// __uninitialized_copy(first, last, result, const __false_type&) @ 0x005C10CC (`__fastcall`, copy ctor 0x005C08E6).
// Body: STLport 4.x _vector.c operator= (the shape of landed vector<CCombatWheel::SSlot>::operator= 0x006F3C20).
// Shape: frame-pointer size TU (`idiv` by 0x14, `leave`); xlen is kept in the dead `x` slot ([ebp+8]) and x._M_finish in the
// `push ecx` word; the tag temporaries share the high byte of that slot ([ebp+0Bh]); __copy's null distance pointer is pushed
// once above the size() >= xlen test for both arms. Same-TU bodies of the out-of-line helpers (as in retail's TU) let VC7.1
// see that the tag references do not escape.
#pragma optimize("y", off)
#pragma optimize("s", on)
#include "engine/CItem.h"
struct __false_type { __false_type() {} };
struct random_access_iterator_tag { random_access_iterator_tag() {} };
inline void* operator new(unsigned int, void* where) { return where; }
struct CInvItem : CItem {                                  // NInventory::CItem with its out-of-line members
    CInvItem(const CInvItem& other);                       // 0x005C08E6
    CInvItem& operator=(const CInvItem& other);            // 0x005C0C24
    void Destroy(unsigned int flags);                      // 0x005C1390 (called with 0 per element)
};
#pragma auto_inline(off)
CInvItem* __fastcall __uninitialized_copy(const CInvItem* first, const CInvItem* last, CInvItem* result, const __false_type&)   // 0x005C10CC
{
    for (; first != last; ++first, ++result)
        new (result) CInvItem(*first);
    return result;
}
CInvItem* __fastcall __copy(const CInvItem* first, const CInvItem* last, CInvItem* result, const random_access_iterator_tag&, int*)   // 0x005C132C
{
    for (int n = int(last - first); n > 0; --n, ++first, ++result)
        *result = *first;
    return result;
}
void __fastcall __destroy_aux(CInvItem* first, CInvItem* last, const __false_type&)   // 0x005C18C6
{
    for (; first != last; ++first)
        first->Destroy(0);
}
#pragma auto_inline(on)
inline CInvItem* __copy_ptrs(const CInvItem* first, const CInvItem* last, CInvItem* result, const __false_type&)
{
    return __copy(first, last, result, random_access_iterator_tag(), (int*)0);
}
struct CItemVector {                                       // STLport vector<NInventory::CItem> (12 bytes)
    typedef unsigned int size_type;
    CInvItem* _M_start;
    CInvItem* _M_finish;
    CInvItem* _M_end_of_storage;
    size_type size() const { return size_type(_M_finish - _M_start); }
    size_type capacity() const { return size_type(_M_end_of_storage - _M_start); }
    CInvItem* _M_allocate_and_copy(size_type n, const CInvItem* first, const CInvItem* last);   // 0x005C144A
    void _M_clear();                                       // 0x005C1F1F
    CItemVector& operator=(const CItemVector& x);
};
CItemVector& CItemVector::operator=(const CItemVector& x)
{
    if (&x != this) {
        const size_type xlen = x.size();
        if (xlen > capacity()) {
            CInvItem* tmp = _M_allocate_and_copy(xlen, x._M_start, x._M_finish);
            _M_clear();
            _M_start = tmp;
            _M_end_of_storage = _M_start + xlen;
        } else if (size() >= xlen) {
            CInvItem* i = __copy_ptrs(x._M_start, x._M_finish, _M_start, __false_type());
            __destroy_aux(i, _M_finish, __false_type());
        } else {
            __copy_ptrs(x._M_start, x._M_start + size(), _M_start, __false_type());
            __uninitialized_copy(x._M_start + size(), x._M_finish, _M_finish, __false_type());
        }
        _M_finish = _M_start + xlen;
    }
    return *this;
}