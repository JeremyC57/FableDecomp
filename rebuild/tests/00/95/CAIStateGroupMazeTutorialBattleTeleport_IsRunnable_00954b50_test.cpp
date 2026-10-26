// CAIStateGroup_MazeTutorialBattleTeleport::IsRunnable() -- allow teleport after configured cooldown with a living target.
// Retail00954b50 length268. Native CMazeBattleDef TutorialTeleportTime5c differs donor60; definition getter008c3510 names CMazeBattleDef at01243bb8.
#ifdef _MSC_VER
#include "engine/CAIStateGroup_MazeTutorialBattleTeleport.h"
#include "engine/CAIStateGroupBase.h"
#include "engine/CAIBrain.h"
#include "engine/CDefClassBase.h"
#include "engine/CDefinitionManager.h"
#include "engine/_quarantine/CMazeBattleDef.h"
#include "fable_string.h"
#else
#define __forceinline inline
class CCharString{public:CCharString(const char*,long);~CCharString();};
class CAIBrain{ public:char prefix[24];void* Creature;};
class CAIStateGroupBase{ public:void* vtable;CAIBrain* PBrain;char state[12];};
struct CAIStateGroup_MazeTutorialBattleTeleport:CAIStateGroupBase{void* _pad_0x14[2];char unused[4];long FrameOfLastTeleport;};
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
struct CRefCounted {                       // second base: the count sits at +4 behind the vptr
    long m_count;
    void DecRef() { --m_count; }
    long GetRefCount() const { return m_count; }
};

struct CDefVirtual {
    virtual ~CDefVirtual();
    virtual void Destroy();                // slot 1
};

struct CDefBase : CDefVirtual, CRefCounted {
    void Release()                         // CDefPointer<T>: destructor and assignment
    {
        if (--m_count == 0)
            Destroy();
    }
    void ReleaseTemporary()                // CDefPointer<CDefBase>: the manager's temporary
    {
        DecRef();
        if (GetRefCount() == 0)
            Destroy();
    }
};

struct CDefPointerBase {                   // CDefPointer<CDefBase>: the manager's return type
    CDefBase* m_p;
    CDefPointerBase() { }
    CDefPointerBase(const CDefPointerBase& other):m_p(other.m_p){if(m_p)++m_p->m_count;}
    ~CDefPointerBase()
    {
        if (m_p) {
            m_p->ReleaseTemporary();
            m_p = 0;
        }
    }
};

struct CSubDef : CDefBase {                 // native CMazeBattleDef data view
    unsigned char _pad08[0x54];
    float TutorialTeleportTime;                // retail +0x5c, donor +0x60
};

struct CDefPointer {                       // CDefPointer<T>
    CSubDef* m_p;
    CDefPointer() { }
    CDefPointer(const CDefPointerBase& other) : m_p((CSubDef*)other.m_p) { if (m_p) ++m_p->m_count; }
    ~CDefPointer()
    {
        if (m_p)
            m_p->Release();
    }
    CDefPointer& operator=(const CDefPointer& other)
    {
        if (m_p != other.m_p) {
            if (m_p) {
                m_p->Release();
                m_p = 0;
            }
            m_p = other.m_p;
            if (m_p)
                ++m_p->m_count;
        }
        return *this;
    }
};

#ifndef _MSC_VER
class CDefinitionManager { public:};
#endif
struct MazeDefinitionManagerMethods:CDefinitionManager {
    CDefPointerBase GetPDefFromGlobalIndex(long index);   // 0x009ADA10 (hidden result pointer)
};

// Virtual native view of shared CDefClassBase: generated header has a flattened vptr.
// Slot0=0042dab0 is a scalar deleting destructor; PDefManager is shared +0x1c.
struct MazeDefinitionNative {
    virtual ~MazeDefinitionNative();
    virtual void v04();
    virtual void v08();
    virtual void v0c();
    virtual void v10();
    virtual void v14();
    virtual void v18();
    virtual void v1c();
    virtual void v20();
    virtual void v24();
    virtual void v28();
    virtual void v2c();
    virtual void v30();
    virtual void v34();
    virtual long* GetSubDefIndex(const CCharString& name);   // slot 14 (+0x38)
    unsigned char _pad[0x1c - 4];
    CDefinitionManager* PDefManager;        // +0x1C
    bool GetSubDef(CDefPointer& out);          // 0x008C3510 GetSubDef<CMazeBattleDef> (same-TU template body)
};

#pragma auto_inline(off)
bool MazeDefinitionNative::GetSubDef(CDefPointer& out)
{
    long* pIndex;
    {
        CCharString name("CMazeBattleDef", -1);
        pIndex = GetSubDefIndex(name);
    }
    if (pIndex) {
        CDefPointer result(static_cast<MazeDefinitionManagerMethods*>(PDefManager)->GetPDefFromGlobalIndex(*pIndex));
        out = result;
        return true;
    }
    return false;
}
#pragma auto_inline(on)

