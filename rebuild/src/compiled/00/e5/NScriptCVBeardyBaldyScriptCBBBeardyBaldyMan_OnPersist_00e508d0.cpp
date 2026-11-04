// NScript::CV_BeardyBaldyScript::CBB_BeardyBaldyMan::OnPersist(CPersistContext&) -- empty override (the entity persists nothing of its own)
// Leaf: ?OnPersist@CBB_BeardyBaldyMan@CV_BeardyBaldyScript@NScript@@EAEXAAVCPersistContext@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E508D0, 3 bytes: ret 4.
// Identity (naming wave 35, family ent): reg 00e4fa6a "BB_BeardyBaldyMan" in 00e4fa20; Alloc 00e507e0; vtable 012e5aec slot 4; PDB ?OnPersist@CBB_BeardyBaldyMan@CV_BeardyBaldyScript@NScript@@; refs=1; Anniversary RTTI agrees (confirmed); was sub_00e508d0.
//   ?OnPersist@CBB_BeardyBaldyMan@CV_BeardyBaldyScript@NScript@@EAEXAAVCPersistContext@@@Z in FableWin.pdb (rebuild/candidates/manual/naming/2026-11-04_anniversary/wave3/labels_alloc.tsv).
// Was: Empty member hook. __fastcall this=ecx. (catalog stand-in _gapscan, Leaf none).
// Class: the class model of the landed Alloc row 0x00E507E0 (rebuild/src/compiled/00/e5/NScriptCVBeardyBaldyScriptCBBBeardyBaldyMan_Alloc_00e507e0.cpp); the PDB member declaration added; the Alloc body (if any) is not part of this graded body.
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
class CV_BeardyBaldyScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    class CBB_BeardyBaldyMan : public CEntityScriptBase {
    public:
        CV_BeardyBaldyScript* ParentClass;              // +0x14
        CQ_SunnyvaleMasterData* PMasterData;            // +0x18
        long lastRandomSpeechIdx1;                      // +0x1C
        long lastRandomSpeechIdx2;                      // +0x20
        CBB_BeardyBaldyMan(CV_BeardyBaldyScript& parent, CScriptDataBase* data, const CScriptThing& me)
            : CEntityScriptBase(*parent.Game, me), ParentClass(&parent)
        {
            PMasterData = (CQ_SunnyvaleMasterData*)data;
        }
        virtual ~CBB_BeardyBaldyMan();
        static CCountedPointer<CEntityScriptBase> __fastcall Alloc(CV_BeardyBaldyScript& parent, CScriptDataBase* data, const CScriptThing& me);
    private:
    virtual void Main();                                 // PDB ?Main@<entity>@..@@EAEXXZ (slot 1)
public:
    virtual CScriptBase* GetParentScript() const;        // PDB ?GetParentScript@<entity>@..@@UBEPAVCScriptBase@@XZ (slot 3)
private:
    virtual void OnPersist(CPersistContext& context);   // PDB ?OnPersist@CBB_BeardyBaldyMan@CV_BeardyBaldyScript@NScript@@EAEXAAVCPersistContext@@@Z (slot 4)
};
};

CCountedPointer<CEntityScriptBase> __fastcall CV_BeardyBaldyScript::CBB_BeardyBaldyMan::Alloc(CV_BeardyBaldyScript& parent, CScriptDataBase* data, const CScriptThing& me)
{
    return CCountedPointer<CEntityScriptBase>(new CBB_BeardyBaldyMan(parent, data, me));
}
}

void NScript::CV_BeardyBaldyScript::CBB_BeardyBaldyMan::OnPersist(CPersistContext&)
{
}
