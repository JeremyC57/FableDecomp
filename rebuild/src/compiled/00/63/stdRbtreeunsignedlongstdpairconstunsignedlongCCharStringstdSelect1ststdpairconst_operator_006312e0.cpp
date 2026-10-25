// std::_Rb_tree<unsigned long, pair<const unsigned long, CCharString>, _Select1st<...>, less<unsigned long>, allocator<...> >::operator=(const _Rb_tree&)
// -- STLport tree assignment: unless self-assigned, clear, then either reset the header for an empty source or clone the
// source's root under the header, re-derive leftmost/rightmost and copy the node count
// Leaf: ??4?$_Rb_tree@KU?$pair@$$CBKVCCharString@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x006312e0, 178 bytes, thiscall `ret 4`.
// Renamed from the generic CRetailTree_006310E0::operator= ("VC6-era Dinkumware _Tree::operator=", FINDINGS chunk 382:
//   the body is STLport 4.x _Rb_tree::operator= with clear() inlined; the old manifest label LTextTreeWalkThrough_* is
//   propagated).
// Identity: the clone callee 0x006310E0 is the landed _M_copy of this tree; CUIDef::operator= 0x00631720 runs this body on
//   +0x1A8 = ActionMap (struct_layouts_egor CUIDef::ActionMap, map<unsigned long, CCharString>, PDB +0x1C4); the other callers are CUIDef::CUIDef 0x006303C0 and 0x00631420. Retail has no identical-code folding: the third CUIDef map, ActionMapAliases
//   (map<unsigned long, unsigned long>, +0x1B4), is assigned through a different body (0x00432A35).
// Callees: _M_erase 0x00630D90 (the recursive subtree free inside clear()), _M_copy 0x006310E0.
// Layout: STLport _Rb_tree {_M_header +0, _M_node_count +4, empty _M_key_compare +8}; node colour +0, parent +4,
//   left +8, right +0xC, value +0x10.
// Shape: STLport `operator=` verbatim: `clear(); _M_node_count = 0;` (both zero stores survive), the key compare
//   copy is empty, `_M_root() = _M_copy(__x._M_root(), _M_header._M_data)` keeps the header in edi across the call,
//   then the _S_minimum / _S_maximum walks.
namespace std {

template <class T> class allocator { };
template <class T1, class T2> struct pair {
    T1 first;                                       // +0x00
    T2 second;                                      // +0x04
};
template <class Pair> struct _Select1st;
template <class T> struct less { };

struct _Rb_tree_node_base {
    typedef _Rb_tree_node_base* _Base_ptr;
    bool      _M_color;                             // +0x00
    _Base_ptr _M_parent;                            // +0x04
    _Base_ptr _M_left;                              // +0x08
    _Base_ptr _M_right;                             // +0x0C
    static _Base_ptr _S_minimum(_Base_ptr x) { while (x->_M_left != 0) x = x->_M_left; return x; }
    static _Base_ptr _S_maximum(_Base_ptr x) { while (x->_M_right != 0) x = x->_M_right; return x; }
};

template <class Value> struct _Rb_tree_node : public _Rb_tree_node_base {
    Value _M_value_field;                           // +0x10
};

template <class V, class A> struct _STLP_alloc_proxy : public A {
    V _M_data;
};

template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
class _Rb_tree {
public:
    typedef _Rb_tree_node<Value> _Node;
    typedef _Node* _Link_type;
    typedef _Rb_tree_node_base* _Base_ptr;

    _STLP_alloc_proxy<_Node*, allocator<_Node> > _M_header;     // +0x00
    unsigned int _M_node_count;                                 // +0x04
    Compare _M_key_compare;                                     // +0x08 (empty)

    _Base_ptr& _M_root() const { return this->_M_header._M_data->_M_parent; }
    _Base_ptr& _M_leftmost() const { return this->_M_header._M_data->_M_left; }
    _Base_ptr& _M_rightmost() const { return this->_M_header._M_data->_M_right; }

    void _M_erase(_Link_type x);                                // 0x00630D90
    _Link_type _M_copy(_Link_type x, _Link_type p);             // 0x006310E0
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
    _Rb_tree& operator=(const _Rb_tree& __x);
};

template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
_Rb_tree<Key, Value, KeyOfValue, Compare, Alloc>&
_Rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::operator=(const _Rb_tree<Key, Value, KeyOfValue, Compare, Alloc>& __x)
{
    if (this != &__x) {
        // Note that _Key may be a constant type.
        clear();
        _M_node_count = 0;
        _M_key_compare = __x._M_key_compare;
        if (__x._M_root() == 0) {
            _M_root() = 0;
            _M_leftmost() = this->_M_header._M_data;
            _M_rightmost() = this->_M_header._M_data;
        }
        else {
            _M_root() = _M_copy((_Link_type)__x._M_root(), this->_M_header._M_data);
            _M_leftmost() = _Rb_tree_node_base::_S_minimum(_M_root());
            _M_rightmost() = _Rb_tree_node_base::_S_maximum(_M_root());
            _M_node_count = __x._M_node_count;
        }
    }
    return *this;
}

}

class CCharString {                                 // 4 bytes: the string-data pointer
public:
    void* PStringData;
};

typedef std::pair<const unsigned long, CCharString> TActionValue;
template std::_Rb_tree<unsigned long, TActionValue, std::_Select1st<TActionValue>, std::less<unsigned long>,
                       std::allocator<TActionValue> >&
std::_Rb_tree<unsigned long, TActionValue, std::_Select1st<TActionValue>, std::less<unsigned long>,
              std::allocator<TActionValue> >::operator=(const std::_Rb_tree<unsigned long, TActionValue,
              std::_Select1st<TActionValue>, std::less<unsigned long>, std::allocator<TActionValue> >&);
