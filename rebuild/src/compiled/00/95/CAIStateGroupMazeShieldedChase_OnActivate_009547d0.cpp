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
class CThing { public:
    const C3DVector& GetPos() const; //004c73d0
};
struct MazeThing : public CThing {char base[0x20];unsigned long masks[9];MazeTable table;char beforeFlags[0x20];unsigned char flags;char beforeDead[0x24];unsigned char dead;
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
if(boss->GetPhase()==0)boss->Home=Creature()->GetPos();
MazeThing* target=Target().GetPItem();MazeCombat* combat;Creature()->GetCombat(&combat);
combat->target=MazeCreaturePointer(target);
if(Target().GetPItem()->flags&8){MazeThing* opponent=Target().GetPItem();MazeThing* actor=Creature();CWorld* world=GetPWorld();world->PThingCombatManager_ptr->RegisterCombatant(actor,opponent,true);}
}