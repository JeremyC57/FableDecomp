// NScript::CQ_OpeningGraveyardSecretPassageScript::CSecretPassageSarcophagos::Main() -- run script frames until the thread is told to terminate
// Leaf: ?Main@CSecretPassageSarcophagos@CQ_OpeningGraveyardSecretPassageScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00DC44E0, 24 bytes, thiscall, plain `ret`; calls Game (+0x04) slot 7 NewScriptFrame (+0x1c), CEntityScriptBase::IsActiveThreadTerminating 0x00F35B30.
// Identity: reg 00dc2a70 "SecretPassageSarcophagos" in 00dc27c0; Alloc 00dc4420; vtable 012da5d4 slot 1; PDB ?Main@CSecretPassageSarcophagos@CQ_OpeningGraveyardSecretPassageScript@NScript@@; refs=1; Anniversary RTTI agrees (confirmed); was CVtable_012da5d4::vfunc_1 (wave3/labels_alloc.tsv); PDB ?Main@CSecretPassageSarcophagos@CQ_OpeningGraveyardSecretPassageScript@NScript@@EAEXXZ in Ego_r.pdb / FableWin.pdb.
// Naming wave 35: was landed as the free __fastcall stand-in `?Main@@YIXPAUCQ_SunnyvaleMasterScript@@@Z` on a pad model (00/dc/CQSunnyvaleMasterScript_Main_00dc44e0.cpp); same bytes.
// Class model: the landed Alloc row 0x00DC4420 (rebuild/src/compiled/00/dc/NScriptCQOpeningGraveyardSecretPassageScriptCSecretPassageSarcophagos_Alloc_00dc4420.cpp) as wave 33 D1 emits from it: its user-declared
//   destructor dropped, Main private virtual as the PDB spells it; the Alloc body is not part of this TU.
class CGameScriptInterfaceBase;
class CScriptDataBase;

// 0x00CDEE00 is CCountedPointer<T>::DeleteData (engine/CCountedPointer.h; reloc_bindings binds both spellings)

#include "engine/CCountedPointer.h"


// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"

// CEntityScriptBase: the retail model from engine/CEntityScriptBase.h (vtable 0x012C3224, 7 slots, 0x14: Game +0x04, Me +0x08).
#include "engine/CEntityScriptBase.h"
// CGameScriptInterfaceBase: the retail interface (slot 7 NewScriptFrame, +0x1c).
#include "engine/CGameScriptInterfaceBase.h"

// CScriptBase / CLevelScriptBase<T>: the retail NScript model from the shared headers (FABLE_RETAIL_NSCRIPT: CScriptBase :
//   CBaseClassNonCopyable : CBaseClass, vtable 0x012C1648, 5 slots, 0x40; CLevelScriptBase<T> Game +0x40, MasterScriptData +0x44).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CLevelScriptBase.h"

namespace NScript {
class CQ_SunnyvaleMasterData;
class CQ_OpeningGraveyardSecretPassageScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    class CSecretPassageSarcophagos : public CEntityScriptBase {
    public:
        CQ_OpeningGraveyardSecretPassageScript* ParentClass; // +0x14
        CQ_SunnyvaleMasterData* PMasterData;            // +0x18

        CSecretPassageSarcophagos(CQ_OpeningGraveyardSecretPassageScript& parent, CScriptDataBase* data, const CScriptThing& me)
            : CEntityScriptBase(*parent.Game, me), ParentClass(&parent)
        {
            PMasterData = (CQ_SunnyvaleMasterData*)data;
        }
        static CCountedPointer<CEntityScriptBase> __fastcall Alloc(CQ_OpeningGraveyardSecretPassageScript& parent, CScriptDataBase* data, const CScriptThing& me);
    private:
    virtual void Main();                                 // PDB ?Main@<entity>@..@@EAEXXZ (slot 1)
public:
    virtual CScriptBase* GetParentScript() const;        // PDB ?GetParentScript@<entity>@..@@UBEPAVCScriptBase@@XZ (slot 3)
};
};

void CQ_OpeningGraveyardSecretPassageScript::CSecretPassageSarcophagos::Main()
{
    do {
        Game->NewScriptFrame();
    } while (!IsActiveThreadTerminating());
}
}
