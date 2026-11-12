#ifdef FABLE_RETAIL_CTCTARGETINGPLAYER_UPDATE_LIST
// Retail opt-in (Codex pass21): original UpdateTargetList helper models; both PDBs and donor/native proof in row00804020.
#ifndef FABLE_TARGETING_PLAYER_SUPPORT_H
#define FABLE_TARGETING_PLAYER_SUPPORT_H
#include "engine/CFilter_Bind2.h"
#include "engine/CFilter_Bind3.h"
#include "engine/CThingFilter_IsInTargetingRangeAndFOV.h"
#include "engine/CThingFilter_IsInFOV.h"
#include "engine/CTargetPointFilters.h"
#include "engine/CThingCompare_BestForTargeting2.h"
template<class A,class B,class C,class D,class E> class CFilter_Bind5 {
protected: const A& F1;const B& F2;const C& F3;const D& F4;const E& F5;
public:CFilter_Bind5(const A&a,const B&b,const C&c,const D&d,const E&e):F1(a),F2(b),F3(c),F4(d),F5(e){}
 template<class T> bool operator()(const T&t)const{return F1(t)&&F2(t)&&F3(t)&&F4(t)&&F5(t);}
};
template<class A,class B,class C,class D,class E> inline CFilter_Bind5<A,B,C,D,E> FABLE_FASTCALL GFFilter_Bind5(const A&a,const B&b,const C&c,const D&d,const E&e){return CFilter_Bind5<A,B,C,D,E>(a,b,c,d,e);}
class CThingFilter_IsTargetable:public std::unary_function<CThingPhysical,bool>{
 bool ZTargeting,UsingLightning;
public:CThingFilter_IsTargetable(bool z,bool l):ZTargeting(z),UsingLightning(l){} bool operator()(const CThing&)const;
};
class CThingFilter_IsInSameVolumeContextAsThing{
 CThing* PThingInside;bool Override;
public:CThingFilter_IsInSameVolumeContextAsThing(const CThing&,bool);bool operator()(const CThing&)const;
};
namespace {
class CThingFilter_IsValidTargetForPlayerCreature{
 bool AcceptStealableStockItems,PlayerIsChild,PlayerCarryingWeapon;
 const CThingPlayerCreature& PlayerCreature;const CThing* PTargetOnlyThing;
public:CThingFilter_IsValidTargetForPlayerCreature(const CThingPlayerCreature&,const CThing*);bool operator()(const CThing&)const;
};
class CThingFilter_IsTargetableBehind:public std::unary_function<NPlayerTargeting::CTargetPoint,bool>{
 const CThing& Targeter;const CThing* PTargetedThing;
public:CThingFilter_IsTargetableBehind(const CThing&t,const CThing*p):Targeter(t),PTargetedThing(p){}bool operator()(const NPlayerTargeting::CTargetPoint&)const;
};
class CTargetPointFilter_IsStealableStockItem:public std::unary_function<NPlayerTargeting::CTargetPoint,bool>{
public:CTargetPointFilter_IsStealableStockItem(){}
 bool operator()(const NPlayerTargeting::CTargetPoint& point)const{
  const CThing& thing=*point.PThing;
  const CTCStockItem* ptc_stock;
  if(thing.PeekTC(&ptc_stock) && ptc_stock->GetShopTC() && ptc_stock->IsStealable())return true;
  return false;
 }
};
class CTargetPointIsNotEnemyOfThing {
 const CThing& Thing;
public:CTargetPointIsNotEnemyOfThing(const CThing& t):Thing(t){}bool operator()(const NPlayerTargeting::CTargetPoint&)const;
};
class CGetBestTargetInDirection{
 const C3DVector& BasePos;C3DVector TestLine;float Direction;const NPlayerTargeting::CTargetPoint* PBestTargetPoint;float CosThetaBestAngle;
public:CGetBestTargetInDirection(const C3DVector&,const C3DVector&,float);void operator()(const NPlayerTargeting::CTargetPoint&);const NPlayerTargeting::CTargetPoint* GetPBestTargetPoint()const{return PBestTargetPoint;}
};
}
namespace {
CGetBestTargetInDirection::CGetBestTargetInDirection(const C3DVector& base_pos,const C3DVector& towards,float direction)
 :BasePos(base_pos),Direction(direction),PBestTargetPoint(0),CosThetaBestAngle(-2.0f)
{
 TestLine=towards-BasePos;
 TestLine.Z=0.0f;
 if(TestLine.GetSquaredMagnitude()>DELTA_NOTIONAL_ZERO*DELTA_NOTIONAL_ZERO)TestLine.AccurateNormalise();
}
}
namespace NGlobalConsole{extern bool GTargetingTargetPreferenceIn3D;}
#endif

#endif
