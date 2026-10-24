// CSteeringAgent::CreateLineList(vector<C3DVector>& lines) -- let every steering behaviour, input and constraint add its debug lines
// Leaf: ?CreateLineList@CSteeringAgent@@QAEXAAV?$vector@VC3DVector@@V?$allocator@VC3DVector@@@_STL@@@_STL@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A1B470, 102 bytes, `ret 4`.
// Identity: Ego_r.pdb `?CreateLineList@CSteeringAgent@@QAEXAAV?$vector@VC3DVector@@...@Z`; its only caller is the
//   tail jump in CNavigatorAStar::CreateLineList 0x00A1BFC0 (`mov ecx,[ecx+20h]` = CNavigatorBase::SteeringAgent).
//   The catalog label "linked_list_for_each_call_virtual" is descriptive.
// Layout (Ego_r.pdb CSteeringAgent with 4-byte retail STLport lists): SteeringBehaviours +0x0C, SteeringBehaviour-
//   Inputs +0x10, SteeringBehaviourConstraints +0x14; each element's CreateLineList is vtable slot 3 (CSB::
//   CreateLineList is virtual in the PDB).
// Shape: three iterator loops; end() is re-read after every virtual call; the loop heads differ only in the
//   alignment padding VC7.1 inserts.
class C3DVector { public: float X, Y, Z; };
class CNavigatorExternalInterface;

namespace _STL {
template <class _Tp> class allocator { public: allocator() {} };
template <class _Tp, class _Alloc> class _STLP_alloc_proxy { public: _Tp _M_data; };
template <class _Tp, class _Alloc = allocator<_Tp> > class vector {
public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Alloc> _M_end_of_storage;
};
struct _List_node_base {
    _List_node_base* _M_next;
    _List_node_base* _M_prev;
};
template <class _Tp> struct _List_node : public _List_node_base {
    _Tp _M_data;
};
template <class _Tp> struct _List_iterator {
    _List_node_base* _M_node;
    _List_iterator(_List_node_base* __x) : _M_node(__x) {}
    _List_iterator(const _List_iterator& __x) : _M_node(__x._M_node) {}
    _Tp& operator*() const { return ((_List_node<_Tp>*)_M_node)->_M_data; }
    _List_iterator& operator++() { _M_node = _M_node->_M_next; return *this; }
    bool operator!=(const _List_iterator& __y) const { return _M_node != __y._M_node; }
};
template <class _Tp, class _Alloc = allocator<_Tp> > class list {
public:
    typedef _List_iterator<_Tp> iterator;
    _STLP_alloc_proxy<_List_node_base*, _Alloc> _M_node;
    iterator begin() { return iterator(_M_node._M_data->_M_next); }
    iterator end() { return iterator(_M_node._M_data); }
};
}

class CSB {
public:
    virtual void v00();
    virtual void v01();
    virtual void v02();
    virtual void CreateLineList(_STL::vector<C3DVector>& lines);   // slot 3
};
class CSBInput {
public:
    virtual void v00();
    virtual void v01();
    virtual void v02();
    virtual void CreateLineList(_STL::vector<C3DVector>& lines);   // slot 3
};
class CSBConstraint {
public:
    virtual void v00();
    virtual void v01();
    virtual void v02();
    virtual void CreateLineList(_STL::vector<C3DVector>& lines);   // slot 3
};

class CSteeringAgent {
public:
    CNavigatorExternalInterface*  NavigatorInterface;               // +0x00
    bool                          DummyObstacle;                    // +0x04
    float                         RepulsionMultiplier;              // +0x08
    _STL::list<CSB*>              SteeringBehaviours;               // +0x0C
    _STL::list<CSBInput*>         SteeringBehaviourInputs;          // +0x10
    _STL::list<CSBConstraint*>    SteeringBehaviourConstraints;     // +0x14
    void CreateLineList(_STL::vector<C3DVector>& lines);
};

void CSteeringAgent::CreateLineList(_STL::vector<C3DVector>& lines)
{
    for (_STL::list<CSB*>::iterator b = SteeringBehaviours.begin(); b != SteeringBehaviours.end(); ++b)
        (*b)->CreateLineList(lines);
    for (_STL::list<CSBInput*>::iterator i = SteeringBehaviourInputs.begin(); i != SteeringBehaviourInputs.end(); ++i)
        (*i)->CreateLineList(lines);
    for (_STL::list<CSBConstraint*>::iterator c = SteeringBehaviourConstraints.begin(); c != SteeringBehaviourConstraints.end(); ++c)
        (*c)->CreateLineList(lines);
}