// std::_Rb_tree::insert_unique(iterator position, const pair<const char, CCharString>& v) -- STLport 4.6 hinted unique insert for the console key-binding map (key character -> command string): begin / end / interior hint cases, falling back on the unhinted insert_unique when the hint does not bracket v
// Leaf: ?insert_unique@?$_Rb_tree@DU?$pair@$$CBDVCCharString@@@std@@U?$_Select1st@U?$pair@$$CBDVCCharString@@@std@@@2@U?$less@D@2@V?$allocator@U?$pair@$$CBDVCCharString@@@std@@@2@@std@@QAE?AU?$_Rb_tree_iterator@U?$pair@$$CBDVCCharString@@@std@@U?$_Nonconst_traits@U?$pair@$$CBDVCCharString@@@std@@@2@@2@U32@ABU?$pair@$$CBDVCCharString@@@2@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x009EF4D0, 520 bytes, thiscall `ret 0Ch` (hidden iterator result, position, v).
// Identity (proven): Ego_r.pdb CConsole::KeyBindingMap is map<char, CCharString> (the only map<char, ..> member); this
//   body's callees are that tree's landed _M_insert 0x009EF0D0 and insert_unique(v) 0x009EF1A0, and its callers are the
//   landed map<char, CCharString>::operator[] 0x009EFCF0 and the map::insert(hint, v) path 0x009EFA04. Keys compare as
//   signed chars (`cmp al,bl ; jle/jl`); the node key sits at +0x10. The catalog's "CCharMap_InsertIfNew" is descriptive.
// Callees (this instantiation): _Rb_tree_iterator::operator++ 0x009EE1F0 / operator-- 0x009EE910 (out of line, thiscall
//   on the iterator; both called only from this tree's insert paths), _M_insert 0x009EF0D0 (private), insert_unique(v)
//   0x009EF1A0.
// Model: STLport 4.6 _tree.c text, as the landed u64 hinted insert 0x0082F930 (and 0x0052A2D0 of this chunk).
extern "C" void* __cdecl malloc(unsigned int size);    // 0x00BFEA0E
extern "C" void __cdecl free(void* block);              // 0x00BFEA14
inline void* operator new(unsigned int, void* p) { return p; }

class CCharString { public: void* PStringData; };

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
    template <class _U1, class _U2> pair(const pair<_U1, _U2>& __p) : first(__p.first), second(__p.second) { }
    pair(const pair<_T1, _T2>& __o) : first(__o.first), second(__o.second) { }
};

template <class _T1, class _T2>
inline bool __fastcall operator<(const pair<_T1, _T2>& __x, const pair<_T1, _T2>& __y)
{
    return __x.first < __y.first || (!(__y.first < __x.first) && __x.second < __y.second);
}

template <class _Tp> struct less {
    bool operator()(const _Tp& __x, const _Tp& __y) const { return __x < __y; }
};
template <class _Tp> struct _Identity {
    const _Tp& operator()(const _Tp& __x) const { return __x; }
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
    static void __cdecl _Rebalance(_Base_ptr __x, _Base_ptr& __root);                         // 0x0042971D
    static _Base_ptr __cdecl _Rebalance_for_erase(_Base_ptr __z, _Base_ptr& __root,
                                                  _Base_ptr& __leftmost, _Base_ptr& __rightmost);   // 0x0042A34E
};
typedef _Rb_global<bool> _Rb_global_inst;

