// std::_Rb_tree::_M_find(const pair<long,long>& k) const -- STLport find of the surface-composition manager's map<pair<long,long>, CEngineTextureMorph>: the node whose key equals k, the header when absent
// Leaf: _M_find
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B2E630, length 82 bytes, `ret 4`, the node pointer in eax.
// Callers: CEngineSurfaceCompositionManager::UpdateDiskCacheTexture @ 0x00B2D330 (call 0x00B2D415) and two more map
//   lookups @ 0x00B2E8B5 / 0x00B2E9C5 (their catalog labels are propagated).
// Identity: same tree as the lower bound @ 0x00B2E830 / _M_insert @ 0x00B2F060 (Ego_r.pdb: map<pair<long,long>,
//   CEngineTextureMorph>; 0x2C-byte node = links + 8-byte key + the 0x14-byte retail CEngineTextureMorph).
// Callees: none (pair's operator< is inline).
// Shape: STLport 4.6 `_M_find` = the lower-bound walk, then `if (y == header || comp(k, key(y))) y = header; return y;`
//   (landed relative: 0x0098FE70); the header stays in edi, k in ebx, k.first hoisted into esi for the loop only.
// Label: the catalog's descriptive `RBTree_FindLE` -- this is the STLport find (0x00B2E830 is the lower bound).
namespace std {

template <class T1, class T2> struct pair {
    typedef T1 first_type;
    typedef T2 second_type;
    T1 first;
    T2 second;
};

template <class T1, class T2>
inline bool operator<(const pair<T1, T2>& x, const pair<T1, T2>& y)
{
    return x.first < y.first || (!(y.first < x.first) && x.second < y.second);
}

template <class T> struct less {
    bool operator()(const T& x, const T& y) const { return x < y; }
};

template <class T> class allocator { };

template <class Pair> struct _Select1st {
    const typename Pair::first_type& operator()(const Pair& x) const { return x.first; }
};

struct _Rb_tree_node_base {
    bool                _M_color;                // +0x00
    _Rb_tree_node_base* _M_parent;               // +0x04
    _Rb_tree_node_base* _M_left;                 // +0x08
    _Rb_tree_node_base* _M_right;                // +0x0C
};

template <class Value> struct _Rb_tree_node : _Rb_tree_node_base {
    Value _M_value_field;                        // +0x10
};

template <class Key, class Value, class KeyOfValue, class Compare, class Alloc> class _Rb_tree {
public:
    typedef _Rb_tree_node<Value> _Node;
    _Node*        _M_header;                     // +0x00 (_STLP_alloc_proxy::_M_data)
    unsigned long _M_node_count;                 // +0x04
    Compare       _M_key_compare;                // +0x08

    static _Node* _S_left(_Rb_tree_node_base* x) { return (_Node*)x->_M_left; }
    static _Node* _S_right(_Rb_tree_node_base* x) { return (_Node*)x->_M_right; }
    static Value& _S_value(_Rb_tree_node_base* x) { return ((_Node*)x)->_M_value_field; }
    static const Key& _S_key(_Rb_tree_node_base* x) { return KeyOfValue()(_S_value(x)); }
    _Node* _M_root() const { return (_Node*)_M_header->_M_parent; }

    _Node* _M_find(const Key& k) const;
};

template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
typename _Rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::_Node*
_Rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::_M_find(const Key& k) const
{
    _Node* y = _M_header;                        // last node which is not less than k
    _Node* x = _M_root();                        // current node
    while (x != 0)
        if (!_M_key_compare(_S_key(x), k))
            y = x, x = _S_left(x);
        else
            x = _S_right(x);
    if (y == _M_header || _M_key_compare(k, _S_key(y)))
        y = _M_header;
    return y;
}

}   // namespace std

struct CCompositeBlendDesc;
struct CCompositeBlendDescVector {               // retail STLport vector<CCompositeBlendDesc>, 12 bytes
    CCompositeBlendDesc* _M_start;
    CCompositeBlendDesc* _M_finish;
    CCompositeBlendDesc* _M_end_of_storage;
};
class CEngineTextureMorph {
public:
    long                      OriginalTextureIndex;   // +0x00
    long                      MorphID;                // +0x04
    CCompositeBlendDescVector Blends;                 // +0x08
};

typedef std::pair<long, long> CMorphKey;
typedef std::pair<const CMorphKey, CEngineTextureMorph> CMorphValue;

template class std::_Rb_tree<CMorphKey, CMorphValue, std::_Select1st<CMorphValue>, std::less<CMorphKey>,
    std::allocator<CMorphValue> >;