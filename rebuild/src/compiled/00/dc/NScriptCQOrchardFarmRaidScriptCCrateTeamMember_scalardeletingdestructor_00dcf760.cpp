// NScript::CQ_OrchardFarmRaidScript::CCrateTeamMember::`scalar deleting destructor'(unsigned int flags) -- the destructor (expanded), then free the object when bit 0 is set
// Leaf: ??_GCCrateTeamMember@CQ_OrchardFarmRaidScript@NScript@@UAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00DCF760, 40 bytes, thiscall, `ret 4`; calls 004aa840=CScriptThing::~CScriptThing ; 00f35b40=CEntityScriptBase::~CEntityScriptBase ; 00bfe9bc=MSVCR71.DLL::operator_delete.
// Identity (naming wave 35, family sdd): Anniversary wave 2026-11-04 (high): vtable 0x012DB364 slot 0; Anniversary RTTI NScript::CQ_OrchardFarmRaidScript::CCrateTeamMember (string-literal match, validated 257/258); PDB name set has the sdd member of NScript::CQ_OrchardFarmRaidScript::CCrateTeamMember; slot layout from named TLC siblings; function referenced by this vtable only.
//   ??_GCCrateTeamMember@CQ_OrchardFarmRaidScript@NScript@@UAEPAXI@Z (rebuild/candidates/manual/naming/2026-11-04_anniversary/labels_high.tsv); PDB: Ego_r.pdb / FableWin.pdb ??_ECCrateTeamMember@CQ_OrchardFarmRaidScript@NScript@@UAEPAXI@Z; Ego_r.pdb / FableWin.pdb ??1CCrateTeamMember@CQ_OrchardFarmRaidScript@NScript@@UAE@XZ.
//   Was:  (catalog stand-in CVtable_012db364, Leaf `scalar_deleting_destructor').
// Shape: the deleting destructor is emitted with the vtable, i.e. in the TU that constructs the object (wave 33 esb): the class model and
//   the PDB Alloc ?Alloc@CCrateTeamMember@CQ_OrchardFarmRaidScript@NScript@@SI?AV?$CCountedPointer@VCEntityScriptBase@@@@AAV23@PAVCScriptDataBase@@ABVCScriptThing@@@Z of the landed row 0x00DCDE20 (rebuild/src/compiled/00/dc/NScriptCQOrchardFarmRaidScriptCCrateTeamMember_Alloc_00dcde20.cpp), its own source with the FABLE_RETAIL_CLEVELSCRIPTBASE opt-in added; its user-declared destructor dropped (retail expands the destructor into the ??_G). The Alloc body is not part of this graded body.
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
#define FABLE_RETAIL_CLEVELSCRIPTBASE   // retail's implicit ~CLevelScriptBase<T> (chunk 713 opt-in): the ??_G calls ~CScriptBase 0x00CBD510
#include "engine/CLevelScriptBase.h"

namespace NScript {
class CQ_SunnyvaleMasterData;
class CQ_OrchardFarmRaidScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    class CCrateTeamMember : public CEntityScriptBase {
    public:
        CQ_OrchardFarmRaidScript* ParentClass;          // +0x14
        CQ_SunnyvaleMasterData* PMasterData;            // +0x18
        unsigned char _members1c[0x14];                 // +0x1C (Ego_r.pdb CurrentAIState, PreviousAIState, TeamID, MemberState, MyTeam; not modelled here)
        CScriptThing ThingToPatrolTo;                   // +0x30 (Ego_r.pdb CCrateTeamMember +0x30)
        CCrateTeamMember(CQ_OrchardFarmRaidScript& parent, CScriptDataBase* data, const CScriptThing& me)
            : CEntityScriptBase(*parent.Game, me), ParentClass(&parent)
        {
            PMasterData = (CQ_SunnyvaleMasterData*)data;
        }
        static CCountedPointer<CEntityScriptBase> __fastcall Alloc(CQ_OrchardFarmRaidScript& parent, CScriptDataBase* data, const CScriptThing& me);
    private:
    virtual void Main();                                 // PDB ?Main@<entity>@..@@EAEXXZ (slot 1)
public:
    virtual CScriptBase* GetParentScript() const;        // PDB ?GetParentScript@<entity>@..@@UBEPAVCScriptBase@@XZ (slot 3)
};
};

CCountedPointer<CEntityScriptBase> __fastcall CQ_OrchardFarmRaidScript::CCrateTeamMember::Alloc(CQ_OrchardFarmRaidScript& parent, CScriptDataBase* data, const CScriptThing& me)
{
    return CCountedPointer<CEntityScriptBase>(new CCrateTeamMember(parent, data, me));
}
}