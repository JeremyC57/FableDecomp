// std::for_each(first, last, f) -- (list<CCountedPointer<CVillageTaskSecuritySweep > > iterators, CGetBestTaskForThing<CVillageTaskSecuritySweep >) apply the best-task functor to every task in the list and return it
// Leaf: ??$for_each@U?$_List_iterator@V?$CCountedPointer@VCVillageTaskSecuritySweep@@@@U?$_Nonconst_traits@V?$CCountedPointer@VCVillageTaskSecuritySweep@@@@@std@@@std@@V?$CGetBestTaskForThing@VCVillageTaskSecuritySweep@@@@@std@@YI?AV?$CGetBestTaskForThing@VCVillageTaskSecuritySweep@@@@U?$_List_iterator@V?$CCountedPointer@VCVillageTaskSecuritySweep@@@@U?$_Nonconst_traits@V?$CCountedPointer@VCVillageTaskSecuritySweep@@@@@std@@@0@0V1@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0065FB72, 50 bytes (unaligned, frame pointer: a size-optimised TU), __fastcall `ret 10h`
//   (the functor is returned through memory: the hidden result pointer in ecx; both iterators and the functor on the stack).
// Identity (proven): std::for_each<_List_iterator<CCountedPointer<CVillageTaskSecuritySweep > >, CGetBestTaskForThing<CVillageTaskSecuritySweep > >; evidence: its only callee is 0x0065EA48, CGetBestTaskForThing<CVillageTaskSecuritySweep>::operator() (H091: the functor's only caller is this function, reached only from 0x006600BE, which only CTCVillage::GetBest...TaskForThing 0x006590F4 reaches with this+0x5C, the Ego_r.pdb list<CCountedPointer<CVillageTaskSecuritySweep> >); the iterated node value at +8 is passed by reference and the 8-byte functor {Thing, PBestTask} is returned.
// Label: the manifest's "_For_each<std::_List_unchecked_iterator<std::_List_val<CCountedPointer<CVillageTaskCarryCrate>,std::allocator<CCountedPointer<CVillageTaskCarryCrate>_>_>_>,CGetBestTaskForThing<CVillageTaskCarryCrate>_>" is propagated (family H093: six for_each COMDATs, one per task class).
// Callees: CGetBestTaskForThing<CVillageTaskSecuritySweep >::operator()(const CCountedPointer<CVillageTaskSecuritySweep >&) 0x0065EA48 (family H091).
// Shape: STLport 4.6 `for (; __first != __last; ++__first) __f(*__first); return __f;` (the landed for_each<C3DVector*, CRescaleBy>
//   0x00A13440 convention); the list iterator is the landed STLport _List_iterator<T, _Nonconst_traits<T> > (0x00BBD6B0). Local
//   classes -- Review at landing.
#pragma optimize("y", off)
#pragma optimize("s", on)
class CThing;
class CVillageTaskSecuritySweep;
struct CCPPointerInfo;
template <class T> class CCountedPointer {
public:
    T*              Data;                                    // +0x00
    CCPPointerInfo* Info;                                    // +0x04
};
template <class T> class CGetBestTaskForThing {
public:
    CThing* Thing;                                           // +0x00
    T*      PBestTask;                                       // +0x04
    CGetBestTaskForThing(const CThing& thing);
    void operator()(const CCountedPointer<T>& task);         // 0x0065EA48
};
namespace std {
struct _List_node_base {
    _List_node_base* _M_next;                                // +0x0
    _List_node_base* _M_prev;                                // +0x4
};
template <class _Tp> struct _List_node : public _List_node_base {
    _Tp _M_data;                                             // +0x8
};
struct _List_iterator_base {
    _List_node_base* _M_node;
    _List_iterator_base(_List_node_base* __x) : _M_node(__x) { }
    void _M_incr() { _M_node = _M_node->_M_next; }
    bool operator==(const _List_iterator_base& __y) const { return _M_node == __y._M_node; }
    bool operator!=(const _List_iterator_base& __y) const { return _M_node != __y._M_node; }
};
template <class _Tp> struct _Nonconst_traits { typedef _Tp value_type; typedef _Tp& reference; typedef _Tp* pointer; };
template <class _Tp, class _Traits> struct _List_iterator : public _List_iterator_base {
    typedef _List_iterator<_Tp, _Traits> _Self;
    typedef _List_node<_Tp> _Node;
    _List_iterator(_List_node_base* __x) : _List_iterator_base(__x) { }
    _List_iterator(const _Self& __x) : _List_iterator_base(__x._M_node) { }
    _Tp& operator*() const { return ((_Node*)_M_node)->_M_data; }
    _Self& operator++() { this->_M_incr(); return *this; }
};
template <class _InputIter, class _Function>
_Function __fastcall for_each(_InputIter __first, _InputIter __last, _Function __f)
{
    for ( ; __first != __last; ++__first)
        __f(*__first);
    return __f;
}
}
typedef std::_List_iterator<CCountedPointer<CVillageTaskSecuritySweep >, std::_Nonconst_traits<CCountedPointer<CVillageTaskSecuritySweep > > > _Iter;
template CGetBestTaskForThing<CVillageTaskSecuritySweep > __fastcall std::for_each(_Iter, _Iter, CGetBestTaskForThing<CVillageTaskSecuritySweep >);