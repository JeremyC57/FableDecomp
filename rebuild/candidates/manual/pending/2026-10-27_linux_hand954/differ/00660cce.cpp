// CTCVillage::RemoveThingFromTask<CVillageTaskSecuritySweep>(CIntelligentPointer<CVillageTaskBase> task, const CThing& thing, std::list<CCountedPointer<CVillageTaskSecuritySweep> >& tasks) const -- if the task involves the thing, drop the thing from it; when the task then asks to be removed, erase it from its list
// Grade: DIFFER(123v123) -- RemoveThingFromTask<T>: erase's discarded result temp homed in the tasks slot (+14h) where retail reuses the find_if temp's thing slot (+10h); 8 forms tried
// Leaf: ??$RemoveThingFromTask@VCVillageTaskSecuritySweep@@@CTCVillage@@QBEXV?$CIntelligentPointer@VCVillageTaskBase@@@@ABVCThing@@AAV?$list@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00660CCE, 123 bytes, thiscall `ret 10h` (`this` unread), frame pointer (size TU); destroys the
//   by-value task pointer.
// Identity (proven): reloc_bindings ??$RemoveThingFromTask@VCVillageTaskSecuritySweep@@@CTCVillage@@QBEXV?$CIntelligentPointer@VCVillageTaskBase@@@@ABVCThing@@...
//   (whose list parameter is the landed caller's CTaskList stand-in); called by the landed dispatcher
//   CTCVillage::RemoveThingFromTask 0x00659EFC on SecuritySweepTasks +0x5C; its erase 0x006601BF is the bound list<CCountedPointer<CVillageTaskSecuritySweep> >::erase and its
//   find_if 0x0065FA88 takes the 4-byte CIsTask predicate (FableWin.pdb find_if<..., CIsTask> per task type; Ego_r.pdb CIsTask::Task).
// Callees: CBaseIntelligentPointer::GetPItem 0x00A01B50, CVillageTaskBase slot 5 IsThingInvolvedWithTask (+0x14) and slot 6
//   RemoveThingFromTask(const CThing&, bool&) (+0x18), std::find_if<..., CIsTask> 0x0065FA88 (__fastcall, iterator returned through
//   the hidden pointer homed in the dead `thing` slot), list::erase(iterator) 0x006601BF, ~CBaseIntelligentPointer 0x00A01C10.
// Shape (family K005, representative 0x00660AE2): frame/size TU (`push dword ptr [ebp+10h]` pass-through, `leave`); the remove flag
//   is the push-ecx byte ([ebp-1]); the find_if arguments evaluated right to left (predicate, end, begin); the erase result discarded.
#pragma optimize("y", off)
#pragma optimize("s", on)
class CThing;
class CCPPointerInfo;
class CBaseObject {
public:
    virtual ~CBaseObject();
    virtual void Validate();
};
class CVillageTaskBase : public CBaseObject {
public:
    virtual long GetTaskType() const;
    virtual bool IsValid() const;
    virtual bool IsTaskAvailable() const;
    virtual bool IsThingInvolvedWithTask(const CThing& thing) const;   // slot 5
    virtual void RemoveThingFromTask(const CThing& thing, bool& remove_task);   // slot 6
};
class CVillageTaskSecuritySweep : public CVillageTaskBase { };
class CBaseObjectPointer;
class CBaseIntelligentPointer {                       // Ego_r: vptr + PData, 8 bytes
public:
    CBaseIntelligentPointer();                           // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                  // 0x00A01C10
    CBaseObject* GetPItem() const;                       // 0x00A01B50
private:
    CBaseObjectPointer* PData;                           // +0x04
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
};
template <class T> class CCountedPointer {
public:
    T*              Data;
    CCPPointerInfo* Info;
};
class CIsTask {                                          // Ego_r.pdb 4 bytes: the task searched for
public:
    CVillageTaskBase* Task;                              // +0x00
    CIsTask(CVillageTaskBase* task) : Task(task) { }
    bool operator()(const CCountedPointer<CVillageTaskBase>& p) const { return (CVillageTaskBase*)p.Data == Task; }
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
    iterator end() { return iterator((_Node*)this->_M_node._M_data); }
    iterator erase(iterator __position);                    // 0x006601BF
};
template <class _InputIter, class _Predicate>
_InputIter __fastcall find_if(_InputIter __first, _InputIter __last, _Predicate __pred);   // 0x0065FA88
}
class CTCVillage {
public:
    template <class T> void RemoveThingFromTask(CIntelligentPointer<CVillageTaskBase> task, const CThing& thing,
                                                std::list<CCountedPointer<T> >& tasks) const;
};
template <class T>
void CTCVillage::RemoveThingFromTask(CIntelligentPointer<CVillageTaskBase> task, const CThing& thing,
                                     std::list<CCountedPointer<T> >& tasks) const
{
    if (task.GetPItem()->IsThingInvolvedWithTask(thing)) {
        bool remove_task;
        task.GetPItem()->RemoveThingFromTask(thing, remove_task);
        if (remove_task) {
            typename std::list<CCountedPointer<T> >::iterator it = std::find_if(tasks.begin(), tasks.end(), CIsTask(task.GetPItem()));
            tasks.erase(it);
        }
    }
}
template void CTCVillage::RemoveThingFromTask<CVillageTaskSecuritySweep>(CIntelligentPointer<CVillageTaskBase>, const CThing&,
                                                       std::list<CCountedPointer<CVillageTaskSecuritySweep> >&) const;
