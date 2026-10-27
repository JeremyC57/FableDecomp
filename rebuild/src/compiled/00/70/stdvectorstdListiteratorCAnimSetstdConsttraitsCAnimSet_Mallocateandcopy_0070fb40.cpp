// std::vector<std::_List_iterator<CAnimSet, std::_Const_traits<CAnimSet> >>::_M_allocate_and_copy(size_type n, std::_List_iterator<CAnimSet, std::_Const_traits<CAnimSet> >* first, std::_List_iterator<CAnimSet, std::_Const_traits<CAnimSet> >* last) -- STLport: allocate n elements and copy-construct [first, last) into the new block, return it
// Leaf: ??$_M_allocate_and_copy@PAU?$_List_iterator@VCAnimSet@@U?$_Const_traits@VCAnimSet@@@std@@@std@@@?$vector@U?$_List_iterator@VCAnimSet@@U?$_Const_traits@VCAnimSet@@@std@@@std@@V?$allocator@U?$_List_iterator@VCAnimSet@@U?$_Const_traits@VCAnimSet@@@std@@@std@@@2@@std@@QAEPAU?$_List_iterator@VCAnimSet@@U?$_Const_traits@VCAnimSet@@@std@@@1@IPAU21@0@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0070fb40, 72 bytes, thiscall, `ret 0Ch`.
// Identity (proven): bound (reloc_bindings) ??$_M_allocate_and_copy@PAU?$_List_iterator@VCAnimSet@@U?$_Const_traits@VCAnimSet@@@std@@@std@@@?$vector@U?$_List_iterator@VCAnimSet@@...@std@@QAE...; its caller is the landed vector::reserve 0x00710B00.
// Callee: malloc 0x00BFEA0E.
// Shape (family L048): allocate(n) = `n ? malloc(n * 4) : 0`, then __uninitialized_copy(first, last, result, __false_type)
//   inline (STLport's default type traits make every class, iterator and enum non-POD): the output walks as
//   first + (result - first) under placement new's null test. Public, as the bound names have it.
class CAnimSet;
namespace std {
struct _List_node_base;
template <class _Tp> struct _Nonconst_traits { };
template <class _Tp> struct _Const_traits { };
template <class _Tp, class _Traits> struct _List_iterator {   // STLport 4.6 list iterator, 4 bytes
    _List_node_base* _M_node;                             // +0x00
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
template std::_List_iterator<CAnimSet, std::_Const_traits<CAnimSet> >* std::vector<std::_List_iterator<CAnimSet, std::_Const_traits<CAnimSet> > >::_M_allocate_and_copy<std::_List_iterator<CAnimSet, std::_Const_traits<CAnimSet> >*>(unsigned int, std::_List_iterator<CAnimSet, std::_Const_traits<CAnimSet> >*, std::_List_iterator<CAnimSet, std::_Const_traits<CAnimSet> >*);