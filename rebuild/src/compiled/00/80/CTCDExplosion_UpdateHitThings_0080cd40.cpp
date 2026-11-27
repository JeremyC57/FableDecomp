// 0080cd40 CTCDExplosion::UpdateHitThings
#define FABLE_RETAIL_CTHING_STATTYPEFLAGS
#define FABLE_RETAIL_CTHINGCREATUREBASE_STATTYPEFLAGS
#define FABLE_RETAIL_CTCMAPWHO
#define FABLE_RETAIL_CTCMAPWHO_SEARCHID
#define FABLE_RETAIL_CTHINGMAP_PEEKCELL_INLINE
#define FABLE_RETAIL_C2DBOXI
#define FABLE_RETAIL_C2DBOXI_COPY
#define FABLE_RETAIL_C2DBOXI_GETWIDTH
#define FABLE_RETAIL_C2DBOXI_GETHEIGHT
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_ORIGINAL_ROUGHAREA
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_ROUGHAREA_EGOD
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_CLIPAREA
// Leaf: ?UpdateHitThings@CTCDExplosion@@AAEXABVC3DVector@@0@Z
// Flags: /O2 /Oy /Gr
#define FABLE_RETAIL_EXPLOSION_HITS_PASS29
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCBASE_COMPONENT
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTCEXPLODINGOBJECT_PASS22
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CTHINGSEARCHTOOLS
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CVECTORMAP_INLINE_LOWERBOUND
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_STLP_VECTOR
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_CARRAY_ORIGINAL_INDEX_BASE_PASS22
#define FABLE_RETAIL_STLP_BACK_INSERTER
#define FABLE_RETAIL_STLP_REMOVE_COPY_IF
#define FABLE_RETAIL_STLP_NOT1
#define FABLE_RETAIL_FILTER_BIND3_FUNCTION
#define FABLE_RETAIL_THINGFILTER_ISNOTLIST_INLINE_CTOR
#define FABLE_RETAIL_STLP_FIND_RANDOM_DISPATCH
#define FABLE_RETAIL_STLP_FIND_NOINLINE_BODY
#include "engine/CThingCreatureBase.h"
#include "engine/C3DVector.h"
#include "engine/CIntelligentPointer.h"
#include "engine/C2DBoxI.h"
#include "engine/CTCDExplosion.h"
#include "engine/CTCExplodingObject.h"
#include "engine/CTCSmashableBase.h"
#include "engine/CMainGameComponent.h"
class CTCMapwho;
#include "engine/CTCMapwho.h"
#include "engine/NThingMap_CThingMap.h"
#include "engine/CThingSearchTools.h"
#include "engine/CFilter_Bind3.h"
#include "engine/CFilter_Bind4.h"
#include "engine/CThingFilter_IsNot.h"
#include "engine/CThingFilter_IsNotList.h"
#include "engine/CThingFilter_IsWithinArea.h"
#include "engine/tcd_explosion_filters.h"
#include "stlport/fable_algorithm.h"

namespace NPhysicsHelpers{
C2DBoxI FABLE_FASTCALL GetLineThingSearchArea(const C3DVector&,const C3DVector&,float);
float FABLE_FASTCALL GetPerpendicularDistanceFromFiniteLine(const C3DVector&,const C3DVector&,const C3DVector&);
}
namespace std {
template<class I,class O,class F> inline O FABLE_FASTCALL transform(I first,I last,O dest,F fn){
    for(;first!=last;++first,++dest)*dest=fn(*first);
    return dest;
}}
inline float CTCDExplosion::GetDistanceFromMeAsRatioOfDamageRadius(const CThing& t)const{
 return NPhysicsHelpers::GetPerpendicularDistanceFromFiniteLine(StartPos,EndPos,t.GetPos())/DamageRadius;
}
void CTCDExplosion::UpdateHitThings(const C3DVector& line_start,const C3DVector& line_end)
{
 C2DBoxI area=NPhysicsHelpers::GetLineThingSearchArea(line_start,line_end,DamageRadius);
 std::vector<const CThing*> things_to_ignore;
 std::transform(HitThings.begin(),HitThings.end(),std::back_inserter(things_to_ignore),CGetThingFromIntelligentPointer());
 std::transform(ImmuneThings.begin(),ImmuneThings.end(),std::back_inserter(things_to_ignore),CGetThingFromIntelligentPointer());
 std::vector<const CThing*> specified_things_to_hit;
 std::transform(SpecifiedThingsToHit.begin(),SpecifiedThingsToHit.end(),std::back_inserter(specified_things_to_hit),CGetThingFromIntelligentPointer());
 std::list<CThingCreatureBase*> creature_list;
 GetThingSearchTools().GetAllThingsInRoughArea<CThingCreatureBase>(area,GFFilter_Bind4(CThingFilter_IsNot(GetParentThing()),CThingFilter_IsNotList(things_to_ignore),CThingFilter_IsWithinArea(line_start,line_end,DamageRadius,true),std::not1(CThingFilter_IsEvadeExplosion(line_start,line_end))),creature_list);
 // FableWin preserves the original for-loop scope in the debug branch sequence.
 if (false) {} else for (std::list<CThingCreatureBase*>::iterator it=creature_list.begin();it!=creature_list.end();++it){
    CThingCreatureBase* pcreature=*it;
    if(specified_things_to_hit.empty()||std::find(specified_things_to_hit.begin(),specified_things_to_hit.end(),pcreature)!=specified_things_to_hit.end())GiveDamageToThing(*pcreature);
 }
 std::list<CThing*> thing_list;
 GetThingSearchTools().GetAllThingsInRoughArea<CThing>(area,GFFilter_Bind3(CThingFilter_IsNot(GetParentThing()),CThingFilter_IsNotList(things_to_ignore),CThingFilter_IsWithinArea(line_start,line_end,DamageRadius,false)),thing_list);
 // FableWin preserves the original for-loop scope in the debug branch sequence.
 if (false) {} else for (std::list<CThing*>::iterator it=thing_list.begin();it!=thing_list.end();++it){
    CThing* pthing=*it;
    if(specified_things_to_hit.empty()||std::find(specified_things_to_hit.begin(),specified_things_to_hit.end(),pthing)!=specified_things_to_hit.end()){
        bool thing_set_on_fire=false;
        CTCExplodingObject* ptc_explosion;
        CTCSmashableBase* ptc_smashable;
        if(pthing->GetTC(&ptc_explosion)){
            if(!thing_set_on_fire){
                float distance_ratio=GetDistanceFromMeAsRatioOfDamageRadius(*pthing);
                ptc_explosion->SetCountdownTimer((long)((0.25f+0.75f*distance_ratio)*CMainGameComponent::GetConstantFPS()));
            }else if(pthing->GetTypeFlags()&4){
                CThingGameObject* thing_game_object=static_cast<CThingGameObject*>(pthing);
                GiveDamageToThing(*thing_game_object);
            }
        }else if(pthing->GetTC(&ptc_smashable)){
            if(pthing->GetTypeFlags()&4){
                CThingGameObject* thing_game_object=static_cast<CThingGameObject*>(pthing);
                GiveDamageToThing(*thing_game_object);
            }else if(!thing_set_on_fire)pthing->Kill(true);
        }
    }
 }
}