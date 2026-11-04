// NScript::CQ_OpeningGraveyardSecretPassageScript::CSecretPassageMan::Init() -- resets the entity state: AttackedByHero = false
// Leaf: ?Init@CSecretPassageMan@CQ_OpeningGraveyardSecretPassageScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00DC4CF0, 5 bytes, thiscall, plain `ret`.
// Identity: reg 00dc2ad7 "SecretPassageMan" in 00dc27c0; Alloc 00dc4c40; vtable 012da6a8 slot 2; PDB ?Init@CSecretPassageMan@CQ_OpeningGraveyardSecretPassageScript@NScript@@; refs=1; Anniversary RTTI agrees (confirmed); was sub_00dc4cf0 (wave3/labels_alloc.tsv); PDB ?Init@CSecretPassageMan@CQ_OpeningGraveyardSecretPassageScript@NScript@@EAEXXZ in FableWin.pdb.
// Naming wave 35: was landed as the gap-scan stand-in T::Clear() on a `struct T` pad model (00/dc/gapscan_sub00dc4cf0_00dc4cf0.cpp, `?Clear@T@@QAEXXZ`); same bytes.
// Class model: the landed Alloc row 0x00DC4C40 (rebuild/src/compiled/00/dc/NScriptCQOpeningGraveyardSecretPassageScriptCSecretPassageMan_Alloc_00dc4c40.cpp) as wave 33 D1 emits from it: its user-declared
//   destructor dropped (the entity destructor is implicit), Init declared private virtual as the PDB spells it; the Alloc body is not part of this TU.
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
class CQ_OpeningGraveyardSecretPassageScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    class CSecretPassageMan : public CEntityScriptBase {
    public:
        CQ_OpeningGraveyardSecretPassageScript* ParentClass; // +0x14
        CQ_SunnyvaleMasterData* PMasterData;            // +0x18
        bool AttackedByHero;                            // +0x1C
        CSecretPassageMan(CQ_OpeningGraveyardSecretPassageScript& parent, CScriptDataBase* data, const CScriptThing& me)
            : CEntityScriptBase(*parent.Game, me), ParentClass(&parent)
        {
            PMasterData = (CQ_SunnyvaleMasterData*)data;
        }
        static CCountedPointer<CEntityScriptBase> __fastcall Alloc(CQ_OpeningGraveyardSecretPassageScript& parent, CScriptDataBase* data, const CScriptThing& me);
    private:
    virtual void Main();                                 // PDB ?Main@<entity>@..@@EAEXXZ (slot 1)
    virtual void Init();                                 // PDB ?Init@CSecretPassageMan@CQ_OpeningGraveyardSecretPassageScript@NScript@@EAEXXZ (slot 2)
public:
    virtual CScriptBase* GetParentScript() const;        // PDB ?GetParentScript@<entity>@..@@UBEPAVCScriptBase@@XZ (slot 3)
};
};

void CQ_OpeningGraveyardSecretPassageScript::CSecretPassageMan::Init()
{
    AttackedByHero = false;
}
}
