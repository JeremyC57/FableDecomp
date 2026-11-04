// NScript::CQ_GuildTrainingScript::CApprenticeSpeedTest::OnPersist(CPersistContext& context) -- saves/loads RaceMode by name (default 0L)
// Leaf: ?OnPersist@CApprenticeSpeedTest@CQ_GuildTrainingScript@NScript@@EAEXAAVCPersistContext@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00D446A0, 35 bytes, thiscall, `ret 4`; calls CPersistContext::Transfer<long> 0x00410BE0 (??$Transfer@J@CPersistContext@@QAEXPBDAAJABJ@Z, already bound).
// Identity (naming wave 35, family op): reg 00d3c027 "ApprenticeSpeedTest" in 00d3bc60; Alloc 00d3e1f0; vtable 012cda40 slot 4; PDB ?OnPersist@CApprenticeSpeedTest@CQ_GuildTrainingScript@NScript@@; refs=1; Anniversary RTTI agrees (confirmed); was _unattributed::PersistTransfer_RaceMode_00d446a0.
//   ?OnPersist@CApprenticeSpeedTest@CQ_GuildTrainingScript@NScript@@EAEXAAVCPersistContext@@@Z in Ego_r.pdb / FableWin.pdb (rebuild/candidates/manual/naming/2026-11-04_anniversary/wave3/labels_alloc.tsv). The persisted keys are the PDB member names.
// Was: CPersistable_00d446a0::PersistTransfer_RaceMode_00d446a0(CPersistContext* context) -- persists the RaceMode dword at +0x1c with a zero default Retail: Fable.exe (TLC, VC7.1) function @ 0x00D446A0, oracle length 35 bytes. (catalog stand-in _unattributed).
// Class: the class model of the landed Alloc row 0x00D3E1F0 (rebuild/src/compiled/00/d3/NScriptCQGuildTrainingScriptCApprenticeSpeedTest_Alloc_00d3e1f0.cpp); the PDB OnPersist declaration added; its long RaceMode. CPersistContext: engine/CPersistContext.h. Only this member is graded.
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
class CQ_GuildTrainingScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    class CApprenticeSpeedTest : public CEntityScriptBase {
    public:
        CQ_GuildTrainingScript* ParentClass;            // +0x14
        CQ_SunnyvaleMasterData* PMasterData;            // +0x18
        long RaceMode;                                  // +0x1C
        CApprenticeSpeedTest(CQ_GuildTrainingScript& parent, CScriptDataBase* data, const CScriptThing& me)
            : CEntityScriptBase(*parent.Game, me), ParentClass(&parent)
        {
            PMasterData = (CQ_SunnyvaleMasterData*)data;
        }
        virtual ~CApprenticeSpeedTest();
        static CCountedPointer<CEntityScriptBase> __fastcall Alloc(CQ_GuildTrainingScript& parent, CScriptDataBase* data, const CScriptThing& me);
    private:
    virtual void Main();                                 // PDB ?Main@<entity>@..@@EAEXXZ (slot 1)
public:
    virtual CScriptBase* GetParentScript() const;        // PDB ?GetParentScript@<entity>@..@@UBEPAVCScriptBase@@XZ (slot 3)
private:
    virtual void OnPersist(CPersistContext& context);   // PDB ?OnPersist@CApprenticeSpeedTest@CQ_GuildTrainingScript@NScript@@EAEXAAVCPersistContext@@@Z (slot 4)
};
};

CCountedPointer<CEntityScriptBase> __fastcall CQ_GuildTrainingScript::CApprenticeSpeedTest::Alloc(CQ_GuildTrainingScript& parent, CScriptDataBase* data, const CScriptThing& me)
{
    return CCountedPointer<CEntityScriptBase>(new CApprenticeSpeedTest(parent, data, me));
}
}

void NScript::CQ_GuildTrainingScript::CApprenticeSpeedTest::OnPersist(CPersistContext& context)
{
    context.Transfer("RaceMode", RaceMode, 0L);
}
