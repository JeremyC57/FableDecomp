// std::list<CEnginePrimitiveHandle>::_M_fill_insert(iterator pos, size_type n, const CEnginePrimitiveHandle& x) -- insert n copies of the handle before pos (each copy adds a reference to the primitive)
// Leaf: ?_M_fill_insert@?$list@VCEnginePrimitiveHandle@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00BBDAD0, 79 bytes, thiscall (`this` unused), `ret 0Ch`.
// Identity: called by list<CEnginePrimitiveHandle>::resize 0x00BBDC60 and 0x00BBDC03 (a one-element insert). The node is
//   {next, prev, CEnginePrimitiveHandle {vtable 0x0122F598, counted Data}} = 0x10 bytes. Catalog
//   "AllocateLinkedListNodesWithRefcount" is descriptive.
// Callees: malloc @ 0x00BFEA0E (one node per copy).
// Shape: STLport 4.x `for (; n > 0; --n) insert(pos, x);` with insert(pos, x) and _M_create_node inline: malloc, the
//   null-tested placement copy (only the derived vtable store survives; the counted pointer copy adds a reference), then
//   the four link stores.
extern "C" void* __cdecl malloc(unsigned int size);    // 0x00BFEA0E
extern "C" void __cdecl free(void* block);              // 0x00BFEA14
inline void* operator new(unsigned int, void* p) { return p; }

class CEngineInternalPrimitiveBase {                    // the persistent primitive a handle counts
public:
    void* m_vtable;                                     // +0x00
    long  RefCount;                                     // +0x04
};

class CIVCountedPointer {                               // counted pointer to the primitive
public:
    CEngineInternalPrimitiveBase* Object;               // +0x00
    CIVCountedPointer(const CIVCountedPointer& o) : Object(o.Object)
    {
        if (Object)
            ++Object->RefCount;
    }
};

class CBaseClass {                                      // vtable 0x01231710
public:
    virtual ~CBaseClass();
};

class CEnginePrimitiveHandle : public CBaseClass {      // vtable 0x0122F598, 8 bytes
public:
    CIVCountedPointer Data;                             // +0x04
    virtual ~CEnginePrimitiveHandle();
};

namespace std {

template <class _Tp> class allocator {
public:
    allocator() { }
};

template <class _Tp, class _Alloc> class _STLP_alloc_proxy : public _Alloc {
public:
    _Tp _M_data;
};

template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }
template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }

struct _List_node_base {
    _List_node_base* _M_next;                           // +0x0
    _List_node_base* _M_prev;                           // +0x4
};
template <class _Tp> struct _List_node : public _List_node_base {
    _Tp _M_data;                                        // +0x8
};

struct _List_iterator_base {
    _List_node_base* _M_node;
    _List_iterator_base(_List_node_base* __x) : _M_node(__x) { }
    void _M_incr() { _M_node = _M_node->_M_next; }
    bool operator==(const _List_iterator_base& __y) const { return _M_node == __y._M_node; }
    bool operator!=(const _List_iterator_base& __y) const { return _M_node != __y._M_node; }
};

template <class _Tp> struct _Nonconst_traits { typedef _Tp value_type; typedef _Tp& reference; typedef _Tp* pointer; };

template <class _Tp, class _Traits> struct _List_iterator : public _List_iterator_base {
    typedef _List_iterator<_Tp, _Traits> _Self;
    typedef _List_node<_Tp> _Node;
    _List_iterator(_List_node_base* __x) : _List_iterator_base(__x) { }
    _List_iterator(const _Self& __x) : _List_iterator_base(__x._M_node) { }
    _Self& operator++() { this->_M_incr(); return *this; }
    _Self operator++(int)
    {
        _Self __tmp = *this;
        this->_M_incr();
        return __tmp;
    }
};

template <class _Tp, class _Alloc> class _List_base {
public:
    typedef _List_node<_Tp> _Node;
    _STLP_alloc_proxy<_Node*, allocator<_Node> > _M_node;   // +0x00: the sentinel node
};

template <class _Tp, class _Alloc = allocator<_Tp> > class list : public _List_base<_Tp, _Alloc> {
public:
    typedef _List_node<_Tp> _Node;
    typedef unsigned int size_type;
    typedef _List_iterator<_Tp, _Nonconst_traits<_Tp> > iterator;

    _Node* _M_create_node(const _Tp& __x)
    {
        _Node* __p = (_Node*)malloc(sizeof(_Node));
        _Construct(&__p->_M_data, __x);
        return __p;
    }
    void _M_deallocate(_Node* __p) { if (__p != 0) free(__p); }
    iterator begin() { return iterator(this->_M_node._M_data->_M_next); }
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
    void _M_fill_insert(iterator __pos, size_type __n, const _Tp& __x);
    void insert(iterator __pos, size_type __n, const _Tp& __x) { _M_fill_insert(__pos, __n, __x); }

    iterator erase(iterator __position)
    {
        _List_node_base* __next_node = __position._M_node->_M_next;
        _List_node_base* __prev_node = __position._M_node->_M_prev;
        _Node* __n = (_Node*)__position._M_node;
        __prev_node->_M_next = __next_node;
        __next_node->_M_prev = __prev_node;
        _Destroy(&__n->_M_data);
        _M_deallocate(__n);
        return iterator((_Node*)__next_node);
    }
    iterator erase(iterator __first, iterator __last);
    void resize(size_type __new_size, const _Tp& __x);
};

template <class _Tp, class _Alloc>
void list<_Tp, _Alloc>::_M_fill_insert(iterator __pos, size_type __n, const _Tp& __x)
{
    for (; __n > 0; --__n)
        insert(__pos, __x);
}

template void list<CEnginePrimitiveHandle>::_M_fill_insert(iterator, size_type, const CEnginePrimitiveHandle&);

} // namespace std