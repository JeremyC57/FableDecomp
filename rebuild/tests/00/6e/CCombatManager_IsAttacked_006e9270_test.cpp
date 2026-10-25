// Test for 0x006e9270 CCombatManager::IsAttacked(const CThingCreatureBase&) const: links the source object; the combatant
// map's find (stubbed) is searched by the creature's 64-bit UniqueID; unknown creatures answer false, known ones
// answer from their combatant info (AttackerInfo.Attacked for IsAttacker, the DefenderInfo.Attackers count for
// IsAttacked).
#include <stdio.h>
#include <string.h>
class CThingCreatureBase {
public:
    unsigned char _pad[0x18];
    unsigned __int64 UniqueID;
};
struct CRetailRbMap_UID_Creature { void* header; unsigned long count; unsigned long comp; };
struct CCombatantInfo {
    CThingCreatureBase* Parent;
    CThingCreatureBase* Attacked;
    unsigned char _attackerInfoRest[0x2c];
    CRetailRbMap_UID_Creature Attackers;
};
struct CRetailRbMap_UID_CombatantInfo {
    struct node {
        long  colour; node* parent; node* left; node* right;
        unsigned __int64 key;
        CCombatantInfo value;
    };
    node* header;
    unsigned long count;
    unsigned long comp;
    node* find(const unsigned __int64& key) const;
};
static CRetailRbMap_UID_CombatantInfo::node g_head, g_nodes[2];
static int g_finds;
CRetailRbMap_UID_CombatantInfo::node* CRetailRbMap_UID_CombatantInfo::find(const unsigned __int64& key) const
{
    ++g_finds;
    for (int i = 0; i < 2; ++i) if (g_nodes[i].key == key) return &g_nodes[i];
    return header;
}
class CCombatManager {
public:
    virtual void v00();
    unsigned char _sequences[0xc];
    CRetailRbMap_UID_CombatantInfo Combatants;
    bool IsAttacked(const CThingCreatureBase& creature) const;
};
void CCombatManager::v00() { }
int main()
{
    CThingCreatureBase c[3];
    memset(c, 0, sizeof(c));
    c[0].UniqueID = (((unsigned __int64)1) << 32) | 1;
    c[1].UniqueID = (((unsigned __int64)2) << 32) | 2;
    c[2].UniqueID = (((unsigned __int64)3) << 32) | 3;
    memset(g_nodes, 0, sizeof(g_nodes));
    g_nodes[0].key = c[0].UniqueID; g_nodes[1].key = c[1].UniqueID;
    if (2 == 1) { g_nodes[0].value.Attacked = &c[1]; g_nodes[1].value.Attacked = 0; }
    else { g_nodes[0].value.Attackers.count = 2; g_nodes[1].value.Attackers.count = 0;
           g_nodes[0].value.Attacked = 0; g_nodes[1].value.Attacked = &c[0]; }
    CCombatManager m;
    m.Combatants.header = &g_head; m.Combatants.count = 2; m.Combatants.comp = 0;
    const CCombatManager& cm = m;
    bool ok = cm.IsAttacked(c[0]) && !cm.IsAttacked(c[1]) && !cm.IsAttacked(c[2]) && g_finds == 3;
    if (!ok) { puts("006e9270_TEST FAIL"); return 1; }
    puts("006e9270_TEST PASS");
    return 0;
}
