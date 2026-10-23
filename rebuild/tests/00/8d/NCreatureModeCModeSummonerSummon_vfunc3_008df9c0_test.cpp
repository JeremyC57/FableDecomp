#include <cstdio>
// BEGIN exact source snapshot SHA256 f4d13b35f9053c4ed105a9f9450428b6397dc7b1bca4ee861a4515e52a787a4e
// NCreatureMode::CModeSummonerSummon::FrameUpdate, retail 0x008DF9C0.
// PDB identity: egor 20132 / debug 0x00822180, vtable slot 3.
// Shared CModeStandBase header is imported under its generated name; retail
// CModeBase/StandBase state is consumed as opaque base storage here.
#define CModeStandBase CModeStandBasePdb
#include "engine/CModeStandBase.h"
#include "engine/CBaseObject.h"
#include <stddef.h>
#undef CModeStandBase
#define CCreatureActionBase CCreatureActionBasePdb
#include "engine/CCreatureActionBase.h"
#undef CCreatureActionBase

struct C3DVector {
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    __forceinline C3DVector operator*(float scale) const
    { return C3DVector(X * scale, Y * scale, Z * scale); }
    __forceinline C3DVector operator+(const C3DVector& other) const
    { return C3DVector(X + other.X, Y + other.Y, Z + other.Z); }
    __forceinline C3DVector& operator+=(const C3DVector& other)
    { X += other.X; Y += other.Y; Z += other.Z; return *this; }
};
struct CTCInterface { virtual void v0() {} };
struct CTCMapEntry { long Type; CTCInterface* Component; };
struct CTCMap {
    CTCMapEntry* First;
    CTCMapEntry* Last;
    CTCMapEntry* LowerBound(const long* type); // 0x0040F020
    CTCMapEntry* Find(long interfaceType)
    {
        long type = interfaceType;
        CTCMapEntry* it = LowerBound(&type);
        return (it == Last || interfaceType < it->Type ? Last : it);
    }
};
struct CThing : CBaseObject {
    unsigned char BeforeComponents[0x20-sizeof(CBaseObject)];
    unsigned long UsingComponents[9];
    CTCMap Interfaces;
    unsigned long HasTC(long type) const { return UsingComponents[type >> 5] & (1UL << (type & 31)); }
    template<class T> T* GetTC(long interfaceType)
    {
        long type = interfaceType;
        CTCMap* map = &Interfaces;
        CTCMapEntry* it = map->LowerBound(&type);
        return (T*)(it == map->Last || interfaceType < it->Type ? map->Last : it)->Component;
    }
    const C3DVector& GetPos() const; // 0x004C73D0
};
struct CThingCreatureBase : CThing {
    bool SetCurrentAction(const struct CCreatureActionBase& action); // 0x006644F0
};
struct CTCAISummon : CTCInterface {
    long GetNumberMySummonLimit() const; // 0x007FDE60
    long SummonCreatures(const C3DVector* position); // 0x007FE040
};
// Original 8-byte wrapper/control-block ABI; GetPItem performs lazy dead-object cleanup.
struct CBaseObjectPointer { CBaseObject* object; unsigned long refcount; };
struct CBaseIntelligentPointer {
    CBaseObjectPointer* PData;
    CBaseIntelligentPointer();
    virtual ~CBaseIntelligentPointer();
    CBaseObject* GetPItem() const; // 0x00a01b50
    void SetPItem(CBaseObject*);
};
template<class T> struct CIntelligentPointer : CBaseIntelligentPointer {
    CIntelligentPointer() : CBaseIntelligentPointer() {}
    T* Get() const { return static_cast<T*>(GetPItem()); }
};
struct CAnimateThing;
struct CTCCreatureModeManager {
    struct CModeBase {
        virtual ~CModeBase() {}
        virtual unsigned long GetType() const = 0;
        virtual void AnimUpdate(CAnimateThing&) = 0;
        virtual void FrameUpdate(); // 0x0073ac50, original nested owner
        CThing* ParentThing; // +4
        unsigned char WeaponBasedFilter[0x0c];
        long FramesBeforeRemovingSelf;
        bool Active;
    };
};
// Layout: donor confirms 0x68; original base's 0x1c layout is PDB23291-23297.
struct CModeStandBase : CTCCreatureModeManager::CModeBase {
    virtual void AnimUpdate(CAnimateThing&);
    unsigned char StandState[0x68-sizeof(CTCCreatureModeManager::CModeBase)];
};
namespace NCreatureMode {
struct CModeSummonerSummon : CModeStandBase {
    virtual unsigned long GetType() const;
    unsigned char Filter[0x0c];
    mutable CIntelligentPointer<CThing> PTarget; // original type, +0x74
    struct { C3DVector* First; C3DVector* Last; C3DVector* End; } CreatePositions;
    long Timer;
    long NumCreated;
    virtual void FrameUpdate();
};
}
// Layout: donor is 0xb4; retail is 0xa8 because its three STLport containers
// are each four bytes smaller. Confirmed by 00693ef0 and 00858030 bodies.
// This caller treats action state as opaque and invokes its genuine external cleanup.
struct CCreatureActionBase {
    virtual ~CCreatureActionBase();
    unsigned char Opaque[0xa4];
};
struct CCombatActionBase : CCreatureActionBase {
    unsigned char CombatState[0x6c];
    virtual ~CCombatActionBase(); // 0x0062E900
};
struct CCreatureAction_SummonerSummonOutOf : CCombatActionBase {
    CCreatureAction_SummonerSummonOutOf(CThingCreatureBase& owner); // 0x008CBD50
};

