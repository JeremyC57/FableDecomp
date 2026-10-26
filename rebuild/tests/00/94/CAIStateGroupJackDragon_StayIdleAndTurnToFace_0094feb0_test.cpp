// CAIStateGroup_JackDragon::StayIdleAndTurnToFace(float,const C3DVector&) -- interpolate facing while idling until the duration expires or the enemy becomes invalid.
// Retail0094feb0 length311. Shared JackDragon/Base/Brain headers; narrow thing view exposes retail physics60 and dead91.
#ifdef _MSC_VER
#include "engine/CAIStateGroup_JackDragon.h"
#include "engine/CAIStateGroupBase.h"
#include "engine/CAIBrain.h"
#else
class CAIBrain{ public:char prefix[24];void* Creature;};
class CAIStateGroupBase{ public:void* vtable;CAIBrain* PBrain;char state[12];};
struct CAIStateGroup_JackDragon:CAIStateGroupBase{void* PEnemy[2];};
#define __forceinline inline
#endif
class C3DVector{ public:float X,Y,Z;C3DVector(){}C3DVector(float x,float y,float z):X(x),Y(y),Z(z){}};
class CRightHandedSet{ public:C3DVector Up,Forward;CRightHandedSet(const C3DVector& direction){Forward=direction;Up=C3DVector(0,0,1);}CRightHandedSet Interpolate(const CRightHandedSet&,float)const;}; //00a23190
struct CDragonPhysics {
virtual ~CDragonPhysics();
virtual void slot1();
virtual void slot2();
virtual void slot3();
virtual void slot4();
virtual void slot5();
virtual void slot6();
virtual void slot7();
virtual void slot8();
virtual void slot9();
virtual void slot10();
virtual void slot11();
virtual void slot12();
virtual void slot13();
virtual void slot14();
virtual void slot15();
virtual void slot16();
virtual void slot17();
virtual void slot18();
virtual void slot19();
virtual void slot20();
virtual void slot21();
virtual void slot22();
virtual void slot23();
virtual void slot24();
virtual void slot25();
virtual void slot26();
virtual void slot27();
virtual void slot28();
virtual void slot29();
virtual void slot30();
virtual void slot31();
virtual void slot32();
virtual void slot33();
virtual void slot34();
virtual void slot35();
virtual void slot36();
virtual void slot37();
virtual void slot38();
virtual void slot39();
virtual void slot40();
virtual void slot41();
virtual void slot42();
virtual void slot43();
virtual void slot44();
virtual void slot45();
virtual void slot46();
virtual void slot47();
virtual void slot48();
virtual void slot49();
virtual void slot50();
virtual void slot51();
virtual void slot52();
virtual void slot53();
virtual void slot54();
virtual void slot55();
virtual void slot56();
virtual void slot57();
virtual void slot58();
virtual void slot59();
virtual void slot60();
virtual void slot61();
virtual void slot62();
virtual void slot63();
virtual void slot64();
virtual void slot65();
virtual void slot66();
virtual void slot67();
virtual void slot68();
virtual void slot69();
virtual void slot70();
virtual void slot71();
virtual const CRightHandedSet& GetFacing()const; //slot72,00724bd0
virtual void slot73();virtual void slot74();virtual void SetFacingAnglesFromRHSet(const CRightHandedSet&); //slot75,007239c0
};
struct CDragonThing{char prefix[0x60];CDragonPhysics* Physics;char beforeDead[0x2d];unsigned char dead;};
struct CDragonPointer{CDragonThing* GetPItem()const;}; //00a01b50
extern long FramesPerSecond; //01375550
struct CDragonWorld{static long GetWorldFrame();}; //0049d870
struct DragonMethods:CAIStateGroup_JackDragon{
CDragonWorld* GetPWorld();void Terminate();bool IsStopping();long AIAction_DoNothing(); //008fcfc0,008fcf40,008fd010,0097e200
__forceinline CDragonThing* Creature(){return (CDragonThing*)((CAIStateGroupBase*)this)->PBrain->Creature;}
__forceinline CDragonPointer& Enemy(){return *(CDragonPointer*)PEnemy;}
void StayIdleAndTurnToFace(float,const C3DVector&);
};
void DragonMethods::StayIdleAndTurnToFace(float duration,const C3DVector& facing){
long start=GetPWorld()->GetWorldFrame();
CRightHandedSet orientation(facing);
do{
AIAction_DoNothing();
orientation=Creature()->Physics->GetFacing().Interpolate(orientation,0.1f);
Creature()->Physics->SetFacingAnglesFromRHSet(orientation);
if(!Enemy().GetPItem()||(Enemy().GetPItem()->dead&1)||IsStopping()){Terminate();return;}
}while((float)(GetPWorld()->GetWorldFrame()-start)/FramesPerSecond<duration);
}

