// _STL::list<C3DVector>::_M_insert_dispatch(iterator, const_iterator, const_iterator, const __false_type&) -- copy-insert a C3DVector range before pos, one node per element
// Leaf: ??$_M_insert_dispatch@
// Retail 0x008FBF10, 0x4E bytes, thiscall `ret 10h` (`this` = the destination list, unused: the position iterator
//   carries the node), void. Manifest label "LinkedList_CloneNodes" (_global) is descriptive; the PDB name set has
//   `??$_M_insert_dispatch@V?$_List_iterator@VC3DVector@@U?$_Const_traits@VC3DVector@@@_STL@@@_STL@@@?$list@VC3DVector@@...`.
//   Six callers, all in the NCameraModes::CPhysicsForceModelBase TU (list<C3DVector>::insert(pos, first, last)).
// Callee: malloc 0x00BFEA0E (node = next/prev + C3DVector = 0x14 bytes; the STLport allocator's `n != 0` test folds).
// Shape: STLport 4.x `_M_insert_dispatch(pos, first, last, const __false_type&)` = `for (; first != last; ++first)
//   insert(pos, *first)`, with insert = _M_create_node (null-tested placement copy of the 12-byte vector) and the
//   tmp->next = n / tmp->prev = n->prev / prev->next = tmp / n->prev = tmp link order. The iterators are passed by
//   value (one node pointer each), the __false_type tag by reference (the fourth dword, unused).
//   InsertPoints below is only an instantiation point.
extern "C" void* __cdecl malloc(unsigned int size);                              // 0x00BFEA0E
extern "C" void  __cdecl free(void* block);                                      // 0x00BFEA14

inline void* operator new(unsigned int, void* __p) { return __p; }

struct C3DVector {                              // retail C3DVector, 12 bytes
    float X, Y, Z;
};

namespace _STL {

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

template <class _Tp, class _Traits> struct _List_iterator {
    typedef _List_iterator<_Tp, _Traits> _Self;
    _List_node_base* _M_node;
    _List_iterator(_List_node_base* __x) : _M_node(__x) { }
    _List_iterator(const _List_iterator<_Tp, _Nonconst_traits<_Tp> >& __x) : _M_node(__x._M_node) { }
    typename _Traits::reference operator*() const { return ((_List_node<_Tp>*)_M_node)->_M_data; }
    _Self& operator++() { _M_node = _M_node->_M_next; return *this; }
    bool operator==(const _List_iterator<_Tp, _Const_traits<_Tp> >& __y) const { return _M_node == __y._M_node; }
    bool operator!=(const _List_iterator<_Tp, _Const_traits<_Tp> >& __y) const { return _M_node != __y._M_node; }
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

}

namespace NCameraModes {
// Instantiation point: CPhysicsCollisionPointsObject-style list<C3DVector> range insert (the TU's callers pass const_iterators).
void InsertPoints(_STL::list<C3DVector>& points, const _STL::list<C3DVector>& source)
{
    points._M_insert_dispatch(points.end(), source.begin(), source.end(), _STL::__false_type());
}
}