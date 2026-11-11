// CTCCameraCollision::OnCreate() -- vtable slot 4: run the base slot, look the parent thing's def up by its global index and fetch its camera-collision sub-def into CameraCollisionDef
// Leaf: ?OnCreate@CTCCameraCollision@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007E3040, 81 bytes (plain `ret`); CTCBase slot 4 override (Ego_r.pdb / FableWin.pdb
// ?OnCreate@CTCCameraCollision@@UAEXXZ).
// Callees: CTCBase::OnCreate @ 0x00686850 (called non-virtually), CTCBase::GetDefinitionManager() const @ 0x006869D0,
// CDefinitionManager::GetDef<CThingBaseDef> @ 0x0042AF3C, CDefClassBase::GetSubDef<CCameraCollisionDef> @ 0x007E3290 (FableWin.pdb
// ??$GetSubDef@VCCameraCollisionDef@@@..), the thing def's release (slot 1).
// Layout: CThing::DefGlobalIndex (ushort +0x8c); CameraCollisionDef (CDefPointer<const CCameraCollisionDef>) +0x0c
// (engine/CTCCameraCollision.h FABLE_RETAIL_CTCCAMERACOLLISION_ONCREATE).
// Name fix (agentNF10 2026-11-11): moved off the local CTCBase / CThing / CDefPointer / CDefClassBase / CDefinitionManager / CThingDef
// stand-ins onto the shared headers. The thing def handle is CDefPointer<const CThingBaseDef>, so 0x0042AF3C binds GetDef<CThingBaseDef>
// (was ?GetDef@CDefinitionManager@@QAE_NJPAV?$CDefPointer@UCThingDef@@@@@Z-style stand-ins), 0x006869D0 the PDB const
// GetDefinitionManager, 0x007E3290 ??$GetSubDef@VCCameraCollisionDef@@@CDefClassBase@@QBE_NPAV?$CDefPointer@$$CBVCCameraCollisionDef@@@@@Z,
// 0x00686850 ?OnCreate@CTCBase@@UAEXXZ; the row's own symbol is the PDB virtual UAEXXZ.
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
#define FABLE_RETAIL_CTCCAMERACOLLISION_ONCREATE
#include "fable_string.h"
#include "engine/CThing.h"
#include "engine/CThingBaseDef.h"
#include "engine/CGameDefinitionManager.h"
#include "engine/CTCCameraCollision.h"

void CTCCameraCollision::OnCreate()
{
    CTCBase::OnCreate();
    CDefPointer<const CThingBaseDef> def;
    GetDefinitionManager().GetDef(ParentThing->GetDefGlobalIndex(), &def);
    def->GetSubDef(&CameraCollisionDef);
}
