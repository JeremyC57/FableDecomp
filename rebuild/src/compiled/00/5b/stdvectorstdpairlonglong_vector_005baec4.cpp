// std::vector<std::pair<long, long>>::vector(const vector& x) -- STLport copy constructor: _Vector_base(x.size(), x.get_allocator()) then __uninitialized_copy of x's elements, finish = the returned end
// Leaf: ??0?$vector@U?$pair@JJ@std@@V?$allocator@U?$pair@JJ@std@@@2@@std@@QAE@ABV01@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x005BAEC4, 55 bytes, thiscall, `ret 4`, ebp frame.
// Identity (proven): bound ??0?$vector@U?$pair@JJ@std@@V?$allocator@U?$pair@JJ@std@@@2@@std@@QAE@ABV01@@Z (from the _M_insert_overflow 0x005BC0FB); its _Vector_base is the landed _Vector_base<std::pair<long,long> > 0x005128B0 (8-byte stride).
// Callees: _Vector_base<std::pair<long, long>>::_Vector_base(size_t, const allocator&) 0x005128B0, std::__uninitialized_copy 0x005BAA1C (`__fastcall`).
// Shape (family M149, representative 0x0059AE54): the landed vector<CEngineTextureMorph> copy constructor 0x005BBDF1 with a
//   power-of-two stride (`sar eax,3` instead of the idiv); `optimize("y", off)` + `optimize("s", on)`.
#pragma optimize("y", off)
#pragma optimize("s", on)
namespace std {
template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
    pair(const pair& __o) : first(__o.first), second(__o.second) { }   // STLport's explicit copy
};
}
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
    _Vector_base(unsigned int __n, const _Alloc& __a);    // 0x005128B0
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
template std::vector<std::pair<long, long> >::vector(const std::vector<std::pair<long, long> >&);