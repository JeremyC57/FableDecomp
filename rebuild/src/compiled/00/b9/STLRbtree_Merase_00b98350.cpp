// std::_Rb_tree::_M_erase(_Rb_tree_node* x) -- STLport erase-without-rebalancing for CEngineScene::MapList (map<long, map<long, CCountedPointer<CEngineSceneMap> > >): recurse right, destroy the row's inner map (clear + free its header), free the node, continue left
// Leaf: ?_M_erase@?$_Rb_tree@JU?$pair@$$CBJV?$map@J
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B98350, 114 bytes, thiscall, `ret 4`.
// Identity: recursive (0x00B9836B) and called by the outer tree's destructor / clear (0x00B94BBF, 0x00B985A2, 0x00B98812,
//   0x00B988A2; the landed CRetailTree_erase00b98350 destructors). The 0x20-byte node holds a long key and an STLport
//   map {header, count, less<long>} whose own _M_erase is 0x00B97580: Ego_r.pdb CEngineScene::MapList. Catalog
//   "Utility_DestroyRefcountedTreeNode" is descriptive.
// Callees: the inner map's _M_erase 0x00B97580 (out of line), free @ 0x00BFEA14 (the inner header, null-tested; the
//   node, known non-null).
// Shape: STLport 4.x `while (x != 0) { _M_erase(_S_right(x)); y = _S_left(x); _Destroy(&x->_M_value_field);
//   deallocate(x); x = y; }` with the inner map's destructor inline (`clear()`: count test, _M_erase(root), header
//   relinked through the re-read header pointer, count zeroed; then the header freed). `this` parks in the `push ecx`
//   frame word. Same source shape as the landed list<CVertexGroupBlock>::erase 0x00ADC4E0 tree teardown.
extern "C" void __cdecl free(void* block);                  // 0x00BFEA14

class CEngineSceneMap;

template <class T> class CCountedPointer {                  // {Data, Info}
public:
    T*    Data;
    long* Info;
};

namespace std {

template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }

template <class T> class allocator {
public:
    void deallocate(T* p, unsigned int) { if (p != 0) free(p); }
};

template <class T> class _STLP_alloc_proxy : public allocator<T> {
public:
    T* _M_data;
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

private:
    void _M_erase(_Link_type __x);
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

typedef pair<const long, CCountedPointer<CEngineSceneMap> > TSceneMapValue;
template <> void _Rb_tree<long, TSceneMapValue, less<long> >::_M_erase(_Link_type __x);   // 0x00B97580 (inner map)

typedef map<long, CCountedPointer<CEngineSceneMap> > TSceneMapRow;
template void _Rb_tree<long, pair<const long, TSceneMapRow>, less<long> >::_M_erase(_Link_type);

} // namespace std