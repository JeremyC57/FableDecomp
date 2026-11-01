// QuadricOptimiserInternals::COptimisedVertex::RemapGroups(const std::map<unsigned long, unsigned long>& remap) -- if any of the vertex's smoothing-group ids is in the remap table, rebuild Groups with the ids mapped and the weights of merged ids summed
// Leaf: ?RemapGroups@COptimisedVertex@QuadricOptimiserInternals@@QAEXABV?$map@KKU?$less@K@std@@V?$allocator@U?$pair@$$CBKK@std@@@2@@std@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AC97D0, 535 bytes, thiscall, `ret 4`, frame `and esp,-8` (the local map's 8-byte
//   alignment of its float values).
// Identity (proven): Ego_r.pdb ?RemapGroups@COptimisedVertex@QuadricOptimiserInternals@@QAEXABV?$map@KK...@@@Z (the only
//   RemapGroups taking the map<unsigned long,unsigned long>, next to COptimisedPrimitive::RemapGroups); the one caller is
//   0x00AC9A26 inside the landed COptimisedPrimitive::RemapGroups 0x00AC99F0 (`Vertices[i].RemapGroups(remap)`); `this+0x9C`
//   is Groups, map<unsigned long,float> (Ego_r +0xA0 with a 16-byte Dinkumware CIndexList::vector above it, 12 in retail);
//   the callees are the landed map<unsigned long,float> members insert_unique 0x00AD3EF0, operator[] 0x00AD6420,
//   _Rb_tree::operator= 0x00AD3E30 and the recursive _M_erase 0x00AD14E0 (the local map's clear() + header free
//   0x00BFEA14, header malloc 0x00BFEA0E).
// Shape: the first loop looks for any group id present in `remap` (find inline: lower_bound descent + end()/key test)
//   and returns when none is; the second builds a local map<unsigned long,float>, inserting (id, 0.0f) and adding the
//   weight through operator[]; then `Groups = newGroups` and the local's destructor (clear() with _M_erase called on the
//   right subtrees, the left chain freed inline, header re-linked, then the header node freed).
#include <stdlib.h>

inline void* operator new(unsigned int, void* __p) { return __p; }

