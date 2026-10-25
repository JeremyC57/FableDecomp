// std::vector::erase(iterator first, iterator last) -- (vector<NInventory::CItem >) STLport range erase: assign the tail [last, finish) down onto first through the out-of-line __copy, destroy the vacated tail through the out-of-line __destroy_aux, set finish to the copy's end, return first
// Leaf: ?erase@?$vector@VCItem@NInventory@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005C1ECC, 55 bytes (size-optimised TU, frame pointer, odd entry), thiscall, `ret 8`, returns first.
// Identity: std::vector<NInventory::CItem >::erase(iterator, iterator) -- proven; evidence: its callers are CTCInventoryBase::ReturnAllConfiscatedItems 0x005BF91A and OnSerialise 0x005BFD5E, which erase [begin, end) of the vector at +0xAC = Ego_r.pdb CTCInventoryBase::ConfiscatedItems (+0xBC less four 4-byte vector shrinks; vector<NInventory::CItem>), the clear() 0x005C2761 and resize 0x005C2A13; the 0x14-byte element's operator= 0x005C0C24 copies ItemType / NumberOfItems, assigns the CCountedPointer PPerItemData at +8 (0x005C077F) and copies AverageBoughtPrice (+0x10), and the destroy loop calls CItem's scalar deleting destructor 0x005C1390 (the PDB names a non-virtual ??_GCItem); the PDB name sets hold vector<NInventory::CItem>::erase
//   Family G072 (seven byte-identical bodies, one COMDAT per T: retail has no ICF). Catalogue label "std::vector_assign_from_range" is propagated/behavioural.
// Callees: std::__copy<T*, T*, int>(first, last, result, const random_access_iterator_tag&, int*) 0x005C12FA (`__fastcall`: first in
//   ecx, last in edx, result / tag reference / null distance pointer on the stack, `ret 0Ch`; 0x14-byte elements, the element's
//   operator= 0x005C0C24); std::__destroy_aux<T*>(first, last, const __false_type&) 0x005C18C6 (`__fastcall`, `ret 4`; CItem's scalar deleting destructor 0x005C1390 called with 0 per element).
// Body: STLport 4.6 `pointer __i = __copy_ptrs(__last, _M_finish, __first, _TrivialAss()); _Destroy(__i, _M_finish);
//   _M_finish = __i; return __first;` -- the family G067 body with __copy left out of line: both callees are same-TU noinline
//   `__fastcall` bodies that never touch their tag, so VC7.1 homes both tags in the live `first` slot ([ebp+0Bh]) as retail does.
#pragma optimize("y", off)
#pragma optimize("s", on)
#include <stddef.h>
template <class T> class CCountedPointer {                   // 8 bytes: the object + its shared CCPPointerInfo count block
public:
    T*    Data;                                              // +0x0
    void* Info;                                              // +0x4 CCPPointerInfo*
    CCountedPointer<T>& operator=(const CCountedPointer<T>& other);
    ~CCountedPointer();
};
namespace NInventory {
class CPerItemData;
class CItem {                                                // 0x14 bytes (Ego_r.pdb)
public:
    long                          ItemType;                  // +0x00
    long                          NumberOfItems;             // +0x04
    CCountedPointer<CPerItemData> PPerItemData;              // +0x08
    long                          AverageBoughtPrice;        // +0x10
    ~CItem();
};
}

namespace std {

struct __false_type { __false_type() { } };
struct random_access_iterator_tag { random_access_iterator_tag() { } };
template <class _Tp> class allocator { public: allocator() { } };

template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};

template <class _Tp, class _Alloc>
class _Vector_base {
public:
    _Tp* _M_start;                                              // +0x00
    _Tp* _M_finish;                                             // +0x04
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;     // +0x08
};

#pragma auto_inline(off)
template <class _RandomAccessIter, class _OutputIter, class _Distance>                   // 0x005C12FA (same-TU body)
__declspec(noinline) _OutputIter __fastcall __copy(_RandomAccessIter __first, _RandomAccessIter __last, _OutputIter __result,
                                                  const random_access_iterator_tag&, _Distance*)
{
    for (_Distance __n = __last - __first; __n > 0; --__n) {
        *__result = *__first;
        ++__first;
        ++__result;
    }
    return __result;
}
#pragma auto_inline(on)

template <class _Tp>
__forceinline _Tp* __copy_ptrs(_Tp* __first, _Tp* __last, _Tp* __result, const __false_type&)
{
    return __copy(__first, __last, __result, random_access_iterator_tag(), (ptrdiff_t*)0);
}

template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }

#pragma auto_inline(off)
template <class _ForwardIterator>                                                        // 0x005C18C6 (same-TU body)
__declspec(noinline) void __fastcall __destroy_aux(_ForwardIterator __first, _ForwardIterator __last, const __false_type&)
{
    for ( ; __first != __last; ++__first)
        _Destroy(&*__first);
}
#pragma auto_inline(on)

template <class _Tp> __forceinline void _Destroy(_Tp* __first, _Tp* __last) { __destroy_aux(__first, __last, __false_type()); }

template <class _Tp, class _Alloc = allocator<_Tp> >
class vector : protected _Vector_base<_Tp, _Alloc> {
public:
    typedef _Tp* pointer;
    typedef _Tp* iterator;
    typedef __false_type _TrivialAss;
    iterator erase(iterator __first, iterator __last)
    {
        pointer __i = __copy_ptrs(__last, this->_M_finish, __first, _TrivialAss());
        _Destroy(__i, this->_M_finish);
        this->_M_finish = __i;
        return __first;
    }
};

typedef NInventory::CItem _Elem;
template vector<_Elem>::iterator vector<_Elem>::erase(vector<_Elem>::iterator, vector<_Elem>::iterator);

} // namespace std