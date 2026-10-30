// std::list::remove_if(_Predicate pred) -- (list<CCountedPointer<CVillageTaskSecuritySweep> >, CIsNotValid<CVillageTaskSecuritySweep>) STLport list::remove_if: erase every task whose IsValid() is false
// Leaf: ??$remove_if@V?$CIsNotValid@VCVillageTaskSecuritySweep@@@@@?$list@V?$CCountedPointer@VCVillageTaskSecuritySweep@@@@V?$allocator@V?$CCountedPointer@VCVillageTaskSecuritySweep@@@@@std@@@std@@QAEXV?$CIsNotValid@VCVillageTaskSecuritySweep@@@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00660494, 61 bytes (size-optimised TU, no padding), thiscall, `ret 4` (the empty
//   predicate by value).
// Identity (proven): std::list<CCountedPointer<CVillageTaskSecuritySweep> >::remove_if<CIsNotValid<CVillageTaskSecuritySweep> >(CIsNotValid<CVillageTaskSecuritySweep>); evidence:
//   it erases through 0x006601BF = list<CCountedPointer<CVillageTaskSecuritySweep> >::erase(iterator) (agent952 H020, proven per list type);
//   its caller 0x006610B5 is CTCVillage::ClearOutInvalidTasks<CVillageTaskSecuritySweep>(list&) (FableWin.pdb ??$ClearOutInvalidTasks@VCVillageTaskSecuritySweep@@@CTCVillage@@ABEX...),
//   which CTCVillage::ClearOutInvalidTasks() 0x0065BE32 (Ego_r.pdb ?ClearOutInvalidTasks@CTCVillage@@AAEXXZ) calls at 0x0065BE65
//   with this+0x5C = engine/CTCVillage.h SecuritySweepTasks; the predicate tests the task's virtual IsValid (slot 3, +0x0C);
//   FableWin.pdb ??$remove_if@V?$CIsNotValid@VCVillageTaskSecuritySweep@@@@@?$list@V?$CCountedPointer@VCVillageTaskSecuritySweep@@@@...@std@@QAEXV?$CIsNotValid@VCVillageTaskSecuritySweep@@@@@Z.
// Label: the manifest's "remove_if<CIsNotValid<CVillageTaskHorn>_>" is propagated (family J107: five byte-identical remove_if bodies; retail has no identical-code
//   folding).
// Callees: list<CCountedPointer<CVillageTaskSecuritySweep> >::erase(iterator) 0x006601BF (agent952's spelling: STLport _List_iterator<T,
//   _Nonconst_traits<T> >), the task's virtual IsValid() const (slot 3, +0x0C; Ego_r.pdb ?IsValid@CVillageTaskSecuritySweep@@UBE_NXZ; RTTI-port
//   labels put IsValid at slot 3 of every CVillageTask vtable).
// Shape: the STLport 4.6 remove_if loop: `first = begin(); last = end(); while (first != last) { next = first; ++next; if
//   (pred(*first)) erase(first); first = next; }`; CIsNotValid<T>::operator()(const CCountedPointer<T>&) const (Ego_r.pdb ??R...
//   QBE_NABV?$CCountedPointer@...) is `!task->IsValid()`. Size-optimised TU: `#pragma optimize("s", on)` (J105).
// Layout: STLport list (4 bytes: the sentinel pointer), node {next, prev, CCountedPointer<T> {Data, Info}}; the task chain is a
//   local partial view (RTTI CVillageTaskSecuritySweep <- CVillageTaskBase <- CBaseObject <- CBaseClass).
#pragma optimize("s", on)
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0
class CBaseObject : public CBaseClass {
public:
    virtual void Validate() const;                          // slot 1
};
enum EVillageTaskType { EVillageTaskType_Force32 = 0x7fffffff };   // enumerators not needed
class CVillageTaskBase : public CBaseObject {
public:
    virtual EVillageTaskType GetTaskType() const = 0;       // slot 2
    virtual bool IsValid() const = 0;                       // slot 3 (+0x0C)
};
class CVillageTaskSecuritySweep : public CVillageTaskBase {   // partial view
public:
    virtual EVillageTaskType GetTaskType() const;
    virtual bool IsValid() const;
};
class CCPPointerInfo;
template <class T> class CCountedPointer {               // 8 bytes: the object, the shared count block
public:
    ~CCountedPointer();                                  // out of line
    T* operator->() const { return Data; }
    T*              Data;                                // +0x0
    CCPPointerInfo* Info;                                // +0x4
};
template <class T> class CIsNotValid {
public:
    bool operator()(const CCountedPointer<T>& task) const { return !task->IsValid(); }
};

namespace std {
template <class _Tp> class allocator { };
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};
struct _List_node_base {
    _List_node_base* _M_next;                              // +0x0
    _List_node_base* _M_prev;                              // +0x4
};
template <class _Tp> struct _List_node : public _List_node_base {
    _Tp _M_data;                                           // +0x8
};
struct _List_iterator_base {
    _List_node_base* _M_node;
    _List_iterator_base(_List_node_base* __x) : _M_node(__x) { }
    _List_iterator_base() { }
    void _M_incr() { _M_node = _M_node->_M_next; }
    bool operator!=(const _List_iterator_base& __y) const { return _M_node != __y._M_node; }
};
template <class _Tp> struct _Nonconst_traits { typedef _Tp value_type; typedef _Tp* pointer; typedef _Tp& reference; };
template <class _Tp, class _Traits>
struct _List_iterator : public _List_iterator_base {
    typedef _List_iterator<_Tp, _Nonconst_traits<_Tp> > iterator;
    typedef _List_iterator<_Tp, _Traits> _Self;
    typedef typename _Traits::reference reference;
    typedef _List_node<_Tp> _Node;
    _List_iterator(_Node* __x) : _List_iterator_base(__x) { }
    _List_iterator() { }
    _List_iterator(const iterator& __x) : _List_iterator_base(__x._M_node) { }
    reference operator*() const { return ((_Node*)_M_node)->_M_data; }
    _Self& operator++() { this->_M_incr(); return *this; }
};
template <class _Tp, class _Alloc>
class _List_base {
public:
    typedef _List_node<_Tp> _Node;
    typedef _STLP_alloc_proxy<_Node*, _Node, allocator<_Node> > _AllocProxy;
    _AllocProxy _M_node;                                   // +0x0: the sentinel node
};
template <class _Tp, class _Alloc = allocator<_Tp> >
class list : public _List_base<_Tp, _Alloc> {
public:
    typedef _List_node<_Tp> _Node;
    typedef _List_iterator<_Tp, _Nonconst_traits<_Tp> > iterator;
    iterator begin() { return iterator((_Node*)(this->_M_node._M_data->_M_next)); }
    iterator end() { return iterator(this->_M_node._M_data); }
    iterator erase(iterator __position);                   // out of line
    template <class _Predicate> void remove_if(_Predicate __pred)
    {
        iterator __first = begin();
        iterator __last = end();
        while (__first != __last) {
            iterator __next = __first;
            ++__next;
            if (__pred(*__first)) erase(__first);
            __first = __next;
        }
    }
};
template void list<CCountedPointer<CVillageTaskSecuritySweep> >::remove_if(CIsNotValid<CVillageTaskSecuritySweep>);
}   // namespace std