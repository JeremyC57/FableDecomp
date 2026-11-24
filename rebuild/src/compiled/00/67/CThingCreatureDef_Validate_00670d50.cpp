// CThingCreatureDef::Validate(const CValidateDefinitionInitBase& validate, const CCharString& instantiation_name) const -- the release build
//   keeps only the side effects of the def checks (results unused): the creature graphic's mesh info (static / animated by graphic type)
//   from the world's mesh bank, the appearance def's DEFAULT animation for an animated mesh, and the mesh's primary-holdable dummy object /
//   main focal helper point
// Leaf: ?Validate@CThingCreatureDef@@UBEXABVCValidateDefinitionInitBase@@ABVCCharString@@@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x00670D50, 533 bytes, __thiscall virtual, `ret 8`.
// Callees: CCharString copy 0x0099EC30 / (const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0; CMeshDataBank::PeekStaticMesh
//   0x00A24280 / PeekAnimatedMesh 0x00A242B0; CDefClassBase::GetSubDef<CAppearanceDef> 0x0042B0A2; CAnimationSet::AnimExists 0x005DC2E0
//   (CAnimFilterNoGroup temporary); C3DMeshInfo::GetMeshStats 0x00A25530; CCRC::Calc 0x004014A0 (the inline CCharString::GetCRC);
//   C3DMeshStats::HasHelperPoint 0x00A2EA20 / HasDummyObject 0x00A2EA60; operator delete 0x00BFE9BC.
// Port (NF101, 2026-11-19, q11286): the validate argument's +0x08 member is CValidateDefinitionInit::World (const CWorld&; shared
//   engine/CValidateDefinitionInit.h: Anniversary TPI 0x7D39, Ego_d `validation_class` / `world` locals, retail 0x0068005B) and the
//   world's +0x3C is PMainMeshBank (CWorld::PeekMeshBank): the landed row's descriptive local CValidateDefinitionInitBase::Resources /
//   CValidateResources::MeshBank model, the renamed CDefinitionManager include and the dozen local classes replaced by the shared headers
//   with the 0x00680050 (CThingObjectDef::Validate) define set and statement shape.
// Include-order: engine/CMeshDataBank.h before engine/CWorld.h, as 0x00680050.
#define FABLE_RETAIL_CDEFPOINTER_RELEASE_SELFDELETING_POINTEE
#define FABLE_RETAIL_CIVCOUNTEDPOINTEEBASE_RELEASE_DELETES
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTHINGBASEDEF
#define FABLE_RETAIL_CTHINGCREATUREDEF_MEMBERS
#define FABLE_RETAIL_CTHINGCREATUREDEF_VALIDATE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCHARSTRING_DATA
#define FABLE_RETAIL_CCHARSTRING_GETCRC_INLINE
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CWORLD_MESH_BANK
#define FABLE_RETAIL_CWORLD_PEEKMESHBANK
#define FABLE_RETAIL_CMESHDATABANK
#define FABLE_RETAIL_CMESHDATABANK_BASES
#define FABLE_RETAIL_CBANKFILE_ENTRYQUERY
#define FABLE_RETAIL_CBANKFILE
#define FABLE_RETAIL_CMESHDATABANK_PEEKSTATICANIMATED
#define FABLE_RETAIL_CMESHDATABANKENTRY
#define FABLE_RETAIL_C3DMESHINFO_GETMESHSTATS
#define FABLE_RETAIL_C3DMESHSTATS_HASHELPERPOINT
#define FABLE_RETAIL_CANIMATIONSET_ANIMEXISTS
#define FABLE_RETAIL_CANIMFILTER
#define FABLE_RETAIL_CAPPEARANCEDEF_COMPLETE
#include "engine/CThingCreatureDef.h"
#include "engine/CValidateDefinitionInitBase.h"
#include "engine/CValidateDefinitionInit.h"
#include "engine/CMeshDataBank.h"
#include "engine/CWorld.h"
#include "engine/C3DMeshInfo.h"
#include "engine/C3DMeshStats.h"
#include "engine/CAppearanceDef.h"
#include "engine/CAnimFilterNoGroup.h"
#define FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_CAPPEARANCEDEF_SIZE_BODY   // NF357: GetSubDef<CAppearanceDef> pick-any copy = size-mode owner 0x0042B0A2 (scaffolding, not retail)
#include "engine/CDefClassBase_GetSubDef.h"
extern const CCharString PRIMARY_HOLDABLE_WEAPON_DUMMY_OBJECT_NAME;     // 0x013BAC30
extern const CCharString SECONDARY_HOLDABLE_WEAPON_DUMMY_OBJECT_NAME;   // 0x013BAC34
extern const CCharString MAIN_FOCAL_POSITION_NAME;                     // 0x013BAC48

void CThingCreatureDef::Validate(const CValidateDefinitionInitBase& validate,const CCharString& instantiation_name)const
{
 const CValidateDefinitionInit& validation_class=static_cast<const CValidateDefinitionInit&>(validate);
 const CWorld& world=validation_class.World;
 CCharString primary_dummy_name(PRIMARY_HOLDABLE_WEAPON_DUMMY_OBJECT_NAME);
 CCharString secondary_dummy_name(SECONDARY_HOLDABLE_WEAPON_DUMMY_OBJECT_NAME);
 const C3DMeshInfo* info=0;
 if(Graphic.Type==5){
  const CMeshDataBank* bank=world.PeekMeshBank();
  info=bank->PeekStaticMesh(Graphic.BankIndex);
 }
 else if(Graphic.Type==4)info=world.PeekMeshBank()->PeekAnimatedMesh(Graphic.BankIndex);
 CDefPointer<const CAppearanceDef> pappearance_def;
 GetSubDef(&pappearance_def);
 Graphic.Type!=4 || pappearance_def->Animation.AnimExists("DEFAULT",CAnimFilterNoGroup());
 if(PrimaryMeleeWeaponDef)
  !info || info->GetMeshStats()->HasDummyObject(primary_dummy_name.GetCRC());
 !info || info->GetMeshStats()->HasHelperPoint(MAIN_FOCAL_POSITION_NAME.GetCRC());
}
