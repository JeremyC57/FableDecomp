// std::_Rb_tree::insert_unique(const value_type& v) -- (map<unsigned __int64, NThingManager::CEntityRuntimePersistence::CPersistedDeadEntity >) STLport 4.6 unique insert: descend comparing the 64-bit keys, step back once through the out-of-line operator-- unless at begin(), insert through _M_insert when the key is new; {iterator, inserted} through the hidden pointer
// Leaf: ?insert_unique@?$_Rb_tree@_KU?$pair@$$CB_KVCPersistedDeadEntity@CEntityRuntimePersistence@NThingManager@@@std@@U?$_Select1st@U?$pair@$$CB_KVCPersistedDeadEntity@CEntityRuntimePersistence@NThingManager@@@std@@@2@U?$less@_K@2@V?$allocator@U?$pair@$$CB_KVCPersistedDeadEntity@CEntityRuntimePersistence@NThingManager@@@std@@@2@@std@@QAE?AU?$pair@U?$_Rb_tree_iterator@U?$pair@$$CB_KVCPersistedDeadEntity@CEntityRuntimePersistence@NThingManager@@@std@@U?$_Nonconst_traits@U?$pair@$$CB_KVCPersistedDeadEntity@CEntityRuntimePersistence@NThingManager@@@std@@@2@@std@@_N@2@ABU?$pair@$$CB_KVCPersistedDeadEntity@CEntityRuntimePersistence@NThingManager@@@2@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0052A130, 198 bytes (+ int3 padding), thiscall, `ret 8` (hidden pair result + value
//   reference).
// Identity (proven): std::_Rb_tree<unsigned __int64, pair<const unsigned __int64, NThingManager::CEntityRuntimePersistence::CPersistedDeadEntity >, _Select1st<...>, less<unsigned __int64>,
//   allocator<...> >::insert_unique(const value_type&); evidence: its callers are CEntityRuntimePersistence::AddPersistedDeadEntity 0x00520330, LoadFromGameStateScript 0x005248B0, the dead-creature loader 0x00524EC0 and 0x0052AE80; its _M_insert 0x00529150 allocates 0x28-byte nodes (an 8-byte key plus the 16-byte UID / GamePersistent / SectionNameCRC record) -- Ego_r.pdb NThingManager::CEntityRuntimePersistence::PersistedDeadEntities, map<unsigned __int64, CEntityRuntimePersistence::CPersistedDeadEntity>.
// Label: the manifest's "BinarySearchTree_Insert" is propagated / behavioural (family J091: the 198-byte 64-bit-key insert_unique bodies; retail
//   has no identical-code folding, so each address is its own tree type).
// Callees: _Rb_tree_iterator::operator-- 0x00527B60 (this instantiation's own out-of-line copy, defined here under
//   auto_inline(off) as in agent951 G006), _M_insert(x, y, v, w) 0x00529150 (thiscall, `ret 14h` with the hidden result).
// Model: agent951 G006 (0x00514C10, set<unsigned long>) with the 64-bit key: less<unsigned __int64> inline (high halves, then low).
namespace NThingManager {
class CEntityRuntimePersistence {                          // partial view: only the nested type is used here
public:
    class CPersistedDeadEntity {                           // 16 bytes (Ego_r.pdb)
    public:
        unsigned __int64 UID;                              // +0x00
        bool GamePersistent;                               // +0x08
        unsigned long SectionNameCRC;                      // +0x0C
    };
};
}
namespace std {
template <class _Tp> class allocator { };
template <class _Tp, class _Alloc> class _STLP_alloc_proxy : public _Alloc {
public:
    _Tp _M_data;
};
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
template <class _Pair> struct _Select1st {
    const typename _Pair::first_type& operator()(const _Pair& __x) const { return __x.first; }
};
struct _Rb_tree_node_base {
    typedef _Rb_tree_node_base* _Base_ptr;
    bool      _M_color;                                    // +0x00
    _Base_ptr _M_parent;                                   // +0x04
    _Base_ptr _M_left;                                     // +0x08
    _Base_ptr _M_right;                                    // +0x0C
};
template <class _Value> struct _Rb_tree_node : public _Rb_tree_node_base {
    _Value _M_value_field;                                 // +0x10
};
template <class _Dummy> class _Rb_global {
public:
    typedef _Rb_tree_node_base* _Base_ptr;
    static _Base_ptr __cdecl _M_decrement(_Base_ptr __x);
};
typedef _Rb_global<bool> _Rb_global_inst;
template <class _Dummy>
_Rb_tree_node_base* __cdecl _Rb_global<_Dummy>::_M_decrement(_Rb_tree_node_base* _M_node)
{
    if (_M_node->_M_color == false && _M_node->_M_parent->_M_parent == _M_node)
        _M_node = _M_node->_M_right;
    else if (_M_node->_M_left != 0) {
        _Base_ptr __y = _M_node->_M_left;
        while (__y->_M_right != 0)
            __y = __y->_M_right;
        _M_node = __y;
    }
    else {
        _Base_ptr __y = _M_node->_M_parent;
        while (_M_node == __y->_M_left) {
            _M_node = __y;
            __y = __y->_M_parent;
        }
        _M_node = __y;
    }
    return _M_node;
}
struct _Rb_tree_base_iterator {
    _Rb_tree_node_base* _M_node;
};
inline bool operator==(const _Rb_tree_base_iterator& __x, const _Rb_tree_base_iterator& __y) { return __x._M_node == __y._M_node; }
template <class _Tp> struct _Nonconst_traits { };
template <class _Value, class _Traits> struct _Rb_tree_iterator : public _Rb_tree_base_iterator {
    typedef _Rb_tree_iterator<_Value, _Traits> _Self;
    typedef _Rb_tree_iterator<_Value, _Nonconst_traits<_Value> > iterator;
    _Rb_tree_iterator() { }
    _Rb_tree_iterator(_Rb_tree_node<_Value>* __x) { _M_node = __x; }
    _Rb_tree_iterator(const iterator& __it) { _M_node = __it._M_node; }
    _Self& operator--();
};
#pragma auto_inline(off)
template <class _Value, class _Traits> __declspec(noinline)
_Rb_tree_iterator<_Value, _Traits>& _Rb_tree_iterator<_Value, _Traits>::operator--()
{                                                          // 0x00527B60 (this instantiation's own copy)
    _M_node = _Rb_global_inst::_M_decrement(_M_node);
    return *this;
}
#pragma auto_inline(on)
template <class _Key, class _Value, class _KeyOfValue, class _Compare, class _Alloc>
class _Rb_tree {
public:
    typedef _Rb_tree_node_base* _Base_ptr;
    typedef _Rb_tree_node<_Value> _Node;
    typedef _Node* _Link_type;
    typedef _Key key_type;
    typedef _Value value_type;
    typedef _Rb_tree_iterator<value_type, _Nonconst_traits<value_type> > iterator;
protected:
    _STLP_alloc_proxy<_Link_type, allocator<_Node> > _M_header;   // +0x00
    unsigned int _M_node_count;                            // +0x04
    _Compare     _M_key_compare;                           // +0x08
    _Link_type& _M_root() const { return (_Link_type&)_M_header._M_data->_M_parent; }
    _Link_type& _M_leftmost() const { return (_Link_type&)_M_header._M_data->_M_left; }
    static _Link_type& _S_left(_Base_ptr __x) { return (_Link_type&)(__x->_M_left); }
    static _Link_type& _S_right(_Base_ptr __x) { return (_Link_type&)(__x->_M_right); }
    static const _Key& _S_key(_Base_ptr __x) { return _KeyOfValue()(((_Link_type)__x)->_M_value_field); }
private:
    iterator _M_insert(_Base_ptr __x, _Base_ptr __y, const value_type& __v, _Base_ptr __w = 0);   // 0x00529150
public:
    iterator begin() { return iterator(_M_leftmost()); }
    pair<iterator, bool> insert_unique(const value_type& __v);
};
template <class _Key, class _Value, class _KeyOfValue, class _Compare, class _Alloc>
pair<typename _Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>::iterator, bool>
_Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>::insert_unique(const _Value& __v)
{
    _Link_type __y = this->_M_header._M_data;
    _Link_type __x = _M_root();
    bool __comp = true;
    while (__x != 0) {
        __y = __x;
        __comp = _M_key_compare(_KeyOfValue()(__v), _S_key(__x));
        __x = __comp ? _S_left(__x) : _S_right(__x);
    }
    iterator __j = iterator(__y);
    if (__comp)
        if (__j == begin())
            return pair<iterator, bool>(_M_insert(/* __x*/ __y, __y, __v), true);
        else
            --__j;
    if (_M_key_compare(_S_key(__j._M_node), _KeyOfValue()(__v)))
        return pair<iterator, bool>(_M_insert(__x, __y, __v), true);
    return pair<iterator, bool>(__j, false);
}
typedef pair<const unsigned __int64, NThingManager::CEntityRuntimePersistence::CPersistedDeadEntity > TValue_0052a130;
typedef _Rb_tree<unsigned __int64, TValue_0052a130, _Select1st<TValue_0052a130 >, less<unsigned __int64>, allocator<TValue_0052a130 > > TTree_0052a130;
template pair<TTree_0052a130::iterator, bool> TTree_0052a130::insert_unique(const TValue_0052a130&);
} // namespace std