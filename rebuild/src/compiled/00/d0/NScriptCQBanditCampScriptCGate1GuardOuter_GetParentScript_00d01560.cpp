// NScript::CQ_BanditCampScript::CGate1GuardOuter::GetParentScript() const -- the entity's parent level script (ParentClass, +0x14)
// Leaf: ?GetParentScript@CGate1GuardOuter@CQ_BanditCampScript@NScript@@UBEPAVCScriptBase@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00D01560, 4 bytes: mov eax,[ecx+14h]; ret.
// Identity (naming wave 35, family ent): reg 00d00b39 "Gate1GuardOuter" in 00d00af0; Alloc 00d014c0; vtable 012c8604 slot 3; PDB ?GetParentScript@CGate1GuardOuter@CQ_BanditCampScript@NScript@@; refs=1; Anniversary RTTI agrees (confirmed); was sub_00d01560.
//   ?GetParentScript@CGate1GuardOuter@CQ_BanditCampScript@NScript@@UBEPAVCScriptBase@@XZ in FableWin.pdb (rebuild/candidates/manual/naming/2026-11-04_anniversary/wave3/labels_alloc.tsv).
// Was: Member getter at this+0x14. __fastcall this=ecx. pack(1) keeps the field at the retail byte offset (natural alignment would shift an odd one). (catalog stand-in _gapscan, Leaf none).
// Class: the class model of the landed Alloc row 0x00D014C0 (rebuild/src/compiled/00/d0/NScriptCQBanditCampScriptCGate1GuardOuter_Alloc_00d014c0.cpp); the Alloc body (if any) is not part of this graded body.
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
class CQ_BanditCampScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    class CGate1GuardOuter : public CEntityScriptBase {
    public:
        CQ_BanditCampScript* ParentClass;               // +0x14
        CQ_SunnyvaleMasterData* PMasterData;            // +0x18
        long AIState;                                   // +0x1C
        CGate1GuardOuter(CQ_BanditCampScript& parent, CScriptDataBase* data, const CScriptThing& me)
            : CEntityScriptBase(*parent.Game, me), ParentClass(&parent)
        {
            PMasterData = (CQ_SunnyvaleMasterData*)data;
        }
        virtual ~CGate1GuardOuter();
        static CCountedPointer<CEntityScriptBase> __fastcall Alloc(CQ_BanditCampScript& parent, CScriptDataBase* data, const CScriptThing& me);
    private:
    virtual void Main();                                 // PDB ?Main@<entity>@..@@EAEXXZ (slot 1)
public:
    virtual CScriptBase* GetParentScript() const;        // PDB ?GetParentScript@<entity>@..@@UBEPAVCScriptBase@@XZ (slot 3)
};
};

CCountedPointer<CEntityScriptBase> __fastcall CQ_BanditCampScript::CGate1GuardOuter::Alloc(CQ_BanditCampScript& parent, CScriptDataBase* data, const CScriptThing& me)
{
    return CCountedPointer<CEntityScriptBase>(new CGate1GuardOuter(parent, data, me));
}
}

CScriptBase* NScript::CQ_BanditCampScript::CGate1GuardOuter::GetParentScript() const
{
    return ParentClass;
}
