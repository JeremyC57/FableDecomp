// CCombatManager::UpdateDefenderAttackPattern(CCombatantInfo& info) -- pick the attack pattern a defender should run against its current
//   attackers: the first attacker whose combat type def maps the defender's ECombatCreatureType (CCombatTypeDef::AttackPatterns) names
//   the pattern's global def index; when one is found and it is not the def the defender already runs (the global index of
//   DefenderInfo.PAttackPattern's instantiation name), fetch that CAttackPatternDef and restart the defender on it.
// Leaf: ?UpdateDefenderAttackPattern@CCombatManager@@AAEXAAUCCombatantInfo@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x006EA0C0, 358 bytes (360 with padding), `ret 4`, no frame pointer, no EH frame.
// Name: Ego_r.pdb (donor 0x00712D10) + FableWin.pdb (0x02419080) ?UpdateDefenderAttackPattern@CCombatManager@@AAEXAAUCCombatantInfo@@@Z;
//   sole caller 0x006ED04D in the landed CCombatManager::UpdateAttackingCombatantsList 0x006ED030. (The manifest label
//   CDefendingCombatantInfo::InitializeDefenderFromCreature was made up: `this` is the CCombatManager, World at +0x20.)
// Callees: CDefClassBase::GetInstantiationName() const (virtual slot 10, CDefString by hidden pointer); CDefStringTable::GetString(long)
//   0x009D49B0 on GDefStringTable 0x013CA828 (the inline CDefString -> CCharString conversion); CDefinitionManager::
//   GetDefGlobalIndexFromName(const CCharString&) const 0x009AD410; ~CCharString 0x0099EAE0; CThingCreatureBase::GetCombatTypeDef() const
//   0x00666D10 (twice); CDefinitionManager::GetDef<CAttackPatternDef>(long, CDefPointer<const CAttackPatternDef>*) const 0x006F0E80
//   (out of line, body visible: FABLE_RETAIL_CDEFINITIONMANAGER_GETDEF_GLOBAL_BODY, so the handle is read once after the call);
//   CCombatManager::ResetDefenderAttackPattern(CCombatantInfo&, CDefPointer<const CAttackPatternDef>) 0x006E8C30 (by-value handle built
//   in its argument slot); the def's Delete (slot 1) on the last reference.
// Inline in retail: CWorld::PeekDefinitionManager (+0x10), the Attackers map's begin/end/operator++ (_Rb_global::_M_increment), the
//   const map<ECombatCreatureType,long>::find with the tree's _M_find (FABLE_RETAIL_STLP_TREE_CONSTFIND), CDefPointer ctor/copy/dtor.
// Shape: `pattern` is zeroed before the virtual call and lives in ebp (spilled at +0x10 because the inlined search borrows ebp for the
//   key). The attacker is a named local of the loop body (`CThingCreatureBase* attacker = it->second`): with it the found node sits in
//   edi and the tree header in edx as in retail; `it->second->GetCombatTypeDef()` written directly swaps the two registers (360v360).
// Layout: CCombatantInfo Parent +0, DefenderInfo +0x34 (Attackers header +0x34, node value +0x18; PAttackPattern +0x64);
//   CCombatTypeDef AttackPatterns +0x84 (node key +0x10, value +0x14), CombatCreatureType +0x90.
// CAttackPatternDef has no shared header (only the non-retail engine/_quarantine one): modelled here as the CParentDefClassBase-derived
//   def it is (first own member +0x3C, the AttackPattern vector read by IncrementDefenderAttackPattern 0x006E8600).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGCREATUREBASE_COMBATTYPE
#define FABLE_RETAIL_JACK_STAGE1_UPDATE
#define FABLE_RETAIL_CCOMBATTYPEDEF
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CWORLD_PEEKDEFINITIONMANAGER
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CDEFINITIONMANAGER_GETDEF_GLOBAL_BODY
#define FABLE_RETAIL_STLP_TREE_CONSTFIND
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER
#define FABLE_RETAIL_CCOMBATMANAGER_DEFENDERATTACKPATTERN
#include <new>
#include "engine/CThingCreatureBase.h"
#include "engine/CCombatTypeDef.h"
#include "engine/CParentDefClassBase.h"
#include "engine/CDefString.h"
#include "engine/CGameDefinitionManager.h"
#include "engine/CWorld.h"
#include "engine/CCombatManager.h"

class CAttackPatternDef : public CParentDefClassBase {
public:
    std::vector<long> AttackPattern;                                     // +0x3c
};

void CCombatManager::UpdateDefenderAttackPattern(CCombatantInfo& info)
{
    long pattern = 0;
    long own_index = World.PeekDefinitionManager().GetDefGlobalIndexFromName(info.DefenderInfo.PAttackPattern->GetInstantiationName());
    ECombatCreatureType creature_type = info.Parent->GetCombatTypeDef().CombatCreatureType;
    for (std::map<unsigned __int64, CThingCreatureBase*>::iterator it = info.DefenderInfo.Attackers.begin();
         it != info.DefenderInfo.Attackers.end(); ++it) {
        CThingCreatureBase* attacker = it->second;
        const CCombatTypeDef& def = attacker->GetCombatTypeDef();
        std::map<ECombatCreatureType, long>::const_iterator found = def.AttackPatterns.find(creature_type);
        if (found != def.AttackPatterns.end() && pattern == 0)
            pattern = found->second;
    }
    if (pattern != 0 && pattern != own_index) {
        CDefPointer<const CAttackPatternDef> p;
        World.PeekDefinitionManager().GetDef(pattern, &p);          // 0x006F0E80
        ResetDefenderAttackPattern(info, p);                         // 0x006E8C30
    }
}