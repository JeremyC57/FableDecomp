// CThingObject::ConstructFromParams(long defIndex, const C3DVector& position, long playerNumber, unsigned long flags, const CCharString& scriptName) -- base CThingGameObject construction; on success fetches the object def by index and starts at full health
// Leaf: ?ConstructFromParams@CThingObject@@IAE_NJABVC3DVector@@JKABVCCharString@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00703050, length 103 bytes (`ret 14h`, + int3 padding).
// Identity: Ego_r.pdb / FableWin.pdb ?ConstructFromParams@CThingObject@@IAE_NJABVC3DVector@@JKABVCCharString@@@Z (protected; called by
//   CThingObject::Create 0x00703210).
// Callees: CThingGameObject::ConstructFromParams 0x008388D0 (eight arguments), CThing::PeekDefinitionManager 0x004C7990,
//   CDefinitionManager::GetDef<CThingObjectDef> 0x0042B040.
// Layout: CThing DefGlobalIndex +0x8C; CThingPhysical MaxHealth +0xB0 / Health +0xB4; CThingObject PDef +0xD0 (Ego_r +0xE8);
//   CThingBaseDef::Health +0x50.
// Shape: `MaxHealth = Health = PeekDefinition().Health;` gives `fld ; fst [b4] ; fstp [b0]`; the PDef address is kept in edi across
//   the GetDef call and re-read for the def pointer.
// Name fix (agentNF9 2026-11-11): moved off the local CThingGameObject / CThingObject / CDefinitionManager models onto the shared
//   headers; 0x004C7990 binds ?PeekDefinitionManager@CThing@@QBEABVCGameDefinitionManager@@XZ, 0x0042B040
//   ??$GetDef@VCThingObjectDef@@@CDefinitionManager@@QBE_NJPAV?$CDefPointer@$$CBVCThingObjectDef@@@@@Z (was
//   ?GetDef@CDefinitionManager@@QAE_NJAAUCDefPointerObjectDef@@@Z), 0x008388D0
//   ?ConstructFromParams@CThingGameObject@@IAE_NJABVC3DVector@@JKABVCCharString@@PAVCThingPhysical@@2_N@Z (was ..QAE_NJPBVC3DVector@@..
//   PAUCThingPhysicalArg@@2_N@Z); the row's own symbol is the protected PDB spelling (was ?ConstructFromParams@CThingObject@@QAE_NJPBVC3DVector@@JKABVCCharString@@@Z).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_MOREACCESS
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFPOINTER_RELEASE_SELFDELETING_POINTEE
#define FABLE_RETAIL_CIVCOUNTEDPOINTEEBASE_RELEASE_DELETES
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CDEFINITIONMANAGER_GETDEF_GLOBAL_BODY
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CRESOURCE
#define FABLE_RETAIL_CTHINGOBJECTDEF_VALIDATE
#define FABLE_RETAIL_CTHINGOBJECT_DEFINITION
#define FABLE_RETAIL_CTHINGGAMEOBJECT_LEVELSCRIPT
#define FABLE_RETAIL_CTHINGOBJECT_CONSTRUCTFROMPARAMS
#define FABLE_RETAIL_CTHINGBASEDEF
#include "fable_string.h"
#include "engine/CThing.h"
#include "engine/CThingObject.h"

bool CThingObject::ConstructFromParams(long defIndex, const C3DVector& position, long playerNumber, unsigned long flags,
                                       const CCharString& scriptName)
{
    if (CThingGameObject::ConstructFromParams(defIndex, position, playerNumber, flags, scriptName, 0, 0, true)) {
        PeekDefinitionManager().GetDef(DefGlobalIndex, &PDef);
        MaxHealth = Health = PeekDefinition().Health;
        return true;
    }
    return false;
}
