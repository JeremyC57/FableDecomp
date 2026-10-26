// CCreatureActionBase::SetAnimation(const CAnimationEntry&,float,float) -- copy animation, derive/clamp action timers and begin animation loading.
// Retail0x00693030 length259; real primary vtable0x01259D24 slots0 dtor,1Validate,2EditUpdate,3PerformAction,
// 4FinishAction,5IsPlayingAnimation,6GetAnimDelayTime. Native action prefix matches actual donor header.
#define CCreatureActionBase CCreatureActionBase_PDB
#include "engine/CCreatureActionBase.h"
#undef CCreatureActionBase
#define CWorld CWorld_PDB
#include "engine/CWorld.h"
#undef CWorld
#define CThingCreatureBase CThingCreatureBase_PDB
#include "engine/CThingCreatureBase.h"
#undef CThingCreatureBase
#define C3DAnimationInfo C3DAnimationInfo_PDB
#include "engine/C3DAnimationInfo.h"
#undef C3DAnimationInfo
#define CMeshDataBank CMeshDataBank_PDB
#include "engine/CMeshDataBank.h"
#undef CMeshDataBank
#include "engine/CIVCountedPointeeBase.h"
class C3DAnimationInfo:public C3DAnimationInfo_PDB { public:void BeginLoading(bool)const;}; //A26730 donorB65BA0 QBEX_N
class CMeshDataBank:public CMeshDataBank_PDB { public:const C3DAnimationInfo* PeekAnimation(unsigned long)const;}; //A242C0 donorB60570
class CWorld { public:unsigned char _beforeMainMeshBank[0x3c];CMeshDataBank* PMainMeshBank;};
class CThingCreatureBase { public:
    float GetAnimDuration(long); //0x006627F0
    CWorld* GetWorld(); //0x004C7AB0
};
struct CBaseIntelligentPointer {
    virtual ~CBaseIntelligentPointer();
    void* PData;
    CThingCreatureBase* GetPItem() const; //0x00A01B50
};
class CAnimComponentMap:public CIVCountedPointeeBase { public:};
struct CAnimComponentPointer {CAnimComponentMap* Object;CAnimComponentPointer():Object(0){}CAnimComponentPointer(const CAnimComponentPointer& p):Object(p.Object){if(Object)++Object->IVRefCount;}};
class CAnimationEntry { public:
    long BankIndex;
    unsigned long AnimName;
    unsigned long GroupName;
    CAnimComponentPointer Components;
    CAnimationEntry(long,long,long,long); //005DED00 genuine external ctor
    CAnimationEntry(const CAnimationEntry& p):BankIndex(p.BankIndex),AnimName(p.AnimName),GroupName(p.GroupName),Components(p.Components) {}
    long GetAnimationTransitionInTime() const; //0x005DC1F0
};
struct CAnimationEntryPointer {
    CAnimationEntry* Data;
    CCPPointerInfo* Info;
    CAnimationEntryPointer& operator=(CAnimationEntry* p){Reset(p);return *this;}
    CAnimationEntry* operator->()const{return Data;}
    void Reset(CAnimationEntry*); //0x00696830
};
extern long g_GameFramesPerSecond; //0x01375550
class CCreatureActionBase { public:
    virtual ~CCreatureActionBase(); //0x0062ED20 deleting
    virtual void Validate()const; //0x0099A400
    virtual void EditUpdate(float); //0x00692DB0
    virtual void PerformAction()=0; //pure virtual native slot3
    virtual void FinishAction(); //0x00694430
    virtual bool IsPlayingAnimation() const; //0x00692850
    virtual long GetAnimDelayTime() const; //0x00692460
    void* IntelligentPointer;
    CBaseIntelligentPointer PCreature;
    long TotalTime, ActionTime, AnimDelayTime, FramesRunning, InterruptionGroupID, InterruptionPriority, ActionID;
    CBaseIntelligentPointer PItemRemoved;
    long Sounds_FirstSound, Sounds_LastSound;
    unsigned long PlayingSoundID;
    float RunningTimeSeconds,LastRunningTimeSeconds;
    long StartFrame,PauseFrame,PauseTimer;
    float AnimMovementStep_X,AnimMovementStep_Y,AnimMovementStep_Z;
    bool AddedActionAnim,Finished,PerformedAction,Initialised;
    long NextMovementPosWorldFrame;
    float NextMovementPos_X,NextMovementPos_Y,NextMovementPos_Z;
    CAnimationEntryPointer PAnimEntry;
    void SetAnimation(const CAnimationEntry&,float,float);
};
FABLE_STATIC_ASSERT(offsetof(CCreatureActionBase,PAnimEntry)==0x74);
void CCreatureActionBase::SetAnimation(const CAnimationEntry& entry,float duration,float actionFraction)
{
    long fps=g_GameFramesPerSecond;
    PAnimEntry=new CAnimationEntry(entry);
    AnimDelayTime=PAnimEntry->GetAnimationTransitionInTime();
    if(duration < 0.0f) {
        CThingCreatureBase* owner=PCreature.GetPItem();
        TotalTime=(long)(owner->GetAnimDuration(entry.BankIndex)*fps);
    }
    else
        TotalTime=(long)(fps*duration);
    ActionTime=(long)(TotalTime*actionFraction);
    ActionTime+=GetAnimDelayTime();
    long totalDelay=GetAnimDelayTime();
    TotalTime=totalDelay+TotalTime;
    CAnimationEntry* anim=PAnimEntry.operator->();
    CWorld* world=PCreature.GetPItem()->GetWorld();
    CMeshDataBank* manager=world->PMainMeshBank;
    manager->PeekAnimation(anim->BankIndex)->BeginLoading(false);
    if(TotalTime<1) TotalTime=1;
    if(ActionTime<0) ActionTime=0;
}