#include <cstdio>
#include <cmath>
#ifndef _MSC_VER
#define __forceinline inline
#define FABLE_ENGINE_CAISTATEGROUPBASE_H
#define FABLE_ENGINE_CAIBRAIN_H
class CThingAICreature;
class CAIBrain{ public:CThingAICreature* Creature;};
class CAIStateGroupBase{ public:void* __vftable;CAIBrain* PBrain;};
#endif
// CAIStateGroup_FlyingThingBase::GetRandomFlightPos(const C3DVector&,float,C3DVector*,bool) -- find a same-region wander position and raise it above local and ground heights.
// Retail00912700,309 bytes. Shared Base/Brain storage; FlyingThingBase absent from active headers. Final bool argument is unused by retail.
#include "engine/CAIStateGroupBase.h"
#include "engine/CAIBrain.h"
class C3DVector { public:float X,Y,Z;};
struct CNavigationMethods {unsigned long GetNavigatorType();void SetNavigatorToType(unsigned long);bool GetNearbyWanderPosition(const C3DVector&,float,C3DVector*,bool);}; //006CB6A0,006CBA80,006CB6E0
struct CInterfaceMap {struct Entry {long type;void* value;};Entry* first;Entry* last;Entry* LowerBound(const long*);}; //0040F020
struct CThingRetail {unsigned char beforeComponents[0x20];unsigned long UsingComponents[9];CInterfaceMap Components;
 const C3DVector& GetPosition();float GetHeightZBelowAt(const C3DVector&,bool,bool); //004C73D0,00666D40
 __forceinline bool GetNavigation(CNavigationMethods** out){if(UsingComponents[0]&128){long key=7;CInterfaceMap* map=&Components;CInterfaceMap::Entry* it=map->LowerBound(&key);if(it==map->last||7<it->type)it=map->last;*out=(CNavigationMethods*)it->value;return true;}return false;}
};
struct CWorldMapMethods {
virtual void s00();virtual void s01();virtual void s02();virtual void s03();virtual void s04();virtual void s05();virtual void s06();virtual void s07();virtual void s08();virtual void s09();virtual void s10();virtual void s11();virtual void s12();virtual void s13();virtual void s14();virtual void s15();virtual long GetMapNumberAt(const C3DVector&)const;float GetGroundSizeZAt(const C3DVector&);}; //slot16=004FC0C0,0063A390
extern unsigned long FlyingNavigatorType; //0129CB44
extern const C3DVector HeightProbeOffset; //0138C500
struct FlyingMethods:CAIStateGroupBase {float MinHeightToKeepAboveGround;float MinHeightToKeepAboveGroundInFlight;float MaxHeightAboveGround;float FlightRange;
 CWorldMapMethods* PeekWorldMap(); //008FCFF0
 bool GetRandomFlightPos(const C3DVector&,float,C3DVector*,bool);
};
bool FlyingMethods::GetRandomFlightPos(const C3DVector& from,float radius,C3DVector* out,bool allowGround){
 if(PeekWorldMap()->GetMapNumberAt(from)!=PeekWorldMap()->GetMapNumberAt(((CThingRetail*)PBrain->Creature)->GetPosition()))return false;
 CNavigationMethods* navigation;
 ((CThingRetail*)PBrain->Creature)->GetNavigation(&navigation);
 unsigned long oldType=navigation->GetNavigatorType();navigation->SetNavigatorToType(FlyingNavigatorType);
 if(!navigation->GetNearbyWanderPosition(from,radius,out,true))return false;
 navigation->SetNavigatorToType(oldType);
 C3DVector probe;probe.X=HeightProbeOffset.X+out->X;probe.Y=HeightProbeOffset.Y+out->Y;probe.Z=HeightProbeOffset.Z+out->Z;
 CThingRetail* creature=(CThingRetail*)PBrain->Creature;
 out->Z=creature->GetHeightZBelowAt(probe,true,true)+MinHeightToKeepAboveGround;
 float ground=PeekWorldMap()->GetGroundSizeZAt(*out);if(ground>out->Z)out->Z=ground;
 return true;
}

