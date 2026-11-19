// CCombatManager::SetupDefenderPlacementWheel(CCombatantInfo& combat_info, bool force_wheel_update) -- when forced: stamp the world frame and the defender's position, collect the defender's attackers that use the combat wheel into a local creature -> attacker-info map and rebuild the wheel from it; always update the wheel
// Leaf: ?SetupDefenderPlacementWheel@CCombatManager@@AAEXAAUCCombatantInfo@@_N@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x006ECEA0, 383 bytes, thiscall `ret 8`, 8-aligned ebp frame (the 64-bit key temporary).
// Name: Ego_r.pdb (0x007167A0, the same 8-aligned frame) + FableWin.pdb (0x0241AD40, combat_manager.obj); the catalog row
//   "CDefendingCombatantInfo::UpdateCombatState" was descriptive.
// Source: FableWin.pdb debug body 0x0241AD40 with Ego_d.pdb's locals (combat_info, force_wheel_update; defender_info; block:
//   attackers, attackers_map; loop: it; body: att_comb_inf, creature). Callees: CWorld::GetFrame 0x0049D870, CThing::GetPos
//   0x004C73D0, the local map's insert_unique 0x006F2A40 and _M_erase 0x006EFCE0 (STLport, malloc / free for its header),
//   CWorld::GetWorldMap (virtual slot 12), CCombatWheel::Init 0x006EC850, CCombatWheel::Update 0x006E8560.
#include "fable_compiler.h"
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CDEFENDINGCOMBATANTINFO_PASS22_INIT
#define FABLE_RETAIL_CCOMBATWHEEL_UPDATE
#define FABLE_RETAIL_CCOMBATMANAGER
#define FABLE_RETAIL_CCOMBATMANAGER_SETUPPLACEMENTWHEEL
#include "stlport/fable_containers.h"
#include "engine/C3DVector.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CWorld.h"
#include "engine/CCombatManager.h"

void CCombatManager::SetupDefenderPlacementWheel(CCombatantInfo& combat_info, bool force_wheel_update)
{
    CDefendingCombatantInfo& defender_info = combat_info.DefenderInfo;
    if (force_wheel_update) {
        defender_info.LastWheelUpdateWorldFrame = GetWorld().GetFrame();
        defender_info.LastVictimWheelUpdatePos = combat_info.Parent->GetPos();
        const std::map<unsigned __int64, CThingCreatureBase*>& attackers = defender_info.Attackers;
        std::map<CThingCreatureBase*, CAttackingCombatantInfo*> attackers_map;
        for (std::map<unsigned __int64, CThingCreatureBase*>::const_iterator it = attackers.begin(); it != attackers.end(); ++it) {
            CThingCreatureBase* creature = (*it).second;
            CCombatantInfo& att_comb_inf = GetCombatantInfo(*creature);
            if (att_comb_inf.AttackerInfo.UsingCombatWheel)
                attackers_map.insert(std::make_pair(creature, &att_comb_inf.AttackerInfo));
        }
        defender_info.CombatWheel.Init(*this, GetWorld().GetWorldMap(), attackers_map, *combat_info.Parent);
    }
    defender_info.CombatWheel.Update();
}