// CAT_Phase3Archer::Init() -- register the parent script's WatchForDeputy2Killed callback with the copied script thing
// Revisit: two-view-CScriptThing -- byte-exact, but models CScriptThing as two classes (inline and out-of-line copy/dtor) where retail had one; merge once the retail mechanism is found (owner 2026-11-02; docs/pipeline/INLINE_DEPTH_REVISIT.md).
// Naming wave 29 (family sfbr): the thread class derives from the retail CSpawnedFuncBase (engine/CSpawnedFuncBase.h; Ego_r.pdb / FableWin.pdb
//   ??0CSpawnedFuncBase@@QAE@ABVCCharString@@K@Z 0x00CDD450, implicit ??1CSpawnedFuncBase@@UAE@XZ 0x00CDD4C0). Was the invented
//   CScriptProcessBaseRuntime / CSpawnedFuncBaseRuntime pair (virtual ~CSpawnedFuncBaseRuntime, RuntimeHookCDD410 / CDD420).
// Retail layout: PDB CAT_Phase3Archer fields ParentClass +0x14, PMasterData +0x18; its inherited CScriptThing is +0x08.
// Retail object is CSpawnedFunc1<CQ_AmbushTradersScript, CScriptThing>, size 0x48: base 0x34, Func +0x34, PClass +0x38, Par1 +0x3c. The nested new-expression keeps the callback-name temporaries through Add while the by-value trigger parameter is destroyed after construction and before Add.
#include <new>
#include "fable_string.h"
#include "engine/CScriptThing.h"

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

template <class T> class CCountedPointer {             // CSpawnedFuncBase::PPredicate (header contract; this row keeps its own
public:                                                //   CCPPointerInfo, so not engine/CCountedPointer.h); the dtor is out of line
    T* Data; CCPPointerInfo* Info;                     //   in retail (CCountedPointer<const NScript::CPredicateBase> 0x00CDD4F0)
    ~CCountedPointer();
};
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

struct CAT_Phase3Archer {
    void* __vftable;
    void* GSI;
    CScriptThingRuntime Thing;
    CQ_AmbushTradersScript* ParentClass; // PDB +0x14
    void* PMasterData;                   // PDB +0x18
    long lastRandomSpeechIdx1;            // PDB +0x1c
    long lastRandomSpeechIdx2;            // PDB +0x20
    void Init();
};
#if defined(_MSC_VER)
FABLE_STATIC_ASSERT(offsetof(CAT_Phase3Archer, ParentClass) == 0x14);
FABLE_STATIC_ASSERT(offsetof(CAT_Phase3Archer, PMasterData) == 0x18);
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

void CAT_Phase3Archer::Init()
{
    ParentClass->Add(
        new CSpawnedFuncPhase3(
            CCharString("ParentClass.", -1) + CCharString("WatchForDeputy2Killed", -1),
            0, &CQ_AmbushTradersScript::WatchForDeputy2Killed, ParentClass, Thing),
        CCharString("", -1));
}