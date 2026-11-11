// CThingCreatureBase::ConstructFromParams(long defIndex, const C3DVector& position, long playerNumber, CThingPhysical* creator, CThingPhysical* originalCreator, const CCharString& scriptName, bool flag) -- construct the game object from creation parameters, then fetch the creature definition into PDef
// Leaf: ?ConstructFromParams@CThingCreatureBase@@IAE_NJABVC3DVector@@JPAVCThingPhysical@@1ABVCCharString@@_N@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00662880, length 92 bytes (+ int3 padding).
// Identity: Ego_r.pdb / FableWin.pdb ?ConstructFromParams@CThingCreatureBase@@IAE_NJABVC3DVector@@JPAVCThingPhysical@@1ABVCCharString@@_N@Z
//   (protected); Ego_r donor order ConstructFromParams / SerialiseGameState / ConstructFromLevelScript = retail 0x00662880 / 0x006628E0 /
//   0x00662930.
// Callees: CThingGameObject::ConstructFromParams 0x008388D0 (eight arguments: the derived arguments re-ordered, with a literal 0 for
//   its fourth), CThing::PeekDefinitionManager 0x004C7990, CDefinitionManager::GetDef<CThingCreatureDef> 0x00513160.
// Layout: CThing::DefGlobalIndex +0x8C; CThingCreatureBase::PDef (CDefPointer<const CThingCreatureDef>) +0xE0.
// Shape: the success path is the `if` body (`return false` is the cold tail); the GetDef arguments are pushed before the accessor runs.
// Name fix (agentNF9 2026-11-11): moved off the local CThingGameObject / CThingCreatureBase / CDefinitionManager models onto the shared
//   headers; 0x004C7990 binds ?PeekDefinitionManager@CThing@@QBEABVCGameDefinitionManager@@XZ (was
//   ?GetDefinitionManager@CThingGameObject@@QBEPAVCDefinitionManager@@XZ), 0x00513160
//   ??$GetDef@VCThingCreatureDef@@@CDefinitionManager@@QBE_NJPAV?$CDefPointer@$$CBVCThingCreatureDef@@@@@Z (was
//   ?GetDef@CDefinitionManager@@QAE_NJPAUCDefPointerCreature@@@Z); the row's own symbol is the protected PDB spelling (was QAE).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_MOREACCESS
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFPOINTER_RELEASE_SELFDELETING_POINTEE
#define FABLE_RETAIL_CIVCOUNTEDPOINTEEBASE_RELEASE_DELETES
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CDEFINITIONMANAGER_GETDEF_GLOBAL_BODY
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CRESOURCE
#define FABLE_RETAIL_CTHINGCREATUREBASE_CONSTRUCT
#define FABLE_RETAIL_CTHINGGAMEOBJECT_LEVELSCRIPT
#include "fable_string.h"
#include "engine/CThing.h"
#include "engine/CThingBaseDef.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CGameDefinitionManager.h"

bool CThingCreatureBase::ConstructFromParams(long defIndex, const C3DVector& position, long playerNumber, CThingPhysical* creator,
                                             CThingPhysical* originalCreator, const CCharString& scriptName, bool flag)
{
    if (CThingGameObject::ConstructFromParams(defIndex, position, playerNumber, 0, scriptName, creator, originalCreator, flag)) {
        PeekDefinitionManager().GetDef(DefGlobalIndex, &PDef);
        return true;
    }
    return false;
}
