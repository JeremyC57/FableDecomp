// Behaviour test for 0056d390: std::_Rb_tree<CUnprovenKey_0056cb00, CUnprovenValue_0056cb00, CUnprovenKeyOfValue_0056cb00, std::less<CUnprovenKey_0056cb00>, std::allocator<CUnprovenValue_0056cb00>>::_Rb_tree (STLport 4.6 _Rb_tree model, real rebalancing; exercises the same member)
#include <cstdio>
extern "C" void* __cdecl malloc(unsigned int __size);   // 0x00BFEA0E
extern "C" void __cdecl free(void* __p);                 // 0x00BFEA14
inline void* __cdecl operator new(unsigned int, void* __p) { return __p; }
inline void __cdecl operator delete(void*, void*) { }
struct CUnprovenKey_0056cb00 { long _M_unproven; };
inline bool operator<(const CUnprovenKey_0056cb00& __x, const CUnprovenKey_0056cb00& __y) { return __x._M_unproven < __y._M_unproven; }
struct CUnprovenValue_0056cb00 { CUnprovenKey_0056cb00 _M_key; unsigned char _M_unproven[4]; };
struct CUnprovenKeyOfValue_0056cb00 { const CUnprovenKey_0056cb00& operator()(const CUnprovenValue_0056cb00& __v) const { return __v._M_key; } };
namespace std {
template <class _Tp> class allocator {
public:
    typedef _Tp value_type;
    allocator() { }
    allocator(const allocator<_Tp>&) { }
    _Tp* allocate(unsigned int __n, const void* = 0) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
    _STLP_alloc_proxy(const _MaybeReboundAlloc& __a, _Value __p) : _MaybeReboundAlloc(__a), _M_data(__p) { }
};
template <class _T1, class _T2> struct pair {
    typedef _T1 first_type;
    typedef _T2 second_type;
    _T1 first;
    _T2 second;
    pair() : first(_T1()), second(_T2()) { }
    pair(const _T1& __a, const _T2& __b) : first(__a), second(__b) { }
};
template <class _Arg1, class _Arg2, class _Result> struct binary_function {
    typedef _Arg1 first_argument_type;
    typedef _Arg2 second_argument_type;
    typedef _Result result_type;
};
template <class _Tp> struct less : public binary_function<_Tp, _Tp, bool> {
    bool operator()(const _Tp& __x, const _Tp& __y) const { return __x < __y; }
};
template <class _Tp> struct _Identity {
    const _Tp& operator()(const _Tp& __x) const { return __x; }
};
template <class _Pair> struct _Select1st {
    const typename _Pair::first_type& operator()(const _Pair& __x) const { return __x.first; }
};
template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }
template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }
template <class _ForwardIterator> inline void _Destroy(_ForwardIterator __first, _ForwardIterator __last) { for (; __first != __last; ++__first) _Destroy(&*__first); }
struct input_iterator_tag { };
struct forward_iterator_tag : public input_iterator_tag { };
struct bidirectional_iterator_tag : public forward_iterator_tag { };
struct _Rb_tree_node_base {
    typedef bool _Color_type;
    typedef _Rb_tree_node_base* _Base_ptr;
    _Color_type _M_color;
    _Base_ptr   _M_parent;
    _Base_ptr   _M_left;
    _Base_ptr   _M_right;
    static _Base_ptr __cdecl _S_minimum(_Base_ptr __x) { while (__x->_M_left != 0) __x = __x->_M_left; return __x; }
    static _Base_ptr __cdecl _S_maximum(_Base_ptr __x) { while (__x->_M_right != 0) __x = __x->_M_right; return __x; }
};
template <class _Value> struct _Rb_tree_node : public _Rb_tree_node_base {
    _Value _M_value_field;
};
template <class _Dummy> class _Rb_global {
public:
    typedef _Rb_tree_node_base* _Base_ptr;
    static void __cdecl _Rebalance(_Rb_tree_node_base* __x, _Rb_tree_node_base*& __root);                 // 0x0042971D
    static _Rb_tree_node_base* __cdecl _Rebalance_for_erase(_Rb_tree_node_base* __z, _Rb_tree_node_base*& __root,
                                                            _Rb_tree_node_base*& __leftmost,
                                                            _Rb_tree_node_base*& __rightmost);              // 0x0042A34E
    static _Rb_tree_node_base* __cdecl _M_increment(_Rb_tree_node_base* __x);
    static _Rb_tree_node_base* __cdecl _M_decrement(_Rb_tree_node_base* __x);
};
typedef _Rb_global<bool> _Rb_global_inst;
template <class _Dummy>
_Rb_tree_node_base* __cdecl _Rb_global<_Dummy>::_M_increment(_Rb_tree_node_base* _M_node)
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
template <class _Iterator> struct iterator_traits {
    typedef typename _Iterator::iterator_category iterator_category;
    typedef typename _Iterator::value_type value_type;
    typedef typename _Iterator::difference_type difference_type;
};
template <class _InputIterator>
inline typename iterator_traits<_InputIterator>::difference_type __cdecl
__distance(const _InputIterator& __first, const _InputIterator& __last, const input_iterator_tag&)
{
    typename iterator_traits<_InputIterator>::difference_type __n = 0;
    _InputIterator __it(__first);
    while (__it != __last) {
        ++__it; ++__n;
    }
    return __n;
}
template <class _InputIterator>
inline typename iterator_traits<_InputIterator>::difference_type __cdecl
distance(const _InputIterator& __first, const _InputIterator& __last)
{
    return __distance(__first, __last, typename iterator_traits<_InputIterator>::iterator_category());
}
struct _Rb_tree_base_iterator {
    typedef _Rb_tree_node_base* _Base_ptr;
    _Base_ptr _M_node;
};
inline bool operator==(const _Rb_tree_base_iterator& __x, const _Rb_tree_base_iterator& __y) { return __x._M_node == __y._M_node; }
inline bool operator!=(const _Rb_tree_base_iterator& __x, const _Rb_tree_base_iterator& __y) { return __x._M_node != __y._M_node; }
template <class _Tp> struct _Nonconst_traits {
    typedef _Tp value_type;
    typedef _Tp& reference;
    typedef _Tp* pointer;
};
template <class _Tp> struct _Const_traits {
    typedef _Tp value_type;
    typedef const _Tp& reference;
    typedef const _Tp* pointer;
};
template <class _Value, class _Traits> struct _Rb_tree_iterator : public _Rb_tree_base_iterator {
    typedef bidirectional_iterator_tag iterator_category;
    typedef int difference_type;
    typedef _Value value_type;
    typedef typename _Traits::reference reference;
    typedef typename _Traits::pointer pointer;
    typedef _Rb_tree_iterator<_Value, _Traits> _Self;
    typedef _Rb_tree_node<_Value>* _Link_type;
    _Rb_tree_iterator() { _M_node = 0; }
    _Rb_tree_iterator(_Link_type __x) { _M_node = __x; }
    _Rb_tree_iterator(const _Rb_tree_iterator<_Value, _Nonconst_traits<_Value> >& __it) { _M_node = __it._M_node; }
    reference operator*() const { return _Link_type(_M_node)->_M_value_field; }
    pointer operator->() const { return &(operator*()); }
    _Self& operator++() { _M_node = _Rb_global_inst::_M_increment(_M_node); return *this; }
    _Self operator++(int) { _Self __tmp = *this; _M_node = _Rb_global_inst::_M_increment(_M_node); return __tmp; }
    _Self& operator--() { _M_node = _Rb_global_inst::_M_decrement(_M_node); return *this; }
    _Self operator--(int) { _Self __tmp = *this; _M_node = _Rb_global_inst::_M_decrement(_M_node); return __tmp; }
};
template <class _Tp, class _Alloc> struct _Rb_tree_base {
    typedef _Rb_tree_node<_Tp> _Node;
    typedef _Alloc allocator_type;
    _Rb_tree_base(const allocator_type& __a) : _M_header(allocator<_Node>(), (_Node*)0) { _M_header._M_data = _M_header.allocate(1); }
    ~_Rb_tree_base() { _M_header.deallocate(_M_header._M_data, 1); }
protected:
    _STLP_alloc_proxy<_Node*, _Node, allocator<_Node> > _M_header;
};
template <class _Key, class _Value, class _KeyOfValue, class _Compare, class _Alloc>
class _Rb_tree : public _Rb_tree_base<_Value, _Alloc> {
    typedef _Rb_tree_base<_Value, _Alloc> _Base;
protected:
    typedef _Rb_tree_node_base* _Base_ptr;
    typedef _Rb_tree_node<_Value> _Node;
    typedef bool _Color_type;
public:
    typedef _Key key_type;
    typedef _Value value_type;
    typedef value_type& reference;
    typedef const value_type& const_reference;
    typedef _Rb_tree_node<_Value>* _Link_type;
    typedef unsigned int size_type;
    typedef int difference_type;
    typedef typename _Base::allocator_type allocator_type;
    enum { _S_rb_tree_red = false, _S_rb_tree_black = true };
protected:
    _Link_type _M_create_node(const value_type& __x)
    {
        _Link_type __tmp = this->_M_header.allocate(1);
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
protected:
    size_type _M_node_count;
    _Compare _M_key_compare;
    _Link_type& _M_root() const { return (_Link_type&)this->_M_header._M_data->_M_parent; }
    _Link_type& _M_leftmost() const { return (_Link_type&)this->_M_header._M_data->_M_left; }
    _Link_type& _M_rightmost() const { return (_Link_type&)this->_M_header._M_data->_M_right; }
    static _Link_type& __cdecl _S_left(_Link_type __x) { return (_Link_type&)(__x->_M_left); }
    static _Link_type& __cdecl _S_right(_Link_type __x) { return (_Link_type&)(__x->_M_right); }
    static _Link_type& __cdecl _S_parent(_Link_type __x) { return (_Link_type&)(__x->_M_parent); }
    static reference __cdecl _S_value(_Link_type __x) { return __x->_M_value_field; }
    static const _Key& __cdecl _S_key(_Link_type __x) { return _KeyOfValue()(_S_value(__x)); }
    static _Color_type& __cdecl _S_color(_Link_type __x) { return (_Color_type&)(__x->_M_color); }
    static _Link_type& __cdecl _S_left(_Base_ptr __x) { return (_Link_type&)(__x->_M_left); }
    static _Link_type& __cdecl _S_right(_Base_ptr __x) { return (_Link_type&)(__x->_M_right); }
    static _Link_type& __cdecl _S_parent(_Base_ptr __x) { return (_Link_type&)(__x->_M_parent); }
    static reference __cdecl _S_value(_Base_ptr __x) { return ((_Link_type)__x)->_M_value_field; }
    static const _Key& __cdecl _S_key(_Base_ptr __x) { return _KeyOfValue()(_S_value(_Link_type(__x))); }
    static _Color_type& __cdecl _S_color(_Base_ptr __x) { return (_Color_type&)(_Link_type(__x)->_M_color); }
    static _Link_type __cdecl _S_minimum(_Link_type __x) { return (_Link_type)_Rb_tree_node_base::_S_minimum(__x); }
    static _Link_type __cdecl _S_maximum(_Link_type __x) { return (_Link_type)_Rb_tree_node_base::_S_maximum(__x); }
public:
    typedef _Rb_tree_iterator<value_type, _Nonconst_traits<value_type> > iterator;
    typedef _Rb_tree_iterator<value_type, _Const_traits<value_type> > const_iterator;
private:
    iterator _M_insert(_Base_ptr __x, _Base_ptr __y, const value_type& __v, _Base_ptr __w = 0);
    _Link_type _M_copy(_Link_type __x, _Link_type __p);
    void _M_erase(_Link_type __x);
public:
    _Rb_tree() : _Rb_tree_base<_Value, _Alloc>(allocator_type()), _M_node_count(0), _M_key_compare(_Compare()) { _M_empty_initialize(); }
    _Rb_tree(const _Compare& __comp) : _Rb_tree_base<_Value, _Alloc>(allocator_type()), _M_node_count(0), _M_key_compare(__comp) { _M_empty_initialize(); }
    _Rb_tree(const _Compare& __comp, const allocator_type& __a) : _Rb_tree_base<_Value, _Alloc>(__a), _M_node_count(0), _M_key_compare(__comp) { _M_empty_initialize(); }
    ~_Rb_tree() { clear(); }
    _Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>& operator=(const _Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>& __x);
private:
    void _M_empty_initialize()
    {
        _S_color(this->_M_header._M_data) = _S_rb_tree_red;
        _M_root() = 0;
        _M_leftmost() = this->_M_header._M_data;
        _M_rightmost() = this->_M_header._M_data;
    }
public:
    iterator begin() { return iterator(_M_leftmost()); }
    const_iterator begin() const { return const_iterator(_M_leftmost()); }
    iterator end() { return iterator(this->_M_header._M_data); }
    const_iterator end() const { return const_iterator(this->_M_header._M_data); }
    bool empty() const { return _M_node_count == 0; }
    size_type size() const { return _M_node_count; }
    pair<iterator, bool> insert_unique(const value_type& __x);
    iterator insert_equal(const value_type& __x);
    iterator insert_unique(iterator __position, const value_type& __x);
    iterator insert_equal(iterator __position, const value_type& __x);
    void erase(iterator __position)
    {
        _Link_type __y = (_Link_type)_Rb_global_inst::_Rebalance_for_erase(__position._M_node,
                                                                           this->_M_header._M_data->_M_parent,
                                                                           this->_M_header._M_data->_M_left,
                                                                           this->_M_header._M_data->_M_right);
        _Destroy(&__y->_M_value_field);
        this->_M_header.deallocate(__y, 1);
        --_M_node_count;
    }
    size_type erase(const key_type& __x);
    void erase(iterator __first, iterator __last)
    {
        if (__first == begin() && __last == end())
            clear();
        else
            while (__first != __last) erase(__first++);
    }
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
    iterator find(const key_type& __x) { return iterator(_M_find(__x)); }
    const_iterator find(const key_type& __x) const { return const_iterator(_M_find(__x)); }
private:
    _Rb_tree_node<_Value>* _M_find(const key_type& __k) const
    {
        _Link_type __y = this->_M_header._M_data;
        _Link_type __x = _M_root();
        while (__x != 0)
            if (!_M_key_compare(_S_key(__x), __k))
                __y = __x, __x = _S_left(__x);
            else
                __x = _S_right(__x);
        if (__y == this->_M_header._M_data || _M_key_compare(__k, _S_key(__y)))
            __y = this->_M_header._M_data;
        return __y;
    }
    _Link_type _M_lower_bound(const key_type& __k) const
    {
        _Link_type __y = this->_M_header._M_data;
        _Link_type __x = _M_root();
        while (__x != 0)
            if (!_M_key_compare(_S_key(__x), __k))
                __y = __x, __x = _S_left(__x);
            else
                __x = _S_right(__x);
        return __y;
    }
    _Link_type _M_upper_bound(const key_type& __k) const
    {
        _Link_type __y = this->_M_header._M_data;
        _Link_type __x = _M_root();
        while (__x != 0)
            if (_M_key_compare(__k, _S_key(__x)))
                __y = __x, __x = _S_left(__x);
            else
                __x = _S_right(__x);
        return __y;
    }
public:
    size_type count(const key_type& __x) const;
    iterator lower_bound(const key_type& __x) { return iterator(_M_lower_bound(__x)); }
    const_iterator lower_bound(const key_type& __x) const { return const_iterator(_M_lower_bound(__x)); }
    iterator upper_bound(const key_type& __x) { return iterator(_M_upper_bound(__x)); }
    const_iterator upper_bound(const key_type& __x) const { return const_iterator(_M_upper_bound(__x)); }
    pair<iterator, iterator> equal_range(const key_type& __x) { return pair<iterator, iterator>(lower_bound(__x), upper_bound(__x)); }
};
template <class _Key, class _Value, class _KeyOfValue, class _Compare, class _Alloc>
typename _Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>::iterator
_Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>::_M_insert(_Rb_tree_node_base* __x_, _Rb_tree_node_base* __y_, const _Value& __v, _Rb_tree_node_base* __w_)
{
    _Link_type __w = (_Link_type)__w_;
    _Link_type __x = (_Link_type)__x_;
    _Link_type __y = (_Link_type)__y_;
    _Link_type __z;
    if (__y == this->_M_header._M_data ||
        (__w == 0 &&
         (__x != 0 ||
          _M_key_compare(_KeyOfValue()(__v), _S_key(__y))))) {
        __z = _M_create_node(__v);
        _S_left(__y) = __z;
        if (__y == this->_M_header._M_data) {
            _M_root() = __z;
            _M_rightmost() = __z;
        }
        else if (__y == _M_leftmost())
            _M_leftmost() = __z;
    }
    else {
        __z = _M_create_node(__v);
        _S_right(__y) = __z;
        if (__y == _M_rightmost())
            _M_rightmost() = __z;
    }
    _S_parent(__z) = __y;
    _S_left(__z) = 0;
    _S_right(__z) = 0;
    _Rb_global_inst::_Rebalance(__z, this->_M_header._M_data->_M_parent);
    ++_M_node_count;
    return iterator(__z);
}
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
void _Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>::_M_erase(_Rb_tree_node<_Value>* __x)
{
    // erase without rebalancing
    while (__x != 0) {
        _M_erase(_S_right(__x));
        _Link_type __y = _S_left(__x);
        _Destroy(&__x->_M_value_field);
        this->_M_header.deallocate(__x, 1);
        __x = __y;
    }
}
template <class _Key, class _Value, class _KeyOfValue, class _Compare, class _Alloc>
_Rb_tree_node<_Value>*
_Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>::_M_copy(_Rb_tree_node<_Value>* __x, _Rb_tree_node<_Value>* __p)
{
    // structural copy.  __x and __p must be non-null.
    _Link_type __top = _M_clone_node(__x);
    __top->_M_parent = __p;
    if (__x->_M_right)
        __top->_M_right = _M_copy(_S_right(__x), __top);
    __p = __top;
    __x = _S_left(__x);
    while (__x != 0) {
        _Link_type __y = _M_clone_node(__x);
        __p->_M_left = __y;
        __y->_M_parent = __p;
        if (__x->_M_right)
            __y->_M_right = _M_copy(_S_right(__x), __y);
        __p = __y;
        __x = _S_left(__x);
    }
    return __top;
}
template <class _Key, class _Value, class _KeyOfValue, class _Compare, class _Alloc>
_Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>&
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
}
namespace std {
static void __cdecl _T_rotate_left(_Rb_tree_node_base* __x, _Rb_tree_node_base*& __root)
{
    _Rb_tree_node_base* __y = __x->_M_right;
    __x->_M_right = __y->_M_left;
    if (__y->_M_left != 0) __y->_M_left->_M_parent = __x;
    __y->_M_parent = __x->_M_parent;
    if (__x == __root) __root = __y;
    else if (__x == __x->_M_parent->_M_left) __x->_M_parent->_M_left = __y;
    else __x->_M_parent->_M_right = __y;
    __y->_M_left = __x;
    __x->_M_parent = __y;
}
static void __cdecl _T_rotate_right(_Rb_tree_node_base* __x, _Rb_tree_node_base*& __root)
{
    _Rb_tree_node_base* __y = __x->_M_left;
    __x->_M_left = __y->_M_right;
    if (__y->_M_right != 0) __y->_M_right->_M_parent = __x;
    __y->_M_parent = __x->_M_parent;
    if (__x == __root) __root = __y;
    else if (__x == __x->_M_parent->_M_right) __x->_M_parent->_M_right = __y;
    else __x->_M_parent->_M_left = __y;
    __y->_M_right = __x;
    __x->_M_parent = __y;
}
template <class _Dummy>
void __cdecl _Rb_global<_Dummy>::_Rebalance(_Rb_tree_node_base* __x, _Rb_tree_node_base*& __root)
{
    __x->_M_color = false;
    while (__x != __root && __x->_M_parent->_M_color == false) {
        if (__x->_M_parent == __x->_M_parent->_M_parent->_M_left) {
            _Rb_tree_node_base* __y = __x->_M_parent->_M_parent->_M_right;
            if (__y && __y->_M_color == false) {
                __x->_M_parent->_M_color = true; __y->_M_color = true;
                __x->_M_parent->_M_parent->_M_color = false; __x = __x->_M_parent->_M_parent;
            } else {
                if (__x == __x->_M_parent->_M_right) { __x = __x->_M_parent; _T_rotate_left(__x, __root); }
                __x->_M_parent->_M_color = true; __x->_M_parent->_M_parent->_M_color = false;
                _T_rotate_right(__x->_M_parent->_M_parent, __root);
            }
        } else {
            _Rb_tree_node_base* __y = __x->_M_parent->_M_parent->_M_left;
            if (__y && __y->_M_color == false) {
                __x->_M_parent->_M_color = true; __y->_M_color = true;
                __x->_M_parent->_M_parent->_M_color = false; __x = __x->_M_parent->_M_parent;
            } else {
                if (__x == __x->_M_parent->_M_left) { __x = __x->_M_parent; _T_rotate_right(__x, __root); }
                __x->_M_parent->_M_color = true; __x->_M_parent->_M_parent->_M_color = false;
                _T_rotate_left(__x->_M_parent->_M_parent, __root);
            }
        }
    }
    __root->_M_color = true;
}
template <class _Dummy>
_Rb_tree_node_base* __cdecl _Rb_global<_Dummy>::_Rebalance_for_erase(_Rb_tree_node_base* __z, _Rb_tree_node_base*& __root,
                                                                     _Rb_tree_node_base*& __leftmost, _Rb_tree_node_base*& __rightmost)
{
    _Rb_tree_node_base* __y = __z;
    _Rb_tree_node_base* __x = 0;
    _Rb_tree_node_base* __x_parent = 0;
    if (__y->_M_left == 0) __x = __y->_M_right;
    else if (__y->_M_right == 0) __x = __y->_M_left;
    else { __y = __y->_M_right; while (__y->_M_left != 0) __y = __y->_M_left; __x = __y->_M_right; }
    if (__y != __z) {
        __z->_M_left->_M_parent = __y; __y->_M_left = __z->_M_left;
        if (__y != __z->_M_right) {
            __x_parent = __y->_M_parent;
            if (__x) __x->_M_parent = __y->_M_parent;
            __y->_M_parent->_M_left = __x; __y->_M_right = __z->_M_right; __z->_M_right->_M_parent = __y;
        } else __x_parent = __y;
        if (__root == __z) __root = __y;
        else if (__z->_M_parent->_M_left == __z) __z->_M_parent->_M_left = __y;
        else __z->_M_parent->_M_right = __y;
        __y->_M_parent = __z->_M_parent;
        bool __c = __y->_M_color; __y->_M_color = __z->_M_color; __z->_M_color = __c;
        __y = __z;
    } else {
        __x_parent = __y->_M_parent;
        if (__x) __x->_M_parent = __y->_M_parent;
        if (__root == __z) __root = __x;
        else if (__z->_M_parent->_M_left == __z) __z->_M_parent->_M_left = __x;
        else __z->_M_parent->_M_right = __x;
        if (__leftmost == __z) {
            if (__z->_M_right == 0) __leftmost = __z->_M_parent;
            else __leftmost = _Rb_tree_node_base::_S_minimum(__x);
        }
        if (__rightmost == __z) {
            if (__z->_M_left == 0) __rightmost = __z->_M_parent;
            else __rightmost = _Rb_tree_node_base::_S_maximum(__x);
        }
    }
    if (__y->_M_color != false) {
        while (__x != __root && (__x == 0 || __x->_M_color == true))
            if (__x == __x_parent->_M_left) {
                _Rb_tree_node_base* __w = __x_parent->_M_right;
                if (__w->_M_color == false) { __w->_M_color = true; __x_parent->_M_color = false; _T_rotate_left(__x_parent, __root); __w = __x_parent->_M_right; }
                if ((__w->_M_left == 0 || __w->_M_left->_M_color == true) && (__w->_M_right == 0 || __w->_M_right->_M_color == true)) {
                    __w->_M_color = false; __x = __x_parent; __x_parent = __x_parent->_M_parent;
                } else {
                    if (__w->_M_right == 0 || __w->_M_right->_M_color == true) {
                        if (__w->_M_left) __w->_M_left->_M_color = true;
                        __w->_M_color = false; _T_rotate_right(__w, __root); __w = __x_parent->_M_right;
                    }
                    __w->_M_color = __x_parent->_M_color; __x_parent->_M_color = true;
                    if (__w->_M_right) __w->_M_right->_M_color = true;
                    _T_rotate_left(__x_parent, __root); break;
                }
            } else {
                _Rb_tree_node_base* __w = __x_parent->_M_left;
                if (__w->_M_color == false) { __w->_M_color = true; __x_parent->_M_color = false; _T_rotate_right(__x_parent, __root); __w = __x_parent->_M_left; }
                if ((__w->_M_right == 0 || __w->_M_right->_M_color == true) && (__w->_M_left == 0 || __w->_M_left->_M_color == true)) {
                    __w->_M_color = false; __x = __x_parent; __x_parent = __x_parent->_M_parent;
                } else {
                    if (__w->_M_left == 0 || __w->_M_left->_M_color == true) {
                        if (__w->_M_right) __w->_M_right->_M_color = true;
                        __w->_M_color = false; _T_rotate_left(__w, __root); __w = __x_parent->_M_left;
                    }
                    __w->_M_color = __x_parent->_M_color; __x_parent->_M_color = true;
                    if (__w->_M_left) __w->_M_left->_M_color = true;
                    _T_rotate_right(__x_parent, __root); break;
                }
            }
        if (__x) __x->_M_color = true;
    }
    return __y;
}
}
struct _TestTree : public std::_Rb_tree<CUnprovenKey_0056cb00, CUnprovenValue_0056cb00, CUnprovenKeyOfValue_0056cb00, std::less<CUnprovenKey_0056cb00 >, std::allocator<CUnprovenValue_0056cb00 > > {
    _TestTree() : std::_Rb_tree<CUnprovenKey_0056cb00, CUnprovenValue_0056cb00, CUnprovenKeyOfValue_0056cb00, std::less<CUnprovenKey_0056cb00 >, std::allocator<CUnprovenValue_0056cb00 > >(std::less<CUnprovenKey_0056cb00 >(), std::allocator<CUnprovenValue_0056cb00 >()) { }
    std::_Rb_tree_node_base* hdr() { return this->_M_header._M_data; }
    unsigned int cnt() { return this->_M_node_count; }
};
int main()
{
    int ok = 1;
    { _TestTree t; if (!(t.cnt() == 0 && t.hdr()->_M_left == t.hdr() && t.hdr()->_M_right == t.hdr() && t.hdr()->_M_parent == 0 && t.hdr()->_M_color == false)) ok = 0; }
    { _TestTree t; for (int i = 1; i <= 5; ++i) { CUnprovenValue_0056cb00 v; v._M_key._M_unproven = i; t.insert_unique(v); } if (!(t.size() == 5)) ok = 0; }
    if (!ok) { std::printf("0056d390_TEST FAIL\n"); return 1; }
    std::printf("0056d390_TEST PASS\n");
    return 0;
}
