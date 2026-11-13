// CThingObjectDef::Validate -- query weapon meshes, default animation and attachment markers.
// Grade: DIFFER(439v439) -- One adjacent load swap at00680196: pappearance_def reload versus conditional cleanup flags
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
#include "engine/CWorld.h"
#include "engine/CMeshDataBank.h"
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
 const CValidateDefinitionInit& validation_class=reinterpret_cast<const CValidateDefinitionInit&>(validate);
 // Layout: donor01fd74a7/01fb5297 prove the reference read at+8 after original base8.
 // The derived complete extent and original member spelling remain unproven.
 const CWorld& world=**reinterpret_cast<const CWorld* const*>(reinterpret_cast<const unsigned char*>(&validation_class)+8);
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
