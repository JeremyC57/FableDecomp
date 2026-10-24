// _STL::vector<C3DTriangle2>::_M_allocate_and_copy(unsigned int n, const C3DTriangle2* first, const C3DTriangle2* last) -- allocate storage for n index triangles and copy-construct [first, last) into it; returns the new block
// Leaf: ??$_M_allocate_and_copy@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A6E470, 83 bytes, thiscall `ret 0Ch` (`this` unused: the allocator is
//   empty). Callee: malloc 0x00BFEA0E (MSVCR71 import thunk, behind STLport allocator<T>::allocate).
// Caller: 0x00A70C01, vector<C3DTriangle2>::reserve 0x00A70BC0 (same chunk).
// Identity: the manifest's "AllocAndCopyRange_6byte" is descriptive. The 6-byte stride is Ego_r.pdb C3DTriangle2
//   (unsigned short VertexIndex[3]), as the landed __uninitialized_copy<C3DTriangle2*> 0x00A6E350 of the same TU.
// Shape: STLport 4.6 member template `_M_allocate_and_copy(n, first, last) { pointer result =
//   _M_end_of_storage.allocate(n); __uninitialized_copy(first, last, result, _IsPODType()); return result; }` --
//   allocate is `n != 0 ? malloc(n * sizeof(T)) : 0`, the inlined __false_type copy loop strength-reduces the
//   destination to `first + (result - first)` (esi) with the placement-new null test, the 6-byte POD copied as a
//   dword + word block (`mov bx,[ebx+4]`), and the block (not the advanced cursor) is returned. __uninitialized_copy
//   must be `__forceinline` (plain inline leaves the loop as a cdecl call).
extern "C" void* __cdecl malloc(unsigned int size);     // 0x00BFEA0E
inline void* __cdecl operator new(unsigned int, void* p) throw() { return p; }   // placement new (null-tested result)

class C3DTriangle2 {                                    // 6 bytes (Ego_r.pdb)
public:
    unsigned short VertexIndex[3];                      // +0x00
};

namespace _STL {

class __false_type { public: __false_type() { } };

template <class _Tp> class allocator {
public:
    allocator() { }
    _Tp* allocate(unsigned int __n) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
};

template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
    _STLP_alloc_proxy(const _MaybeReboundAlloc& __a, _Value __p) : _MaybeReboundAlloc(__a), _M_data(__p) { }
};

template <class _T1, class _T2>
inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }

template <class _InputIter, class _ForwardIter>
__forceinline _ForwardIter __uninitialized_copy(_InputIter __first, _InputIter __last, _ForwardIter __result, const __false_type&)
{
    _ForwardIter __cur = __result;
    for (; __first != __last; ++__first, ++__cur)
        _Construct(&*__cur, *__first);
    return __cur;
}

template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Vector_base(const _Alloc& __a) : _M_start(0), _M_finish(0), _M_end_of_storage(__a, 0) { }
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;
};

template <class _Tp, class _Alloc = allocator<_Tp> > class vector : public _Vector_base<_Tp, _Alloc> {
public:
    typedef _Tp* pointer;
    typedef __false_type _IsPODType;
    template <class _ForwardIterator>
    pointer _M_allocate_and_copy(unsigned int __n, _ForwardIterator __first, _ForwardIterator __last)
    {
        pointer __result = this->_M_end_of_storage.allocate(__n);
        __uninitialized_copy(__first, __last, __result, _IsPODType());
        return __result;
    }
};

template C3DTriangle2* vector<C3DTriangle2>::_M_allocate_and_copy<const C3DTriangle2*>(unsigned int, const C3DTriangle2*, const C3DTriangle2*);

}