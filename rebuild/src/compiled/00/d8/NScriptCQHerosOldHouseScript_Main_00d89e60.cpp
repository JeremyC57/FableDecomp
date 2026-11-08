// NScript::CQ_HerosOldHouseScript::Main() -- Hero's old house (Oakvale): wait for OakBay, bind the ghost fisherman, his wife and the extra booty, spawn WatchForBooty, then until the booty is dug up fail the quest once if the wife is attacked while the hero is helping; finally deactivate the quest
// Leaf: ?Main@CQ_HerosOldHouseScript@NScript@@UAEXXZ
// Naming wave 29 (family sfbh): engine/CSpawnedFuncBase.h now carries retail's implicit destructor and the real CCharString /
//   CCountedPointer<const NScript::CPredicateBase> members; header contract met here: engine/CCountedPointer.h included before it.
// Retail: Fable.exe (TLC, VC7.1) @ 0x00D89E60, 976 bytes, thiscall, plain `ret`.
// Identity: vtable slot 2 (Main) of 0x012D5018 (reference .rdata 0x012D5020), the vtable the landed ctor 0x00D89D70
//   NScript::CQ_HerosOldHouseScript::CQ_HerosOldHouseScript stores (slot 1 RegisterMain 0x00D89DC0, slot 3 Init 0x00D89DA0
//   landed); Ego_r.pdb ?Main@CQ_HerosOldHouseScript@NScript@@UAEXXZ; the bound names are the PDB's nested classes CGhostFisherman,
//   CFishermansWife, CExtraBooty (landed Allocs 0x00D8A4B0 / 0x00D8A650 / 0x00D8A7F0), the thread is the PDB member
//   ?WatchForBooty@CQ_HerosOldHouseScript@NScript@@QAEXXZ 0x00D8A240 (name string "WatchForBooty" beside the stored pointer).
// Layout: CLevelScriptBase<CQ_SunnyvaleMasterData> (engine/CLevelScriptBase.h; the landed ctor's base), then the six Ego_r bools
//   Helping +0x48 .. BootyDugUp +0x4D (the landed OnPersist 0x00D8AB10 model).
// Callees: CCharString 0x0099EBF0 / 0x0099EC30 / ~ 0x0099EAE0, CWideString(const unsigned short*) 0x0099B6B0 / ~ 0x0099B510,
//   operator new 0x00BFEA1A, AddEntityScriptBinding 0x00CB8230 (binding vtable 0x012D5074), PostAddScriptedEntities 0x00CB8930,
//   CSpawnedFuncBase 0x00CDD450 (spawn vtable 0x012D502C), AddSpawnedFunction 0x00CB7E50, IsActiveThreadTerminating 0x00CB7940,
//   ~CScriptThing 0x004AA840; CGameScriptInterface slots 7, 12, 64, 72, 281 DeactivateQuestLater, 289 SetQuestAsFailed,
//   655 GetActiveQuestName.
// Shape: models of the landed CQ_HeroSoulsNostroScript / CQ_OakValeBanditRaidScript Mains. The SetQuestAsFailed reason is the
//   IMPLICIT conversion `L""` -> const CWideString& (the temporary is built before Game's vtable is loaded and the vtable is
//   shared by the nested GetActiveQuestName call); an explicit CWideString(L"") loads Game first and reshuffles registers
//   through the whole body (80 differing lines). The `!questFailed && IsRegionLoaded("OakBay") && WifeAttacked` test is
//   materialised into a byte because its CCharString temporary is conditional (flag bit 0x10).
#include "fable_compiler.h"
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
#include "engine/CScriptThing.h"                       // : CBaseClass, vtable 0x01238C8C; ~CScriptThing 0x004AA840 out of line

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
    FABLE_FORCEINLINE CEntityScriptBinding(const CCharString& name, T* parent, TAllocFunc alloc, unsigned long flags)
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
class CQ_HerosOldHouseScript : public CLevelScriptBase<NScript::CQ_SunnyvaleMasterData> {   // base: the landed ctor 0x00D89D70
public:
    class CGhostFisherman { public: static CCountedPointer<CEntityScriptBase> __fastcall Alloc(CQ_HerosOldHouseScript& parent, CScriptDataBase* data, const CScriptThing& thing); };   // 0x00D8A4B0
    class CFishermansWife { public: static CCountedPointer<CEntityScriptBase> __fastcall Alloc(CQ_HerosOldHouseScript& parent, CScriptDataBase* data, const CScriptThing& thing); };   // 0x00D8A650
    class CExtraBooty { public: static CCountedPointer<CEntityScriptBase> __fastcall Alloc(CQ_HerosOldHouseScript& parent, CScriptDataBase* data, const CScriptThing& thing); };       // 0x00D8A7F0
    virtual void Main();
    void WatchForBooty();                              // 0x00D8A240
    bool Helping;                                      // +0x48
    bool Helped;                                       // +0x49
    bool MissionAborted;                               // +0x4A
    bool WifeAttacked;                                 // +0x4B
    bool WifeAttackedAndLeft;                          // +0x4C
    bool BootyDugUp;                                   // +0x4D
};
} // namespace NScript
using NScript::CQ_HerosOldHouseScript;

void NScript::CQ_HerosOldHouseScript::Main()
{
    while (!Game->IsRegionLoaded("OakBay")) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_HerosOldHouseScript>("GhostFisherman", this, &CGhostFisherman::Alloc, 1));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_HerosOldHouseScript>("FishermansWife", this, &CFishermansWife::Alloc, 1));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_HerosOldHouseScript>("ExtraBooty", this, &CExtraBooty::Alloc, 1));
    PostAddScriptedEntities();
    Game->PostAddScriptedEntities();
    AddSpawnedFunction(new CSpawnedFunc<CQ_HerosOldHouseScript>("WatchForBooty", 0, this, &NScript::CQ_HerosOldHouseScript::WatchForBooty), CCharString(""));
    MissionAborted = false;
    CScriptThing wife = Game->GetThingWithScriptName("FishermansWife");
    bool questFailed = false;
    while (!BootyDugUp) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
        if (!questFailed && Game->IsRegionLoaded("OakBay") && WifeAttacked) {
            if (IsActiveThreadTerminating())
                return;
            if (Helping) {
                if (IsActiveThreadTerminating())
                    return;
                Game->SetQuestAsFailed(Game->GetActiveQuestName(), false, L"", true);
                questFailed = true;
                Helping = false;
            }
        }
    }
    if (IsActiveThreadTerminating())
        return;
    Game->DeactivateQuestLater(Game->GetActiveQuestName(), 0);
}