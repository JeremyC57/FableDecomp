// std::vector<void*>::vector(const vector& x) -- STLport copy constructor of a pointer vector: base-construct with x.size() elements, then trivially copy x's range
// Retail: Fable.exe (TLC, VC7.1) @ 0x00477428, 56 bytes, `ret 4` (returns this).
// Callees: _Vector_base<T*>::_Vector_base(size_t n, const allocator&) @ 0x00450E3A (zero the three words,
// allocate n, set start/finish/end), __uninitialized_copy(first, last, result, const __true_type&) @ 0x0045EFE5
// (`__fastcall` /Gr algorithm: first in ecx, last in edx; the POD copy).
// Shape: STLport 4.x `vector(const vector& __x) : _Vector_base(__x.size(), __x.get_allocator()) { _M_finish =
// uninitialized_copy(__x._M_start, __x._M_finish, _M_start); }` -- the copy goes through the 3-argument public
// `uninitialized_copy`, which builds the `__true_type` tag itself: that is why retail loads `_M_start` into eax
// BEFORE forming the tag address in esi (`mov eax,[edi] ; lea esi,[ebp+0Bh] ; push esi ; push eax`); calling the
// 4-argument `__uninitialized_copy(..., _IsPODType())` directly pushes `[edi]` after the tag. Both empty
// temporaries -- the allocator copy and the tag -- are homed in the high byte of the dead `x` argument slot
// (`lea [ebp+0Bh]`, twice); the size is `(finish - start) >> 2`. Frame-pointer size TU (`optimize("y", off)` +
// `("s", on)`).
// Label correction: the catalog's "lua_stackspace" is propagated; this is the STLport copy constructor shared
// (COMDAT-folded) by every vector of pointers -- five callers (0x0047740C, 0x0067BCA8, 0x0067BF98, 0x0067C308,
// 0x006B5D05).
// Leaf: ??0?$vector
#pragma optimize("y", off)
#pragma optimize("s", on)

namespace std {

struct __true_type { __true_type() { } };

template <class _Tp> class allocator {
public:
    allocator() { }
    allocator(const allocator&) { }
};

void** __fastcall __uninitialized_copy(void* const* __first, void* const* __last, void** __result,
                                       const __true_type&);                                 // 0x0045EFE5
__forceinline void** uninitialized_copy(void* const* __first, void* const* __last, void** __result)
{
    return __uninitialized_copy(__first, __last, __result, __true_type());
}

template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _Tp* _M_end_of_storage;
    _Vector_base(unsigned int __n, const _Alloc& __a);                                      // 0x00450E3A
};

template <class _Tp, class _Alloc = allocator<_Tp> > class vector : public _Vector_base<_Tp, _Alloc> {
public:
    typedef _Vector_base<_Tp, _Alloc> _Base;
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

template class vector<void*>;

}