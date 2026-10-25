// Behaviour test for 00ad3ef0: std::_Rb_tree<map<unsigned long, float>>::insert_unique (same STLport 4.6 template code, keys of the same
// signedness, a test-side _M_insert that links without rebalancing)
#include <cstdio>
#include <new>
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
template <class _Tp> struct _Identity {
    const _Tp& operator()(const _Tp& __x) const { return __x; }
};
struct _Rb_tree_node_base {
    typedef _Rb_tree_node_base* _Base_ptr;
    bool      _M_color;                                     // +0x00
    _Base_ptr _M_parent;                                    // +0x04
    _Base_ptr _M_left;                                      // +0x08
    _Base_ptr _M_right;                                     // +0x0C
};
template <class _Value> struct _Rb_tree_node : public _Rb_tree_node_base {
    _Value _M_value_field;                                  // +0x10
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
// The out-of-line decrement (its own retail row, 0x00AD1850) is defined in this TU as in retail; noinline keeps it a call
// and lets VC7.1 home the iterator in the dead argument slot.
#pragma auto_inline(off)
template <class _Value, class _Traits> __declspec(noinline)
_Rb_tree_iterator<_Value, _Traits>& _Rb_tree_iterator<_Value, _Traits>::operator--()
{
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
    _Rb_tree() { _M_header._M_data = (_Link_type)::operator new(sizeof(_Node)); _M_header._M_data->_M_color = false;
        _M_header._M_data->_M_parent = 0; _M_header._M_data->_M_left = _M_header._M_data->_M_right = _M_header._M_data; _M_node_count = 0; }
    unsigned int count() const { return _M_node_count; }
protected:
    _STLP_alloc_proxy<_Link_type, allocator<_Node> > _M_header;   // +0x00
    unsigned int _M_node_count;                                    // +0x04
    _Compare     _M_key_compare;                                   // +0x08
    _Link_type& _M_root() const { return (_Link_type&)_M_header._M_data->_M_parent; }
    _Link_type& _M_leftmost() const { return (_Link_type&)_M_header._M_data->_M_left; }
    _Link_type& _M_rightmost() const { return (_Link_type&)_M_header._M_data->_M_right; }
    static _Link_type& _S_left(_Base_ptr __x) { return (_Link_type&)(__x->_M_left); }
    static _Link_type& _S_right(_Base_ptr __x) { return (_Link_type&)(__x->_M_right); }
    static _Link_type& _S_parent(_Base_ptr __x) { return (_Link_type&)(__x->_M_parent); }
    static const _Key& _S_key(_Base_ptr __x) { return _KeyOfValue()(((_Link_type)__x)->_M_value_field); }
private:
    iterator _M_insert(_Base_ptr __x, _Base_ptr __y, const value_type& __v, _Base_ptr __w = 0);   // 0x00AD3440
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
template <class _Key, class _Value, class _KeyOfValue, class _Compare, class _Alloc>
typename _Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>::iterator
_Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>::_M_insert(_Base_ptr __x, _Base_ptr __y, const _Value& __v, _Base_ptr)
{
    _Link_type __z = (_Link_type)::operator new(sizeof(_Node));
    new (&__z->_M_value_field) _Value(__v);
    if (__y == _M_header._M_data || __x != 0 || _M_key_compare(_KeyOfValue()(__v), _S_key(__y))) {
        _S_left(__y) = __z;
        if (__y == _M_header._M_data) { _M_root() = __z; _M_rightmost() = __z; }
        else if (__y == _M_leftmost()) _M_leftmost() = __z;
    } else {
        _S_right(__y) = __z;
        if (__y == _M_rightmost()) _M_rightmost() = __z;
    }
    _S_parent(__z) = (_Link_type)__y; _S_left(__z) = 0; _S_right(__z) = 0; __z->_M_color = true;
    ++_M_node_count;
    return iterator(__z);
}
typedef unsigned long _TreeKey;
typedef pair<unsigned long const, int > _TreeValue;
typedef _Rb_tree<_TreeKey, _TreeValue, _Select1st<_TreeValue>, less<_TreeKey>, allocator<_TreeValue> > _Tree;
}

typedef std::_Tree Tree;
int main()
{
    Tree t;
    unsigned long keys[6] = { 0x80000000u, 0x10u, 0xF0000000u, 0xC0000000u, 0x8u, 0x20u };
    int ok = 1;
    for (int i = 0; i < 6; ++i) {
        std::pair<Tree::iterator, bool> r = t.insert_unique(std::_TreeValue(keys[i], i));
        ok &= r.second && (((Tree::_Link_type)r.first._M_node)->_M_value_field.first) == keys[i];
    }
    for (int i = 5; i >= 0; --i) {
        std::pair<Tree::iterator, bool> r = t.insert_unique(std::_TreeValue(keys[i], i));
        ok &= !r.second && (((Tree::_Link_type)r.first._M_node)->_M_value_field.first) == keys[i];
    }
    ok &= t.count() == 6;
    std::printf(ok ? "00ad3ef0_TEST PASS\n" : "00ad3ef0_TEST FAIL\n");
    return ok ? 0 : 1;
}
