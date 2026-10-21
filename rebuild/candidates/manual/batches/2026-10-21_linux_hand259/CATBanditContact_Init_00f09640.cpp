// CAT_BanditContact::Init() -- register the parent script's WatchForHeroAttackOnContact and WatchForContactDeath
// callbacks (spawned functions over a copy of the contact's script thing) with CQ_AmbushTradersScript::Add
// Leaf: Init
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00F09640, length 494 bytes.
// Callees: operator new @ 0x00BFEA1A (0x48), CCharString(const char*, long) @ 0x0099EBF0 / ~CCharString @ 0x0099EAE0,
//          operator+(const CCharString&, const CCharString&) @ 0x0099F570, CSpawnedFuncBase ctor @ 0x00CDD450,
//          CScriptThing copy @ 0x004ABE90 / dtor @ 0x004AA840, CQ_AmbushTradersScript::Add @ 0x00CB7E50. Strings
//          "ParentClass." 0x012C9934, "WatchForHeroAttackOnContact" 0x012F815C, "WatchForContactDeath" 0x012F8144,
//          "" 0x0122D70E. Spawned-func vtable 0x012F7EBC (SuspendableProcess 0x00F06370), callbacks 0x00F09950 /
//          0x00F09830.
// Layout: model copied from the landed CAT_Phase3Archer::Init (0x00D169E0): Ego_r.pdb CAT_BanditContact ParentClass
//         +0x14, PMasterData +0x18; CScriptThing at +8; CSpawnedFunc1<CQ_AmbushTradersScript, CScriptThing> 0x48 bytes.
// Shape: two copies of the landed nested new-expression (callback-name temporaries live through Add, flag bits 1/2/4
//         then 8/0x10/0x20 in ebx).
#include <new>
#include "fable_string.h"
#include "engine/CSpawnedFuncBase.h"
#include "engine/CScriptThing.h"
#include "engine/CASuspendableProcess.h"

struct CCPPointerInfo { long References; };
struct C3DVector { float X, Y, Z; };

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

struct CSpawnedFuncAmbush;
struct CQ_AmbushTradersScript {
    void WatchForHeroAttackOnContact(CScriptThingRuntime thing);
    void WatchForContactDeath(CScriptThingRuntime thing);
    void Add(CSpawnedFuncAmbush* function, const CCharString& name); // 0x00CB7E50; observed function at +4, name at +8
};
typedef void (CQ_AmbushTradersScript::*SpawnedScriptCallback)(CScriptThingRuntime);
struct CSpawnedFuncAmbush : CSpawnedFuncBaseRuntime {
    SpawnedScriptCallback Func;
    CQ_AmbushTradersScript* PClass;
    CScriptThingStored Par1;
    virtual void SuspendableProcess(); // 0x00F06370 (vtable 0x012F7EBC)
    CSpawnedFuncAmbush(const CCharString& name, unsigned long id,
                       SpawnedScriptCallback func, CQ_AmbushTradersScript* owner,
                       CScriptThingRuntime par1);
};
#if defined(_MSC_VER)
FABLE_STATIC_ASSERT(sizeof(CSpawnedFuncAmbush) == 0x48);
#endif

struct CAT_BanditContact {
    void* __vftable;
    void* GSI;
    CScriptThingRuntime Thing;
    CQ_AmbushTradersScript* ParentClass; // PDB +0x14
    void* PMasterData;                   // PDB +0x18
        void Init();
};
#if defined(_MSC_VER)
FABLE_STATIC_ASSERT(offsetof(CAT_BanditContact, ParentClass) == 0x14);
FABLE_STATIC_ASSERT(offsetof(CAT_BanditContact, PMasterData) == 0x18);
#endif

void CScriptProcessBaseRuntime::RuntimeHookCDD410() {}
void CScriptProcessBaseRuntime::RuntimeHookCDD420() {}
// Runtime slot body is external: retail D16780 copies Par1 directly into the
// callback's by-value argument, then sets StopRunning. The callback's native
// copy/destruction behavior is not synthesized into this Init translation unit.
CSpawnedFuncAmbush::CSpawnedFuncAmbush(const CCharString& name, unsigned long id,
                                       SpawnedScriptCallback func, CQ_AmbushTradersScript* owner,
                                       CScriptThingRuntime par1)
    : CSpawnedFuncBaseRuntime(name, id), Func(func), PClass(owner), Par1(par1)
{
}

CCharString FABLE_FASTCALL operator+(const CCharString& left, const CCharString& right);

void CAT_BanditContact::Init()
{
    ParentClass->Add(
        new CSpawnedFuncAmbush(
            CCharString("ParentClass.", -1) + CCharString("WatchForHeroAttackOnContact", -1),
            0, &CQ_AmbushTradersScript::WatchForHeroAttackOnContact, ParentClass, Thing),
        CCharString("", -1));
    ParentClass->Add(
        new CSpawnedFuncAmbush(
            CCharString("ParentClass.", -1) + CCharString("WatchForContactDeath", -1),
            0, &CQ_AmbushTradersScript::WatchForContactDeath, ParentClass, Thing),
        CCharString("", -1));
}
