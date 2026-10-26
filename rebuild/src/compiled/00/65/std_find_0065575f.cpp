// std::__find(_List_iterator first, _List_iterator last, const CThing*& val, const input_iterator_tag&) -- (list<CIntelligentPointer<const CThing> >) STLport linear search of a list of intelligent pointers for the node whose pointee is val
// Leaf: ??$__find@U?$_List_iterator@V?$CIntelligentPointer@$$CBVCThing@@@@U?$_Nonconst_traits@V?$CIntelligentPointer@$$CBVCThing@@@@@std@@@std@@PAVCThing@@@std@@YI?AU?$_List_iterator@V?$CIntelligentPointer@$$CBVCThing@@@@U?$_Nonconst_traits@V?$CIntelligentPointer@$$CBVCThing@@@@@std@@@0@U10@0ABQAVCThing@@ABUinput_iterator_tag@0@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0065575F, 51 bytes (+ int3 padding to 53), `__fastcall`: the hidden iterator
//   result in ecx (spilled to [ebp-4]), &val in edx; first, last and the unused category tag on the stack (`ret 0Ch`).
// Identity (proven): __find over list<CIntelligentPointer<const CThing> >::iterator with a CThing* key; evidence: its only caller is the find forwarder 0x00655CDC, called from NPlayerGui::CDrawPerceivers::AddPerceivingThing(CThing&) 0x0064A2DB (PDB) and its removal twin 0x0064639C, both on this+0x3C = Ego_r.pdb CDrawPerceivers::Things, list<CIntelligentPointer<const CThing> >, with the key &thing (CThing*); FableWin.pdb holds the matching Dinkumware _Find<list<CIntelligentPointer<const CThing> >::iterator, CThing*>.
// Label: the manifest's "_Find<std::_List_unchecked_iterator<std::_List_val<CIntelligentPointer<CThing_const_>,std::allocator<CIntelligentPointer<CThing_const_>_>_>_>,CThing*>" is a propagated Dinkumware spelling (family H019: seven rows that differ only in the
//   call displacement; retail has no identical-code folding, so each address is one instantiation).
// Callees: CBaseIntelligentPointer::GetPItem() const 0x00A01B50 on each node's pointer (`lea ecx,[esi+8]`).
// Shape: STLport 4.6 `while (__first != __last && !(*__first == __val)) ++__first; return __first;` (the landed __find
//   0x0069CAF0 model) with the key a raw pointer: the node's pointer is compared with the key through the engine's
//   operator==(const CIntelligentPointer<T>&, const CBaseObject*): *val is read into edi before each GetPItem. Frame-pointer size TU (the forwarders that call it carry the same pragmas).
#pragma optimize("y", off)
#pragma optimize("s", on)
class CBaseObject { public: virtual ~CBaseObject(); };
class CThing : public CBaseObject { };

class CBaseIntelligentPointer {
public:
    virtual ~CBaseIntelligentPointer();
    CBaseObject* GetPItem() const;                  // 0x00A01B50
    void* PRecord;                                  // +0x04
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
};
// FableWin.pdb ??$?8V<T>@@@@YI_NABV?$CIntelligentPointer@V<T>@@@@PBVCBaseObject@@@Z: the engine's free __fastcall (/Gr)
// comparison of an intelligent pointer with an object pointer (the key converts to const CBaseObject* at the call, so it
// is read first); declared plain `inline` the size TU keeps it out of line, as __fastcall it inlines like retail.
template <class T> inline bool __fastcall operator==(const CIntelligentPointer<T>& p, const CBaseObject* q) { return p.GetPItem() == q; }
namespace std {
struct input_iterator_tag { input_iterator_tag() { } };
template <class _Tp> struct _Nonconst_traits { typedef _Tp value_type; typedef _Tp& reference; typedef _Tp* pointer; };
struct _List_node_base {
    _List_node_base* _M_next;                       // +0x0
    _List_node_base* _M_prev;                       // +0x4
};
template <class _Tp> struct _List_node : public _List_node_base {
    _Tp _M_data;                                    // +0x8
};
struct _List_iterator_base {
    _List_node_base* _M_node;
    _List_iterator_base(_List_node_base* __x) : _M_node(__x) { }
    void _M_incr() { _M_node = _M_node->_M_next; }
    bool operator==(const _List_iterator_base& __y) const { return _M_node == __y._M_node; }
    bool operator!=(const _List_iterator_base& __y) const { return _M_node != __y._M_node; }
};
template <class _Tp, class _Traits> struct _List_iterator : public _List_iterator_base {
    typedef _List_iterator<_Tp, _Traits> _Self;
    _List_iterator(_List_node_base* __x) : _List_iterator_base(__x) { }
    _List_iterator(const _Self& __x) : _List_iterator_base(__x._M_node) { }
    typename _Traits::reference operator*() const { return ((_List_node<_Tp>*)_M_node)->_M_data; }
    _Self& operator++() { _M_incr(); return *this; }
};
template <class _InputIter, class _Tp>
_InputIter __fastcall __find(_InputIter __first, _InputIter __last, const _Tp& __val, const input_iterator_tag&)
{
    while (__first != __last && !(*__first == __val))
        ++__first;
    return __first;
}
}
typedef CIntelligentPointer<const CThing > TElem;
typedef std::_List_iterator<TElem, std::_Nonconst_traits<TElem> > TIter;
template TIter __fastcall std::__find(TIter, TIter, CThing* const&, const std::input_iterator_tag&);