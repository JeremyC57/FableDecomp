// std::_Rb_tree<ETableSprites,pair<const ETableSprites,long>,_Select1st<..>,less<ETableSprites>,allocator<..> >::insert_unique(const value_type& v) -- STLport unique insert for the UI def's sprite map: walk down to the insertion parent, step back once when the walk ended going left, and insert only when no equal key exists
// Leaf: ?insert_unique@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00632990, 143 bytes (+ int3 padding), `ret 8` (hidden pair<iterator,bool>
// result, &v).
// Identity: the only caller is the binary map reader 0x00632830 (GFSerialiseMapBinaryIn, call at 0x006328E4, reached from the
// CUIDef Sprites transfer 0x006327A0), so the tree is map<NUISystem::ETableSprites, long>: a signed key at node +0x10
// (`setl`, `jge`). The catalog label "RBTree_find_insertion_bounds" is descriptive.
// Callees: `_Rb_global<bool>::_M_decrement` @ 0x004294E2 (STLport's out-of-line predecessor step, __cdecl, `add esp,4`;
// catalogued as "RBTree::GetMaxNode"), _Rb_tree::_M_insert(x, y, v, w = 0) @ 0x00632A30 (out of line, iterator by hidden
// pointer).
// Shape: STLport 4.x insert_unique verbatim: the key of v is loaded once before the walk, `comp` lives in cl, `y`/`j` in
// edi/eax; the begin() test compares with header->_M_left; the first _M_insert passes (y, y, v) (STLport's `/* __x */ __y`),
// the second (x, y, v); both return their iterator through a temporary homed in the dead `&v` argument slot. The
// pair<iterator, bool> result is built in the caller's hidden slot.
namespace std {

template <class T1, class T2> struct pair {
    typedef T1 first_type;
    typedef T2 second_type;
    T1 first;
    T2 second;
    pair(const T1& a, const T2& b) : first(a), second(b) { }
    pair(const pair& p) : first(p.first), second(p.second) { }
};

template <class T> struct less { bool operator()(const T& a, const T& b) const { return a < b; } };
template <class Pair> struct _Select1st { const typename Pair::first_type& operator()(const Pair& x) const { return x.first; } };

struct _Rb_tree_node_base {
    typedef bool _Color_type;
    typedef _Rb_tree_node_base* _Base_ptr;
    _Color_type _M_color;                           // +0x00
    _Base_ptr   _M_parent;                          // +0x04
    _Base_ptr   _M_left;                            // +0x08
    _Base_ptr   _M_right;                           // +0x0C
};

template <class Value> struct _Rb_tree_node : public _Rb_tree_node_base {
    Value _M_value_field;                           // +0x10
};

template <class Dummy> struct _Rb_global {
    static _Rb_tree_node_base* __cdecl _M_decrement(_Rb_tree_node_base* x);   // 0x004294E2
};

struct _Rb_tree_base_iterator {
    _Rb_tree_node_base* _M_node;
    void _M_decrement() { _M_node = _Rb_global<bool>::_M_decrement(_M_node); }
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

template <class T> class allocator { };

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

private:
    iterator _M_insert(_Base_ptr x, _Base_ptr y, const value_type& v, _Base_ptr w = 0);
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
            return pair<iterator, bool>(_M_insert(y, y, v), true);
        else
            --j;
    if (_M_key_compare(_S_key(j._M_node), KeyOfValue()(v)))
        return pair<iterator, bool>(_M_insert(x, y, v), true);
    return pair<iterator, bool>(j, false);
}

}

namespace NUISystem {
enum ETableSprites { TABLE_SPRITE_0 };
}

template std::pair<std::_Rb_tree<NUISystem::ETableSprites, std::pair<const NUISystem::ETableSprites, long>,
                                 std::_Select1st<std::pair<const NUISystem::ETableSprites, long> >,
                                 std::less<NUISystem::ETableSprites>,
                                 std::allocator<std::pair<const NUISystem::ETableSprites, long> > >::iterator, bool>
std::_Rb_tree<NUISystem::ETableSprites, std::pair<const NUISystem::ETableSprites, long>,
              std::_Select1st<std::pair<const NUISystem::ETableSprites, long> >,
              std::less<NUISystem::ETableSprites>,
              std::allocator<std::pair<const NUISystem::ETableSprites, long> > >::insert_unique(
    const std::pair<const NUISystem::ETableSprites, long>& v);