unsigned long FlyingNavigatorType=4;
const C3DVector HeightProbeOffset={0,0,25};
static int errors,mapCalls,navGets,sets,wanderCalls,heightCalls,groundCalls,searchCalls;
static bool mapsSame,wanderSuccess;
static unsigned long navigatorType;
static float heightValue,groundValue;
static C3DVector creaturePos={1,2,3},fromPos={10,20,30},generated={40,50,60};
static CThingRetail* expectedCreature;
static CNavigationMethods* expectedNav;
static C3DVector* expectedOut;
static float expectedRadius;
static CWorldMapMethods map;
CWorldMapMethods* FlyingMethods::PeekWorldMap(){return &map;}
const C3DVector& CThingRetail::GetPosition(){if(this!=expectedCreature)++errors;return creaturePos;}
CInterfaceMap::Entry* CInterfaceMap::LowerBound(const long* key){++searchCalls;if(*key!=7)++errors;Entry* p=first;while(p!=last&&p->type<*key)++p;return p;}
unsigned long CNavigationMethods::GetNavigatorType(){++navGets;if(this!=expectedNav)++errors;return navigatorType;}
void CNavigationMethods::SetNavigatorToType(unsigned long t){++sets;if(this!=expectedNav)++errors;if((sets==1&&t!=4)||(sets==2&&t!=9))++errors;navigatorType=t;}
bool CNavigationMethods::GetNearbyWanderPosition(const C3DVector& p,float radius,C3DVector* out,bool flag){++wanderCalls;if(this!=expectedNav||&p!=&fromPos||radius!=expectedRadius||out!=expectedOut||!flag||navigatorType!=4)++errors;if(wanderSuccess)*out=generated;return wanderSuccess;}
float CThingRetail::GetHeightZBelowAt(const C3DVector& p,bool a,bool b){++heightCalls;if(this!=expectedCreature||p.X!=generated.X||p.Y!=generated.Y||p.Z!=generated.Z+25||!a||!b||navigatorType!=9)++errors;return heightValue;}
float CWorldMapMethods::GetGroundSizeZAt(const C3DVector& p){++groundCalls;if(this!=&map||&p!=expectedOut)++errors;return groundValue;}
void CWorldMapMethods::s00(){}
void CWorldMapMethods::s01(){}
void CWorldMapMethods::s02(){}
void CWorldMapMethods::s03(){}
void CWorldMapMethods::s04(){}
void CWorldMapMethods::s05(){}
void CWorldMapMethods::s06(){}
void CWorldMapMethods::s07(){}
void CWorldMapMethods::s08(){}
void CWorldMapMethods::s09(){}
void CWorldMapMethods::s10(){}
void CWorldMapMethods::s11(){}
void CWorldMapMethods::s12(){}
void CWorldMapMethods::s13(){}
void CWorldMapMethods::s14(){}
void CWorldMapMethods::s15(){}
long CWorldMapMethods::GetMapNumberAt(const C3DVector& p)const{++mapCalls;if(this!=&map)++errors;if(&p==&creaturePos)return 2;if(&p!=&fromPos)++errors;return mapsSame?2:3;}
int main(){
 CThingRetail creature;CNavigationMethods nav;CAIBrain brain;FlyingMethods group;CInterfaceMap::Entry entries[2];
 expectedCreature=&creature;expectedNav=&nav;brain.Creature=(CThingAICreature*)&creature;group.PBrain=&brain;group.MinHeightToKeepAboveGround=7;
 for(int i=0;i<9;++i)creature.UsingComponents[i]=0;creature.UsingComponents[0]=128;creature.Components.first=entries;creature.Components.last=entries+1;entries[0].type=7;entries[0].value=&nav;entries[1].type=999;entries[1].value=&nav;
 for(int scenario=0;scenario<6;++scenario){
  mapCalls=navGets=sets=wanderCalls=heightCalls=groundCalls=searchCalls=0;navigatorType=9;
  mapsSame=scenario!=0;wanderSuccess=scenario!=1;heightValue=10;groundValue=scenario==2?25:scenario==3?17:5;expectedRadius=11.5f;
  entries[0].type=scenario==5?8:7;
  C3DVector result={-1,-2,-3};expectedOut=&result;
  bool ok=group.GetRandomFlightPos(fromPos,expectedRadius,&result,(scenario&1)!=0);
  if(mapCalls!=2||ok!=(scenario>1))++errors;
  if(scenario==0){if(sets||navGets||wanderCalls||heightCalls||groundCalls||searchCalls||result.Z!=-3)++errors;}
  else if(scenario==1){if(sets!=1||navigatorType!=4||wanderCalls!=1||heightCalls||groundCalls||result.Z!=-3)++errors;}
  else {float want=groundValue>17?groundValue:17;if(sets!=2||navigatorType!=9||navGets!=1||searchCalls!=1||wanderCalls!=1||heightCalls!=1||groundCalls!=1||result.X!=40||result.Y!=50||result.Z!=want)++errors;}
 }
 if(errors){std::printf("00912700_TEST FAIL %d\n",errors);return 1;}std::printf("00912700_TEST PASS\n");return 0;
}