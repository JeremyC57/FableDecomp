// std::__uninitialized_fill_n(CLineVector* first, unsigned int n, const CLineVector& x, const __false_type&) -- copy-construct n copies of one C2DLineF vector into raw storage (each copy allocates its own block and copies the 16-byte lines), return the end
// Leaf: ??$__uninitialized_fill_n@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A74A20, 159 bytes (+ int3 padding to 160), `ret 8` (/Gr: first in ecx, n in edx;
//   x and the tag reference on the stack).
// Callers: 0x00A74D4A, 0x00A7509A, 0x00A75196 (the vector<vector<C2DLineF> > fill/insert paths). Callee: malloc 0x00BFEA0E.
// Label: catalog "Vector_ConstructRanges_16ByteElements" is descriptive.
// Shape: STLport `for (; n > 0; --n, ++cur) _Construct(&*cur, x); return cur;` with an unsigned count (`jbe`), the
//   placement-new null test, and vector's copy constructor inline: `_Vector_base(x.size(), x.get_allocator())` (three
//   zero stores, `n ? malloc(n * 16) : 0`, start/finish/end stores) then `_M_finish = __uninitialized_copy(x._M_start,
//   x._M_finish, _M_start, __false_type())` -- the inner copy is inline too (null-tested placement copy of four dwords).
extern "C" void* __cdecl malloc(unsigned int size);         // 0x00BFEA0E
extern "C" void __cdecl free(void* block);                  // 0x00BFEA14
inline void* operator new(unsigned int, void* p) { return p; }

class C2DVector {
public:
    float X;
    float Y;
    C2DVector(const C2DVector& v) : X(v.X), Y(v.Y) { }   // memberwise copy: dword moves, not a block copy
};

class C2DLineF {                                            // 16 bytes: two end points
public:
    C2DVector Start;                                        // +0x00
    C2DVector End;                                          // +0x08
};

namespace std {
struct __false_type { __false_type() { } };

template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }

template <class _InputIter, class _ForwardIter>
inline _ForwardIter __uninitialized_copy(_InputIter __first, _InputIter __last, _ForwardIter __result, const __false_type&)
{
    _ForwardIter __cur = __result;
    for (; __first != __last; ++__first, ++__cur)
        _Construct(&*__cur, *__first);
    return __cur;
}

template <class _Tp> class allocator {
public:
    allocator() { }
    allocator(const allocator<_Tp>&) { }
    _Tp* allocate(unsigned int __n) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};

template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
    _STLP_alloc_proxy(const _MaybeReboundAlloc& __a, _Value __p) : _MaybeReboundAlloc(__a), _M_data(__p) { }
};

template <class _Tp, class _Alloc> class _Vector_base {
public:
    typedef _Alloc allocator_type;
    _Vector_base(const _Alloc& __a) : _M_start(0), _M_finish(0), _M_end_of_storage(__a, 0) { }
    _Vector_base(unsigned int __n, const _Alloc& __a) : _M_start(0), _M_finish(0), _M_end_of_storage(__a, 0)
    {
        _M_start = _M_end_of_storage.allocate(__n);
        _M_finish = _M_start;
        _M_end_of_storage._M_data = _M_start + __n;
    }
    _Tp* _M_start;                                          // +0x00
    _Tp* _M_finish;                                         // +0x04
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage; // +0x08
};

template <class _Tp, class _Alloc = allocator<_Tp> > class vector : public _Vector_base<_Tp, _Alloc> {
public:
    typedef unsigned int size_type;
    typedef _Tp* pointer;
    typedef const _Tp* const_pointer;
    typedef _Alloc allocator_type;
    typedef __false_type _IsPODType;
    allocator_type get_allocator() const { return (const allocator_type&)this->_M_end_of_storage; }
    size_type size() const { return size_type(this->_M_finish - this->_M_start); }
    vector(const vector<_Tp, _Alloc>& __x) : _Vector_base<_Tp, _Alloc>(__x.size(), __x.get_allocator())
    {
        this->_M_finish = __uninitialized_copy((const_pointer)__x._M_start, (const_pointer)__x._M_finish, this->_M_start, _IsPODType());
    }
};
}
typedef std::vector<C2DLineF> CLineVector;                 // 12 bytes

namespace std {
template <class _ForwardIter, class _Size, class _Tp>
_ForwardIter __fastcall __uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x, const __false_type&)
{
    _ForwardIter __cur = __first;
    for (; __n > 0; --__n, ++__cur)
        _Construct(&*__cur, __x);
    return __cur;
}
template CLineVector* __fastcall __uninitialized_fill_n(CLineVector*, unsigned int, const CLineVector&, const __false_type&);
}