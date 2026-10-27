// std::vector<CArray<CRandomAppearanceMorph::CBodyPartMesh>>::_M_allocate_and_copy(unsigned int n, const CArray<CRandomAppearanceMorph::CBodyPartMesh>* first, const CArray<CRandomAppearanceMorph::CBodyPartMesh>* last) -- allocate n elements and copy-construct [first, last) into them
// Leaf: ?_M_allocate_and_copy@?$vector@V?$CArray@VCBodyPartMesh@CRandomAppearanceMorph@@@@V?$allocator@
// Retail: Fable.exe (TLC, VC7.1) @ 0x006758B0, 78 bytes, thiscall `ret 0Ch`, returns the new block.
// Identity (proven): reloc_bindings ?_M_allocate_and_copy@?$vector@V?$CArray@VCBodyPartMesh@CRandomAppearanceMorph@@@@V?$allocator@...@std@@@std@@QAEPAV?$CArray@...@@IPBV3@0@Z (12-byte stride); the element copy ctor 0x00675230 carries only the stand-in ??0CElement12_00675230@@; caller vector::operator= 0x00675980.
// Callees: malloc 0x00BFEA0E (`n ? malloc(n * 0xC) : 0`), the element copy constructor (behind the placement-new null test).
// Shape (family K129, representative 0x00515020): STLport 4.6 `_M_allocate_and_copy` with allocate and __uninitialized_copy
//   inline in a speed TU; the const-pointer overload, as its bound name spells it.
extern "C" void* __cdecl malloc(unsigned int bytes);                                   // 0x00BFEA0E
inline void* operator new(unsigned int, void* where) { return where; }
class CRandomAppearanceMorph {
public:
    class CBodyPartMesh;
};
template <class T> class CArray {                       // STLport vector-backed, 12 bytes
public:
    CArray(const CArray<T>& other);                      // 0x00675230
    T* _M_start; T* _M_finish; T* _M_end_of_storage;
};
namespace std {
struct __false_type { };
template <class _Tp> class allocator { };
template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }
template <class _InputIter, class _ForwardIter>
inline _ForwardIter __uninitialized_copy(_InputIter __first, _InputIter __last, _ForwardIter __result, const __false_type&)
{
    _ForwardIter __cur = __result;
    for (; __first != __last; ++__first, ++__cur)
        _Construct(&*__cur, *__first);
    return __cur;
}
template <class _Tp, class _Alloc = allocator<_Tp> > class vector {
public:
    typedef _Tp* pointer;
    pointer _M_start;                                       // +0x00
    pointer _M_finish;                                      // +0x04
    pointer _M_end_of_storage;                              // +0x08
    static pointer allocate(unsigned int n) { return n != 0 ? (pointer)malloc(n * sizeof(_Tp)) : 0; }
    pointer _M_allocate_and_copy(unsigned int __n, const _Tp* __first, const _Tp* __last)
    {
        pointer __result = allocate(__n);
        __uninitialized_copy(__first, __last, __result, __false_type());
        return __result;
    }
};
template CArray<CRandomAppearanceMorph::CBodyPartMesh>* vector<CArray<CRandomAppearanceMorph::CBodyPartMesh> >::_M_allocate_and_copy(unsigned int, const CArray<CRandomAppearanceMorph::CBodyPartMesh>*, const CArray<CRandomAppearanceMorph::CBodyPartMesh>*);
}