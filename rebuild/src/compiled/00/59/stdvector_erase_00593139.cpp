// std::vector::erase(iterator position) -- (vector<pair<long, long> >) STLport single-element erase: when position is not the last element, assign the tail [position + 1, finish) down onto it through the out-of-line __copy; drop finish by one element (trivial destructor); return position
// Leaf: ?erase@?$vector@U?$pair@JJ@std@@V?$allocator@U?$pair@JJ@std@@@2@@std@@QAEPAU?$pair@JJ@2@PAU32@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00593139, 42 bytes (+ int3 padding to 44), thiscall, `ret 4`, returns position.
// Identity (proven): vector<pair<long, long> >::erase(iterator); evidence: its callers CTCInventory::GetStyleCardsListUIComponent 0x005850FD, 0x00586080 and CTCInventory::OpenPCInventoryMenu 0x005885A0 erase from a local vector filled by NInventory::CTCInventoryBase::GetAllItems 0x005BF482, whose PDB parameter is vector<pair<long, long> >&; 8-byte stride; __copy 0x0059269B (two-dword element copy).
// Label: the manifest's "GetNewTreeNode" is propagated across byte-identical bodies (family H014: seven rows that differ only in
//   the element stride (here 8 bytes: `lea ecx,[edi+8h]`, `add [esi+4],-8h`) and the __copy call; retail has
//   no identical-code folding, so each address is one instantiation); this is STLport 4.6 `vector::erase(iterator)`.
// Callees: std::__copy<pair<long, long>*, pair<long, long>*, int> @ 0x0059269B (`__fastcall`: position + 1 in ecx, finish in edx; position, the
//   `random_access_iterator_tag` reference -- homed in the dead high byte of the position argument slot -- and
//   `(int*)0` on the stack).
// Body: STLport 4.6 `if (__position + 1 != end()) __copy_ptrs(__position + 1, _M_finish, __position, _TrivialAss());
//   --_M_finish; _Destroy(_M_finish); return __position;` with __copy_ptrs(__false_type) = `__copy(first, last, result,
//   random_access_iterator_tag(), (ptrdiff_t*)0)`; __copy is a same-TU noinline body (retail's sits a few hundred bytes
//   earlier in the same TU).
// Pragma: optimize("s", on) + optimize("y", off) -- a frame-pointer size TU (`push ebp ; mov ebp,esp`).
#pragma optimize("s", on)
#pragma optimize("y", off)
#include <stddef.h>

namespace std {
struct __false_type { __false_type() { } };
struct random_access_iterator_tag { random_access_iterator_tag() { } };
template <class _Tp> class allocator { public: allocator() { } };
template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
};
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
template <class _RandomAccessIter, class _OutputIter, class _Distance>                  // 0x0059269B (same-TU body)
__declspec(noinline) _OutputIter __fastcall __copy(_RandomAccessIter __first, _RandomAccessIter __last,
                                                   _OutputIter __result, const random_access_iterator_tag&, _Distance*)
{
    for (_Distance __n = __last - __first; __n > 0; --__n) {
        *__result = *__first;
        ++__first;
        ++__result;
    }
    return __result;
}
template <class _Tp>
inline _Tp* __copy_ptrs(_Tp* __first, _Tp* __last, _Tp* __result, const __false_type&)
{
    return __copy(__first, __last, __result, random_access_iterator_tag(), (ptrdiff_t*)0);
}
template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }
template <class _Tp, class _Alloc = allocator<_Tp> >
class vector : protected _Vector_base<_Tp, _Alloc> {
public:
    typedef _Tp* iterator;
    typedef __false_type _TrivialAss;
    iterator end() { return this->_M_finish; }
    iterator erase(iterator __position)
    {
        if (__position + 1 != end())
            __copy_ptrs(__position + 1, this->_M_finish, __position, _TrivialAss());
        --this->_M_finish;
        _Destroy(this->_M_finish);
        return __position;
    }
};
}
typedef std::pair<long, long> TElem;
template std::vector<TElem>::iterator std::vector<TElem>::erase(std::vector<TElem>::iterator);