// NScript::CV_FishermanScript::CM_FishermanWaspSpawn::GetParentScript() const -- the entity's parent level script (ParentClass, +0x14)
// Leaf: ?GetParentScript@CM_FishermanWaspSpawn@CV_FishermanScript@NScript@@UBEPAVCScriptBase@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E7B930, 4 bytes: mov eax,[ecx+14h]; ret.
// Identity (naming wave 35, family ent): reg 00e7b58d "M_FishermanWaspSpawn" in 00e7b4e0; Alloc 00e7b890; vtable 012ea0d0 slot 3; PDB ?GetParentScript@CM_FishermanWaspSpawn@CV_FishermanScript@NScript@@; refs=1; Anniversary RTTI agrees (confirmed); was sub_00e7b930.
//   ?GetParentScript@CM_FishermanWaspSpawn@CV_FishermanScript@NScript@@UBEPAVCScriptBase@@XZ in FableWin.pdb (rebuild/candidates/manual/naming/2026-11-04_anniversary/wave3/labels_alloc.tsv).
// Was: Member getter at this+0x14. __fastcall this=ecx. pack(1) keeps the field at the retail byte offset (natural alignment would shift an odd one). (catalog stand-in _gapscan, Leaf none).
// Class: the class model of the landed Alloc row 0x00E7B890 (rebuild/src/compiled/00/e7/NScriptCVFishermanScriptCMFishermanWaspSpawn_Alloc_00e7b890.cpp); the Alloc body (if any) is not part of this graded body.
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
class CV_FishermanScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    class CM_FishermanWaspSpawn : public CEntityScriptBase {
    public:
        CV_FishermanScript* ParentClass;                // +0x14
        CQ_SunnyvaleMasterData* PMasterData;            // +0x18

        CM_FishermanWaspSpawn(CV_FishermanScript& parent, CScriptDataBase* data, const CScriptThing& me)
            : CEntityScriptBase(*parent.Game, me), ParentClass(&parent)
        {
            PMasterData = (CQ_SunnyvaleMasterData*)data;
        }
        virtual ~CM_FishermanWaspSpawn();
        static CCountedPointer<CEntityScriptBase> __fastcall Alloc(CV_FishermanScript& parent, CScriptDataBase* data, const CScriptThing& me);
    private:
    virtual void Main();                                 // PDB ?Main@<entity>@..@@EAEXXZ (slot 1)
public:
    virtual CScriptBase* GetParentScript() const;        // PDB ?GetParentScript@<entity>@..@@UBEPAVCScriptBase@@XZ (slot 3)
};
};

CCountedPointer<CEntityScriptBase> __fastcall CV_FishermanScript::CM_FishermanWaspSpawn::Alloc(CV_FishermanScript& parent, CScriptDataBase* data, const CScriptThing& me)
{
    return CCountedPointer<CEntityScriptBase>(new CM_FishermanWaspSpawn(parent, data, me));
}
}

CScriptBase* NScript::CV_FishermanScript::CM_FishermanWaspSpawn::GetParentScript() const
{
    return ParentClass;
}
