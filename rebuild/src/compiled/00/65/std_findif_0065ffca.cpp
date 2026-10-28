// std::find_if(std::list<CCountedPointer<CVillageTaskHorn> >::iterator first, iterator last, CIsHornTaskWithThisHorn pred) -- the first village horn task the predicate accepts (the task using a given horn), or last
// Leaf: ??$find_if@U?$_List_iterator@V?$CCountedPointer@VCVillageTaskHorn@@@@U?$_Nonconst_traits@V?$CCountedPointer@VCVillageTaskHorn@@@@@std@@@std@@VCIsHornTaskWithThisHorn@@@std@@YI?AU?$_List_iterator@V?$CCountedPointer@VCVillageTaskHorn@@@@U?$_Nonconst_traits@V?$CCountedPointer@VCVillageTaskHorn@@@@@std@@@0@U10@0VCIsHornTaskWithThisHorn@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0065FFCA, 64 bytes, __fastcall (hidden result in ecx), ebp frame, `ret 10h`, /O1 TU.
// Identity (proven): the catalog label "CGuiControlTree::SelectNextEntryByName" is propagated. Its only caller is
//   CTCVillage::UpdateHornTasks (landed 0x0065BE6C) at 0x0065BF25, which builds a CIsHornTaskWithThisHorn (Ego_r.pdb: PHorn,
//   CIntelligentPointer<CThing>, 8 bytes) and searches its horn-task list; the predicate's operator() 0x0065F116 copies the element
//   with CCountedPointer<CVillageTaskHorn>'s copy ctor 0x0065D89B (reloc_bindings). Ego_r.pdb has the same search as Dinkumware
//   ??$find_if@V?$_List_iterator...CCountedPointer@VCVillageTaskHorn...@VCIsHornTaskWithThisHorn@@@std@@YI (donor 0x006A00B8); this is
//   the STLport 4.6 spelling.
// Callees: CIntelligentPointer<CThing>::CIntelligentPointer(const CIntelligentPointer&) 0x004A9730 (the predicate copy),
//   std::__find_if<..., CIsHornTaskWithThisHorn>(first, last, pred, const input_iterator_tag&) 0x0065F96D (stand-in body below),
//   CBaseIntelligentPointer::~CBaseIntelligentPointer() 0x00A01C10 (the by-value predicate, destroyed by the callee).
// Shape: STLport `return __find_if(__first, __last, __pred, _STLP_ITERATOR_CATEGORY(__first, _InputIter));`; the same-TU noinline
//   __find_if body lets VC7.1 home the empty tag in the `last` argument slot (`lea esi,[ebp+0Fh]`) as retail does.
#pragma optimize("s", on)
class CThing;
class CVillageTaskHorn;

class CBaseIntelligentPointer {
public:
    virtual ~CBaseIntelligentPointer();            // 0x00A01C10
    void* PData;                                   // +0x04
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    CIntelligentPointer(const CIntelligentPointer& other);   // 0x004A9730 for <CThing>
};
template <class T> class CCountedPointer {
public:
    T*    Data;
    void* Info;
    CCountedPointer(const CCountedPointer& other); // 0x0065D89B for <CVillageTaskHorn>
    ~CCountedPointer();
};

class CIsHornTaskWithThisHorn {
public:
    CIntelligentPointer<CThing> PHorn;             // +0x00
    bool operator()(CCountedPointer<CVillageTaskHorn> task) const;   // 0x0065F116
};

namespace std {
struct input_iterator_tag { };
struct forward_iterator_tag : public input_iterator_tag { };
struct bidirectional_iterator_tag : public forward_iterator_tag { };
template <class _Tp> struct _Nonconst_traits { };
struct _List_node_base { _List_node_base* _M_next; _List_node_base* _M_prev; };
template <class _Tp> struct _List_node : public _List_node_base { _Tp _M_data; };
struct _List_iterator_base {
    _List_node_base* _M_node;
    __forceinline _List_iterator_base(_List_node_base* __x) : _M_node(__x) { }
    __forceinline void _M_incr() { _M_node = _M_node->_M_next; }
    __forceinline bool operator!=(const _List_iterator_base& __y) const { return _M_node != __y._M_node; }
};
template <class _Tp, class _Traits> struct _List_iterator : public _List_iterator_base {
    __forceinline _List_iterator(_List_node_base* __x) : _List_iterator_base(__x) { }
    __forceinline _List_iterator(const _List_iterator& __x) : _List_iterator_base(__x._M_node) { }
    __forceinline _Tp& operator*() const { return ((_List_node<_Tp>*)_M_node)->_M_data; }
    __forceinline _List_iterator& operator++() { _M_incr(); return *this; }
};
__forceinline bidirectional_iterator_tag iterator_category(const _List_iterator_base&) { return bidirectional_iterator_tag(); }

#pragma auto_inline(off)
template <class _InputIter, class _Predicate>
__declspec(noinline) _InputIter __fastcall __find_if(_InputIter __first, _InputIter __last, _Predicate __pred, const input_iterator_tag&)
{
    while (__first != __last && !__pred(*__first))
        ++__first;
    return __first;
}
#pragma auto_inline(on)

template <class _InputIter, class _Predicate>
_InputIter __fastcall find_if(_InputIter __first, _InputIter __last, _Predicate __pred)
{
    return __find_if(__first, __last, __pred, iterator_category(__first));
}
}

typedef std::_List_iterator<CCountedPointer<CVillageTaskHorn>, std::_Nonconst_traits<CCountedPointer<CVillageTaskHorn> > > CHornTaskIterator_0065ffca;
template CHornTaskIterator_0065ffca __fastcall std::find_if(CHornTaskIterator_0065ffca, CHornTaskIterator_0065ffca, CIsHornTaskWithThisHorn);