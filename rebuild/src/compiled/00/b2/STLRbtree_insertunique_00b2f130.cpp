// std::_Rb_tree::insert_unique(const pair<const pair<long,long>, CEngineTextureMorph>& v) -- STLport unique insert of the surface-composition manager's map<pair<long,long>, CEngineTextureMorph>: walk down to the insertion parent, step back once when the walk ended going left, and insert only when no equal key exists
// Leaf: ?insert_unique@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B2F130, length 175 bytes, `ret 8` (hidden pair<iterator,bool> result, &v).
// Callers: two sites in the map's insert paths @ 0x00B2F388 / 0x00B2F4AB (inside 0x00B2F240, catalogued RBTree_InsertOrFind).
// Identity: same tree as _M_find 0x00B2E630 / _M_lower_bound 0x00B2E830 (Ego_r.pdb map<pair<long,long>,
//   CEngineTextureMorph>; the 0x2C-byte node of _M_insert 0x00B2F060 = links + key + 0x14-byte retail CEngineTextureMorph).
// Callees: _Rb_tree_base_iterator::_M_decrement() @ 0x00B2E5E0 (thiscall on &j, this TU's out-of-line copy; catalogued
//   RBTree_FindNext), _Rb_tree::_M_insert(x, y, v, w = 0) @ 0x00B2F060 (iterator through the hidden pointer).
// Shape: STLport 4.x insert_unique: `comp = key(v) < key(x)` with pair's operator< folded into `jg`/`jl`/`jl` and comp
//   held in al; `iterator j(y)` lives in v's dead argument slot (v itself is kept in ebp), and both _M_insert calls write
//   their iterator result into that same slot (`lea ecx,[esp+18h]` / `[esp+28h]`); the begin() test passes y twice.
//   That slot sharing only reproduces with _M_insert spelled as an explicit iterator out-pointer (`*_M_insert(&j, ...)`,
//   the same thiscall hidden-result ABI); the by-value `pair(_M_insert(y, y, v), true)` form gives the temporary its own
//   `push ecx` frame slot (177v175).
// Label: the catalog's descriptive `RBTree_FindInsertPosition` -- this is the STLport insert_unique.
namespace std {

template <class T1, class T2> struct pair {
    typedef T1 first_type;
    typedef T2 second_type;
    T1 first;
    T2 second;
    pair(const T1& a, const T2& b) : first(a), second(b) { }
    pair(const pair& p) : first(p.first), second(p.second) { }
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
    typedef bool _Color_type;
    typedef _Rb_tree_node_base* _Base_ptr;
    _Color_type _M_color;                         // +0x00
    _Base_ptr   _M_parent;                        // +0x04
    _Base_ptr   _M_left;                          // +0x08
    _Base_ptr   _M_right;                         // +0x0C
};

template <class Value> struct _Rb_tree_node : public _Rb_tree_node_base {
    Value _M_value_field;                         // +0x10
};

struct _Rb_tree_base_iterator {
    _Rb_tree_node_base* _M_node;
    void _M_decrement();                          // 0x00B2E5E0 (out of line in this TU)
};

template <class Value> struct _Rb_tree_iterator : public _Rb_tree_base_iterator {
    _Rb_tree_iterator() { }
    _Rb_tree_iterator(_Rb_tree_node_base* x) { _M_node = x; }
    _Rb_tree_iterator(const _Rb_tree_iterator& it) { _M_node = it._M_node; }
    _Rb_tree_iterator& operator--() { _M_decrement(); return *this; }
    bool operator==(const _Rb_tree_iterator& y) const { return _M_node == y._M_node; }
};

template <class V, class A> struct _STLP_alloc_proxy : public A {
    V _M_data;
};

template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
class _Rb_tree {
public:
    typedef _Rb_tree_node<Value> _Node;
    typedef _Rb_tree_node_base* _Base_ptr;
    typedef _Node* _Link_type;
    typedef Value value_type;
    typedef _Rb_tree_iterator<Value> iterator;

    _STLP_alloc_proxy<_Node*, allocator<_Node> > _M_header;     // +0x00
    unsigned int _M_node_count;                                 // +0x04
    Compare _M_key_compare;                                     // +0x08

    _Link_type& _M_root() const { return (_Link_type&)_M_header._M_data->_M_parent; }
    _Link_type& _M_leftmost() const { return (_Link_type&)_M_header._M_data->_M_left; }
    static _Link_type& _S_left(_Base_ptr x) { return (_Link_type&)(x->_M_left); }
    static _Link_type& _S_right(_Base_ptr x) { return (_Link_type&)(x->_M_right); }
    static const Key& _S_key(_Base_ptr x) { return KeyOfValue()(((_Link_type)x)->_M_value_field); }

    iterator begin() { return iterator(_M_leftmost()); }

    // _M_insert returns its iterator through the hidden pointer; spelled as the explicit out-pointer (same ABI) so the
    // result lands in j's slot as in retail (GOTCHAS wave 17).
private:
    iterator* _M_insert(iterator* result, _Base_ptr x, _Base_ptr y, const value_type& v, _Base_ptr w);   // 0x00B2F060
public:
    pair<iterator, bool> insert_unique(const value_type& v);
};

template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
pair<typename _Rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::iterator, bool>
_Rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::insert_unique(const value_type& v)
{
    _Link_type y = this->_M_header._M_data;
    _Link_type x = _M_root();
    bool comp = true;
    while (x != 0) {
        y = x;
        comp = _M_key_compare(KeyOfValue()(v), _S_key(x));
        x = comp ? _S_left(x) : _S_right(x);
    }
    iterator j = iterator(y);
    if (comp)
        if (j == begin())
            return pair<iterator, bool>(*_M_insert(&j, y, y, v, 0), true);
        else
            --j;
    if (_M_key_compare(_S_key(j._M_node), KeyOfValue()(v)))
        return pair<iterator, bool>(*_M_insert(&j, x, y, v, 0), true);
    return pair<iterator, bool>(j, false);
}

}   // namespace std

class CCompositeBlendDesc;
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
typedef std::_Rb_tree<CMorphKey, CMorphValue, std::_Select1st<CMorphValue>, std::less<CMorphKey>,
    std::allocator<CMorphValue> > CMorphTree;

template std::pair<CMorphTree::iterator, bool> CMorphTree::insert_unique(const CMorphValue& v);