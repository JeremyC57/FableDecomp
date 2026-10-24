// std::list<CWeaponTrailEntry>::push_back(const CWeaponTrailEntry& x) -- STLport list push_back: insert(end(), x), discarding the returned iterator
// Leaf: ?push_back@?$list@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0076FE10, 25 bytes, thiscall `ret 4`.
// Callee: list<CWeaponTrailEntry>::insert(iterator, const CWeaponTrailEntry&) @ 0x0076FC60 (hidden iterator result,
//   `ret 0Ch`).
// Callers: CTCWeaponTrail::UpdateWeaponTrails 0x0076EFA0 (twice: the ProjectileWeaponTrail list at +0x60).
// Label correction: catalogued as `WeaponTrail_Callback`; the body is STLport `list::push_back`. Same shape as the landed
//   CPointerRecordList::push_back 0x004CD2D0 (identical bytes modulo the callee).
// Shape: `insert(end(), x)` -- the end iterator (the sentinel at +0) is built in its argument slot through the iterator's
//   user copy constructor (`push ecx ; mov eax,esp ; mov [eax],edx`), and the discarded iterator result is homed in the
//   dead `x` argument slot (`lea eax,[esp+0Ch] ; push eax`).
class CEnginePrimitiveWeaponTrail {
public:
    class CWeaponTrailEntry;                             // 28 bytes: two C3DVectors and a float
};

namespace std {

struct _List_node_base {
    _List_node_base* _M_next;
    _List_node_base* _M_prev;
};

template <class _Tp> struct _List_iterator {
    _List_node_base* _M_node;
    _List_iterator(_List_node_base* __x) : _M_node(__x) { }
    _List_iterator(const _List_iterator& __x) : _M_node(__x._M_node) { }
};

template <class _Tp> class list {
public:
    typedef _List_iterator<_Tp> iterator;

    _List_node_base* _M_node;                            // the sentinel (retail list = one pointer)

    iterator end() { return iterator(_M_node); }
    iterator insert(iterator __position, const _Tp& __x);   // 0x0076FC60
    void push_back(const _Tp& __x) { insert(end(), __x); }
};

template void list<CEnginePrimitiveWeaponTrail::CWeaponTrailEntry>::push_back(const CEnginePrimitiveWeaponTrail::CWeaponTrailEntry&);

}