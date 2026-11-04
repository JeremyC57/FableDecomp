// NScript::CQ_HeroSoulsThunderScript::CHeroThunder::OnPredicateFail() -- the entity was killed (Me.MsgIsKilledBy("")): set CQ_SunnyvaleMasterData::KilledThunder
// Leaf: ?OnPredicateFail@CHeroThunder@CQ_HeroSoulsThunderScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00D7B4B0, 61 bytes, CScriptThing slot 18 MsgIsKilledBy (+0x48) on Me with a temporary CCharString("") (0x0099EBF0 / ~ 0x0099EAE0); PMasterData +0x18 -> KilledThunder +0x63.
// Identity (naming wave 35, family opfmd): reg 00d7aeca "HeroThunder" in 00d7ae80; Alloc 00d7b400; vtable 012d3660 slot 5; PDB ?OnPredicateFail@CHeroThunder@CQ_HeroSoulsThunderScript@NScript@@; refs=1; Anniversary RTTI agrees (confirmed); was _unattributed::Query__00d7b4b0.
//   ?OnPredicateFail@CHeroThunder@CQ_HeroSoulsThunderScript@NScript@@EAEXXZ in Ego_r.pdb / FableWin.pdb (rebuild/candidates/manual/naming/2026-11-04_anniversary/wave3/labels_alloc.tsv). The flag is named by engine/CQ_SunnyvaleMasterData.h (bool KilledThunder; // +0x63).
// Was: CNamedQuery_00d7b4b0::Query__00d7b4b0() -- asks virtual slot 18 of the embedded object at +0x8 about ""; on true sets the byte at +0x63 of the object at +0x18 Retail: Fable.exe (TLC, VC7.1) function @ 0x00D7B4B0, orac... (catalog stand-in _unattributed).
// Class: the class model of the landed Alloc row 0x00D7B400 (rebuild/src/compiled/00/d7/NScriptCQHeroSoulsThunderScriptCHeroThunder_Alloc_00d7b400.cpp); the PDB member declaration added.
#define FABLE_RETAIL_CCHARSTRING
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
class CQ_HeroSoulsThunderScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    class CHeroThunder : public CEntityScriptBase {
    public:
        CQ_HeroSoulsThunderScript* ParentClass;         // +0x14
        CQ_SunnyvaleMasterData* PMasterData;            // +0x18

        CHeroThunder(CQ_HeroSoulsThunderScript& parent, CScriptDataBase* data, const CScriptThing& me)
            : CEntityScriptBase(*parent.Game, me), ParentClass(&parent)
        {
            PMasterData = (CQ_SunnyvaleMasterData*)data;
        }
        virtual ~CHeroThunder();
        static CCountedPointer<CEntityScriptBase> __fastcall Alloc(CQ_HeroSoulsThunderScript& parent, CScriptDataBase* data, const CScriptThing& me);
    private:
    virtual void Main();                                 // PDB ?Main@<entity>@..@@EAEXXZ (slot 1)
public:
    virtual CScriptBase* GetParentScript() const;        // PDB ?GetParentScript@<entity>@..@@UBEPAVCScriptBase@@XZ (slot 3)
private:
    virtual void OnPredicateFail();   // PDB ?OnPredicateFail@CHeroThunder@CQ_HeroSoulsThunderScript@NScript@@EAEXXZ (slot 5)
};
};

CCountedPointer<CEntityScriptBase> __fastcall CQ_HeroSoulsThunderScript::CHeroThunder::Alloc(CQ_HeroSoulsThunderScript& parent, CScriptDataBase* data, const CScriptThing& me)
{
    return CCountedPointer<CEntityScriptBase>(new CHeroThunder(parent, data, me));
}
}

void NScript::CQ_HeroSoulsThunderScript::CHeroThunder::OnPredicateFail()
{
    if (Me.MsgIsKilledBy(""))
        PMasterData->KilledThunder = true;
}
