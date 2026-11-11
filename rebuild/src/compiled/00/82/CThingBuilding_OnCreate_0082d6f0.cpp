// CThingBuilding::OnCreate() -- gives the graphic appearance the def's graphic and the building's scale, clears OverrideAutomaticLocking, runs the game-object creation and, when that succeeds, seeds health and maximum health from the def
// Leaf: ?OnCreate@CThingBuilding@@MAE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0082D6F0, 218 bytes (+6 int3). `ret`, bool in al. CThingBuilding vtable 0x0124509C slot 8 (+0x20).
// Identity: Ego_r.pdb ?OnCreate@CThingBuilding@@MAE_NXZ (protected virtual; CThing::OnCreate 0x004C9560 is slot 8), donor order
//   GetMaterialOnHitByMelee / OnCreate / GetFloorMaterial = retail 0x0082D6B0 / 0x0082D6F0 / 0x0082D7D0.
// Callees: CThing::PeekDefinitionManager 0x004C7990 (x2), CDefinitionManager::GetDef<CThingBuildingDef> 0x004AB840 (x2), CDefClassBase
//   virtual slot 1 (Delete), NGraphicAppearance::CTCGraphicAppearance::SetScale(float) 0x004BFA50, CThingGameObject::OnCreate() 0x00838A40.
// Shape: the appearance pointer is loaded into ebx before the first PeekDefinition (inline: zeroed holder, GetDef, release, raw read);
//   the inline SetMainGraphic copies the 14-byte graphic record as three dwords and a word through `add edi,78h`; the def's Health is
//   read after the second PeekDefinition and stored to Health then MaxHealth (`fst` / `fstp`, `MaxHealth = Health = ...`); the
//   success path is the `if (CThingGameObject::OnCreate()) { ... }` block so the false return is laid out last.
// Layout: CThing PTCGraphicAppearance +0x64, DefGlobalIndex +0x8C; CThingPhysical Scale +0x9C, MaxHealth +0xB0, Health +0xB4;
//   CThingBuilding::OverrideAutomaticLocking bit 0 of +0xD4 (Ego_r bool:1); CTCGraphicAppearance::MainGraphic +0x44; CThingBaseDef
//   Health +0x50 and CThingBuildingDef Graphic +0x78 (Ego_r +0x7C).
// Name fix (agentNF9 2026-11-11): moved off the local models (a same-TU stand-in GetDef_004ab840 body, a padded CThingBuilding with a
//   descriptive m_flagD4) onto the shared headers; 0x004C7990 binds ?PeekDefinitionManager@CThing@@QBEABVCGameDefinitionManager@@XZ,
//   0x004AB840 ??$GetDef@VCThingBuildingDef@@@CDefinitionManager@@QBE_NJPAV?$CDefPointer@$$CBVCThingBuildingDef@@@@@Z, 0x004BFA50
//   ?SetScale@CTCGraphicAppearance@NGraphicAppearance@@QAEXM@Z, 0x00838A40 ?OnCreate@CThingGameObject@@MAE_NXZ (were
//   ?GetDefinitionManager@CThingBuilding@@QBEPAVCDefinitionManager@@XZ, ?GetDef_004ab840@..., ?SetScale@CTCGraphicAppearance@@QAEXM@Z,
//   ?OnCreateGameObject@CThingBuilding@@QAE_NXZ); the row's own symbol is the protected virtual (was public non-virtual QAE).
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
#define FABLE_RETAIL_CTHINGBUILDING
#define FABLE_RETAIL_CTHINGBUILDINGDEF
#define FABLE_RETAIL_CTHINGBUILDING_DEFINITION
#define FABLE_RETAIL_CTHINGBUILDING_PEEKSIMDEF
#define FABLE_RETAIL_CTHINGBASEDEF
#define FABLE_RETAIL_CTHINGGAMEOBJECT_ONCREATE
#define FABLE_RETAIL_CTCGRAPHICAPPEARANCE_MOVEMENT
#define FABLE_RETAIL_CTCGRAPHICAPPEARANCE_SETMAINGRAPHIC
#include "fable_string.h"
#include "engine/CThing.h"
#include "engine/CThingBuilding.h"
#include "engine/NGraphicAppearance_CTCGraphicAppearance.h"

bool CThingBuilding::OnCreate()
{
    NGraphicAppearance::CTCGraphicAppearance* appearance = PTCGraphicAppearance;
    appearance->SetMainGraphic(PeekDefinition().Graphic);
    appearance->SetScale(Scale);
    OverrideAutomaticLocking = false;
    if (CThingGameObject::OnCreate()) {
        MaxHealth = Health = PeekDefinition().Health;
        return true;
    }
    return false;
}
