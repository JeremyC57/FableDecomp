// Behaviour test for 0x00979910 IsInMeleeWithCombatWheelAttackers: the copied logic over a model whose combat manager
// hands out each creature's combatant info and whose attacker map is a small red-black-style tree walked in order.
#include <cstdio>

class CThingCreatureBase;
struct CAttackerNode {
    long _M_color;
    CAttackerNode* _M_parent;
    CAttackerNode* _M_left;
    CAttackerNode* _M_right;
    unsigned long firstLow, firstHigh;          // the 64-bit unique id (unused here)
    CThingCreatureBase* second;
};
struct CAttackerIterator {
    CAttackerNode* _M_node;
    CAttackerIterator(CAttackerNode* node) : _M_node(node) { }
    bool operator!=(const CAttackerIterator& other) const { return _M_node != other._M_node; }
    CAttackerIterator& operator++()             // the STLport in-order successor (0x006EF790)
    {
        if (_M_node->_M_right != 0) {
            _M_node = _M_node->_M_right;
            while (_M_node->_M_left != 0)
                _M_node = _M_node->_M_left;
        } else {
            CAttackerNode* y = _M_node->_M_parent;
            while (_M_node == y->_M_right) {
                _M_node = y;
                y = y->_M_parent;
            }
            if (_M_node->_M_right != y)
                _M_node = y;
        }
        return *this;
    }
    CAttackerNode* operator->() const { return _M_node; }
};
struct CAttackerMap {
    CAttackerNode* _M_header;
    CAttackerIterator begin() const { return CAttackerIterator(_M_header->_M_left); }
    CAttackerIterator end() const { return CAttackerIterator(_M_header); }
};
struct CCombatantInfo {
    CThingCreatureBase* Attacked;
    bool UsingCombatWheel;
    CAttackerMap Attackers;
};
struct CCombatManager {
    const CCombatantInfo& GetCombatantInfo(const CThingCreatureBase& creature) const;
};
class CThingCreatureBase {
public:
    CCombatManager* Manager;
    CCombatantInfo Info;
    bool IsInMeleeAsAttacker() const { return Info.Attacked != 0; }
    bool IsInMeleeAsAttacked() const { return Info.Attackers._M_header->_M_left != Info.Attackers._M_header; }
    const CCombatManager& GetCombatManager() const { return *Manager; }
};
const CCombatantInfo& CCombatManager::GetCombatantInfo(const CThingCreatureBase& creature) const { return creature.Info; }

namespace {
bool IsInMeleeWithCombatWheelAttackers(const CThingCreatureBase& creature)
{
    if (creature.IsInMeleeAsAttacker())
        return true;
    if (creature.IsInMeleeAsAttacked()) {
        const CCombatantInfo& info = creature.GetCombatManager().GetCombatantInfo(creature);
        for (CAttackerIterator it = info.Attackers.begin(); it != info.Attackers.end(); ++it) {
            if (creature.GetCombatManager().GetCombatantInfo(*it->second).UsingCombatWheel)
                return true;
        }
    }
    return false;
}
}

static void Empty(CAttackerNode& header)
{
    header._M_parent = 0; header._M_left = &header; header._M_right = &header;
}
static CAttackerNode g_header[4];
static void Init(CThingCreatureBase& c, CCombatManager* manager, int index)
{
    c.Manager = manager; c.Info.Attacked = 0; c.Info.UsingCombatWheel = false;
    Empty(g_header[index]); c.Info.Attackers._M_header = &g_header[index];
}

int main()
{
    CCombatManager manager;
    CThingCreatureBase victim, attackerA, attackerB, other;
    Init(victim, &manager, 0); Init(attackerA, &manager, 1); Init(attackerB, &manager, 2); Init(other, &manager, 3);
    int fails = 0;

    if (IsInMeleeWithCombatWheelAttackers(victim)) ++fails;            // neither attacking nor attacked
    other.Info.Attacked = &victim;
    if (!IsInMeleeWithCombatWheelAttackers(other)) ++fails;            // an attacker: true at once
    // victim attacked by A (root) and B (its left child): header parent = root, left = leftmost, right = rightmost
    CAttackerNode root, left;
    CAttackerNode& header = g_header[0];
    root._M_parent = &header; root._M_left = &left; root._M_right = 0; root.second = &attackerA;
    left._M_parent = &root; left._M_left = 0; left._M_right = 0; left.second = &attackerB;
    header._M_parent = &root; header._M_left = &left; header._M_right = &root;
    if (IsInMeleeWithCombatWheelAttackers(victim)) ++fails;            // no attacker uses the wheel
    attackerA.Info.UsingCombatWheel = true;
    if (!IsInMeleeWithCombatWheelAttackers(victim)) ++fails;           // the second attacker in order does
    attackerA.Info.UsingCombatWheel = false; attackerB.Info.UsingCombatWheel = true;
    if (!IsInMeleeWithCombatWheelAttackers(victim)) ++fails;           // the first one does
    if (fails) { std::printf("00979910_TEST FAIL\n"); return 1; }
    std::printf("00979910_TEST PASS\n");
    return 0;
}