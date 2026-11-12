// CAIStateGroup_SeekScriptTarget::Update() -- finish any action, call out the enemy once (the dialogue def's EnemySighted speech or a battle cry), mark the enemy in danger, unsheathe a melee weapon and run to the script target until reached
// Leaf: ?Update@CAIStateGroup_SeekScriptTarget@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0093C030, 505 bytes, plain `ret` (vtable slot 12; Ego_r.pdb name above).
// Callees: CAIStateGroup_SeekScriptTarget::IsStillRunnable 0x0093BFA0, CThingCreatureBase::IsPerformingAnyAction 0x006622A0 /
//   IsFreeToFinishAction 0x006621C0 / FinishCurrentAction 0x00662650 / GetCombatManager 0x00661ED0, IS_THING_ALIVE (GetPItem 0x00A01B50),
//   CAIStateGroupBase::GetWorld 0x008FCFC0, CWorld::GetWorldSeed 0x0049D810 + GFRandom (expanded; GFROR13 0x00497890),
//   CombatDialogueManager::GetPDialogueDef (inline, EnemySighted +0x48) / SaySpeech 0x008AFC40 / PlayBattleCry 0x008AF850,
//   CThing::GetTC<CTCStealth> (expanded with the inline CVectorMap::LowerBound), CTCStealth::SetAsInDanger 0x00747B80,
//   CCombatManager::CreatureUnsheathWeapon 0x006EBDD0, the CIntelligentPointer<const CThing> conversion (0x00A01B10 / 0x00A01B50 /
//   0x00A01B90), CAIStateGroupBase::AIAction_MoveToThingProximity 0x0097E380, CAIStateGroupBase::Terminate 0x008FCF40.
// Shape: on the shared headers (the earlier local model had this in edi; the port gives retail's esi). The danger status 2 is
//   written as a cast: the NStealth::EDangerStatus enumerators are in neither PDB name set.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CVECTORMAP_INLINE_LOWERBOUND
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGCREATUREBASE_FINISHACTION
#define FABLE_RETAIL_CTHINGCREATUREBASE_ACTIONQUERY
#define FABLE_RETAIL_CTHINGCREATUREBASE_FREETOFINISH
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COMPARE_NULL_INT
#define FABLE_RETAIL_CINTELLIGENTPOINTER_CONVERT_MUTABLE
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CTCSTEALTH_SETSNEAKING
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CWORLD_COMBATMANAGER
#define FABLE_RETAIL_CCOMBATMANAGER_DIALOGUE
#define FABLE_RETAIL_CCOMBATMANAGER_UNSHEATH
#define FABLE_RETAIL_COMBATDIALOGUEMANAGER_SPEECH
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CAISTATEGROUPBASE_GETCREATURE
#define FABLE_RETAIL_CAISTATEGROUPBASE_AIACTIONS
#define FABLE_RETAIL_CAISTATEGROUPBASE_SPEECH
#define FABLE_RETAIL_CAIBRAIN_GETCREATURE
#define FABLE_RETAIL_CAISTATEGROUP_SEEKSCRIPTTARGET
#include "rebuild_abi.h"
#include "engine/fable_ai_action_enums.h"
#include "engine/EUnsheatheWeaponType.h"
#include "engine/GFRandom.h"
#include "engine/CIntelligentPointer.h"
#include "engine/ThingAlive.h"
#include "engine/CThingAICreature.h"
#include "engine/CTCStealth.h"
#include "engine/CCombatDialogueDef.h"
#include "engine/CCombatManager.h"
#include "engine/CWorld.h"
#include "engine/CAIStateGroupBase.h"
#include "engine/CAIBrain.h"
#include "engine/CAIStateGroup_SeekScriptTarget.h"

void CAIStateGroup_SeekScriptTarget::Update()
{
    if (IsStillRunnable()) {
        if (GetCreature().IsPerformingAnyAction() && GetCreature().IsFreeToFinishAction())
            GetCreature().FinishCurrentAction();
        bool reached = false;
        bool called_out = false;
        EAIActionReturn ret;
        do {
            if (!IS_THING_ALIVE(PEnemy)) {
                Terminate();
                return;
            }
            if (!called_out) {
                if (GFRandom(100, GetWorld().GetWorldSeed()) > 50) {
                    unsigned long speech = GetWorld().GetCombatManager().GetCombatDialogueManager().GetPDialogueDef()->EnemySighted;
                    GetWorld().GetCombatManager().GetCombatDialogueManager().SaySpeech(GetCreature(), speech);
                    called_out = true;
                } else {
                    called_out = GetWorld().GetCombatManager().GetCombatDialogueManager().PlayBattleCry(GetCreature());
                }
            }
            CTCStealth* stealth;
            if (PEnemy->GetTC(&stealth))
                stealth->SetAsInDanger(GetCreature(), (NStealth::EDangerStatus)2);
            GetCreature().GetCombatManager().CreatureUnsheathWeapon(GetCreature(), UNSHEATHE_MELEE_WEAPON);
            ret = AIAction_MoveToThingProximity(ACTION_REPEAT_CALLBACK, PEnemy, 1.0f, reached, AI_MOVEMENT_RUNNING, true, false, true, true, 0.0f);
            if (!IsStillRunnable()) {
                Terminate();
                return;
            }
        } while (ret == AI_ACTION_IN_PROGRESS);
        if (ret == AI_ACTION_FINISHED)
            return;
    }
    Terminate();
}