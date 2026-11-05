// NScript::CQR_EscortTraderScript::CAttackingBandit::Init() -- mark the trader group and register WatchForBanditDead on ParentClass
// Leaf: ?Init@CAttackingBandit@CQR_EscortTraderScript@NScript@@EAEXXZ
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8). Also: the local CCPPointerInfo members References->RefCount are the header CCPPointerInfo's (same offsets).
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Revisit: two-view-CScriptThing -- byte-exact, but models CScriptThing as two classes (inline and out-of-line copy/dtor) where retail had one; merge once the retail mechanism is found (owner 2026-11-02; docs/pipeline/INLINE_DEPTH_REVISIT.md).
// Naming wave 29 (family sfbr): the thread class derives from the retail CSpawnedFuncBase (engine/CSpawnedFuncBase.h; Ego_r.pdb / FableWin.pdb
//   ??0CSpawnedFuncBase@@QAE@ABVCCharString@@K@Z 0x00CDD450, implicit ??1CSpawnedFuncBase@@UAE@XZ 0x00CDD4C0). Was the invented
//   CScriptProcessBaseRuntime / CSpawnedFuncBaseRuntime pair (virtual ~CSpawnedFuncBaseRuntime, RuntimeHookCDD410 / CDD420).
// Native 0x00E1F3E0, 0x132 bytes; PDB fields ParentClass +0x14, PMasterData +0x18, Screamed +0x1c.
// Retail object is CSpawnedFunc1<CQR_EscortTraderScript, CScriptThing>, size 0x48: base 0x34, Func +0x34, PClass +0x38, Par1 +0x3c. The nested new-expression keeps the callback-name temporaries through Add while the by-value trigger parameter is destroyed after construction and before Add.
// Naming wave 35: identity reg 00e1e1c0 "AttackingBandit" in 00e1e110; Alloc 00e1ed50; vtable 012e1554 slot 2; PDB ?Init@CAttackingBandit@CQR_EscortTraderScript@NScript@@; refs=1; Anniversary RTTI agrees (confirmed); was under-qualified CAttackingBandit::Init (wave3/labels_alloc.tsv); PDB ?Init@CAttackingBandit@CQR_EscortTraderScript@NScript@@EAEXXZ in Ego_r.pdb / FableWin.pdb.
//   Was `?Init@CAttackingBandit@@QAEXXZ`; this source now defines the PDB member (vptr placeholder dropped (base CEntityScriptBase made polymorphic); outer stand-in CQR_EscortTraderScript moved into NScript); same bytes.
#include <new>
#include "fable_string.h"
#include "engine/CScriptThing.h"
#include "engine/CGameScriptInterface.h"

#include "engine/CCountedPointer.h"
#include "engine/C3DVector.h"

struct CScriptThingRuntime;
struct CCountedScriptThingRuntime {
    CScriptThingRuntime* Data;
    CCPPointerInfo* Info;
    CCountedScriptThingRuntime() : Data(0), Info(0) {}
    CCountedScriptThingRuntime(const CCountedScriptThingRuntime& other) {
        Data = other.Data;
        Info = other.Info;
        if (Info) ++Info->RefCount;
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
{ PImp.Data=other.PImp_Data; PImp.Info=other.PImp_Info; if (PImp.Info) ++PImp.Info->RefCount; }

// CSpawnedFuncBase: the retail model of engine/CSpawnedFuncBase.h (FABLE_RETAIL_SUSPENDABLE_PROCESS: : CSuspendableProcessMicrothread
//   : CASuspendableProcess : CBaseClassNonCopyable : CBaseClass, 0x34, vtable 0x012C2F9C; ctor 0x00CDD450, implicit dtor 0x00CDD4C0).
#define FABLE_RETAIL_SUSPENDABLE_PROCESS
#include "engine/CSpawnedFuncBase.h"

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
namespace NScript {
class CQR_EscortTraderScript {
public:
    void WatchForBanditDead(CScriptThingRuntime thing);
    void Add(CSpawnedFuncPhase3* function, const CCharString& name); // 0x00CB7E50; observed function at +4, name at +8
public:
    class CAttackingBandit;          // the entity class below (naming wave 35)
};
}
using NScript::CQR_EscortTraderScript;
typedef void (CQR_EscortTraderScript::*SpawnedScriptCallback)(CScriptThingRuntime);
struct CSpawnedFuncPhase3 : CSpawnedFuncBase {
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

class CEntityScriptBase { public:                 // donor layout: +0 vptr, +4 Game, +8 Me
    virtual ~CEntityScriptBase();   // retail slot 0 (was the vptr placeholder: the stand-in is polymorphic, as retail)
    CGameScriptInterface* Game;             // +0x04
    CScriptThingRuntime Me;                 // +0x08
};
namespace NScript {
class CQR_EscortTraderScript::CAttackingBandit : public CEntityScriptBase {
public:
    CQR_EscortTraderScript* ParentClass; // PDB +0x14
    void* PMasterData; // PDB +0x18
    bool Screamed; // PDB +0x1c
private:
    virtual void Init();                          // PDB ?Init@CAttackingBandit@CQR_EscortTraderScript@NScript@@EAEXXZ
};
}
typedef NScript::CQR_EscortTraderScript::CAttackingBandit CAttackingBandit;    // global spelling used below
#if defined(_MSC_VER)
FABLE_STATIC_ASSERT(offsetof(CAttackingBandit, ParentClass) == 0x14);
FABLE_STATIC_ASSERT(offsetof(CAttackingBandit, PMasterData) == 0x18);
FABLE_STATIC_ASSERT(offsetof(CAttackingBandit, Screamed) == 0x1c);
#endif

// Native CSpawnedFunc1<CQR_EscortTraderScript, CScriptThing> virtual slot 4 is
// 0x00E1F5E0 (directly copies Par1 into the member callback and sets StopRunning).
// Its member callback thunk is 0x00E1F520. Both implementations remain external. The
// related Archer specialization has different addresses, so its D16780 body is not used here.
CSpawnedFuncPhase3::CSpawnedFuncPhase3(const CCharString& name, unsigned long id,
                                       SpawnedScriptCallback func, CQR_EscortTraderScript* owner,
                                       CScriptThingRuntime par1)
    : CSpawnedFuncBase(name, id), Func(func), PClass(owner), Par1(par1)
{
}

#if defined(_MSC_VER)
void* __cdecl operator new(unsigned int bytes); // 0x00BFEA1A
#else
void* operator new(size_t bytes) throw(std::bad_alloc); // host fixture adapter
#endif
CCharString FABLE_FASTCALL operator+(const CCharString& left, const CCharString& right);

void NScript::CQR_EscortTraderScript::CAttackingBandit::Init()
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