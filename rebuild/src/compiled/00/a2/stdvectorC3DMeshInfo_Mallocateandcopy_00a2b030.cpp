// std::vector<C3DMeshInfo>::_M_allocate_and_copy(unsigned int n, C3DMeshInfo* first, C3DMeshInfo* last) -- allocate n elements and copy-construct [first, last) into them
// Leaf: ??$_M_allocate_and_copy@PAVC3DMeshInfo@@@?$vector@VC3DMeshInfo@@V?$allocator@VC3DMeshInfo@@@std@@@std@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A2B030, 78 bytes, thiscall `ret 0Ch`, returns the new block.
// Identity (proven): reloc_bindings ??$_M_allocate_and_copy@PAVC3DMeshInfo@@@?$vector@VC3DMeshInfo@@V?$allocator@VC3DMeshInfo@@@std@@@std@@QAEPAVC3DMeshInfo@@IPAV2@0@Z; the copies call the bound ??0C3DMeshInfo@@QAE@ABV0@@Z 0x00A2A400; caller vector<C3DMeshInfo>::reserve 0x00A2C210.
// Callees: malloc 0x00BFEA0E (`n ? malloc(n * 0x60) : 0`), the element copy constructor (behind the placement-new null test).
// Shape (family K129, representative 0x00515020): STLport 4.6 `_M_allocate_and_copy` with allocate and __uninitialized_copy
//   inline in a speed TU; the member-template overload (non-const iterators), as its bound name spells it.
extern "C" void* __cdecl malloc(unsigned int bytes);                                   // 0x00BFEA0E
inline void* operator new(unsigned int, void* where) { return where; }
class C3DMeshInfo {                                      // retail 0x60 bytes; opaque here
public:
    C3DMeshInfo(const C3DMeshInfo& other);               // 0x00A2A400
    unsigned char _data[0x60];
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
    template <class _ForwardIterator>
    pointer _M_allocate_and_copy(unsigned int __n, _ForwardIterator __first, _ForwardIterator __last)
    {
        pointer __result = allocate(__n);
        __uninitialized_copy(__first, __last, __result, __false_type());
        return __result;
    }
};
template C3DMeshInfo* vector<C3DMeshInfo >::_M_allocate_and_copy<C3DMeshInfo*>(unsigned int, C3DMeshInfo*, C3DMeshInfo*);
}