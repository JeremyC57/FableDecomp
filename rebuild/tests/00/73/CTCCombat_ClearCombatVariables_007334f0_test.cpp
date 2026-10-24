// Behaviour test for 0x007334F0 CTCCombat::ClearCombatVariables().
#include <stdio.h>
// ---- test model: STLport-shaped tree of combat abilities ----
struct CCombatAbilityData {
    virtual ~CCombatAbilityData() {}
    virtual long GetDataType() const { return DataType; }
    long DataType;
    long InterfaceType;
    long WeaponClass;
};
template <class T> struct CCountedPointer {
    T*    Object;
    long* Count;
    T* operator->() const { return Object; }
};
struct CCombatAbility {
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
class CTCCombat {
public:
    long ComboChainPos, ComboChainID, ComboChainGracePeriodCountdown, HandednessGracePeriodCountdown, UnarmedCombatIdleCountdown;
    long BlockCounterTimer;
    bool PerformingAction, PerformingStrikeAction, StartingToBlock, Blocking;
    long BlockHoldTimer, BlockingNumFrames;
    CCombatAbilityMap CombatAbilities;
    bool ReadyForMelee;
    void ClearCombatVariables();
};
void CTCCombat::ClearCombatVariables()
{
    StartingToBlock = false;
    PerformingAction = false;
    PerformingStrikeAction = false;
    ComboChainPos = -777;
    ComboChainID = -999;
    Blocking = false;
    ComboChainGracePeriodCountdown = 0;
    HandednessGracePeriodCountdown = 0;
    UnarmedCombatIdleCountdown = 0;
    for (CCombatAbilityMap::iterator it = CombatAbilities.begin(); it != CombatAbilities.end(); ++it) {
        it->second.second.Reset();
    }
    BlockHoldTimer = -1;
    BlockCounterTimer = -1;
    BlockingNumFrames = 0;
    ReadyForMelee = true;
}
int main()
{
    int fail = 0;
    _Rb_tree_node_base header; _Rb_tree_node nodes[5];
    CTCCombat c;
    TestInitMap(c.CombatAbilities, &header);
    long keys[5] = { 3, 1, 4, 1, 0 };
    for (int i = 0; i < 5; ++i) { TestInitNode(&nodes[i], keys[i], 0); TestInsert(c.CombatAbilities, &nodes[i]); }
    c.ComboChainPos = 3; c.ComboChainID = 4; c.ComboChainGracePeriodCountdown = 9; c.HandednessGracePeriodCountdown = 9;
    c.UnarmedCombatIdleCountdown = 9; c.BlockCounterTimer = 7; c.PerformingAction = c.PerformingStrikeAction = true;
    c.StartingToBlock = c.Blocking = true; c.BlockHoldTimer = 8; c.BlockingNumFrames = 8; c.ReadyForMelee = false;
    c.ClearCombatVariables();
    if (c.ComboChainPos != -777 || c.ComboChainID != -999) fail |= 1;
    if (c.ComboChainGracePeriodCountdown || c.HandednessGracePeriodCountdown || c.UnarmedCombatIdleCountdown) fail |= 2;
    if (c.PerformingAction || c.PerformingStrikeAction || c.StartingToBlock || c.Blocking) fail |= 4;
    if (c.BlockHoldTimer != -1 || c.BlockCounterTimer != -1 || c.BlockingNumFrames != 0 || !c.ReadyForMelee) fail |= 8;
    for (int i = 0; i < 5; ++i)
        if (nodes[i]._M_value_field.second.second.RepeatTimer != -1 || nodes[i]._M_value_field.second.second.ActiveTimer != -1) fail |= 16;
    // empty map: only the scalar state
    CTCCombat e; _Rb_tree_node_base h2; TestInitMap(e.CombatAbilities, &h2); e.ReadyForMelee = false;
    e.ClearCombatVariables();
    if (!e.ReadyForMelee || e.ComboChainID != -999) fail |= 32;
    if (fail) { printf("007334f0_TEST FAIL %d\n", fail); return 1; }
    printf("007334f0_TEST PASS\n");
    return 0;
}