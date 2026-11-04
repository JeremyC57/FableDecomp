// NScript::CQ_BreakSiegeScript::CDisplacedTrader::OnPersist(CPersistContext& context) -- saves/loads TimesSpokenTo by name (default 0L)
// Leaf: ?OnPersist@CDisplacedTrader@CQ_BreakSiegeScript@NScript@@EAEXAAVCPersistContext@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00D21B10, 35 bytes, thiscall, `ret 4`; calls CPersistContext::Transfer<long> 0x00410BE0 (??$Transfer@J@CPersistContext@@QAEXPBDAAJABJ@Z, already bound).
// Identity (naming wave 35, family op): reg 00d1d4fa "DisplacedTrader" in 00d1d100; Alloc 00d1fc30; vtable 012cb070 slot 4; PDB ?OnPersist@CDisplacedTrader@CQ_BreakSiegeScript@NScript@@; refs=1; Anniversary RTTI agrees (confirmed); was _unattributed::PersistTransfer_TimesSpokenTo_00d21b10.
//   ?OnPersist@CDisplacedTrader@CQ_BreakSiegeScript@NScript@@EAEXAAVCPersistContext@@@Z in Ego_r.pdb / FableWin.pdb (rebuild/candidates/manual/naming/2026-11-04_anniversary/wave3/labels_alloc.tsv). The persisted keys are the PDB member names.
// Was: CPersistable_00d21b10::PersistTransfer_TimesSpokenTo_00d21b10(CPersistContext* context) -- persists the TimesSpokenTo dword at +0x1c with a zero default Retail: Fable.exe (TLC, VC7.1) function @ 0x00D21B10, oracle len... (catalog stand-in _unattributed).
// Class: the class model of the landed Alloc row 0x00D1FC30 (rebuild/src/compiled/00/d1/NScriptCQBreakSiegeScriptCDisplacedTrader_Alloc_00d1fc30.cpp); the PDB OnPersist declaration added; its long TimesSpokenTo. CPersistContext: engine/CPersistContext.h. Only this member is graded.
#include "engine/CPersistContext.h"
class CGameScriptInterfaceBase;
class CScriptDataBase;

// 0x00CDEE00 is CCountedPointer<T>::DeleteData (engine/CCountedPointer.h; reloc_bindings binds both spellings)

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
class CQ_BreakSiegeScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    class CDisplacedTrader : public CEntityScriptBase {
    public:
        CQ_BreakSiegeScript* ParentClass;               // +0x14
        CQ_SunnyvaleMasterData* PMasterData;            // +0x18
        long TimesSpokenTo;                             // +0x1C
        CTimer RandomMutteringsDelay;                   // +0x20
        CDisplacedTrader(CQ_BreakSiegeScript& parent, CScriptDataBase* data, const CScriptThing& me)
            : CEntityScriptBase(*parent.Game, me), ParentClass(&parent)
        {
            PMasterData = (CQ_SunnyvaleMasterData*)data;
        }
        virtual ~CDisplacedTrader();
        static CCountedPointer<CEntityScriptBase> __fastcall Alloc(CQ_BreakSiegeScript& parent, CScriptDataBase* data, const CScriptThing& me);
    private:
    virtual void Main();                                 // PDB ?Main@<entity>@..@@EAEXXZ (slot 1)
public:
    virtual CScriptBase* GetParentScript() const;        // PDB ?GetParentScript@<entity>@..@@UBEPAVCScriptBase@@XZ (slot 3)
private:
    virtual void OnPersist(CPersistContext& context);   // PDB ?OnPersist@CDisplacedTrader@CQ_BreakSiegeScript@NScript@@EAEXAAVCPersistContext@@@Z (slot 4)
};
};

CCountedPointer<CEntityScriptBase> __fastcall CQ_BreakSiegeScript::CDisplacedTrader::Alloc(CQ_BreakSiegeScript& parent, CScriptDataBase* data, const CScriptThing& me)
{
    return CCountedPointer<CEntityScriptBase>(new CDisplacedTrader(parent, data, me));
}
}

void NScript::CQ_BreakSiegeScript::CDisplacedTrader::OnPersist(CPersistContext& context)
{
    context.Transfer("TimesSpokenTo", TimesSpokenTo, 0L);
}
