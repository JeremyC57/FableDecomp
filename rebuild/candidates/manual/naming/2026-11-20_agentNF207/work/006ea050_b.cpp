// CCombatManager::IsDefenderDueForAttack(const CThingCreatureBase& defender) const -- false unless the defender is a registered combatant that currently has attackers; then the verdict of the CCombatantInfo overload on its combatant info
// Leaf: ?IsDefenderDueForAttack@CCombatManager@@QBE_NABVCThingCreatureBase@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x006EA050, 106 bytes (112 with padding).
// Callees: the combatant map's _M_find<unsigned __int64> @ 0x006F0250 (declared only, twice),
//          CCombatManager::IsDefenderDueForAttack(const CCombatantInfo&) const @ 0x006E97F0.
// Shape: the id is read ONCE into a register pair (ebx:ebp) and re-stored into the frame slot before each find; the second
//   lookup is unchecked (`add eax,18h`).
// Inferred: the id local is a signed __int64, so each find(const unsigned __int64&) binds a converted temporary: retail keeps
//   the id in ebx:ebp (0x006EA058 / 0x006EA05C) and re-stores it before each call (0x006EA06D / 0x006EA08C). An unsigned local
//   (with or without a same-type cast at the call) is passed by address and lives in the frame: DIFFER(92v106).
// DRAFT (NF207): identity pending the coordinator (decision item); not handed back.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CCOMBATMANAGER_ISDEFENDERDUEFORATTACK
#define FABLE_RETAIL_STLP_TREE_MFIND_DECL
#define FABLE_RETAIL_STLP_TREE_CONSTFIND
#include "stlport/fable_map.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CCombatManager.h"

bool CCombatManager::IsDefenderDueForAttack(const CThingCreatureBase& defender) const
{
    __int64 id = defender.GetUniqueID();
    std::map<unsigned __int64, CCombatantInfo>::const_iterator it = Combatants.find(id);
    if (it != Combatants.end() && it->second.DefenderInfo.Attackers.size() != 0)
        return IsDefenderDueForAttack(Combatants.find(id)->second);
    return false;
}