struct _Rb_tree_base_iterator {
    typedef _Rb_tree_node_base* _Base_ptr;
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

template <class _Tp> struct _Nonconst_traits { typedef _Tp value_type; typedef _Tp& reference; typedef _Tp* pointer; };
template <class _Tp> struct _Const_traits { typedef _Tp value_type; typedef const _Tp& reference; typedef const _Tp* pointer; };

template <class _Value, class _Traits> struct _Rb_tree_iterator : public _Rb_tree_base_iterator {
    typedef _Rb_tree_iterator<_Value, _Traits> _Self;
    typedef _Rb_tree_node<_Value>* _Link_type;
    _Rb_tree_iterator() { }
    _Rb_tree_iterator(_Link_type __x) { _M_node = __x; }
    _Rb_tree_iterator(const _Self& __it) { _M_node = __it._M_node; }
    typename _Traits::reference operator*() const { return _Link_type(_M_node)->_M_value_field; }
    __declspec(noinline) _Self& operator++() { _M_increment(); return *this; }   // out of line in retail (per instantiation)
    __declspec(noinline) _Self& operator--()           // out of line in retail (per instantiation)
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
        return *this;
    }
    bool operator==(const _Self& __y) const { return _M_node == __y._M_node; }
    bool operator!=(const _Self& __y) const { return _M_node != __y._M_node; }
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
    typedef _Rb_tree_iterator<value_type, _Const_traits<value_type> > const_iterator;

    _STLP_alloc_proxy<_Link_type, allocator<_Node> > _M_header;   // +0x00
    unsigned int _M_node_count;                                    // +0x04
    _Compare     _M_key_compare;                                   // +0x08

    _Link_type _M_allocate_node() { return (_Link_type)malloc(sizeof(_Node)); }
    void _M_deallocate_node(_Link_type __p) { if (__p != 0) free(__p); }
protected:
    _Link_type _M_create_node(const value_type& __x)
    {
        _Link_type __tmp = _M_allocate_node();
        new (&__tmp->_M_value_field) value_type(__x);
        return __tmp;
    }
protected:
    _Link_type _M_clone_node(_Link_type __x)
    {
        _Link_type __tmp = _M_create_node(__x->_M_value_field);
        __tmp->_M_color = __x->_M_color;
        __tmp->_M_left = 0;
        __tmp->_M_right = 0;
        return __tmp;
    }
public:
    _Link_type& _M_root() const { return (_Link_type&)_M_header._M_data->_M_parent; }
    _Link_type& _M_leftmost() const { return (_Link_type&)_M_header._M_data->_M_left; }
    _Link_type& _M_rightmost() const { return (_Link_type&)_M_header._M_data->_M_right; }
    static _Link_type& _S_left(_Base_ptr __x) { return (_Link_type&)(__x->_M_left); }
    static _Link_type& _S_right(_Base_ptr __x) { return (_Link_type&)(__x->_M_right); }
    // map key access spelled directly: STLport's `_KeyOfValue()(_S_value(x))` temporary makes VC7.1 load the node key
    // before v.first (2 bytes longer); the map's _Select1st reads the same `.first`.
    static const _Key& _S_key(_Base_ptr __x) { return ((_Link_type)__x)->_M_value_field.first; }

    iterator begin() { return iterator(_M_leftmost()); }
    iterator end() { return iterator(_M_header._M_data); }

private:
    iterator _M_insert(_Base_ptr __x, _Base_ptr __y, const value_type& __v, _Base_ptr __w = 0);
private:
    _Link_type _M_copy(_Link_type __x, _Link_type __p);
public:
    pair<iterator, bool> insert_unique(const value_type& __x);
    iterator insert_unique(iterator __position, const value_type& __x);
    unsigned int size() const { return _M_node_count; }
    _Base_ptr _M_lower_bound(const key_type& __k) const;
    void erase(iterator __position)
    {
        _Link_type __y = (_Link_type)_Rb_global_inst::_Rebalance_for_erase(__position._M_node,
            _M_header._M_data->_M_parent, _M_header._M_data->_M_left, _M_header._M_data->_M_right);
        __y->_M_value_field.~value_type();
        _M_deallocate_node(__y);
        --_M_node_count;
    }
};

}   // namespace std

namespace std {
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



typedef std::pair<const char, CCharString> TKeyBindingValue;
typedef std::_Rb_tree<char, TKeyBindingValue, std::_Select1st<TKeyBindingValue>, std::less<char>,
                      std::allocator<TKeyBindingValue> > TKeyBindingTree;
template TKeyBindingTree::iterator TKeyBindingTree::insert_unique(TKeyBindingTree::iterator, const TKeyBindingValue&);