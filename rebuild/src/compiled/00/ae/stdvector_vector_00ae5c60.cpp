// std::vector::vector(const vector& x) -- (vector<CUnprovenElement_00ae5730>) STLport copy constructor: allocate x.size() 16-byte elements, then copy-construct x's range into them through the out-of-line __uninitialized_copy
// Leaf: ??0?$vector@UCUnprovenElement_00ae5730@@V?$allocator@UCUnprovenElement_00ae5730@@@std@@@std@@QAE@ABV01@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AE5C60, 87 bytes (+ int3 padding), thiscall, `ret 4`.
// Identity: STLport 4.6 vector(const vector&) over a 16-byte element (`sar esi,4` / `shl eax,4`), the sibling of the landed
//   vector<C3DVertexBlend2> copy constructor 0x00AE5D80 in the same TU; its copy loop is the landed out-of-line
//   std::__uninitialized_copy 0x00AE5730 (/Gr `__fastcall`: first ecx, last edx, result and the __false_type tag pushed).
//   Element: nothing in retail names it (the only caller 0x00AE69D0 is unreferenced), so the row keeps the element name the
//   landed 0x00AE5730 row gave it (one address, one name; four dwords, implicit copy). NOTES.
// Callees: malloc 0x00BFEA0E (allocator<T>::allocate: `n != 0 ? malloc(n * 16) : 0`), 0x00AE5730.
// Extent: the catalog row (273 bytes) ran through int3 padding into eight more functions (0x00AE5CC0 .. 0x00AE5D70); SPLITS.tsv.
#include "fable_compiler.h"
#include "rebuild_abi.h"
extern "C" void* __cdecl malloc(unsigned int size);       // 0x00BFEA0E
struct CUnprovenElement_00ae5730 {                        // the landed 0x00AE5730 row's element: 16 bytes
    long _M_unproven0;
    long _M_unproven1;
    long _M_unproven2;
    long _M_unproven3;
};
namespace std {
struct __false_type { __false_type() { } };
template <class _Tp> class allocator {
public:
    allocator() { }
    allocator(const allocator<_Tp>&) { }
    _Tp* allocate(unsigned int __n, const void* = 0) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
};
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
    _STLP_alloc_proxy(const _MaybeReboundAlloc& __a, _Value __p) : _MaybeReboundAlloc(__a), _M_data(__p) { }
};
template <class _InputIter, class _ForwardIter>
_ForwardIter FABLE_FASTCALL __uninitialized_copy(_InputIter __first, _InputIter __last, _ForwardIter __result, const __false_type&);   // 0x00AE5730
template <class _Tp, class _Alloc> class _Vector_base {
public:
    typedef _Alloc allocator_type;
    _Tp* _M_start;                                        // +0x00
    _Tp* _M_finish;                                       // +0x04
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;   // +0x08
    _Vector_base(unsigned int __n, const _Alloc& __a)
        : _M_start(0), _M_finish(0), _M_end_of_storage(__a, 0)
    {
        _M_start = _M_end_of_storage.allocate(__n);
        _M_finish = _M_start;
        _M_end_of_storage._M_data = _M_start + __n;
    }
};
template <class _Tp, class _Alloc = allocator<_Tp> > class vector : public _Vector_base<_Tp, _Alloc> {
public:
    typedef vector<_Tp, _Alloc> _Self;
    typedef const _Tp* const_pointer;
    typedef _Alloc allocator_type;
    unsigned int size() const { return (unsigned int)(this->_M_finish - this->_M_start); }
    allocator_type get_allocator() const { return this->_M_end_of_storage; }
    vector(const _Self& __x)
        : _Vector_base<_Tp, _Alloc>(__x.size(), __x.get_allocator())
    {
        this->_M_finish = __uninitialized_copy((const_pointer)__x._M_start, (const_pointer)__x._M_finish, this->_M_start, __false_type());
    }
};
}
template std::vector<CUnprovenElement_00ae5730 >::vector(const std::vector<CUnprovenElement_00ae5730 >&);