// std::__find(list<CIntelligentPointer<CThingAICreature> >::iterator first, iterator last, CThingAICreature* const& val, const input_iterator_tag&) -- STLport input-iterator find: the first node whose intelligent pointer holds val
// Leaf: ??$__find@U?$_List_iterator@V?$CIntelligentPointer@VCThingAICreature@@@@U?$_Nonconst_traits@V?$CIntelligentPointer@VCThingAICreature@@@@@std@@@std@@PAVCThingAICreature@@@std@@YI?AU?$_List_iterator@V?$CIntelligentPointer@VCThingAICreature@@@@U?$_Nonconst_traits@V?$CIntelligentPointer@VCThingAICreature@@@@@std@@@0@U10@0ABQAVCThingAICreature@@ABUinput_iterator_tag@0@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00511BF0, 54 bytes, /Gr `__fastcall` returning the iterator through the hidden pointer
//   (ecx), `val` in edx, first / last / the tag on the stack (`ret 0Ch`).
// Identity (probable): callers CWorldMap::PopulateRegionWithVillagers 0x00506190 (the value is a local creature pointer) and the find front end 0x005132D0 (user-copy-constructed iterators: the non-const list iterator) right after the TU's random_shuffle<CThingAICreature**> 0x00513270; FableWin.pdb's CWorldMap TU holds exactly one list find, find<list<CIntelligentPointer<CThingAICreature> >::iterator, CThingAICreature*> (next to its random_shuffle<CThingAICreature*> instantiations).
// Callees: CBaseIntelligentPointer::GetPItem() const 0x00A01B50.
// Shape (family M105, representative 0x00511BF0): `while (first != last && !(*first == val)) ++first; return first;` (the
//   landed 0x00567240 model) with the intelligent pointer's `==` as `GetPItem() == p` inline; val is re-read every pass
//   (ebx) because the out-of-line GetPItem may alias it.
class CBaseObject;
class CBaseObjectPointer;
class CThing;
class CThingAICreature;
class CBaseIntelligentPointer {                       // vtable 0x0129C95C (Ego_r: vptr + PData, 8 bytes)
public:
    virtual ~CBaseIntelligentPointer();                  // 0x00A01C10
    CBaseObject* GetPItem() const;                       // 0x00A01B50
private:
    CBaseObjectPointer* PData;                           // +0x04
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
    bool operator==(const T* item) const { return GetPItem() == item; }
};
namespace std {
struct input_iterator_tag { };
template <class _Tp> struct _Nonconst_traits { typedef _Tp value_type; typedef _Tp& reference; typedef _Tp* pointer; };
template <class _Tp> struct _Const_traits { typedef _Tp value_type; typedef const _Tp& reference; typedef const _Tp* pointer; };
struct _List_node_base {
    _List_node_base* _M_next;
    _List_node_base* _M_prev;
};
template <class _Tp> struct _List_node : public _List_node_base {
    _Tp _M_data;
};
struct _List_iterator_base {
    _List_node_base* _M_node;
    _List_iterator_base(_List_node_base* __x) : _M_node(__x) { }
    _List_iterator_base() { }
    void _M_incr() { _M_node = _M_node->_M_next; }
    bool operator!=(const _List_iterator_base& __y) const { return _M_node != __y._M_node; }
};
template <class _Tp, class _Traits> struct _List_iterator : public _List_iterator_base {
    typedef typename _Traits::reference reference;
    typedef _List_iterator<_Tp, _Nonconst_traits<_Tp> > iterator;
    _List_iterator(_List_node<_Tp>* __x) : _List_iterator_base(__x) { }
    _List_iterator() { }
    _List_iterator(const iterator& __x) : _List_iterator_base(__x._M_node) { }
    reference operator*() const { return ((_List_node<_Tp>*)_M_node)->_M_data; }
    _List_iterator& operator++() { this->_M_incr(); return *this; }
};
template <class _InputIter, class _Tp>
_InputIter __fastcall __find(_InputIter __first, _InputIter __last, const _Tp& __val, const input_iterator_tag&)
{
    while (__first != __last && !(*__first == __val))
        ++__first;
    return __first;
}
typedef _List_iterator<CIntelligentPointer<CThingAICreature>, _Nonconst_traits<CIntelligentPointer<CThingAICreature> > > _It;
template _It __fastcall __find<_It, CThingAICreature*>(_It, _It, CThingAICreature* const&, const input_iterator_tag&);
}