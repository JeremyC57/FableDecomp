// Link test for 0x00AD5030: hinted inserts into a map<unsigned long,float> tree go through the source object's
// std::_Rb_tree<...>::insert_unique(iterator, const value_type&); the test supplies the STLport _M_insert and unhinted
// insert_unique bodies (explicitly instantiated here) and a no-op rebalance (the tree stays a valid search tree).
#include <cstdio>
#include <cstdlib>
#include <new>
using std::malloc; using std::free;

struct CCPPointerInfo {
    long  RefCount;                                     // +0
    void (__fastcall* DeleteFunc)(void* data);          // +4, Data in ecx
    void* Data;                                         // +8
};

template <class T> class CCountedPointer {
public:
    T*              Data;                               // +0
    CCPPointerInfo* Info;                               // +4
    CCountedPointer(const CCountedPointer& other) : Data(other.Data), Info(other.Info)
    {
        if (Info)
            ++Info->RefCount;
    }
    ~CCountedPointer()
    {
        if (Info) {
            --Info->RefCount;
            if (Info->RefCount == 0) {
                Info->DeleteFunc(Info->Data);
                delete Info;
            }
        }
        Data = 0;
        Info = 0;
    }
};

namespace QuadricOptimiserInternals {
class CVertexPairContraction;
class CContractionReference {
public:
    CCountedPointer<CVertexPairContraction> Contraction;   // +0
    unsigned long                           SubIndex;      // +8
};
}


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
    _Link_type _M_create_node(const value_type& __x)
    {
        _Link_type __tmp = _M_allocate_node();
        new (&__tmp->_M_value_field) value_type(__x);
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

    iterator _M_insert(_Base_ptr __x, _Base_ptr __y, const value_type& __v, _Base_ptr __w = 0);
    _Link_type _M_copy(_Link_type __x, _Link_type __p);
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
_Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>::_M_insert(_Base_ptr __x_, _Base_ptr __y_, const value_type& __v,
                                                                 _Base_ptr __w_)
{
    _Link_type __w = (_Link_type)__w_;
    _Link_type __x = (_Link_type)__x_;
    _Link_type __y = (_Link_type)__y_;
    _Link_type __z;

    if (__y == _M_header._M_data ||
        (__w == 0 &&                    // if w != 0, the remainder fails to false
         (__x != 0 ||                   // if x != 0, the remainder succeeds to true
          _M_key_compare(_KeyOfValue()(__v), _S_key(__y))))) {
        __z = _M_create_node(__v);
        _S_left(__y) = __z;             // also makes _M_leftmost() = z when y == header
        if (__y == _M_header._M_data) {
            _M_root() = __z;
            _M_rightmost() = __z;
        }
        else if (__y == _M_leftmost())
            _M_leftmost() = __z;        // maintain _M_leftmost() pointing to the min node
    }
    else {
        __z = _M_create_node(__v);
        _S_right(__y) = __z;
        if (__y == _M_rightmost())
            _M_rightmost() = __z;       // maintain _M_rightmost() pointing to the max node
    }
    __z->_M_parent = __y;
    _S_left(__z) = 0;
    _S_right(__z) = 0;
    _Rb_global_inst::_Rebalance(__z, _M_header._M_data->_M_parent);
    ++_M_node_count;
    return iterator(__z);
}
}   // namespace std
namespace std {
template <class _Key, class _Value, class _KeyOfValue, class _Compare, class _Alloc>
pair<typename _Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>::iterator, bool>
_Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>::insert_unique(const value_type& __v)
{
    _Link_type __y = _M_header._M_data;
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
}   // namespace std

template <> void __cdecl std::_Rb_global<bool>::_Rebalance(_Base_ptr __x, _Base_ptr& __root) { __x->_M_color = false; }
typedef std::pair<const unsigned long, float> TGroupValue;
typedef std::_Rb_tree<unsigned long, TGroupValue, std::_Select1st<TGroupValue>, std::less<unsigned long>,
                      std::allocator<TGroupValue> > TGroupMapTree;
template TGroupMapTree::iterator TGroupMapTree::_M_insert(std::_Rb_tree_node_base*, std::_Rb_tree_node_base*, const TGroupValue&, std::_Rb_tree_node_base*);
template std::pair<TGroupMapTree::iterator, bool> TGroupMapTree::insert_unique(const TGroupValue&);
typedef TGroupMapTree::_Link_type Link;
static int Walk(TGroupMapTree& t, unsigned long* keys, float* vals)
{
    int n = 0;
    for (TGroupMapTree::iterator it = t.begin(); it != t.end(); ++it) { keys[n] = (*it).first; vals[n] = (*it).second; ++n; }
    return n;
}
int main()
{
    int fail = 0;
    TGroupMapTree t;
    Link h = (Link)malloc(sizeof(*h));
    h->_M_color = false; h->_M_parent = 0; h->_M_left = h; h->_M_right = h;
    t._M_header._M_data = h; t._M_node_count = 0;
    // empty tree, begin hint: falls back on insert_unique(v)
    TGroupMapTree::iterator r = t.insert_unique(t.begin(), TGroupValue(50, 5.0f));
    if ((*r).first != 50 || t._M_node_count != 1) fail |= 1;
    // begin hint with a smaller key
    r = t.insert_unique(t.begin(), TGroupValue(10, 1.0f));
    if ((*r).first != 10 || t._M_node_count != 2) fail |= 2;
    // end hint with a larger key
    r = t.insert_unique(t.end(), TGroupValue(90, 9.0f));
    if ((*r).first != 90 || t._M_node_count != 3) fail |= 4;
    // interior hint (position = 50) bracketing 30
    TGroupMapTree::iterator p = t.begin(); ++p;
    r = t.insert_unique(p, TGroupValue(30, 3.0f));
    if ((*r).first != 30 || t._M_node_count != 4) fail |= 8;
    // equal key at the hint returns the hint, no insert
    p = t.begin(); ++p;   // 30
    r = t.insert_unique(p, TGroupValue(30, 7.0f));
    if (r != p || (*r).second != 3.0f || t._M_node_count != 4) fail |= 16;
    // wrong hint (begin) for a middle key falls back
    r = t.insert_unique(t.begin(), TGroupValue(70, 7.0f));
    if ((*r).first != 70 || t._M_node_count != 5) fail |= 32;
    // interior hint just before the key (position = 70, after = 90): inserted after the hint
    p = t.begin(); ++p; ++p; ++p;   // 70
    r = t.insert_unique(p, TGroupValue(80, 8.0f));
    if ((*r).first != 80 || t._M_node_count != 6) fail |= 64;
    unsigned long keys[16]; float vals[16];
    int n = Walk(t, keys, vals);
    static const unsigned long want[6] = { 10, 30, 50, 70, 80, 90 };
    if (n != 6) fail |= 128;
    for (int i = 0; i < n && i < 6; ++i) if (keys[i] != want[i] || vals[i] != want[i] / 10.0f) fail |= 256;
    if (fail) { std::printf("00ad5030_TEST FAIL %d\n", fail); return 1; }
    std::printf("00ad5030_TEST PASS\n");
    return 0;
}
