// std::_Rb_tree::erase(iterator __first, iterator __last) -- (set<NScreenEffect::CScreenEffectPriorityStack::CStackEntry*>) erase a range: the whole tree is a clear(), else node by node
// Leaf: ?erase@?$_Rb_tree@PAUCStackEntry@CScreenEffectPriorityStack@NScreenEffect@@PAV123@U?$_Identity@PAUCStackEntry@CScreenEffectPriorityStack@NScreenEffect@@@std@@U?$less@PAUCStackEntry@CScreenEffectPriorityStack@NScreenEffect@@@5@V?$allocator@PAUCStackEntry@CScreenEffectPriorityStack@NScreenEffect@@@5@@std@@QAEXU?$_Rb_tree_iterator@PAUCStackEntry@CScreenEffectPriorityStack@NScreenEffect@@U?$_Nonconst_traits@PAUCStackEntry@CScreenEffectPriorityStack@NScreenEffect@@@std@@@2@0@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00883E50, 189 bytes, thiscall, `ret 8`.
// Identity (proven): its only caller is 0x00884710, an erase(CStackEntry* const&) (bound with the stand-in CKeySet) called by CScreenEffectPriorityStack::DeleteRadialBlurEntry / DeleteDisplacementEntry; Ego_r.pdb CScreenEffectPriorityStack's RadialBlurKeys / DisplacementKeys / ScreenFilterKeys are all set<CStackEntry*>.
// Callees: this tree's _M_erase(_Rb_tree_node*) 0x00882BC0, std::_Rb_global<bool>::_Rebalance_for_erase 0x0042A34E (cdecl),
//   free 0x00BFEA14.
// Shape (family M118, representative 0x00528EF0): STLport 4.6 `if (__first._M_node == _M_leftmost() && __last._M_node ==
//   _M_header._M_data) clear(); else while (__first != __last) erase(__first++);` -- clear() inline (`if (_M_node_count != 0)
//   { _M_erase(_M_root()); _M_leftmost() = _M_header; _M_root() = 0; _M_rightmost() = _M_header; _M_node_count = 0; }`),
//   _M_increment inline in the post-increment, erase(iterator) as in family M063 with a trivial value destructor.
extern "C" void __cdecl free(void*);                      // 0x00BFEA14
namespace NScreenEffect { class CScreenEffectPriorityStack { public: struct CStackEntry; }; }
namespace std {
template <class _Tp> class allocator { public: allocator() { } void deallocate(_Tp* __p, unsigned int) { if (__p) free(__p); } };
template <class _Tp> struct less { };
template <class _T1, class _T2> struct pair { _T1 first; _T2 second; };
template <class _Pair> struct _Select1st { };
template <class _Tp> struct _Identity { };
template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }
struct _Rb_tree_node_base {
    typedef _Rb_tree_node_base* _Base_ptr;
    bool _M_color;
    _Base_ptr _M_parent;
    _Base_ptr _M_left;
    _Base_ptr _M_right;
};
template <class _Value> struct _Rb_tree_node : public _Rb_tree_node_base { _Value _M_value_field; };
template <class _Dummy> class _Rb_global {
public:
    static _Rb_tree_node_base* __cdecl _Rebalance_for_erase(_Rb_tree_node_base* __z, _Rb_tree_node_base*& __root,
                                                            _Rb_tree_node_base*& __leftmost, _Rb_tree_node_base*& __rightmost);   // 0x0042A34E
};
typedef _Rb_global<bool> _Rb_global_inst;
template <class _Tp> struct _Nonconst_traits { };
struct _Rb_tree_base_iterator {
    typedef _Rb_tree_node_base::_Base_ptr _Base_ptr;
    _Base_ptr _M_node;
    void _M_increment()
    {
        if (_M_node->_M_right != 0) {
            _M_node = _M_node->_M_right;
            while (_M_node->_M_left != 0)
                _M_node = _M_node->_M_left;
        }
        else {
            _Base_ptr __y = _M_node->_M_parent;
            while (_M_node == __y->_M_right) {
                _M_node = __y;
                __y = __y->_M_parent;
            }
            if (_M_node->_M_right != __y)
                _M_node = __y;
        }
    }
};
template <class _Value, class _Traits> struct _Rb_tree_iterator : public _Rb_tree_base_iterator {
    _Rb_tree_iterator() { }
    _Rb_tree_iterator(const _Rb_tree_iterator& __it) { _M_node = __it._M_node; }
    _Rb_tree_iterator& operator++() { _M_increment(); return *this; }
    _Rb_tree_iterator operator++(int) { _Rb_tree_iterator __tmp = *this; _M_increment(); return __tmp; }
    bool operator!=(const _Rb_tree_iterator& __y) const { return _M_node != __y._M_node; }
};
template <class _Tp, class _Alloc> class _STLP_alloc_proxy : public _Alloc { public: _Tp _M_data; };
template <class _Key, class _Value, class _KeyOfValue, class _Compare, class _Alloc = allocator<_Value> > class _Rb_tree {
public:
    typedef _Rb_tree_node_base* _Base_ptr;
    typedef _Rb_tree_node<_Value>* _Link_type;
    typedef _Rb_tree_iterator<_Value, _Nonconst_traits<_Value> > iterator;
    _Base_ptr& _M_root() const { return _M_header._M_data->_M_parent; }
    _Base_ptr& _M_leftmost() const { return _M_header._M_data->_M_left; }
    _Base_ptr& _M_rightmost() const { return _M_header._M_data->_M_right; }
    void _M_erase(_Link_type __x);                        // 0x00882BC0
    void clear()
    {
        if (_M_node_count != 0) {
            _M_erase((_Link_type)_M_root());
            _M_leftmost() = _M_header._M_data;
            _M_root() = 0;
            _M_rightmost() = _M_header._M_data;
            _M_node_count = 0;
        }
    }
    void erase(iterator __position)
    {
        _Link_type __y = (_Link_type)_Rb_global_inst::_Rebalance_for_erase(__position._M_node,
            _M_header._M_data->_M_parent, _M_header._M_data->_M_left, _M_header._M_data->_M_right);
        _Destroy(&__y->_M_value_field);
        _M_header.deallocate(__y, 1);
        --_M_node_count;
    }
    void erase(iterator __first, iterator __last)
    {
        if (__first._M_node == _M_header._M_data->_M_left && __last._M_node == _M_header._M_data)
            clear();
        else
            while (__first != __last) erase(__first++);
    }
    _STLP_alloc_proxy<_Base_ptr, allocator<_Rb_tree_node<_Value> > > _M_header;
    unsigned int _M_node_count;
    _Compare _M_key_compare;
};
template void _Rb_tree<NScreenEffect::CScreenEffectPriorityStack::CStackEntry*, NScreenEffect::CScreenEffectPriorityStack::CStackEntry*, _Identity<NScreenEffect::CScreenEffectPriorityStack::CStackEntry*>, less<NScreenEffect::CScreenEffectPriorityStack::CStackEntry*>, allocator<NScreenEffect::CScreenEffectPriorityStack::CStackEntry*> >::erase(_Rb_tree<NScreenEffect::CScreenEffectPriorityStack::CStackEntry*, NScreenEffect::CScreenEffectPriorityStack::CStackEntry*, _Identity<NScreenEffect::CScreenEffectPriorityStack::CStackEntry*>, less<NScreenEffect::CScreenEffectPriorityStack::CStackEntry*>, allocator<NScreenEffect::CScreenEffectPriorityStack::CStackEntry*> >::iterator, _Rb_tree<NScreenEffect::CScreenEffectPriorityStack::CStackEntry*, NScreenEffect::CScreenEffectPriorityStack::CStackEntry*, _Identity<NScreenEffect::CScreenEffectPriorityStack::CStackEntry*>, less<NScreenEffect::CScreenEffectPriorityStack::CStackEntry*>, allocator<NScreenEffect::CScreenEffectPriorityStack::CStackEntry*> >::iterator);
}