// std::vector::_M_allocate_and_copy(unsigned int n, CNavigationPosition* first, CNavigationPosition* last) -- allocate room for n 12-byte navigation positions and copy-construct [first, last) into it, return the block
// Leaf: ??$_M_allocate_and_copy@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A7E9D0, 88 bytes (+ int3 padding to 96), thiscall `ret 0Ch` (`this` unused: the
//   allocator is empty).
// Caller: vector<CNavigationPosition>::reserve 0x00A80980 (this chunk; reached from CNavQuadTree::LoadFromFile's
//   `ActionPoints.reserve(count)` at 0x00A7B0FA) -- the non-const pointer instantiation. Its byte-identical twin
//   0x00A7F9F0 is the const-pointer instantiation called by vector<CNavigationPosition>::operator= (0x00A80860).
// Callees: malloc 0x00BFEA0E (allocator::allocate: `n ? malloc(n * 12) : 0`).
// Label: catalog "Vector_Allocate_12ByteElements_Copy" is descriptive.
// Shape: STLport `pointer result = _M_end_of_storage.allocate(n); __uninitialized_copy(first, last, result,
//   _IsPODType()); return result;` with the __false_type copy loop inline (placement-new null test on the destination,
//   CNavigationPosition's implicit copy: the C2DVector's memberwise copy, then the layer) -- VC strength-reduces the
//   layer read to `[esi+ecx]` with esi = first - result.
extern "C" void* __cdecl malloc(unsigned int size);         // 0x00BFEA0E
extern "C" void __cdecl free(void* block);                  // 0x00BFEA14
inline void* operator new(unsigned int, void* p) { return p; }

class C2DVector {
public:
    float X;
    float Y;
    C2DVector(const C2DVector& v) : X(v.X), Y(v.Y) { }
};

class CNavigationPosition {                                 // 12 bytes (Ego_r.pdb)
public:
    C2DVector Position;                                     // +0x00
    long      Layer;                                        // +0x08
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
    _Tp* allocate(unsigned int __n) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};

template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};

template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Tp* _M_start;                                          // +0x00
    _Tp* _M_finish;                                         // +0x04
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage; // +0x08
};

template <class _Tp, class _Alloc = allocator<_Tp> > class vector : public _Vector_base<_Tp, _Alloc> {
public:
    typedef unsigned int size_type;
    typedef _Tp* pointer;
    typedef __false_type _IsPODType;
    template <class _ForwardIterator>
    pointer _M_allocate_and_copy(size_type __n, _ForwardIterator __first, _ForwardIterator __last)
    {
        pointer __result = this->_M_end_of_storage.allocate(__n);
        __uninitialized_copy(__first, __last, __result, _IsPODType());
        return __result;
    }
};
}

template CNavigationPosition* std::vector<CNavigationPosition>::_M_allocate_and_copy<CNavigationPosition*>(unsigned int, CNavigationPosition*, CNavigationPosition*);