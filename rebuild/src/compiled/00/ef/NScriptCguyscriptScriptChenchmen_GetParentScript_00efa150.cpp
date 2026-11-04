// NScript::Cguy_scriptScript::Chenchmen::GetParentScript() const -- the entity's parent level script (ParentClass, +0x14)
// Leaf: ?GetParentScript@Chenchmen@Cguy_scriptScript@NScript@@UBEPAVCScriptBase@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00EFA150, 4 bytes: mov eax,[ecx+14h]; ret.
// Identity (naming wave 35, family ent): reg 00efa057 "henchmen" in 00efa010; Alloc 00efa0b0; vtable 012f69b0 slot 3; PDB ?GetParentScript@Chenchmen@Cguy_scriptScript@NScript@@; refs=1; was sub_00efa150.
//   ?GetParentScript@Chenchmen@Cguy_scriptScript@NScript@@UBEPAVCScriptBase@@XZ in FableWin.pdb (rebuild/candidates/manual/naming/2026-11-04_anniversary/wave3/labels_alloc.tsv).
// Was: Member getter at this+0x14. __fastcall this=ecx. pack(1) keeps the field at the retail byte offset (natural alignment would shift an odd one). (catalog stand-in _gapscan, Leaf none).
// Class: the class model of the landed Alloc row 0x00EFA0B0 (rebuild/src/compiled/00/ef/NScriptCguyscriptScriptChenchmen_Alloc_00efa0b0.cpp); the Alloc body (if any) is not part of this graded body.
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
class Cjoss_test_scriptsData;
class Cguy_scriptScript : public CLevelScriptBase<Cjoss_test_scriptsData> {
public:
    class Chenchmen : public CEntityScriptBase {
    public:
        Cguy_scriptScript* ParentClass;                 // +0x14
        Cjoss_test_scriptsData* PMasterData;            // +0x18

        Chenchmen(Cguy_scriptScript& parent, CScriptDataBase* data, const CScriptThing& me)
            : CEntityScriptBase(*parent.Game, me), ParentClass(&parent)
        {
            PMasterData = (Cjoss_test_scriptsData*)data;
        }
        virtual ~Chenchmen();
        static CCountedPointer<CEntityScriptBase> __fastcall Alloc(Cguy_scriptScript& parent, CScriptDataBase* data, const CScriptThing& me);
    private:
    virtual void Main();                                 // PDB ?Main@<entity>@..@@EAEXXZ (slot 1)
public:
    virtual CScriptBase* GetParentScript() const;        // PDB ?GetParentScript@<entity>@..@@UBEPAVCScriptBase@@XZ (slot 3)
};
};

CCountedPointer<CEntityScriptBase> __fastcall Cguy_scriptScript::Chenchmen::Alloc(Cguy_scriptScript& parent, CScriptDataBase* data, const CScriptThing& me)
{
    return CCountedPointer<CEntityScriptBase>(new Chenchmen(parent, data, me));
}
}

CScriptBase* NScript::Cguy_scriptScript::Chenchmen::GetParentScript() const
{
    return ParentClass;
}
