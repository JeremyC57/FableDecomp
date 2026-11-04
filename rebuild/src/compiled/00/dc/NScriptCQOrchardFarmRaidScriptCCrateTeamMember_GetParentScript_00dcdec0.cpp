// NScript::CQ_OrchardFarmRaidScript::CCrateTeamMember::GetParentScript() const -- the entity's parent level script (ParentClass, +0x14)
// Leaf: ?GetParentScript@CCrateTeamMember@CQ_OrchardFarmRaidScript@NScript@@UBEPAVCScriptBase@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00DCDEC0, 4 bytes: mov eax,[ecx+14h]; ret.
// Identity (naming wave 35, family ent): Anniversary wave 2026-11-04 (high): vtable 0x012DB364 slot 3; Anniversary RTTI NScript::CQ_OrchardFarmRaidScript::CCrateTeamMember (string-literal match, validated 257/258); PDB name set has the GetParentScript member of NScript::CQ_OrchardFarmRaidScript::CCrateTeamMember; slot layout from named TLC siblings; function referenced by this vtable only.
//   ?GetParentScript@CCrateTeamMember@CQ_OrchardFarmRaidScript@NScript@@UBEPAVCScriptBase@@XZ in FableWin.pdb (rebuild/candidates/manual/naming/2026-11-04_anniversary/labels_high.tsv).
// Was: Member getter at this+0x14. __fastcall this=ecx. pack(1) keeps the field at the retail byte offset (natural alignment would shift an odd one). (catalog stand-in _gapscan, Leaf none).
// Class: the class model of the landed Alloc row 0x00DCDE20 (rebuild/src/compiled/00/dc/NScriptCQOrchardFarmRaidScriptCCrateTeamMember_Alloc_00dcde20.cpp); the Alloc body (if any) is not part of this graded body.
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
        virtual ~CCrateTeamMember();
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

CScriptBase* NScript::CQ_OrchardFarmRaidScript::CCrateTeamMember::GetParentScript() const
{
    return ParentClass;
}
