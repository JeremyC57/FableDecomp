// CEntityScriptBase::`scalar deleting destructor'(unsigned int flags) -- the destructor (expanded), then free the object when bit 0 is set
// Leaf: ??_GCEntityScriptBase@@UAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00CDEBF0, 30 bytes, thiscall, `ret 4`; calls 00f35b40=CEntityScriptBase::~CEntityScriptBase ; 00bfe9bc=MSVCR71.DLL::operator_delete.
// Identity (naming wave 35, family sdd): base vtable 012c3224 slot 0: every entity Alloc (614, e.g. 00d1e090) stores 012c3224 into the new block before the CScriptThing copy 0x004ABE90 and the class vtable = the inlined CEntityScriptBase ctor (PDB ??0CEntityScriptBase@@QAE@AAVCGameScriptInterfaceBase@@ABVCScriptThing@@@Z); slots 1 and 3 are _purecall 0x00BFEA08 (pure Main / GetParentScript); PDB ??_ECEntityScriptBase@@UAEPAXI@Z + ??1CEntityScriptBase@@UAE@XZ; refs=1; was CVtable_012c3224::`scalar deleting destructor'.
//   ??_GCEntityScriptBase@@UAEPAXI@Z (rebuild/candidates/manual/naming/2026-11-04_anniversary/wave3/labels_alloc.tsv); PDB: FableWin.pdb ??_ECEntityScriptBase@@UAEPAXI@Z; Ego_r.pdb / FableWin.pdb ??1CEntityScriptBase@@UAE@XZ.
//   Was: __fastcall this=ecx, flags=stack. base /O2 /Oy (no pragma). family len30 x22 (template 00412b00) (catalog stand-in CVtable_012c3224, Leaf `vector_deleting_destructor').
// Shape: the deleting destructor is emitted with the vtable, i.e. in the TU that constructs the object (wave 33 esb): the class model and
//   the PDB Alloc ?Alloc@CClickableDoor@CPersonalScript_GlobalThingsScript@NScript@@SI?AV?$CCountedPointer@VCEntityScriptBase@@@@AAV23@PAVCScriptDataBase@@ABVCScriptThing@@@Z of the landed row 0x00CDEE10 (rebuild/src/compiled/00/cd/NScriptCPersonalScriptGlobalThingsScriptCClickableDoor_Alloc_00cdee10.cpp), its own source with the FABLE_RETAIL_CLEVELSCRIPTBASE opt-in added; its local CCountedPointer stand-in replaced by engine/CCountedPointer.h (wave 33 D4). The Alloc body is not part of this graded body.
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
#define FABLE_RETAIL_CLEVELSCRIPTBASE   // retail's implicit ~CLevelScriptBase<T> (chunk 713 opt-in): the ??_G calls ~CScriptBase 0x00CBD510
#include "engine/CLevelScriptBase.h"

namespace NScript {
class CQ_SunnyvaleMasterData;
class CPersonalScript_GlobalThingsScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    class CClickableDoor : public CEntityScriptBase {
    public:
        CPersonalScript_GlobalThingsScript* ParentClass; // +0x14
        CQ_SunnyvaleMasterData* PMasterData;            // +0x18
        bool IsDoorOpen;                                // +0x1C
        CClickableDoor(CPersonalScript_GlobalThingsScript& parent, CScriptDataBase* data, const CScriptThing& me)
            : CEntityScriptBase(*parent.Game, me), ParentClass(&parent)
        {
            PMasterData = (CQ_SunnyvaleMasterData*)data;
        }
        virtual ~CClickableDoor();
        static CCountedPointer<CEntityScriptBase> __fastcall Alloc(CPersonalScript_GlobalThingsScript& parent, CScriptDataBase* data, const CScriptThing& me);
    private:
    virtual void Main();                                 // PDB ?Main@<entity>@..@@EAEXXZ (slot 1)
public:
    virtual CScriptBase* GetParentScript() const;        // PDB ?GetParentScript@<entity>@..@@UBEPAVCScriptBase@@XZ (slot 3)
};
};

CCountedPointer<CEntityScriptBase> __fastcall CPersonalScript_GlobalThingsScript::CClickableDoor::Alloc(CPersonalScript_GlobalThingsScript& parent, CScriptDataBase* data, const CScriptThing& me)
{
    return CCountedPointer<CEntityScriptBase>(new CClickableDoor(parent, data, me));
}
}
