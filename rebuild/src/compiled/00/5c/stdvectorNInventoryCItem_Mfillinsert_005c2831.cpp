// std::vector<NInventory::CItem>::_M_fill_insert(iterator position, size_type n, const NInventory::CItem& x) -- STLport fill-insert of n copies of x at position: in place when the spare capacity suffices (shift the tail up, then fill), else through _M_insert_overflow
// Leaf: ?_M_fill_insert@
// Retail: Fable.exe @ 0x005C2831, 237 bytes, `ret 0Ch` (thiscall). Callers: vector::insert(pos, n, x) @ 0x005C2A09 (a tail
// `jmp`) and vector::resize(n, x) @ 0x005C2A13 (call 0x005C2A51). Catalog label "STL_VectorInsertRange" is descriptive.
// Callees (all the CItem instantiations of this TU): CItem copy ctor @ 0x005C08E6 (x_copy), __uninitialized_copy(first,
// last, result, const __false_type&) @ 0x005C0DF0, __copy_backward(first, last, result, const random_access_iterator_tag&,
// int*) @ 0x005C135E (this chunk's row), __uninitialized_fill_n(first, n, x, const __false_type&) @ 0x005C0E1B,
// fill(first, last, x) @ 0x005C1121 (CItem::operator= @ 0x005C0C24 per element), ~CCountedPointer<CPerItemData> @ 0x005C04BF
// (x_copy's only non-trivial member: the implicit CItem destructor inlines to that one call on [ebp-0Ch]),
// _M_insert_overflow(pos, x, const __false_type&, n, false) @ 0x005C276C.
// Body: STLport 4.x _vector.c _M_fill_insert (`_Tp __x_copy = __x;` then the elems_after > n / else arms, each ending in fill).
// Shape: frame-pointer size TU (`idiv` by 0x14, `leave`); elems_after and later `old_finish - n` live in the dead `x` slot
// ([ebp+10h]); every tag temporary sits in the high byte of `position`'s slot ([ebp+0Bh], never written). Same-TU bodies of
// the out-of-line helpers (as in retail's TU) let VC7.1 see that the tag and x_copy references do not escape.
// Layout: a local CItem model -- engine/CItem.h flattens the CCountedPointer member whose out-of-line destructor this body calls.
#pragma optimize("y", off)
#pragma optimize("s", on)
struct __false_type { __false_type() {} };
struct random_access_iterator_tag { random_access_iterator_tag() {} };
inline void* operator new(unsigned int, void* where) { return where; }
namespace NInventory { class CPerItemData; }
template <class T> class CCountedPointer { public:
    T*    Data;                                            // +0x00
    void* Info;                                            // +0x04 CCPPointerInfo*
    ~CCountedPointer();                                    // 0x005C04BF
};
namespace NInventory {
class CItem {                                              // 0x14 bytes
public:
    long ItemType;                                         // +0x00
    long NumberOfItems;                                    // +0x04
    CCountedPointer<CPerItemData> PPerItemData;            // +0x08
    long AverageBoughtPrice;                               // +0x10
    CItem(const CItem& other);                             // 0x005C08E6
    CItem& operator=(const CItem& other);                  // 0x005C0C24
};
}
using NInventory::CItem;
#pragma auto_inline(off)
CItem* __fastcall __uninitialized_copy(const CItem* first, const CItem* last, CItem* result, const __false_type&)   // 0x005C0DF0
{
    for (; first != last; ++first, ++result)
        new (result) CItem(*first);
    return result;
}
CItem* __fastcall __uninitialized_fill_n(CItem* first, unsigned int n, const CItem& x, const __false_type&)   // 0x005C0E1B
{
    for (; n > 0; --n, ++first)
        new (first) CItem(x);
    return first;
}
CItem* __fastcall __copy_backward(CItem* first, CItem* last, CItem* result, const random_access_iterator_tag&, int*)   // 0x005C135E
{
    for (int n = int(last - first); n > 0; --n)
        *--result = *--last;
    return result;
}
void __fastcall fill(CItem* first, CItem* last, const CItem& x)   // 0x005C1121
{
    for (; first != last; ++first)
        *first = x;
}
#pragma auto_inline(on)
inline CItem* __copy_backward_ptrs(CItem* first, CItem* last, CItem* result, const __false_type&)
{
    return __copy_backward(first, last, result, random_access_iterator_tag(), (int*)0);
}
struct CItemVector {                                       // STLport vector<NInventory::CItem> (12 bytes)
    typedef unsigned int size_type;
    CItem* _M_start;
    CItem* _M_finish;
    CItem* _M_end_of_storage;
    void _M_insert_overflow(CItem* position, const CItem& x, const __false_type&, size_type fill_len, bool atend);   // 0x005C276C
    void _M_fill_insert(CItem* position, size_type n, const CItem& x);
};
void CItemVector::_M_fill_insert(CItem* position, size_type n, const CItem& x)
{
    if (n != 0) {
        if (size_type(_M_end_of_storage - _M_finish) >= n) {
            CItem x_copy = x;
            const size_type elems_after = _M_finish - position;
            CItem* old_finish = _M_finish;
            if (elems_after > n) {
                __uninitialized_copy(_M_finish - n, _M_finish, _M_finish, __false_type());
                _M_finish += n;
                __copy_backward_ptrs(position, old_finish - n, old_finish, __false_type());
                fill(position, position + n, x_copy);
            } else {
                __uninitialized_fill_n(_M_finish, n - elems_after, x_copy, __false_type());
                _M_finish += n - elems_after;
                __uninitialized_copy(position, old_finish, _M_finish, __false_type());
                _M_finish += elems_after;
                fill(position, old_finish, x_copy);
            }
        } else
            _M_insert_overflow(position, x, __false_type(), n, false);
    }
}