// Behaviour test for 0x007340B0 CTCCombat::OnKnockDown().
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
class CTCSpecialAbilities {
public:
    int cancelled;
    void CancelCurrentMutexAbility() { ++cancelled; }
};
class CThingCreatureBase : public CThing {
public:
    int reorienter;
    void SetBodyReorienterActive(bool active) { reorienter = active ? 1 : 2; }
};
class CTCCombat {
public:
    CThingCreatureBase* ParentCreature;
    CCombatAbilityMap CombatAbilities;
    void OnKnockDown();
};
void CTCCombat::OnKnockDown()
{
    CCombatAbilityMap::iterator last = CombatAbilities.upper_bound(1);
    for (CCombatAbilityMap::iterator it = CombatAbilities.lower_bound(1); it != last; ++it)
        it->second.second.Reset();
    ParentCreature->SetBodyReorienterActive(true);
    CTCSpecialAbilities* abilities;
    if (ParentCreature->GetInterface(&abilities, 0xA7))
        abilities->CancelCurrentMutexAbility();
}
int main()
{
    int fail = 0;
    _Rb_tree_node_base header; _Rb_tree_node nodes[4];
    CTCCombat c; TestInitMap(c.CombatAbilities, &header);
    long keys[4] = { 2, 1, 1, 0 };
    for (int i = 0; i < 4; ++i) { TestInitNode(&nodes[i], keys[i], 0); TestInsert(c.CombatAbilities, &nodes[i]); }
    CTCSpecialAbilities sa; sa.cancelled = 0;
    CInterfaceMap::SEntry entries[2]; entries[0].type = 0x30; entries[0].component = 0;
    entries[1].type = 0xA7; entries[1].component = (CTCBase*)&sa;
    CThingCreatureBase creature; creature.TestSet(entries, 2); creature.reorienter = 0;
    c.ParentCreature = &creature;
    c.OnKnockDown();
    if (creature.reorienter != 1 || sa.cancelled != 1) fail |= 1;
    for (int i = 0; i < 4; ++i) {
        const CCombatAbility& a = nodes[i]._M_value_field.second.second;
        bool reset = a.RepeatTimer == -1 && a.ActiveTimer == -1;
        if (keys[i] == 1 ? !reset : reset) fail |= 2;
    }
    creature.TestSet(entries, 1);                 // no special abilities: nothing cancelled
    c.OnKnockDown();
    if (sa.cancelled != 1) fail |= 4;
    if (fail) { printf("007340b0_TEST FAIL %d\n", fail); return 1; }
    printf("007340b0_TEST PASS\n");
    return 0;
}