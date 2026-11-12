// CTCTargetingAI::GetPBestCreatureForInteractionTypeWithinRangeLimit(ECreatureInteractionType type, float min_range, float max_range) const
// -- the nearest conscious creature, available for the given interaction type and able to see the parent thing, in the 2D
// range band [min_range, max_range] around the parent thing.
// Leaf: ?GetPBestCreatureForInteractionTypeWithinRangeLimit@CTCTargetingAI@@UBEPAVCThingCreatureBase@@W4ECreatureInteractionType@@MM@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x007F67C0, 233 bytes (`ret 0Ch`). Ego_r 0x007F1DE0 / FableWin 0x02536890 (UBE: const virtual).
// Callees: CThing::GetPos @ 0x004C73D0, __ftol2 @ 0x00BFEA70 (X, Y, max range), CTCBase::PeekThingSearchTools @ 0x00686D90 (const,
// so the method is const), CThingSearchTools::GetPBestThingInRoughArea<CFilter_Bind4<CThingFilter_IsAvailableForCreatureInteraction,
// CThingFilter_In2DRangeLimit, CThingFilter_CanSeeEachOther, std::unary_negate<CThingFilter_IsUnconscious> >, CThingCompare_Nearest,
// CThingAICreature> @ 0x007F70D0 (FableWin 0x02537170: T is CThingAICreature, not CThingCreatureBase).
// FableWin's debug TU (0x025369A0..0x02538010) shows the original spelling: the filter ctors, std::not1, GFFilter_Bind4 and the Bind4
// ctor, all inline in retail.
// Shape: C2DBoxI(pos.ToC2DCoordI(), (long)max_range) (X, Y, range truncated in that order); the filters and comparator are
// temporaries in the call (the empty negated filter homed in the dead max_range slot).
// Visibility: the template body of GetPBestThingInRoughArea is in the header (FABLE_RETAIL_CTHINGSEARCHTOOLS_BESTROUGH_BODY): retail's
// compiler saw it (it calls the instance out of line). With a declaration only, VC7.1 schedules the `pbest_creature = 0` store ahead
// of the `fld [pos.X]`; with the body visible it follows it, as in retail.
// Layout: the filter classes and CFilter_Bind4 have no shared engine header; they are modelled here from Ego_r.pdb
// (IsAvailableForCreatureInteraction {SourceThing, Type}, In2DRangeLimit {Pos, MinRange, MaxRange}, CanSeeEachOther {Creature},
// CFilter_Bind4 {F1..F4} const references) with the FableWin ctor signatures.
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

CThingCreatureBase* CTCTargetingAI::GetPBestCreatureForInteractionTypeWithinRangeLimit(ECreatureInteractionType type,float min_range,float max_range) const
{
 const CThingAICreature& parent=static_cast<const CThingAICreature&>(GetParentThing());
 const C3DVector& pos=GetParentThing().GetPos();
 CThingAICreature* pbest_creature=0;
 C2DBoxI area(pos.ToC2DCoordI(),(long)max_range);
 PeekThingSearchTools().GetPBestThingInRoughArea(area,
  GFFilter_Bind4(CThingFilter_IsAvailableForCreatureInteraction(parent,type),CThingFilter_In2DRangeLimit(pos,min_range,max_range),
   CThingFilter_CanSeeEachOther(parent),std::not1(CThingFilter_IsUnconscious())),
  CThingCompare_Nearest(pos),&pbest_creature);
 return pbest_creature;
}