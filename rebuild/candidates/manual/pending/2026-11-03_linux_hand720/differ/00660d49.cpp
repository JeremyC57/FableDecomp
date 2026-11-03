// CTCVillage::RemoveThingFromTask<CVillageTaskCarryCrate>(CIntelligentPointer<CVillageTaskBase> task, const CThing& thing, list<CCountedPointer<CVillageTaskCarryCrate> >& tasks) const
// Leaf: ??$RemoveThingFromTask@VCVillageTaskCarryCrate@@@CTCVillage@@ABEXV?$CIntelligentPointer@VCVillageTaskBase@@@@ABVCThing@@AAV?$list@V?$CCountedPointer@VCVillageTaskCarryCrate@@@@V?$allocator@V?$CCountedPointer@VCVillageTaskCarryCrate@@@@@std@@@std@@@Z
// Grade: DIFFER(123v123) -- one operand: the discarded erase() result temporary is homed at [ebp+14h] (the dead `tasks` argument slot) where retail reuses [ebp+10h] (the find_if result slot); 31 probes: iterator declaration/assignment forms, iterator default/copy ctors, /Gr, optimize letters, same-TU erase and find_if bodies (alone and together, with both the named-iterator and the assign-to-default-iterator forms) moved nothing else.
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00660D49, 123 bytes, ebp frame, `ret 10h`; T = CVillageTaskCarryCrate (proven by its callees: list<CCountedPointer<CVillageTaskCarryCrate> >::erase 0x006601E9, find_if<..., CIsTask> 0x0065FAB2, and the dispatcher 0x00659EFC that passes the matching task list).
// Name: Ego_r.pdb/FableWin.pdb ??$RemoveThingFromTask@VCVillageTaskCarryCrate@@@CTCVillage@@ABEXV?$CIntelligentPointer@VCVillageTaskBase@@@@ABVCThing@@AAV?$list@...@std@@@Z (private const member template, six instantiations; the manifest names all six "RemoveThingFromTask<CVillageTaskPickCrops>").
#pragma optimize("y", off)
#pragma optimize("s", on)
class CThing;
class CCPPointerInfo;
#define FABLE_RETAIL_BASE_OBJECT
#include "engine/CBaseObject.h"
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"
class CVillageTaskBase : public CBaseObject {
public:
    virtual long GetTaskType() const;
    virtual bool IsValid() const;
    virtual bool IsTaskAvailable() const;
    virtual bool IsThingInvolvedWithTask(const CThing& thing) const;
    virtual void RemoveThingFromTask(const CThing& thing, bool& remove_task);
};
class CVillageTaskCarryCrate : public CVillageTaskBase { };
template <class T> class CCountedPointer {
public:
    T*              Data;
    CCPPointerInfo* Info;
};
class CIsTask {
public:
    CVillageTaskBase* Task;
    CIsTask(const CVillageTaskBase& task) : Task((CVillageTaskBase*)&task) { }
    bool operator()(const CCountedPointer<CVillageTaskBase>& p) const;
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
};
template <class _Tp> struct _Nonconst_traits { typedef _Tp value_type; typedef _Tp* pointer; typedef _Tp& reference; };
template <class _Tp, class _Traits> struct _List_iterator : public _List_iterator_base {
    typedef _List_iterator<_Tp, _Nonconst_traits<_Tp> > iterator;
    typedef _List_node<_Tp> _Node;
    _List_iterator(_Node* __x) : _List_iterator_base(__x) { }
    _List_iterator(const iterator& __x) : _List_iterator_base(__x._M_node) { }
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
    iterator erase(iterator __position);
};
template <class _InputIter, class _Predicate>
_InputIter __fastcall find_if(_InputIter __first, _InputIter __last, _Predicate __pred);
}
class CTCVillage {
private:
    template <class T> void RemoveThingFromTask(CIntelligentPointer<CVillageTaskBase> task, const CThing& thing,
                                                std::list<CCountedPointer<T> >& tasks) const;
};
template <class T>
void CTCVillage::RemoveThingFromTask(CIntelligentPointer<CVillageTaskBase> task, const CThing& thing,
                                     std::list<CCountedPointer<T> >& tasks) const
{
    if (task->IsThingInvolvedWithTask(thing))
    {
        bool remove_task;
        task->RemoveThingFromTask(thing, remove_task);
        if (remove_task)
        {
            typename std::list<CCountedPointer<T> >::iterator it = std::find_if(tasks.begin(), tasks.end(), CIsTask(*task.Get()));
            tasks.erase(it);
        }
    }
}
template void CTCVillage::RemoveThingFromTask<CVillageTaskCarryCrate>(CIntelligentPointer<CVillageTaskBase>, const CThing&, std::list<CCountedPointer<CVillageTaskCarryCrate> >&) const;