extern long FramesPerSecond; //01375550
struct MazeThing{char base[0x20];unsigned long masks[9];MazeTable table;char beforeDef[0x24];MazeDefinitionNative* definition;char beforeDead[0x1d];unsigned char dead;
__forceinline void GetBoss(MazeBoss** out){if(masks[5]&0x2000)*out=(MazeBoss*)table.Lookup<173>();}
__forceinline void GetCombat(MazeCombat** out){if(masks[0]&0x100)*out=(MazeCombat*)table.LookupConst<8>();}
};
struct MazeWorld{static long GetWorldFrame();}; //0049d870
struct TeleportMethods:CAIStateGroup_MazeTutorialBattleTeleport{
__forceinline MazeThing* Creature(){return (MazeThing*)((CAIStateGroupBase*)this)->PBrain->Creature;}
bool HasBeenHitByLightning(); //00954c60
MazeWorld* GetPWorld(); //008fcfc0
bool IsRunnable();};
bool TeleportMethods::IsRunnable(){
MazeCombat* combat;Creature()->GetCombat(&combat);MazeThing* target=combat->GetTarget();
if(target&&!(target->dead&1)){
CDefPointer def;def.m_p=0;Creature()->definition->GetSubDef(def);
float elapsed=(float)(GetPWorld()->GetWorldFrame()-FrameOfLastTeleport);
if(elapsed > FramesPerSecond*def.m_p->TutorialTeleportTime){*(MazePointer*)_pad_0x14=MazePointer(target);return true;}
}
return false;
}

#include <cstdio>
#include <cstring>
#include <new>
static int errors,scenario,ctors,dtors,sets,pointerGets,targetCalls,worldCalls,frameCalls,destroys,stringLive,indices,managerCalls;
static MazeThing actor,target,oldTarget;static MazeWorld world;static long now;long FramesPerSecond=10;
static CSubDef definition;static long defIndex=77;static MazeDefinitionManagerMethods manager;
MazePointerBase::MazePointerBase():item(0){++ctors;}
MazePointerBase::~MazePointerBase(){++dtors;}
MazeThing* MazePointerBase::GetPItem()const{++pointerGets;return item;}
void MazePointerBase::SetPItem(MazeThing* p){++sets;item=p;}
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

MazeThing* MazeCombat::GetTarget(){++targetCalls;return scenario==0?0:&target;}
MazeEntry* MazeTable::FindConst(const long& key){if(this!=&actor.table||key!=8)++errors;return begin;}
MazeWorld* TeleportMethods::GetPWorld(){++worldCalls;return &world;}
long MazeWorld::GetWorldFrame(){++frameCalls;return now;}
CDefVirtual::~CDefVirtual(){}
void CDefVirtual::Destroy(){++destroys;}
CCharString::CCharString(const char* name,long length){if(stringLive||std::strcmp(name,"CMazeBattleDef")||length!=-1)++errors;++stringLive;}
CCharString::~CCharString(){--stringLive;}
MazeDefinitionNative::~MazeDefinitionNative(){}
void MazeDefinitionNative::v04(){}
void MazeDefinitionNative::v08(){}
void MazeDefinitionNative::v0c(){}
void MazeDefinitionNative::v10(){}
void MazeDefinitionNative::v14(){}
void MazeDefinitionNative::v18(){}
void MazeDefinitionNative::v1c(){}
void MazeDefinitionNative::v20(){}
void MazeDefinitionNative::v24(){}
void MazeDefinitionNative::v28(){}
void MazeDefinitionNative::v2c(){}
void MazeDefinitionNative::v30(){}
void MazeDefinitionNative::v34(){}

long* MazeDefinitionNative::GetSubDefIndex(const CCharString&){++indices;if(stringLive!=1)++errors;return &defIndex;}
CDefPointerBase MazeDefinitionManagerMethods::GetPDefFromGlobalIndex(long value){++managerCalls;if(this!=&manager||value!=77||stringLive)++errors;CDefPointerBase p;p.m_p=&definition;++definition.m_count;return p;}
int main(){TeleportMethods g;CAIBrain brain;MazeCombat combat;MazeDefinitionNative root;
#ifdef _MSC_VER
((CAIStateGroupBase*)&g)->PBrain=&brain;brain.Creature=(CThingAICreature*)&actor;
#else
g.PBrain=&brain;brain.Creature=&actor;
#endif
MazeEntry entries[2];entries[0].key=8;entries[0].value=&combat;actor.table.begin=entries;actor.table.end=entries+1;actor.masks[0]=0x100;actor.definition=&root;root.PDefManager=&manager;
MazePointer* saved=new(g._pad_0x14) MazePointer(&oldTarget);
for(scenario=0;scenario<7;++scenario){ctors=dtors=sets=pointerGets=targetCalls=worldCalls=frameCalls=destroys=stringLive=indices=managerCalls=0;saved->item=&oldTarget;target.dead=scenario==1?1:0;g.FrameOfLastTeleport=100;now=scenario==2?119:scenario==3?120:121;definition.TutorialTeleportTime=scenario==6?3.0f:2.0f;definition.m_count=scenario==5?0:1;
bool success=scenario==4||scenario==5;bool result=g.IsRunnable();int entered=scenario>=2;
if(result!=success||targetCalls!=1||worldCalls!=entered||frameCalls!=entered||ctors!=success||dtors!=success||sets!=2*success||pointerGets!=success||saved->item!=(success?&target:&oldTarget)||g.FrameOfLastTeleport!=100||definition.m_count!=(scenario==5?0:1)||destroys!=(scenario==5)||indices!=entered||managerCalls!=entered||stringLive)++errors;}
saved->~MazePointer();if(errors){std::printf("00954b50_TEST FAIL %d\n",errors);return 1;}std::printf("00954b50_TEST PASS\n");return 0;}