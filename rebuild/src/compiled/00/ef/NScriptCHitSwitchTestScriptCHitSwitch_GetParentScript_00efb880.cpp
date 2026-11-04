// NScript::CHitSwitchTestScript::CHitSwitch::GetParentScript() const -- the entity's parent level script (ParentClass, +0x14)
// Leaf: ?GetParentScript@CHitSwitch@CHitSwitchTestScript@NScript@@UBEPAVCScriptBase@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00EFB880, 4 bytes: mov eax,[ecx+14h]; ret.
// Identity (naming wave 35, family ent): reg 00efb787 "HitSwitch" in 00efb740; Alloc 00efb7e0; vtable 012f6b9c slot 3; PDB ?GetParentScript@CHitSwitch@CHitSwitchTestScript@NScript@@; refs=1; was sub_00efb880.
//   ?GetParentScript@CHitSwitch@CHitSwitchTestScript@NScript@@UBEPAVCScriptBase@@XZ in FableWin.pdb (rebuild/candidates/manual/naming/2026-11-04_anniversary/wave3/labels_alloc.tsv).
// Was: Member getter at this+0x14. __fastcall this=ecx. pack(1) keeps the field at the retail byte offset (natural alignment would shift an odd one). (catalog stand-in _gapscan, Leaf none).
// Class: the class model of the landed Alloc row 0x00EFB7E0 (rebuild/src/compiled/00/ef/NScriptCHitSwitchTestScriptCHitSwitch_Alloc_00efb7e0.cpp); the Alloc body (if any) is not part of this graded body.
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
class CDanyalTestScriptsData;
class CHitSwitchTestScript : public CLevelScriptBase<CDanyalTestScriptsData> {
public:
    class CHitSwitch : public CEntityScriptBase {
    public:
        CHitSwitchTestScript* ParentClass;              // +0x14
        CDanyalTestScriptsData* PMasterData;            // +0x18

        CHitSwitch(CHitSwitchTestScript& parent, CScriptDataBase* data, const CScriptThing& me)
            : CEntityScriptBase(*parent.Game, me), ParentClass(&parent)
        {
            PMasterData = (CDanyalTestScriptsData*)data;
        }
        virtual ~CHitSwitch();
        static CCountedPointer<CEntityScriptBase> __fastcall Alloc(CHitSwitchTestScript& parent, CScriptDataBase* data, const CScriptThing& me);
    private:
    virtual void Main();                                 // PDB ?Main@<entity>@..@@EAEXXZ (slot 1)
public:
    virtual CScriptBase* GetParentScript() const;        // PDB ?GetParentScript@<entity>@..@@UBEPAVCScriptBase@@XZ (slot 3)
};
};

CCountedPointer<CEntityScriptBase> __fastcall CHitSwitchTestScript::CHitSwitch::Alloc(CHitSwitchTestScript& parent, CScriptDataBase* data, const CScriptThing& me)
{
    return CCountedPointer<CEntityScriptBase>(new CHitSwitch(parent, data, me));
}
}

CScriptBase* NScript::CHitSwitchTestScript::CHitSwitch::GetParentScript() const
{
    return ParentClass;
}
