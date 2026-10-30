// NScript::CQ_OrchardFarmRaidScript::CGuardTeamMember::Alloc(CQ_OrchardFarmRaidScript& parent, CScriptDataBase* data, const CScriptThing& me) -- entity-script factory of the "GuardTeamMember" binding: new CGuardTeamMember(parent, data, me) (CEntityScriptBase(*parent.Game, me), ParentClass = &parent, the members' constructors, PMasterData = data) handed to a fresh CCountedPointer<CEntityScriptBase>
// Leaf: ?Alloc@CGuardTeamMember@CQ_OrchardFarmRaidScript@NScript@@SI?AV?$CCountedPointer@VCEntityScriptBase@@@@AAV23@PAVCScriptDataBase@@ABVCScriptThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00DCDE20, 153 bytes (+ int3 padding), static `__fastcall` (hidden result
//   pointer ecx, parent edx, data and me on the stack), `ret 8`, returns the result pointer.
// Identity: no PDB entry: the debug-build CQ_OrchardFarmRaidScript has no CGuardTeamMember (its other entity classes, e.g. CCrateTeamMember, are there); the class is named by the binding convention C<binding name> that holds for the 14 PDB-named rows of this family (descriptive class name), and its members between +0x1C and +0x30 are descriptive.
//   CQ_OrchardFarmRaidScript::Main stores this address as the factory of a CEntityScriptBinding<CQ_OrchardFarmRaidScript> named "GuardTeamMember"
//   (site 0x00DCC926, then CScriptBase::AddEntityScriptBinding 0x00CB8230); that Main 0x00DCC770 is slot 2 of the script vtable 0x012DAF78, which the "Q_OrchardFarmRaid" script's Alloc stores (CScriptInfoManager registration).
//   Family F014 (15 bodies, one per entity class): the manifest label "CTriangleList::CTriangleList" is propagated and
//   wrong; the landed twins of this shape (0x00D62360 etc.) are named by vtable address (Create_CVtable_...).
// Callees: scalar operator new 0x00BFEA1A (0x3C = sizeof(CGuardTeamMember), then 0x0C for the count block),
//   CScriptThing::CScriptThing(const CScriptThing&) 0x004ABE90 (Me), CScriptThing::CScriptThing() 0x006E7B40 (_scriptThing030 +0x30), and the
//   count block's deleter 0x00CDEE00 (stored, not called: `if (p) p->~CEntityScriptBase()` deleting, `__fastcall`).
// Data: vtables CEntityScriptBase 0x012C3224 (stored before the Me copy) and CGuardTeamMember 0x012DB364.
// Layout: Ego_r.pdb CQ_OrchardFarmRaidScript::CGuardTeamMember (60 bytes = the retail new size): ParentClass +0x14, PMasterData +0x18, _unknown01c +0x1C, _scriptThing030 +0x30.
//   CEntityScriptBase: vptr, Game +0x04, Me +0x08 (Ego_r 20 bytes). The parent's Game is CLevelScriptBase's +0x40.
// Shape: the PDB ctor (parent, data, me) inlined: base and ParentClass in the init list, PMasterData assigned in the body
//   (its store follows the member constructor call); STLport-free counted pointer ctor: Data first, the null test on the
//   argument, the 12-byte {1, deleter, Data} block read back from Data, three exits (block, failed new, null object).
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
    class CGuardTeamMember : public CEntityScriptBase {
    public:
        CQ_OrchardFarmRaidScript* ParentClass;          // +0x14
        CQ_SunnyvaleMasterData* PMasterData;            // +0x18
        unsigned char _unknown01c[0x14];                // +0x1C (descriptive: no PDB layout)
        CScriptThing _scriptThing030;                   // +0x30 (descriptive: no PDB layout)
        CGuardTeamMember(CQ_OrchardFarmRaidScript& parent, CScriptDataBase* data, const CScriptThing& me)
            : CEntityScriptBase(*parent.Game, me), ParentClass(&parent)
        {
            PMasterData = (CQ_SunnyvaleMasterData*)data;
        }
        virtual ~CGuardTeamMember();
        static CCountedPointer<CEntityScriptBase> __fastcall Alloc(CQ_OrchardFarmRaidScript& parent, CScriptDataBase* data, const CScriptThing& me);
    private:
    virtual void Main();                                 // PDB ?Main@<entity>@..@@EAEXXZ (slot 1)
public:
    virtual CScriptBase* GetParentScript() const;        // PDB ?GetParentScript@<entity>@..@@UBEPAVCScriptBase@@XZ (slot 3)
};
};

CCountedPointer<CEntityScriptBase> __fastcall CQ_OrchardFarmRaidScript::CGuardTeamMember::Alloc(CQ_OrchardFarmRaidScript& parent, CScriptDataBase* data, const CScriptThing& me)
{
    return CCountedPointer<CEntityScriptBase>(new CGuardTeamMember(parent, data, me));
}
}