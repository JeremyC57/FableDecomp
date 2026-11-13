#ifndef FABLE_ENGINE_CFILTER_BIND4_H
#define FABLE_ENGINE_CFILTER_BIND4_H
// CFilter_Bind4 (new, chunk 2015, row 0x007A0D90): the four-filter member of the CFilter_Bind2 / CFilter_Bind3 family (protected const
// references, no functor copies: retail stores the four filter addresses into the 16-byte object, 0x007A0F75..0x007A0F92). Names: Ego_r.pdb /
// FableWin.pdb ??0?$CFilter_Bind4@VCThingFilter_IsNot@@V1@VCThingFilter_IsTargetable@@VCThingFilter_In2DRange@@@@QAE@ABVCThingFilter_IsNot@@0ABV..@@Z,
// ??$?RVCThingCreatureBase@@@?$CFilter_Bind4@..@@QBE_NABVCThingCreatureBase@@@Z (the template call operator) and the __fastcall factory
// ??$GFFilter_Bind4@VCThingFilter_IsNot@@V1@VCThingFilter_IsTargetable@@VCThingFilter_In2DRange@@@@YI?AV?$CFilter_Bind4@..@@ABVCThingFilter_IsNot@@0ABV..@@Z.
#include "rebuild_abi.h"
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
#endif
