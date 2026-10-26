// std::__find_if(first, last, pred, const input_iterator_tag&) -- (list<CCountedPointer<CVillageTaskTorch > > iterators, CIsTask) STLport: the first task the predicate accepts (or last)
// Leaf: ??$__find_if@U?$_List_iterator@V?$CCountedPointer@VCVillageTaskTorch@@@@U?$_Nonconst_traits@V?$CCountedPointer@VCVillageTaskTorch@@@@@std@@@std@@VCIsTask@@@std@@YI?AU?$_List_iterator@V?$CCountedPointer@VCVillageTaskTorch@@@@U?$_Nonconst_traits@V?$CCountedPointer@VCVillageTaskTorch@@@@@std@@@0@U10@0VCIsTask@@ABUinput_iterator_tag@0@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0065EAD6, 90 bytes (unaligned, frame pointer: a size-optimised TU), __fastcall `ret 0Ch`
//   (hidden result pointer in ecx, the tag reference in edx -- never read -- both iterators and the 4-byte CIsTask by value on the stack).
// Identity (proven): std::__find_if<_List_iterator<CCountedPointer<CVillageTaskTorch > >, CIsTask>; evidence: its only caller is the find_if 0x0065F9E0, called only by 0x00660AE2, which CTCVillage::RemoveThingFromTask(CIntelligentPointer<CVillageTaskBase>, const CThing&) 0x00659EFC calls with this+0x4C -- the TorchTasks list (Ego_r.pdb CTCVillage::TorchTasks, list<CCountedPointer<CVillageTaskTorch> >), so 0x00660AE2 is CTCVillage::RemoveThingFromTask<CVillageTaskTorch> and the iterator is that list's; the predicate CIsTask (Ego_r.pdb: one CVillageTaskBase* Task) is compared with the converted task's Data; the converting copy 0x0065DA52 is this element's own instantiation, the temporary's destructor 0x0065DA6B is shared by all six (one CCountedPointer<CVillageTaskBase>).
// Label: the manifest's "FindBankFileAsyncEntry_ByID" is propagated (family H094: six __find_if COMDATs, one per task list).
// Callees: CCountedPointer<CVillageTaskBase>::CCountedPointer(const CCountedPointer<CVillageTaskTorch >&) 0x0065DA52 (the converting copy that makes
//   CIsTask's `const CCountedPointer<CVillageTaskBase>&` argument), CCountedPointer<CVillageTaskBase>::~CCountedPointer 0x0065DA6B.
// Shape: STLport 4.6 `while (__first != __last && !__pred(*__first)) ++__first; return __first;` -- the converted temporary lives only
//   in the right operand of `&&`, hence the conditional-destruction flag at [ebp-4]; CIsTask::operator() (Ego_r.pdb
//   ??RCIsTask@@QBE_NABV?$CCountedPointer@VCVillageTaskBase@@@@@Z) inline. Local classes -- Review at landing.
#pragma optimize("y", off)
#pragma optimize("s", on)
class CVillageTaskBase;
class CVillageTaskTorch;
class CCPPointerInfo;
template <class T> class CCountedPointer {
public:
    T*              Data;                                    // +0x00
    CCPPointerInfo* Info;                                    // +0x04
    template <class U> CCountedPointer(const CCountedPointer<U>& other);   // converting copy, out of line
    ~CCountedPointer();                                      // out of line
};
class CIsTask {                                              // Ego_r.pdb: CVillageTaskBase* Task
public:
    CVillageTaskBase* Task;                                  // +0x00
    CIsTask(const CVillageTaskBase& task);
    bool operator()(const CCountedPointer<CVillageTaskBase>& p) const { return p.Data == Task; }
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
struct input_iterator_tag { };
template <class _InputIter, class _Predicate>
_InputIter __fastcall __find_if(_InputIter __first, _InputIter __last, _Predicate __pred, const input_iterator_tag&)
{
    while (__first != __last && !__pred(*__first))
        ++__first;
    return __first;
}
}
typedef std::_List_iterator<CCountedPointer<CVillageTaskTorch >, std::_Nonconst_traits<CCountedPointer<CVillageTaskTorch > > > _Iter;
template _Iter __fastcall std::__find_if(_Iter, _Iter, CIsTask, const std::input_iterator_tag&);