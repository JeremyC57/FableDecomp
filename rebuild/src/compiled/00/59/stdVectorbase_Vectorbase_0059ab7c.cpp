// std::_Vector_base::_Vector_base(size_t n, const allocator_type& a) -- (_Vector_base<CUnprovenElement_0059ab7c >) STLport vector base sized constructor: null the three pointers, malloc n elements (none for zero), finish = start, end of storage = start + n
// Leaf: ??0?$_Vector_base@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0059AB7C, 52 bytes (unaligned entry, size TU), thiscall, `ret 8` (n and the
//   allocator reference; returns this).
// Identity: element CUnprovenElement_0059ab7c (8 bytes: `shl eax,3` / `lea [eax+edi*8]`) -- descriptive; evidence: DESCRIPTIVE element: an 8-byte element (`sar eax,3` in the caller); its only caller is the vector copy constructor 0x0059AE54, which NUISystem::CFrontEndManager::RefreshAvailableSavedGamesForProfile 0x00598463 applies to the vector returned by a front-end component's virtual slot 0xD4; the element is not named.
//   Family G010 (10 bodies, 4- and 8-byte elements, each its own COMDAT: retail has no ICF, so each address is one element
//   type); the manifest label "Vector_Reserve" is propagated/behavioural.
// Callees: malloc 0x00BFEA0E (`__cdecl`, `pop ecx`) through the inline allocator::allocate (`n ? malloc(n * size) : 0`; the
//   zero register doubles as the null result).
// Body: STLport 4.x _vector.h
//     _Vector_base(size_t __n, const _Alloc& __a) : _M_start(0), _M_finish(0), _M_end_of_storage(__a, 0)
//     { _M_start = _M_end_of_storage.allocate(__n); _M_finish = _M_start; _M_end_of_storage._M_data = _M_start + __n; }
#pragma optimize("s", on)
#include <stddef.h>
extern "C" void* __cdecl malloc(size_t size);              // 0x00BFEA0E
class CUnprovenElement_0059ab7c { public: unsigned long Word0, Word1; };
namespace std {
template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
};
template <class T> class allocator {
public:
    allocator() {}
    T* allocate(size_t n, const void* = 0) { return n != 0 ? (T*)malloc(n * sizeof(T)) : 0; }
};
template <class Value, class Tp, class MaybeReboundAlloc>
class _STLP_alloc_proxy : public MaybeReboundAlloc {
public:
    Value _M_data;
    _STLP_alloc_proxy(const MaybeReboundAlloc& a, Value p) : MaybeReboundAlloc(a), _M_data(p) {}
};
template <class T, class Alloc>
class _Vector_base {
public:
    _Vector_base(size_t n, const Alloc& a)
        : _M_start(0), _M_finish(0), _M_end_of_storage(a, 0)
    {
        _M_start = _M_end_of_storage.allocate(n);
        _M_finish = _M_start;
        _M_end_of_storage._M_data = _M_start + n;
    }
    T* _M_start;                                           // +0x00
    T* _M_finish;                                          // +0x04
    _STLP_alloc_proxy<T*, T, Alloc> _M_end_of_storage;     // +0x08
};
typedef CUnprovenElement_0059ab7c _Elem;
template class _Vector_base<_Elem, allocator<_Elem> >;
}