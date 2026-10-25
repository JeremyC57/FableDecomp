// list<CNavWayPoint>::push_front(const CNavWayPoint& x) -- link a copy of the way point in front of the route
// Leaf: ?push_front@?$list@VCNavWayPoint@@V?$allocator@VCNavWayPoint@@@std@@@std@@QAEXABVCNavWayPoint@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A21FC0, 67 bytes, `ret 4`. Callers 0x00A1E296 / 0x00A1F242 (CNavigatorAStar).
//   The catalog label "CNavigatorPathNode_Create" is descriptive.
// Element: CNavWayPoint (Ego_r.pdb: C2DVector Position +0, uchar NavLayer +8, bool Removable +9; 0x0C with padding),
//   the CNavigatorAStar::Route list (node 0x14). C2DVector has a user copy constructor, so the implicit CNavWayPoint
//   copy is memberwise (two dwords, two bytes) rather than a 3-dword block copy.
// Shape: STLport insert inlined: the position node read before the node is created (malloc 0x00BFEA0E), placement new
//   tests &node->_M_data, then the four link stores through the cached `__p = __n->_M_prev` local (STLport 4.6 form).
//   The list iterator needs its user copy constructor: without it VC7.1 pushes edi inside the construct branch
//   instead of in the prologue.
extern "C" void* __cdecl malloc(unsigned int size);    // 0x00BFEA0E
inline void* operator new(unsigned int, void* where) { return where; }

class C2DVector { public: float X, Y; C2DVector(const C2DVector& o) : X(o.X), Y(o.Y) {} };

class CNavWayPoint {                                    // 0x0C bytes (PDB 10 + padding)
public:
    C2DVector     Position;                             // +0x00
    unsigned char NavLayer;                             // +0x08
    bool          Removable;                            // +0x09
};

namespace std {
template <class _T1, class _T2>
inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }
template <class _Tp> class allocator {
public:
    allocator() {}
    _Tp* allocate(unsigned int __n) { return (_Tp*)malloc(__n * sizeof(_Tp)); }
};
struct _List_node_base {
    _List_node_base* _M_next;
    _List_node_base* _M_prev;
};
template <class _Tp> struct _List_node : public _List_node_base {
    _Tp _M_data;
};
template <class _Tp, class _Alloc> class _STLP_alloc_proxy : public _Alloc { public: _Tp _M_data; };
template <class _Tp> struct _List_iterator {
    _List_node_base* _M_node;
    _List_iterator(_List_node_base* __x) : _M_node(__x) {}
    _List_iterator(const _List_iterator& __x) : _M_node(__x._M_node) {}
};
template <class _Tp, class _Alloc = allocator<_Tp> > class list {
public:
    typedef _List_node<_Tp> _Node;
    typedef _List_iterator<_Tp> iterator;
    _STLP_alloc_proxy<_List_node_base*, allocator<_Node> > _M_node;
    _Node* _M_create_node(const _Tp& __x)
    {
        _Node* __p = _M_node.allocate(1);
        _Construct(&__p->_M_data, __x);
        return __p;
    }
    iterator begin() { return iterator(_M_node._M_data->_M_next); }
    iterator end() { return iterator(_M_node._M_data); }
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
    void push_front(const _Tp& __x) { insert(begin(), __x); }
    void push_back(const _Tp& __x) { insert(end(), __x); }
};
}

template void std::list<CNavWayPoint>::push_front(const CNavWayPoint&);