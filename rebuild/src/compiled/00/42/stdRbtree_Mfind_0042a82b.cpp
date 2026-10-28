// std::_Rb_tree::_M_find(const key_type& k) const -- (map<unsigned long, NUISystem::CUIState> (NUISystem::CComponent::States)) STLport exact-key lookup over an unsigned 32-bit key: walk from the root keeping the last node whose key is not less than k, then return it unless it is the header or k is less than its key (then the header, i.e. end())
// Leaf: ?_M_find@?$_Rb_tree@KU?$pair@$$CBKVCUIState@NUISystem@@@std@@U?$_Select1st@U?$pair@$$CBKVCUIState@NUISystem@@@std@@@2@U?$less@K@2@V?$allocator@U?$pair@$$CBKVCUIState@NUISystem@@@std@@@2@@std@@ABEPAU?$_Rb_tree_node@U?$pair@$$CBKVCUIState@NUISystem@@@std@@@2@ABK@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0042A82B, 54 bytes (unaligned entry, size-optimised TU), thiscall, `ret 4`;
//   returns the node pointer (header node pointer = end()).
// Identity (proven): map<unsigned long, NUISystem::CUIState> (NUISystem::CComponent::States); evidence: its callers are the find front ends 0x0042B402 / 0x0042B6DC (`ret 8`, the iterator through a hidden pointer); the landed NUISystem::CChangingStateComponent::GetState 0x0052CEB0 and GetCurrentState 0x0052CF10 call 0x0042B402 on CComponent::States (Ego_r.pdb map<unsigned long, NUISystem::CUIState>).
// Label: the manifest's "BinarySearchTree_FindLessOrEqual" is propagated / behavioural (family H045: six byte-identical bodies, each its own
//   COMDAT: retail has no identical-code folding, so each address is one tree type).
// Shape: STLport 4.6 _tree.c `_Rb_tree::_M_find` (the out-of-line node lookup behind map::find), the landed G008 model
//   0x0042A879 with an unsigned key (`jb`/`jae` where the signed G008 keys give `jl`/`jge`).
// Node layout: color +0, parent +4, left +8, right +0xC, key at +0x10 (first word of the value); tree: header node
//   pointer +0, node count +4, comparator +8. Only the key is read; the mapped type only names the instantiation.
#pragma optimize("s", on)
namespace NUISystem {
class CComponent;
class CList;
class CUIState { public: unsigned char _body[0x24]; };   // Ego_r.pdb 36 bytes (only its size matters here)
}

namespace std {

template <class _Tp> class allocator { };

template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
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
    bool                _M_color;
    _Rb_tree_node_base* _M_parent;
    _Rb_tree_node_base* _M_left;
    _Rb_tree_node_base* _M_right;
};
template <class _Value> struct _Rb_tree_node : public _Rb_tree_node_base {
    _Value _M_value_field;
};

template <class _Tp, class _Alloc> class _Rb_tree_base {
public:
    _STLP_alloc_proxy<_Rb_tree_node<_Tp>*, _Rb_tree_node<_Tp>, allocator<_Rb_tree_node<_Tp> > > _M_header;
};

template <class _Key, class _Value, class _KeyOfValue, class _Compare, class _Alloc = allocator<_Value> >
class _Rb_tree : public _Rb_tree_base<_Value, _Alloc> {
public:
    typedef _Rb_tree_node<_Value>* _Link_type;
    unsigned int _M_node_count;
    _Compare _M_key_compare;
    static const _Key& _S_key(_Link_type __x) { return _KeyOfValue()(__x->_M_value_field); }
    static _Link_type _S_left(_Link_type __x) { return (_Link_type)__x->_M_left; }
    static _Link_type _S_right(_Link_type __x) { return (_Link_type)__x->_M_right; }
    protected:
    _Link_type _M_root() const { return (_Link_type)this->_M_header._M_data->_M_parent; }
    public:
    private:
    _Rb_tree_node<_Value>* _M_find(const _Key& __k) const;
    public:
};

template <class _Key, class _Value, class _KeyOfValue, class _Compare, class _Alloc>
_Rb_tree_node<_Value>* _Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>::_M_find(const _Key& __k) const
{
    _Link_type __y = this->_M_header._M_data;      // last node which is not less than __k
    _Link_type __x = _M_root();                    // current node
    while (__x != 0)
        if (!_M_key_compare(_S_key(__x), __k))
            __y = __x, __x = _S_left(__x);
        else
            __x = _S_right(__x);
    if (__y == this->_M_header._M_data || _M_key_compare(__k, _S_key(__y)))
        __y = this->_M_header._M_data;
    return __y;
}

typedef unsigned long _TreeKey;
typedef pair<const _TreeKey, NUISystem::CUIState > _TreeValue;
template class _Rb_tree<_TreeKey, _TreeValue, _Select1st<_TreeValue>, less<_TreeKey>, allocator<_TreeValue> >;

} // namespace std