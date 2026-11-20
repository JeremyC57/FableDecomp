// CCombatManager::IsAttackerOn(const CThingCreatureBase& attacker, const CThingCreatureBase& defender) const -- looks the attacker's 64-bit UniqueID up in the combatant map and reports whether that entry is attacking the given defender (AttackerInfo.Attacked)
// Leaf: ?IsAttackerOn@CCombatManager@@QBE_NABVCThingCreatureBase@@0@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006E9310, length 64 bytes.
// Identity: Ego_r.pdb (0x00712570) + FableWin.pdb (0x02412B50) ?IsAttackerOn@CCombatManager@@QBE_NABVCThingCreatureBase@@0@Z
//   (public const, two CThingCreatureBase const references). Ego_r order IsAttacked 0x00712490 / IsAttacker 0x00712500 /
//   IsAttackerOn 0x00712570 = retail 0x006E9270 / 0x006E92C0 (both landed under those names) / 0x006E9310; the body is the
//   IsAttacker twin with the second argument compared to the entry's AttackerInfo.Attacked (node +0x1c). Was the stand-in
//   CCombatManager_Retail::IsCreatureFightingThing_006e9310 (NF204 NAME_FIXES; renamed by NF207, 2026-11-20, q12300).
// Callees: the combatant map's _M_find<unsigned __int64> @ 0x006F0250 (STLport red-black tree lookup comparing the high
//   dword first; out of line, declared only: FABLE_RETAIL_STLP_TREE_MFIND_DECL, as the stand-in find was).
// The manifest's "CCodeSectionManager::IsSectionRegisteredLongName" label is propagated: the only caller is
// CAIStateGroup_SummonerCharge::IsRunnable (0x0096FC0A). Retail CCombatManager (Ego_r minus 4 per map):
// RegisteredCombatSequences +4, Combatants (map<unsigned __int64, CCombatantInfo>) +0x10. The creature's UniqueID is
// CThing +0x18 (PDB); the STLport node is {colour, parent, left, right} + the pair at +0x10, so the id is at +0x10 and the
// info's AttackerInfo.Attacked at +0x1c. The id is copied into a local (the by-value GetUniqueID() result) before its
// address is passed, which is why both dwords are stored to the frame first. Byte result with explicit true/false
// returns; the defender is the left operand of the pointer compare (it is loaded into a register, the node field stays
// in memory).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CCOMBATMANAGER_ISATTACKERON
#define FABLE_RETAIL_STLP_TREE_MFIND_DECL
#define FABLE_RETAIL_STLP_TREE_CONSTFIND
#include "stlport/fable_map.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CCombatManager.h"

bool CCombatManager::IsAttackerOn(const CThingCreatureBase& attacker, const CThingCreatureBase& defender) const
{
    std::map<unsigned __int64, CCombatantInfo>::const_iterator it = Combatants.find(attacker.GetUniqueID());
    if (it != Combatants.end() && &defender == it->second.AttackerInfo.Attacked)
        return true;
    return false;
}
