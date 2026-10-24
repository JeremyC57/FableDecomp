// Behaviour test for 0x00733F70 CTCCombat::SetBlockBroken(CThing&).
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
class CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type) { SEntry* it = first; while (it != last && it->type < *type) ++it; return it; }
};
class CThing {
public:
    unsigned int  m_presentBits[9];
    CInterfaceMap m_interfaces;
    template <class T> bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1U << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            *ppOut = (T*)(it == map->last || interfaceType < it->type ? map->last : it)->component;
            return true;
        }
        return false;
    }
    void TestSet(CInterfaceMap::SEntry* entries, int n)
    {
        for (int i = 0; i < 9; ++i) m_presentBits[i] = 0;
        for (int i = 0; i < n; ++i) m_presentBits[entries[i].type >> 5] |= 1U << (entries[i].type & 31);
        m_interfaces.first = entries; m_interfaces.last = entries + n;
    }
};
class CTCCombat {
public:
    long BlockCounterTimer;
    bool StartingToBlock, Blocking;
    CCombatAbilityMap CombatAbilities;
    int unsetCalls;
    void UnsetBlocking() { ++unsetCalls; StartingToBlock = false; Blocking = false; }
    void SetBlockBroken(CThing& attacker);
};
void CTCCombat::SetBlockBroken(CThing& attacker)
{
    CTCCombat* attackerCombat;
    if (attacker.GetInterface(&attackerCombat, 0x30))
        attackerCombat->BlockCounterTimer = -1;
    UnsetBlocking();
    CCombatAbilityMap::iterator last = CombatAbilities.upper_bound(1);
    for (CCombatAbilityMap::iterator it = CombatAbilities.lower_bound(1); it != last; ++it)
        it->second.second.Reset();
}
int main()
{
    int fail = 0;
    _Rb_tree_node_base header; _Rb_tree_node nodes[6];
    CTCCombat c; TestInitMap(c.CombatAbilities, &header); c.unsetCalls = 0; c.Blocking = c.StartingToBlock = true;
    long keys[6] = { 1, 0, 2, 1, 3, 1 };
    for (int i = 0; i < 6; ++i) { TestInitNode(&nodes[i], keys[i], 0); TestInsert(c.CombatAbilities, &nodes[i]); }
    CTCCombat attackerCombat; attackerCombat.BlockCounterTimer = 40;
    CInterfaceMap::SEntry entries[2]; entries[0].type = 0x10; entries[0].component = 0;
    entries[1].type = 0x30; entries[1].component = (CTCBase*)&attackerCombat;
    CThing attacker; attacker.TestSet(entries, 2);
    c.SetBlockBroken(attacker);
    if (attackerCombat.BlockCounterTimer != -1) fail |= 1;
    if (c.unsetCalls != 1 || c.Blocking || c.StartingToBlock) fail |= 2;
    for (int i = 0; i < 6; ++i) {
        const CCombatAbility& a = nodes[i]._M_value_field.second.second;
        bool reset = a.RepeatTimer == -1 && a.ActiveTimer == -1;
        bool untouched = a.RepeatTimer == 5 && a.ActiveTimer == 6;
        if (keys[i] == 1 ? !reset : !untouched) fail |= 4;
    }
    // attacker without a combat component: nothing to cancel
    CThing plain; plain.TestSet(entries, 1); attackerCombat.BlockCounterTimer = 40;
    c.SetBlockBroken(plain);
    if (attackerCombat.BlockCounterTimer != 40 || c.unsetCalls != 2) fail |= 8;
    if (fail) { printf("00733f70_TEST FAIL %d\n", fail); return 1; }
    printf("00733f70_TEST PASS\n");
    return 0;
}