// std::_Rb_tree::insert_unique(iterator position, const value_type& v) -- (map<EHeroAbility, NUISystem::CComponent*>) STLport 4.6 hinted unique insert: begin / end / interior hint cases, falling back on insert_unique(v)
// Grade: DIFFER(294v319) (graded once; the family is parked on its representative 0x0042C1C1) -- register allocation: retail keeps `this` in ebx and the key value in edi across the _M_decrement/_M_increment calls (v pointer in edi on the begin() path); VC7.1 here keeps this in edi and &v in ebx, and lays the empty-tree insert_unique(v) tail inline (`ja` over it) where retail jumps to the shared tail (`jbe`)
// Leaf: ?insert_unique@?$_Rb_tree@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005F9853, 319 bytes, size TU (`push ebp`, `leave`), thiscall `ret 0Ch` (hidden iterator result, position, v).
// Identity: its forwarder 0x005F9CA5 is called by the operator[] 0x005F9EA5, used on CTCInventoryAbilities +0x1A0 / +0x1AC = Ego_r UnassignedIcons / AbilitiesMenuEntries (map<EHeroAbility, NUISystem::CComponent*>). The manifest label "RBTree_FindInsertionPoint" is propagated/behavioural.
// Callees: _M_insert(x, y, v, w) 0x005F926B, insert_unique(v) 0x005F92F3, _Rb_global<bool>::_M_increment 0x004292C0 / _M_decrement 0x004294E2 (static __cdecl).
// Tried: the STLport body verbatim (the landed /O2 twin 0x00AD5030's model with _Rb_global stepping), `_S_key` through `_KeyOfValue()(_S_value(x))`, same-TU noinline stand-in bodies for _M_insert/insert_unique(v) (in class under auto_inline(off): nothing inlines; out of class: no change).
#pragma optimize("s", on)
enum EHeroAbility { HERO_ABILITY_0 }; namespace NUISystem { class CComponent; }
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
    pair(const _T1& __a, const _T2& __b) : first(__a), second(__b) { }
    pair(const pair<_T1, _T2>& __o) : first(__o.first), second(__o.second) { }
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

template <class _Dummy> class _Rb_global {
public:
    typedef _Rb_tree_node_base* _Base_ptr;
    static _Base_ptr __cdecl _M_increment(_Base_ptr __x);                  // 0x004292C0 (STLport _tree.c, shared)
    static _Base_ptr __cdecl _M_decrement(_Base_ptr __x);                  // 0x004294E2 (STLport _tree.c, shared)
};
typedef _Rb_global<bool> _Rb_global_inst;

struct _Rb_tree_base_iterator {
    typedef _Rb_tree_node_base* _Base_ptr;
    _Base_ptr _M_node;
    void _M_increment() { _M_node = _Rb_global_inst::_M_increment(_M_node); }
    void _M_decrement() { _M_node = _Rb_global_inst::_M_decrement(_M_node); }
};

template <class _Tp> struct _Nonconst_traits { typedef _Tp value_type; typedef _Tp& reference; typedef _Tp* pointer; };
template <class _Tp> struct _Const_traits { typedef _Tp value_type; typedef const _Tp& reference; typedef const _Tp* pointer; };

template <class _Value, class _Traits> struct _Rb_tree_iterator : public _Rb_tree_base_iterator {
    typedef _Rb_tree_iterator<_Value, _Traits> _Self;
    typedef _Rb_tree_node<_Value>* _Link_type;
    _Rb_tree_iterator() { }
    _Rb_tree_iterator(_Link_type __x) { _M_node = __x; }
    _Rb_tree_iterator(const _Self& __it) { _M_node = __it._M_node; }
    _Self& operator++() { _M_increment(); return *this; }
    _Self& operator--() { _M_decrement(); return *this; }
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

    _STLP_alloc_proxy<_Link_type, allocator<_Node> > _M_header;   // +0x00
    unsigned int _M_node_count;                                    // +0x04
    _Compare     _M_key_compare;                                   // +0x08

    _Link_type& _M_rightmost() const { return (_Link_type&)_M_header._M_data->_M_right; }
    static _Link_type& _S_right(_Base_ptr __x) { return (_Link_type&)(__x->_M_right); }
    static const _Key& _S_key(_Base_ptr __x) { return ((_Link_type)__x)->_M_value_field.first; }
    unsigned int size() const { return _M_node_count; }

    iterator _M_insert(_Base_ptr __x, _Base_ptr __y, const value_type& __v, _Base_ptr __w = 0);   // 0x005F926B
    pair<iterator, bool> insert_unique(const value_type& __x);                                    // 0x005F92F3
    iterator insert_unique(iterator __position, const value_type& __x);
};

template <class _Key, class _Value, class _KeyOfValue, class _Compare, class _Alloc>
typename _Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>::iterator
_Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>::insert_unique(iterator __position, const value_type& __v)
{
    if (__position._M_node == _M_header._M_data->_M_left) {   // begin()
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
            if (__after._M_node == _M_header._M_data)
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
    else if (__position._M_node == _M_header._M_data) {   // end()
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
                (__after._M_node == _M_header._M_data || _M_key_compare(_KeyOfValue()(__v), _S_key(__after._M_node)))) {
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
}   // namespace std

typedef std::pair<const EHeroAbility, NUISystem::CComponent*> TValue;
typedef std::_Rb_tree<EHeroAbility, TValue, std::_Select1st<TValue>, std::less<EHeroAbility>, std::allocator<TValue> > TTree;
template TTree::iterator TTree::insert_unique(TTree::iterator, const TValue&);
