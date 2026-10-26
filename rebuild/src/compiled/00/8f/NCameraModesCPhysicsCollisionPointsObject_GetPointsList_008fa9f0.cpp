// NCameraModes::CPhysicsCollisionPointsObject::GetPointsList() -- rebuild Points as the concatenation of every SmartPointList entry's point list
// Leaf: ?GetPointsList@CPhysicsCollisionPointsObject@NCameraModes@@QAEXXZ
// Grade: RELOCATION_MATCH (near-miss retry agent901) -- the STLport 4.6 iterator shape (_List_iterator_base with _M_incr and ==/!= under a _List_iterator with a user copy constructor) lets VC7.1 rotate both loops as retail does (as the landed 0x008FCA40 in this TU)
// Retail 0x008FA9F0, 0x85 bytes (row 0x90 with int3 padding), thiscall on ecx (CPhysicsCollisionPointsObject*), void. Manifest label
//   "LinkedList_ClearAndReinit" (_global) is wrong: the PDB name set has
//   `?GetPointsList@CPhysicsCollisionPointsObject@NCameraModes@@...`; the field layout (+0 Points, +4 SmartPointList) fits.
// Callees: free 0x00BFEA14 (Points.clear), malloc 0x00BFEA0E (node allocation, 0xC + 8 = 0x14 bytes).
// Shape: Points.clear(), then nested iterator loops over SmartPointList and each entry's point list with
//   Points.push_back; the iterators need the STLport 4.6 shape (_List_iterator_base + user copy ctor) for VC7.1 to
//   rotate both loops and cache the source end in a register as retail does.
extern "C" void* __cdecl malloc(unsigned int size);                              // 0x00BFEA0E
extern "C" void  __cdecl free(void* block);                                      // 0x00BFEA14

inline void* operator new(unsigned int, void* __p) { return __p; }

class C3DVector { public:                              // retail C3DVector, 12 bytes
    float X, Y, Z;
};

namespace std {

struct __false_type { };

template <class _Tp> class allocator {
public:
    allocator() { }
    allocator(const allocator&) { }
    _Tp* allocate(unsigned int __n, const void* = 0) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
    void deallocate(_Tp* __p, unsigned int) { free(__p); }
};

template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
    _STLP_alloc_proxy(const _MaybeReboundAlloc& __a, _Value __p) : _MaybeReboundAlloc(__a), _M_data(__p) { }
};

template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }
template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }

struct _List_node_base {
    _List_node_base* _M_next;                   // +0x00
    _List_node_base* _M_prev;                   // +0x04
};

template <class _Tp> struct _List_node : public _List_node_base {
    _Tp _M_data;                                // +0x08
};

template <class _Tp> struct _Nonconst_traits { typedef _Tp& reference; typedef _Tp* pointer; };
template <class _Tp> struct _Const_traits { typedef const _Tp& reference; typedef const _Tp* pointer; };

struct _List_iterator_base {
    _List_node_base* _M_node;
    _List_iterator_base(_List_node_base* __x) : _M_node(__x) { }
    void _M_incr() { _M_node = _M_node->_M_next; }
    bool operator==(const _List_iterator_base& __y) const { return _M_node == __y._M_node; }
    bool operator!=(const _List_iterator_base& __y) const { return _M_node != __y._M_node; }
};

template <class _Tp, class _Traits> struct _List_iterator : public _List_iterator_base {
    typedef _List_iterator<_Tp, _Traits> _Self;
    _List_iterator(_List_node_base* __x) : _List_iterator_base(__x) { }
    _List_iterator(const _List_iterator<_Tp, _Nonconst_traits<_Tp> >& __x) : _List_iterator_base(__x._M_node) { }
    typename _Traits::reference operator*() const { return ((_List_node<_Tp>*)_M_node)->_M_data; }
    _Self& operator++() { this->_M_incr(); return *this; }
};

template <class _Tp, class _Alloc> class _List_base {
public:
    typedef _List_node<_Tp> _Node;
    _STLP_alloc_proxy<_List_node_base*, _Node, allocator<_Node> > _M_node;
    void clear()
    {
        _Node* __cur = (_Node*)this->_M_node._M_data->_M_next;
        while (__cur != this->_M_node._M_data) {
            _Node* __tmp = __cur;
            __cur = (_Node*)__cur->_M_next;
            _Destroy(&__tmp->_M_data);
            this->_M_node.deallocate(__tmp, 1);
        }
        this->_M_node._M_data->_M_next = this->_M_node._M_data;
        this->_M_node._M_data->_M_prev = this->_M_node._M_data;
    }
};

template <class _Tp, class _Alloc = allocator<_Tp> >
class list : public _List_base<_Tp, _Alloc> {
public:
    typedef _List_node<_Tp> _Node;
    typedef _List_iterator<_Tp, _Nonconst_traits<_Tp> > iterator;
    typedef _List_iterator<_Tp, _Const_traits<_Tp> > const_iterator;
    iterator begin() { return iterator(this->_M_node._M_data->_M_next); }
    iterator end() { return iterator(this->_M_node._M_data); }
    const_iterator begin() const { return const_iterator(this->_M_node._M_data->_M_next); }
    const_iterator end() const { return const_iterator(this->_M_node._M_data); }

    _Node* _M_create_node(const _Tp& __x)
    {
        _Node* __p = this->_M_node.allocate(1);
        _Construct(&__p->_M_data, __x);
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
        return __tmp;
    }
    void push_back(const _Tp& __x) { insert(end(), __x); }
    template <class _InputIter>
    void _M_insert_dispatch(iterator __position, _InputIter __first, _InputIter __last, const __false_type&)
    {
        for ( ; __first != __last; ++__first)
            insert(__position, *__first);
    }
    template <class _InputIter>
    void insert(iterator __pos, _InputIter __first, _InputIter __last)
    {
        _M_insert_dispatch(__pos, __first, __last, __false_type());
    }
};


template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
};

}

namespace NCameraModes {

class CPhysicsCollisionPointsObject {
public:
    std::list<C3DVector>                                    Points;            // +0x00
    std::list<std::pair<float, std::list<C3DVector> > >   SmartPointList;    // +0x04
    std::list<std::pair<float, long> >                     NumTestsPerTheta;  // +0x08
    long  ScanDensity;                                                          // +0x0c
    float StartTheta;                                                           // +0x10
    float DeltaTheta;                                                           // +0x14
    long  DebugGraphicsRed;                                                     // +0x18
    long  DebugGraphicsGreen;                                                   // +0x1c
    long  DebugGraphicsBlue;                                                    // +0x20
    void GetPointsList();
};

void CPhysicsCollisionPointsObject::GetPointsList()
{
    Points.clear();
    for (std::list<std::pair<float, std::list<C3DVector> > >::iterator it = SmartPointList.begin();
         it != SmartPointList.end(); ++it)
        for (std::list<C3DVector>::iterator point = (*it).second.begin(); point != (*it).second.end(); ++point)
            Points.push_back(*point);
}

}