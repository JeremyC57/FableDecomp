// NScript::CQ_AmbushTradersScript::CAT_BanditAlly::OnPersist(CPersistContext& context) -- saves/loads BanditHired and BanditFollowing by name (default false)
// Leaf: ?OnPersist@CAT_BanditAlly@CQ_AmbushTradersScript@NScript@@EAEXAAVCPersistContext@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00F0A8E0, 67 bytes, thiscall, `ret 4`; calls CPersistContext::Transfer<bool> 0x004045C0 (??$Transfer@_N@CPersistContext@@QAEXPBDAA_NAB_N@Z, already bound).
// Identity (naming wave 35, family op): reg 00f02515 "AT_BanditAlly" in 00f024c0; Alloc 00f037a0; vtable 012f7d88 slot 4; PDB ?OnPersist@CAT_BanditAlly@CQ_AmbushTradersScript@NScript@@; refs=1; Anniversary RTTI agrees (confirmed); was _unattributed::PersistTransfer2_00f0a8e0.
//   ?OnPersist@CAT_BanditAlly@CQ_AmbushTradersScript@NScript@@EAEXAAVCPersistContext@@@Z in Ego_r.pdb / FableWin.pdb (rebuild/candidates/manual/naming/2026-11-04_anniversary/wave3/labels_alloc.tsv). The persisted keys are the PDB member names.
// Was: CPersistable_00f0a8e0::PersistTransfer2_00f0a8e0(CPersistContext* context) -- persists the "BanditHired" and "BanditFollowing" byte flags at +0x1c/+0x1d Retail: Fable.exe (TLC, VC7.1) function @ 0x00F0A8E0, oracle len... (catalog stand-in _unattributed).
// Class: the class model of the landed Alloc row 0x00F037A0 (rebuild/src/compiled/00/f0/NScriptCQAmbushTradersScriptCATBanditAlly_Alloc_00f037a0.cpp); the PDB OnPersist declaration added; its bool BanditHired, bool BanditFollowing. CPersistContext: engine/CPersistContext.h. Only this member is graded.
#include "engine/CPersistContext.h"
class CGameScriptInterfaceBase;
class CScriptDataBase;

// CCountedPointer<T> / CCPPointerInfo: the shared retail model (engine/CCountedPointer.h; 0x00CDEE00 is
//   CCountedPointer<CEntityScriptBase>::DeleteData), in place of the Alloc row's local stand-in (deleter Destroy_00cdee00; wave 33 D4).
#include "engine/CCountedPointer.h"

// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
// CTimer: the retail model from engine/CTimer.h (Ego_r.pdb CTimer {long TimerIndex}, 0x4; the setter is the retail
//   operator=(long), Ego_r 0x00CCE720). Bodies the row expands inline follow the include.
#include "engine/CTimer.h"

// CEntityScriptBase: the retail model from engine/CEntityScriptBase.h (vtable 0x012C3224, 7 slots, 0x14: Game +0x04, Me +0x08).
#include "engine/CEntityScriptBase.h"

// CScriptBase / CLevelScriptBase<T>: the retail NScript model from the shared headers (FABLE_RETAIL_NSCRIPT: CScriptBase :
//   CBaseClassNonCopyable : CBaseClass, vtable 0x012C1648, 5 slots, 0x40; CLevelScriptBase<T> Game +0x40, MasterScriptData +0x44).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CLevelScriptBase.h"

namespace NScript {
class CQ_SunnyvaleMasterData;
class CQ_AmbushTradersScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    class CAT_BanditAlly : public CEntityScriptBase {
    public:
        CQ_AmbushTradersScript* ParentClass;            // +0x14
        CQ_SunnyvaleMasterData* PMasterData;            // +0x18
        bool BanditHired;                               // +0x1C
        bool BanditFollowing;                           // +0x1D
        bool BanditTeleported;                          // +0x1E
        bool BanditSwitchedAllegience;                  // +0x1F
        bool BanditRemovedFromBowerstonePosh;           // +0x20
        bool ResetInvalidRegionTimer;                   // +0x21
        long BanditHealthBarID;                         // +0x24
        CTimer InvalidRegionTimer;                      // +0x28
        CAT_BanditAlly(CQ_AmbushTradersScript& parent, CScriptDataBase* data, const CScriptThing& me)
            : CEntityScriptBase(*parent.Game, me), ParentClass(&parent)
        {
            PMasterData = (CQ_SunnyvaleMasterData*)data;
        }
        virtual ~CAT_BanditAlly();
        static CCountedPointer<CEntityScriptBase> __fastcall Alloc(CQ_AmbushTradersScript& parent, CScriptDataBase* data, const CScriptThing& me);
    private:
    virtual void Main();                                 // PDB ?Main@<entity>@..@@EAEXXZ (slot 1)
public:
    virtual CScriptBase* GetParentScript() const;        // PDB ?GetParentScript@<entity>@..@@UBEPAVCScriptBase@@XZ (slot 3)
private:
    virtual void OnPersist(CPersistContext& context);   // PDB ?OnPersist@CAT_BanditAlly@CQ_AmbushTradersScript@NScript@@EAEXAAVCPersistContext@@@Z (slot 4)
};
};

CCountedPointer<CEntityScriptBase> __fastcall CQ_AmbushTradersScript::CAT_BanditAlly::Alloc(CQ_AmbushTradersScript& parent, CScriptDataBase* data, const CScriptThing& me)
{
    return CCountedPointer<CEntityScriptBase>(new CAT_BanditAlly(parent, data, me));
}
}

void NScript::CQ_AmbushTradersScript::CAT_BanditAlly::OnPersist(CPersistContext& context)
{
    context.Transfer("BanditHired", BanditHired, false);
    context.Transfer("BanditFollowing", BanditFollowing, false);
}
