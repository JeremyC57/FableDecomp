// std::list<CWall>::push_back(const CWall& x) -- append one 24-byte corridor wall to a steering corridor's wall list: malloc a node, copy the wall in, link it before the sentinel
// Leaf: ?push_back@?$list@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A88B10, 80 bytes, thiscall `ret 4`.
// Identity: the 4 callers (0x00A8832E, 0x00A883C8, 0x00A8844C, 0x00A884D4) all sit in CSBCCorridor::Process 0x00A880C0, which
//   fills CSBCCorridor::Walls (Ego_r: list<CWall> Walls); CWall is Ego_r's 24-byte {C2DVector Point1, Point2, ToInternal}
//   (the node is malloc(0x20): next, prev, 6 data words). The catalog's "LinkedList_InsertNodeLarge" is descriptive.
// Callees: CRT malloc 0x00BFEA0E (the STLport node allocator).
// Shape: STLport 4.x `push_back(x) { insert(end(), x); }` with insert inline: `_M_create_node(x)` (allocate one node, then the
//   placement-new copy under its null test -- six dword moves through edi), then `tmp->_M_next = n; tmp->_M_prev = p;
//   p->_M_next = tmp; n->_M_prev = tmp;` with p = n->_M_prev read once. No EH frame: the TU has no /EHsc.
extern "C" void* __cdecl malloc(unsigned int size);                              // 0x00BFEA0E

inline void* operator new(unsigned int, void* where) { return where; }

class C2DVector {
public:
    float X;                                            // +0x00
    float Y;                                            // +0x04
};

class CWall {
public:
    C2DVector Point1;                                   // +0x00
    C2DVector Point2;                                   // +0x08
    C2DVector ToInternal;                               // +0x10
};

namespace std {

template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }

struct _List_node_base {
    _List_node_base* _M_next;                           // +0x00
    _List_node_base* _M_prev;                           // +0x04
};

template <class _Tp> struct _List_node : public _List_node_base {
    _Tp _M_data;                                        // +0x08
};

template <class _Tp> class allocator {
public:
    allocator() { }
};

template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
    _Tp* allocate(unsigned int __n) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
};

template <class _Tp, class _Alloc> class _List_base {
public:
    _STLP_alloc_proxy<_List_node<_Tp>*, _List_node<_Tp>, allocator<_List_node<_Tp> > > _M_node;   // +0x00 sentinel
};

template <class _Tp> struct _List_iterator {
    _List_node_base* _M_node;
    _List_iterator(_List_node_base* __x) : _M_node(__x) { }
    _List_iterator(const _List_iterator& __x) : _M_node(__x._M_node) { }
};

template <class _Tp, class _Alloc = allocator<_Tp> >
class list : public _List_base<_Tp, _Alloc> {
public:
    typedef _List_node<_Tp> _Node;
    typedef _List_iterator<_Tp> iterator;

    _Node* _M_create_node(const _Tp& __x)
    {
        _Node* __p = this->_M_node.allocate(1);
        _Construct(&__p->_M_data, __x);
        return __p;
    }
    iterator end() { return iterator(this->_M_node._M_data); }
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
    void push_back(const _Tp& __x) { insert(end(), __x); }
};

template void list<CWall>::push_back(const CWall& __x);
}