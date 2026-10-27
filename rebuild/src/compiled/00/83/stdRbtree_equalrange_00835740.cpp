// std::_Rb_tree::equal_range(const void*& k) -- (set<void*>) the [lower_bound(k), upper_bound(k)) iterator pair
// Leaf: ?equal_range@?$_Rb_tree@PAXPAXU?$_Identity@PAX@std@@U?$less@PAX@2@V?$allocator@PAX@2@@std@@QAE?AU?$pair@U?$_Rb_tree_iterator@PAXU?$_Nonconst_traits@PAX@std@@@std@@U12@@2@ABQAX@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00835740, 83 bytes, thiscall, `ret 8` (the pair by hidden pointer, then k).
// Identity (proven): its only caller is set<void*>::erase(const key_type&) 0x00835960 (bound ?Erase@CSuspenderSet@@QAEKABQAX@Z) that CThingAICreature::SuspendAI 0x00833A10 runs on its suspender set: Ego_r.pdb CThingAICreature::Suspenders is set<void*>.
// Callees: none (both bound walks inline).
// Shape (family M089, representative 0x004C4080): STLport 4.6 `pair<iterator, iterator>(lower_bound(__x), upper_bound(__x))`
//   -- the arguments evaluated right to left, so the upper-bound walk (keep x while k < key(x)) runs first, then the
//   lower-bound walk (keep x while !(key(x) < k)); pointer keys compare unsigned; speed TU.
namespace std {
template <class _Tp> class allocator { };
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc { public: _Value _M_data; };
template <class _T1, class _T2> struct pair {
    typedef _T1 first_type;
    pair(const _T1& __a, const _T2& __b) : first(__a), second(__b) { }
    _T1 first;
    _T2 second;
};
template <class _Tp> struct less { bool operator()(const _Tp& __x, const _Tp& __y) const { return __x < __y; } };
template <class _Tp> struct _Identity { const _Tp& operator()(const _Tp& __x) const { return __x; } };
template <class _Pair> struct _Select1st { const typename _Pair::first_type& operator()(const _Pair& __x) const { return __x.first; } };
struct _Rb_tree_node_base {
    typedef _Rb_tree_node_base* _Base_ptr;
    bool _M_color;                                        // +0x00
    _Base_ptr _M_parent;                                  // +0x04
    _Base_ptr _M_left;                                    // +0x08
    _Base_ptr _M_right;                                   // +0x0C
};
template <class _Value> struct _Rb_tree_node : public _Rb_tree_node_base { _Value _M_value_field; };
template <class _Tp> struct _Nonconst_traits { };
template <class _Value, class _Traits> struct _Rb_tree_iterator {
    _Rb_tree_iterator(_Rb_tree_node_base* __x) : _M_node(__x) { }
    _Rb_tree_iterator(const _Rb_tree_iterator& __it) : _M_node(__it._M_node) { }
    _Rb_tree_node_base* _M_node;
};
template <class _Key, class _Value, class _KeyOfValue, class _Compare, class _Alloc>
class _Rb_tree {
public:
    typedef _Rb_tree_node_base* _Base_ptr;
    typedef _Rb_tree_node<_Value> _Node;
    typedef _Rb_tree_iterator<_Value, _Nonconst_traits<_Value> > iterator;
    _STLP_alloc_proxy<_Node*, _Node, allocator<_Node> > _M_header;   // +0x00
    unsigned int _M_node_count;                                       // +0x04
    _Compare _M_key_compare;                                          // +0x08
    _Base_ptr _M_root() const { return this->_M_header._M_data->_M_parent; }
    static const _Key& _S_key(_Base_ptr __x) { return _KeyOfValue()(((_Node*)__x)->_M_value_field); }
    _Base_ptr _M_lower_bound(const _Key& __k) const
    {
        _Base_ptr __y = this->_M_header._M_data;
        _Base_ptr __x = _M_root();
        while (__x != 0)
            if (!_M_key_compare(_S_key(__x), __k))
                __y = __x, __x = __x->_M_left;
            else
                __x = __x->_M_right;
        return __y;
    }
    _Base_ptr _M_upper_bound(const _Key& __k) const
    {
        _Base_ptr __y = this->_M_header._M_data;
        _Base_ptr __x = _M_root();
        while (__x != 0)
            if (_M_key_compare(__k, _S_key(__x)))
                __y = __x, __x = __x->_M_left;
            else
                __x = __x->_M_right;
        return __y;
    }
    iterator lower_bound(const _Key& __x) { return iterator(_M_lower_bound(__x)); }
    iterator upper_bound(const _Key& __x) { return iterator(_M_upper_bound(__x)); }
    pair<iterator, iterator> equal_range(const _Key& __x) { return pair<iterator, iterator>(lower_bound(__x), upper_bound(__x)); }
};
template class _Rb_tree<void*, void*, _Identity<void*>, less<void*>, allocator<void*> >;
}