#include <cstdio>
static int errors,scenario,idles,getters,interpolations,setters,stops,terminates,worlds,frames;
static CDragonThing owner,enemy;
static CDragonThing* target;
static DragonMethods* expectedGroup;
static C3DVector initial(2,3,4);
static CRightHandedSet physicsFacing(initial),expectedInput(initial);
static CDragonWorld world;
long FramesPerSecond=10;
static bool Equal(const CRightHandedSet& a,const CRightHandedSet& b){return a.Up.X==b.Up.X&&a.Up.Y==b.Up.Y&&a.Up.Z==b.Up.Z&&a.Forward.X==b.Forward.X&&a.Forward.Y==b.Forward.Y&&a.Forward.Z==b.Forward.Z;}
CDragonPhysics::~CDragonPhysics(){}
void CDragonPhysics::slot1(){}
void CDragonPhysics::slot2(){}
void CDragonPhysics::slot3(){}
void CDragonPhysics::slot4(){}
void CDragonPhysics::slot5(){}
void CDragonPhysics::slot6(){}
void CDragonPhysics::slot7(){}
void CDragonPhysics::slot8(){}
void CDragonPhysics::slot9(){}
void CDragonPhysics::slot10(){}
void CDragonPhysics::slot11(){}
void CDragonPhysics::slot12(){}
void CDragonPhysics::slot13(){}
void CDragonPhysics::slot14(){}
void CDragonPhysics::slot15(){}
void CDragonPhysics::slot16(){}
void CDragonPhysics::slot17(){}
void CDragonPhysics::slot18(){}
void CDragonPhysics::slot19(){}
void CDragonPhysics::slot20(){}
void CDragonPhysics::slot21(){}
void CDragonPhysics::slot22(){}
void CDragonPhysics::slot23(){}
void CDragonPhysics::slot24(){}
void CDragonPhysics::slot25(){}
void CDragonPhysics::slot26(){}
void CDragonPhysics::slot27(){}
void CDragonPhysics::slot28(){}
void CDragonPhysics::slot29(){}
void CDragonPhysics::slot30(){}
void CDragonPhysics::slot31(){}
void CDragonPhysics::slot32(){}
void CDragonPhysics::slot33(){}
void CDragonPhysics::slot34(){}
void CDragonPhysics::slot35(){}
void CDragonPhysics::slot36(){}
void CDragonPhysics::slot37(){}
void CDragonPhysics::slot38(){}
void CDragonPhysics::slot39(){}
void CDragonPhysics::slot40(){}
void CDragonPhysics::slot41(){}
void CDragonPhysics::slot42(){}
void CDragonPhysics::slot43(){}
void CDragonPhysics::slot44(){}
void CDragonPhysics::slot45(){}
void CDragonPhysics::slot46(){}
void CDragonPhysics::slot47(){}
void CDragonPhysics::slot48(){}
void CDragonPhysics::slot49(){}
void CDragonPhysics::slot50(){}
void CDragonPhysics::slot51(){}
void CDragonPhysics::slot52(){}
void CDragonPhysics::slot53(){}
void CDragonPhysics::slot54(){}
void CDragonPhysics::slot55(){}
void CDragonPhysics::slot56(){}
void CDragonPhysics::slot57(){}
void CDragonPhysics::slot58(){}
void CDragonPhysics::slot59(){}
void CDragonPhysics::slot60(){}
void CDragonPhysics::slot61(){}
void CDragonPhysics::slot62(){}
void CDragonPhysics::slot63(){}
void CDragonPhysics::slot64(){}
void CDragonPhysics::slot65(){}
void CDragonPhysics::slot66(){}
void CDragonPhysics::slot67(){}
void CDragonPhysics::slot68(){}
void CDragonPhysics::slot69(){}
void CDragonPhysics::slot70(){}
void CDragonPhysics::slot71(){}
void CDragonPhysics::slot73(){}
void CDragonPhysics::slot74(){}

const CRightHandedSet& CDragonPhysics::GetFacing()const{++getters;if(getters!=idles||setters!=idles-1)++errors;return physicsFacing;}
CRightHandedSet CRightHandedSet::Interpolate(const CRightHandedSet& rhs,float amount)const{
++interpolations;if(this!=&physicsFacing||amount!=0.1f||!Equal(rhs,expectedInput)||getters!=interpolations)++errors;
CRightHandedSet result=rhs;result.Up.X+=1;result.Up.Y+=2;result.Up.Z+=3;result.Forward.X+=4;result.Forward.Y+=5;result.Forward.Z+=6;expectedInput=result;return result;
}
void CDragonPhysics::SetFacingAnglesFromRHSet(const CRightHandedSet& rhs){++setters;if(setters!=interpolations||!Equal(rhs,expectedInput))++errors;physicsFacing=rhs;}
CDragonThing* CDragonPointer::GetPItem()const{if(this!=(CDragonPointer*)expectedGroup->PEnemy)++errors;return target;}
CDragonWorld* DragonMethods::GetPWorld(){++worlds;return &world;}
long CDragonWorld::GetWorldFrame(){++frames;if(frames!=worlds)++errors;return 100+(frames-1);}
long DragonMethods::AIAction_DoNothing(){++idles;return 4;}
bool DragonMethods::IsStopping(){++stops;return scenario==3;}
void DragonMethods::Terminate(){++terminates;}
int main(){DragonMethods group;CAIBrain brain;CDragonPhysics physics;expectedGroup=&group;owner.Physics=&physics;
#ifdef _MSC_VER
((CAIStateGroupBase*)&group)->PBrain=&brain;brain.Creature=(CThingAICreature*)&owner;
#else
group.PBrain=&brain;brain.Creature=&owner;
#endif
for(scenario=0;scenario<6;++scenario){idles=getters=interpolations=setters=stops=terminates=worlds=frames=0;target=scenario==1?0:&enemy;enemy.dead=scenario==2?1:0;expectedInput=CRightHandedSet(initial);physicsFacing=CRightHandedSet(initial);
float duration=scenario==4?-1.0f:scenario==5?0.0f:0.25f;
group.StayIdleAndTurnToFace(duration,initial);
int loops=scenario==0?3:1;if(idles!=loops||getters!=loops||interpolations!=loops||setters!=loops)++errors;
if(scenario>=1&&scenario<=3){if(terminates!=1||worlds!=1||frames!=1||stops!=(scenario==3))++errors;}
else {if(terminates||worlds!=loops+1||frames!=loops+1||stops!=loops)++errors;}
}
if(errors){std::printf("0094feb0_TEST FAIL %d\n",errors);return 1;}std::printf("0094feb0_TEST PASS\n");return 0;}