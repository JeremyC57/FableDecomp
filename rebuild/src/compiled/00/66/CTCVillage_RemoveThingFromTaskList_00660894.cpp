// CTCVillage::RemoveThingFromTaskList(const CThing& thing, std::list<CCountedPointer<CVillageTaskHorn> >& tasks) const -- drop the thing from every task of the list that involves it, erasing each task that asks to be removed
// Leaf: ??$RemoveThingFromTaskList@VCVillageTaskHorn@@@CTCVillage@@ABEXABVCThing@@AAV?$list@V?$CCountedPointer@VCVillageTaskHorn@@@@V?$allocator@V?$CCountedPointer@VCVillageTaskHorn@@@@@std@@@std@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00660894, 118 bytes (unaligned, frame pointer: a size-optimised TU with
//   optimize("y", off)), private const __fastcall member template, `ret 8`; `this` is never read.
// Identity (proven): CTCVillage::RemoveThingFromTaskList<CVillageTaskHorn> (Ego_r.pdb name set); evidence: (1) the list::erase it calls,
//   0x00660141, is the landed list<CCountedPointer<CVillageTaskHorn> >::erase; (2) its converting copy 0x0065DB2A is catalog-bound as
//   CCountedPointer<CVillageTaskBase>::CCountedPointer<CVillageTaskHorn>; (3) CTCVillage::RemoveThingFromAllTasks 0x00659EA9 calls it with
//   `this+0x50`, the retail 4-byte list CTCVillage::HornTasks.
// Label: the manifest's "RemoveThingFromTaskList<CVillageTaskTorch>" is propagated across the six byte-identical bodies (family J002; retail has no
//   identical-code folding, so each address is its own instantiation).
// Callees: CCountedPointer<CVillageTaskBase>::CCountedPointer<CVillageTaskHorn>(const CCountedPointer<CVillageTaskHorn>&) 0x0065DB2A,
//   CCountedPointer<CVillageTaskBase>::~CCountedPointer() 0x0065DA6B, std::list<CCountedPointer<CVillageTaskHorn> >::erase(iterator) 0x00660141;
//   CVillageTaskBase virtuals through the vtable: slot 5 IsThingInvolvedWithTask(const CThing&) const (+0x14), slot 6
//   RemoveThingFromTask(const CThing&, bool&) (+0x18) (CVillageTaskTorch vtable 0x0125A8AC: 0x00860310 / 0x008605B0).
// Shape: begin, cached end ([ebp-8]) and the `next` cursor declared before the loop (`mov edi,esi` ahead of the entry test,
//   `++next` at the top, `it = next` at the bottom); the CCountedPointer<CVillageTaskBase> conversion is a NAMED local in its
//   own block (retail reads the temporary's Data from the frame, `mov ecx,[ebp-14h]`, not from the ctor's eax) and the
//   involved flag lives outside the block ([ebp-1]); the erase result iterator is discarded ([ebp-0Ch]). Grade 1 (unnamed
//   temporary in the if, next = it inside the loop) was DIFFER(115v118). Local classes: engine/CTCVillage.h carries the
//   Ego_r layout (12-byte lists) -- header debt.
#pragma optimize("y", off)
#pragma optimize("s", on)
class CThing;
class CCPPointerInfo;
#define FABLE_RETAIL_BASE_OBJECT
#include "engine/CBaseObject.h"                       // : CBaseClass, vtable 0x0129A7D8 (~ slot 0, Validate slot 1)
class CVillageTaskBase : public CBaseObject {
public:
    virtual long GetTaskType() const;
    virtual bool IsValid() const;
    virtual bool IsTaskAvailable() const;
    virtual bool IsThingInvolvedWithTask(const CThing& thing) const;
    virtual void RemoveThingFromTask(const CThing& thing, bool& remove_task);
};
class CVillageTaskHorn : public CVillageTaskBase { };
template <class T> class CCountedPointer {
public:
    T*              Data;
    CCPPointerInfo* Info;
    template <class U> CCountedPointer(const CCountedPointer<U>& other);   // converting copy, out of line (0x0065DB2A for U = CVillageTaskHorn)
    ~CCountedPointer();                                      // out of line (0x0065DA6B for CVillageTaskBase)
    T* operator->() const { return Data; }
};
namespace std {
template <class _Tp> class allocator { public: allocator() { } };
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc { public: _Value _M_data; };
struct _List_node_base {
    _List_node_base* _M_next;
    _List_node_base* _M_prev;
};
template <class _Tp> struct _List_node : public _List_node_base { _Tp _M_data; };
struct _List_iterator_base {
    _List_node_base* _M_node;
    _List_iterator_base(_List_node_base* __x) : _M_node(__x) { }
    void _M_incr() { _M_node = _M_node->_M_next; }
    bool operator!=(const _List_iterator_base& __y) const { return _M_node != __y._M_node; }
};
template <class _Tp> struct _Nonconst_traits { typedef _Tp value_type; typedef _Tp* pointer; typedef _Tp& reference; };
template <class _Tp, class _Traits> struct _List_iterator : public _List_iterator_base {
    typedef _List_iterator<_Tp, _Nonconst_traits<_Tp> > iterator;
    typedef _List_iterator<_Tp, _Traits> _Self;
    typedef _List_node<_Tp> _Node;
    _List_iterator(_Node* __x) : _List_iterator_base(__x) { }
    _List_iterator(const iterator& __x) : _List_iterator_base(__x._M_node) { }
    _Tp& operator*() const { return ((_Node*)_M_node)->_M_data; }
    _Self& operator++() { this->_M_incr(); return *this; }
};
template <class _Tp, class _Alloc> class _List_base {
public:
    typedef _List_node<_Tp> _Node;
    _STLP_alloc_proxy<_Node*, _Node, allocator<_Node> > _M_node;
};
template <class _Tp, class _Alloc = allocator<_Tp> > class list : public _List_base<_Tp, _Alloc> {
public:
    typedef _List_node<_Tp> _Node;
    typedef _List_iterator<_Tp, _Nonconst_traits<_Tp> > iterator;
    iterator begin() { return iterator((_Node*)(this->_M_node._M_data->_M_next)); }
    iterator end() { return iterator(this->_M_node._M_data); }
    iterator erase(iterator __position);                    // 0x00660141
};
}
class CTCVillage {
private:
    template <class T> void RemoveThingFromTaskList(const CThing& thing, std::list<CCountedPointer<T> >& tasks) const;
};
template <class T>
void CTCVillage::RemoveThingFromTaskList(const CThing& thing, std::list<CCountedPointer<T> >& tasks) const
{
    typename std::list<CCountedPointer<T> >::iterator it = tasks.begin();
    typename std::list<CCountedPointer<T> >::iterator end = tasks.end();
    typename std::list<CCountedPointer<T> >::iterator next = it;
    while (it != end)
    {
        ++next;
        bool involved;
        {
            CCountedPointer<CVillageTaskBase> task(*it);
            involved = task->IsThingInvolvedWithTask(thing);
        }
        if (involved)
        {
            bool remove_task;
            (*it)->RemoveThingFromTask(thing, remove_task);
            if (remove_task)
                tasks.erase(it);
        }
        it = next;
    }
}
template void CTCVillage::RemoveThingFromTaskList<CVillageTaskHorn>(const CThing&, std::list<CCountedPointer<CVillageTaskHorn> >&) const;