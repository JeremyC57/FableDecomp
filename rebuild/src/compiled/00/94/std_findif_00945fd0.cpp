// std::find_if(list<CIntelligentPointer<CThing> >::const_iterator first, const_iterator last, CIsCookingPot pred) -- the first element the predicate accepts, else last
// Leaf: ??$find_if@U?$_List_iterator@V?$CIntelligentPointer@VCThing@@@@U?$_Const_traits@V?$CIntelligentPointer@VCThing@@@@@std@@@std@@VCIsCookingPot@?A0x
// Retail: Fable.exe (TLC, VC7.1) @ 0x00945fd0, 65 bytes, /Gr `__fastcall` (the hidden result pointer in ecx; first, last and the
//   predicate on the stack), `ret 0Ch`.
// Identity (proven): Ego_r.pdb names std::find_if<list<CIntelligentPointer<CThing> >::const_iterator, (anonymous)::CIsCookingPot> (Dinkumware iterator spelling; STLport's is _List_iterator<CIntelligentPointer<CThing>, _Const_traits<...> >); the predicate's operator() 0x00945DE0 takes a const CIntelligentPointer<CThing>& (Ego_r.pdb); caller CAIStateGroup_SitForMeal::GetPCookingPot 0x00945A90 (landed); the predicate is in an anonymous namespace (Ego_r.pdb ?A0xaf7c6789).
// Callee: CIsCookingPot::operator()(const CIntelligentPointer<CThing>&) const 0x00945DE0 (the node's element at +8).
// Body: STLport 4.6 find_if forwarding to the inline __find_if(first, last, pred, iterator category): the predicate is copied
//   for the inner by-value call (`mov [esp+18h],eax`), then `while (first != last && !pred(*first)) ++first; return first;`.
class CThing;
class CBaseObjectPointer;
class CBaseIntelligentPointer {                            // Ego_r: vptr + PData, 8 bytes
public:
    virtual ~CBaseIntelligentPointer();
private:
    CBaseObjectPointer* PData;
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer { };
namespace std {
struct _List_node_base { _List_node_base* _M_next; _List_node_base* _M_prev; };
template <class _Tp> struct _List_node : public _List_node_base { _Tp _M_data; };
template <class _Tp> struct _Const_traits { typedef const _Tp& reference; typedef const _Tp* pointer; };
struct _List_iterator_base {
    _List_node_base* _M_node;
    _List_iterator_base(_List_node_base* __x) : _M_node(__x) { }
    void _M_incr() { _M_node = _M_node->_M_next; }
};
template <class _Tp, class _Traits> struct _List_iterator : public _List_iterator_base {
    typedef typename _Traits::reference reference;
    _List_iterator(_List_node_base* __x) : _List_iterator_base(__x) { }
    reference operator*() const { return ((_List_node<_Tp>*)_M_node)->_M_data; }
    _List_iterator& operator++() { _M_incr(); return *this; }
};
inline bool operator!=(const _List_iterator_base& __x, const _List_iterator_base& __y) { return __x._M_node != __y._M_node; }
struct input_iterator_tag { };
struct forward_iterator_tag : public input_iterator_tag { };
struct bidirectional_iterator_tag : public forward_iterator_tag { };
template <class _Tp, class _Traits> inline bidirectional_iterator_tag __iterator_category(const _List_iterator<_Tp, _Traits>&) { return bidirectional_iterator_tag(); }
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
    return __find_if(__first, __last, __pred, __iterator_category(__first));
}
}
namespace {
class CIsCookingPot {
public:
    bool Free;                                             // +0x00
    bool operator()(const CIntelligentPointer<CThing>& thing) const;   // 0x00945DE0
};
}   // anonymous namespace
typedef std::_List_iterator<CIntelligentPointer<CThing>, std::_Const_traits<CIntelligentPointer<CThing> > > TConstIter;
template TConstIter __fastcall std::find_if<TConstIter, CIsCookingPot>(TConstIter, TConstIter, CIsCookingPot);