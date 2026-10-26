#include <cstdio>
#include <cstdlib>
#include <new>
// Test model of the screen-filter bypass map: STLport _Rb_tree<CThing*, pair<CThing* const, CByPassState>, ...>
// (header node, malloc'd 0x10-byte link + value nodes).
struct CTCGraphicAppearance {
    int Calls;
    bool LastByPass;
    void SetByPassColorFilter(bool byPass) { ++Calls; LastByPass = byPass; }
};
class CThing { public:
    int Id;
    unsigned char m_flags91;                            // bit 0: dead
    CTCGraphicAppearance* m_pGraphicAppearance;
};
class CIntelligentPointer {
public:
    CIntelligentPointer() : PItem(0) { }
    CIntelligentPointer(const CIntelligentPointer& other) : PItem(other.GetPItem()) { }
    CThing* GetPItem() const { return PItem; }
    void SetPItem(CThing* item) { PItem = item; }
private:
    CThing* PItem;
};
struct CByPassState {                                   // CThingGraphicByPassFlagState
    CIntelligentPointer thing;
    bool bypassReset;
    CByPassState() : bypassReset(false) { }
};
template <class _T1, class _T2>
struct pair {
    typedef _T1 first_type;
    _T1 first;
    _T2 second;
    pair(const _T1& __a, const _T2& __b) : first(__a), second(__b) { }
    pair(const pair& __o) : first(__o.first), second(__o.second) { }
};
template <class _Tp>
inline void _Construct(_Tp* __p, const _Tp& __val) { new (__p) _Tp(__val); }
struct _Rb_tree_node_base {
    bool _M_color;
    _Rb_tree_node_base* _M_parent;
    _Rb_tree_node_base* _M_left;
    _Rb_tree_node_base* _M_right;
};
template <class _Value>
struct _Rb_tree_node : public _Rb_tree_node_base {
    _Value _M_value_field;
};
static int g_rebalanceCalls = 0;
struct _Rb_global_inst {
    static void _Rebalance(_Rb_tree_node_base*, _Rb_tree_node_base*&) { ++g_rebalanceCalls; }   // stand-in (0x0042971D)
    static _Rb_tree_node_base* _M_increment(_Rb_tree_node_base* _M_node)
    {
        if (_M_node->_M_right != 0) {
            _M_node = _M_node->_M_right;
            while (_M_node->_M_left != 0)
                _M_node = _M_node->_M_left;
        }
        else {
            _Rb_tree_node_base* __y = _M_node->_M_parent;
            while (_M_node == __y->_M_right) {
                _M_node = __y;
                __y = __y->_M_parent;
            }
            if (_M_node->_M_right != __y)
                _M_node = __y;
        }
        return _M_node;
    }
};
template <class _Value>
struct _Rb_tree_iterator {
    _Rb_tree_node_base* _M_node;
    _Rb_tree_iterator() { }
    _Rb_tree_iterator(_Rb_tree_node_base* __x) { _M_node = __x; }
    _Value& operator*() const { return ((_Rb_tree_node<_Value>*)_M_node)->_M_value_field; }
    bool operator==(const _Rb_tree_iterator& __y) const { return _M_node == __y._M_node; }
    bool operator!=(const _Rb_tree_iterator& __y) const { return _M_node != __y._M_node; }
    _Rb_tree_iterator& operator++() { _M_node = _Rb_global_inst::_M_increment(_M_node); return *this; }
};
template <class _Pair>
struct _Select1st {
    const typename _Pair::first_type& operator()(const _Pair& __x) const { return __x.first; }
};
template <class _Tp>
struct less {
    bool operator()(const _Tp& __x, const _Tp& __y) const { return __x < __y; }
};
template <class _Key, class _Value, class _KeyOfValue, class _Compare>
class _Rb_tree {
public:
    typedef _Rb_tree_node<_Value> _Node;
    typedef _Rb_tree_node<_Value>* _Link_type;
    typedef _Rb_tree_node_base* _Base_ptr;
    typedef _Value value_type;
    typedef _Key key_type;
    typedef _Rb_tree_iterator<_Value> iterator;
    _Node* _M_header;
    unsigned int _M_node_count;
    _Compare _M_key_compare;
    _Rb_tree() : _M_node_count(0)
    {
        _M_header = (_Node*)malloc(sizeof(_Node));
        _M_header->_M_color = false;
        _M_header->_M_parent = 0;
        _M_header->_M_left = _M_header;
        _M_header->_M_right = _M_header;
    }
    _Node* _M_allocate() { return (_Node*)malloc(sizeof(_Node)); }
    _Base_ptr& _M_root() const { return _M_header->_M_parent; }
    _Base_ptr& _M_leftmost() const { return _M_header->_M_left; }
    _Base_ptr& _M_rightmost() const { return _M_header->_M_right; }
    static _Base_ptr& _S_left(_Base_ptr __x) { return __x->_M_left; }
    static _Base_ptr& _S_right(_Base_ptr __x) { return __x->_M_right; }
    static _Base_ptr& _S_parent(_Base_ptr __x) { return __x->_M_parent; }
    static const _Key& _S_key(_Base_ptr __x) { return _KeyOfValue()(((_Link_type)__x)->_M_value_field); }
    iterator begin() { return iterator(_M_leftmost()); }
    iterator end() { return iterator(_M_header); }
    _Compare key_comp() const { return _M_key_compare; }

