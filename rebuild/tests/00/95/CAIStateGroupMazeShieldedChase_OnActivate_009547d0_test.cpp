// CAIStateGroup_MazeShieldedChase::OnActivate() -- save boss home, set combat target and register a creature opponent.
// Retail009547d0 length294. Shared group/base/brain headers; native boss Home22 is unaligned (packed layout), combat target40 and thing flags6c.
#ifdef _MSC_VER
#include "engine/CAIStateGroup_MazeShieldedChase.h"
#include "engine/CAIStateGroupBase.h"
#include "engine/CAIBrain.h"
#include "engine/CWorld.h"
#else
#define __forceinline inline
class CAIBrain{ public:char prefix[24];void* Creature;};
class CAIStateGroupBase{ public:void* vtable;CAIBrain* PBrain;char state[12];};
struct CAIStateGroup_MazeShieldedChase:CAIStateGroupBase{void* _pad_0x14[2];char unused[4];long unusedFrame;};
#endif
struct MazeThing;
struct MazeCreaturePointerBase{virtual ~MazeCreaturePointerBase();MazeThing* item;MazeCreaturePointerBase();MazeThing* GetPItem()const;void SetPItem(MazeThing*);}; //00a01c10,00a01b10,00a01b50,00a01b90
struct MazeCreaturePointer:MazeCreaturePointerBase{__forceinline MazeCreaturePointer(MazeThing* p){SetPItem(p);}__forceinline MazeCreaturePointer& operator=(const MazeCreaturePointer& p){SetPItem(p.GetPItem());return *this;}}; //derived scalar destructor004d6915,vtable0123e7d8
class C3DVector{ public:float X,Y,Z;};
#pragma pack(push,1)
struct MazeBoss{char beforeHome[0x22];C3DVector Home;long GetPhase();};
#pragma pack(pop) //007aed10
struct MazeCombat{char base[0x40];MazeCreaturePointer target;};
struct MazeEntry{long key;void* value;};
struct MazeTable{MazeEntry* begin;MazeEntry* end;MazeEntry* Find(const long&);MazeEntry* FindConst(const long&); //0040f020,004365b0
 template<int Key> __forceinline void* Lookup(){long key=Key;MazeEntry* p=Find(key);if(p==end)p=end;else if(p->key>Key)p=end;return p->value;}
 template<int Key> __forceinline void* LookupConst(){long key=Key;MazeEntry* p=FindConst(key);return (p==end || p->key>Key ? end:p)->value;}
};
struct MazeThing{char base[0x20];unsigned long masks[9];MazeTable table;char beforeFlags[0x20];unsigned char flags;char beforeDead[0x24];unsigned char dead;
const C3DVector& GetPosition(); //004c73d0
__forceinline void GetBoss(MazeBoss** out){if(masks[5]&0x2000)*out=(MazeBoss*)table.Lookup<173>();}
__forceinline void GetCombat(MazeCombat** out){if(masks[0]&0x100)*out=(MazeCombat*)table.Lookup<8>();}
};
class CCombatManager{ public:void RegisterCombatant(MazeThing*,MazeThing*,bool);}; //006edc50
#ifndef _MSC_VER
class CWorld{ public:char prefix[0x4c];CCombatManager* PThingCombatManager_ptr;};
#endif
struct ShieldMethods:CAIStateGroup_MazeShieldedChase{
__forceinline MazeThing* Creature(){return (MazeThing*)((CAIStateGroupBase*)this)->PBrain->Creature;}
__forceinline MazeCreaturePointer& Target(){return *(MazeCreaturePointer*)((CAIStateGroupBase*)this+1);}
CWorld* GetPWorld(); //008fcfc0
void OnActivate();};
void ShieldMethods::OnActivate(){
MazeBoss* boss;Creature()->GetBoss(&boss);
if(boss->GetPhase()==0)boss->Home=Creature()->GetPosition();
MazeThing* target=Target().GetPItem();MazeCombat* combat;Creature()->GetCombat(&combat);
combat->target=MazeCreaturePointer(target);
if(Target().GetPItem()->flags&8){MazeThing* opponent=Target().GetPItem();MazeThing* actor=Creature();CWorld* world=GetPWorld();world->PThingCombatManager_ptr->RegisterCombatant(actor,opponent,true);}
}

#include <cstdio>
#include <new>
static int errors,scenario,ctors,dtors,sets,pointerGets,positionCalls,worldCalls,registrations,phaseCalls;
static ShieldMethods* group;static MazeThing actor,target;static CCombatManager manager;static CWorld world;static MazeBoss boss;static C3DVector position={2,3,4};
MazeCreaturePointerBase::MazeCreaturePointerBase():item(0){++ctors;}
MazeCreaturePointerBase::~MazeCreaturePointerBase(){++dtors;}
MazeThing* MazeCreaturePointerBase::GetPItem()const{++pointerGets;return item;}
void MazeCreaturePointerBase::SetPItem(MazeThing* p){++sets;item=p;}
long MazeBoss::GetPhase(){++phaseCalls;if(this!=&boss)++errors;return scenario&1;}
const C3DVector& MazeThing::GetPosition(){++positionCalls;if(this!=&actor)++errors;return position;}
MazeEntry* MazeTable::Find(const long& key){if(this!=&actor.table)++errors;return key==173?begin:begin+1;}
CWorld* ShieldMethods::GetPWorld(){++worldCalls;if(dtors!=1||sets!=2)++errors;return &world;}
void CCombatManager::RegisterCombatant(MazeThing* a,MazeThing* b,bool force){++registrations;if(this!=&manager||a!=&actor||b!=&target||!force||worldCalls!=1)++errors;}
int main(){ShieldMethods g;group=&g;CAIBrain brain;
#ifdef _MSC_VER
((CAIStateGroupBase*)&g)->PBrain=&brain;brain.Creature=(CThingAICreature*)&actor;
#else
g.PBrain=&brain;brain.Creature=&actor;
#endif
union Storage{void* alignment;char bytes[sizeof(MazeCombat)];} storage;
MazeCombat* combat=(MazeCombat*)storage.bytes;MazeCreaturePointer* combatTarget=new(&combat->target) MazeCreaturePointer(0);MazeCreaturePointer* saved=new(&g.Target()) MazeCreaturePointer(&target);
MazeEntry entries[3];entries[0].key=173;entries[0].value=&boss;entries[1].key=8;entries[1].value=combat;actor.table.begin=entries;actor.table.end=entries+2;actor.masks[5]=0x2000;actor.masks[0]=0x100;world.PThingCombatManager_ptr=&manager;
for(scenario=0;scenario<4;++scenario){ctors=dtors=sets=pointerGets=positionCalls=worldCalls=registrations=phaseCalls=0;target.flags=scenario>=2?8:0;boss.Home.X=9;boss.Home.Y=8;boss.Home.Z=7;g.OnActivate();bool registered=scenario>=2;
if(phaseCalls!=1||ctors!=1||dtors!=1||sets!=2||pointerGets!=3+registered||positionCalls!=(!(scenario&1))||worldCalls!=registered||registrations!=registered||combatTarget->item!=&target||boss.Home.X!=((scenario&1)?9:2)||boss.Home.Y!=((scenario&1)?8:3)||boss.Home.Z!=((scenario&1)?7:4))++errors;}
saved->~MazeCreaturePointer();combatTarget->~MazeCreaturePointer();if(errors){std::printf("009547d0_TEST FAIL %d\n",errors);return 1;}std::printf("009547d0_TEST PASS\n");return 0;}