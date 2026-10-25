// std::vector::_M_allocate_and_copy(size_type __n, const_pointer __first, const_pointer __last) -- (vector<CDefIndex >) allocate __n elements and copy-construct [__first, __last) into them; return the block
// Leaf: ??$_M_allocate_and_copy@PBVCDefIndex@@@?$vector@VCDefIndex@@V?$allocator@VCDefIndex@@@std@@@std@@IAEPAVCDefIndex@@IPBV2@0@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004E2589, 63 bytes (size-optimised TU, odd entry), thiscall (this unused), `ret 0Ch`.
// Identity: std::vector<CDefIndex >::_M_allocate_and_copy<const CDefIndex*> -- proven; evidence: its only caller is
//   std::vector<CDefIndex >::operator= 0x004E24BA (this chunk, G062: its callers are the landed CContainerRewardHeroDef::Copy 0x004E249B (ObjectFamilies at +0x28, Ego_r.pdb CArray<CDefIndex>) and CGameScriptInterface::CreatureGeneratorSetFamily 0x008A9020), which calls it for the reallocation branch.
//   STLport spelling (the Dinkumware PDB name sets have no _M_ helpers). Family G063 (the seven _M_allocate_and_copy of G062).
// Callees: malloc 0x00BFEA0E (allocator::allocate).
// Shape: STLport 4.6 _vector.h member template without EH: allocate, the __uninitialized_copy loop strength-reduced to one
//   (result - first) offset register, return the block; /Os needs __forceinline on allocate and the helpers.
#pragma optimize("s", on)
extern "C" void* __cdecl malloc(unsigned int size);        // 0x00BFEA0E
inline void* operator new(unsigned int, void* p) { return p; }
class CDefIndex { public: long Index; };                 // 4-byte def index, trivial copy
namespace std {
struct __false_type { __false_type() { } };
template <class _T1, class _T2> __forceinline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }
template <class _InputIter, class _ForwardIter>
__forceinline _ForwardIter __uninitialized_copy(_InputIter __first, _InputIter __last, _ForwardIter __result, const __false_type&)
{
    _ForwardIter __cur = __result;
    for (; __first != __last; ++__first, ++__cur)
        _Construct(&*__cur, *__first);
    return __cur;
}
template <class _Tp> class allocator {
public:
    __forceinline _Tp* allocate(unsigned int __n) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
};
template <class _Value, class _Alloc> class _STLP_alloc_proxy : public _Alloc {
public:
    _Value _M_data;
};
template <class _Tp, class _Alloc = allocator<_Tp> > class vector {
public:
    typedef _Tp* pointer;
    typedef unsigned int size_type;
    typedef __false_type _IsPODType;                       // __type_traits of a class/enum element
    pointer _M_start;
    pointer _M_finish;
    _STLP_alloc_proxy<pointer, _Alloc> _M_end_of_storage;
protected:
    template <class _ForwardIterator>
    pointer _M_allocate_and_copy(size_type __n, _ForwardIterator __first, _ForwardIterator __last)
    {
        pointer __result = this->_M_end_of_storage.allocate(__n);
        __uninitialized_copy(__first, __last, __result, _IsPODType());
        return __result;
    }
};
typedef CDefIndex _Elem;
template _Elem* vector<_Elem>::_M_allocate_and_copy<const _Elem*>(unsigned int, const _Elem*, const _Elem*);
}