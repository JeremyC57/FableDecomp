#include <cstdio>
#include <stdlib.h>
#include <stddef.h>
// BEGIN exact source snapshot SHA256 a99aa1485c61286da517bd6c2cbf9aa86eb9be8c8113173c8ed7b4b55348d91f
// 008E8830 NCreatureMode::CModeBriarRoseTeleportInvulnerable::FrameUpdate.
// Original nested mode owner, retail action sizes, and weak control blocks.
#define CModeStandBase CModeStandBasePdb
#include "engine/CModeStandBase.h"
#include "engine/CBaseObject.h"
#include <stddef.h>
#undef CModeStandBase
#define CCreatureActionBase CCreatureActionBasePdb
#include "engine/CCreatureActionBase.h"
#undef CCreatureActionBase

extern long g_FrameTicks_01375550;
struct CTCGraphicAppearance { void SetAlpha(unsigned char); }; // 004BFAC0
struct CThing : CBaseObject {
    unsigned char BeforeAppearance[0x64-sizeof(CBaseObject)];
    CTCGraphicAppearance* PTCGraphicAppearance;
    unsigned char BeforeFlags[4];
    unsigned long ThingTypeFlags;
    unsigned char BeforeKilled[0x91-0x70];
    unsigned char KilledFlags;
};
struct CCreatureActionBase;
struct CThingCreatureBase : CThing {
    bool IsPerformingAnyAction() const; // 006622A0
    bool SetCurrentAction(const CCreatureActionBase&); // 006644F0
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
struct CCreatureAction_BriarRoseForcePush : CCombatActionBase {
    CIntelligentPointer<CThingCreatureBase> PEnemy; // PDB129219, retail +0x114
    // CombatActionBase has a distinct PTarget at +0xa8 inside opaque CombatState.
    CCreatureAction_BriarRoseForcePush(CThingCreatureBase&,CThingCreatureBase&); // 008E8C50
};
namespace NCreatureMode {
struct CModeBriarRoseTeleportInvulnerable : CModeStandBase {
    virtual unsigned long GetType() const;
    unsigned char Filter[0x0c];
    long Timer;
    CIntelligentPointer<CThingCreatureBase> PTarget;
    CIntelligentPointer<CThing> PEffect;
    virtual void FrameUpdate();
};
void CModeBriarRoseTeleportInvulnerable::FrameUpdate()
{
    CTCCreatureModeManager::CModeBase::FrameUpdate();
    if ((ParentThing->ThingTypeFlags & 8) && static_cast<CThingCreatureBase*>(ParentThing)->IsPerformingAnyAction())
        return;

    --Timer;
    if (Timer <= 0) {
        CThingCreatureBase& owner = *static_cast<CThingCreatureBase*>(ParentThing);
        if (PTarget.Get() &&
            !(*(unsigned char*)((char*)PTarget.Get() + 0x91) & 1) &&
            owner.SetCurrentAction(CCreatureAction_BriarRoseForcePush(
                owner, *(CThingCreatureBase*)PTarget.Get())))
            return;
    }

    long alpha = (long)(((float)Timer * 255.0f) /
                        ((float)g_FrameTicks_01375550 * 1.25f));
    if (alpha < 0)
        alpha = 0;
    else if (alpha > 255)
        alpha = 255;
    ParentThing->PTCGraphicAppearance->SetAlpha((unsigned char)alpha);
}
}

// END exact source snapshot
long g_FrameTicks_01375550=10;
static bool performingAction,acceptAction;
static int baseUpdates,actionChecks,actionSets,actionConstructs,combatDestroys,alphaCalls;
static unsigned char lastAlpha;
static CThingCreatureBase* observedOwner;
static CThingCreatureBase* observedTarget;
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

void CTCCreatureModeManager::CModeBase::FrameUpdate(){++baseUpdates;}
void CModeStandBase::AnimUpdate(CAnimateThing&){}
unsigned long NCreatureMode::CModeBriarRoseTeleportInvulnerable::GetType()const{return 0;}
bool CThingCreatureBase::IsPerformingAnyAction()const{++actionChecks;return performingAction;}
bool CThingCreatureBase::SetCurrentAction(const CCreatureActionBase&){++actionSets;return acceptAction;}
CCreatureActionBase::~CCreatureActionBase(){}
CCombatActionBase::~CCombatActionBase(){++combatDestroys;}
CCreatureAction_BriarRoseForcePush::CCreatureAction_BriarRoseForcePush(CThingCreatureBase& owner,CThingCreatureBase& target)
{++actionConstructs;observedOwner=&owner;observedTarget=&target;PEnemy.SetPItem(&target);}
void CTCGraphicAppearance::SetAlpha(unsigned char a){++alphaCalls;lastAlpha=a;}
#define CHECK(x) do { if(!(x)) return std::printf("008e8830_TEST FAIL line %d\n",__LINE__),1; } while(0)
int main(){
 static NCreatureMode::CModeBriarRoseTeleportInvulnerable mode;
 static CThingCreatureBase owner,target;
 static CTCGraphicAppearance graphic;
 CHECK(sizeof(CBaseObject)==8 && sizeof(CBaseIntelligentPointer)==8);
 CHECK(offsetof(CBaseIntelligentPointer,PData)==4 && sizeof(CThingCreatureBase)>=0x92);
 CHECK(offsetof(CThing,PTCGraphicAppearance)==0x64 && offsetof(CThing,ThingTypeFlags)==0x6c && offsetof(CThing,KilledFlags)==0x91);
 CHECK(sizeof(CCreatureActionBase)==0xa8 && sizeof(CCombatActionBase)==0x114 && sizeof(CCreatureAction_BriarRoseForcePush)==0x11c);
 CHECK(sizeof(CTCCreatureModeManager::CModeBase)==0x1c && sizeof(CModeStandBase)==0x68);
 CHECK(offsetof(NCreatureMode::CModeBriarRoseTeleportInvulnerable,Timer)==0x74 && offsetof(NCreatureMode::CModeBriarRoseTeleportInvulnerable,PTarget)==0x78 && offsetof(NCreatureMode::CModeBriarRoseTeleportInvulnerable,PEffect)==0x80);
 owner.PTCGraphicAppearance=&graphic; mode.ParentThing=&owner; mode.PTarget.SetPItem(&target);
 CHECK(LiveBlocks==1 && target.IntelligentPointer->refcount==1);
 mode.Timer=5;mode.FrameUpdate();
 CHECK(baseUpdates==1 && mode.Timer==4 && lastAlpha==81 && alphaCalls==1 && actionSets==0 && actionChecks==0);
 // Accepted action: its typed target shares and releases the same control block.
 mode.Timer=0;acceptAction=true;int pointersBefore=LivePointers;mode.FrameUpdate();
 CHECK(baseUpdates==2 && mode.Timer==-1 && actionSets==1 && actionConstructs==1 && combatDestroys==1 && alphaCalls==1);
 CHECK(observedOwner==&owner && observedTarget==&target && LivePointers==pointersBefore && target.IntelligentPointer->refcount==1);
 // Rejected action continues to alpha update and cleans its temporary.
 mode.Timer=0;acceptAction=false;mode.FrameUpdate();
 CHECK(actionSets==2 && combatDestroys==2 && alphaCalls==2 && lastAlpha==0 && target.IntelligentPointer->refcount==1);
 // Killed flag suppresses construction, but weak wrapper remains bound.
 target.KilledFlags=1;mode.Timer=0;int reads=PointerReads;mode.FrameUpdate();
 CHECK(actionSets==2 && PointerReads==reads+2 && mode.PTarget.Get()==&target && lastAlpha==0);
 target.KilledFlags=0;
 // Missing target and actual object death are distinct cases.
 mode.PTarget.SetPItem(0);CHECK(target.IntelligentPointer->refcount==0 && LiveBlocks==1);
 mode.Timer=0;mode.FrameUpdate();CHECK(actionSets==2 && lastAlpha==0);
 mode.PTarget.SetPItem(&target);DestroyObject(&target);mode.Timer=0;reads=PointerReads;mode.FrameUpdate();
 CHECK(PointerReads==reads+1 && !mode.PTarget.PData && LiveBlocks==0 && actionSets==2);
 // Saturation at upper limit, and ordinary upper boundary.
 mode.Timer=100;mode.FrameUpdate();CHECK(lastAlpha==255);
 mode.Timer=1;mode.FrameUpdate();CHECK(lastAlpha==0 && mode.Timer==0);
 // Active action guard precedes timer update and all alpha/target work.
 owner.ThingTypeFlags=8;performingAction=true;mode.Timer=3;int alphas=alphaCalls;reads=PointerReads;
 mode.FrameUpdate();CHECK(mode.Timer==3 && alphaCalls==alphas && PointerReads==reads && actionChecks==1);
 performingAction=false;mode.FrameUpdate();CHECK(mode.Timer==2 && alphaCalls==alphas+1 && actionChecks==2);
 CHECK(LiveBlocks==0);
 std::printf("008e8830_TEST PASS\n");return 0;
}