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
class CWorld { public: static long __fastcall GetFrame(); }; //0049d870
struct DragonMethods:CAIStateGroup_JackDragon{
CWorld* GetPWorld();void Terminate();bool IsStopping();long AIAction_DoNothing(); //008fcfc0,008fcf40,008fd010,0097e200
__forceinline CDragonThing* Creature(){return (CDragonThing*)((CAIStateGroupBase*)this)->PBrain->Creature;}
__forceinline CDragonPointer& Enemy(){return *(CDragonPointer*)PEnemy;}
void StayIdleAndTurnToFace(float,const C3DVector&);
};
void DragonMethods::StayIdleAndTurnToFace(float duration,const C3DVector& facing){
long start=GetPWorld()->GetFrame();
CRightHandedSet orientation(facing);
do{
AIAction_DoNothing();
orientation=Creature()->Physics->GetFacing().Interpolate(orientation,0.1f);
Creature()->Physics->SetFacingAnglesFromRHSet(orientation);
if(!Enemy().GetPItem()||(Enemy().GetPItem()->dead&1)||IsStopping()){Terminate();return;}
}while((float)(GetPWorld()->GetFrame()-start)/FramesPerSecond<duration);
}