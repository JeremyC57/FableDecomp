#ifndef FABLE_ENGINE_COMBATDIALOGUEMANAGER_H
#define FABLE_ENGINE_COMBATDIALOGUEMANAGER_H
// Original PDB type and retail layout evidence: Codex pass 11 row 0x007355F0 NAMING_EVIDENCE.md.
#include "stlport/fable_map.h"
#include "engine/CDefPointer.h"
#include "engine/CIntelligentPointer.h"
class CCombatDialogueDef;class CThingCreatureBase;
class CombatDialogueManager {
public:class TSpeechGroupInfo {public:long NextAllowedFrame;};
private:
 std::map<unsigned long,TSpeechGroupInfo> SpeechGroupInfoMap;CDefPointer<const CCombatDialogueDef> PDialogueDef;
 CIntelligentPointer<CThingCreatureBase> PCurrentTalker;unsigned long CurrentTalkerTextGroupIndex;long LastSpeechFrame;long LastBattleCryFrame;
#ifdef FABLE_RETAIL_COMBATDIALOGUEMANAGER
 // Retail opt-in (chunk 755, row 0x008AF500): Ego_r.pdb ?AttemptToSpeak@CombatDialogueManager@@AAEXXZ (private; declaration only).
 void AttemptToSpeak();
#endif
};
FABLE_STATIC_ASSERT(sizeof(CombatDialogueManager)==0x24);
#endif
