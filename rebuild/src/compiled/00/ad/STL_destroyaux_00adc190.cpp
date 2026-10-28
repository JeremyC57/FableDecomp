// std::__destroy_aux(CVertexGroupBlock* first, CVertexGroupBlock* last, const __false_type&) -- destroy a run of vertex-group blocks: each one's group -> index map (nodes, header) then its group vector storage
// Flags: /Gr
// Leaf: ??$__destroy_aux@PAVCVertexGroupBlock
// Retail: Fable.exe (TLC, VC7.1) @ 0x00ADC190, 0x8D bytes, __fastcall (first ecx, last edx), `ret 4` (the
// __false_type reference).
// Identity: the 0x18-byte element is Ego_r.pdb CVertexGroupBlock (Groups vector<unsigned long> +0, GroupToIndexMap
// map<unsigned long, long> +0xC; header +0xC, count +0x10) whose tree erase 0x004507A1 is the one the landed
// CVertexGroupBlockOptimiser::CalcCurrentBlockSegments 0x00AD9B20 calls. Catalog
// "LTextTreeWalkThrough_DestroyRange" is propagated.
// Callees: _Rb_tree<unsigned long, pair<const unsigned long, long>, ...>::_M_erase(node) @ 0x004507A1 (thiscall, the
// right subtrees), free @ 0x00BFEA14 (nodes, the tree header, the vector storage).
// Shape: STLport `for (; first != last; ++first) _Destroy(&*first);` -- ~CVertexGroupBlock runs ~map
// (`~_Rb_tree() { clear(); }` with the top level of the recursive `_M_erase` inlined: `while (x) { _M_erase(right(x));
// y = left(x); deallocate(x); x = y; }`, the header reset, then `~_Rb_tree_base` frees the header) and then
// ~vector (`if (start) free(start)`). `last` lives in the `push ecx` frame word across the loop.
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
template void __destroy_aux<CVertexGroupBlock*>(CVertexGroupBlock*, CVertexGroupBlock*, const __false_type&);
} // namespace std