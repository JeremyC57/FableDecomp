// CV_SingingStonesScript::Main() -- the Singing Stones quest main: wait for the Witchwood2 region to load, bind the SingingStone / ManWithDoorName entity scripts and spawn the WatchForCompleteTune, WatchForSpokenToDemonDoors and WatchForRegionLeaving threads
// Retail: Fable.exe (TLC, VC7.1) @ 0x00ED10A0, 0x2f9 bytes, plain `ret`.
// Callees: CCharString 0x0099EBF0 / 0x0099EC30 / ~ 0x0099EAE0, IsActiveThreadTerminating 0x00CB7940, operator new 0x00BFEA1A,
//   CSpawnedFuncBase 0x00CDD450 (CSpawnedFunc<CV_SingingStonesScript> vtable 0x012F2B20), AddSpawnedFunction 0x00CB7E50,
//   AddEntityScriptBinding 0x00CB8230 (binding vtable 0x012F2BE8), CScriptBase::PostAddScriptedEntities 0x00CB8930;
//   allocators 0x00ED1420 / 0x00ED16E0; threads WatchForCompleteTune 0x00ED2BB0, WatchForSpokenToDemonDoors 0x00ED13B0,
//   WatchForRegionLeaving 0x00ED2A50; CGameScriptInterface slots 7 NewScriptFrame, 12 IsRegionLoaded, 64 PostAddScriptedEntities.
// Shape: translator WAIT_UNTIL loop, then the CQ_GuildTrainingWoodsMeleeScript::Main 0x00D66620 binding model; the spawns
//   use the inline SpawnFunction (region temporary built after operator new).
// NScript (sweep 3): the script classes live in namespace NScript (Ego_r/FableWin name sets, e.g.
//   ?IsHeroDressedAsLady@CV_BordelloScript@NScript@@QAE_NXZ; every CV_/CQ_ script class is spelled ...@NScript@@ and
//   class-keyed); this row's class is now declared there (wrapped, struct->class). Same bytes.
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    CCharString(const CCharString& other);              // 0x0099EC30
    ~CCharString();                                     // 0x0099EAE0
};
// Layout: CSpawnedFuncBase is the retail polymorphic suspendable-process chain from the shared headers
//   (FABLE_RETAIL_SUSPENDABLE_PROCESS opt-in: CSpawnedFuncBase : CSuspendableProcessMicrothread : CASuspendableProcess :
//   CBaseClassNonCopyable : CBaseClass, 0x34 bytes, vtable 0x012C2F9C; ctor 0x00CDD450).
#define FABLE_RETAIL_SUSPENDABLE_PROCESS
#include "engine/CSpawnedFuncBase.h"
template <class T> struct CSpawnedFunc : CSpawnedFuncBase {
    void (T::*Func)();                                  // +0x34
    T* PClass;                                          // +0x38
    __forceinline CSpawnedFunc(const CCharString& name, unsigned long id, T* pclass, void (T::*func)())
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

class CGameScriptInterface;
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
class CScriptBase { public:                                     // retail 0x40 bytes
    unsigned char _body[0x40];
    bool IsActiveThreadTerminating();                                                 // 0x00CB7940
    void AddSpawnedFunction(CSpawnedFuncBase* func, const CCharString& regionName);   // 0x00CB7E50
    void AddEntityScriptBinding(CEntityScriptBindingBase* binding);                   // 0x00CB8230
    void PostAddScriptedEntities();                                                   // 0x00CB8930
    __forceinline void SpawnFunction(CSpawnedFuncBase* func, const char* regionName = "") { AddSpawnedFunction(func, CCharString(regionName)); }
};
namespace NScript {
class CV_SingingStonesScript : public CScriptBase {
public:
    struct CSingingStone {
        static CCountedPointer<CEntityScriptBase> __fastcall Alloc(CV_SingingStonesScript& parent, CScriptDataBase* data, const CScriptThing& thing);   // 0x00ED1420
    };
    struct CManWithDoorName {
        static CCountedPointer<CEntityScriptBase> __fastcall Alloc(CV_SingingStonesScript& parent, CScriptDataBase* data, const CScriptThing& thing);   // 0x00ED16E0
    };
    CGameScriptInterface* Game;                          // +0x40
    void* PMasterData;                                   // +0x44
    void Main();
    void WatchForCompleteTune();                         // 0x00ED2BB0
    void WatchForSpokenToDemonDoors();                   // 0x00ED13B0
    void WatchForRegionLeaving();                        // 0x00ED2A50
};
} // namespace NScript
using NScript::CV_SingingStonesScript;

void NScript::CV_SingingStonesScript::Main()
{
    while (!Game->IsRegionLoaded("Witchwood2")) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    AddEntityScriptBinding(new CEntityScriptBinding<CV_SingingStonesScript>("SingingStone", this, &CSingingStone::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CV_SingingStonesScript>("ManWithDoorName", this, &CManWithDoorName::Alloc, 0));
    PostAddScriptedEntities();
    Game->PostAddScriptedEntities();
    SpawnFunction(new CSpawnedFunc<CV_SingingStonesScript>("WatchForCompleteTune", 0, this, &NScript::CV_SingingStonesScript::WatchForCompleteTune));
    SpawnFunction(new CSpawnedFunc<CV_SingingStonesScript>("WatchForSpokenToDemonDoors", 0, this, &NScript::CV_SingingStonesScript::WatchForSpokenToDemonDoors));
    SpawnFunction(new CSpawnedFunc<CV_SingingStonesScript>("WatchForRegionLeaving", 0, this, &NScript::CV_SingingStonesScript::WatchForRegionLeaving));
}