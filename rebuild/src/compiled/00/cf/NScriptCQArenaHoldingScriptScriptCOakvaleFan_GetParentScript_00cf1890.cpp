// NScript::CQ_ArenaHoldingScriptScript::COakvaleFan::GetParentScript() const -- the entity's parent level script (ParentClass, +0x14)
// Leaf: ?GetParentScript@COakvaleFan@CQ_ArenaHoldingScriptScript@NScript@@UBEPAVCScriptBase@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00CF1890, 4 bytes: mov eax,[ecx+14h]; ret.
// Identity (naming wave 35, family ent): reg 00cf0973 "OakvaleFan" in 00cf0790; Alloc 00cf17f0; vtable 012c6260 slot 3; PDB ?GetParentScript@COakvaleFan@CQ_ArenaHoldingScriptScript@NScript@@; refs=1; Anniversary RTTI agrees (confirmed); was sub_00cf1890.
//   ?GetParentScript@COakvaleFan@CQ_ArenaHoldingScriptScript@NScript@@UBEPAVCScriptBase@@XZ in FableWin.pdb (rebuild/candidates/manual/naming/2026-11-04_anniversary/wave3/labels_alloc.tsv).
// Was: Member getter at this+0x14. __fastcall this=ecx. pack(1) keeps the field at the retail byte offset (natural alignment would shift an odd one). (catalog stand-in _gapscan, Leaf none).
// Class: the class model of the landed Alloc row 0x00CF17F0 (rebuild/src/compiled/00/cf/NScriptCQArenaHoldingScriptScriptCOakvaleFan_Alloc_00cf17f0.cpp); its local CCountedPointer stand-in replaced by engine/CCountedPointer.h (wave 33 D4); the Alloc body (if any) is not part of this graded body.
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
    class COakvaleFan : public CEntityScriptBase {
    public:
        CQ_ArenaHoldingScriptScript* ParentClass;       // +0x14
        CQ_SunnyvaleMasterData* PMasterData;            // +0x18

        COakvaleFan(CQ_ArenaHoldingScriptScript& parent, CScriptDataBase* data, const CScriptThing& me)
            : CEntityScriptBase(*parent.Game, me), ParentClass(&parent)
        {
            PMasterData = (CQ_SunnyvaleMasterData*)data;
        }
        virtual ~COakvaleFan();
        static CCountedPointer<CEntityScriptBase> __fastcall Alloc(CQ_ArenaHoldingScriptScript& parent, CScriptDataBase* data, const CScriptThing& me);
    private:
    virtual void Main();                                 // PDB ?Main@<entity>@..@@EAEXXZ (slot 1)
public:
    virtual CScriptBase* GetParentScript() const;        // PDB ?GetParentScript@<entity>@..@@UBEPAVCScriptBase@@XZ (slot 3)
};
};

CCountedPointer<CEntityScriptBase> __fastcall CQ_ArenaHoldingScriptScript::COakvaleFan::Alloc(CQ_ArenaHoldingScriptScript& parent, CScriptDataBase* data, const CScriptThing& me)
{
    return CCountedPointer<CEntityScriptBase>(new COakvaleFan(parent, data, me));
}
}

CScriptBase* NScript::CQ_ArenaHoldingScriptScript::COakvaleFan::GetParentScript() const
{
    return ParentClass;
}
