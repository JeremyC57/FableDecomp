// CBH_GangLeader::Init() -- reset the gang leader's hit count, stage flags and health marks, then register the parent bounty-hunt script's WatchForLeaderKilled and WatchForDecapitation callbacks
// Leaf: Init
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00D16DC0, length 519 bytes (vtable 0x012CA5CC).
// Callees: operator new @ 0x00BFEA1A (0x48), CCharString(const char*, long) @ 0x0099EBF0 / ~CCharString @ 0x0099EAE0,
//          operator+ @ 0x0099F570, CSpawnedFuncBase ctor @ 0x00CDD450, CScriptThing copy @ 0x004ABE90 / dtor
//          @ 0x004AA840, CQ_BountyHuntScript::Add @ 0x00CB7E50. Strings "ParentClass." 0x012C9934,
//          "WatchForLeaderKilled" 0x012CA694, "WatchForDecapitation" 0x012CA67C, "" 0x0122D70E. Spawned-func vtable
//          0x012CA5E0, callbacks 0x00D17090 / 0x00D16FD0.
// Layout: Ego_r.pdb CBH_GangLeader (44 bytes): ParentClass +0x14, PMasterData +0x18, hitCount +0x1c, stage1..4Done
//         +0x20..+0x23, HealthInitial +0x24, HealthCurrent +0x28; the spawned-func model is the landed
//         CAT_BanditContact::Init 0x00F09640.
// Shape: the member resets come first (the float zeros are integer stores from the cached zero register), then the
//         landed two-callback body.
#include <new>
#include "fable_string.h"
#include "engine/CSpawnedFuncBase.h"
#include "engine/CScriptThing.h"
#include "engine/CASuspendableProcess.h"

class CCPPointerInfo { public: long References; };
class C3DVector { public: float X, Y, Z; };

struct CScriptThingRuntime {
    virtual ~CScriptThingRuntime(); // CScriptThing RTTI slot 0, 0x004AB170
    CScriptThingRuntime* PImp_Data;
    CCPPointerInfo* PImp_Info;
    virtual const CCharString& GetName() const;       // slot 1, 0x004AA8B0
    virtual CCharString GetDefName() const;           // slot 2, 0x004AA8D0
    virtual CCharString GetDataString() const;        // slot 3, 0x004AA900
    virtual void SetDataString(const CCharString&);   // slot 4, 0x004AA930
    virtual C3DVector GetFocalPos() const;            // slot 5, 0x004AA940
    virtual const C3DVector& GetPos() const;          // slot 6, 0x004AA980
    CScriptThingRuntime() : PImp_Data(0), PImp_Info(0) {}
    CScriptThingRuntime(const CScriptThingRuntime& other)
        : PImp_Data(other.PImp_Data), PImp_Info(other.PImp_Info)
    { if (PImp_Info) ++PImp_Info->References; }
    CScriptThingRuntime(const struct CScriptThingStored& other);
};
#if defined(_MSC_VER)
FABLE_STATIC_ASSERT(sizeof(CScriptThingRuntime) == sizeof(CScriptThing));
#endif

// Same retail CScriptThing view, using the native out-of-line copy constructor
// for CSpawnedFunc1::Par1; the caller's by-value argument uses the inline copy above.
struct CScriptThingStored {
    virtual ~CScriptThingStored();
    CScriptThingRuntime* PImp_Data;
    CCPPointerInfo* PImp_Info;
    virtual const CCharString& GetName() const;
    virtual CCharString GetDefName() const;
    virtual CCharString GetDataString() const;
    virtual void SetDataString(const CCharString&);
    virtual C3DVector GetFocalPos() const;
    virtual const C3DVector& GetPos() const;
    CScriptThingStored(const CScriptThingRuntime& other); // retail 0x004ABE90
};
inline CScriptThingRuntime::CScriptThingRuntime(const CScriptThingStored& other)
    : PImp_Data(other.PImp_Data), PImp_Info(other.PImp_Info)
{ if (PImp_Info) ++PImp_Info->References; }

struct CScriptProcessBaseRuntime {
    virtual ~CScriptProcessBaseRuntime();
    virtual void RunSuspendableProcess();
    virtual void SuspendSuspendableProcess(bool suspend);
    virtual void Restart();
    virtual void SuspendableProcess() = 0;
    virtual void RuntimeHookCDD410(); // native one-byte ret at 0x00CDD410; original name/signature unresolved
    virtual void RuntimeHookCDD420(); // native one-byte ret at 0x00CDD420; original name/signature unresolved
    bool Running;
    bool StopRunning;
    bool TerminateUsingExceptions;
    unsigned char _pad_07;
    float TimeProcessSliceStarted;
    float TimesliceLength;
    CScriptProcessBaseRuntime(bool running, long stackSize, float fraction); // 0x00A44740
};

