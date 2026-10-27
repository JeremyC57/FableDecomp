// std::vector<NThingMap::CThingMapCell>::_M_allocate_and_copy(size_type n, const NThingMap::CThingMapCell* first, const NThingMap::CThingMapCell* last) -- STLport: allocate n elements and copy-construct [first, last) into the new block, return it
// Leaf: ??$_M_allocate_and_copy@PBVCThingMapCell@NThingMap@@@?$vector@VCThingMapCell@NThingMap@@V?$allocator@VCThingMapCell@NThingMap@@@std@@@std@@QAEPAVCThingMapCell@NThingMap@@IPBV23@0@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0085dca0, 72 bytes, thiscall, `ret 0Ch`.
// Identity (proven): bound (reloc_bindings) as the non-template ?_M_allocate_and_copy@?$vector@VCThingMapCell@NThingMap@@@std@@QAEPAVCThingMapCell@NThingMap@@IPBV34@0@Z (no allocator argument); its caller is the landed vector<CThingMapCell>::operator= 0x0085DFD0 (const pointers).
// Callee: malloc 0x00BFEA0E.
// Shape (family L048): allocate(n) = `n ? malloc(n * 4) : 0`, then __uninitialized_copy(first, last, result, __false_type)
//   inline (STLport's default type traits make every class, iterator and enum non-POD): the output walks as
//   first + (result - first) under placement new's null test. Public, as the bound names have it.
class CMapwhoNode;
namespace NThingMap {
class CThingMapCell {                                      // Ego_r.pdb, 4 bytes
public:
    CMapwhoNode* PFirstNode;                              // +0x00
};
}
extern "C" void* __cdecl malloc(unsigned int size);        // 0x00BFEA0E
inline void* __cdecl operator new(unsigned int, void* where) { return where; }
namespace std {
struct __false_type { };
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
    _Tp* allocate(unsigned int __n) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
};
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};
template <class _Tp, class _Alloc = allocator<_Tp> >
class vector {                                             // STLport 4.6, 12 bytes
public:
    typedef _Tp* pointer;
    typedef unsigned int size_type;
    _Tp* _M_start;                                        // +0x00
    _Tp* _M_finish;                                       // +0x04
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;  // +0x08
    template <class _ForwardIterator>
    pointer _M_allocate_and_copy(size_type __n, _ForwardIterator __first, _ForwardIterator __last)
    {
        pointer __result = this->_M_end_of_storage.allocate(__n);
        __uninitialized_copy(__first, __last, __result, __false_type());
        return __result;
    }
};
}
template NThingMap::CThingMapCell* std::vector<NThingMap::CThingMapCell >::_M_allocate_and_copy<const NThingMap::CThingMapCell*>(unsigned int, const NThingMap::CThingMapCell*, const NThingMap::CThingMapCell*);