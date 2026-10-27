// std::_List_iterator::operator--() -- STLport list iterator pre-decrement: step to the previous node and return the iterator
// Leaf: ??F?$_List_iterator@UCUnprovenElement_0076dae0@@U?$_Nonconst_traits@UCUnprovenElement_0076dae0@@@std@@@std@@QAEAAU01@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0076DAE0, 10 bytes, plain `ret` (thiscall, returns `this`).
// Identity: the STLport 4.6 `_List_iterator<_Tp, _Traits>::operator--()` (`this->_M_decr(); return *this;`, _M_decr =
//   `_M_node = _M_node->_M_prev`, the node's _M_prev at +4). It sits in a run of tiny STLport list helpers emitted into
//   the CTCEnemy / CTCBandit / CTCWeaponTrail code (0x0076DA90 / 0x0076DAA0 `return _M_node`, 0x0076DAB0 iterator from a
//   node, 0x0076DAC0 iterator copy, 0x0076DAD0 `&_M_node->_M_data` at +8). It has no direct callers (an unreferenced
//   inline instantiation kept by the link), so the element type is unproven: `CUnprovenElement_0076dae0` (descriptive;
//   the TU's lists are list<CIntelligentPointer<const CThing> > (CTCEnemy) and list<CWeaponTrailEntry> (CTCWeaponTrail)).
//   Catalog row was the placeholder `_gapscan::sub_0076dae0`.
// Shape: `mov eax,ecx ; mov ecx,[eax] ; mov edx,[ecx+4] ; mov [eax],edx`.
struct CUnprovenElement_0076dae0;

namespace std {
struct _List_node_base {
    _List_node_base* _M_next;                   // +0
    _List_node_base* _M_prev;                   // +4
};

template <class _Tp> struct _Nonconst_traits {
    typedef _Tp value_type;
    typedef _Tp* pointer;
    typedef _Tp& reference;
};

struct _List_iterator_base {
    _List_node_base* _M_node;
    void _M_incr() { _M_node = _M_node->_M_next; }
    void _M_decr() { _M_node = _M_node->_M_prev; }
};

template <class _Tp, class _Traits>
struct _List_iterator : public _List_iterator_base {
    typedef _List_iterator<_Tp, _Traits> _Self;
    _Self& operator--() { this->_M_decr(); return *this; }
};

template struct _List_iterator<CUnprovenElement_0076dae0, _Nonconst_traits<CUnprovenElement_0076dae0> >;
}