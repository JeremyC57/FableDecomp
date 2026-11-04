// NScript::CQ_HeroSoulsNostroScript::CNostro::OnPredicateFail() -- set CQ_SunnyvaleMasterData::CollectedSoulFromNostro
// Leaf: ?OnPredicateFail@CNostro@CQ_HeroSoulsNostroScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00D89350, 8 bytes; PMasterData +0x18 -> CollectedSoulFromNostro +0x68.
// Identity (naming wave 35, family opfmd): reg 00d87e4b "Nostro" in 00d87cb0; Alloc 00d892a0; vtable 012d4f50 slot 5; PDB ?OnPredicateFail@CNostro@CQ_HeroSoulsNostroScript@NScript@@; refs=1; Anniversary RTTI agrees (confirmed); was _unattributed::SetFlag_00d89350.
//   ?OnPredicateFail@CNostro@CQ_HeroSoulsNostroScript@NScript@@EAEXXZ in Ego_r.pdb / FableWin.pdb (rebuild/candidates/manual/naming/2026-11-04_anniversary/wave3/labels_alloc.tsv). The flag is named by engine/CQ_SunnyvaleMasterData.h (bool CollectedSoulFromNostro; // +0x68).
// Was: CClass_00d89350::SetFlag_00d89350() -- sets the byte flag at +0x68 of the object at +0x18 Retail: Fable.exe (TLC, VC7.1) function @ 0x00D89350, oracle length 8 bytes. (catalog stand-in _unattributed).
// Class: the class model of the landed Alloc row 0x00D892A0 (rebuild/src/compiled/00/d8/NScriptCQHeroSoulsNostroScriptCNostro_Alloc_00d892a0.cpp); the PDB member declaration added.
#define FABLE_RETAIL_NSCRIPT
#include "engine/CQ_SunnyvaleMasterData.h"   // the master data the entity writes (members at their retail offsets)
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
class CQ_HeroSoulsNostroScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    class CNostro : public CEntityScriptBase {
    public:
        CQ_HeroSoulsNostroScript* ParentClass;          // +0x14
        CQ_SunnyvaleMasterData* PMasterData;            // +0x18

        CNostro(CQ_HeroSoulsNostroScript& parent, CScriptDataBase* data, const CScriptThing& me)
            : CEntityScriptBase(*parent.Game, me), ParentClass(&parent)
        {
            PMasterData = (CQ_SunnyvaleMasterData*)data;
        }
        virtual ~CNostro();
        static CCountedPointer<CEntityScriptBase> __fastcall Alloc(CQ_HeroSoulsNostroScript& parent, CScriptDataBase* data, const CScriptThing& me);
    private:
    virtual void Main();                                 // PDB ?Main@<entity>@..@@EAEXXZ (slot 1)
public:
    virtual CScriptBase* GetParentScript() const;        // PDB ?GetParentScript@<entity>@..@@UBEPAVCScriptBase@@XZ (slot 3)
private:
    virtual void OnPredicateFail();   // PDB ?OnPredicateFail@CNostro@CQ_HeroSoulsNostroScript@NScript@@EAEXXZ (slot 5)
};
};

CCountedPointer<CEntityScriptBase> __fastcall CQ_HeroSoulsNostroScript::CNostro::Alloc(CQ_HeroSoulsNostroScript& parent, CScriptDataBase* data, const CScriptThing& me)
{
    return CCountedPointer<CEntityScriptBase>(new CNostro(parent, data, me));
}
}

void NScript::CQ_HeroSoulsNostroScript::CNostro::OnPredicateFail()
{
    PMasterData->CollectedSoulFromNostro = true;
}
