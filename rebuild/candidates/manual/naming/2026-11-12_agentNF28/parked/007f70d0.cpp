// CThingSearchTools::GetPBestThingInRoughArea<CFilter_Bind4<CThingFilter_IsAvailableForCreatureInteraction,CThingFilter_In2DRangeLimit,
//   CThingFilter_CanSeeEachOther,std::unary_negate<CThingFilter_IsUnconscious> >,CThingCompare_Nearest,CThingAICreature>(area, filter, compare, out)
//   -- the nearest available, in-range, mutually visible, conscious AI creature in the rough area
// Leaf: ??$GetPBestThingInRoughArea@V?$CFilter_Bind4@VCThingFilter_IsAvailableForCreatureInteraction@@VCThingFilter_In2DRangeLimit@@VCThingFilter_CanSeeEachOther@@V?$unary_negate@VCThingFilter_IsUnconscious@@@std@@@@VCThingCompare_Nearest@@VCThingAICreature@@@CThingSearchTools@@QBE_NABVC2DBoxI@@ABV?$CFilter_Bind4@VCThingFilter_IsAvailableForCreatureInteraction@@VCThingFilter_In2DRangeLimit@@VCThingFilter_CanSeeEachOther@@V?$unary_negate@VCThingFilter_IsUnconscious@@@std@@@@ABVCThingCompare_Nearest@@PAPAVCThingAICreature@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x007F70D0, 613 bytes.
// NF28 (2026-11-12, queue land1011ec): was landed under the propagated label GetPBestThingInRoughArea<CFilter_Bind4<CThingFilter_IsAICreatureAwareOf,
//   CThingFilter_IsUnconscious,CThingFilter_IsAnAlly,...>> with local models (CThingSearchTools_Calls, Filters). Identity: Ego_r.pdb 0x007F2390 and
//   FableWin.pdb 0x02537170 both name exactly this instance; its only caller CTCTargetingAI::GetPBestCreatureForInteractionTypeWithinRangeLimit
//   0x007F67C0 (landed, call +0xD8) binds this spelling, and the filter calls in the body are IsAvailableForCreatureInteraction 0x006A7430,
//   In2DRangeLimit 0x006A7700, CanSeeEachOther 0x006A78D0 and IsUnconscious 0x006A73E0 (negated), as the caller builds them.
// Model: the caller's TU (0x007F67C0): the same shared headers and filter classes, the shared FABLE_RETAIL_CTHINGSEARCHTOOLS_BESTROUGH_BODY
//   template body (native 0x00942390), explicitly instantiated.
#include "fable_compiler.h"
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_CTCTARGETINGAI_ATTACK
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CTHINGSEARCHTOOLS
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_BESTROUGH
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_BESTROUGH_BODY
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_BESTROUGH_THISEND
#define FABLE_RETAIL_CTCMAPWHO
#define FABLE_RETAIL_CTCMAPWHO_SEARCHID
#define FABLE_RETAIL_CWORLDMAP
#define FABLE_RETAIL_CTHING_STATTYPEFLAGS
#define FABLE_RETAIL_C2DBOXI_RADIUS_INITIALIZERS
#define FABLE_RETAIL_C2DBOXI_CTORS
#define FABLE_RETAIL_C2DBOXI
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_STLP_REMOVE_COPY_IF
#define FABLE_RETAIL_STLP_NOT1
#include "fable_string.h"
#include "engine/C3DVector.h"
#include "engine/C2DBoxI.h"
#include "engine/CThing.h"
#include "engine/CThingAICreature.h"
#include "engine/CTargetingDef.h"
#include "engine/CTCTargetingAI.h"
#include "engine/CWorldMap.h"
#include "engine/CTCMapwho.h"
#include "engine/NThingMap_CThingMap.h"
#include "engine/CThingSearchTools.h"
#include "engine/CThingCompare_Nearest.h"
class CThingFilter_IsAvailableForCreatureInteraction {
public:
 CThingFilter_IsAvailableForCreatureInteraction(const CThingAICreature& source,ECreatureInteractionType type):SourceThing(const_cast<CThingAICreature*>(&source)),Type(type){}
 bool operator()(const CThingAICreature&)const;                 // 0x006A7430
private:
 CThingAICreature* SourceThing;
 ECreatureInteractionType Type;
};
class CThingFilter_In2DRangeLimit {
public:
 CThingFilter_In2DRangeLimit(const C3DVector& pos,float min_range,float max_range):Pos(pos),MinRange(min_range),MaxRange(max_range){}
 bool operator()(const CThing&)const;                           // 0x006A7700
private:
 C3DVector Pos;
 float MinRange;
 float MaxRange;
};
class CThingFilter_CanSeeEachOther {
public:
 CThingFilter_CanSeeEachOther(const CThingCreatureBase& creature):Creature(const_cast<CThingCreatureBase*>(&creature)){}
 bool operator()(const CThingCreatureBase&)const;               // 0x006A78D0
private:
 CThingCreatureBase* Creature;
};
class CThingFilter_IsUnconscious : public std::unary_function<CThing,bool> {
public:
 bool operator()(const CThing&)const;                           // 0x006A73E0
};
template<class TF1,class TF2,class TF3,class TF4> class CFilter_Bind4 {
protected:
 const TF1& F1;
 const TF2& F2;
 const TF3& F3;
 const TF4& F4;
public:
 CFilter_Bind4(const TF1& f1,const TF2& f2,const TF3& f3,const TF4& f4):F1(f1),F2(f2),F3(f3),F4(f4){}
 template<class T> bool operator()(const T& t)const{return F1(t)&&F2(t)&&F3(t)&&F4(t);}
};
template<class A,class B,class C,class D> inline CFilter_Bind4<A,B,C,D> FABLE_FASTCALL GFFilter_Bind4(const A& a,const B& b,const C& c,const D& d){return CFilter_Bind4<A,B,C,D>(a,b,c,d);}

template bool CThingSearchTools::GetPBestThingInRoughArea<
    CFilter_Bind4<CThingFilter_IsAvailableForCreatureInteraction,CThingFilter_In2DRangeLimit,CThingFilter_CanSeeEachOther,
                  std::unary_negate<CThingFilter_IsUnconscious> >,
    CThingCompare_Nearest,CThingAICreature>(
    const C2DBoxI&,
    const CFilter_Bind4<CThingFilter_IsAvailableForCreatureInteraction,CThingFilter_In2DRangeLimit,CThingFilter_CanSeeEachOther,
                        std::unary_negate<CThingFilter_IsUnconscious> >&,
    const CThingCompare_Nearest&,CThingAICreature**) const;
