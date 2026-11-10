// NScript::CQ_PrisonWardenGameScript::CPrisonWarden::OnPredicateFail() -- the warden's predicate failed: clear every quest info element from the screen (Game->RemoveAllQuestInfoElements())
// Leaf: ?OnPredicateFail@CPrisonWarden@CQ_PrisonWardenGameScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00DDE6F0, 11 bytes, thiscall: mov ecx,[ecx+4] ; mov eax,[ecx] ; jmp [eax+54Ch] (a tail call).
// Identity: slot 5 (OnPredicateFail, engine/CEntityScriptBase.h) of the entity vtable 0x012DCC50 that the landed Alloc 0x00DDE630
//   stores (slot 2 Init 0x00DDE6E0, slot 3 GetParentScript 0x00DDE6D0 landed); Ego_r.pdb / FableWin.pdb
//   ?OnPredicateFail@CPrisonWarden@CQ_PrisonWardenGameScript@NScript@@EAEXXZ (private virtual).
// Callee: CEntityScriptBase::Game (+0x04) slot 339 (+0x54C) = CGameScriptInterfaceBase::RemoveAllQuestInfoElements() const
//   (engine/CGameScriptInterfaceBase.h; CGameScriptInterface vtable 0x01260F0C + 0x54C = 0x00891C30).
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
#include "engine/CGameScriptInterfaceBase.h"

namespace NScript {
class CQ_SunnyvaleMasterData;
class CQ_PrisonWardenGameScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    class CPrisonWarden : public CEntityScriptBase {
    public:
        CQ_PrisonWardenGameScript* ParentClass;      // +0x14
        CQ_SunnyvaleMasterData* PMasterData;            // +0x18
        static CCountedPointer<CEntityScriptBase> __fastcall Alloc(CQ_PrisonWardenGameScript& parent, CScriptDataBase* data, const CScriptThing& me);
    private:
    virtual void Main();                                 // PDB ?Main@<entity>@..@@EAEXXZ (slot 1)
    virtual void OnPredicateFail();                      // PDB ?OnPredicateFail@CPrisonWarden@CQ_PrisonWardenGameScript@NScript@@EAEXXZ (slot 5)
public:
    virtual CScriptBase* GetParentScript() const;        // PDB ?GetParentScript@<entity>@..@@UBEPAVCScriptBase@@XZ (slot 3)
};
};

void CQ_PrisonWardenGameScript::CPrisonWarden::OnPredicateFail()
{
    Game->RemoveAllQuestInfoElements();
}
}