// std::_Rb_tree<K, pair<const K, T>, ...>::insert_unique(iterator position, const value_type& v) -- STLport 4.6 hinted unique insert of map<NUISystem::CComponent*, pair<CWideString, long> > (CTCInventoryTrade NumBuyable/Sellable/WantedThings): begin / end / interior hint cases, falling back on the unhinted insert_unique when the hint does not bracket v
// Leaf: ?insert_unique@?$_Rb_tree@PAVCComponent@NUISystem@@U?$pair@QAVCComponent@NUISystem@@U?$pair@VCWideString@@J@std@@@std@@U?$_Select1st@U?$pair@QAVCComponent@NUISystem@@U?$pair@VCWideString@@J@std@@@std@@@4@U?$less@PAVCComponent@NUISystem@@@4@V?$allocator@U?$pair@QAVCComponent@NUISystem@@U?$pair@VCWideString@@J@std@@@std@@@4@@std@@QAE?AU?$_Rb_tree_iterator@U?$pair@QAVCComponent@NUISystem@@U?$pair@VCWideString@@J@std@@@std@@U?$_Nonconst_traits@U?$pair@QAVCComponent@NUISystem@@U?$pair@VCWideString@@J@std@@@std@@@2@@2@U32@ABU?$pair@QAVCComponent@NUISystem@@U?$pair@VCWideString@@J@std@@@2@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005B2B76, 319 bytes, thiscall `ret 0Ch` (hidden iterator result, position, v);
//   size-optimised tree TU with frame pointer (odd entry).
// Identity (proven): _Rb_tree<NUISystem::CComponent*, pair<NUISystem::CComponent* const, pair<CWideString, long> >, ...>::insert_unique(iterator, const value_type&) of
//   map<NUISystem::CComponent*, pair<CWideString, long> > (CTCInventoryTrade NumBuyable/Sellable/WantedThings); evidence: the instance's only caller is the map::insert forwarder 0x005B2EC4, called only by the landed std::map<CComponent*, pair<CWideString, long> >::operator[] 0x005B3100 (Ego_r.pdb CTCInventoryTrade NumBuyableThings / NumSellableThings / NumWantedThings); node 0x1C with an out-of-line value copy; node 0x1C bytes.
// Label: the manifest's "CMemoryAllocatorVariableSize_InsertOrFind_Script" is propagated across family H039 (six hinted map inserts).
// Callees (this instantiation): _M_insert 0x005B24CD (node creator 0x005B204B, `push 1Ch`: 0x10 + sizeof(value_type)),
//   insert_unique(v) 0x005B2555; _Rb_global<bool>::_M_increment 0x004292C0 / _M_decrement 0x004294E2 (cdecl, shared).
// Shape: STLport 4.6 _tree.c verbatim; the _Rb_global step functions are defined in the TU (as STLport's template
//   statics are) so VC7.1 sees they write no memory and keeps v's key in edi across the `++after` call (declared-only
//   steps reload it: 317v319); the node key is spelled `node->_M_value_field.first` (the _KeyOfValue temporary loads it
//   before v's key), as in the landed /O2 sibling 0x00AD5030.
#pragma optimize("y", off)
#pragma optimize("s", on)
namespace NUISystem {
class CComponent;
class CList;
class CUIState { public: unsigned char _body[0x24]; };   // Ego_r.pdb 36 bytes (only its size matters here)
}
class CWideString { public: void* _data; };      // one pointer (only its size matters here)
namespace std {
template <class _Tp> class allocator { public: allocator() { } };
template <class _Tp, class _Alloc> class _STLP_alloc_proxy : public _Alloc { public: _Tp _M_data; };
template <class _T1, class _T2> struct pair {
    typedef _T1 first_type; typedef _T2 second_type;
    _T1 first; _T2 second;
    pair(const _T1& __a, const _T2& __b) : first(__a), second(__b) { }
};
template <class _Tp> struct less { bool operator()(const _Tp& __x, const _Tp& __y) const { return __x < __y; } };
template <class _Pair> struct _Select1st {
    const typename _Pair::first_type& operator()(const _Pair& __x) const { return __x.first; }
};
struct _Rb_tree_node_base {
    typedef bool _Color_type;
    typedef _Rb_tree_node_base* _Base_ptr;
    _Color_type _M_color; _Base_ptr _M_parent; _Base_ptr _M_left; _Base_ptr _M_right;
};
template <class _Value> struct _Rb_tree_node : public _Rb_tree_node_base { _Value _M_value_field; };
template <class _Dummy> class _Rb_global {
public:
    typedef _Rb_tree_node_base* _Base_ptr;
    static _Base_ptr __cdecl _M_increment(_Base_ptr __x);       // 0x004292C0
    static _Base_ptr __cdecl _M_decrement(_Base_ptr __x);       // 0x004294E2
};
template <class _Dummy> _Rb_tree_node_base* __cdecl _Rb_global<_Dummy>::_M_decrement(_Rb_tree_node_base* _M_node)
{
    if (_M_node->_M_color == false && (_M_node->_M_parent)->_M_parent == _M_node)
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
template <class _Dummy> _Rb_tree_node_base* __cdecl _Rb_global<_Dummy>::_M_increment(_Rb_tree_node_base* _M_node)
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
    return _M_node;
}
typedef _Rb_global<bool> _Rb_global_inst;
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
    _Self& operator++() { _M_node = _Rb_global_inst::_M_increment(_M_node); return *this; }
    _Self& operator--() { _M_node = _Rb_global_inst::_M_decrement(_M_node); return *this; }
};
template <class _Key, class _Value, class _KeyOfValue, class _Compare, class _Alloc>
class _Rb_tree {
public:
    typedef _Rb_tree_node_base* _Base_ptr;
    typedef _Rb_tree_node<_Value> _Node;
    typedef _Node* _Link_type;
    typedef _Key key_type;
    typedef _Value value_type;
    typedef _Rb_tree_iterator<value_type, _Nonconst_traits<value_type> > iterator;
    _STLP_alloc_proxy<_Link_type, allocator<_Node> > _M_header;
    unsigned int _M_node_count;
    _Compare _M_key_compare;
    _Link_type& _M_rightmost() const { return (_Link_type&)_M_header._M_data->_M_right; }
    static _Link_type& _S_right(_Base_ptr __x) { return (_Link_type&)(__x->_M_right); }
    static const _Key& _S_key(_Base_ptr __x) { return ((_Link_type)__x)->_M_value_field.first; }
    unsigned int size() const { return _M_node_count; }
    iterator _M_insert(_Base_ptr __x, _Base_ptr __y, const value_type& __v, _Base_ptr __w = 0);
    pair<iterator, bool> insert_unique(const value_type& __x);
    iterator insert_unique(iterator __position, const value_type& __x);
};
template <class _Key, class _Value, class _KeyOfValue, class _Compare, class _Alloc>
typename _Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>::iterator
_Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>::insert_unique(iterator __position, const value_type& __v)
{
    if (__position._M_node == this->_M_header._M_data->_M_left) {
        if (size() <= 0)
            return insert_unique(__v).first;
        if (_M_key_compare(_KeyOfValue()(__v), _S_key(__position._M_node)))
            return _M_insert(__position._M_node, __position._M_node, __v);
        else {
            bool __comp_pos_v = _M_key_compare(_S_key(__position._M_node), _KeyOfValue()(__v));
            if (__comp_pos_v == false)
                return __position;
            iterator __after = __position;
            ++__after;
            if (__after._M_node == this->_M_header._M_data)
                return _M_insert(0, __position._M_node, __v, __position._M_node);
            if (_M_key_compare(_KeyOfValue()(__v), _S_key(__after._M_node))) {
                if (_S_right(__position._M_node) == 0)
                    return _M_insert(0, __position._M_node, __v, __position._M_node);
                else
                    return _M_insert(__after._M_node, __after._M_node, __v);
            }
            else
                return insert_unique(__v).first;
        }
    }
    else if (__position._M_node == this->_M_header._M_data) {
        if (_M_key_compare(_S_key(_M_rightmost()), _KeyOfValue()(__v)))
            return _M_insert(0, _M_rightmost(), __v, __position._M_node);
        else
            return insert_unique(__v).first;
    }
    else {
        iterator __before = __position;
        --__before;
        bool __comp_v_pos = _M_key_compare(_KeyOfValue()(__v), _S_key(__position._M_node));
        if (__comp_v_pos && _M_key_compare(_S_key(__before._M_node), _KeyOfValue()(__v))) {
            if (_S_right(__before._M_node) == 0)
                return _M_insert(0, __before._M_node, __v, __before._M_node);
            else
                return _M_insert(__position._M_node, __position._M_node, __v);
        }
        else {
            iterator __after = __position;
            ++__after;
            bool __comp_pos_v = !__comp_v_pos;
            if (!__comp_v_pos)
                __comp_pos_v = _M_key_compare(_S_key(__position._M_node), _KeyOfValue()(__v));
            if ((!__comp_v_pos) && __comp_pos_v &&
                (__after._M_node == this->_M_header._M_data || _M_key_compare(_KeyOfValue()(__v), _S_key(__after._M_node)))) {
                if (_S_right(__position._M_node) == 0)
                    return _M_insert(0, __position._M_node, __v, __position._M_node);
                else
                    return _M_insert(__after._M_node, __after._M_node, __v);
            }
            else {
                if (__comp_v_pos == __comp_pos_v)
                    return __position;
                else
                    return insert_unique(__v).first;
            }
        }
    }
}
typedef pair<NUISystem::CComponent* const, std::pair<CWideString, long> > TValue;
template _Rb_tree<NUISystem::CComponent*, TValue, _Select1st<TValue>, less<NUISystem::CComponent* >, allocator<TValue> >::iterator
_Rb_tree<NUISystem::CComponent*, TValue, _Select1st<TValue>, less<NUISystem::CComponent* >, allocator<TValue> >::insert_unique(iterator, const TValue&);
}