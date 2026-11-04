// NScript::CQ_ArenaHoldingScriptScript::CEntranceGuard::OnPersist(CPersistContext& context) -- saves/loads IntroDone by name (default false)
// Leaf: ?OnPersist@CEntranceGuard@CQ_ArenaHoldingScriptScript@NScript@@EAEXAAVCPersistContext@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00CF6890, 33 bytes, thiscall, `ret 4`; calls CPersistContext::Transfer<bool> 0x004045C0 (??$Transfer@_N@CPersistContext@@QAEXPBDAA_NAB_N@Z, already bound).
// Identity (naming wave 35, family op): reg 00cf08a2 "EntranceGuard" in 00cf0790; Alloc 00cf0b60; vtable 012c6158 slot 4; PDB ?OnPersist@CEntranceGuard@CQ_ArenaHoldingScriptScript@NScript@@; refs=1; Anniversary RTTI agrees (confirmed); was _unattributed::PersistTransfer_IntroDone_00cf6890.
//   ?OnPersist@CEntranceGuard@CQ_ArenaHoldingScriptScript@NScript@@EAEXAAVCPersistContext@@@Z in FableWin.pdb (rebuild/candidates/manual/naming/2026-11-04_anniversary/wave3/labels_alloc.tsv). The persisted keys are the PDB member names.
// Was: CPersistable_00cf6890::PersistTransfer_IntroDone_00cf6890(CPersistContext* context) -- persists the "IntroDone" flag at +0x1c Retail: Fable.exe (TLC, VC7.1) function @ 0x00CF6890, oracle length 33 bytes. (catalog stand-in _unattributed).
// Class: the class model of the landed Alloc row 0x00CF0B60 (rebuild/src/compiled/00/cf/NScriptCQArenaHoldingScriptScriptCEntranceGuard_Alloc_00cf0b60.cpp); the PDB OnPersist declaration added; its bool IntroDone. CPersistContext: engine/CPersistContext.h. Only this member is graded.
#include "engine/CPersistContext.h"
class CGameScriptInterfaceBase;
class CScriptDataBase;

// CCountedPointer<T> / CCPPointerInfo: the shared retail model (engine/CCountedPointer.h; 0x00CDEE00 is
//   CCountedPointer<CEntityScriptBase>::DeleteData), in place of the Alloc row's local stand-in (deleter Destroy_00cdee00; wave 33 D4).
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
class CQ_ArenaHoldingScriptScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    class CEntranceGuard : public CEntityScriptBase {
    public:
        CQ_ArenaHoldingScriptScript* ParentClass;       // +0x14
        CQ_SunnyvaleMasterData* PMasterData;            // +0x18
        bool IntroDone;                                 // +0x1C
        bool DoneArena;                                 // +0x1D
        CEntranceGuard(CQ_ArenaHoldingScriptScript& parent, CScriptDataBase* data, const CScriptThing& me)
            : CEntityScriptBase(*parent.Game, me), ParentClass(&parent)
        {
            PMasterData = (CQ_SunnyvaleMasterData*)data;
        }
        virtual ~CEntranceGuard();
        static CCountedPointer<CEntityScriptBase> __fastcall Alloc(CQ_ArenaHoldingScriptScript& parent, CScriptDataBase* data, const CScriptThing& me);
    private:
    virtual void Main();                                 // PDB ?Main@<entity>@..@@EAEXXZ (slot 1)
public:
    virtual CScriptBase* GetParentScript() const;        // PDB ?GetParentScript@<entity>@..@@UBEPAVCScriptBase@@XZ (slot 3)
private:
    virtual void OnPersist(CPersistContext& context);   // PDB ?OnPersist@CEntranceGuard@CQ_ArenaHoldingScriptScript@NScript@@EAEXAAVCPersistContext@@@Z (slot 4)
};
};

CCountedPointer<CEntityScriptBase> __fastcall CQ_ArenaHoldingScriptScript::CEntranceGuard::Alloc(CQ_ArenaHoldingScriptScript& parent, CScriptDataBase* data, const CScriptThing& me)
{
    return CCountedPointer<CEntityScriptBase>(new CEntranceGuard(parent, data, me));
}
}

void NScript::CQ_ArenaHoldingScriptScript::CEntranceGuard::OnPersist(CPersistContext& context)
{
    context.Transfer("IntroDone", IntroDone, false);
}
