// std::vector<C3DTriangle2>::_M_allocate_and_copy(unsigned int n, const C3DTriangle2* first, const C3DTriangle2* last) -- allocate storage for n triangles and placement copy [first, last) into it; returns the new block
// Leaf: ?_M_allocate_and_copy@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A53650, length 83 bytes (+13 int3 pad), `ret 0Ch`.
// Caller: 0x00A54CFE (vector<C3DTriangle2>::operator=, the reallocating arm). Callee: malloc 0x00BFEA0E.
// Identity: catalogued as `Array_Copy_Construct`. Element (Ego_r.pdb): C3DTriangle2 = unsigned short VertexIndex[3].
// Shape: STLport 4.x `_M_allocate_and_copy` verbatim: the malloc allocator's `n != 0 ? malloc(n * 6) : 0` (`lea ; shl`),
//   then `__uninitialized_copy(first, last, result, __false_type())` inlined (placement null test, the 6-byte block copy
//   through ebx/ebp, the destination strength-reduced as result - first + cur); `this` is never read and the new
//   block (not the advanced pointer) is returned in eax. `__uninitialized_copy` is `__forceinline` (plain inline leaves it
//   out of line here, a separate COMDAT call).
extern "C" void* __cdecl malloc(unsigned int);       // 0x00BFEA0E
inline void* operator new(unsigned int, void* p) { return p; }   // placement new (the null test on the destination)

class C3DTriangle2 {                                 // 6 bytes
public:
    unsigned short VertexIndex[3];                   // +0x00
};

namespace std {
struct __false_type { __false_type() { } };          // STLport _IS_POD_ITER result for a class element

template <class _T1, class _T2>
inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }

template <class _InputIter, class _ForwardIter>
__forceinline _ForwardIter __uninitialized_copy(_InputIter __first, _InputIter __last, _ForwardIter __result, const __false_type&)
{
    _ForwardIter __cur = __result;
    for ( ; __first != __last; ++__first, ++__cur)
        _Construct(&*__cur, *__first);
    return __cur;
}

template <class _Tp>
class allocator {
public:
    allocator() { }
    _Tp* allocate(unsigned int __n) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
};

template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};

template <class _Tp, class _Alloc>
class _Vector_base {
public:
    _Tp* _M_start;                                   // +0x00
    _Tp* _M_finish;                                  // +0x04
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;   // +0x08
};

template <class _Tp, class _Alloc = allocator<_Tp> >
class vector : public _Vector_base<_Tp, _Alloc> {
public:
    protected:
    _Tp* _M_allocate_and_copy(unsigned int __n, const _Tp* __first, const _Tp* __last)
    {
        _Tp* __result = this->_M_end_of_storage.allocate(__n);
        __uninitialized_copy(__first, __last, __result, __false_type());
        return __result;
    }
    public:
};

template C3DTriangle2* vector<C3DTriangle2>::_M_allocate_and_copy(unsigned int, const C3DTriangle2*, const C3DTriangle2*);
}