// CAIStateGroup_MazeTutorialBattleRespondToLightning::IsRunnable() -- choose a living lightning target outside boss phase2 and save hit time.
// Retail00954e30 length262. Shared group/base/brain headers; native component map44,dead91.
#ifdef _MSC_VER
#include "engine/CAIStateGroup_MazeTutorialBattleRespondToLightning.h"
#include "engine/CAIStateGroupBase.h"
#include "engine/CAIBrain.h"
#else
#define __forceinline inline
class CAIBrain{ public:char prefix[24];void* Creature;};
class CAIStateGroupBase{ public:void* vtable;CAIBrain* PBrain;char state[12];};
struct CAIStateGroup_MazeTutorialBattleRespondToLightning:CAIStateGroupBase{void* _pad_0x14[2];char unused[4];long FrameOfLastHit;};
#endif
struct MazeThing;
struct MazePointerBase{virtual ~MazePointerBase();MazeThing* item;MazePointerBase();MazeThing* GetPItem()const;void SetPItem(MazeThing*);}; //00a01c10,00a01b10,00a01b50,00a01b90
struct MazePointer:MazePointerBase{__forceinline MazePointer(MazeThing* p){SetPItem(p);}__forceinline MazePointer& operator=(const MazePointer& p){SetPItem(p.GetPItem());return *this;}}; //derived scalar destructor004a9710,vtable01238c6c
struct MazeBoss{long GetPhase();}; //007aed10
struct MazeCombat{virtual ~MazeCombat();
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
virtual MazeThing* GetTarget(); //slot29
};
struct MazeEntry{long key;void* value;};
struct MazeTable{MazeEntry* begin;MazeEntry* end;MazeEntry* Find(const long&);MazeEntry* FindConst(const long&); //0040f020,004365b0
 template<int Key> __forceinline void* Lookup(){long key=Key;MazeEntry* p=Find(key);if(p==end)p=end;else if(p->key>Key)p=end;return p->value;}
 template<int Key> __forceinline void* LookupConst(){long key=Key;MazeEntry* p=FindConst(key);return (p==end || p->key>Key ? end:p)->value;}
};
struct MazeThing{char base[0x20];unsigned long masks[9];MazeTable table;char beforeDead[0x45];unsigned char dead;
__forceinline void GetBoss(MazeBoss** out){if(masks[5]&0x2000)*out=(MazeBoss*)table.Lookup<173>();}
__forceinline void GetCombat(MazeCombat** out){if(masks[0]&0x100)*out=(MazeCombat*)table.LookupConst<8>();}
};
struct MazeWorld{static long GetWorldFrame();}; //0049d870
struct LightningMethods:CAIStateGroup_MazeTutorialBattleRespondToLightning{
__forceinline MazeThing* Creature(){return (MazeThing*)((CAIStateGroupBase*)this)->PBrain->Creature;}
bool HasBeenHitByLightning(); //00954c60
MazeWorld* GetPWorld(); //008fcfc0
bool IsRunnable();};
bool LightningMethods::IsRunnable(){
MazeBoss* boss;Creature()->GetBoss(&boss);if(boss->GetPhase()==2)return false;
MazeCombat* combat;Creature()->GetCombat(&combat);MazeThing* target=combat->GetTarget();
if(target&&!(target->dead&1)&&HasBeenHitByLightning()){
FrameOfLastHit=GetPWorld()->GetWorldFrame();
*(MazePointer*)_pad_0x14=MazePointer(target);
return true;}
return false;
}

#include <cstdio>
#include <new>
static int errors,scenario,ctors,dtors,sets,pointerGets,phaseCalls,targetCalls,hitCalls,worldCalls,frameCalls;
static LightningMethods* group;static MazeThing actor,target,oldTarget;static MazeWorld world;static MazeBoss boss;
MazePointerBase::MazePointerBase():item(0){++ctors;}
MazePointerBase::~MazePointerBase(){++dtors;}
MazeThing* MazePointerBase::GetPItem()const{++pointerGets;return item;}
void MazePointerBase::SetPItem(MazeThing* p){++sets;item=p;}
long MazeBoss::GetPhase(){++phaseCalls;if(this!=&boss)++errors;return scenario==0?2:1;}
MazeCombat::~MazeCombat(){}
void MazeCombat::slot1(){}
void MazeCombat::slot2(){}
void MazeCombat::slot3(){}
void MazeCombat::slot4(){}
void MazeCombat::slot5(){}
void MazeCombat::slot6(){}
void MazeCombat::slot7(){}
void MazeCombat::slot8(){}
void MazeCombat::slot9(){}
void MazeCombat::slot10(){}
void MazeCombat::slot11(){}
void MazeCombat::slot12(){}
void MazeCombat::slot13(){}
void MazeCombat::slot14(){}
void MazeCombat::slot15(){}
void MazeCombat::slot16(){}
void MazeCombat::slot17(){}
void MazeCombat::slot18(){}
void MazeCombat::slot19(){}
void MazeCombat::slot20(){}
void MazeCombat::slot21(){}
void MazeCombat::slot22(){}
void MazeCombat::slot23(){}
void MazeCombat::slot24(){}
void MazeCombat::slot25(){}
void MazeCombat::slot26(){}
void MazeCombat::slot27(){}
void MazeCombat::slot28(){}

MazeThing* MazeCombat::GetTarget(){++targetCalls;return scenario==1?0:&target;}
MazeEntry* MazeTable::Find(const long& key){if(this!=&actor.table||key!=173)++errors;return begin;}
MazeEntry* MazeTable::FindConst(const long& key){if(this!=&actor.table||key!=8)++errors;return begin+1;}
bool LightningMethods::HasBeenHitByLightning(){++hitCalls;return scenario!=3;}
MazeWorld* LightningMethods::GetPWorld(){++worldCalls;return &world;}
long MazeWorld::GetWorldFrame(){++frameCalls;return 12345;}
int main(){LightningMethods g;group=&g;CAIBrain brain;MazeCombat combat;
#ifdef _MSC_VER
((CAIStateGroupBase*)&g)->PBrain=&brain;brain.Creature=(CThingAICreature*)&actor;
#else
g.PBrain=&brain;brain.Creature=&actor;
#endif
MazeEntry entries[3];entries[0].key=173;entries[0].value=&boss;entries[1].key=8;entries[1].value=&combat;actor.table.begin=entries;actor.table.end=entries+2;actor.masks[5]=0x2000;actor.masks[0]=0x100;
MazePointer* saved=new(g._pad_0x14) MazePointer(&oldTarget);
for(scenario=0;scenario<5;++scenario){ctors=dtors=sets=pointerGets=phaseCalls=targetCalls=hitCalls=worldCalls=frameCalls=0;saved->item=&oldTarget;target.dead=scenario==2?1:0;g.FrameOfLastHit=99;bool result=g.IsRunnable();bool success=scenario==4;
if(result!=success||phaseCalls!=1||targetCalls!=(scenario!=0)||hitCalls!=(scenario>=3)||worldCalls!=success||frameCalls!=success||ctors!=success||dtors!=success||sets!=2*success||pointerGets!=success||saved->item!=(success?&target:&oldTarget)||g.FrameOfLastHit!=(success?12345:99))++errors;}
saved->~MazePointer();if(errors){std::printf("00954e30_TEST FAIL %d\n",errors);return 1;}std::printf("00954e30_TEST PASS\n");return 0;}