namespace std {
template <class _Tp> class allocator { public: allocator() { } };
template <class _Tp, class _Alloc> class _STLP_alloc_proxy : public _Alloc {
public:
    _Tp _M_data;
    _STLP_alloc_proxy(const _Alloc& __a, _Tp __p) : _Alloc(__a), _M_data(__p) { }
};
template <class _T1, class _T2> struct pair {
    typedef _T1 first_type;
    typedef _T2 second_type;
    _T1 first;
    _T2 second;
    pair(const _T1& __a, const _T2& __b) : first(__a), second(__b) { }
};
template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }
template <class _Tp> struct less {
    less() { }
    bool operator()(const _Tp& __x, const _Tp& __y) const { return __x < __y; }
};
template <class _Pair> struct _Select1st {
    const typename _Pair::first_type& operator()(const _Pair& __x) const { return __x.first; }
};
typedef bool _Rb_tree_Color_type;
const _Rb_tree_Color_type _S_rb_tree_red = false;
const _Rb_tree_Color_type _S_rb_tree_black = true;
struct _Rb_tree_node_base {
    typedef _Rb_tree_Color_type _Color_type;
    typedef _Rb_tree_node_base* _Base_ptr;
    _Color_type _M_color;                                   // +0x00
    _Base_ptr   _M_parent;                                  // +0x04
    _Base_ptr   _M_left;                                    // +0x08
    _Base_ptr   _M_right;                                   // +0x0C
    static _Base_ptr _S_minimum(_Base_ptr __x) { while (__x->_M_left != 0) __x = __x->_M_left; return __x; }
    static _Base_ptr _S_maximum(_Base_ptr __x) { while (__x->_M_right != 0) __x = __x->_M_right; return __x; }
};
template <class _Value> struct _Rb_tree_node : public _Rb_tree_node_base {
    _Value _M_value_field;                                  // +0x10
};
// _Rb_global: the STLport 4.6 _tree.c node-level helpers (one instantiation per TU)
template <class _Dummy> class _Rb_global {
public:
    typedef _Rb_tree_node_base* _Base_ptr;
    static void _Rotate_left(_Base_ptr __x, _Base_ptr& __root);
    static void _Rotate_right(_Base_ptr __x, _Base_ptr& __root);
    static void _Rebalance(_Base_ptr __x, _Base_ptr& __root);
    static _Base_ptr _M_increment(_Base_ptr __x);
    static _Base_ptr _M_decrement(_Base_ptr __x);
};
typedef _Rb_global<bool> _Rb_global_inst;
template <class _Dummy>
_Rb_tree_node_base* _Rb_global<_Dummy>::_M_increment(_Rb_tree_node_base* _M_node)
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
template <class _Dummy>
_Rb_tree_node_base* _Rb_global<_Dummy>::_M_decrement(_Rb_tree_node_base* _M_node)
{
    if (_M_node->_M_color == _S_rb_tree_red && _M_node->_M_parent->_M_parent == _M_node)
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
template <class _Dummy>
void _Rb_global<_Dummy>::_Rotate_left(_Rb_tree_node_base* __x, _Rb_tree_node_base*& __root)
{
    _Rb_tree_node_base* __y = __x->_M_right;
    __x->_M_right = __y->_M_left;
    if (__y->_M_left != 0)
        __y->_M_left->_M_parent = __x;
    __y->_M_parent = __x->_M_parent;
    if (__x == __root)
        __root = __y;
    else if (__x == __x->_M_parent->_M_left)
        __x->_M_parent->_M_left = __y;
    else
        __x->_M_parent->_M_right = __y;
    __y->_M_left = __x;
    __x->_M_parent = __y;
}
template <class _Dummy>
void _Rb_global<_Dummy>::_Rotate_right(_Rb_tree_node_base* __x, _Rb_tree_node_base*& __root)
{
    _Rb_tree_node_base* __y = __x->_M_left;
    __x->_M_left = __y->_M_right;
    if (__y->_M_right != 0)
        __y->_M_right->_M_parent = __x;
    __y->_M_parent = __x->_M_parent;
    if (__x == __root)
        __root = __y;
    else if (__x == __x->_M_parent->_M_right)
        __x->_M_parent->_M_right = __y;
    else
        __x->_M_parent->_M_left = __y;
    __y->_M_right = __x;
    __x->_M_parent = __y;
}
template <class _Dummy>
void _Rb_global<_Dummy>::_Rebalance(_Rb_tree_node_base* __x, _Rb_tree_node_base*& __root)
{
    __x->_M_color = _S_rb_tree_red;
    while (__x != __root && __x->_M_parent->_M_color == _S_rb_tree_red) {
        if (__x->_M_parent == __x->_M_parent->_M_parent->_M_left) {
            _Rb_tree_node_base* __y = __x->_M_parent->_M_parent->_M_right;
            if (__y && __y->_M_color == _S_rb_tree_red) {
                __x->_M_parent->_M_color = _S_rb_tree_black;
                __y->_M_color = _S_rb_tree_black;
                __x->_M_parent->_M_parent->_M_color = _S_rb_tree_red;
                __x = __x->_M_parent->_M_parent;
            }
            else {
                if (__x == __x->_M_parent->_M_right) {
                    __x = __x->_M_parent;
                    _Rotate_left(__x, __root);
                }
                __x->_M_parent->_M_color = _S_rb_tree_black;
                __x->_M_parent->_M_parent->_M_color = _S_rb_tree_red;
                _Rotate_right(__x->_M_parent->_M_parent, __root);
            }
        }
        else {
            _Rb_tree_node_base* __y = __x->_M_parent->_M_parent->_M_left;
            if (__y && __y->_M_color == _S_rb_tree_red) {
                __x->_M_parent->_M_color = _S_rb_tree_black;
                __y->_M_color = _S_rb_tree_black;
                __x->_M_parent->_M_parent->_M_color = _S_rb_tree_red;
                __x = __x->_M_parent->_M_parent;
            }
            else {
                if (__x == __x->_M_parent->_M_left) {
                    __x = __x->_M_parent;
                    _Rotate_right(__x, __root);
                }
                __x->_M_parent->_M_color = _S_rb_tree_black;
                __x->_M_parent->_M_parent->_M_color = _S_rb_tree_red;
                _Rotate_left(__x->_M_parent->_M_parent, __root);
            }
        }
    }
    __root->_M_color = _S_rb_tree_black;
}
struct _Rb_tree_base_iterator {
    typedef _Rb_tree_node_base::_Base_ptr _Base_ptr;
    _Base_ptr _M_node;
};
inline bool operator==(const _Rb_tree_base_iterator& __x, const _Rb_tree_base_iterator& __y) { return __x._M_node == __y._M_node; }
inline bool operator!=(const _Rb_tree_base_iterator& __x, const _Rb_tree_base_iterator& __y) { return __x._M_node != __y._M_node; }
template <class _Tp> struct _Nonconst_traits { typedef _Tp& reference; typedef _Tp* pointer; };
template <class _Tp> struct _Const_traits { typedef const _Tp& reference; typedef const _Tp* pointer; };
template <class _Value, class _Traits> struct _Rb_tree_iterator : public _Rb_tree_base_iterator {
    typedef typename _Traits::reference reference;
    typedef typename _Traits::pointer pointer;
    typedef _Rb_tree_iterator<_Value, _Traits> _Self;
    typedef _Rb_tree_iterator<_Value, _Nonconst_traits<_Value> > iterator;
    _Rb_tree_iterator() { }
    _Rb_tree_iterator(_Rb_tree_node<_Value>* __x) { _M_node = __x; }
    _Rb_tree_iterator(const iterator& __it) { _M_node = __it._M_node; }
    reference operator*() const { return ((_Rb_tree_node<_Value>*)_M_node)->_M_value_field; }
    pointer operator->() const { return &(operator*()); }
    _Self& operator++() { _M_node = _Rb_global_inst::_M_increment(_M_node); return *this; }
    _Self& operator--() { _M_node = _Rb_global_inst::_M_decrement(_M_node); return *this; }
};
template <class _Tp, class _Alloc> struct _Rb_tree_base {
    typedef _Rb_tree_node<_Tp> _Node;
    _Rb_tree_base(const _Alloc& __a) : _M_header(__a, (_Node*)0) { _M_header._M_data = (_Node*)malloc(sizeof(_Node)); }
    ~_Rb_tree_base() { if (_M_header._M_data) free(_M_header._M_data); }
protected:
    _STLP_alloc_proxy<_Node*, _Alloc> _M_header;                   // +0x00
};
template <class _Key, class _Value, class _KeyOfValue, class _Compare, class _Alloc>
class _Rb_tree : public _Rb_tree_base<_Value, _Alloc> {
public:
    typedef _Rb_tree_node_base* _Base_ptr;
    typedef _Rb_tree_node<_Value> _Node;
    typedef _Node* _Link_type;
    typedef _Key key_type;
    typedef _Value value_type;
    typedef unsigned int size_type;
    typedef _Rb_tree_iterator<value_type, _Nonconst_traits<value_type> > iterator;
    typedef _Rb_tree_iterator<value_type, _Const_traits<value_type> > const_iterator;
protected:
    size_type _M_node_count;                                       // +0x04
    _Compare  _M_key_compare;                                      // +0x08
    _Link_type _M_create_node(const value_type& __x)
    {
        _Link_type __tmp = (_Link_type)malloc(sizeof(_Node));
        _Construct(&__tmp->_M_value_field, __x);
        return __tmp;
    }
    _Link_type _M_clone_node(_Link_type __x)
    {
        _Link_type __tmp = _M_create_node(__x->_M_value_field);
        __tmp->_M_color = __x->_M_color;
        __tmp->_M_left = 0;
        __tmp->_M_right = 0;
        return __tmp;
    }
    void _M_put_node(_Link_type __p) { free(__p); }
    _Link_type& _M_root() const { return (_Link_type&)this->_M_header._M_data->_M_parent; }
    _Link_type& _M_leftmost() const { return (_Link_type&)this->_M_header._M_data->_M_left; }
    _Link_type& _M_rightmost() const { return (_Link_type&)this->_M_header._M_data->_M_right; }
    static _Link_type& _S_left(_Base_ptr __x) { return (_Link_type&)(__x->_M_left); }
    static _Link_type& _S_right(_Base_ptr __x) { return (_Link_type&)(__x->_M_right); }
    static _Link_type& _S_parent(_Base_ptr __x) { return (_Link_type&)(__x->_M_parent); }
    static const _Key& _S_key(_Base_ptr __x) { return _KeyOfValue()(((_Link_type)__x)->_M_value_field); }
    static _Link_type _S_minimum(_Base_ptr __x) { return (_Link_type)_Rb_tree_node_base::_S_minimum(__x); }
    static _Link_type _S_maximum(_Base_ptr __x) { return (_Link_type)_Rb_tree_node_base::_S_maximum(__x); }
    void _M_empty_initialize()
    {
        this->_M_header._M_data->_M_color = _S_rb_tree_red;
        _M_root() = 0;
        _M_leftmost() = this->_M_header._M_data;
        _M_rightmost() = this->_M_header._M_data;
    }
private:
    void _M_erase(_Link_type __x);                                                   // 0x00AD14E0 (private in STLport 4.6 _tree.h: AAE, as the 347 landed _M_erase rows; naming wave 20)
    iterator _M_insert(_Base_ptr __x, _Base_ptr __y, const value_type& __v, _Base_ptr __w = 0);   // 0x00AD3440
    _Link_type _M_copy(_Link_type __x, _Link_type __p);
public:
    _Rb_tree() : _Rb_tree_base<_Value, _Alloc>(_Alloc()), _M_node_count(0), _M_key_compare(_Compare())
    { _M_empty_initialize(); }
    ~_Rb_tree() { clear(); }
    _Rb_tree& operator=(const _Rb_tree& __x);                                        // 0x00AD3E30
    _Compare key_comp() const { return _M_key_compare; }
    iterator begin() { return iterator(_M_leftmost()); }
    iterator end() { return iterator(this->_M_header._M_data); }
    const_iterator begin() const { return const_iterator(_M_leftmost()); }
    const_iterator end() const { return const_iterator(this->_M_header._M_data); }
    void clear()
    {
        if (_M_node_count != 0) {
            _M_erase(_M_root());
            _M_leftmost() = this->_M_header._M_data;
            _M_root() = 0;
            _M_rightmost() = this->_M_header._M_data;
            _M_node_count = 0;
        }
    }
    pair<iterator, bool> insert_unique(const value_type& __x);                       // 0x00AD3EF0
    iterator insert_unique(iterator __position, const value_type& __x);              // 0x00AD5030
    const_iterator find(const _Key& __k) const
    {
        _Base_ptr __y = this->_M_header._M_data;
        _Base_ptr __x = _M_root();
        while (__x != 0)
            if (!_M_key_compare(_S_key(__x), __k))
                __y = __x, __x = _S_left(__x);
            else
                __x = _S_right(__x);
        if (__y == this->_M_header._M_data || _M_key_compare(__k, _S_key(__y)))
            __y = this->_M_header._M_data;
        return const_iterator((_Link_type)__y);
    }
    _Base_ptr _M_lower_bound(const _Key& __k) const
    {
        _Base_ptr __y = this->_M_header._M_data;
        _Base_ptr __x = _M_root();
        while (__x != 0)
            if (!_M_key_compare(_S_key(__x), __k))
                __y = __x, __x = _S_left(__x);
            else
                __x = _S_right(__x);
        return __y;
    }
    iterator lower_bound(const _Key& __k) { return iterator((_Link_type)_M_lower_bound(__k)); }
};
// ---- STLport 4.6 _tree.c member bodies, in the TU as the templates are (out of line in retail) ----
template <class _Key, class _Value, class _KeyOfValue, class _Compare, class _Alloc>
__declspec(noinline) typename _Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>::iterator
_Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>::_M_insert(_Base_ptr __x_, _Base_ptr __y_, const _Value& __v, _Base_ptr __w_)
{
    _Link_type __w = (_Link_type)__w_;
    _Link_type __x = (_Link_type)__x_;
    _Link_type __y = (_Link_type)__y_;
    _Link_type __z;
    if (__y == this->_M_header._M_data ||
        (__w == 0 &&                       // If w != 0, the remainder fails to false
         (__x != 0 ||                      // If x != 0, the remainder succeeds to true
          _M_key_compare(_KeyOfValue()(__v), _S_key(__y))))) {
        __z = _M_create_node(__v);
        _S_left(__y) = __z;                // also makes _M_leftmost() = __z when __y == _M_header
        if (__y == this->_M_header._M_data) {
            _M_root() = __z;
            _M_rightmost() = __z;
        }
        else if (__y == _M_leftmost())
            _M_leftmost() = __z;           // maintain _M_leftmost() pointing to min node
    }
    else {
        __z = _M_create_node(__v);
        _S_right(__y) = __z;
        if (__y == _M_rightmost())
            _M_rightmost() = __z;          // maintain _M_rightmost() pointing to max node
    }
    _S_parent(__z) = __y;
    _S_left(__z) = 0;
    _S_right(__z) = 0;
    _Rb_global_inst::_Rebalance(__z, this->_M_header._M_data->_M_parent);
    ++_M_node_count;
    return iterator(__z);
}
template <class _Key, class _Value, class _KeyOfValue, class _Compare, class _Alloc>
__declspec(noinline) pair<typename _Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>::iterator, bool>
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
template <class _Key, class _Value, class _KeyOfValue, class _Compare, class _Alloc>
__declspec(noinline) typename _Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>::iterator
_Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>::insert_unique(iterator __position, const _Value& __v)
{
    if (__position._M_node == this->_M_header._M_data->_M_left) {   // begin()
        if (_M_node_count > 0 && _M_key_compare(_KeyOfValue()(__v), _S_key(__position._M_node)))
            return _M_insert(__position._M_node, __position._M_node, __v, __position._M_node);
        else
            return insert_unique(__v).first;
    }
    else if (__position._M_node == this->_M_header._M_data) {       // end()
        if (_M_key_compare(_S_key(_M_rightmost()), _KeyOfValue()(__v)))
            return _M_insert(0, _M_rightmost(), __v, 0);
        else
            return insert_unique(__v).first;
    }
    else {
        iterator __before = __position;
        --__before;
        if (_M_key_compare(_S_key(__before._M_node), _KeyOfValue()(__v)) &&
            _M_key_compare(_KeyOfValue()(__v), _S_key(__position._M_node))) {
            if (_S_right(__before._M_node) == 0)
                return _M_insert(0, __before._M_node, __v, __before._M_node);
            else
                return _M_insert(__position._M_node, __position._M_node, __v, __position._M_node);
        }
        else
            return insert_unique(__v).first;
    }
}
template <class _Key, class _Value, class _KeyOfValue, class _Compare, class _Alloc>
__declspec(noinline) typename _Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>::_Link_type
_Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>::_M_copy(_Link_type __x, _Link_type __p)
{                               // structural copy.  __x and __p must be non-null.
    _Link_type __top = _M_clone_node(__x);
    _S_parent(__top) = __p;
    if (_S_right(__x))
        _S_right(__top) = _M_copy(_S_right(__x), __top);
    __p = __top;
    __x = _S_left(__x);
    while (__x != 0) {
        _Link_type __y = _M_clone_node(__x);
        _S_left(__p) = __y;
        _S_parent(__y) = __p;
        if (_S_right(__x))
            _S_right(__y) = _M_copy(_S_right(__x), __y);
        __p = __y;
        __x = _S_left(__x);
    }
    return __top;
}
template <class _Key, class _Value, class _KeyOfValue, class _Compare, class _Alloc>
__declspec(noinline) _Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>&
_Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>::operator=(const _Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>& __x)
{
    if (this != &__x) {
        // Note that _Key may be a constant type.
        clear();
        _M_node_count = 0;
        _M_key_compare = __x._M_key_compare;
        if (__x._M_root() == 0) {
            _M_root() = 0;
            _M_leftmost() = this->_M_header._M_data;
            _M_rightmost() = this->_M_header._M_data;
        }
        else {
            _M_root() = _M_copy(__x._M_root(), this->_M_header._M_data);
            _M_leftmost() = _S_minimum(_M_root());
            _M_rightmost() = _S_maximum(_M_root());
            _M_node_count = __x._M_node_count;
        }
    }
    return *this;
}
template <class _Key, class _Value, class _KeyOfValue, class _Compare, class _Alloc>
void _Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>::_M_erase(_Link_type __x)
{                               // erase without rebalancing
    while (__x != 0) {
        _M_erase(_S_right(__x));
        _Link_type __y = _S_left(__x);
        _M_put_node(__x);
        __x = __y;
    }
}
template <class _Key, class _Tp, class _Compare = less<_Key>, class _Alloc = allocator<pair<const _Key, _Tp> > >
class map {
public:
    typedef _Key key_type;
    typedef pair<const _Key, _Tp> value_type;
private:
    typedef _Rb_tree<key_type, value_type, _Select1st<value_type>, _Compare, _Alloc> _Rep_type;
    _Rep_type _M_t;
public:
    typedef typename _Rep_type::iterator iterator;
    typedef typename _Rep_type::const_iterator const_iterator;
    map() : _M_t() { }
    map& operator=(const map& __x) { _M_t = __x._M_t; return *this; }
    _Compare key_comp() const { return _M_t.key_comp(); }
    iterator begin() { return _M_t.begin(); }
    iterator end() { return _M_t.end(); }
    const_iterator begin() const { return _M_t.begin(); }
    const_iterator end() const { return _M_t.end(); }
    const_iterator find(const key_type& __x) const { return _M_t.find(__x); }
    iterator lower_bound(const key_type& __x) { return _M_t.lower_bound(__x); }
    pair<iterator, bool> insert(const value_type& __x) { return _M_t.insert_unique(__x); }
    iterator insert(iterator __pos, const value_type& __x) { return _M_t.insert_unique(__pos, __x); }
    __declspec(noinline) _Tp& operator[](const key_type& __k)                      // 0x00AD6420
    {
        iterator __i = lower_bound(__k);
        // __i->first is greater than or equivalent to __k.
        if (__i == end() || key_comp()(__k, (*__i).first))
            __i = insert(__i, value_type(__k, _Tp()));
        return (*__i).second;
    }
};
}

