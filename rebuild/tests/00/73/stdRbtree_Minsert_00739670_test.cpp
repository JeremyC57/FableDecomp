// Behaviour test for 0x00739670 _Rb_tree<ECombatAbilityDataType, ...>::_M_insert(x, y, v, w).
#include <stdio.h>
#include <stdlib.h>
#include <new>
static int g_rebalances = 0;
template <class T> class CCountedPointer { public:
    T*    Object;
    long* Count;
    CCountedPointer(T* o, long* c) : Object(o), Count(c) {}
    CCountedPointer(const CCountedPointer& o) : Object(o.Object), Count(o.Count)
    {
        if (Count)
            ++*Count;
    }
};
class CCombatAbilityData { public: int id; };
class CCombatAbility { public: long RepeatTimer; long ActiveTimer; };
struct SCombatAbilityPair { CCountedPointer<CCombatAbilityData> first; CCombatAbility second; };
struct SCombatAbilityValue { long first; SCombatAbilityPair second; };
template <class T1, class T2>
inline void _Construct(T1* p, const T2& value)
{
    new (p) T1(value);
}
struct _Rb_tree_node_base {
    long                _M_color;
    _Rb_tree_node_base* _M_parent;
    _Rb_tree_node_base* _M_left;
    _Rb_tree_node_base* _M_right;
};
struct _Rb_tree_node : _Rb_tree_node_base { SCombatAbilityValue _M_value_field; };
struct _Rb_global {
    static void _Rebalance(_Rb_tree_node_base* x, _Rb_tree_node_base*& root) { ++g_rebalances; (void)x; (void)root; }   // test stub: no rotation
};
struct _Rb_tree_iterator {
    _Rb_tree_node_base* _M_node;
    _Rb_tree_iterator(_Rb_tree_node_base* x) { _M_node = x; }
};
struct CCombatAbilityTree {
    typedef _Rb_tree_node*     _Link_type;
    typedef _Rb_tree_iterator  iterator;
    typedef SCombatAbilityValue value_type;
    _Rb_tree_node_base* _M_header;
    unsigned int        _M_node_count;
    _Rb_tree_node_base*& _M_root() const { return _M_header->_M_parent; }
    _Rb_tree_node_base*& _M_leftmost() const { return _M_header->_M_left; }
    _Rb_tree_node_base*& _M_rightmost() const { return _M_header->_M_right; }
    static _Rb_tree_node_base*& _S_left(_Rb_tree_node_base* x) { return x->_M_left; }
    static _Rb_tree_node_base*& _S_right(_Rb_tree_node_base* x) { return x->_M_right; }
    static _Rb_tree_node_base*& _S_parent(_Rb_tree_node_base* x) { return x->_M_parent; }
    static const long& _S_key(_Rb_tree_node_base* x) { return ((_Link_type)x)->_M_value_field.first; }
    static _Link_type allocate(unsigned int n) { return n != 0 ? (_Link_type)malloc(n * sizeof(_Rb_tree_node)) : 0; }
    _Link_type _M_create_node(const value_type& x)
    {
        _Link_type tmp = allocate(1);
        _Construct(&tmp->_M_value_field, x);
        return tmp;
    }
    iterator _M_insert(_Rb_tree_node_base* x_, _Rb_tree_node_base* y_, const value_type& v, _Rb_tree_node_base* w_ = 0);
    iterator insert_equal(const value_type& v);
};
CCombatAbilityTree::iterator CCombatAbilityTree::_M_insert(_Rb_tree_node_base* x_, _Rb_tree_node_base* y_, const value_type& v, _Rb_tree_node_base* w_)
{
    _Link_type w = (_Link_type)w_;
    _Link_type x = (_Link_type)x_;
    _Link_type y = (_Link_type)y_;
    _Link_type z;

    if (y == _M_header || (w == 0 && (x != 0 || v.first < _S_key(y)))) {
        z = _M_create_node(v);
        _S_left(y) = z;
        if (y == _M_header) {
            _M_root() = z;
            _M_rightmost() = z;
        }
        else if (y == _M_leftmost())
            _M_leftmost() = z;
    }
    else {
        z = _M_create_node(v);
        _S_right(y) = z;
        if (y == _M_rightmost())
            _M_rightmost() = z;
    }
    _S_parent(z) = y;
    _S_left(z) = 0;
    _S_right(z) = 0;
    _Rb_global::_Rebalance(z, _M_header->_M_parent);
    ++_M_node_count;
    return iterator(z);
}
CCombatAbilityTree::iterator CCombatAbilityTree::insert_equal(const value_type& v)
{
    _Rb_tree_node_base* y = _M_header;
    _Rb_tree_node_base* x = _M_root();
    while (x != 0) {
        y = x;
        x = (v.first < _S_key(x)) ? _S_left(x) : _S_right(x);
    }
    return _M_insert(x, y, v);
}
// in-order walk (STLport increment) into keys[]
static int TestWalk(CCombatAbilityTree& t, long* keys, int max)
{
    int n = 0;
    _Rb_tree_node_base* h = t._M_header;
    _Rb_tree_node_base* x = h->_M_left;
    while (x != h && n < max) {
        keys[n++] = CCombatAbilityTree::_S_key(x);
        if (x->_M_right) { x = x->_M_right; while (x->_M_left) x = x->_M_left; }
        else { _Rb_tree_node_base* y = x->_M_parent; while (x == y->_M_right) { x = y; y = y->_M_parent; } if (x->_M_right != y) x = y; }
    }
    return n;
}
static SCombatAbilityValue TestValue(long key, CCombatAbilityData* d, long* count)
{
    SCombatAbilityValue v = { key, { CCountedPointer<CCombatAbilityData>(d, count), { 3, 4 } } };
    return v;
}
int main()
{
    int fail = 0;
    _Rb_tree_node_base header = { 0, 0, &header, &header };
    CCombatAbilityTree t; t._M_header = &header; t._M_node_count = 0;
    CCombatAbilityData d; d.id = 1; long count = 0;
    SCombatAbilityValue v5 = TestValue(5, &d, &count);
    long before = count;
    // first node: y == header -> root, leftmost, rightmost
    _Rb_tree_node_base* a = t._M_insert(0, &header, v5)._M_node;
    if (header._M_parent != a || header._M_left != a || header._M_right != a || a->_M_parent != &header) fail |= 1;
    if (count != before + 1) fail |= 2;                       // the value copy took a reference
    if (((_Rb_tree_node*)a)->_M_value_field.second.first.Object != &d || ((_Rb_tree_node*)a)->_M_value_field.second.second.ActiveTimer != 4) fail |= 4;
    // smaller key under a (x == 0, w == 0, key < key(y)) -> left child, new leftmost
    SCombatAbilityValue v2 = TestValue(2, &d, 0);
    _Rb_tree_node_base* b = t._M_insert(0, a, v2)._M_node;
    if (a->_M_left != b || header._M_left != b || b->_M_parent != a || b->_M_left || b->_M_right) fail |= 8;
    // larger key under a -> right child, new rightmost
    SCombatAbilityValue v8 = TestValue(8, &d, 0);
    _Rb_tree_node_base* c = t._M_insert(0, a, v8)._M_node;
    if (a->_M_right != c || header._M_right != c) fail |= 16;
    // w != 0 forces the right side even for a smaller key
    SCombatAbilityValue v1 = TestValue(1, &d, 0);
    _Rb_tree_node_base* e = t._M_insert(0, b, v1, b)._M_node;
    if (b->_M_right != e || header._M_left != b) fail |= 32;
    // x != 0 forces the left side even for a larger key
    SCombatAbilityValue v9 = TestValue(9, &d, 0);
    _Rb_tree_node_base* f = t._M_insert(c, c, v9)._M_node;
    if (c->_M_left != f || header._M_right != c) fail |= 64;
    if (t._M_node_count != 5 || g_rebalances != 5) fail |= 128;
    if (fail) { printf("00739670_TEST FAIL %d\n", fail); return 1; }
    printf("00739670_TEST PASS\n");
    return 0;
}