void NCreatureMode::CModeSummonerSummon::FrameUpdate()
{
    CTCCreatureModeManager::CModeBase::FrameUpdate();
    if (ParentThing->HasTC(0xfa)) {
        CTCAISummon* summon = ParentThing->GetTC<CTCAISummon>(0xfa);
        if (NumCreated != summon->GetNumberMySummonLimit()) {
            C3DVector position = ParentThing->GetPos() * 0.5f;
            if (PTarget.Get() &&
                !(*(unsigned char*)((char*)PTarget.Get() + 0x91) & 1))
                position += PTarget.Get()->GetPos() * 0.5f;
            long created = summon->SummonCreatures(&position);
            NumCreated = NumCreated + created;
            return;
        }
    }
    CThingCreatureBase* owner = static_cast<CThingCreatureBase*>(ParentThing);
    owner->SetCurrentAction(CCreatureAction_SummonerSummonOutOf(*owner));
}

// END exact source snapshot
#include <stdlib.h>
#include <stddef.h>

static int baseUpdates, limitCalls, summonCalls, actionConstructs, actionDestroys, actionSets;
static long limitValue, summonResult;
static CThing* actionOwner;
static C3DVector parentPosition, targetPosition, capturedPosition;
static bool acceptAction;

CTCMapEntry* CTCMap::LowerBound(const long* type)
{
    CTCMapEntry* it = First;
    while (it != Last && it->Type < *type) ++it;
    return it;
}
struct TestThing : CThingCreatureBase {
    unsigned char tail[0x91 - sizeof(CThingCreatureBase)];
    unsigned char DeadBit;
    C3DVector Position;
};
const C3DVector& CThing::GetPos() const
{
    const TestThing* thing = (const TestThing*)this;
    return thing->Position;
}
bool CThingCreatureBase::SetCurrentAction(const CCreatureActionBase&)
{
    ++actionSets; return acceptAction;
}
// Faithful external-helper stand-ins: refs count wrappers, not the living object.
static int LivePointers,LiveBlocks,PointerReads;
CBaseIntelligentPointer::CBaseIntelligentPointer():PData(0){++LivePointers;}
static void ReleaseBlock(CBaseObjectPointer* p){if(p && --p->refcount==0 && !p->object){free(p);--LiveBlocks;}}
CBaseIntelligentPointer::~CBaseIntelligentPointer(){ReleaseBlock(PData);--LivePointers;}
CBaseObject* CBaseIntelligentPointer::GetPItem()const{
 ++PointerReads;
 if(!PData)return 0;
 if(PData->object)return PData->object;
 ReleaseBlock(PData);const_cast<CBaseIntelligentPointer*>(this)->PData=0;return 0;
}
void CBaseIntelligentPointer::SetPItem(CBaseObject* object){
 if(PData && object && PData->object==object)return;
 ReleaseBlock(PData);PData=0;
 if(!object)return;
 if(object->IntelligentPointer){PData=object->IntelligentPointer;++PData->refcount;}
 else{PData=(CBaseObjectPointer*)malloc(sizeof(CBaseObjectPointer));PData->object=object;PData->refcount=1;object->IntelligentPointer=PData;++LiveBlocks;}
}
static void DestroyObject(CBaseObject* object){
 if(object->IntelligentPointer){CBaseObjectPointer* p=object->IntelligentPointer;p->object=0;if(!p->refcount){free(p);--LiveBlocks;}object->IntelligentPointer=0;}
}
void CTCCreatureModeManager::CModeBase::FrameUpdate() { ++baseUpdates; }
void CModeStandBase::AnimUpdate(CAnimateThing&) {}
unsigned long NCreatureMode::CModeSummonerSummon::GetType() const { return 0; }
long CTCAISummon::GetNumberMySummonLimit() const { ++limitCalls; return limitValue; }
long CTCAISummon::SummonCreatures(const C3DVector* position)
{
    ++summonCalls; capturedPosition = *position; return summonResult;
}
CCreatureActionBase::~CCreatureActionBase() {}
CCombatActionBase::~CCombatActionBase() { ++actionDestroys; }
CCreatureAction_SummonerSummonOutOf::CCreatureAction_SummonerSummonOutOf(CThingCreatureBase& owner)
{ ++actionConstructs; actionOwner = &owner; }

