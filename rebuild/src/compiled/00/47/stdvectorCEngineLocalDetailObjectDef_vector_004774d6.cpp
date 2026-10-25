// std::vector<CEngineLocalDetailObjectDef>::vector(const vector& x) -- STLport copy constructor: base-construct with x.size() elements, then placement-copy every element of x
// Retail: Fable.exe (TLC, VC7.1) @ 0x004774D6, 90 bytes, `ret 4` (returns this).
// Callees: _Vector_base<CEngineLocalDetailObjectDef>::_Vector_base(size_t n, const allocator&) @ 0x00477530 (row of
// this chunk), CEngineLocalDetailObjectDef::CEngineLocalDetailObjectDef(const CEngineLocalDetailObjectDef&) @
// 0x0047756C (landed).
// Shape: STLport 4.x `vector(const vector& __x) : _Vector_base(__x.size(), __x.get_allocator()) { _M_finish =
// uninitialized_copy(__x._M_start, __x._M_finish, _M_start); }` with the `__false_type` copy loop inlined:
// `for (; first != last; ++first, ++cur) _Construct(&*cur, *first);` (placement new, hence the null test). The
// size is a signed divide by the 0x88-byte element; the constant stays in edi and also steps both pointers.
// `this` is parked in the one-word frame, the copy's end in the dead `x` slot, the allocator temporary in that
// slot's high byte (`lea eax,[ebp+0Bh]`). Frame-pointer size TU (`optimize("y", off)` + `("s", on)`), the
// helpers `__forceinline`.
// Label: the catalog's "CEngineLocalDetailObjectDef_vector_CopyAndResize" (single caller 0x00477418).
// Leaf: ??0?$vector@VCEngineLocalDetailObjectDef@@
#pragma optimize("y", off)
#pragma optimize("s", on)
#include <new>

class CEngineLocalDetailObjectDef {                    // retail 0x88 bytes; a class in the PDB (mangled V)
public:
    unsigned char _body[0x88];
    CEngineLocalDetailObjectDef(const CEngineLocalDetailObjectDef& other);              // 0x0047756C
};

namespace std {

struct __false_type { __false_type() { } };

template <class _Tp> class allocator {
public:
    allocator() { }
    allocator(const allocator&) { }
};

template <class _T1, class _T2> __forceinline void _Construct(_T1* __p, const _T2& __val)
{
    new (__p) _T1(__val);
}

template <class _InputIter, class _ForwardIter>
__forceinline _ForwardIter __uninitialized_copy(_InputIter __first, _InputIter __last, _ForwardIter __result,
                                                const __false_type&)
{
    _ForwardIter __cur = __result;
    for (; __first != __last; ++__first, ++__cur)
        _Construct(&*__cur, *__first);
    return __cur;
}

template <class _InputIter, class _ForwardIter>
__forceinline _ForwardIter uninitialized_copy(_InputIter __first, _InputIter __last, _ForwardIter __result)
{
    return __uninitialized_copy(__first, __last, __result, __false_type());
}

template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _Tp* _M_end_of_storage;
    _Vector_base(unsigned int __n, const _Alloc& __a);                                      // 0x00477530
};

template <class _Tp, class _Alloc = allocator<_Tp> > class vector : public _Vector_base<_Tp, _Alloc> {
public:
    typedef const _Tp* const_pointer;
    unsigned int size() const { return (unsigned int)(this->_M_finish - this->_M_start); }
    _Alloc get_allocator() const { return _Alloc(); }
    vector(const vector& __x);
};

template <class _Tp, class _Alloc>
vector<_Tp, _Alloc>::vector(const vector<_Tp, _Alloc>& __x)
    : _Vector_base<_Tp, _Alloc>(__x.size(), __x.get_allocator())
{
    this->_M_finish = uninitialized_copy((const_pointer)__x._M_start, (const_pointer)__x._M_finish,
                                         this->_M_start);
}

template class vector<CEngineLocalDetailObjectDef>;

}