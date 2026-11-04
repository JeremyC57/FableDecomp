// NScript::CV_LostTraderScript::CLostTraderTrader::OnPersist(CPersistContext& context) -- saves/loads stateBrain by name (default 0L)
// Leaf: ?OnPersist@CLostTraderTrader@CV_LostTraderScript@NScript@@EAEXAAVCPersistContext@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00EA5960, 35 bytes, thiscall, `ret 4`; calls CPersistContext::Transfer<long> 0x00410BE0 (??$Transfer@J@CPersistContext@@QAEXPBDAAJABJ@Z, already bound).
// Identity (naming wave 35, family op): reg 00ea3247 "LostTraderTrader" in 00ea2e80; Alloc 00ea34f0; vtable 012edda8 slot 4; PDB ?OnPersist@CLostTraderTrader@CV_LostTraderScript@NScript@@; refs=1; Anniversary RTTI agrees (confirmed); was _unattributed::PersistTransfer_stateBrain_00ea5960.
//   ?OnPersist@CLostTraderTrader@CV_LostTraderScript@NScript@@EAEXAAVCPersistContext@@@Z in Ego_r.pdb / FableWin.pdb (rebuild/candidates/manual/naming/2026-11-04_anniversary/wave3/labels_alloc.tsv). The persisted keys are the PDB member names.
// Was: CPersistable_00ea5960::PersistTransfer_stateBrain_00ea5960(CPersistContext* context) -- persists the stateBrain dword at +0x1c with a zero default Retail: Fable.exe (TLC, VC7.1) function @ 0x00EA5960, oracle length 35... (catalog stand-in _unattributed).
// Class: the class model of the landed Alloc row 0x00EA34F0 (rebuild/src/compiled/00/ea/NScriptCVLostTraderScriptCLostTraderTrader_Alloc_00ea34f0.cpp); the PDB OnPersist declaration added; its long stateBrain. CPersistContext: engine/CPersistContext.h. Only this member is graded.
#include "engine/CPersistContext.h"
class CGameScriptInterfaceBase;
class CScriptDataBase;

// 0x00CDEE00 is CCountedPointer<T>::DeleteData (engine/CCountedPointer.h; reloc_bindings binds both spellings)

#include "engine/CCountedPointer.h"


// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"

// CEntityScriptBase: the retail model from engine/CEntityScriptBase.h (vtable 0x012C3224, 7 slots, 0x14: Game +0x04, Me +0x08).
#include "engine/CEntityScriptBase.h"

// CScriptBase / CLevelScriptBase<T>: the retail NScript model from the shared headers (FABLE_RETAIL_NSCRIPT: CScriptBase :
//   CBaseClassNonCopyable : CBaseClass, vtable 0x012C1648, 5 slots, 0x40; CLevelScriptBase<T> Game +0x40, MasterScriptData +0x44).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CLevelScriptBase.h"

namespace NScript {
class CQ_SunnyvaleMasterData;
class CV_LostTraderScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    class CLostTraderTrader : public CEntityScriptBase {
    public:
        CV_LostTraderScript* ParentClass;               // +0x14
        CQ_SunnyvaleMasterData* PMasterData;            // +0x18
        long stateBrain;                                // +0x1C
        long YellForHelp;                               // +0x20
        long YellAtHero;                                // +0x24
        long RandomComment;                             // +0x28
        CLostTraderTrader(CV_LostTraderScript& parent, CScriptDataBase* data, const CScriptThing& me)
            : CEntityScriptBase(*parent.Game, me), ParentClass(&parent)
        {
            PMasterData = (CQ_SunnyvaleMasterData*)data;
        }
        virtual ~CLostTraderTrader();
        static CCountedPointer<CEntityScriptBase> __fastcall Alloc(CV_LostTraderScript& parent, CScriptDataBase* data, const CScriptThing& me);
    private:
    virtual void Main();                                 // PDB ?Main@<entity>@..@@EAEXXZ (slot 1)
public:
    virtual CScriptBase* GetParentScript() const;        // PDB ?GetParentScript@<entity>@..@@UBEPAVCScriptBase@@XZ (slot 3)
private:
    virtual void OnPersist(CPersistContext& context);   // PDB ?OnPersist@CLostTraderTrader@CV_LostTraderScript@NScript@@EAEXAAVCPersistContext@@@Z (slot 4)
};
};

CCountedPointer<CEntityScriptBase> __fastcall CV_LostTraderScript::CLostTraderTrader::Alloc(CV_LostTraderScript& parent, CScriptDataBase* data, const CScriptThing& me)
{
    return CCountedPointer<CEntityScriptBase>(new CLostTraderTrader(parent, data, me));
}
}

void NScript::CV_LostTraderScript::CLostTraderTrader::OnPersist(CPersistContext& context)
{
    context.Transfer("stateBrain", stateBrain, 0L);
}