struct CSpawnedFuncBaseRuntime : CScriptProcessBaseRuntime {
    virtual ~CSpawnedFuncBaseRuntime(); // ordinary base destructor 0x00CDD4C0 cleans PPredicate
    unsigned long SpawnedFuncBaseState; // +0x10 word in the retail CSpawnedFuncBase subobject
    unsigned long ID;
    long LastScriptFrame;
    long CurrentScriptFrame;
    CCharString Name;
    CCharString AssociatedRegionName;
    bool FuncPaused;
    bool Interrupted;
    unsigned char _pad_2a[2];
    void* PPredicate_Data;
    CCPPointerInfo* PPredicate_Info;
    CSpawnedFuncBaseRuntime(const CCharString& name, unsigned long id); // 0x00CDD450
};

struct CSpawnedFuncBounty;
struct CQ_BountyHuntScript {
    void WatchForLeaderKilled(CScriptThingRuntime thing);
    void WatchForDecapitation(CScriptThingRuntime thing);
    void Add(CSpawnedFuncBounty* function, const CCharString& name); // 0x00CB7E50
};
typedef void (CQ_BountyHuntScript::*SpawnedScriptCallback)(CScriptThingRuntime);
struct CSpawnedFuncBounty : CSpawnedFuncBaseRuntime {
    SpawnedScriptCallback Func;
    CQ_BountyHuntScript* PClass;
    CScriptThingStored Par1;
    virtual void SuspendableProcess(); // vtable 0x012CA5E0
    CSpawnedFuncBounty(const CCharString& name, unsigned long id,
                       SpawnedScriptCallback func, CQ_BountyHuntScript* owner,
                       CScriptThingRuntime par1);
};
#if defined(_MSC_VER)
FABLE_STATIC_ASSERT(sizeof(CSpawnedFuncBounty) == 0x48);
#endif

struct CBH_GangLeader {
    void* __vftable;
    void* GSI;                               // +0x04
    CScriptThingRuntime Thing;               // +0x08
    CQ_BountyHuntScript* ParentClass;        // +0x14
    void* PMasterData;                       // +0x18
    long hitCount;                           // +0x1c
    bool stage1Done;                         // +0x20
    bool stage2Done;                         // +0x21
    bool stage3Done;                         // +0x22
    bool stage4Done;                         // +0x23
    float HealthInitial;                     // +0x24
    float HealthCurrent;                     // +0x28
    void Init();
};
#if defined(_MSC_VER)
FABLE_STATIC_ASSERT(offsetof(CBH_GangLeader, ParentClass) == 0x14);
FABLE_STATIC_ASSERT(offsetof(CBH_GangLeader, HealthCurrent) == 0x28);
#endif

void CScriptProcessBaseRuntime::RuntimeHookCDD410() {}
void CScriptProcessBaseRuntime::RuntimeHookCDD420() {}
CSpawnedFuncBounty::CSpawnedFuncBounty(const CCharString& name, unsigned long id,
                                       SpawnedScriptCallback func, CQ_BountyHuntScript* owner,
                                       CScriptThingRuntime par1)
    : CSpawnedFuncBaseRuntime(name, id), Func(func), PClass(owner), Par1(par1)
{
}

CCharString FABLE_FASTCALL operator+(const CCharString& left, const CCharString& right);

void CBH_GangLeader::Init()
{
    hitCount = 0;
    HealthInitial = 0;
    HealthCurrent = 0;
    stage1Done = false;
    stage2Done = false;
    stage3Done = false;
    stage4Done = false;
    ParentClass->Add(
        new CSpawnedFuncBounty(
            CCharString("ParentClass.", -1) + CCharString("WatchForLeaderKilled", -1),
            0, &CQ_BountyHuntScript::WatchForLeaderKilled, ParentClass, Thing),
        CCharString("", -1));
    ParentClass->Add(
        new CSpawnedFuncBounty(
            CCharString("ParentClass.", -1) + CCharString("WatchForDecapitation", -1),
            0, &CQ_BountyHuntScript::WatchForDecapitation, ParentClass, Thing),
        CCharString("", -1));
}