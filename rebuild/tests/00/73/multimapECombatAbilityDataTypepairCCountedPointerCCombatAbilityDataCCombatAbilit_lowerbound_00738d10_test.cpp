// Behaviour test for 0x00738D10 multimap<ECombatAbilityDataType, ...>::lower_bound(const key&).
#include <stdio.h>
// ---- test model: STLport-shaped tree of combat abilities ----
class CCombatAbilityData { public:
    virtual ~CCombatAbilityData() {}
    virtual long GetDataType() const { return DataType; }
    long DataType;
    long InterfaceType;
    long WeaponClass;
};
template <class T> class CCountedPointer { public:
    T*    Object;
    long* Count;
    T* operator->() const { return Object; }
};
class CCombatAbility { public:
    long RepeatTimer;
    long ActiveTimer;
    void Reset()
    {
        ActiveTimer = -1;
        RepeatTimer = -1;
    }
};
struct SCombatAbilityPair { CCountedPointer<CCombatAbilityData> first; CCombatAbility second; };
struct SCombatAbilityValue { long first; SCombatAbilityPair second; };
struct _Rb_tree_node_base {
    long                _M_color;
    _Rb_tree_node_base* _M_parent;
    _Rb_tree_node_base* _M_left;
    _Rb_tree_node_base* _M_right;
};
struct _Rb_tree_node : _Rb_tree_node_base { SCombatAbilityValue _M_value_field; };
struct _Rb_global {
    static _Rb_tree_node_base* _M_increment(_Rb_tree_node_base* _M_node)
    {
        if (_M_node->_M_right != 0) {
            _M_node = _M_node->_M_right;
            while (_M_node->_M_left != 0)
                _M_node = _M_node->_M_left;
        }
        else {
            _Rb_tree_node_base* y = _M_node->_M_parent;
            while (_M_node == y->_M_right) {
                _M_node = y;
                y = y->_M_parent;
            }
            if (_M_node->_M_right != y)
                _M_node = y;
        }
        return _M_node;
    }
};
struct _Rb_tree_iterator {
    _Rb_tree_node_base* _M_node;
    _Rb_tree_iterator() {}
    _Rb_tree_iterator(_Rb_tree_node_base* x) { _M_node = x; }
    SCombatAbilityValue& operator*() const { return ((_Rb_tree_node*)_M_node)->_M_value_field; }
    SCombatAbilityValue* operator->() const { return &(operator*()); }
    _Rb_tree_iterator& operator++() { _M_node = _Rb_global::_M_increment(_M_node); return *this; }
    bool operator==(const _Rb_tree_iterator& o) const { return _M_node == o._M_node; }
    bool operator!=(const _Rb_tree_iterator& o) const { return _M_node != o._M_node; }
};
struct CCombatAbilityMap {
    typedef _Rb_tree_iterator iterator;
    _Rb_tree_node_base* _M_header;
    unsigned int        _M_node_count;
    iterator begin() { return iterator(_M_header->_M_left); }
    iterator end() { return iterator(_M_header); }
    static const long& _S_key(_Rb_tree_node_base* x) { return ((_Rb_tree_node*)x)->_M_value_field.first; }
    _Rb_tree_node_base* _M_lower_bound(const long& k) const
    {
        _Rb_tree_node_base* y = _M_header;
        _Rb_tree_node_base* x = _M_header->_M_parent;
        while (x != 0)
            if (!(_S_key(x) < k)) y = x, x = x->_M_left; else x = x->_M_right;
        return y;
    }
    _Rb_tree_node_base* _M_upper_bound(const long& k) const
    {
        _Rb_tree_node_base* y = _M_header;
        _Rb_tree_node_base* x = _M_header->_M_parent;
        while (x != 0)
            if (k < _S_key(x)) y = x, x = x->_M_left; else x = x->_M_right;
        return y;
    }
    iterator lower_bound(const long& k) { return iterator(_M_lower_bound(k)); }
    iterator upper_bound(const long& k) { return iterator(_M_upper_bound(k)); }
};
// plain (unbalanced) BST insert for building fixtures: equal keys go right, header bookkeeping as STLport
static void TestInsert(CCombatAbilityMap& m, _Rb_tree_node* z)
{
    _Rb_tree_node_base* h = m._M_header;
    _Rb_tree_node_base* y = h;
    _Rb_tree_node_base* x = h->_M_parent;
    while (x) { y = x; x = (z->_M_value_field.first < CCombatAbilityMap::_S_key(x)) ? x->_M_left : x->_M_right; }
    z->_M_parent = y; z->_M_left = 0; z->_M_right = 0;
    if (y == h) { h->_M_parent = z; h->_M_left = z; h->_M_right = z; }
    else if (z->_M_value_field.first < CCombatAbilityMap::_S_key(y)) { y->_M_left = z; if (y == h->_M_left) h->_M_left = z; }
    else { y->_M_right = z; if (y == h->_M_right) h->_M_right = z; }
    ++m._M_node_count;
}
static void TestInitMap(CCombatAbilityMap& m, _Rb_tree_node_base* header)
{
    header->_M_color = 0; header->_M_parent = 0; header->_M_left = header; header->_M_right = header;
    m._M_header = header; m._M_node_count = 0;
}
static void TestInitNode(_Rb_tree_node* n, long key, CCombatAbilityData* data)
{
    n->_M_color = 0;
    n->_M_value_field.first = key;
    n->_M_value_field.second.first.Object = data;
    n->_M_value_field.second.first.Count = 0;
    n->_M_value_field.second.second.RepeatTimer = 5;
    n->_M_value_field.second.second.ActiveTimer = 6;
}
// ---- end test model ----
struct CTestTree : CCombatAbilityMap {
    iterator lower_bound_ool(const long& k);
};
CTestTree::iterator CTestTree::lower_bound_ool(const long& k)
{
    return iterator(_M_lower_bound(k));
}
int main()
{
    int fail = 0;
    _Rb_tree_node_base header; _Rb_tree_node nodes[5];
    CTestTree t; TestInitMap(t, &header);
    long keys[5] = { 5, 2, 8, 2, 6 };
    for (int i = 0; i < 5; ++i) { TestInitNode(&nodes[i], keys[i], 0); TestInsert(t, &nodes[i]); }
    long k;
    k = 2; if (t.lower_bound_ool(k)._M_node != &nodes[1]) fail |= 1;   // first of the equal keys
    k = 3; if (t.lower_bound_ool(k)._M_node != &nodes[0]) fail |= 2;   // next larger key
    k = 6; if (t.lower_bound_ool(k)._M_node != &nodes[4]) fail |= 4;
    k = 9; if (t.lower_bound_ool(k)._M_node != &header) fail |= 8;     // past the end
    k = -1; if (t.lower_bound_ool(k)._M_node != &nodes[1]) fail |= 16;
    CTestTree e; _Rb_tree_node_base h2; TestInitMap(e, &h2);
    k = 1; if (e.lower_bound_ool(k)._M_node != &h2) fail |= 32;
    if (fail) { printf("00738d10_TEST FAIL %d\n", fail); return 1; }
    printf("00738d10_TEST PASS\n");
    return 0;
}