static int check(bool ok) { return ok ? 0 : 1; }
int main()
{
    static TestThing parent;
    static TestThing target;
    static CTCAISummon component;
    static CTCMapEntry entries[2];
    static NCreatureMode::CModeSummonerSummon mode;

    parent.UsingComponents[7] = 1UL << 26;
    entries[0].Type = 0xfa; entries[0].Component = &component;
    entries[1].Type = 0x7fffffff; entries[1].Component = 0;
    parent.Interfaces.First = entries; parent.Interfaces.Last = entries + 1;
    parent.Position.X = 10.0f; parent.Position.Y = -4.0f; parent.Position.Z = 8.0f;
    target.Position.X = 6.0f; target.Position.Y = 2.0f; target.Position.Z = -10.0f;
    target.DeadBit = 0;
    mode.ParentThing = &parent;
    mode.PTarget.SetPItem(&target);
    if (sizeof(CCreatureActionBase)!=0xa8 || sizeof(CCombatActionBase)!=0x114 ||
        sizeof(CBaseObject)!=8 || sizeof(CBaseIntelligentPointer)!=8 ||
        offsetof(CBaseIntelligentPointer,PData)!=4 || sizeof(CTCCreatureModeManager::CModeBase)!=0x1c ||
        sizeof(CModeStandBase)!=0x68 || offsetof(NCreatureMode::CModeSummonerSummon,PTarget)!=0x74 ||
        offsetof(NCreatureMode::CModeSummonerSummon,NumCreated)!=0x8c ||
        offsetof(TestThing,DeadBit)!=0x91 || LiveBlocks!=1 || target.IntelligentPointer->refcount!=1)
        return std::printf("008df9c0_TEST FAIL layout/lifetime\n"),1;
    mode.NumCreated = 1;
    limitValue = 3; summonResult = 2;
    mode.FrameUpdate();
    if (baseUpdates != 1 || limitCalls != 1 || summonCalls != 1 || actionSets != 0 ||
        mode.NumCreated != 3 || capturedPosition.X != 8.0f || capturedPosition.Y != -1.0f || capturedPosition.Z != -1.0f)
        return std::printf("008df9c0_TEST FAIL\n"), 1;

    // Dead target is omitted; position uses only the summoner's half-position.
    target.DeadBit = 1; mode.NumCreated = 0; limitValue = 4;
    mode.FrameUpdate();
    if (baseUpdates != 2 || summonCalls != 2 || capturedPosition.X != 5.0f || capturedPosition.Y != -2.0f || capturedPosition.Z != 4.0f)
        return std::printf("008df9c0_TEST FAIL\n"), 1;

    // At limit, it creates the local SummonOutOf action and submits it.
    mode.NumCreated = 4; limitValue = 4; acceptAction = true;
    mode.FrameUpdate();
    if (baseUpdates != 3 || limitCalls != 3 || summonCalls != 2 || actionConstructs != 1 ||
        actionDestroys != 1 || actionSets != 1 || actionOwner != &parent)
        return std::printf("008df9c0_TEST FAIL\n"), 1;

    // Without the component, it follows the same action path without querying the limit.
    parent.UsingComponents[7] = 0;
    mode.FrameUpdate();
    if (baseUpdates != 4 || limitCalls != 3 || actionConstructs != 2 || actionDestroys != 2 || actionSets != 2)
        return std::printf("008df9c0_TEST FAIL\n"), 1;
    // Wrappers share the original control block and never own the thing itself.
    {
        CIntelligentPointer<CThing> other;
        other.SetPItem(&target);
        if (target.IntelligentPointer->refcount!=2 || other.Get()!=&target)
            return std::printf("008df9c0_TEST FAIL shared block\n"),1;
    }
    if(target.IntelligentPointer->refcount!=1 || LiveBlocks!=1)
        return std::printf("008df9c0_TEST FAIL wrapper release\n"),1;
    // Object death leaves an orphan block until this mode next reads its wrapper.
    DestroyObject(&target);
    parent.UsingComponents[7]=1UL<<26; mode.NumCreated=0; limitValue=4;
    int readsBefore=PointerReads;
    mode.FrameUpdate();
    if(mode.PTarget.PData || LiveBlocks || PointerReads!=readsBefore+1 ||
       capturedPosition.X!=5.0f || capturedPosition.Y!=-2.0f || capturedPosition.Z!=4.0f)
        return std::printf("008df9c0_TEST FAIL lazy orphan cleanup\n"),1;
    // Last wrapper release on a still-live object retains its zero-reference block.
    mode.PTarget.SetPItem(&target);
    mode.PTarget.SetPItem(0);
    if(!target.IntelligentPointer || target.IntelligentPointer->refcount || LiveBlocks!=1)
        return std::printf("008df9c0_TEST FAIL live zero-reference block\n"),1;
    DestroyObject(&target);
    if(LiveBlocks || target.IntelligentPointer)
        return std::printf("008df9c0_TEST FAIL final block cleanup\n"),1;
    std::printf("008df9c0_TEST PASS\n");
    return 0;
}