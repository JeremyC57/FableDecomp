// std::list<const CThing*>::remove_if(std::unary_negate<NCameraModes::CGetThingsWithCollisionMesh> pred) -- drop every thing whose physics component has no physics-mesh info (the camera's evasion scan keeps only things with a collision mesh)
// Leaf: ??$remove_if@V?$unary_negate@VCGetThingsWithCollisionMesh@NCameraModes@@@std@@@?$list@PBVCThing@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008FBDD0, 67 bytes, thiscall, `ret 4` (the empty predicate's word).
// Identity: FableWin.pdb `??$remove_if@V?$unary_negate@VCGetThingsWithCollisionMesh@NCameraModes@@@std@@@?$list@PBVCThing@@...@Z`;
//   called by the landed NCameraModes::CPhysicsForceModelBase::DoThingEvasion 0x008FAF70 (`things.remove_if(not1(
//   CGetThingsWithCollisionMesh()))`) and the jmp 0x008FBF00.
// Callees: CTCPhysicsBase virtual slot 93 GetPhysicsMeshInfo (+0x174), free @ 0x00BFEA14 (the node).
// Layout: retail CThing PhysicsTC +0x60 (Ego_r.pdb +0x70); node = next, prev, const CThing* (+8).
// Shape: STLport 4.6 `remove_if` + 4.6 `erase` (pointer element: nothing to destroy); `!_M_pred(x)` over
//   `t->GetPhysicsTC()->GetPhysicsMeshInfo() != 0` is one `test eax,eax ; jne`. The const inline accessor GetPhysicsTC()
//   (FableWin.pdb) is what puts the vtable in edx (`mov edx,[ecx] ; call [edx+174h]`); the bare member chain
//   `t->PhysicsTC->...` reuses eax for it.
extern "C" void __cdecl free(void* block);                  // 0x00BFEA14

class CThingPhysicsMeshInfo;
class CTCPhysicsBase {
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29();
    virtual void v30(); virtual void v31(); virtual void v32(); virtual void v33(); virtual void v34(); virtual void v35();
    virtual void v36(); virtual void v37(); virtual void v38(); virtual void v39(); virtual void v40(); virtual void v41();
    virtual void v42(); virtual void v43(); virtual void v44(); virtual void v45(); virtual void v46(); virtual void v47();
    virtual void v48(); virtual void v49(); virtual void v50(); virtual void v51(); virtual void v52(); virtual void v53();
    virtual void v54(); virtual void v55(); virtual void v56(); virtual void v57(); virtual void v58(); virtual void v59();
    virtual void v60(); virtual void v61(); virtual void v62(); virtual void v63(); virtual void v64(); virtual void v65();
    virtual void v66(); virtual void v67(); virtual void v68(); virtual void v69(); virtual void v70(); virtual void v71();
    virtual void v72(); virtual void v73(); virtual void v74(); virtual void v75(); virtual void v76(); virtual void v77();
    virtual void v78(); virtual void v79(); virtual void v80(); virtual void v81(); virtual void v82(); virtual void v83();
    virtual void v84(); virtual void v85(); virtual void v86(); virtual void v87(); virtual void v88(); virtual void v89();
    virtual void v90(); virtual void v91(); virtual void v92();
    virtual const CThingPhysicsMeshInfo* GetPhysicsMeshInfo() const;   // slot 93 (+0x174)
};

class CThing {
public:
    unsigned char   _pad00[0x60];
    CTCPhysicsBase* PhysicsTC;                              // +0x60 (Ego_r.pdb +0x70)
    const CTCPhysicsBase* GetPhysicsTC() const { return PhysicsTC; }   // FableWin.pdb ?GetPhysicsTC@CThing@@QBEPBVCTCPhysicsBase@@XZ
};

namespace std {
template <class _Arg, class _Result> struct unary_function {
    typedef _Arg argument_type;
    typedef _Result result_type;
};
template <class _Predicate>
class unary_negate : public unary_function<typename _Predicate::argument_type, bool> {
protected:
    _Predicate _M_pred;
public:
    explicit unary_negate(const _Predicate& __x) : _M_pred(__x) { }
    bool operator()(const typename _Predicate::argument_type& __x) const { return !_M_pred(__x); }
};
}

namespace NCameraModes {
class CGetThingsWithCollisionMesh : public std::unary_function<const CThing*, bool> {
public:
    CGetThingsWithCollisionMesh() { }
    bool operator()(const CThing* thing) const { return thing->GetPhysicsTC()->GetPhysicsMeshInfo() != 0; }
};
}

namespace std {
template <class _Tp> class allocator {
public:
    allocator() { }
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};

template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};

template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }

struct _List_node_base {
    _List_node_base* _M_next;                               // +0x0
    _List_node_base* _M_prev;                               // +0x4
};
template <class _Tp> struct _List_node : public _List_node_base {
    _Tp _M_data;                                            // +0x8
};

struct _List_iterator_base {
    _List_node_base* _M_node;
    _List_iterator_base(_List_node_base* __x) : _M_node(__x) { }
    void _M_incr() { _M_node = _M_node->_M_next; }
    bool operator==(const _List_iterator_base& __y) const { return _M_node == __y._M_node; }
    bool operator!=(const _List_iterator_base& __y) const { return _M_node != __y._M_node; }
};

template <class _Tp> struct _List_iterator : public _List_iterator_base {
    typedef _List_node<_Tp> _Node;
    _List_iterator(_List_node_base* __x) : _List_iterator_base(__x) { }
    _List_iterator(const _List_iterator& __x) : _List_iterator_base(__x._M_node) { }
    _Tp& operator*() const { return ((_Node*)_M_node)->_M_data; }
    _Tp* operator->() const { return &((_Node*)_M_node)->_M_data; }
    _List_iterator& operator++() { this->_M_incr(); return *this; }
};

template <class _Tp, class _Alloc> class _List_base {
public:
    typedef _List_node<_Tp> _Node;
    _STLP_alloc_proxy<_Node*, _Node, allocator<_Node> > _M_node;
};

template <class _Tp, class _Alloc = allocator<_Tp> >
class list : public _List_base<_Tp, _Alloc> {
public:
    typedef _List_node<_Tp> _Node;
    typedef _List_iterator<_Tp> iterator;
    iterator begin() { return iterator(this->_M_node._M_data->_M_next); }
    iterator end() { return iterator(this->_M_node._M_data); }
    iterator erase(iterator __position)
    {
        _List_node_base* __next_node = __position._M_node->_M_next;
        _List_node_base* __prev_node = __position._M_node->_M_prev;
        _Node* __n = (_Node*)__position._M_node;
        __prev_node->_M_next = __next_node;
        __next_node->_M_prev = __prev_node;
        _Destroy(&__n->_M_data);
        this->_M_node.deallocate(__n, 1);
        return iterator(__next_node);
    }
    void remove(const _Tp& __val)
    {
        iterator __first = begin();
        iterator __last = end();
        while (__first != __last) {
            iterator __next = __first;
            ++__next;
            if (__val == *__first)
                erase(__first);
            __first = __next;
        }
    }
    template <class _Predicate> void remove_if(_Predicate __pred)
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
};
}

template void std::list<const CThing*>::remove_if(std::unary_negate<NCameraModes::CGetThingsWithCollisionMesh>);