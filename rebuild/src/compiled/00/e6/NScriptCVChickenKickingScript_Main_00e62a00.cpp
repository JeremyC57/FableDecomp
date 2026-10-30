// CV_ChickenKickingScript::Main() -- the Chicken Kicking quest main: wait for the OakBay region to load, bind the ChickenMaster / ChickenSign / KickedChicken / Spectator entity scripts and spawn the CreateSpectators and LookAfterOrganiser threads
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E62A00, 0x347 bytes, plain `ret`.
// Callees: CCharString 0x0099EBF0 / 0x0099EC30 / ~ 0x0099EAE0, IsActiveThreadTerminating 0x00CB7940, operator new 0x00BFEA1A,
//   CSpawnedFuncBase 0x00CDD450 (CSpawnedFunc<CV_ChickenKickingScript> vtable 0x012E732C), AddSpawnedFunction 0x00CB7E50,
//   AddEntityScriptBinding 0x00CB8230 (binding vtable 0x012E73C8), CScriptBase::PostAddScriptedEntities 0x00CB8930;
//   allocators 0x00E634B0 / 0x00E64D10 / 0x00E63590 / 0x00E63660 (all bound with flags 1); threads CreateSpectators
//   0x00E62D60, LookAfterOrganiser 0x00E631F0; CGameScriptInterface slots 7, 12 IsRegionLoaded, 64.
// Shape: same as CV_SingingStonesScript::Main 0x00ED10A0 (WAIT_UNTIL, binding model, inline SpawnFunction).
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
#include "engine/CSpawnedFunc.h"                         // vtable per instantiation; ctor inline, slot 4 SuspendableProcess

class CScriptThing;
class CScriptDataBase;
class CEntityScriptBase;
class CActiveEntityScriptBase;
#include "engine/CCountedPointer.h"

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
// CScriptBase: the retail NScript model from the shared headers (FABLE_RETAIL_NSCRIPT: CScriptBase :
//   CBaseClassNonCopyable : CBaseClass, vtable 0x012C1648, 5 slots, 0x40).
#include "engine/CScriptBase.h"
namespace NScript {
class CV_ChickenKickingScript : public CScriptBase {
public:
    struct CChickenMaster {
        static CCountedPointer<CEntityScriptBase> __fastcall Alloc(CV_ChickenKickingScript& parent, CScriptDataBase* data, const CScriptThing& thing);   // 0x00E634B0
    };
    struct CChickenSign {
        static CCountedPointer<CEntityScriptBase> __fastcall Alloc(CV_ChickenKickingScript& parent, CScriptDataBase* data, const CScriptThing& thing);   // 0x00E64D10
    };
    struct CKickedChicken {
        static CCountedPointer<CEntityScriptBase> __fastcall Alloc(CV_ChickenKickingScript& parent, CScriptDataBase* data, const CScriptThing& thing);   // 0x00E63590
    };
    struct CSpectator {
        static CCountedPointer<CEntityScriptBase> __fastcall Alloc(CV_ChickenKickingScript& parent, CScriptDataBase* data, const CScriptThing& thing);   // 0x00E63660
    };
    CGameScriptInterface* Game;                          // +0x40
    void* PMasterData;                                   // +0x44
    void Main();
    void CreateSpectators();                             // 0x00E62D60
    void LookAfterOrganiser();                           // 0x00E631F0
};
} // namespace NScript
using NScript::CV_ChickenKickingScript;

void NScript::CV_ChickenKickingScript::Main()
{
    while (!Game->IsRegionLoaded("OakBay")) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    AddEntityScriptBinding(new CEntityScriptBinding<CV_ChickenKickingScript>("ChickenMaster", this, &CChickenMaster::Alloc, 1));
    AddEntityScriptBinding(new CEntityScriptBinding<CV_ChickenKickingScript>("ChickenSign", this, &CChickenSign::Alloc, 1));
    AddEntityScriptBinding(new CEntityScriptBinding<CV_ChickenKickingScript>("KickedChicken", this, &CKickedChicken::Alloc, 1));
    AddEntityScriptBinding(new CEntityScriptBinding<CV_ChickenKickingScript>("Spectator", this, &CSpectator::Alloc, 1));
    PostAddScriptedEntities();
    Game->PostAddScriptedEntities();
    AddSpawnedFunction(new CSpawnedFunc<CV_ChickenKickingScript>("CreateSpectators", 0, this, &NScript::CV_ChickenKickingScript::CreateSpectators), CCharString(""));
    AddSpawnedFunction(new CSpawnedFunc<CV_ChickenKickingScript>("LookAfterOrganiser", 0, this, &NScript::CV_ChickenKickingScript::LookAfterOrganiser), CCharString(""));
}