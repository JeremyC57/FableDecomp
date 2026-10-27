// std::find_if<_List_iterator<CIntelligentPointer<CThing>, _Const_traits<...> >, CIsFreeBench>(first, last, pred) -- the first thing in [first, last) the predicate accepts (last if none)
// Leaf: ??$find_if@U?$_List_iterator@V?$CIntelligentPointer@VCThing@@@@U?$_Const_traits@V?$CIntelligentPointer@VCThing@@@@@std@@@std@@VCIsFreeBench@@@std@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00918110, 50 bytes, /Gr `__fastcall` (the iterator result through the hidden pointer in
//   ecx; first, last and the empty predicate on the stack), `ret 0Ch`.
// Identity (proven): only caller 0x00917E30, which sits between CAIStateGroup_CleanHouse and CAIStateGroup_CleanWork (Ego_r.pdb CAIStateGroup_Clean::GetBench runs find_if<..., CIsFreeBench>); the predicate 0x00918050 tests an object (type 5) with property bit 17 -- the bench bit CIsBenchWithOnePerson 0x0094B9B0 (landed) also tests -- and a free action point.
// Callees: CIsFreeBench::operator()(const CIntelligentPointer<CThing>&) const 0x00918050.
// Shape (family K046, representative 0x00918110): STLport `while (__first != __last && !__pred(*__first)) ++__first;
//   return __first;` through find_if's by-value hand-off to __find_if(first, last, pred,
//   input_iterator_tag): retail addresses the empty predicate's copy in the dead `first` slot with no copy
//   instruction, which VC7.1 does only when the copy constructor is user-declared (a trivial copy moves its byte) --
//   the no-op copy constructor is a modelling device, not a PDB fact.
class CThing;
class CBaseObjectPointer;
class CBaseIntelligentPointer {                          // Ego_r: vptr + PData, 8 bytes
public:
    virtual ~CBaseIntelligentPointer();                  // 0x00A01C10
private:
    CBaseObjectPointer* PData;                           // +0x04
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer { };
class CAIStateGroup_LightHomeCandle;
class CIsFreeBench {
public:
    CIsFreeBench(const CIsFreeBench&) { }                 // see Shape
    bool operator()(const CIntelligentPointer<CThing>& thing) const;   // 0x00918050
};
namespace std {
struct input_iterator_tag { };
struct forward_iterator_tag : public input_iterator_tag { };
struct bidirectional_iterator_tag : public forward_iterator_tag { };
struct _List_node_base {
    _List_node_base* _M_next;                            // +0x00
    _List_node_base* _M_prev;                            // +0x04
};
template <class _Tp> struct _List_node : public _List_node_base {
    _Tp _M_data;                                         // +0x08
};
template <class _Tp> struct _Const_traits { typedef _Tp value_type; typedef const _Tp& reference; typedef const _Tp* pointer; };
struct _List_iterator_base {
    _List_node_base* _M_node;
    bool operator==(const _List_iterator_base& __y) const { return _M_node == __y._M_node; }
    bool operator!=(const _List_iterator_base& __y) const { return _M_node != __y._M_node; }
};
template <class _Tp, class _Traits> struct _List_iterator : public _List_iterator_base {
    typedef bidirectional_iterator_tag iterator_category;
    typedef typename _Traits::reference reference;
    reference operator*() const { return ((_List_node<_Tp>*)_M_node)->_M_data; }
    _List_iterator& operator++() { _M_node = _M_node->_M_next; return *this; }
};
template <class _InputIter, class _Predicate>
inline _InputIter __find_if(_InputIter __first, _InputIter __last, _Predicate __pred, const input_iterator_tag&)
{
    while (__first != __last && !__pred(*__first))
        ++__first;
    return __first;
}
template <class _InputIter, class _Predicate>
_InputIter __fastcall find_if(_InputIter __first, _InputIter __last, _Predicate __pred)
{
    return __find_if(__first, __last, __pred, input_iterator_tag());
}
typedef _List_iterator<CIntelligentPointer<CThing>, _Const_traits<CIntelligentPointer<CThing> > > _Iter;
template _Iter __fastcall find_if<_Iter, CIsFreeBench>(_Iter, _Iter, CIsFreeBench);
}