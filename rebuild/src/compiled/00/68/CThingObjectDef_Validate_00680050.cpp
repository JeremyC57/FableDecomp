// CThingObjectDef::Validate(const CValidateDefinitionInitBase& validate, const CCharString& instantiation_name) const -- the release build keeps only the side effects of the def checks: the object graphic's mesh info (static / animated by graphic type), the appearance sub-def's DEFAULT animation, and for a weapon-like property (0x100 / 0x200) the mesh's ACTION_POINT_01 dummy object
// Grade: RELOCATION_MATCH (439v439; relocation identity clean)
// Include-order: engine/CMeshDataBank.h is included before engine/CWorld.h (CWorld::PeekMeshBank returns it). This order is part of the
//   match: it moves the c1 ordinal of CCharString::NullChar from 0xA68 (bucket 0x28) to 0xBFB (bucket 0x3B), which changes how c2 numbers
//   its candidate records and so the reload order at 0x00680196 (edi = pappearance_def.Object, then eax = the temporary-cleanup mask).
//   About 155 statement-level variants (hand1, Fable advisor fable8, hand2a) found no source-level fix. Ego_d's thing_object_def.obj holds
//   only this function, so the original file's symbol ordinals came from its include list alone; the exact original list is not
//   recoverable. Landed with this note by owner decision (2026-11-19). Fragile: new declarations ahead of CAnimFilterBase in a shared
//   header can break the match (fable8); recheck this row after header changes.
// Retail: 0x00680050,439 bytes,const virtual thiscall.
// Leaf: ?Validate@CThingObjectDef@@UBEXABVCValidateDefinitionInitBase@@ABVCCharString@@@Z
// Flags: none
#define FABLE_RETAIL_CDEFPOINTER_RELEASE_SELFDELETING_POINTEE
#define FABLE_RETAIL_CIVCOUNTEDPOINTEEBASE_RELEASE_DELETES
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTHINGBASEDEF
#define FABLE_RETAIL_CTHINGOBJECTDEF_VALIDATE
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
#define FABLE_RETAIL_CANIMATIONSET_ANIMEXISTS
#define FABLE_RETAIL_CANIMFILTER
#define FABLE_RETAIL_CAPPEARANCEDEF_COMPLETE
#include "engine/CThingObjectDef.h"
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
extern const CCharString START_WEAPON_POINT_NAME;
extern const CCharString END_WEAPON_POINT_NAME;
extern const CCharString PRIMARY_HOLDABLE_WEAPON_DUMMY_OBJECT_NAME;
extern const CCharString ACTION_POINT_01;
void CThingObjectDef::Validate(const CValidateDefinitionInitBase& validate,const CCharString& instantiation_name)const
{
 const CValidateDefinitionInit& validation_class=static_cast<const CValidateDefinitionInit&>(validate);
 const CWorld& world=validation_class.World;
 CCharString start_weapon_point_name(START_WEAPON_POINT_NAME);
 CCharString end_weapon_point_name(END_WEAPON_POINT_NAME);
 CCharString holdable_object_dummy_name(PRIMARY_HOLDABLE_WEAPON_DUMMY_OBJECT_NAME);
 const CCharString& object_name=instantiation_name;
 const C3DMeshInfo* info=0;
 if(Graphic.Type==5 && Graphic.BankIndex)info=world.PeekMeshBank()->PeekStaticMesh(Graphic.BankIndex);
 else if(Graphic.Type==4 && Graphic.BankIndex)info=world.PeekMeshBank()->PeekAnimatedMesh(Graphic.BankIndex);
 CDefPointer<const CAppearanceDef> pappearance_def(NULL_DEF);
 Graphic.Type==4 && GetSubDef(&pappearance_def) && !pappearance_def->Animation.AnimExists("DEFAULT",CAnimFilterNoGroup());
 if(Property==0x100 || Property==0x200)
  info && !info->GetMeshStats()->HasDummyObject(ACTION_POINT_01.GetCRC());
}