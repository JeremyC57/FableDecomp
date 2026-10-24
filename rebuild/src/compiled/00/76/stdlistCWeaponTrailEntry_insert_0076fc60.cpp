// std::list<CWeaponTrailEntry>::insert(iterator pos, const CWeaponTrailEntry& x) -- STLport single-element list insert: malloc a node, copy the 28-byte weapon-trail entry into it, link it in before pos and return an iterator to it
// Leaf: ?insert@?$list@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0076FC60, 91 bytes, thiscall `ret 0Ch` with `this` never read (hidden
//   iterator result, pos by value, &x).
// Callee: malloc @ 0x00BFEA0E (STLport's node allocator: allocate(1) = malloc(0x24), no out-of-memory test).
// Callers: list::push_back 0x0076FE10 (UpdateWeaponTrails 0x0076EFA0) and SetHitNotification 0x008651B0.
// Element: CEnginePrimitiveWeaponTrail::CWeaponTrailEntry (two C3DVectors then a float, 28 bytes; retail ctor 0x0076DE20),
//   the model of the landed range insert 0x0076FD40.
// Label correction: catalogued as `WeaponTrail_CreateNode`; the body is STLport `list<CWeaponTrailEntry>::insert`.
// Shape: STLport 4.x `insert(pos, x) { _Node* tmp = _M_create_node(x); n = pos._M_node; p = n->_M_prev; tmp->_M_next = n;
//   tmp->_M_prev = p; p->_M_next = tmp; n->_M_prev = tmp; return tmp; }`: the placement new null-tests &tmp->_M_data, the
//   entry copies memberwise (C3DVector's user copy constructor: seven in-order dword moves, not `rep movsd`), and the
//   iterator (user constructors) returns through the hidden pointer.
extern "C" void* __cdecl malloc(unsigned int size);     // 0x00BFEA0E
inline void* operator new(unsigned int, void* where) { return where; }

class C3DVector {                                       // user memberwise copy: the entry copies as seven dword moves
public:
    float X, Y, Z;
    C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) { }
};

class CEnginePrimitiveWeaponTrail {
public:
    class CWeaponTrailEntry {                            // 28 bytes
    public:
        C3DVector Point0;                                // +0x00
        C3DVector Point1;                                // +0x0C
        float     Time;                                  // +0x18
    };
};

namespace std {

struct _List_node_base {
    _List_node_base* _M_next;
    _List_node_base* _M_prev;
};

template <class _Tp> struct _List_node : public _List_node_base {
    _Tp _M_data;
};

template <class _Tp> struct _List_iterator {
    _List_node_base* _M_node;
    _List_iterator(_List_node_base* __x) : _M_node(__x) { }
    _List_iterator(const _List_iterator& __x) : _M_node(__x._M_node) { }
};

template <class _Tp> class list {
public:
    typedef _List_node<_Tp>     _Node;
    typedef _List_iterator<_Tp> iterator;

    _List_node_base* _M_node;

    _Node* _M_create_node(const _Tp& __x)
    {
        _Node* __p = (_Node*)malloc(sizeof(_Node));
        new (&__p->_M_data) _Tp(__x);
        return __p;
    }

    iterator insert(iterator __position, const _Tp& __x);
};

template <class _Tp>
typename list<_Tp>::iterator list<_Tp>::insert(iterator __position, const _Tp& __x)
{
    _Node* __tmp = _M_create_node(__x);
    _List_node_base* __n = __position._M_node;
    _List_node_base* __p = __n->_M_prev;
    __tmp->_M_next = __n;
    __tmp->_M_prev = __p;
    __p->_M_next = __tmp;
    __n->_M_prev = __tmp;
    return iterator(__tmp);
}

template class list<CEnginePrimitiveWeaponTrail::CWeaponTrailEntry>;

}