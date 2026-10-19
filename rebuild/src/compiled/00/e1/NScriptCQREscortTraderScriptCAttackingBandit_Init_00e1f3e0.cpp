// CAttackingBandit::Init() -- mark the trader group and register WatchForBanditDead on ParentClass
// Native 0x00E1F3E0, 0x132 bytes; PDB fields ParentClass +0x14, PMasterData +0x18, Screamed +0x1c.
// Retail object is CSpawnedFunc1<CQR_EscortTraderScript, CScriptThing>, size 0x48: base 0x34, Func +0x34, PClass +0x38, Par1 +0x3c. The nested new-expression keeps the callback-name temporaries through Add while the by-value trigger parameter is destroyed after construction and before Add.
#include <new>
#include "fable_string.h"
#include "engine/CSpawnedFuncBase.h"
#include "engine/CScriptThing.h"
#include "engine/CASuspendableProcess.h"
#include "engine/CGameScriptInterface.h"

struct CCPPointerInfo { long References; };
struct C3DVector { float X, Y, Z; };

struct CScriptThingRuntime;
struct CCountedScriptThingRuntime {
    CScriptThingRuntime* Data;
    CCPPointerInfo* Info;
    CCountedScriptThingRuntime() : Data(0), Info(0) {}
    CCountedScriptThingRuntime(const CCountedScriptThingRuntime& other) {
        Data = other.Data;
        Info = other.Info;
        if (Info) ++Info->References;
    }
};
struct CScriptThingRuntime {
    virtual ~CScriptThingRuntime(); // CScriptThing RTTI slot 0, 0x004AB170
    CCountedScriptThingRuntime PImp;
    virtual const CCharString& GetName() const;       // slot 1, 0x004AA8B0
    virtual CCharString GetDefName() const;           // slot 2, 0x004AA8D0
    virtual CCharString GetDataString() const;        // slot 3, 0x004AA900
    virtual void SetDataString(const CCharString&);   // slot 4, 0x004AA930
    virtual C3DVector GetFocalPos() const;            // slot 5, 0x004AA940
    virtual const C3DVector& GetPos() const;          // slot 6, 0x004AA980
    CScriptThingRuntime() : PImp() {}
    CScriptThingRuntime(const CScriptThingRuntime& other) : PImp(other.PImp) {}
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
    : PImp()
{ PImp.Data=other.PImp_Data; PImp.Info=other.PImp_Info; if (PImp.Info) ++PImp.Info->References; }

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
    virtual ~CSpawnedFuncBaseRuntime(); // 0x00CDD4C0; ordinary base destructor releases predicate and both strings
    CSpawnedFuncBaseRuntime(const CCharString& name, unsigned long id); // 0x00CDD450
};

struct CSpawnedFuncPhase3;
// PE evidence: native calls the true thiscall method at GSI vtable slot465
// (+0x744). VC7.1's default single-inheritance member-pointer representation is
// one code pointer; this documented union bridge invokes that native signature
// through the actual donor class without adding fabricated virtual methods.
// This avoids the unsupported free-function-pointer __fastcall extension and
// leaves ECX=this with both real reference arguments on the stack.
typedef void (CGameScriptInterface::*EntitySetCombatTypeMethod)(const CScriptThingRuntime&, const CCharString&);
union EntitySetCombatTypeSlot465 { void* code; EntitySetCombatTypeMethod method; };
#if defined(_MSC_VER)
FABLE_STATIC_ASSERT(sizeof(EntitySetCombatTypeMethod) == sizeof(void*));
FABLE_STATIC_ASSERT(sizeof(EntitySetCombatTypeSlot465) == sizeof(void*));
#endif
struct CGameScriptInterfaceVTableView {
    void* UnresolvedSlots000To464[465];
    EntitySetCombatTypeSlot465 EntitySetCombatType;
};
struct CQR_EscortTraderScript {
    void WatchForBanditDead(CScriptThingRuntime thing);
    void Add(CSpawnedFuncPhase3* function, const CCharString& name); // 0x00CB7E50; observed function at +4, name at +8
};
typedef void (CQR_EscortTraderScript::*SpawnedScriptCallback)(CScriptThingRuntime);
struct CSpawnedFuncPhase3 : CSpawnedFuncBaseRuntime {
    SpawnedScriptCallback Func;
    CQR_EscortTraderScript* PClass;
    CScriptThingStored Par1;
    virtual void SuspendableProcess(); // slot 4, 0x00E1F5E0 for this specialization
    CSpawnedFuncPhase3(const CCharString& name, unsigned long id,
                       SpawnedScriptCallback func, CQR_EscortTraderScript* owner,
                       CScriptThingRuntime par1);
};
#if defined(_MSC_VER)
FABLE_STATIC_ASSERT(sizeof(CSpawnedFuncPhase3) == 0x48);
#endif

struct CEntityScriptBase {                 // donor layout: +0 vptr, +4 Game, +8 Me
    void* __vftable;
    CGameScriptInterface* Game;             // +0x04
    CScriptThingRuntime Me;                 // +0x08
};
struct CAttackingBandit : CEntityScriptBase {
    CQR_EscortTraderScript* ParentClass; // PDB +0x14
    void* PMasterData; // PDB +0x18
    bool Screamed; // PDB +0x1c
    void Init();
};
#if defined(_MSC_VER)
FABLE_STATIC_ASSERT(offsetof(CAttackingBandit, ParentClass) == 0x14);
FABLE_STATIC_ASSERT(offsetof(CAttackingBandit, PMasterData) == 0x18);
FABLE_STATIC_ASSERT(offsetof(CAttackingBandit, Screamed) == 0x1c);
#endif

void CScriptProcessBaseRuntime::RuntimeHookCDD410() {}
void CScriptProcessBaseRuntime::RuntimeHookCDD420() {}
// Native CSpawnedFunc1<CQR_EscortTraderScript, CScriptThing> virtual slot 4 is
// 0x00E1F5E0 (directly copies Par1 into the member callback and sets StopRunning).
// Its member callback thunk is 0x00E1F520. Both implementations remain external. The
// related Archer specialization has different addresses, so its D16780 body is not used here.
CSpawnedFuncPhase3::CSpawnedFuncPhase3(const CCharString& name, unsigned long id,
                                       SpawnedScriptCallback func, CQR_EscortTraderScript* owner,
                                       CScriptThingRuntime par1)
    : CSpawnedFuncBaseRuntime(name, id), Func(func), PClass(owner), Par1(par1)
{
}

#if defined(_MSC_VER)
void* __cdecl operator new(unsigned int bytes); // 0x00BFEA1A
#else
void* operator new(size_t bytes) throw(std::bad_alloc); // host fixture adapter
#endif
CCharString FABLE_FASTCALL operator+(const CCharString& left, const CCharString& right);

void CAttackingBandit::Init()
{
    Screamed = false;
    {
        CCharString combatType("QR_EscortTrader", -1);
        (Game->*((CGameScriptInterfaceVTableView*)Game->__vftable)->EntitySetCombatType.method)(Me, combatType);
    }
    ParentClass->Add(
        new CSpawnedFuncPhase3(
            CCharString("ParentClass.", -1) + CCharString("WatchForBanditDead", -1),
            0, &CQR_EscortTraderScript::WatchForBanditDead, ParentClass, Me),
        CCharString("", -1));
}