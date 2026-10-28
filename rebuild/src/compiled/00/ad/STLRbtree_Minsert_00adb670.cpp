// std::_Rb_tree::_M_insert(_Rb_tree_node_base* x, _Rb_tree_node_base* y, const value_type& v, _Rb_tree_node_base* w) -- link a new node for the optimiser's group-info map (map<unsigned long, CVertexGroupBlockOptimiser::CGroupInfo>) below y, fix the header's leftmost/rightmost/root, rebalance
// Leaf: ?_M_insert@?$_Rb_tree@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00ADB670, 0xCB bytes (+ int3 padding), thiscall, `ret 14h` (hidden iterator
// result, x, y, v, w), returns the result pointer.
// Identity: 0x1C-byte node (0x10 link block + 12-byte value) keyed by an unsigned long at +0x10 (`cmp eax,[ebx+10h]
// ; jb` = less<unsigned long>): Ego_r.pdb map<unsigned long, CVertexGroupBlockOptimiser::CGroupInfo> (CGroupInfo
// {ulong Count; long BlockIndex}), the tree whose landed insert_unique is 0x00ADBA40. Catalog
// "std_set_insert_uint_triple" is descriptive.
// Callees: malloc @ 0x00BFEA0E (_M_create_node, allocate(1)), _Rb_global<bool>::_Rebalance(x, root) @ 0x0042971D
// (__cdecl).
// Shape: STLport 4.x `if (y == header || (w == 0 && (x != 0 || comp(key(v), key(y))))) { z = create(v); left(y) = z;
// if (y == header) { root = z; rightmost = z; } else if (y == leftmost) leftmost = z; } else { z = create(v);
// right(y) = z; if (y == rightmost) rightmost = z; } parent(z) = y; left(z) = right(z) = 0; _Rebalance(z, root);
// ++count; return iterator(z);` -- each branch has its own inlined _M_create_node (placement-new null test, the
// value copied as three dwords).
extern "C" void* __cdecl malloc(unsigned int size);         // 0x00BFEA0E

inline void* operator new(unsigned int, void* p) { return p; }   // placement new (STLport <new>)

class CVertexGroupBlockOptimiser {
public:
    class CGroupInfo {                       // Ego_r.pdb, 8 bytes
    public:
        unsigned long Count;                 // +0x0
        long          BlockIndex;            // +0x4
        CGroupInfo() : Count(0), BlockIndex(-1) { }
    };
};

namespace std {

template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }

template <class T> class allocator {
public:
    T* allocate(unsigned int n) { return n != 0 ? (T*)malloc(n * sizeof(T)) : 0; }
};
template <class T> class less {
public:
    bool operator()(const T& a, const T& b) const { return a < b; }
};
template <class T1, class T2> class pair {
public:
    typedef T1 first_type;
    T1 first;
    T2 second;
    pair(const T1& a, const T2& b) : first(a), second(b) { }
    pair(const pair& p) : first(p.first), second(p.second) { }
};
template <class P> struct _Select1st {
    const typename P::first_type& operator()(const P& x) const { return x.first; }
};
struct _Rb_tree_node_base {
    bool                _M_color;                                // +0x00
    _Rb_tree_node_base* _M_parent;                               // +0x04
    _Rb_tree_node_base* _M_left;                                 // +0x08
    _Rb_tree_node_base* _M_right;                                // +0x0C
};
template <class V> struct _Rb_tree_node : public _Rb_tree_node_base {
    V _M_value_field;                                            // +0x10
};
template <class _Dummy> struct _Rb_global {
    static void __cdecl _Rebalance(_Rb_tree_node_base* __x, _Rb_tree_node_base*& __root);   // 0x0042971D
};
typedef _Rb_global<bool> _Rb_global_inst;

template <class V> struct _Rb_tree_iterator {
    _Rb_tree_node_base* _M_node;
    _Rb_tree_iterator() { }
    _Rb_tree_iterator(_Rb_tree_node_base* x) : _M_node(x) { }
    _Rb_tree_iterator(const _Rb_tree_iterator& it) : _M_node(it._M_node) { }
};
template <class V, class A> class _Rb_tree_base {
public:
    typedef _Rb_tree_node<V> _Node;
    struct _Header_proxy : public allocator<_Node> {
        _Node* _M_data;
    } _M_header;
};
template <class K, class V, class KeyOfValue, class Compare, class A> class _Rb_tree : public _Rb_tree_base<V, A> {
public:
    typedef _Rb_tree_node<V>       _Node;
    typedef _Node*                 _Link_type;
    typedef _Rb_tree_node_base*    _Base_ptr;
    typedef _Rb_tree_iterator<V>   iterator;
    unsigned int _M_node_count;
    Compare      _M_key_compare;

    protected:
    _Base_ptr& _M_root() const { return this->_M_header._M_data->_M_parent; }
    public:
    protected:
    _Base_ptr& _M_leftmost() const { return this->_M_header._M_data->_M_left; }
    public:
    protected:
    _Base_ptr& _M_rightmost() const { return this->_M_header._M_data->_M_right; }
    public:
    static _Base_ptr& _S_left(_Base_ptr x) { return x->_M_left; }
    static _Base_ptr& _S_right(_Base_ptr x) { return x->_M_right; }
    static _Base_ptr& _S_parent(_Base_ptr x) { return x->_M_parent; }
    static const K& _S_key(_Base_ptr x) { return KeyOfValue()(((_Link_type)x)->_M_value_field); }

protected:
    _Link_type _M_create_node(const V& __x)
    {
        _Link_type __tmp = this->_M_header.allocate(1);
        _Construct(&__tmp->_M_value_field, __x);
        return __tmp;
    }
public:

private:
    iterator _M_insert(_Base_ptr __x, _Base_ptr __y, const V& __v, _Base_ptr __w = 0);
};

template <class K, class V, class KeyOfValue, class Compare, class A>
typename _Rb_tree<K, V, KeyOfValue, Compare, A>::iterator
_Rb_tree<K, V, KeyOfValue, Compare, A>::_M_insert(_Base_ptr __x_, _Base_ptr __y_, const V& __v, _Base_ptr __w_)
{
    _Link_type __w = (_Link_type)__w_;
    _Link_type __x = (_Link_type)__x_;
    _Link_type __y = (_Link_type)__y_;
    _Link_type __z;

    if (__y == this->_M_header._M_data ||
        (__w == 0 && (__x != 0 || _M_key_compare(KeyOfValue()(__v), _S_key(__y))))) {
        __z = _M_create_node(__v);
        _S_left(__y) = __z;
        if (__y == this->_M_header._M_data) {
            _M_root() = __z;
            _M_rightmost() = __z;
        }
        else if (__y == _M_leftmost())
            _M_leftmost() = __z;
    }
    else {
        __z = _M_create_node(__v);
        _S_right(__y) = __z;
        if (__y == _M_rightmost())
            _M_rightmost() = __z;
    }
    _S_parent(__z) = __y;
    _S_left(__z) = 0;
    _S_right(__z) = 0;
    _Rb_global_inst::_Rebalance(__z, _M_root());
    ++_M_node_count;
    return iterator(__z);
}

typedef pair<const unsigned long, CVertexGroupBlockOptimiser::CGroupInfo> SGroupInfoEntry;
template class _Rb_tree<unsigned long, SGroupInfoEntry, _Select1st<SGroupInfoEntry>, less<unsigned long>, allocator<SGroupInfoEntry> >;
} // namespace std