// CV_FisticuffsClubScript::Main() -- the Fisticuffs Club quest main: initialise the state, bind the FisticuffsClubRing / FisticuffsMember / Tyler / RingFighter entity scripts and spawn the KeepVariablesUpToDate and ActivateFisticuffsAtNight threads
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E82220, 0x2ab bytes, plain `ret`.
// Callees: InitialiseVariables 0x00E82650, operator new 0x00BFEA1A, CCharString 0x0099EBF0 / 0x0099EC30 / ~ 0x0099EAE0,
//   CSpawnedFuncBase 0x00CDD450 (CSpawnedFunc<CV_FisticuffsClubScript> vtable 0x012EAF28), AddSpawnedFunction 0x00CB7E50,
//   AddEntityScriptBinding 0x00CB8230 (binding vtable 0x012EAFEC), CScriptBase::PostAddScriptedEntities 0x00CB8930
//   (manifest "CTCVillage::OnInitialActivate" is propagated); allocators 0x00E82870 / 0x00E838A0 / 0x00E83570 / 0x00E83780;
//   threads KeepVariablesUpToDate 0x00E824E0, ActivateFisticuffsAtNight 0x00E83D50; CGameScriptInterface slot 64.
// Shape: CQ_GuildTrainingWoodsMeleeScript::Main 0x00D66620 binding model; the spawns use the inline SpawnFunction
//   (region temporary built after operator new).
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
class CV_FisticuffsClubScript : public CScriptBase {
public:
    struct CFisticuffsClubRing {
        static CCountedPointer<CEntityScriptBase> __fastcall Alloc(CV_FisticuffsClubScript& parent, CScriptDataBase* data, const CScriptThing& thing);   // 0x00E82870
    };
    struct CFisticuffsMember {
        static CCountedPointer<CEntityScriptBase> __fastcall Alloc(CV_FisticuffsClubScript& parent, CScriptDataBase* data, const CScriptThing& thing);   // 0x00E838A0
    };
    struct CTyler {
        static CCountedPointer<CEntityScriptBase> __fastcall Alloc(CV_FisticuffsClubScript& parent, CScriptDataBase* data, const CScriptThing& thing);   // 0x00E83570
    };
    struct CRingFighter {
        static CCountedPointer<CEntityScriptBase> __fastcall Alloc(CV_FisticuffsClubScript& parent, CScriptDataBase* data, const CScriptThing& thing);   // 0x00E83780
    };
    CGameScriptInterface* Game;                          // +0x40
    void* PMasterData;                                   // +0x44
    void Main();
    void InitialiseVariables();                          // 0x00E82650
    void KeepVariablesUpToDate();                        // 0x00E824E0
    void ActivateFisticuffsAtNight();                    // 0x00E83D50
};
} // namespace NScript
using NScript::CV_FisticuffsClubScript;

void NScript::CV_FisticuffsClubScript::Main()
{
    InitialiseVariables();
    AddEntityScriptBinding(new CEntityScriptBinding<CV_FisticuffsClubScript>("FisticuffsClubRing", this, &CFisticuffsClubRing::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CV_FisticuffsClubScript>("FisticuffsMember", this, &CFisticuffsMember::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CV_FisticuffsClubScript>("Tyler", this, &CTyler::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CV_FisticuffsClubScript>("RingFighter", this, &CRingFighter::Alloc, 0));
    PostAddScriptedEntities();
    Game->PostAddScriptedEntities();
    SpawnFunction(new CSpawnedFunc<CV_FisticuffsClubScript>("KeepVariablesUpToDate", 0, this, &NScript::CV_FisticuffsClubScript::KeepVariablesUpToDate));
    SpawnFunction(new CSpawnedFunc<CV_FisticuffsClubScript>("ActivateFisticuffsAtNight", 0, this, &NScript::CV_FisticuffsClubScript::ActivateFisticuffsAtNight));
}