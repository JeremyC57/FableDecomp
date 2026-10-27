// std::vector<CCompositeBlendDesc>::vector(const vector& x) -- STLport copy constructor: _Vector_base(x.size(), x.get_allocator()) then __uninitialized_copy of x's elements, finish = the returned end
// Leaf: ??0?$vector@VCCompositeBlendDesc@@V?$allocator@VCCompositeBlendDesc@@@std@@@std@@QAE@ABV01@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x005BB346, 55 bytes, thiscall, `ret 4`, ebp frame.
// Identity (proven): bound ??0?$vector@VCCompositeBlendDesc@@V?$allocator@VCCompositeBlendDesc@@@std@@@std@@QAE@ABV01@@Z (callers 0x006CB390, 0x00BEE280); the landed CEngineTextureMorph copy constructor 0x005BB9FA calls it on Blends (vector<CCompositeBlendDesc>); its range copy is the landed __uninitialized_copy<const CCompositeBlendDesc*, CCompositeBlendDesc*> 0x005BAF28.
// Callees: _Vector_base<CCompositeBlendDesc>::_Vector_base(size_t, const allocator&) 0x005BAA6B, std::__uninitialized_copy 0x005BAF28 (`__fastcall`).
// Shape (family M149, representative 0x0059AE54): the landed vector<CEngineTextureMorph> copy constructor 0x005BBDF1 with a
//   power-of-two stride (`sar eax,4` instead of the idiv); `optimize("y", off)` + `optimize("s", on)`.
#pragma optimize("y", off)
#pragma optimize("s", on)
class CCompositeBlendDesc {                              // 16 bytes in retail
public:
    CCompositeBlendDesc(const CCompositeBlendDesc& other);   // 0x005BA48A
    unsigned long Members[4];
};
namespace std {
struct __false_type { __false_type() { } };
template <class _Tp> class allocator {
public:
    allocator() { }
    allocator(const allocator&) { }
};
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};
template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Vector_base(unsigned int __n, const _Alloc& __a);    // 0x005BAA6B
    _Tp* _M_start;                                        // +0x0
    _Tp* _M_finish;                                       // +0x4
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;   // +0x8
};
template <class _InputIter, class _ForwardIter>
_ForwardIter __fastcall __uninitialized_copy(_InputIter __first, _InputIter __last, _ForwardIter __result, const __false_type&);
template <class _Tp, class _Alloc = allocator<_Tp> >
class vector : protected _Vector_base<_Tp, _Alloc> {
public:
    typedef unsigned int size_type;
    typedef _Alloc allocator_type;
    typedef const _Tp* const_pointer;
    size_type size() const { return size_type(this->_M_finish - this->_M_start); }
    allocator_type get_allocator() const { return allocator_type(); }
    vector(const vector<_Tp, _Alloc>& __x)
        : _Vector_base<_Tp, _Alloc>(__x.size(), __x.get_allocator())
    {
        this->_M_finish = __uninitialized_copy((const_pointer)__x._M_start, (const_pointer)__x._M_finish,
                                               this->_M_start, __false_type());
    }
};
}
template std::vector<CCompositeBlendDesc >::vector(const std::vector<CCompositeBlendDesc >&);