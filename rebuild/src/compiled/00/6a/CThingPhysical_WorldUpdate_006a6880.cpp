// CThingPhysical::WorldUpdate() -- apply one frame of every health-over-time effect (an equal share of the health left), drop the finished ones, then the CThing world update
// Leaf: ?WorldUpdate@CThingPhysical@@UAE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x006A6880, 111 bytes, thiscall, tail `jmp` to CThing::WorldUpdate.
// Identity (proven): slot 19 of the CThingPhysical vtable (rtti-vtables CThingPhysical#19; the CThing slot-19 body 0x004C8050
//   is the tail target, bound ?WorldUpdate@CThing@@UAE_NXZ); Ego_r.pdb ?WorldUpdate@CThingPhysical@@UAE_NXZ. The list at +0x98
//   is Ego_r CThingPhysical::HealthOverTimeEffects (list<CThingPhysical::CHealthOverTimeHelper>, 4-byte STLport list at the
//   retail offset of engine/CThingPhysical.h); the node payload is CHealthOverTimeHelper {FramesLeftForAddingHealth +0,
//   HealthLeftToAdd +4} (Ego_r layout, 8 bytes).
// Callees: CThingPhysical::InternalModifyHealth(float, bool) 0x006A62E0 (Ego_r ?InternalModifyHealth@CThingPhysical@@AAEXM_N@Z,
//   private), free 0x00BFEA14 (STLport node deallocate), CThing::WorldUpdate 0x004C8050 (tail).
// Shape: CHealthOverTimeHelper::Update(CThingPhysical&) / IsFinished() const (Ego_r out-of-line members of the same TU) are
//   auto-inlined; the loop erases finished helpers with STLport list::erase (trivial element: no destructor call).
extern "C" void __cdecl free(void* __p);                 // 0x00BFEA14
namespace std {
template <class _Tp> class allocator { public:
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};
template <class _Tp, class _Alloc> class _STLP_alloc_proxy : public _Alloc { public:
    _Tp _M_data;
};
template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }
struct _List_node_base {
    _List_node_base* _M_next;
    _List_node_base* _M_prev;
};
template <class _Tp> struct _List_node : public _List_node_base {
    _Tp _M_data;
};
struct _List_iterator_base {
    _List_node_base* _M_node;
    _List_iterator_base(_List_node_base* __x) : _M_node(__x) {}
    void _M_incr() { _M_node = _M_node->_M_next; }
    void _M_decr() { _M_node = _M_node->_M_prev; }
    bool operator==(const _List_iterator_base& __y) const { return _M_node == __y._M_node; }
    bool operator!=(const _List_iterator_base& __y) const { return _M_node != __y._M_node; }
};
template <class _Tp> struct _List_iterator : public _List_iterator_base {
    typedef _List_node<_Tp> _Node;
    _List_iterator(_Node* __x) : _List_iterator_base(__x) {}
    _Tp& operator*() const { return ((_Node*)_M_node)->_M_data; }
    _Tp* operator->() const { return &(operator*()); }
    _List_iterator& operator++() { this->_M_incr(); return *this; }
    _List_iterator& operator--() { this->_M_decr(); return *this; }
};
template <class _Tp, class _Alloc> class _List_base { public:
    typedef _List_node<_Tp> _Node;
    _STLP_alloc_proxy<_Node*, allocator<_Node> > _M_node;   // +0x0
};
template <class _Tp, class _Alloc = allocator<_Tp> > class list : public _List_base<_Tp, _Alloc> { public:
    typedef _List_node<_Tp> _Node;
    typedef _List_iterator<_Tp> iterator;
    iterator begin() { return iterator((_Node*)(this->_M_node._M_data->_M_next)); }
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
        return iterator((_Node*)__next_node);
    }
};
} // namespace std
class CThing { public:
    virtual bool WorldUpdate();                          // slot 19 0x004C8050
    unsigned char _body[0x98 - 4];
};
class CThingPhysical : public CThing {
public:
    class CHealthOverTimeHelper {                        // Ego_r.pdb, 8 bytes
    public:
        void Update(CThingPhysical& thing);
        bool IsFinished() const;
        long  FramesLeftForAddingHealth;                 // +0x00
        float HealthLeftToAdd;                           // +0x04
    };
    virtual bool WorldUpdate();                          // slot 19
private:
    void InternalModifyHealth(float, bool);              // 0x006A62E0
    friend class CHealthOverTimeHelper;
public:
    std::list<CHealthOverTimeHelper> HealthOverTimeEffects;   // +0x98
};
void CThingPhysical::CHealthOverTimeHelper::Update(CThingPhysical& thing)
{
    float health = HealthLeftToAdd / FramesLeftForAddingHealth;
    thing.InternalModifyHealth(health, true);
    HealthLeftToAdd -= health;
    --FramesLeftForAddingHealth;
}
bool CThingPhysical::CHealthOverTimeHelper::IsFinished() const
{
    return FramesLeftForAddingHealth <= 0;
}
bool CThingPhysical::WorldUpdate()
{
    std::list<CHealthOverTimeHelper>::iterator it = HealthOverTimeEffects.begin();
    while (it != HealthOverTimeEffects.end()) {
        it->Update(*this);
        if (it->IsFinished())
            it = HealthOverTimeEffects.erase(it);
        else
            ++it;
    }
    return CThing::WorldUpdate();
}