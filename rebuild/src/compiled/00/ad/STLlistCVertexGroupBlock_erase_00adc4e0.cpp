// std::list<CVertexGroupBlock>::erase(iterator first, iterator last) -- unlink and destroy a run of blocks from the optimiser's block list (each block's group map and group vector), return last
// Leaf: ?erase@?$list@VCVertexGroupBlock@@@std@@QAE?AU?$_List_iterator@VCVertexGroupBlock@@@2@U32@0@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00ADC4E0, 0xC1 bytes, thiscall (`this` unused), `ret 0Ch` (hidden iterator
// result, first, last), returns the result pointer.
// Identity: the list node is {next +0, prev +4, CVertexGroupBlock +8} (map header +0x14, map count +0x18, vector start
// +8): Ego_r.pdb CVertexGroupBlockOptimiser::Blocks, list<CVertexGroupBlock>; same node destruction as the sibling
// _List_base::clear 0x00ADC250. Catalog "LTextTreeWalkThrough_DestroyRange" is propagated.
// Callees: _Rb_tree<unsigned long, pair<const unsigned long, long>, ...>::_M_erase(node) @ 0x004507A1, free @
// 0x00BFEA14 (tree nodes, tree header, vector storage, the list node).
// Shape: STLport 4.x `while (first != last) erase(first++); return last;` with erase(iterator) inline (`prev->next =
// next; next->prev = prev; _Destroy(&n->_M_data); deallocate(n)`); the post-increment writes the advanced node back
// into the `first` argument slot; ~CVertexGroupBlock inline (top level of _M_erase inlined).
extern "C" void __cdecl free(void* block);                  // 0x00BFEA14

namespace std {

struct __false_type { __false_type() { } };

template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }

template <class _ForwardIterator>
void __destroy_aux(_ForwardIterator __first, _ForwardIterator __last, const __false_type&)
{
    for (; __first != __last; ++__first)
        _Destroy(&*__first);
}

template <class _ForwardIterator> inline void _Destroy(_ForwardIterator __first, _ForwardIterator __last)
{
    __destroy_aux(__first, __last, __false_type());
}
template <> inline void _Destroy<unsigned long*>(unsigned long*, unsigned long*) { }   // trivial destructor

template <class T> class allocator {
public:
    void deallocate(T* p, unsigned int) { if (p != 0) free(p); }
};

template <class T> class _STLP_alloc_proxy : public allocator<T> {
public:
    T* _M_data;
};

template <class T> class _Vector_base {
public:
    T*                    _M_start;                         // +0
    T*                    _M_finish;                        // +4
    _STLP_alloc_proxy<T>  _M_end_of_storage;                // +8
    ~_Vector_base()
    {
        if (_M_start != 0)
            _M_end_of_storage.deallocate(_M_start, _M_end_of_storage._M_data - _M_start);
    }
};

template <class T> class vector : public _Vector_base<T> {
public:
    ~vector() { _Destroy(this->_M_start, this->_M_finish); }
};

template <class T> struct less {
    bool operator()(const T& a, const T& b) const { return a < b; }
};
template <class T1, class T2> struct pair {
    T1 first;
    T2 second;
};

struct _Rb_tree_node_base {
    bool                _M_color;                           // +0x00
    _Rb_tree_node_base* _M_parent;                          // +0x04
    _Rb_tree_node_base* _M_left;                            // +0x08
    _Rb_tree_node_base* _M_right;                           // +0x0C
};
template <class V> struct _Rb_tree_node : public _Rb_tree_node_base {
    V _M_value_field;                                       // +0x10
};

template <class V> class _Rb_tree_base {
public:
    typedef _Rb_tree_node<V> _Node;
    _STLP_alloc_proxy<_Node> _M_header;                     // +0x00
    ~_Rb_tree_base() { _M_header.deallocate(_M_header._M_data, 1); }
};

