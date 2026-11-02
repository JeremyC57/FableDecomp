// NScript::CQ_PrisonWardenGameScript::Main() -- prison warden's office: bind the warden, the secret book, the office exit and the do-not-target marker, teleport the hero to WardensOfficeStart facing its angle, wait for PrisonOffice, then spawn WardenKeyMiniGame
// Leaf: ?Main@CQ_PrisonWardenGameScript@NScript@@UAEXXZ
// Naming wave 29 (family sfbh): engine/CSpawnedFuncBase.h now carries retail's implicit destructor and the real CCharString /
//   CCountedPointer<const NScript::CPredicateBase> members; header contract met here: engine/CCountedPointer.h included before it.
// Retail: Fable.exe (TLC, VC7.1) @ 0x00DDDE20, 1021 bytes, thiscall, plain `ret` (ends 0x00DDE21C).
// Identity: vtable slot 2 (Main) of 0x012DCB40, the vtable the landed ctor 0x00DDDC90 stores; Ego_r.pdb
//   ?Main@CQ_PrisonWardenGameScript@NScript@@UAEXXZ (donor 0x00E182B0, beside ?RegisterMain@ 0x00E18180 and
//   ?WardenKeyMiniGame@ 0x00E17290); the bound Allocs are the landed CPrisonWarden / CSecretBook / CExitWardensOffice /
//   CDoNotTarget ::Alloc 0x00DDE630 / 0x00DDE720 / 0x00DDEC30 / 0x00DDEDF0 (names = the binding strings); the spawned thread is
//   the landed WardenKeyMiniGame 0x00DDE230 (name string "WardenKeyMiniGame" beside the stored pointer).
// Callees: CCharString 0x0099EBF0 / 0x0099EC30 / ~ 0x0099EAE0, operator new 0x00BFEA1A, operator delete 0x00BFE9BC,
//   ~CBaseClass 0x0099A2E0, AddEntityScriptBinding 0x00CB8230 (binding vtable 0x012DCBC4), PostAddScriptedEntities 0x00CB8930,
//   CSpawnedFuncBase 0x00CDD450 (spawn vtable 0x012DCB54), AddSpawnedFunction 0x00CB7E50, IsActiveThreadTerminating 0x00CB7940;
//   CGameScriptInterface slots 7, 12, 64, 70 GetHero, 72 GetThingWithScriptName, 472 EntityTeleportToThing,
//   474 EntitySetFacingAngle; CScriptThing slot 10 GetAngleXY.
// Shape: model of the landed CQ_HerosOldHouseScript Main (this chunk); the two GetThingWithScriptName temporaries expand
//   ~CScriptThing inline (vtable 0x01238C8C store, CCountedPointer release, ~CBaseClass), the header's inline-dtor contract.
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    CCharString(const CCharString& other);              // 0x0099EC30
    ~CCharString();                                     // 0x0099EAE0
};
class CWideString { public:
    void* PStringData;
    CWideString(const unsigned short* text);            // 0x0099B6B0
    ~CWideString();                                     // 0x0099B510
};
// Layout: CSpawnedFuncBase is the retail polymorphic suspendable-process chain from the shared headers
//   (FABLE_RETAIL_SUSPENDABLE_PROCESS opt-in: CSpawnedFuncBase : CSuspendableProcessMicrothread : CASuspendableProcess :
//   CBaseClassNonCopyable : CBaseClass, 0x34 bytes, vtable 0x012C2F9C; ctor 0x00CDD450).
#define FABLE_RETAIL_SUSPENDABLE_PROCESS
#include "engine/CCountedPointer.h"                     // CCountedPointer<T> for CSpawnedFuncBase::PPredicate (header contract)
#include "engine/CSpawnedFuncBase.h"
#include "engine/CSpawnedFunc.h"                         // vtable per instantiation; ctor inline, slot 4 SuspendableProcess

class CScriptThing;
class CScriptDataBase;
class CEntityScriptBase;
class CActiveEntityScriptBase;
#include "engine/CCountedPointer.h"
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"                       // : CBaseClass, vtable 0x01238C8C; ~CBaseClass 0x0099A2E0
inline CScriptThing::~CScriptThing() { }               // retail expands the temporaries' dtor inline

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
    __forceinline CEntityScriptBinding(const CCharString& name, T* parent, TAllocFunc alloc, unsigned long flags)
        : EntityScriptName(name), PParentScript(parent), ClassAllocFunc(alloc), ClassAllocValid(true), Flags(flags) {}
    virtual const CCharString& GetEntityScriptName() const { return EntityScriptName; }
};

#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
// CScriptBase: the retail NScript model from the shared headers (FABLE_RETAIL_NSCRIPT: CScriptBase :
//   CBaseClassNonCopyable : CBaseClass, vtable 0x012C1648, 5 slots, 0x40).
#include "engine/CScriptBase.h"
#include "engine/CLevelScriptBase.h"
namespace NScript { class CQ_SunnyvaleMasterData; }
namespace NScript {
class CQ_PrisonWardenGameScript : public CLevelScriptBase<NScript::CQ_SunnyvaleMasterData> {   // base: the landed ctor 0x00DDDC90
public:
    class CPrisonWarden { public: static CCountedPointer<CEntityScriptBase> __fastcall Alloc(CQ_PrisonWardenGameScript& parent, CScriptDataBase* data, const CScriptThing& thing); };          // 0x00DDE630
    class CSecretBook { public: static CCountedPointer<CEntityScriptBase> __fastcall Alloc(CQ_PrisonWardenGameScript& parent, CScriptDataBase* data, const CScriptThing& thing); };            // 0x00DDE720
    class CExitWardensOffice { public: static CCountedPointer<CEntityScriptBase> __fastcall Alloc(CQ_PrisonWardenGameScript& parent, CScriptDataBase* data, const CScriptThing& thing); };     // 0x00DDEC30
    class CDoNotTarget { public: static CCountedPointer<CEntityScriptBase> __fastcall Alloc(CQ_PrisonWardenGameScript& parent, CScriptDataBase* data, const CScriptThing& thing); };           // 0x00DDEDF0
    virtual void Main();
    void WardenKeyMiniGame();                          // 0x00DDE230
    bool SecretDiaryOpened;                            // +0x48
    bool HeroHasSecretDiaryKey;                        // +0x49
};
} // namespace NScript
using NScript::CQ_PrisonWardenGameScript;

void NScript::CQ_PrisonWardenGameScript::Main()
{
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_PrisonWardenGameScript>("PrisonWarden", this, &CPrisonWarden::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_PrisonWardenGameScript>("SecretBook", this, &CSecretBook::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_PrisonWardenGameScript>("ExitWardensOffice", this, &CExitWardensOffice::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_PrisonWardenGameScript>("DoNotTarget", this, &CDoNotTarget::Alloc, 0));
    PostAddScriptedEntities();
    Game->PostAddScriptedEntities();
    Game->EntityTeleportToThing(Game->GetHero(), Game->GetThingWithScriptName("WardensOfficeStart"), false);
    Game->EntitySetFacingAngle(Game->GetHero(), Game->GetThingWithScriptName("WardensOfficeStart").GetAngleXY(), true);
    while (!Game->IsRegionLoaded("PrisonOffice")) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    AddSpawnedFunction(new CSpawnedFunc<CQ_PrisonWardenGameScript>("WardenKeyMiniGame", 0, this, &NScript::CQ_PrisonWardenGameScript::WardenKeyMiniGame), CCharString(""));
}