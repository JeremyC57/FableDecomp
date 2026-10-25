// std::_Rb_tree::_M_lower_bound(const pair<long,long>& k) const -- STLport lower bound of the surface-composition manager's map<pair<long,long>, CEngineTextureMorph>: the first node whose key does not sort before k, the header when none
// Leaf: _M_lower_bound
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B2E830, length 53 bytes, `ret 4`, the node pointer in eax.
// Callers: map<pair<long,long>,CEngineTextureMorph>::operator[] @ 0x00B2F5E0 (call 0x00B2F5EE) and the map's other
//   lookups @ 0x00B2EA30 / 0x00B2EBB0 (their catalog names List_InitializeNode_44Bytes / CActiveFile::OnReadFinished are
//   propagated labels).
// Identity: Ego_r.pdb names map<pair<long,long>, CEngineTextureMorph> (find / lower_bound / _Lbound) and only one other
//   pair<long,long>-keyed tree (QuadricOptimiserInternals' CVertexPairContraction map); the 0x2C-byte node this tree's
//   _M_insert (0x00B2F060) allocates is 0x10 links + 8-byte key + the 0x14-byte retail CEngineTextureMorph
//   {OriginalTextureIndex, MorphID, vector<CCompositeBlendDesc> Blends}.
// Callees: none (pair's operator< is inline).
// Shape: STLport 4.6 `_M_lower_bound`: `y = header; x = root; while (x) if (!comp(key(x), k)) y = x, x = left(x); else
//   x = right(x); return y;` with the pair operator< (`x.first < y.first || (!(y.first < x.first) && x.second <
//   y.second)`) folded into the signed `jg`/`jl`/`jl` branch chain and k.first hoisted into esi.
// Label: the catalog's descriptive `RBTree_FindLE` -- this is the STLport lower bound.
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

    _Node* _M_lower_bound(const Key& k) const;
};

template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
typename _Rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::_Node*
_Rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::_M_lower_bound(const Key& k) const
{
    _Node* y = _M_header;                        // last node which is not less than k
    _Node* x = _M_root();                        // current node
    while (x != 0)
        if (!_M_key_compare(_S_key(x), k))
            y = x, x = _S_left(x);
        else
            x = _S_right(x);
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