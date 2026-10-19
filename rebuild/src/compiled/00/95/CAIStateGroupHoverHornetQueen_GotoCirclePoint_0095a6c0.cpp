// CAIStateGroup_HoverHornetQueen::GotoCirclePoint(long,float) -- move to a circle marker while checking the route and target.
// Retail0095a6c0 length259. Donor header places CurrentCirclePoint at50 and CircleMarkers54;
// retail uses4c and vector begin/end50/54, so a native method/layout view is required.
#ifdef _MSC_VER
#include "engine/CAIStateGroup_HoverHornetQueen.h"
#endif
struct C3DVector {float X,Y,Z;};
struct CQueenThing {const C3DVector& GetPosition();}; //004c73d0
struct CQueenPointer {virtual ~CQueenPointer();CQueenThing* item;CQueenThing* GetPItem()const;}; //00a01b50
struct CQueenNative {
virtual ~CQueenNative();
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
virtual long MoveTo(long,const C3DVector&,const C3DVector&,bool*,float); //slot22
void* brain;unsigned char baseState[12];CQueenPointer target;
C3DVector StartPos,ArenaCentrePos;bool bOneTime;unsigned char alignment[3];
CQueenThing* PEmitter;CQueenThing* PEmitter2;unsigned char Things[12];
long CurrentCirclePoint;CQueenPointer* first;CQueenPointer* last;CQueenPointer* capacity;
bool IsStillRunnable(); //0095a420
bool IsRouteToPosBlockedByTarget(long); //0095a490
void Terminate(); //008fcf40
long GotoCirclePoint(long,float);
};
long CQueenNative::GotoCirclePoint(long index,float tolerance){
if((unsigned long)(last-first)==0)return 3;
if(!IsStillRunnable()){Terminate();return 2;}
CQueenPointer* markers=first;
C3DVector point=markers[index].GetPItem()->GetPosition();
long result;bool blocked;bool arrived;
do{
result=MoveTo(2,point,target.GetPItem()->GetPosition(),&arrived,tolerance);
if(!IsStillRunnable()){Terminate();return 2;}
blocked=IsRouteToPosBlockedByTarget(index);
}while(result==4&&!blocked);
if(blocked)result=3;
if(result==2)CurrentCirclePoint=index;
return result;
}