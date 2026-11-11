// CTCActionUseBase::OnCreate() -- run the base hook, look the parent thing's def up by its global index and fetch its action-use sub-def into PActionUseDef
// Leaf: ?OnCreate@CTCActionUseBase@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007EAC10, 81 bytes (plain `ret`); CTCBase slot 4 override (Ego_r.pdb / FableWin.pdb
// ?OnCreate@CTCActionUseBase@@UAEXXZ). The same body as CTCCameraCollision's slot 4 (0x007E3040).
// Callees: CTCBase::OnCreate @ 0x00686850 (called non-virtually), CTCBase::GetDefinitionManager() const @ 0x006869D0,
// CDefinitionManager::GetDef<CThingBaseDef> @ 0x0042AF3C, CDefClassBase::GetSubDef<CActionUseDef> @ 0x007EAE40 (FableWin.pdb
// ??$GetSubDef@VCActionUseDef@@@..), the thing def's release (slot 1).
// Shape: the parent read into a named local first (retail's eax / edx roles for the def index, unlike 0x007E3040).
// Layout: CThing::DefGlobalIndex (ushort +0x8c); PActionUseDef (CDefPointer<const CActionUseDef>) +0x0c (engine/CTCActionUseBase.h).
// Name fix (agentNF10 2026-11-11): moved off the local CTCBase_Calls / CDefinitionManager_Retail / CActionUseDef_Retail / CThingDef
// stand-ins onto the shared headers (engine/CTCActionUseBase.h FABLE_RETAIL_CTCACTIONUSEBASE_ONCREATE). 0x0042AF3C binds
// GetDef<CThingBaseDef> (was ?GetDef@CDefinitionManager_Retail@@QAE_NJPAV?$CDefPointer@UCThingDef@@@@@Z), 0x006869D0 the PDB const
// ?GetDefinitionManager@CTCBase@@IBEABVCGameDefinitionManager@@XZ (was CTCBase_Calls), 0x007EAE40
// ??$GetSubDef@VCActionUseDef@@@CDefClassBase@@QBE_NPAV?$CDefPointer@$$CBVCActionUseDef@@@@@Z, 0x00686850 ?OnCreate@CTCBase@@UAEXXZ (was
// CTCBase_Calls::OnCreate); the row's own symbol is the PDB virtual UAEXXZ.
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CDEFINITIONMANAGER_GETDEF_GLOBAL_BODY
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CRESOURCE
#define FABLE_RETAIL_CTCACTIONUSEBASE_ONCREATE
#include "fable_string.h"
#include "engine/CThing.h"
#include "engine/CThingBaseDef.h"
#include "engine/CGameDefinitionManager.h"
#include "engine/CTCActionUseBase.h"

void CTCActionUseBase::OnCreate()
{
    CTCBase::OnCreate();
    CThing* thing = ParentThing;
    CDefPointer<const CThingBaseDef> def;
    GetDefinitionManager().GetDef(thing->GetDefGlobalIndex(), &def);
    def->GetSubDef(&PActionUseDef);
}
