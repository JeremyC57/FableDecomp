// CV_KnotholeGladeGatesScript::Main() -- reset the quest flags, bind the KGG_VillagerOutside / KGG_SurpriseBalverine entity scripts, then spawn WatchForTermination and DoMission in Knothole Glade
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E9FCD0, 493 bytes (496 with padding), plain `ret`.
// Callees: the same-TU flag reset 0x00E9FEC0 (clears +0x48..+0x54, +0x58, +0x68; no Ego_r name, called here
//   ResetMissionState; the manifest's "CMeleeApprentice::Init" is propagated), operator new 0x00BFEA1A,
//   CCharString(const char*, long) 0x0099EBF0 / CCharString(const CCharString&) 0x0099EC30 / ~CCharString 0x0099EAE0,
//   CScriptBase::AddEntityScriptBinding 0x00CB8230, CScriptBase::PostAddScriptedEntities 0x00CB8930,
//   CScriptBase::AddSpawnedFunction 0x00CB7E50, CSpawnedFuncBase 0x00CDD450 (CSpawnedFunc vtable 0x012ED9E8);
//   the nested entity scripts' static __fastcall Alloc functions CKGG_VillagerOutside 0x00E9FF70 and
//   CKGG_SurpriseBalverine 0x00EA09F0, the spawned WatchForTermination 0x00E9FEF0 and DoMission 0x00EA1090; through the
//   interface (+0x40) PostAddScriptedEntities (slot 64).
// Strings: "KGG_VillagerOutside" 0x012EDA24, "KGG_SurpriseBalverine" 0x012EDA04, "KnotholeGlade" 0x012C513C,
//   "WatchForTermination" 0x012C84B0, "DoMission" 0x012CB73C.
// Layout: CEntityScriptBinding<CV_KnotholeGladeGatesScript> (0x1C bytes, vtable 0x012EDA1C) and CSpawnedFunc (0x3C) as
//   the landed CQ_DragonBossFightScript::Main 0x00D255E0; both bindings pass Flags 1.
// Shape: name strings inside new-expressions are conditional temporaries (one ebx flag bit each); the region name is
//   the implicit conversion of AddSpawnedFunction's second argument, built before the new-expression.
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
public:
    void AddSpawnedFunction(CSpawnedFuncBase*, const CCharString&);   // 0x00CB7E50
};
namespace NScript {
class CV_KnotholeGladeGatesScript : public CScriptBase {
public:
    struct CKGG_VillagerOutside {
        static CCountedPointer<CEntityScriptBase> __fastcall Alloc(CV_KnotholeGladeGatesScript& parent, CScriptDataBase* data, const CScriptThing& thing);   // 0x00E9FF70
    };
    struct CKGG_SurpriseBalverine {
        static CCountedPointer<CEntityScriptBase> __fastcall Alloc(CV_KnotholeGladeGatesScript& parent, CScriptDataBase* data, const CScriptThing& thing);   // 0x00EA09F0
    };
    CGameScriptInterface* Game;                         // +0x40
    void*                 Data;                         // +0x44
    void ResetMissionState();                           // 0x00E9FEC0
    void WatchForTermination();                         // 0x00E9FEF0
    void DoMission();                                   // 0x00EA1090
    void Main();
};
void CV_KnotholeGladeGatesScript::Main()
{
    ResetMissionState();
    AddEntityScriptBinding(new CEntityScriptBinding<CV_KnotholeGladeGatesScript>("KGG_VillagerOutside", this, &CKGG_VillagerOutside::Alloc, 1));
    AddEntityScriptBinding(new CEntityScriptBinding<CV_KnotholeGladeGatesScript>("KGG_SurpriseBalverine", this, &CKGG_SurpriseBalverine::Alloc, 1));
    PostAddScriptedEntities();
    Game->PostAddScriptedEntities();
    AddSpawnedFunction(new CSpawnedFunc<CV_KnotholeGladeGatesScript>("WatchForTermination", 0, this, &CV_KnotholeGladeGatesScript::WatchForTermination), "KnotholeGlade");
    AddSpawnedFunction(new CSpawnedFunc<CV_KnotholeGladeGatesScript>("DoMission", 0, this, &CV_KnotholeGladeGatesScript::DoMission), "KnotholeGlade");
}
}