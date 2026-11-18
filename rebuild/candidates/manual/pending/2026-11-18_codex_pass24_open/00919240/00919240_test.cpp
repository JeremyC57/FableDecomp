#include <vector>
#include <cstdio>
#include <cstdlib>
#include <limits>
static void check(bool b){if(!b){std::puts("FAIL");std::exit(1);}}
struct C2DCoordI{long X,Y;C2DCoordI(long x,long y):X(x),Y(y){}};
struct C3DVector{float X,Y,Z;C2DCoordI ToC2DCoordI()const{return C2DCoordI((long)X,(long)Y);}};
struct C2DBoxI{long TLX,TLY,BRX,BRY;C2DBoxI(const C2DCoordI&p,long r):TLX(p.X-r),TLY(p.Y-r),BRX(p.X+r),BRY(p.Y+r){}};
struct CInterestingToVillagersDef{float Interest;};template<class T>struct CDefPointer{T*p;T*operator->()const{return p;}};
struct CTCInterestingToVillagers{CDefPointer<const CInterestingToVillagersDef>PDef;};
struct CThing{int id;C3DVector pos;const C3DVector&GetPos()const{return pos;}};
static std::vector<int>visits;
struct CThingGameObject:CThing{CTCInterestingToVillagers*component;bool GetTC(CTCInterestingToVillagers**out){visits.push_back(id);*out=component;return component!=0;}};
struct CThingAICreature:CThing{};
enum ETCInterfaceType{TCI_INTERESTING_TO_VILLAGERS=179};
struct CThingFilter_HasTC{ETCInterfaceType Type;CThingFilter_HasTC(ETCInterfaceType t):Type(t){}};
struct CThingSearchTools{std::vector<CThingGameObject*>found;mutable int calls;mutable long box[4];template<class T,class Container,class Filter>void GetAllThingsInRoughArea(const C2DBoxI&a,const Filter&f,Container&out)const{check(f.Type==TCI_INTERESTING_TO_VILLAGERS);++calls;box[0]=a.TLX;box[1]=a.TLY;box[2]=a.BRX;box[3]=a.BRY;for(unsigned i=0;i<found.size();++i)out.push_back(static_cast<T*>(found[i]));}};
struct CWorld{static long frame;static long GetFrame(){return frame;}};long CWorld::frame;
struct CMainGameComponent{static long fps;static long GetConstantFPS(){return fps;}};long CMainGameComponent::fps;
static unsigned long GFRandom(unsigned long n,unsigned long*seed){unsigned long v=(*seed*0x24a1+0x24df)&0xffffffffUL;*seed=((v>>13)|(v<<19))&0xffffffffUL;return n?*seed%n:0;}
static float selected,randomRange;static unsigned long*randomSeedArg;static int floatCalls;
static float GFFloatRandom(float range,unsigned long*p){randomRange=range;randomSeedArg=p;++floatCalls;return selected;}
struct CAIStateGroup_LookAtInterestingThings{CThingAICreature*creature;CThingSearchTools*tools;unsigned long seed;long FrameToLook;CThingAICreature&GetCreature(){return *creature;}const CThingSearchTools&PeekThingSearchTools()const{return *tools;}unsigned long*GetPWorldSeed(){return &seed;}CThing*FindInterestingThing();};
CThing* CAIStateGroup_LookAtInterestingThings::FindInterestingThing()
{
 std::vector<CThingGameObject*> objects;
 PeekThingSearchTools().GetAllThingsInRoughArea<CThingGameObject>(C2DBoxI(GetCreature().GetPos().ToC2DCoordI(),10),CThingFilter_HasTC(TCI_INTERESTING_TO_VILLAGERS),objects);
 if(objects.empty()) {
  unsigned long random=GFRandom(120,GetPWorldSeed());
  long fps=CMainGameComponent::GetConstantFPS();
  FrameToLook=CWorld::GetFrame()+(random+240)*fps;
  return 0;
 }
 float total_interest=0.0f;
 for(long n=0;n<objects.size();++n) {
  CTCInterestingToVillagers* interesting;
  objects[n]->GetTC(&interesting);
  total_interest+=interesting->PDef->Interest;
 }
 float chosen_interest=GFFloatRandom(total_interest,GetPWorldSeed());
 total_interest=0.0f;
 for(long n=0;n<objects.size();++n) {
  CTCInterestingToVillagers* interesting;
  objects[n]->GetTC(&interesting);
  total_interest+=interesting->PDef->Interest;
  if(chosen_interest<=total_interest)return objects[n];
 }
 return 0;
}

int main(){unsigned long cases=0;
 for(int count=0;count<5;++count)for(int profile=0;profile<256;++profile)for(int pick=0;pick<8;++pick)for(int position=0;position<4;++position)for(int seedCase=0;seedCase<4;++seedCase){
 CThingAICreature creature;const float ps[]={-16.5f,-0.5f,0.0f,8.5f};creature.pos.X=ps[position];creature.pos.Y=ps[3-position];creature.pos.Z=128.0f;CInterestingToVillagersDef defs[4];CTCInterestingToVillagers comps[4];CThingGameObject objs[4];CThingSearchTools tools;tools.calls=0;float total=0;for(int i=0;i<count;++i){const float w[]={0.0f,1.0f,2.0f,4.0f};defs[i].Interest=w[(profile>>(i*2))&3];comps[i].PDef.p=&defs[i];objs[i].id=i;objs[i].component=&comps[i];tools.found.push_back(&objs[i]);total+=defs[i].Interest;}
 CAIStateGroup_LookAtInterestingThings g;g.creature=&creature;g.tools=&tools;const unsigned long seeds[]={0,1,0x80000000UL,0xffffffffUL};g.seed=seeds[seedCase];g.FrameToLook=-1;CWorld::frame=1024;CMainGameComponent::fps=seedCase&1?32:16;selected=pick==0?-1.0f:pick==1?0.0f:pick==2?1.0f:pick==3?total*0.5f:pick==4?total:pick==5?total+1.0f:pick==6?std::numeric_limits<float>::quiet_NaN():std::numeric_limits<float>::infinity();floatCalls=0;randomRange=-1;randomSeedArg=0;visits.clear();CThing*actual=g.FindInterestingThing();
 check(tools.calls==1);check(tools.box[0]==long(creature.pos.X)-10&&tools.box[1]==long(creature.pos.Y)-10&&tools.box[2]==long(creature.pos.X)+10&&tools.box[3]==long(creature.pos.Y)+10);CThing*expected=0;std::vector<int>order;
 if(count){for(int i=0;i<count;++i)order.push_back(i);float cumulative=0;for(int i=0;i<count;++i){order.push_back(i);cumulative+=defs[i].Interest;if(selected<=cumulative){expected=&objs[i];break;}}check(floatCalls==1&&randomRange==total&&randomSeedArg==&g.seed&&g.seed==seeds[seedCase]&&g.FrameToLook==-1);}else{unsigned long v=(seeds[seedCase]*0x24a1+0x24df)&0xffffffffUL;unsigned long next=((v>>13)|(v<<19))&0xffffffffUL;check(g.seed==next&&g.FrameToLook==1024+long((next%120+240)*CMainGameComponent::fps)&&floatCalls==0);}
 check(actual==expected&&visits==order&&tools.found.size()==unsigned(count));++cases;
 }check(cases==163840);std::puts("00919240_TEST PASS");return 0;}
