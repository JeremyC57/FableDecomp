// std::__find_if<_List_iterator<CCountedPointer<CVillageTaskTorch> >,CIsThingInvolvedWithTask>(first, last, pred, const input_iterator_tag&) -- first torch task the thing is involved with, or last
// Leaf: __find_if
// Retail: Fable.exe (TLC, VC7.1) @ 0x0065EA75, 97 bytes, `__fastcall` (hidden result in ecx, the empty tag reference in
// edx, both iterators and the 4-byte predicate on the stack, `ret 0Ch`). Only caller: std::find_if @ 0x0065F943 (landed as
// "ConstructCRangeString"), itself called by CTCVillage::IsThingInTaskList<CVillageTaskTorch> 0x0065FF5E on TorchTasks.
// Callees: CCountedPointer<CVillageTaskBase>(const CCountedPointer<CVillageTaskTorch>&) @ 0x0065DA52 (out of line; the
// count bump), the task's virtual IsThingInvolvedWithTask(const CThing&) const (slot 5, +0x14; CVillageTaskTorch's is
// 0x00860310), ~CCountedPointer @ 0x0065DA6B (out of line).
// Label correction: the catalog "Container_FindEntry" is descriptive; this is STLport `__find_if` for input iterators.
// Shape: STLport verbatim `while (first != last && !pred(*first)) ++first; return first;` with the predicate's operator()
// INLINE taking `const CCountedPointer<CVillageTaskBase>&`: the up-cast copy of `*first` is a temporary of the loop
// condition, so its destruction is conditional on the `&&` (the `or [ebp-4],1` / `test byte ptr [ebp-4],1` flag; a
// by-value parameter is destroyed unconditionally inside the inlined call instead). The condition lands in bl.
// Opt: size TU (`optimize("s", on)`; VC7.1 picks the ebp frame for the temporary and its flag word).
#pragma optimize("s", on)

class CThing;

class CVillageTaskBase {
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual bool IsThingInvolvedWithTask(const CThing& thing) const;   // slot 5 (+0x14)
};
class CVillageTaskTorch : public CVillageTaskBase {
};

template <class T> class CCountedPointer {
public:
    T*    m_pObject;                                  // +0x00
    void* m_pInfo;                                    // +0x04 the shared count block
    template <class U> CCountedPointer(const CCountedPointer<U>& other);   // 0x0065DA52 (out of line, the up-cast copy)
    ~CCountedPointer();                               // 0x0065DA6B (out of line)
    T* operator->() const { return m_pObject; }
};

class CIsThingInvolvedWithTask {                      // Ego_r.pdb, 4 bytes
public:
    const CThing* Thing;                              // +0x00
    bool operator()(const CCountedPointer<CVillageTaskBase>& task) const { return task->IsThingInvolvedWithTask(*Thing); }
};

namespace std {
struct input_iterator_tag { };

struct _List_node_base {
    _List_node_base* _M_next;                         // +0x00
    _List_node_base* _M_prev;                         // +0x04
};
template <class _Tp> struct _List_node : public _List_node_base {
    _Tp _M_data;                                      // +0x08
};
template <class _Tp> struct _List_iterator {
    _List_node_base* _M_node;
    _List_iterator(_List_node<_Tp>* __x) : _M_node(__x) { }
    _List_iterator(const _List_iterator& __x) : _M_node(__x._M_node) { }
    _Tp& operator*() const { return ((_List_node<_Tp>*)_M_node)->_M_data; }
    _List_iterator& operator++() { _M_node = _M_node->_M_next; return *this; }
    bool operator!=(const _List_iterator& __y) const { return _M_node != __y._M_node; }
};

template <class _InputIter, class _Predicate>
_InputIter __fastcall __find_if(_InputIter __first, _InputIter __last, _Predicate __pred, const input_iterator_tag&)
{
    while (__first != __last && !__pred(*__first))
        ++__first;
    return __first;
}

template _List_iterator<CCountedPointer<CVillageTaskTorch> > __fastcall __find_if(_List_iterator<CCountedPointer<CVillageTaskTorch> >,
    _List_iterator<CCountedPointer<CVillageTaskTorch> >, CIsThingInvolvedWithTask, const input_iterator_tag&);
}