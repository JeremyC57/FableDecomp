// CThingAICreature::InitialiseFromDef() -- copy the movement speeds and the attack immunity from the creature def, set the speeds up, then give the creature its brain: the overriding brain name when set (forced to BRAIN_STAND_AROUND_LIKE_A_MORON by the debug switch), else the def's BrainDef
// Leaf: ?InitialiseFromDef@CThingAICreature@@IAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x008338D0, 313 bytes (+7 int3), `ret`. Ego_r.pdb / FableWin.pdb ?InitialiseFromDef@CThingAICreature@@IAEXXZ
//   (protected).
// Callees (PDB spellings): CThingAICreature::InitialiseMovementSpeeds 0x00830DD0, CCharString::operator=(const char*) 0x0099EFE0
//   ("BRAIN_STAND_AROUND_LIKE_A_MORON" @ 0x01272AC0), CThing::PeekDefinitionManager 0x004C7990, CDefinitionManager::GetDef<CBrainDef>
//   (long, CDefPointer<const CBrainDef>*) 0x0079BD80 and (const CCharString&, CDefPointer<const CBrainDef>*) 0x006D3E80,
//   CThingAICreature::SetNewBrain(CDefPointer<const CBrainDef>) 0x00833010 (by value), the def's virtual slot 1 at a zero count.
// Globals: the debug "every creature stands around" switch byte 0x013B86EA (no PDB data name on disk; the established spelling).
// Layout: CThingCreatureBase PDef +0xE0; CThingCreatureDef MaxWalkingSpeed +0x90 .. MaxFlyingSpeed +0xA0, BrainDef +0xE8, ImmuneToAttack
//   bit 3 of +0x148 (FABLE_RETAIL_CTHINGCREATUREDEF_MEMBERS); CThingAICreature OverridingBrainName +0x190, the speeds +0x1B8..+0x1C4
//   (FABLE_RETAIL_CTHINGAICREATURE_INITFROMDEF), ImmuneToAttack bit 6 of +0x1C8.
// Name fix (agentNF12 2026-11-11): moved off the local CThing / CThingCreatureBase / CThingAICreature / CThingCreatureDef / CDefPointer /
// CDefinitionManager stand-ins (and same-TU stand-in GetDef bodies) onto the shared headers. 0x004C7990 binds
// CThing::PeekDefinitionManager (was ?GetDefinitionManager@CThingAICreature@@QAEPAVCDefinitionManager@@XZ), 0x0079BD80 / 0x006D3E80 the
// const GetDef<CBrainDef> templates, 0x00833010 SetNewBrain(CDefPointer<const CBrainDef>); the row's own symbol is the PDB protected
// IAEXXZ (was QAEXXZ).
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER
#define FABLE_RETAIL_CDEFINITIONMANAGER_GETDEF_GLOBAL_BODY
#define FABLE_RETAIL_CDEFINITIONMANAGER_GETDEF_NAME_BODY
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_MOREACCESS
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGCREATUREDEF_MEMBERS
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CTHINGAICREATURE_HOMEBUILDING
#define FABLE_RETAIL_CTHINGAICREATURE_SAVE
#define FABLE_RETAIL_CTHINGAICREATURE_LEVELSCRIPT
#define FABLE_RETAIL_CTHINGAICREATURE_SETNEWBRAIN
#define FABLE_RETAIL_CTHINGAICREATURE_INITFROMDEF
#include "fable_string.h"
#include "engine/CDefClassBase.h"
#include "engine/CThing.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CThingCreatureDef.h"
#include "engine/CThingAICreature.h"
#include "engine/CDefPointer.h"
#include "engine/CDefinitionManager.h"
#include "engine/CGameDefinitionManager.h"

extern bool g_bForceStandAroundBrain;                          // 0x013B86EA

void CThingAICreature::InitialiseFromDef()
{
    MaxWalkingSpeed = PDef->MaxWalkingSpeed;
    MaxJoggingSpeed = PDef->MaxJoggingSpeed;
    MaxRunningSpeed = PDef->MaxRunningSpeed;
    MaxFlyingSpeed = PDef->MaxFlyingSpeed;
    ImmuneToAttack = PDef->ImmuneToAttack;
    InitialiseMovementSpeeds();
    if (g_bForceStandAroundBrain)
        OverridingBrainName = "BRAIN_STAND_AROUND_LIKE_A_MORON";
    if (OverridingBrainName.IsEmpty()) {
        if (PDef->BrainDef) {
            CDefPointer<const CBrainDef> brainDef;
            if (PeekDefinitionManager().GetDef(PeekDefinition().BrainDef, &brainDef))
                SetNewBrain(brainDef);
        }
    } else {
        CDefPointer<const CBrainDef> brainDef;
        PeekDefinitionManager().GetDef(OverridingBrainName, &brainDef);
        SetNewBrain(brainDef);
    }
}
