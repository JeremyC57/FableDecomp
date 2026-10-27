// std::list<CCountedPointer<CVillageTaskSecuritySweep> >::remove_if<CIsTask>(CIsTask pred) -- STLport remove_if: erase every task the predicate matches (the task whose CVillageTaskBase is pred.Task)
// Leaf: ??$remove_if@VCIsTask@@@?$list@V?$CCountedPointer@VCVillageTaskSecuritySweep@@@@V?$allocator@V?$CCountedPointer@VCVillageTaskSecuritySweep@@@@@std@@@std@@QAEXVCIsTask@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x006602f0, 88 bytes, thiscall, `ret 4`; size-optimised TU with an ebp frame.
// Identity (proven): bound under this name in reloc_bindings (Ego_r.pdb holds the same instantiations); callers the landed
//   CTCVillage::SetSecuritySweepTaskAsCompleted 0x0065A031 (CTCVillage's task list) and the tail jump in RemoveThingFromTask 0x00660D49.
// Callees: CCountedPointer<CVillageTaskBase>::CCountedPointer<CVillageTaskSecuritySweep>(const CCountedPointer<CVillageTaskSecuritySweep>&) 0x0065DB75, ~CCountedPointer
//   <CVillageTaskBase> 0x0065DA6B, list<CCountedPointer<CVillageTaskSecuritySweep> >::erase(iterator) 0x006601BF.
// Shape (family L027): STLport 4.6 _list.c remove_if verbatim under `#pragma optimize("s", on)`; CIsTask::operator() takes
//   const CCountedPointer<CVillageTaskBase>& (Ego_r.pdb CIsTask {CVillageTaskBase* Task}) so each element is converted into a
//   temporary, compared (`sete` into a frame byte) and released; erase's iterator is built in its argument slot.
#pragma optimize("s", on)
class CBaseClass {
public:
    virtual ~CBaseClass();
};
class CBaseObjectPointer;
class CBaseObject : public CBaseClass {
public:
    CBaseObjectPointer* IntelligentPointer;                 // +0x04
};
class CVillageTaskBase : public CBaseObject { };            // partial view
class CCPPointerInfo;
template <class T> class CCountedPointer {
public:
    T*              Data;                                   // +0x00
    CCPPointerInfo* Info;                                   // +0x04
    template <class U> CCountedPointer(const CCountedPointer<U>& other);   // converting copy (out of line)
    ~CCountedPointer();
};
class CIsTask {                                             // Ego_r.pdb 4 bytes
public:
    bool operator()(const CCountedPointer<CVillageTaskBase>& task) const { return task.Data == Task; }
    CVillageTaskBase* Task;                                 // +0x00
};
namespace std {
template <class _Tp> class allocator { };
template <class _Value, class _Tp, class _MaybeReboundAlloc> struct _STLP_alloc_proxy {
    _Value _M_data;
};
struct _List_node_base {
    _List_node_base* _M_next;                               // +0x00
    _List_node_base* _M_prev;                               // +0x04
};
template <class _Tp> struct _List_node : public _List_node_base {
    _Tp _M_data;                                            // +0x08
};
template <class _Tp> struct _Nonconst_traits { };
template <class _Tp, class _Traits> struct _List_iterator {
    _List_node_base* _M_node;
    _List_iterator(_List_node_base* __x) : _M_node(__x) { }
    _List_iterator(const _List_iterator& __x) : _M_node(__x._M_node) { }   // STLport: user copy constructor
    _Tp& operator*() const { return ((_List_node<_Tp>*)_M_node)->_M_data; }
    _List_iterator& operator++() { _M_node = _M_node->_M_next; return *this; }
    bool operator!=(const _List_iterator& __y) const { return _M_node != __y._M_node; }
};
template <class _Tp, class _Alloc = allocator<_Tp> >
class list {                                                // retail STLport list (4 bytes)
public:
    typedef _List_iterator<_Tp, _Nonconst_traits<_Tp> > iterator;
    _STLP_alloc_proxy<_List_node_base*, _List_node_base, _Alloc> _M_node;
    iterator begin() { return iterator(_M_node._M_data->_M_next); }
    iterator end() { return iterator(_M_node._M_data); }
    iterator erase(iterator __position);                    // out of line
    template <class _Predicate> void remove_if(_Predicate __pred);
};
template <class _Tp, class _Alloc> template <class _Predicate>
void list<_Tp, _Alloc>::remove_if(_Predicate __pred)
{
    iterator __first = begin();
    iterator __last = end();
    while (__first != __last) {
        iterator __next = __first;
        ++__next;
        if (__pred(*__first))
            erase(__first);
        __first = __next;
    }
}
}   // namespace std
class CVillageTaskSecuritySweep : public CVillageTaskBase { };   // partial view
template void std::list<CCountedPointer<CVillageTaskSecuritySweep> >::remove_if<CIsTask>(CIsTask);