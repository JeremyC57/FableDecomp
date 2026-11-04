// NScript::CQ_WaspBossScript::CWaspHelper::GetParentScript() const -- the entity's parent level script (ParentClass, +0x14)
// Leaf: ?GetParentScript@CWaspHelper@CQ_WaspBossScript@NScript@@UBEPAVCScriptBase@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E0F860, 4 bytes: mov eax,[ecx+14h]; ret.
// Identity (naming wave 35, family ent): reg 00e0edb7 "WaspHelper" in 00e0ea40; Alloc 00e0f7c0; vtable 012e0284 slot 3; PDB ?GetParentScript@CWaspHelper@CQ_WaspBossScript@NScript@@; refs=1; Anniversary RTTI agrees (confirmed); was sub_00e0f860.
//   ?GetParentScript@CWaspHelper@CQ_WaspBossScript@NScript@@UBEPAVCScriptBase@@XZ in FableWin.pdb (rebuild/candidates/manual/naming/2026-11-04_anniversary/wave3/labels_alloc.tsv).
// Was: Member getter at this+0x14. __fastcall this=ecx. pack(1) keeps the field at the retail byte offset (natural alignment would shift an odd one). (catalog stand-in _gapscan, Leaf none).
// Class: the class model of the landed Alloc row 0x00E0F7C0 (rebuild/src/compiled/00/e0/NScriptCQWaspBossScriptCWaspHelper_Alloc_00e0f7c0.cpp); the Alloc body (if any) is not part of this graded body.
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
class CQ_WaspBossScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    class CWaspHelper : public CEntityScriptBase {
    public:
        CQ_WaspBossScript* ParentClass;                 // +0x14
        CQ_SunnyvaleMasterData* PMasterData;            // +0x18
        bool PleadedToHero;                             // +0x1C
        bool LeadToRegion;                              // +0x1D
        bool WavedOver;                                 // +0x1E
        CWaspHelper(CQ_WaspBossScript& parent, CScriptDataBase* data, const CScriptThing& me)
            : CEntityScriptBase(*parent.Game, me), ParentClass(&parent)
        {
            PMasterData = (CQ_SunnyvaleMasterData*)data;
        }
        virtual ~CWaspHelper();
        static CCountedPointer<CEntityScriptBase> __fastcall Alloc(CQ_WaspBossScript& parent, CScriptDataBase* data, const CScriptThing& me);
    private:
    virtual void Main();                                 // PDB ?Main@<entity>@..@@EAEXXZ (slot 1)
public:
    virtual CScriptBase* GetParentScript() const;        // PDB ?GetParentScript@<entity>@..@@UBEPAVCScriptBase@@XZ (slot 3)
};
};

CCountedPointer<CEntityScriptBase> __fastcall CQ_WaspBossScript::CWaspHelper::Alloc(CQ_WaspBossScript& parent, CScriptDataBase* data, const CScriptThing& me)
{
    return CCountedPointer<CEntityScriptBase>(new CWaspHelper(parent, data, me));
}
}

CScriptBase* NScript::CQ_WaspBossScript::CWaspHelper::GetParentScript() const
{
    return ParentClass;
}