namespace QuadricOptimiserInternals {
class COptimisedVertex {                           // Ego_r.pdb (retail 0xB4 bytes: CIndexList's vector is 12 bytes)
public:
    unsigned char _front[0x9C];                    // Position, Normal, TextureCoord, PositionError, Cost, FaceList, Exists, UniqueIndex, OutputIndex
    std::map<unsigned long, float> Groups;         // +0x9C
    unsigned char _back[0x10];                     // PairContractions
    void RemapGroups(const std::map<unsigned long, unsigned long>& remap);
};

void COptimisedVertex::RemapGroups(const std::map<unsigned long, unsigned long>& remap)
{
    bool anyRemapped = false;
    std::map<unsigned long, float>::iterator it;
    for (it = Groups.begin(); it != Groups.end(); ++it) {
        if (remap.find(it->first) != remap.end()) {
            anyRemapped = true;
            break;
        }
    }
    if (!anyRemapped)
        return;

    std::map<unsigned long, float> newGroups;
    for (it = Groups.begin(); it != Groups.end(); ++it) {
        unsigned long group = it->first;
        float weight = it->second;
        std::map<unsigned long, unsigned long>::const_iterator remapped = remap.find(group);
        if (remapped != remap.end())
            group = remapped->second;
        newGroups.insert(std::map<unsigned long, float>::value_type(group, 0.0f));
        newGroups[group] += weight;
    }
    Groups = newGroups;
}
}