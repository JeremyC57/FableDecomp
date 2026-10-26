// CTCVillage::RemoveThingFromTask(CIntelligentPointer<CVillageTaskBase> task, const CThing& thing, std::list<CCountedPointer<CVillageTaskPickCrops> >& tasks) const -- drop the thing from one task and erase that task from the list when it asks to be removed
// Grade: DIFFER(123v123) -- erase's hidden iterator result is homed at [ebp+14h] (the dead list slot) where retail reuses [ebp+10h], the find_if result's slot
// Leaf: ??$RemoveThingFromTask@VCVillageTaskPickCrops@@@CTCVillage@@ABEXV?$CIntelligentPointer@VCVillageTaskBase@@@@ABVCThing@@AAV?$list@V?$CCountedPointer@VCVillageTaskPickCrops@@@@V?$allocator@V?$CCountedPointer@VCVillageTaskPickCrops@@@@@std@@@std@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00660C53, 123 bytes (unaligned, frame pointer: a size-optimised TU with
//   optimize("y", off)), private const __thiscall member template, `ret 10h` (the 8-byte CIntelligentPointer by value, the
//   thing, the list); `this` is never read; the by-value task is destroyed by the callee (~CBaseIntelligentPointer).
// Identity (proven): CTCVillage::RemoveThingFromTask<CVillageTaskPickCrops> (Ego_r.pdb name set); evidence: (1) the list::erase it calls,
//   0x00660195, is the landed list<CCountedPointer<CVillageTaskPickCrops> >::erase; (2) its find_if 0x0065FA5E forwards to __find_if 0x0065EBE4,
//   agent952's H094 std::__find_if over the CVillageTaskPickCrops list; (3) CTCVillage::RemoveThingFromTask(CIntelligentPointer<CVillageTaskBase>,
//   const CThing&) 0x00659EFC calls it with `this+0x58`, the retail 4-byte list CTCVillage::CropTasks.
// Label: the manifest's "RemoveThingFromTask<CVillageTaskPickCrops>" is propagated across the six byte-identical bodies (family J003).
// Callees: CBaseIntelligentPointer::GetPItem() const 0x00A01B50 (three times: CIntelligentPointer::operator-> / operator*),
//   std::find_if<list<CCountedPointer<CVillageTaskPickCrops> >::iterator, CIsTask> 0x0065FA5E (__fastcall, hidden iterator result in ecx; the landed
//   0x0065FA5E row is mislabelled -- see Label corrections), std::list<CCountedPointer<CVillageTaskPickCrops> >::erase(iterator) 0x00660195,
//   CBaseIntelligentPointer::~CBaseIntelligentPointer() 0x00A01C10 (the by-value parameter); CVillageTaskBase virtual slots 5
//   IsThingInvolvedWithTask (+0x14) and 6 RemoveThingFromTask (+0x18).
// Shape: `if (task->IsThingInvolvedWithTask(thing)) { bool remove; task->RemoveThingFromTask(thing, remove); if (remove) {
//   iterator it = find_if(tasks.begin(), tasks.end(), CIsTask(*task)); tasks.erase(it); } }` -- CIsTask {Task} pushed by value,
//   both iterators built in their argument slots, find_if's result homed in the dead `thing` slot [ebp+10h].
#pragma optimize("y", off)
#pragma optimize("s", on)
class CThing;
struct CCPPointerInfo;
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
    virtual bool IsThingInvolvedWithTask(const CThing& thing) const;
    virtual void RemoveThingFromTask(const CThing& thing, bool& remove_task);
};
class CVillageTaskPickCrops : public CVillageTaskBase { };
class CBaseIntelligentPointer {
public:
    virtual ~CBaseIntelligentPointer();                      // 0x00A01C10
    CBaseObject* GetPItem() const;                           // 0x00A01B50
    void* PItemInfo;
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    T* operator->() const { return (T*)GetPItem(); }
    T& operator*() const { return *(T*)GetPItem(); }
};
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
    iterator erase(iterator __position);                    // 0x00660195
};
template <class _InputIter, class _Predicate>
_InputIter __fastcall find_if(_InputIter __first, _InputIter __last, _Predicate __pred);   // 0x0065FA5E
}
class CTCVillage {
private:
    template <class T> void RemoveThingFromTask(CIntelligentPointer<CVillageTaskBase> task, const CThing& thing, std::list<CCountedPointer<T> >& tasks) const;
};
template <class T>
void CTCVillage::RemoveThingFromTask(CIntelligentPointer<CVillageTaskBase> task, const CThing& thing, std::list<CCountedPointer<T> >& tasks) const
{
    if (task->IsThingInvolvedWithTask(thing))
    {
        bool remove_task;
        task->RemoveThingFromTask(thing, remove_task);
        if (remove_task)
        {
            typename std::list<CCountedPointer<T> >::iterator it = std::find_if(tasks.begin(), tasks.end(), CIsTask(*task));
            tasks.erase(it);
        }
    }
}
template void CTCVillage::RemoveThingFromTask<CVillageTaskPickCrops>(CIntelligentPointer<CVillageTaskBase>, const CThing&, std::list<CCountedPointer<CVillageTaskPickCrops> >&) const;
