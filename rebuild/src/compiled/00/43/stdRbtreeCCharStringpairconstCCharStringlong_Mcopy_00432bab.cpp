// std::_Rb_tree<CCharString, pair<const CCharString, long>, ...>::_M_copy(_Link_type x, _Link_type p) -- structural copy of a red-black subtree under parent p (right subtrees recursively, the left spine iteratively)
// Leaf: ?_M_copy@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00432BAB, length 149 bytes, `ret 8` after `leave`.
// Name: STLport 4.x `_Rb_tree::_M_copy` of the map<CCharString, long> tree whose _M_create_node is the landed 0x00431773 (and
// whose _M_insert is 0x004316CF); callers: the tree's operator= 0x00432B6F and itself (0x00432BE1, 0x00432C20). The catalog's
// "CExpression_DigScript_CloneTree" is a label.
// Callees: _M_create_node(const value_type&) @ 0x00431773 (landed, out of line), itself.
// Pragmas: #pragma optimize("s", on) + #pragma optimize("y", off) -- frame pointer, `this` homed at [ebp-4].
// Shape: the same STLport body as the matched 0x0042CB90 (pair<const unsigned long, CUIState> tree), but here _M_clone_node is
// INLINED (create_node, colour copy, null children -- the two `and [..],0` scheduled before the colour store): `top` lives in ebx
// and `p` in the dead p-argument slot [ebp+0xC]; in 0x0042CB90 the clone is the out-of-line 0x0042BE11. In this size TU the
// clone needs __forceinline.
// Layout: STLport _Rb_tree_node (color +0, parent +4, left +8, right +0xc, value +0x10); CCharString is one pointer.
#pragma optimize("s", on)
#pragma optimize("y", off)
class CCharStringData;
class CCharString {
public:
    CCharStringData* PStringData;
};
namespace std {
template <class T1, class T2> struct pair {
    T1 first;
    T2 second;
};
template <class T> struct less {};
template <class T> class allocator {};
template <class V> struct _Select1st {};
struct _Rb_tree_node_base {
    bool                _M_color;                        // +0x00 (_Rb_tree_Color_type)
    _Rb_tree_node_base* _M_parent;                       // +0x04
    _Rb_tree_node_base* _M_left;                         // +0x08
    _Rb_tree_node_base* _M_right;                        // +0x0c
};
template <class V> struct _Rb_tree_node : _Rb_tree_node_base {
    V _M_value_field;                                    // +0x10
};
template <class K, class V, class KoV, class C, class A> class _Rb_tree {
public:
    typedef _Rb_tree_node<V>* _Link_type;
    _Rb_tree_node_base* _M_header;
    unsigned int        _M_node_count;
    static _Link_type& _S_left(_Rb_tree_node_base* x) { return (_Link_type&)(x->_M_left); }
    static _Link_type& _S_right(_Rb_tree_node_base* x) { return (_Link_type&)(x->_M_right); }
    static _Link_type& _S_parent(_Rb_tree_node_base* x) { return (_Link_type&)(x->_M_parent); }
    static bool& _S_color(_Rb_tree_node_base* x) { return x->_M_color; }
    _Link_type _M_create_node(const V& x);               // 0x00431773
    __forceinline _Link_type _M_clone_node(_Link_type x)
    {
        _Link_type tmp = _M_create_node(x->_M_value_field);
        _S_color(tmp) = _S_color(x);
        _S_left(tmp) = 0;
        _S_right(tmp) = 0;
        return tmp;
    }
    _Link_type _M_copy(_Link_type x, _Link_type p);
};
template <class K, class V, class KoV, class C, class A>
typename _Rb_tree<K, V, KoV, C, A>::_Link_type _Rb_tree<K, V, KoV, C, A>::_M_copy(_Link_type x, _Link_type p)
{
    _Link_type top = _M_clone_node(x);
    top->_M_parent = p;
    if (x->_M_right)
        top->_M_right = _M_copy(_S_right(x), top);
    p = top;
    x = _S_left(x);
    while (x != 0)
    {
        _Link_type y = _M_clone_node(x);
        p->_M_left = y;
        y->_M_parent = p;
        if (x->_M_right)
            y->_M_right = _M_copy(_S_right(x), y);
        p = y;
        x = _S_left(x);
    }
    return top;
}
template class _Rb_tree<CCharString, pair<const CCharString, long>, _Select1st<pair<const CCharString, long> >,
                        less<CCharString>, allocator<pair<const CCharString, long> > >;
}