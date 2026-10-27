// std::_Rb_tree::_M_copy(_Link_type __x, _Link_type __p) -- (map<EOpinionReactionType, float>) STLport structural tree copy: clone __x under __p, recurse into every right subtree and walk down the left spine; returns the cloned top
// Leaf: ?_M_copy@?$_Rb_tree@W4EOpinionReactionType@@U?$pair@$$CBW4EOpinionReactionType@@M@std@@U?$_Select1st@U?$pair@$$CBW4EOpinionReactionType@@M@std@@@3@U?$less@W4EOpinionReactionType@@@3@V?$allocator@U?$pair@$$CBW4EOpinionReactionType@@M@std@@@3@@std@@AAEPAU?$_Rb_tree_node@U?$pair@$$CBW4EOpinionReactionType@@M@std@@@2@PAU32@0@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0067D3B0, 151 bytes, thiscall, `ret 8`.
// Identity (proven): recursive, and its only other caller is the tree operator= 0x0067D2F0, which the landed
//   COpinionReactionManagerDef::operator= 0x0067CE50 (vtable 0x0125B7DC RTTI) calls on the member at retail +0x60:
//   with the retail 12-byte containers that is COpinionReactionManagerDef::PreReactionDelay, Ego_r.pdb map<EOpinionReactionType,
//   float> (AttitudeBlur +0x3C, then AttitudeCondition, TargetingCondition, PreReactionDelay, ToleranceToBeingHit at
//   +0x48/+0x54/+0x60/+0x6C, the four bool maps sharing one operator= 0x0045DD2C at +0x78..+0x9C). The landed callers
//   carry the stand-in CRetailTree_0067d3b0::Copy and the landed _M_create_node 0x0067B110 the CUnproven*_0067b110 types.
// Callees: _Rb_tree::_M_create_node(const value_type&) 0x0067B110 (out of line in this TU).
// Shape (family M189, representative 0x0067D0F0): STLport 4.6 _tree.c `_M_copy` verbatim (exceptions off) with
//   `_M_clone_node` inline around the out-of-line `_M_create_node` (the landed 0x00AD3500 spelling, which inlines it). Speed TU.
enum EOpinionReactionType { OPINION_REACTION_TYPE_NONE = 0 };
namespace std {
template <class _Tp> class allocator {
public:
    allocator() { }
};
template <class _Tp, class _Alloc> class _STLP_alloc_proxy : public _Alloc {
public:
    _Tp _M_data;
};
template <class _T1, class _T2> struct pair {
    typedef _T1 first_type;
    typedef _T2 second_type;
    _T1 first;
    _T2 second;
};
template <class _Tp> struct less {
    bool operator()(const _Tp& __x, const _Tp& __y) const { return __x < __y; }
};
template <class _Pair> struct _Select1st {
    const typename _Pair::first_type& operator()(const _Pair& __x) const { return __x.first; }
};
struct _Rb_tree_node_base {
    typedef bool _Color_type;
    typedef _Rb_tree_node_base* _Base_ptr;
    _Color_type _M_color;                               // +0x00
    _Base_ptr   _M_parent;                              // +0x04
    _Base_ptr   _M_left;                                // +0x08
    _Base_ptr   _M_right;                               // +0x0C
};
template <class _Value> struct _Rb_tree_node : public _Rb_tree_node_base {
    _Value _M_value_field;                              // +0x10
};
template <class _Key, class _Value, class _KeyOfValue, class _Compare, class _Alloc>
class _Rb_tree {
public:
    typedef _Rb_tree_node_base* _Base_ptr;
    typedef _Rb_tree_node<_Value> _Node;
    typedef _Node* _Link_type;
    typedef _Value value_type;

    _STLP_alloc_proxy<_Link_type, allocator<_Node> > _M_header;   // +0x00
    unsigned int _M_node_count;                                    // +0x04
    _Compare     _M_key_compare;                                   // +0x08
protected:
    _Link_type _M_create_node(const value_type& __x);              // 0x0067B110
    _Link_type _M_clone_node(_Link_type __x)
    {
        _Link_type __tmp = _M_create_node(__x->_M_value_field);
        __tmp->_M_color = __x->_M_color;
        __tmp->_M_left = 0;
        __tmp->_M_right = 0;
        return __tmp;
    }
public:
    static _Link_type& _S_left(_Base_ptr __x) { return (_Link_type&)(__x->_M_left); }
    static _Link_type& _S_right(_Base_ptr __x) { return (_Link_type&)(__x->_M_right); }
private:
    _Link_type _M_copy(_Link_type __x, _Link_type __p);
};
template <class _Key, class _Value, class _KeyOfValue, class _Compare, class _Alloc>
typename _Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>::_Link_type
_Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>::_M_copy(_Link_type __x, _Link_type __p)
{
    // structural copy: __x and __p must be non-null
    _Link_type __top = _M_clone_node(__x);
    __top->_M_parent = __p;
    if (__x->_M_right)
        __top->_M_right = _M_copy(_S_right(__x), __top);
    __p = __top;
    __x = _S_left(__x);
    while (__x != 0) {
        _Link_type __y = _M_clone_node(__x);
        __p->_M_left = __y;
        __y->_M_parent = __p;
        if (__x->_M_right)
            __y->_M_right = _M_copy(_S_right(__x), __y);
        __p = __y;
        __x = _S_left(__x);
    }
    return __top;
}
}
typedef std::pair<const EOpinionReactionType, float> TReactionValue;
typedef std::_Rb_tree<EOpinionReactionType, TReactionValue, std::_Select1st<TReactionValue>, std::less<EOpinionReactionType>,
                      std::allocator<TReactionValue> > TReactionTree;
template TReactionTree::_Link_type TReactionTree::_M_copy(TReactionTree::_Link_type, TReactionTree::_Link_type);