// Behaviour test for 0x00739B00 _Rb_tree<ECombatAbilityDataType, ...>::insert_equal(const value_type&).
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
    CCombatAbilityData d; d.id = 1; long count = 1;
    long keys[6] = { 4, 1, 7, 4, 0, 9 };
    _Rb_tree_node_base* made[6];
    for (int i = 0; i < 6; ++i) {
        SCombatAbilityValue v = TestValue(keys[i], &d, &count);
        made[i] = t.insert_equal(v)._M_node;
        if (CCombatAbilityTree::_S_key(made[i]) != keys[i]) fail |= 1;
    }
    long got[8]; int n = TestWalk(t, got, 8);
    long want[6] = { 0, 1, 4, 4, 7, 9 };
    if (n != 6) fail |= 2;
    for (int i = 0; i < 6 && i < n; ++i) if (got[i] != want[i]) fail |= 4;
    if (t._M_node_count != 6 || g_rebalances != 6) fail |= 8;
    if (header._M_left != made[4] || header._M_right != made[5] || header._M_parent != made[0]) fail |= 16;
    // equal key goes after the existing one (to its right)
    if (made[0]->_M_right == 0) fail |= 32;
    // each node copy took a reference (TestValue's temporaries took and dropped none: copy ctor only)
    if (count < 1 + 6) fail |= 64;
    if (fail) { printf("00739b00_TEST FAIL %d\n", fail); return 1; }
    printf("00739b00_TEST PASS\n");
    return 0;
}