// _STL::vector<CEngineGraphic>::erase(CEngineGraphic* first, CEngineGraphic* last) -- STLport range erase of the 14-byte engine graphic vector: slide the tail down over [first, last), drop the vacated end, return first
// Leaf: erase
// Retail: Fable.exe (TLC, VC7.1) @ 0x004EA40B, 59 bytes, `ret 8` (thiscall).
// Caller: vector<CEngineGraphic>::resize(n, x) @ 0x004EA3C0.. (`if (n < size()) erase(begin() + n, end()); else
//   insert(end(), n - size(), x);` -- the insert arm calls the fill-insert 0x004EA446).
// Label correction: the catalog's "Vector_InsertMoveRange_0xe" is descriptive; the element is CEngineGraphic (Ego_r.pdb,
//   14 bytes, 2-byte packed), the element of NGraphicAppearance::CReplaceableMeshes' CArray<CEngineGraphic>.
// Shape: STLport `iterator erase(iterator first, iterator last) { pointer i = __copy_ptrs(last, _M_finish, first,
//   _TrivialAss()); _Destroy(i, _M_finish); _M_finish = i; return first; }` with everything inlined: the non-POD
//   __copy_ptrs is `__copy(..., random_access_iterator_tag(), (ptrdiff_t*)0)` (`n = (finish - last) / 14`, then n implicit
//   14-byte assignments `movsd x3 ; movsw`, the source walked as `result + (last - first)`), the trivial destructor loop
//   vanishes. Size TU without a frame pointer: `#pragma optimize("s", on)` (`push 0Eh ; pop esi ; idiv esi`); the helpers
//   are `__forceinline` (the size TU would out-line them).
#pragma optimize("s", on)

namespace _STL {
struct __false_type { };
struct random_access_iterator_tag { random_access_iterator_tag() { } };

#pragma pack(push, 2)
struct CEngineGraphic {                         // Ego_r.pdb, 14 bytes
    unsigned long BankIndex;                    // +0x00
    float         AnimStep;                     // +0x04
    float         RenderSizeX;                  // +0x08
    bool          AdditiveAlpha;                // +0x0c
    unsigned char Type;                         // +0x0d
};
#pragma pack(pop)

template <class T> struct __type_traits {
    typedef __false_type has_trivial_assignment_operator;
    typedef __false_type has_trivial_destructor;
};
template <class _RandomAccessIter, class _OutputIter, class _Distance>
__forceinline _OutputIter __copy(_RandomAccessIter first, _RandomAccessIter last, _OutputIter result,
                                 const random_access_iterator_tag&, _Distance*)
{
    for (_Distance n = last - first; n > 0; --n) {
        *result = *first;
        ++first;
        ++result;
    }
    return result;
}
template <class _Tp>
__forceinline _Tp* __copy_ptrs(const _Tp* first, const _Tp* last, _Tp* result, const __false_type&)
{
    return __copy(first, last, result, random_access_iterator_tag(), (int*)0);
}
template <class _Tp> __forceinline void _Destroy(_Tp* p) { p->~_Tp(); }
template <class _ForwardIter>
__forceinline void __destroy_aux(_ForwardIter first, _ForwardIter last, const __false_type&)
{
    for (; first != last; ++first)
        _Destroy(&*first);
}
template <class _Tp> __forceinline void _Destroy(_Tp* first, _Tp* last)
{
    typedef typename __type_traits<_Tp>::has_trivial_destructor _Trivial_destructor;
    __destroy_aux(first, last, _Trivial_destructor());
}
template <class T> class vector {
public:
    typedef typename __type_traits<T>::has_trivial_assignment_operator _TrivialAss;
    T* _M_start;                                // +0x00
    T* _M_finish;                               // +0x04
    T* _M_end_of_storage;                       // +0x08
    T* erase(T* first, T* last)
    {
        T* i = __copy_ptrs(last, _M_finish, first, _TrivialAss());
        _Destroy(i, _M_finish);
        _M_finish = i;
        return first;
    }
};
template CEngineGraphic* vector<CEngineGraphic>::erase(CEngineGraphic* first, CEngineGraphic* last);
}