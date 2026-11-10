#include <cstdio>
#include <cstdlib>
#include <vector>
#include <algorithm>
#include <functional>
#include <iterator>
#include <cmath>
static void check(bool b){if(!b)std::abort();}
class C3DVector{ public:float X,Y,Z;C3DVector(float x=0,float y=0,float z=0):X(x),Y(y),Z(z){}C3DVector operator-(const C3DVector& b)const{return C3DVector(X-b.X,Y-b.Y,Z-b.Z);}float GetMagnitude()const{return float(std::sqrt(X*X+Y*Y+Z*Z));}};
class CThingCreatureBase{ public:bool dead;C3DVector pos;bool IsDead()const{return dead;}const C3DVector& GetPos()const{return pos;}};
template<class T>class CIntelligentPointer{ public:T* p;bool operator!=(int)const{return p!=0;}T* operator->()const{return p;}};
template<class T>bool IS_THING_ALIVE(const T& p){return p!=0&&!p->IsDead();}
class CWorldMap{ public:};class CThingSearchTools{ public:};class CTCShapeManager{ public:};
class CWorld{ public:CWorldMap map;CThingSearchTools tools;unsigned long seed;CWorldMap& GetWorldMap(){return map;}const CThingSearchTools& PeekThingSearchTools()const{return tools;}unsigned long* GetWorldSeed()const{return const_cast<unsigned long*>(&seed);}};
static CWorld world;static std::vector<C3DVector> supplied;static CThingCreatureBase target;static int queries,world_calls;
namespace NCreatureHiding{
static void GetValidPositions(std::vector<C3DVector>& out,const CWorldMap& map,const CThingSearchTools& tools,const C3DVector& pos,float radius,bool b,const CTCShapeManager* shapes){
 check(&map==&world.map&&&tools==&world.tools&&&pos==&target.pos&&radius==11.0f&&!b&&!shapes);++queries;out=supplied;
}}
class CVectorFilter_FurtherFromPointThan:public std::unary_function<C3DVector,bool>{
 float Radius;const C3DVector CentrePos;
public:CVectorFilter_FurtherFromPointThan(const C3DVector& p,float r):Radius(r),CentrePos(p){}bool operator()(const C3DVector& p)const{return (p-CentrePos).GetMagnitude()>Radius;}
};
// Fixed-width arithmetic models the 32-bit target seed on both test hosts.
static unsigned long GFRandom(unsigned long range,unsigned long* seed){
 unsigned int v=static_cast<unsigned int>(*seed);v=v*0x24a1U+0x24dfU;v=(v>>13)|(v<<19);*seed=v;return range?v%range:0;
}
struct CAIStateGroup_JackOfBladesStage1{
 CIntelligentPointer<CThingCreatureBase> PTargetThing;
 CWorld& GetWorld()const{++world_calls;return world;}
 bool GetRushToPosForLunge(C3DVector&);
};
#ifdef _MSC_VER
typedef unsigned __int64 WideUInt;
#else
typedef unsigned long WideUInt;
#endif
bool CAIStateGroup_JackOfBladesStage1::GetRushToPosForLunge(C3DVector& rush_to_pos)
{
 if(IS_THING_ALIVE(PTargetThing)){
  std::vector<C3DVector> validPositions;
  NCreatureHiding::GetValidPositions(validPositions,GetWorld().GetWorldMap(),GetWorld().PeekThingSearchTools(),PTargetThing->GetPos(),11.0f,false,0);
  if(validPositions.size()==0)return false;
  std::vector<C3DVector>::iterator it=std::remove_if(validPositions.begin(),validPositions.end(),std::not1(CVectorFilter_FurtherFromPointThan(PTargetThing->GetPos(),10.0f)));
  rush_to_pos=validPositions[GFRandom(std::distance(validPositions.begin(),it),GetWorld().GetWorldSeed())];
  return true;
 }
 return false;
}

int main(){
 CAIStateGroup_JackOfBladesStage1 state;target.pos=C3DVector(16,8,4);int cases=0;
 const unsigned long seeds[5]={0,1,0xffffffffUL,123456789UL,0x80000000UL};
 for(int live=0;live<3;++live)for(int n=0;n<=6;++n)for(int mask=0;mask<64;++mask)for(int seed=0;seed<5;++seed){
  state.PTargetThing.p=live?&target:0;target.dead=live==2;supplied.clear();std::vector<C3DVector> retained;
  for(int j=0;j<n;++j){float offset=(mask&(1<<j))?float(11+j):float(5+j);C3DVector v(target.pos.X+offset,target.pos.Y,target.pos.Z);supplied.push_back(v);if(offset>10)retained.push_back(v);}
  world.seed=seeds[seed];queries=world_calls=0;C3DVector result(-8,-4,-2);bool expected=live==1&&n>0;
  check(state.GetRushToPosForLunge(result)==expected);check(queries==(live==1?1:0));check(world_calls==(live==1?(n?3:2):0));
  if(expected){
   WideUInt stepped=(static_cast<WideUInt>(seeds[seed])*WideUInt(9377)+WideUInt(9439))&WideUInt(0xffffffffUL);
   unsigned long rotated=static_cast<unsigned long>((stepped/WideUInt(8192))+(stepped%WideUInt(8192))*WideUInt(524288));
   check(world.seed==rotated);C3DVector wanted=retained.empty()?supplied[0]:retained[rotated%retained.size()];
   check(result.X==wanted.X&&result.Y==wanted.Y&&result.Z==wanted.Z);
  }else{check(world.seed==seeds[seed]);check(result.X==-8&&result.Y==-4&&result.Z==-2);}
  ++cases;
 }
 check(cases==6720);std::puts("00971ed0_TEST PASS");return 0;
}