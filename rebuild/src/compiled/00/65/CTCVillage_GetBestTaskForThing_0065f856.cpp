// CTCVillage::GetBestTaskForThing(const CThing& thing, std::list<CCountedPointer<CVillageTaskPatrol> >& tasks) const -- run the best-task functor over the task list and return the best task it found
// Leaf: ??$GetBestTaskForThing@VCVillageTaskPatrol@@@CTCVillage@@ABEPAVCVillageTaskPatrol@@ABVCThing@@AAV?$list@V?$CCountedPointer@VCVillageTaskPatrol@@@@V?$allocator@V?$CCountedPointer@VCVillageTaskPatrol@@@@@std@@@std@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0065F856, 46 bytes (unaligned, frame pointer: a size-optimised TU with
//   optimize("y", off)), private const __fastcall member template, `ret 8`; `this` is never read.
// Identity (proven): CTCVillage::GetBestTaskForThing<CVillageTaskPatrol> (Ego_r.pdb name set:
//   ??$GetBestTaskForThing@VCVillageTaskPatrol@@@CTCVillage@@ABE...); evidence: (1) its only callee is 0x0065E854, the
//   std::for_each over list<CCountedPointer<CVillageTaskPatrol> > with CGetBestTaskForThing<CVillageTaskPatrol> (agent952 family H093, whose own callee is
//   that functor's operator()); (2) its only caller 0x00658E90 passes `this+0x54`, the retail 4-byte list
//   CTCVillage::PatrolTasks (Ego_r.pdb order TorchTasks, HornTasks, PatrolTasks, CropTasks, SecuritySweepTasks, CrateTasks
//   from +0x4C).
// Label: the manifest's "GetBestCarryCrateTask" is propagated across the six byte-identical bodies (family J001; retail has no
//   identical-code folding, so each address is its own instantiation).
// Callees: std::for_each<list<CCountedPointer<CVillageTaskPatrol> >::iterator, CGetBestTaskForThing<CVillageTaskPatrol> > 0x0065E854 (__fastcall: hidden
//   functor result in ecx, both iterators and the 8-byte functor on the stack).
// Shape: `return for_each(tasks.begin(), tasks.end(), CGetBestTaskForThing<T>(thing)).PBestTask;` -- the functor {Thing,
//   PBestTask = 0} is pushed by value (no user copy constructor), the iterators have STLport's user copy constructor so each is
//   built in its argument slot (`push ecx ; mov ecx,esp ; mov [ecx],reg`), the returned functor lands in the 8-byte frame
//   local and +4 is read back. STLport 4.6 list (4 bytes: the _STLP_alloc_proxy sentinel pointer). CTCVillage is a local
//   declaration: engine/CTCVillage.h carries the Ego_r layout (12-byte lists) -- header debt.
#pragma optimize("y", off)
#pragma optimize("s", on)
class CThing;
class CVillageTaskPatrol;
class CCPPointerInfo;
template <class T> class CCountedPointer {
public:
    T*              Data;                                    // +0x00
    CCPPointerInfo* Info;                                    // +0x04
};
template <class T> class CGetBestTaskForThing {
public:
    const CThing* Thing;                                     // +0x00
    T*            PBestTask;                                 // +0x04
    CGetBestTaskForThing(const CThing& thing) : Thing(&thing), PBestTask(0) { }
    void operator()(const CCountedPointer<T>& task);         // out of line
};
namespace std {
template <class _Tp> class allocator { public: allocator() { } };
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc { public: _Value _M_data; };
struct _List_node_base {
    _List_node_base* _M_next;                                // +0x0
    _List_node_base* _M_prev;                                // +0x4
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
    _STLP_alloc_proxy<_Node*, _Node, allocator<_Node> > _M_node;   // +0x0: the sentinel node
};
template <class _Tp, class _Alloc = allocator<_Tp> > class list : public _List_base<_Tp, _Alloc> {
public:
    typedef _List_node<_Tp> _Node;
    typedef _List_iterator<_Tp, _Nonconst_traits<_Tp> > iterator;
    iterator begin() { return iterator((_Node*)(this->_M_node._M_data->_M_next)); }
    iterator end() { return iterator(this->_M_node._M_data); }
};
template <class _InputIter, class _Function>
_Function __fastcall for_each(_InputIter __first, _InputIter __last, _Function __f);   // 0x0065E854
}
class CTCVillage {
private:
    template <class T> T* GetBestTaskForThing(const CThing& thing, std::list<CCountedPointer<T> >& tasks) const;
};
template <class T>
T* CTCVillage::GetBestTaskForThing(const CThing& thing, std::list<CCountedPointer<T> >& tasks) const
{
    return std::for_each(tasks.begin(), tasks.end(), CGetBestTaskForThing<T>(thing)).PBestTask;
}
template CVillageTaskPatrol* CTCVillage::GetBestTaskForThing<CVillageTaskPatrol>(const CThing&, std::list<CCountedPointer<CVillageTaskPatrol> >&) const;