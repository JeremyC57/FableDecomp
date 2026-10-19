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

#include <cstdio>
static int errors,scenario,checks,moves,routes,terminates,itemGets,destroyed;
static CQueenThing marker,enemy;
static C3DVector markerPos={10,20,30},enemyPos={40,50,60};
CQueenPointer::~CQueenPointer(){++destroyed;}
CQueenThing* CQueenPointer::GetPItem()const{++itemGets;return item;}
const C3DVector& CQueenThing::GetPosition(){if(this==&marker)return markerPos;if(this!=&enemy)++errors;return enemyPos;}
CQueenNative::~CQueenNative(){++destroyed;}
void CQueenNative::slot1(){}
void CQueenNative::slot2(){}
void CQueenNative::slot3(){}
void CQueenNative::slot4(){}
void CQueenNative::slot5(){}
void CQueenNative::slot6(){}
void CQueenNative::slot7(){}
void CQueenNative::slot8(){}
void CQueenNative::slot9(){}
void CQueenNative::slot10(){}
void CQueenNative::slot11(){}
void CQueenNative::slot12(){}
void CQueenNative::slot13(){}
void CQueenNative::slot14(){}
void CQueenNative::slot15(){}
void CQueenNative::slot16(){}
void CQueenNative::slot17(){}
void CQueenNative::slot18(){}
void CQueenNative::slot19(){}
void CQueenNative::slot20(){}
void CQueenNative::slot21(){}

bool CQueenNative::IsStillRunnable(){++checks;return scenario!=1 && !(scenario==2&&checks==2);}
bool CQueenNative::IsRouteToPosBlockedByTarget(long n){++routes;if(n!=1)++errors;return scenario==3||scenario==4;}
void CQueenNative::Terminate(){++terminates;}
long CQueenNative::MoveTo(long kind,const C3DVector& p,const C3DVector& q,bool* arrived,float tolerance){
++moves;if(kind!=2||p.X!=10||p.Y!=20||p.Z!=30||&q!=&enemyPos||tolerance!=2.5f)++errors;*arrived=true;
if(scenario==3)return 4;if(scenario==4)return 1;if(scenario==5&&moves<3)return 4;if(scenario==6)return 1;return 2;
}
int main(){
{
CQueenPointer markers[2];markers[0].item=&enemy;markers[1].item=&marker;
CQueenNative group;group.target.item=&enemy;group.first=markers;group.capacity=markers+2;
for(scenario=0;scenario<7;++scenario){checks=moves=routes=terminates=itemGets=0;group.last=scenario==0?markers:markers+2;group.CurrentCirclePoint=-1;
long r=group.GotoCirclePoint(1,2.5f);
long expected=scenario==0||scenario==3||scenario==4?3:scenario==6?1:2;
if(r!=expected)++errors;
if(scenario==0){if(checks||moves||routes||terminates||itemGets)++errors;}
else if(scenario==1){if(checks!=1||moves||routes||terminates!=1||itemGets)++errors;}
else if(scenario==2){if(checks!=2||moves!=1||routes||terminates!=1||itemGets!=2)++errors;}
else {int count=scenario==5?3:1;if(checks!=count+1||moves!=count||routes!=count||terminates||itemGets!=count+1)++errors;}
if(group.CurrentCirclePoint!=(scenario==5?1:-1))++errors;
}
}
if(destroyed!=4)++errors;
if(errors){std::printf("0095a6c0_TEST FAIL %d\n",errors);return 1;}std::printf("0095a6c0_TEST PASS\n");return 0;
}