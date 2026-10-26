// std::_Rb_tree::equal_range(const key_type& k) -- (map<void*, CBulletTimeManager::CBulletTimeInfo>) STLport 4.6: the [lower_bound(k), upper_bound(k)) node pair, both walks from the root under the header
// Leaf: ?equal_range@?$_Rb_tree@PAXU?$pair@QAXVCBulletTimeInfo@CBulletTimeManager@@@std@@U?$_Select1st@U?$pair@QAXVCBulletTimeInfo@CBulletTimeManager@@@std@@@2@U?$less@PAX@2@V?$allocator@U?$pair@QAXVCBulletTimeInfo@CBulletTimeManager@@@std@@@2@@std@@QAE?AU?$pair@U?$_Rb_tree_iterator@U?$pair@QAXVCBulletTimeInfo@CBulletTimeManager@@@std@@U?$_Nonconst_traits@U?$pair@QAXVCBulletTimeInfo@CBulletTimeManager@@@std@@@2@@std@@U12@@2@ABQAX@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004C6900, 83 bytes (+ int3 padding to 96), thiscall `ret 8` (hidden pair result, key by reference).
// Identity (proven): map<void*, CBulletTimeManager::CBulletTimeInfo>; evidence: its only caller is the tree's erase(const key&) 0x004C6B60, called by 0x004C6140 -- landed descriptively as a time-multiplier controller's RemoveRequest; it erases a pointer key from the 12-byte map at +0 and sets the byte at +0x0D when the map is empty, which is Ego_r.pdb's CBulletTimeManager (BulletTimeInfoMap +0, Override / UseDefaultMultiplier +0x10 / +0x11 behind a 16-byte debug map, so +0x0C / +0x0D in retail) and its RemoveBulletTimeSlowdownMultiplier(void*); Ego_r.pdb also emits an equal_range (_Eqrange) on map<void*, CBulletTimeManager::CBulletTimeInfo>, the only map in that class
// Label: the manifest's "RBTree_FindInsertPosition" is propagated (family H068: six equal_range bodies over 4-byte keys compared unsigned;
//   retail has no identical-code folding, so each address is one instantiation).
// Shape: the pair's arguments are evaluated right to left, so the upper_bound walk runs first and lands in `second`; both walks
//   are the inline _M_lower_bound / _M_upper_bound of STLport 4.6 sharing the root load.
class CBulletTimeManager {
public:
    class CBulletTimeInfo {                           // 8 bytes
    public:
        float Multiplier;                             // +0x00
        float SoundPitch;                             // +0x04
    };
};

namespace std {

template <class _Tp> class allocator { };             // empty: only the node allocator's type is used here

template <class _T1, class _T2> struct pair {
    typedef _T1 first_type;
    typedef _T2 second_type;
    _T1 first;
    _T2 second;
    pair(const _T1& __a, const _T2& __b) : first(__a), second(__b) { }
};

template <class _Tp> struct less {
    bool operator()(const _Tp& __x, const _Tp& __y) const { return __x < __y; }
};
template <class _Tp> struct _Identity {
    const _Tp& operator()(const _Tp& __x) const { return __x; }
};
template <class _Pair> struct _Select1st {
    const typename _Pair::first_type& operator()(const _Pair& __x) const { return __x.first; }
};

template <class _Tp, class _Alloc> class _STLP_alloc_proxy : public _Alloc {
public:
    _Tp _M_data;
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

struct _Rb_tree_base_iterator {
    typedef _Rb_tree_node_base* _Base_ptr;
    _Base_ptr _M_node;
};

template <class _Tp> struct _Nonconst_traits { typedef _Tp value_type; typedef _Tp& reference; typedef _Tp* pointer; };

template <class _Value, class _Traits> struct _Rb_tree_iterator : public _Rb_tree_base_iterator {
    typedef _Rb_tree_iterator<_Value, _Traits> _Self;
    typedef _Rb_tree_node<_Value>* _Link_type;
    _Rb_tree_iterator() { }
    _Rb_tree_iterator(_Link_type __x) { _M_node = __x; }
    _Rb_tree_iterator(const _Self& __it) { _M_node = __it._M_node; }
};

template <class _Key, class _Value, class _KeyOfValue, class _Compare, class _Alloc>
class _Rb_tree {
public:
    typedef _Key key_type;
    typedef _Rb_tree_node_base* _Base_ptr;
    typedef _Rb_tree_node<_Value> _Node;
    typedef _Node* _Link_type;
    typedef _Value value_type;
    typedef _Rb_tree_iterator<value_type, _Nonconst_traits<value_type> > iterator;

    _STLP_alloc_proxy<_Link_type, allocator<_Node> > _M_header;   // +0x00
    unsigned int _M_node_count;                                    // +0x04
    _Compare     _M_key_compare;                                   // +0x08

    _Link_type& _M_root() const { return (_Link_type&)this->_M_header._M_data->_M_parent; }
    static _Link_type& _S_left(_Link_type __x) { return (_Link_type&)(__x->_M_left); }
    static _Link_type& _S_right(_Link_type __x) { return (_Link_type&)(__x->_M_right); }
    static const _Key& _S_key(_Link_type __x) { return _KeyOfValue()(__x->_M_value_field); }

    _Link_type _M_lower_bound(const key_type& __k) const
    {
        _Link_type __y = this->_M_header._M_data;   /* Last node which is not less than __k. */
        _Link_type __x = _M_root();                  /* Current node. */
        while (__x != 0)
            if (!_M_key_compare(_S_key(__x), __k))
                __y = __x, __x = _S_left(__x);
            else
                __x = _S_right(__x);
        return __y;
    }
    _Link_type _M_upper_bound(const key_type& __k) const
    {
        _Link_type __y = this->_M_header._M_data;   /* Last node which is greater than __k. */
        _Link_type __x = _M_root();                  /* Current node. */
        while (__x != 0)
            if (_M_key_compare(__k, _S_key(__x)))
                __y = __x, __x = _S_left(__x);
            else
                __x = _S_right(__x);
        return __y;
    }
    iterator lower_bound(const key_type& __x) { return iterator(_M_lower_bound(__x)); }
    iterator upper_bound(const key_type& __x) { return iterator(_M_upper_bound(__x)); }
    pair<iterator, iterator> equal_range(const key_type& __x)
    {
        return pair<iterator, iterator>(lower_bound(__x), upper_bound(__x));
    }
};
}   // namespace std

template std::pair<std::_Rb_tree<void*, std::pair<void* const, CBulletTimeManager::CBulletTimeInfo >, std::_Select1st<std::pair<void* const, CBulletTimeManager::CBulletTimeInfo > >, std::less<void* >, std::allocator<std::pair<void* const, CBulletTimeManager::CBulletTimeInfo > > >::iterator, std::_Rb_tree<void*, std::pair<void* const, CBulletTimeManager::CBulletTimeInfo >, std::_Select1st<std::pair<void* const, CBulletTimeManager::CBulletTimeInfo > >, std::less<void* >, std::allocator<std::pair<void* const, CBulletTimeManager::CBulletTimeInfo > > >::iterator>
    std::_Rb_tree<void*, std::pair<void* const, CBulletTimeManager::CBulletTimeInfo >, std::_Select1st<std::pair<void* const, CBulletTimeManager::CBulletTimeInfo > >, std::less<void* >, std::allocator<std::pair<void* const, CBulletTimeManager::CBulletTimeInfo > > >::equal_range(void* const&);