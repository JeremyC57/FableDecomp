// CTCDegradableBase::OnCreate() -- clear the last hit frame, fetch the parent thing's definition, take its degradable sub-def, clear the current stage and remember the parent as the physical thing
// Leaf: ?OnCreate@CTCDegradableBase@@MAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008EC870, 98 bytes (plain `ret`; vtable 0x0126A6A4 slot 4, Ego_r.pdb
// `?OnCreate@CTCDegradableBase@@MAEXXZ`; the catalog "CTCDegradableBarrier::vfunc_4" is the slot name).
// Callees: CTCBase::GetDefinitionManager() const @ 0x006869D0, CDefinitionManager::GetDef<CThingBaseDef> @ 0x0042AF3C,
// CDefClassBase::GetSubDef<CDegradableDef> @ 0x008ECDA0 (FableWin.pdb ??$GetSubDef@VCDegradableDef@@@..); the thing def's release.
// Layout: engine/CTCDegradableBase.h (Ego_r.pdb, 0x1c): LastWorldFrameHit +0xc, PDefinition +0x10, PCurrentDegradableInfo +0x14,
// PParentThingPhysical +0x18; CThing::DefGlobalIndex (ushort +0x8c).
// Name fix (agentNF10 2026-11-11): moved off the local CThing / CDefPointer / CDefClassBase / CDefinitionManager / CTCBase /
// CTCDegradableBase stand-ins onto the shared headers (new engine/CTCDegradableBase.h). 0x0042AF3C binds GetDef<CThingBaseDef> (was
// ?GetDef@CDefinitionManager@@QAE_NJPAV?$CDefPointer@VCDefClassBase@@@@@Z), 0x006869D0 the PDB const GetDefinitionManager, 0x008ECDA0
// ??$GetSubDef@VCDegradableDef@@@CDefClassBase@@QBE_NPAV?$CDefPointer@$$CBVCDegradableDef@@@@@Z (was a non-template spelling); the
// row's own symbol is the PDB protected virtual MAEXXZ.
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
#define FABLE_RETAIL_CTHINGPHYSICAL
#include "fable_string.h"
#include "engine/CThing.h"
#include "engine/CThingPhysical.h"
#include "engine/CThingBaseDef.h"
#include "engine/CDegradableDef.h"
#include "engine/CGameDefinitionManager.h"
#include "engine/CTCDegradableBase.h"

void CTCDegradableBase::OnCreate()
{
    LastWorldFrameHit = 0;
    CDefPointer<const CThingBaseDef> def;
    GetDefinitionManager().GetDef(ParentThing->GetDefGlobalIndex(), &def);
    def->GetSubDef(&PDefinition);
    PCurrentDegradableInfo = 0;
    PParentThingPhysical = static_cast<CThingPhysical*>(ParentThing);
}