    _Link_type _M_create_node(const value_type& __x)                         // 0x008840D0
    {
        _Link_type __tmp = _M_allocate();
        _Construct(&__tmp->_M_value_field, __x);
        return __tmp;
    }
    _Link_type _M_clone_node(_Link_type __x)                                  // 0x00884120
    {
        _Link_type __tmp = _M_create_node(__x->_M_value_field);
        __tmp->_M_color = __x->_M_color;
        __tmp->_M_left = 0;
        __tmp->_M_right = 0;
        return __tmp;
    }
    iterator _M_insert(_Base_ptr __x_, _Base_ptr __y_, const value_type& __v, _Base_ptr __w_)   // 0x008843F0
    {
        _Link_type __w = (_Link_type)__w_;
        _Link_type __x = (_Link_type)__x_;
        _Link_type __y = (_Link_type)__y_;
        _Link_type __z;
        if (__y == _M_header || (__w == 0 && (__x != 0 || _M_key_compare(_KeyOfValue()(__v), _S_key(__y))))) {
            __z = _M_create_node(__v);
            _S_left(__y) = __z;
            if (__y == _M_header) {
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
        _Rb_global_inst::_Rebalance(__z, _M_header->_M_parent);
        ++_M_node_count;
        return iterator(__z);
    }
    _Link_type _M_copy(_Link_type __x, _Link_type __p)                       // 0x00884490
    {
        _Link_type __top = _M_clone_node(__x);
        __top->_M_parent = __p;
        if (__x->_M_right)
            __top->_M_right = _M_copy((_Link_type)_S_right(__x), __top);
        __p = __top;
        __x = (_Link_type)_S_left(__x);
        while (__x != 0) {
            _Link_type __y = _M_clone_node(__x);
            __p->_M_left = __y;
            __y->_M_parent = __p;
            if (__x->_M_right)
                __y->_M_right = _M_copy((_Link_type)_S_right(__x), __y);
            __p = __y;
            __x = (_Link_type)_S_left(__x);
        }
        return __top;
    }
    iterator lower_bound(const key_type& __k)
    {
        _Base_ptr __y = _M_header;
        _Base_ptr __x = _M_root();
        while (__x != 0)
            if (!_M_key_compare(_S_key(__x), __k))
                __y = __x, __x = _S_left(__x);
            else
                __x = _S_right(__x);
        return iterator(__y);
    }
    iterator insert_unique(iterator, const value_type& __v)                   // stand-in for 0x00884830 (hint ignored)
    {
        _Base_ptr __y = _M_header;
        _Base_ptr __x = _M_root();
        bool __comp = true;
        while (__x != 0) {
            __y = __x;
            __comp = _M_key_compare(_KeyOfValue()(__v), _S_key(__x));
            __x = __comp ? _S_left(__x) : _S_right(__x);
        }
        return _M_insert(__x, __y, __v, 0);
    }
};
template <class _Key, class _Tp, class _Compare>
class map {
public:
    typedef _Key key_type;
    typedef pair<const _Key, _Tp> value_type;
    typedef _Rb_tree<key_type, value_type, _Select1st<value_type>, _Compare> _Rep_type;
    typedef typename _Rep_type::iterator iterator;
    _Rep_type _M_t;
    iterator begin() { return _M_t.begin(); }
    iterator end() { return _M_t.end(); }
    iterator lower_bound(const key_type& __x) { return _M_t.lower_bound(__x); }
    _Compare key_comp() const { return _M_t.key_comp(); }
    iterator insert(iterator __position, const value_type& __x) { return _M_t.insert_unique(__position, __x); }
    _Tp& operator[](const key_type& __k)                                      // 0x00884C70
    {
        iterator __i = lower_bound(__k);
        if (__i == end() || key_comp()(__k, (*__i).first))
            __i = insert(__i, value_type(__k, _Tp()));
        return (*__i).second;
    }
};
typedef map<CThing*, CByPassState, less<CThing*> > CResetMap;
typedef CResetMap::_Rep_type CTree;
typedef CResetMap::value_type CValue;
// _M_create_node @ 0x008840D0: malloc a node and copy-construct the pair into it.
int main()
{
    bool ok = true;
    CTree t;
    CThing th = { 7, 0, 0 };
    CByPassState st;
    st.thing.SetPItem(&th);
    st.bypassReset = true;
    CValue v(&th, st);
    CTree::_Link_type n = t._M_create_node(v);
    if (n == 0 || n->_M_value_field.first != &th || n->_M_value_field.second.thing.GetPItem() != &th
        || !n->_M_value_field.second.bypassReset)
        ok = false;
    if (t._M_node_count != 0) ok = false;               // creating a node does not link or count it
    free(n);
    if (!ok) { std::printf("008840d0_TEST FAIL\n"); return 1; }
    std::printf("008840d0_TEST PASS\n");
    return 0;
}