template <class K, class V, class Compare> class _Rb_tree : public _Rb_tree_base<V> {
public:
    typedef _Rb_tree_node<V>    _Node;
    typedef _Node*              _Link_type;
    typedef _Rb_tree_node_base* _Base_ptr;
    unsigned int _M_node_count;                             // +0x04
    Compare      _M_key_compare;                            // +0x08

    protected:
    _Base_ptr& _M_root() const { return this->_M_header._M_data->_M_parent; }
    public:
    protected:
    _Base_ptr& _M_leftmost() const { return this->_M_header._M_data->_M_left; }
    public:
    protected:
    _Base_ptr& _M_rightmost() const { return this->_M_header._M_data->_M_right; }
    public:
    static _Link_type& _S_left(_Base_ptr x) { return (_Link_type&)x->_M_left; }
    static _Link_type& _S_right(_Base_ptr x) { return (_Link_type&)x->_M_right; }

private:
    void _M_erase(_Link_type __x);                          // 0x004507A1
public:
    void clear()
    {
        if (_M_node_count != 0) {
            _M_erase((_Link_type)_M_root());
            _M_leftmost() = this->_M_header._M_data;
            _M_root() = 0;
            _M_rightmost() = this->_M_header._M_data;
            _M_node_count = 0;
        }
    }
    ~_Rb_tree() { clear(); }
};

template <class K, class V, class Compare>
void _Rb_tree<K, V, Compare>::_M_erase(_Link_type __x)
{
    // erase without rebalancing
    while (__x != 0) {
        _M_erase(_S_right(__x));
        _Link_type __y = _S_left(__x);
        _Destroy(&__x->_M_value_field);
        this->_M_header.deallocate(__x, 1);
        __x = __y;
    }
}

template <class K, class T, class Compare = less<K> > class map {
public:
    typedef pair<const K, T> value_type;
    _Rb_tree<K, value_type, Compare> _M_t;
};

} // namespace std

class CVertexGroupBlock {                                   // Ego_r.pdb, 0x18 bytes in retail
public:
    std::vector<unsigned long>    Groups;                  // +0x00
    std::map<unsigned long, long> GroupToIndexMap;         // +0x0C
};

namespace std {

struct _List_node_base {
    _List_node_base* _M_next;                               // +0x0
    _List_node_base* _M_prev;                               // +0x4
};
template <class _Tp> struct _List_node : public _List_node_base {
    _Tp _M_data;                                            // +0x8
};

struct _List_iterator_base {
    _List_node_base* _M_node;
    _List_iterator_base(_List_node_base* __x) : _M_node(__x) { }
    void _M_incr() { _M_node = _M_node->_M_next; }
    bool operator==(const _List_iterator_base& __y) const { return _M_node == __y._M_node; }
    bool operator!=(const _List_iterator_base& __y) const { return _M_node != __y._M_node; }
};

template <class _Tp> struct _List_iterator : public _List_iterator_base {
    _List_iterator(_List_node_base* __x) : _List_iterator_base(__x) { }
    _List_iterator(const _List_iterator& __x) : _List_iterator_base(__x._M_node) { }
    _List_iterator operator++(int)
    {
        _List_iterator __tmp = *this;
        this->_M_incr();
        return __tmp;
    }
};

template <class _Tp> class _List_base {
public:
    typedef _List_node<_Tp> _Node;
    _STLP_alloc_proxy<_Node> _M_node;                       // sentinel
};

template <class _Tp> class list : public _List_base<_Tp> {
public:
    typedef _List_node<_Tp>     _Node;
    typedef _List_iterator<_Tp> iterator;

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
    iterator erase(iterator __first, iterator __last);
};

template <class _Tp>
typename list<_Tp>::iterator list<_Tp>::erase(iterator __first, iterator __last)
{
    while (__first != __last)
        erase(__first++);
    return __last;
}

template list<CVertexGroupBlock>::iterator list<CVertexGroupBlock>::erase(iterator, iterator);
} // namespace std