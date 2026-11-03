#ifndef FABLE_ENGINE_CCOMBATMANAGER_H
#define FABLE_ENGINE_CCOMBATMANAGER_H
// Original PDB type and retail layout evidence: Codex pass 11 row 0x007355F0 NAMING_EVIDENCE.md.
#include "engine/CBaseClassNonCopyable.h"
#include "engine/CCombatantInfo.h"
#include "engine/CombatDialogueManager.h"
#include "fable_string.h"
class CWorld;
class CCombatManager:public CBaseClassNonCopyable {
public:
 void RegisterCombatant(CThingCreatureBase&,CThingCreatureBase&,bool);
 void DeregisterCombatant(CThingCreatureBase&);
 const CCombatantInfo& GetCombatantInfo(const CThingCreatureBase&)const;
private:
 std::map<CCharString,CCombatSequenceBase* (__fastcall*)(CThingCreatureBase&,CThingCreatureBase&)> RegisteredCombatSequences;
 std::map<unsigned __int64,CCombatantInfo> Combatants;bool CombatStrafeTurningEnabled;CWorld& World;
 bool DrawDebug;bool TraceDebug;float ProximityForNavigation;bool EnableAINonAttackMode;float AttackPatternIdleStandardInterval;
 bool AttacksFromNearestCombatantOnly;CombatDialogueManager DialogueManager;
};
FABLE_STATIC_ASSERT(sizeof(CCombatManager)==0x5c);
#endif
