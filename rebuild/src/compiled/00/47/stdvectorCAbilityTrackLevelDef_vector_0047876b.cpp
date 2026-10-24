// std::vector<CAbilityTrackLevelDef>::vector(const vector& x) -- STLport copy constructor: base-construct with x.size() elements, then placement-copy every element of x
// Retail: Fable.exe (TLC, VC7.1) @ 0x0047876B, 83 bytes, `ret 4` (returns this).
// Callees: _Vector_base<CAbilityTrackLevelDef>::_Vector_base(size_t n, const allocator&) @ 0x004787BE (row of
// this chunk), CAbilityTrackLevelDef::CAbilityTrackLevelDef(const CAbilityTrackLevelDef&) @
// 0x004787F6 (landed).
// Shape: STLport 4.x `vector(const vector& __x) : _Vector_base(__x.size(), __x.get_allocator()) { _M_finish =
// uninitialized_copy(__x._M_start, __x._M_finish, _M_start); }` with the `__false_type` copy loop inlined:
// `for (; first != last; ++first, ++cur) _Construct(&*cur, *first);` (placement new, hence the null test). The
// size is a signed divide by the 0x48-byte element (`push 48h ; pop ecx`); `this` stays in ebx, the copy's end
// goes to the dead `x` slot, the allocator temporary to that slot's high byte (`lea eax,[ebp+0Bh]`). Same source
// as the 0x88-byte sibling 0x004774D6 (CEngineLocalDetailObjectDef); only the element size changes the shape.
// Frame-pointer size TU (`optimize("y", off)` + `("s", on)`), the
// helpers `__forceinline`.
// Label: the catalog's "CAbilityTrackLevelDef_vector_CopyAndResize" (single caller 0x004786F0).
// Leaf: ??0?$vector
#pragma optimize("y", off)
#pragma optimize("s", on)
#include <new>

struct CAbilityTrackLevelDef {
    unsigned char _body[0x48];
    CAbilityTrackLevelDef(const CAbilityTrackLevelDef& other);              // 0x004787F6
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
    _Vector_base(unsigned int __n, const _Alloc& __a);                                      // 0x004787BE
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

template class vector<CAbilityTrackLevelDef>;

}