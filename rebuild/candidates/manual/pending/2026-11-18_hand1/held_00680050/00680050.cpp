// CThingObjectDef::Validate(const CValidateDefinitionInitBase& validate, const CCharString& instantiation_name) const -- the release build keeps only the side effects of the def checks: the object graphic's mesh info (static / animated by graphic type), the appearance sub-def's DEFAULT animation, and for a weapon-like property (0x100 / 0x200) the mesh's ACTION_POINT_01 dummy object
// Grade: RELOCATION_MATCH (439v439; relocation identity clean)
// Fable8 note: the only change from the escalated draft is the include order: engine/CMeshDataBank.h before engine/CWorld.h (CWorld::PeekMeshBank returns it). That moves the c1 ordinal of CCharString::NullChar from 0xA68 (bucket 0x28) to 0xBFB (bucket 0x3B). c2 numbers a function's candidate records from the records of its unused constants, which it frees by walking a 64-bucket table (0x10882050; integers hash by value & 0x3F, address constants by the c1 symbol ordinal & 0x3F of the symbol they name, hash 0x1077E10B, walk 0x1077FF3B). The root candidates (this, world, info, pappearance_def, the cleanup-flags temporary, ...) take the freed ids last-freed-first, the split pass runs in ascending id and reloads come out in piece-id order. Retail needs id(pappearance_def) < id(flags): here CCharString::NullChar (constant 18) must land in a higher bucket than the START/END/PRIMARY externs, "DEFAULT" and the CAnimFilterNoGroup vftable, so that the flags temporary takes record 18 and pappearance_def keeps record 12.
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
