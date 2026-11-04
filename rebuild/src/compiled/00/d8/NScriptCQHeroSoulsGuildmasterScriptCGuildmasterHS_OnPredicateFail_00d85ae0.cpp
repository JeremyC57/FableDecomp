// NScript::CQ_HeroSoulsGuildmasterScript::CGuildmaster_HS::OnPredicateFail() -- the entity was killed (Me.MsgIsKilledBy("")): set CQ_SunnyvaleMasterData::KilledGM
// Leaf: ?OnPredicateFail@CGuildmaster_HS@CQ_HeroSoulsGuildmasterScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00D85AE0, 61 bytes, CScriptThing slot 18 MsgIsKilledBy (+0x48) on Me with a temporary CCharString("") (0x0099EBF0 / ~ 0x0099EAE0); PMasterData +0x18 -> KilledGM +0x67.
// Identity (naming wave 35, family opfmd): reg 00d84db3 "Guildmaster_HS" in 00d84d60; Alloc 00d85a30; vtable 012d4a98 slot 5; PDB ?OnPredicateFail@CGuildmaster_HS@CQ_HeroSoulsGuildmasterScript@NScript@@; refs=1; Anniversary RTTI agrees (confirmed); was _unattributed::Query__00d85ae0.
//   ?OnPredicateFail@CGuildmaster_HS@CQ_HeroSoulsGuildmasterScript@NScript@@EAEXXZ in Ego_r.pdb / FableWin.pdb (rebuild/candidates/manual/naming/2026-11-04_anniversary/wave3/labels_alloc.tsv). The flag is named by engine/CQ_SunnyvaleMasterData.h (bool KilledGM; // +0x67).
// Was: CNamedQuery_00d85ae0::Query__00d85ae0() -- asks virtual slot 18 of the embedded object at +0x8 about ""; on true sets the byte at +0x67 of the object at +0x18 Retail: Fable.exe (TLC, VC7.1) function @ 0x00D85AE0, orac... (catalog stand-in _unattributed).
// Class: the class model of the landed Alloc row 0x00D85A30 (rebuild/src/compiled/00/d8/NScriptCQHeroSoulsGuildmasterScriptCGuildmasterHS_Alloc_00d85a30.cpp); the PDB member declaration added.
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
class CQ_HeroSoulsGuildmasterScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    class CGuildmaster_HS : public CEntityScriptBase {
    public:
        CQ_HeroSoulsGuildmasterScript* ParentClass;     // +0x14
        CQ_SunnyvaleMasterData* PMasterData;            // +0x18

        CGuildmaster_HS(CQ_HeroSoulsGuildmasterScript& parent, CScriptDataBase* data, const CScriptThing& me)
            : CEntityScriptBase(*parent.Game, me), ParentClass(&parent)
        {
            PMasterData = (CQ_SunnyvaleMasterData*)data;
        }
        virtual ~CGuildmaster_HS();
        static CCountedPointer<CEntityScriptBase> __fastcall Alloc(CQ_HeroSoulsGuildmasterScript& parent, CScriptDataBase* data, const CScriptThing& me);
    private:
    virtual void Main();                                 // PDB ?Main@<entity>@..@@EAEXXZ (slot 1)
public:
    virtual CScriptBase* GetParentScript() const;        // PDB ?GetParentScript@<entity>@..@@UBEPAVCScriptBase@@XZ (slot 3)
private:
    virtual void OnPredicateFail();   // PDB ?OnPredicateFail@CGuildmaster_HS@CQ_HeroSoulsGuildmasterScript@NScript@@EAEXXZ (slot 5)
};
};

CCountedPointer<CEntityScriptBase> __fastcall CQ_HeroSoulsGuildmasterScript::CGuildmaster_HS::Alloc(CQ_HeroSoulsGuildmasterScript& parent, CScriptDataBase* data, const CScriptThing& me)
{
    return CCountedPointer<CEntityScriptBase>(new CGuildmaster_HS(parent, data, me));
}
}

void NScript::CQ_HeroSoulsGuildmasterScript::CGuildmaster_HS::OnPredicateFail()
{
    if (Me.MsgIsKilledBy(""))
        PMasterData->KilledGM = true;
}
