// Original anonymous target-range helper.
// Leaf: ?GetTargetingRangeForTarget@?A0x
// Flags: (none)
// Bracket: Group the fallback return value.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_CDEFPOINTER_SETDYNAMICCAST_REREAD
#define FABLE_RETAIL_CDEFPOINTER_ORIGINAL_DIRECT_DEREF_PASS26
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFPOINTER_RELEASE_SELFDELETING_POINTEE
#define FABLE_RETAIL_CIVCOUNTEDPOINTEEBASE_RELEASE_ORIGINAL_SCOPES_PASS22
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CDEFINITIONMANAGER_GETDEF_GLOBAL_BODY
#define FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_NOINLINE
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CRESOURCE
#define FABLE_RETAIL_CTHINGBASEDEF
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CTCTARGETED
#define FABLE_RETAIL_CTCTARGETED_PTARGETINGDEF
#define FABLE_RETAIL_CTCAISCRATCHPAD_TARGETINGOFFSET
#include "fable_string.h"
#include "engine/CThing.h"
#include "engine/CThingBaseDef.h"
#include "engine/CThingCreatureDef.h"
#include "engine/CTCTargeted.h"
#include "engine/CTCAIScratchpad.h"
#include "engine/CTargetingDef.h"
#include "engine/CGameDefinitionManager.h"
#include "engine/CMainGameComponent.h"

#include "engine/CDefClassBase_GetSubDef.h"
namespace {
float __fastcall GetTargetingRange(const CTargetingDef& def, ETargetingType type){return def.GetTargetingRange(type);}   // 0x00802890
float __fastcall GetTargetingRangeForTargetType(unsigned long type, const CTargetingDef& def, const CThing& targeter,
                                                const CThing* carried);             // 0x008028D0

float __fastcall GetTargetingRangeForTarget(const CThing& target_thing,const CThing& targeting_thing,const CThing* pcarried_thing)
{
 const CTCTargeted* ptc_targeted;
 target_thing.PeekTC(&ptc_targeted);
 CDefPointer<const CTargetingDef> ptargeting_def=ptc_targeted->GetPTargetingDef();
 if(ptargeting_def.IsNull()) {
  if(target_thing.HasTC(TCI_ACTION_USE_SCRIPTED_HOOK)) {
   const CGameDefinitionManager& def_manager=CMainGameComponent::Get()->PeekDefinitionManager();
   long def_index=def_manager.GetDefGlobalIndexFromName("CREATURE_BASE_TEMPLATE");
   if(def_index>0) {
    CDefPointer<const CThingCreatureDef> pdef;
    def_manager.GetDef(def_index,&pdef);
    if(!pdef.IsNull()) {
     CDefPointer<const CTargetingDef> pcreature_targeting_def;
     if(pdef->GetSubDef(&pcreature_targeting_def))
      return (GetTargetingRange(*pcreature_targeting_def,(ETargetingType)1));
    }
   }
  }
 }else {
  unsigned long type_flags=ptc_targeted->GetTargetingFlags();
  float dist=GetTargetingRangeForTargetType(type_flags,*ptargeting_def,targeting_thing,pcarried_thing);
  const CTCAIScratchpad* tc_ai_scratchpad;
  if(target_thing.PeekTC(&tc_ai_scratchpad))dist+=tc_ai_scratchpad->GetTargetingDistanceOffset();
  return dist;
 }
 return 0.0f;
}
}

float __fastcall GetTargetingRangeForTarget_Use(const CThing& target, const CThing& targeter, const CThing* carried)
{
    return GetTargetingRangeForTarget(target, targeter, carried);
}