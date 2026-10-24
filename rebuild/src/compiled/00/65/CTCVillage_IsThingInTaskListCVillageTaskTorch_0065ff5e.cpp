// CTCVillage::IsThingInTaskList<CVillageTaskTorch>(const CThing& thing, list<CCountedPointer<CVillageTaskTorch> >& tasks) -- true when some task in the list involves the thing
// Leaf: ??$IsThingInTaskList@
// Retail: Fable.exe (TLC, VC7.1) @ 0x0065FF5E, 48 bytes, thiscall `ret 8` (`this` unused). Name: Ego_r.pdb
// `??$IsThingInTaskList@VCVillageTaskTorch@@@CTCVillage@@ABE_NABVCThing@@AAV?$list@...` (name set; private const member
// template). Callers: CTCVillage::UpdateTorchTasks at 0x00659717 / 0x0065978E / 0x006597A5 with `this` in ecx and
// &TorchTasks (retail +0x4C) as the list.
// Callees: std::find_if<list<CCountedPointer<CVillageTaskTorch>>::iterator, CIsThingInvolvedWithTask> @ 0x0065F943
// (`__fastcall`: hidden result in ecx -- the dead list-argument slot [ebp+0Ch] -- iterators and functor on the stack).
// Label correction: the catalog "CPersistTraits_IsModified" is propagated.
// Shape: `return find_if(tasks.begin(), tasks.end(), CIsThingInvolvedWithTask(thing)) != tasks.end();` -- right to
// left: the functor (the thing reference, `push [ebp+8]`), end() (the sentinel, kept in esi for the compare), begin()
// (sentinel->next), each iterator copy-built in its slot; `xor ecx,ecx ; cmp ; setne cl ; mov al,cl` for the bool.
// Opt: frame-pointer size TU: `optimize("y", off)` + `optimize("s", on)` (the landed sibling 0x0065FF8E's pragmas).
#pragma optimize("y", off)
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
    void* m_pInfo;                                    // +0x04
};

class CIsThingInvolvedWithTask {                      // Ego_r.pdb, 4 bytes
public:
    const CThing* Thing;                              // +0x00
    CIsThingInvolvedWithTask(const CThing& thing) : Thing(&thing) { }
};

namespace std {
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
    bool operator!=(const _List_iterator& __y) const { return _M_node != __y._M_node; }
};
template <class _Tp> class list {
public:
    typedef _List_node<_Tp> _Node;
    typedef _List_iterator<_Tp> iterator;
    _Node* _M_node;                                   // retail STLport list: the sentinel pointer only
    iterator begin() { return iterator((_Node*)(_M_node->_M_next)); }
    iterator end() { return iterator(_M_node); }
};
template <class _InputIter, class _Predicate>
_InputIter __fastcall find_if(_InputIter __first, _InputIter __last, _Predicate __pred);   // 0x0065F943
}

class CTCVillage {
public:
    template <class T> bool IsThingInTaskList(const CThing& thing, std::list<CCountedPointer<T> >& tasks) const;
};

template <class T>
bool CTCVillage::IsThingInTaskList(const CThing& thing, std::list<CCountedPointer<T> >& tasks) const
{
    return std::find_if(tasks.begin(), tasks.end(), CIsThingInvolvedWithTask(thing)) != tasks.end();
}

template bool CTCVillage::IsThingInTaskList<CVillageTaskTorch>(const CThing&, std::list<CCountedPointer<CVillageTaskTorch> >&) const;