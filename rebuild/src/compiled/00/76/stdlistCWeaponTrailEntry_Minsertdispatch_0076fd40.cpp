// std::list<CWeaponTrailEntry>::_M_insert_dispatch(iterator pos, const_iterator first, const_iterator last, const __false_type&) -- STLport list range insert: for each weapon-trail entry in [first, last) malloc a node, copy the 28-byte entry into it and link it in before pos
// Leaf: ??$_M_insert_dispatch@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0076FD40, 102 bytes, thiscall `ret 10h` with `this` never read (pos,
//   first, last by value, the empty tag by reference).
// Callee: malloc @ 0x00BFEA0E (STLport's node allocator: allocate(1) = malloc(0x24), no out-of-memory test).
// Callers: the inline list copy constructors in the landed __uninitialized_copy 0x007705A0, CEnginePrimitiveWeaponTrail
//   0x007703B0, __uninitialized_fill_n 0x007702B0 and list::operator= / the CTCWeaponTrail copies.
// Element: CEnginePrimitiveWeaponTrail::CWeaponTrailEntry (Ego_r.pdb ctor `??0CWeaponTrailEntry@CEnginePrimitiveWeapon
//   Trail@@QAE@ABVC3DVector@@0M@Z`, retail 0x0076DE20: two C3DVectors then a float, 28 bytes); node = links + entry, 0x24.
// Label correction: catalogued as `LinkedList_InsertAll`; the body is STLport `list::_M_insert_dispatch(..., __false_type)`.
// Shape: STLport 4.x `for (; first != last; ++first) insert(pos, *first);` with insert and _M_create_node inline: the
//   placement new null-tests &node->_M_data (`lea ecx,[eax+8] ; test ecx,ecx`), the entry copy is memberwise (seven
//   in-order dword moves), then `n = pos._M_node ; p = n->_M_prev` links the node (next, prev, p->next, n->prev).
extern "C" void* __cdecl malloc(unsigned int size);     // 0x00BFEA0E
inline void* operator new(unsigned int, void* where) { return where; }

class C3DVector {                                       // user memberwise copy: the entry copies as seven dword moves (implicit = rep movsd)
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

struct __false_type { __false_type() { } };

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
    _Tp& operator*() const { return ((_List_node<_Tp>*)_M_node)->_M_data; }
    _List_iterator& operator++() { _M_node = _M_node->_M_next; return *this; }
    bool operator!=(const _List_iterator& __y) const { return _M_node != __y._M_node; }
};

template <class _Tp> struct _List_const_iterator {
    _List_node_base* _M_node;
    const _Tp& operator*() const { return ((_List_node<_Tp>*)_M_node)->_M_data; }
    _List_const_iterator& operator++() { _M_node = _M_node->_M_next; return *this; }
    bool operator!=(const _List_const_iterator& __y) const { return _M_node != __y._M_node; }
};

template <class _Tp> class list {
public:
    typedef _List_node<_Tp>           _Node;
    typedef _List_iterator<_Tp>       iterator;
    typedef _List_const_iterator<_Tp> const_iterator;

    _List_node_base* _M_node;

    _Node* _M_create_node(const _Tp& __x)
    {
        _Node* __p = (_Node*)malloc(sizeof(_Node));
        new (&__p->_M_data) _Tp(__x);
        return __p;
    }

    iterator insert(iterator __position, const _Tp& __x)
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

    template <class _InputIter>
    void _M_insert_dispatch(iterator __pos, _InputIter __first, _InputIter __last, const __false_type&)
    {
        for (; __first != __last; ++__first)
            insert(__pos, *__first);
    }
};

typedef CEnginePrimitiveWeaponTrail::CWeaponTrailEntry CWeaponTrailEntry;

template void list<CWeaponTrailEntry>::_M_insert_dispatch<list<CWeaponTrailEntry>::const_iterator>(
    list<CWeaponTrailEntry>::iterator, list<CWeaponTrailEntry>::const_iterator, list<CWeaponTrailEntry>::const_iterator,
    const __false_type&);

}