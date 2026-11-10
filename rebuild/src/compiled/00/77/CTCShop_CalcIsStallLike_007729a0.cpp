// CTCShop::CalcIsStallLike -- determine whether this shop behaves like a stall.
// Retail: 0x007729A0, 319 bytes; thiscall.
// Grade: VERIFIED_MATCH; see ROW.json and PROBES.md for verification or remaining residue.
// Leaf: ?CalcIsStallLike@CTCShop@@QAE_NXZ
// Flags: none
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFPOINTER_RELEASE_SELFDELETING_POINTEE
#define FABLE_RETAIL_CIVCOUNTEDPOINTEEBASE_RELEASE_DELETES
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_GET_ORIGINAL_CHAIN
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COMPARE_NULL_INT
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CDEFINITIONMANAGER_GETDEF_GLOBAL_BODY
#define FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_BODY
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CRESOURCE
#define FABLE_RETAIL_CTCSHOP_CALC_STALL
#define FABLE_RETAIL_CTCTHINGOWNER
#include "engine/CThing.h"
#include "engine/CThingBaseDef.h"
#include "engine/CShopDef.h"
#include "engine/CTCShop.h"
#include "engine/CTCThingOwner.h"
#include "engine/CGameDefinitionManager.h"
#include "engine/ThingAlive.h"
bool CTCShop::CalcIsStallLike()
{
 CDefPointer<const CThingBaseDef> pdef;
 GetDefinitionManager().GetDef(ParentThing->GetDefGlobalIndex(),&pdef);
 CDefPointer<const CShopDef> ShopDef;
 if(pdef->GetSubDef(&ShopDef) && ShopDef->IsStockDisplayPermanent)return false;
 CTCThingOwner* tc_owner;
 if(GetParentThing().GetTC(&tc_owner)){
  const std::list<CIntelligentPointer<CThing> >& objects=tc_owner->GetObjectList();
  for(std::list<CIntelligentPointer<CThing> >::const_iterator i=objects.begin();i!=objects.end();++i){
   if(IS_THING_ALIVE(*i) && (*i)->HasTC(TCI_DOOR))return false;
  }
 }
 return true;
}