// NScript::CQ_BountyHuntScript::CBH_GangDeputy2::Init() -- register the parent script's WatchForDeputy2Killed callback with the copied script thing
// Leaf: ?Init@CBH_GangDeputy2@CQ_BountyHuntScript@NScript@@EAEXXZ
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8). Also: the local CCPPointerInfo members References->RefCount are the header CCPPointerInfo's (same offsets).
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Revisit: two-view-CScriptThing -- byte-exact, but models CScriptThing as two classes (inline and out-of-line copy/dtor) where retail had one; merge once the retail mechanism is found (owner 2026-11-02; docs/pipeline/INLINE_DEPTH_REVISIT.md).
// Naming wave 29 (family sfbr): the thread class derives from the retail CSpawnedFuncBase (engine/CSpawnedFuncBase.h; Ego_r.pdb / FableWin.pdb
//   ??0CSpawnedFuncBase@@QAE@ABVCCharString@@K@Z 0x00CDD450, implicit ??1CSpawnedFuncBase@@UAE@XZ 0x00CDD4C0). Was the invented
//   CScriptProcessBaseRuntime / CSpawnedFuncBaseRuntime pair (virtual ~CSpawnedFuncBaseRuntime, RuntimeHookCDD410 / CDD420).
// Retail layout: PDB CBH_GangDeputy2 fields ParentClass +0x14, PMasterData +0x18; its inherited CScriptThing is +0x08.
// Retail object is CSpawnedFunc1<CQ_AmbushTradersScript, CScriptThing>, size 0x48: base 0x34, Func +0x34, PClass +0x38, Par1 +0x3c. The nested new-expression keeps the callback-name temporaries through Add while the by-value trigger parameter is destroyed after construction and before Add.
// Naming wave 35: identity reg 00d13799 "BH_GangDeputy2" in 00d13580; Alloc 00d161a0; vtable 012ca58c slot 2; PDB ?Init@CBH_GangDeputy2@CQ_BountyHuntScript@NScript@@; refs=1; Anniversary RTTI agrees (confirmed); replaces CAT_Phase3Archer::Init: CAT_Phase3Archer::Init is at 00f09420 (its own vtable slot) (wave3/labels_alloc.tsv); PDB ?Init@CBH_GangDeputy2@CQ_BountyHuntScript@NScript@@EAEXXZ in Ego_r.pdb / FableWin.pdb.
//   Was `?Init@CAT_Phase3Archer@@QAEXXZ`; this source now defines the PDB member (vptr placeholder dropped (own); class CAT_Phase3Archer renamed CBH_GangDeputy2); same bytes.
#include <new>
#include "fable_string.h"
#include "engine/CScriptThing.h"

#include "engine/CCountedPointer.h"
#include "engine/C3DVector.h"

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
    { if (PImp_Info) ++PImp_Info->RefCount; }
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
{ if (PImp_Info) ++PImp_Info->RefCount; }

// CSpawnedFuncBase: the retail model of engine/CSpawnedFuncBase.h (FABLE_RETAIL_SUSPENDABLE_PROCESS: : CSuspendableProcessMicrothread
//   : CASuspendableProcess : CBaseClassNonCopyable : CBaseClass, 0x34, vtable 0x012C2F9C; ctor 0x00CDD450, implicit dtor 0x00CDD4C0).
#define FABLE_RETAIL_SUSPENDABLE_PROCESS
#include "engine/CSpawnedFuncBase.h"

struct CSpawnedFuncPhase3;
struct CQ_AmbushTradersScript {
    void WatchForDeputy2Killed(CScriptThingRuntime thing);
    void Add(CSpawnedFuncPhase3* function, const CCharString& name); // 0x00CB7E50; observed function at +4, name at +8
};
typedef void (CQ_AmbushTradersScript::*SpawnedScriptCallback)(CScriptThingRuntime);
struct CSpawnedFuncPhase3 : CSpawnedFuncBase {
    SpawnedScriptCallback Func;
    CQ_AmbushTradersScript* PClass;
    CScriptThingStored Par1;
    virtual void SuspendableProcess(); // 0x00D16780
    CSpawnedFuncPhase3(const CCharString& name, unsigned long id,
                       SpawnedScriptCallback func, CQ_AmbushTradersScript* owner,
                       CScriptThingRuntime par1);
};
#if defined(_MSC_VER)
FABLE_STATIC_ASSERT(sizeof(CSpawnedFuncPhase3) == 0x48);
#endif

namespace NScript {
class CQ_BountyHuntScript {    // the PDB owner script; only its nested entity class is modelled here
public:
    class CBH_GangDeputy2;
};
}
using NScript::CQ_BountyHuntScript;
namespace NScript {
class CQ_BountyHuntScript::CBH_GangDeputy2 {
public:
        void* GSI;
    CScriptThingRuntime Thing;
    CQ_AmbushTradersScript* ParentClass; // PDB +0x14
    void* PMasterData;                   // PDB +0x18
    long lastRandomSpeechIdx1;            // PDB +0x1c
    long lastRandomSpeechIdx2;            // PDB +0x20
private:
    virtual void Init();                          // PDB ?Init@CBH_GangDeputy2@CQ_BountyHuntScript@NScript@@EAEXXZ
};
}
typedef NScript::CQ_BountyHuntScript::CBH_GangDeputy2 CBH_GangDeputy2;    // global spelling used below
#if defined(_MSC_VER)
FABLE_STATIC_ASSERT(offsetof(CBH_GangDeputy2, ParentClass) == 0x14);
FABLE_STATIC_ASSERT(offsetof(CBH_GangDeputy2, PMasterData) == 0x18);
#endif

// Runtime slot body is external: retail D16780 copies Par1 directly into the
// callback's by-value argument, then sets StopRunning. The callback's native
// copy/destruction behavior is not synthesized into this Init translation unit.
CSpawnedFuncPhase3::CSpawnedFuncPhase3(const CCharString& name, unsigned long id,
                                       SpawnedScriptCallback func, CQ_AmbushTradersScript* owner,
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

void NScript::CQ_BountyHuntScript::CBH_GangDeputy2::Init()
{
    ParentClass->Add(
        new CSpawnedFuncPhase3(
            CCharString("ParentClass.", -1) + CCharString("WatchForDeputy2Killed", -1),
            0, &CQ_AmbushTradersScript::WatchForDeputy2Killed, ParentClass, Thing),
        CCharString("", -1));
}