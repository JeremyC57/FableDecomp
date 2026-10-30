// CQ_CinemaTestScript::Main() -- the cinema test quest's main: reset the mission flags, bind the "Dummy" entity script, finish adding scripted entities (locally and in the game), then spawn the WatchForTermination and DoMission threads
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00F018B0, length 389 bytes (+ int3 padding), plain `ret`.
// Identity: Ego_r.pdb ?Main@CQ_CinemaTestScript@NScript@@UAEXXZ (the class's members include InitialiseVariables,
// DoMission, WatchForTermination and the CDummy entity script with its static __fastcall Alloc).
// Callees: operator new 0x00BFEA1A, CCharString(const char*, long) 0x0099EBF0 / CCharString(const CCharString&) 0x0099EC30
// / ~CCharString 0x0099EAE0, CScriptBase::AddEntityScriptBinding 0x00CB8230 (landed), CScriptBase::PostAddScriptedEntities
// 0x00CB8930 (sorts the binding map when dirty), CSpawnedFuncBase::CSpawnedFuncBase 0x00CDD450 (CSpawnedFunc vtable
// 0x012F7314), CScriptBase::AddSpawnedFunction 0x00CB7E50, CDummy::Alloc 0x00F01B60, WatchForTermination 0x00F01A40,
// DoMission 0x00F01EB0; the game script interface's PostAddScriptedEntities (slot 64, +0x100).
// Strings: "Dummy" 0x012F6F0C, "WatchForTermination" 0x012C84B0, "DoMission" 0x012CB73C, "" 0x0122D70E.
// Layout: retail = Ego_r.pdb - 0x14: MissionSucceeded +0x48, MissionFailed +0x49, MissionOver +0x4A;
// CEntityScriptBinding<CQ_CinemaTestScript> (0x1C bytes, vtable 0x012F7330, Ego_r.pdb): EntityScriptName +4,
// PParentScript +8, Func +0xC (left unset), ClassAllocFunc +0x10, ClassAllocValid +0x14, Flags +0x18.
// Shape: InitialiseVariables is inline; each name string is a temporary guarded by its own bit of one flag word (VC7.1
// keeps the flags in ebx: 1, 2, 4); the spawns use the Kraken DoMission (0x00DF4E00) inline `const char*` wrapper.
// NScript (sweep 3): the script classes live in namespace NScript (Ego_r/FableWin name sets, e.g.
//   ?IsHeroDressedAsLady@CV_BordelloScript@NScript@@QAE_NXZ; every CV_/CQ_ script class is spelled ...@NScript@@ and
//   class-keyed); this row's class is now declared there (struct->class; already NScript). Same bytes.
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    CCharString(const CCharString& other);              // 0x0099EC30
    ~CCharString();                                     // 0x0099EAE0
};
void* __cdecl operator new(unsigned int size);          // 0x00BFEA1A
// Layout: CSpawnedFuncBase is the retail polymorphic suspendable-process chain from the shared headers
//   (FABLE_RETAIL_SUSPENDABLE_PROCESS opt-in: CSpawnedFuncBase : CSuspendableProcessMicrothread : CASuspendableProcess :
//   CBaseClassNonCopyable : CBaseClass, 0x34 bytes, vtable 0x012C2F9C; ctor 0x00CDD450).
#define FABLE_RETAIL_SUSPENDABLE_PROCESS
#include "engine/CSpawnedFuncBase.h"
template <class T> struct CSpawnedFunc : CSpawnedFuncBase {
    void (T::*Func)();                                  // +0x34
    T* PClass;                                          // +0x38
    CSpawnedFunc(const CCharString& name, unsigned long id, T* pclass, void (T::*func)())
        : CSpawnedFuncBase(name, id), Func(func), PClass(pclass) {}
    virtual void SuspendableProcess() { (PClass->*Func)(); }
};

class CScriptThing;
class CScriptDataBase;
class CEntityScriptBase;
class CActiveEntityScriptBase;
template <class T> class CCountedPointer { public: T* Data; void* Info; };

class __declspec(novtable) CEntityScriptBindingBase { public:
    virtual const CCharString& GetEntityScriptName() const = 0;
};
template <class T> struct CEntityScriptBinding : CEntityScriptBindingBase {
    typedef CCountedPointer<CEntityScriptBase> (__fastcall *TAllocFunc)(T& parent, CScriptDataBase* data, const CScriptThing& thing);
    CCharString     EntityScriptName;                   // +0x04
    T*              PParentScript;                      // +0x08
    void (T::*Func)();                                  // +0x0c
    TAllocFunc      ClassAllocFunc;                     // +0x10
    bool            ClassAllocValid;                    // +0x14
    unsigned long   Flags;                              // +0x18
    CEntityScriptBinding(const CCharString& name, T* parent, TAllocFunc alloc, unsigned long flags)
        : EntityScriptName(name), PParentScript(parent), ClassAllocFunc(alloc), ClassAllocValid(true), Flags(flags) {}
    virtual const CCharString& GetEntityScriptName() const { return EntityScriptName; }
};

// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"

namespace NScript {
}   // namespace NScript (CScriptBase is global in retail)
class CScriptBase { public:                                    // retail 0x40 bytes
    unsigned char _body[0x40];
    void AddEntityScriptBinding(CEntityScriptBindingBase* binding);                   // 0x00CB8230
    void PostAddScriptedEntities();                                                   // 0x00CB8930
    void SpawnFunction(CSpawnedFuncBase* func, const char* regionName = "") { AddSpawnedFunction(func, CCharString(regionName)); }
public:
    void AddSpawnedFunction(CSpawnedFuncBase*, const CCharString&);   // 0x00CB7E50
};
namespace NScript {
class CQ_CinemaTestScript : public CScriptBase {
public:
    struct CDummy {
        static CCountedPointer<CEntityScriptBase> __fastcall Alloc(CQ_CinemaTestScript& parent, CScriptDataBase* data, const CScriptThing& thing);   // 0x00F01B60
    };
    CGameScriptInterface* GSI;                          // +0x40
    void*                 Data;                         // +0x44
    bool                  MissionSucceeded;             // +0x48 (PDB +0x5c)
    bool                  MissionFailed;                // +0x49
    bool                  MissionOver;                  // +0x4a
    void InitialiseVariables()
    {
        MissionSucceeded = false;
        MissionFailed = false;
        MissionOver = false;
    }
    void Main();
    void WatchForTermination();                         // 0x00F01A40
    void DoMission();                                   // 0x00F01EB0
};
void CQ_CinemaTestScript::Main()
{
    InitialiseVariables();
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_CinemaTestScript>("Dummy", this, &CDummy::Alloc, 0));
    PostAddScriptedEntities();
    GSI->PostAddScriptedEntities();
    SpawnFunction(new CSpawnedFunc<CQ_CinemaTestScript>("WatchForTermination", 0, this, &CQ_CinemaTestScript::WatchForTermination));
    SpawnFunction(new CSpawnedFunc<CQ_CinemaTestScript>("DoMission", 0, this, &CQ_CinemaTestScript::DoMission));
}
}