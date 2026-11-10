#include <stdio.h>
#include <vector>
class C3DVector {public:float X,Y,Z;C3DVector(float x=0,float y=0,float z=0):X(x),Y(y),Z(z){}};
class C2DVector {public:float X,Y;C2DVector(const C3DVector& p):X(p.X),Y(p.Y){}};
class C2DBoxI {public:long TLX,TLY,BRX,BRY;};
class C2DBoxF {public:float TLX,TLY,BRX,BRY;C2DBoxF(const C2DVector& p,float r):TLX(p.X-r),TLY(p.Y-r),BRX(p.X+r),BRY(p.Y+r){}C2DBoxI ToC2DBoxI()const{C2DBoxI r;r.TLX=(long)TLX;r.TLY=(long)TLY;r.BRX=(long)BRX;r.BRY=(long)BRY;return r;}};
class CThing {public:bool Preferred,Occupied;};
class CThingAICreature:public CThing {public:C3DVector Position;const C3DVector& GetPos()const{return Position;}};
class CThingFilter_IsPreferredFirePoint {public:CThingFilter_IsPreferredFirePoint(){}bool operator()(const CThing& t)const{return t.Preferred;}};
class CThingFilter_IsOccupiableOccupied {public:CThingFilter_IsOccupiableOccupied(){}bool operator()(const CThing& t)const{return t.Occupied;}};
namespace std {template<class P>class unary_negate {P pred;public:explicit unary_negate(const P& p):pred(p){}bool operator()(const CThing& t)const{return !pred(t);}};template<class P>inline unary_negate<P> not1(const P& p){return unary_negate<P>(p);}}
template<class A,class B>class CFilter_Bind2 {const A& F1;const B& F2;public:CFilter_Bind2(const A&a,const B&b):F1(a),F2(b){}bool operator()(const CThing& t)const{return F1(t)&&F2(t);}};
template<class A,class B>inline CFilter_Bind2<A,B> GFFilter_Bind2(const A&a,const B&b){return CFilter_Bind2<A,B>(a,b);}
static C2DBoxI captured;static int calls;static CThing things[4];
class CThingSearchTools {public:template<class T,class C,class F>void GetAllThingsInRoughArea(const C2DBoxI& box,const F& filter,C& out)const{captured=box;++calls;for(int i=0;i<4;++i)if(filter(things[i]))out.push_back(&things[i]);}};
class CAIStateGroup_RangedCombatUsePreferredFirePoint {public:CThingAICreature* Creature;CThingSearchTools* Search;CThingAICreature& GetCreature()const{return *Creature;}const CThingSearchTools& PeekThingSearchTools()const{return *Search;}void GetFirePointList(std::vector<CThing*>&);};
void CAIStateGroup_RangedCombatUsePreferredFirePoint::GetFirePointList(std::vector<CThing*>& fire_points){
 PeekThingSearchTools().GetAllThingsInRoughArea<CThing>(C2DBoxF(GetCreature().GetPos(),30.0f).ToC2DBoxI(),GFFilter_Bind2(CThingFilter_IsPreferredFirePoint(),std::not1(CThingFilter_IsOccupiableOccupied())),fire_points);
}

int main(){
 CThingAICreature creature;CThingSearchTools search;CAIStateGroup_RangedCombatUsePreferredFirePoint row;row.Creature=&creature;row.Search=&search;
 for(int i=0;i<4;++i){things[i].Preferred=(i&1)!=0;things[i].Occupied=(i&2)!=0;}
 for(int x=-4;x<=4;++x)for(int y=-4;y<=4;++y){
  creature.Position=C3DVector(x*8+0.5f,y*8-0.5f,100);std::vector<CThing*> out;out.push_back(&things[0]);calls=0;row.GetFirePointList(out);
  if(calls!=1||out.size()!=2||out[0]!=&things[0]||out[1]!=&things[1])return 1;
  if(captured.TLX!=(long)(creature.Position.X-30)||captured.TLY!=(long)(creature.Position.Y-30)||captured.BRX!=(long)(creature.Position.X+30)||captured.BRY!=(long)(creature.Position.Y+30))return 2;
 }
 puts("0095d4c0_TEST PASS");return 0;
}