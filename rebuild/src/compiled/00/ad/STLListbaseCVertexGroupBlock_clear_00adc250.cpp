// _STL::_List_base<CVertexGroupBlock>::clear() -- free every node of the optimiser's block list (each block's group map and group vector destroyed in place) and relink the sentinel to itself
// Leaf: ?clear@?$_List_base@VCVertexGroupBlock
// Retail: Fable.exe (TLC, VC7.1) @ 0x00ADC250, 0xB7 bytes (+ int3 padding), thiscall, plain `ret`.
// Identity: the list node is {next +0, prev +4, CVertexGroupBlock +8} (map count at node +0x18, map header +0x14,
// vector start +8): Ego_r.pdb CVertexGroupBlockOptimiser::Blocks, list<CVertexGroupBlock>. Catalog
// "LTextTreeWalkThrough_Cleanup" is propagated.
// Callees: _Rb_tree<unsigned long, pair<const unsigned long, long>, ...>::_M_erase(node) @ 0x004507A1, free @
// 0x00BFEA14 (tree nodes, tree header, vector storage, the list node).
// Shape: STLport 4.x `cur = node->next; while (cur != node) { tmp = cur; cur = cur->next; _Destroy(&tmp->_M_data);
// deallocate(tmp); } node->next = node; node->prev = node;` -- ~CVertexGroupBlock as in __destroy_aux 0x00ADC190
// (top level of _M_erase inlined); `this` and `tmp` live in the two frame words.
extern "C" void __cdecl free(void* block);                  // 0x00BFEA14

namespace _STL {

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

    _Base_ptr& _M_root() const { return this->_M_header._M_data->_M_parent; }
    _Base_ptr& _M_leftmost() const { return this->_M_header._M_data->_M_left; }
    _Base_ptr& _M_rightmost() const { return this->_M_header._M_data->_M_right; }
    static _Link_type& _S_left(_Base_ptr x) { return (_Link_type&)x->_M_left; }
    static _Link_type& _S_right(_Base_ptr x) { return (_Link_type&)x->_M_right; }

    void _M_erase(_Link_type __x);                          // 0x004507A1
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

} // namespace _STL

class CVertexGroupBlock {                                   // Ego_r.pdb, 0x18 bytes in retail
public:
    _STL::vector<unsigned long>    Groups;                  // +0x00
    _STL::map<unsigned long, long> GroupToIndexMap;         // +0x0C
};

namespace _STL {

struct _List_node_base {
    _List_node_base* _M_next;                               // +0x0
    _List_node_base* _M_prev;                               // +0x4
};
template <class _Tp> struct _List_node : public _List_node_base {
    _Tp _M_data;                                            // +0x8
};

template <class _Tp> class _List_base {
public:
    typedef _List_node<_Tp> _Node;
    _STLP_alloc_proxy<_Node> _M_node;                       // sentinel
    void clear();
};

template <class _Tp>
void _List_base<_Tp>::clear()
{
    _List_node<_Tp>* __cur = (_List_node<_Tp>*)this->_M_node._M_data->_M_next;
    while (__cur != this->_M_node._M_data) {
        _List_node<_Tp>* __tmp = __cur;
        __cur = (_List_node<_Tp>*)__cur->_M_next;
        _Destroy(&__tmp->_M_data);
        this->_M_node.deallocate(__tmp, 1);
    }
    this->_M_node._M_data->_M_next = this->_M_node._M_data;
    this->_M_node._M_data->_M_prev = this->_M_node._M_data;
}

template class _List_base<CVertexGroupBlock>;
} // namespace _STL