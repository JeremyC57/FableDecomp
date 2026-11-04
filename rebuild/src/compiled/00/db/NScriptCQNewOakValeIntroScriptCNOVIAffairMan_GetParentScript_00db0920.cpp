// NScript::CQ_NewOakValeIntroScript::CNOVI_AffairMan::GetParentScript() const -- the entity's parent level script (ParentClass, +0x14)
// Leaf: ?GetParentScript@CNOVI_AffairMan@CQ_NewOakValeIntroScript@NScript@@UBEPAVCScriptBase@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00DB0920, 4 bytes: mov eax,[ecx+14h]; ret.
// Identity (naming wave 35, family ent): reg 00dabdca "NOVI_AffairMan" in 00dabac0; Alloc 00db0880; vtable 012d89ec slot 3; PDB ?GetParentScript@CNOVI_AffairMan@CQ_NewOakValeIntroScript@NScript@@; refs=1; Anniversary RTTI agrees (confirmed); was sub_00db0920.
//   ?GetParentScript@CNOVI_AffairMan@CQ_NewOakValeIntroScript@NScript@@UBEPAVCScriptBase@@XZ in FableWin.pdb (rebuild/candidates/manual/naming/2026-11-04_anniversary/wave3/labels_alloc.tsv).
// Was: Member getter at this+0x14. __fastcall this=ecx. pack(1) keeps the field at the retail byte offset (natural alignment would shift an odd one). (catalog stand-in _gapscan, Leaf none).
// Class: the class model of the landed Alloc row 0x00DB0880 (rebuild/src/compiled/00/db/NScriptCQNewOakValeIntroScriptCNOVIAffairMan_Alloc_00db0880.cpp); the Alloc body (if any) is not part of this graded body.
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
class CQ_NewOakValeIntroScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    class CNOVI_AffairMan : public CEntityScriptBase {
    public:
        CQ_NewOakValeIntroScript* ParentClass;          // +0x14
        CQ_SunnyvaleMasterData* PMasterData;            // +0x18
        bool EncounterOver;                             // +0x1C
        bool SaidFirstRangedComment;                    // +0x1D
        bool HeroAgreedToKeepQuiet;                     // +0x1E
        bool HeroSaidHeWouldReportMan;                  // +0x1F
        long BadgerIndex;                               // +0x20
        CNOVI_AffairMan(CQ_NewOakValeIntroScript& parent, CScriptDataBase* data, const CScriptThing& me)
            : CEntityScriptBase(*parent.Game, me), ParentClass(&parent)
        {
            PMasterData = (CQ_SunnyvaleMasterData*)data;
        }
        virtual ~CNOVI_AffairMan();
        static CCountedPointer<CEntityScriptBase> __fastcall Alloc(CQ_NewOakValeIntroScript& parent, CScriptDataBase* data, const CScriptThing& me);
    private:
    virtual void Main();                                 // PDB ?Main@<entity>@..@@EAEXXZ (slot 1)
public:
    virtual CScriptBase* GetParentScript() const;        // PDB ?GetParentScript@<entity>@..@@UBEPAVCScriptBase@@XZ (slot 3)
};
};

CCountedPointer<CEntityScriptBase> __fastcall CQ_NewOakValeIntroScript::CNOVI_AffairMan::Alloc(CQ_NewOakValeIntroScript& parent, CScriptDataBase* data, const CScriptThing& me)
{
    return CCountedPointer<CEntityScriptBase>(new CNOVI_AffairMan(parent, data, me));
}
}

CScriptBase* NScript::CQ_NewOakValeIntroScript::CNOVI_AffairMan::GetParentScript() const
{
    return ParentClass;
}
