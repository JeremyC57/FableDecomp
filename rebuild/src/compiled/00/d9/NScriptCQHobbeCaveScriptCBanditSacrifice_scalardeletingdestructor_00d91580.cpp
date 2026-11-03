// NScript::CQ_HobbeCaveScript::CBanditSacrifice::`scalar deleting destructor'(unsigned int flags) -- the implicit ~CBanditSacrifice (its members' destructors, then ~CEntityScriptBase), then free the entity when bit 0 is set
// Leaf: ??_GCBanditSacrifice@CQ_HobbeCaveScript@NScript@@UAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00D91580, 40 bytes, thiscall, `ret 4`; calls 004aa840=CScriptThing::~CScriptThing ; 00f35b40=CEntityScriptBase::~CEntityScriptBase ; 00bfe9bc=MSVCR71.DLL::operator_delete.
// Naming wave 33 (family esb): slot 0 of vtable 0x012D5A94 (no RTTI: NScript TU), the vtable the landed Alloc 0x00D90E90 stores
//   (?Alloc@CBanditSacrifice@CQ_HobbeCaveScript@NScript@@SI?AV?$CCountedPointer@VCEntityScriptBase@@@@AAV23@PAVCScriptDataBase@@ABVCScriptThing@@@Z, Ego_r.pdb / FableWin.pdb); FableWin.pdb ??_GCBanditSacrifice@CQ_HobbeCaveScript@NScript@@UAEPAXI@Z; Ego_r.pdb ??_ECBanditSacrifice@CQ_HobbeCaveScript@NScript@@UAEPAXI@Z; FableWin.pdb ??1CBanditSacrifice@CQ_HobbeCaveScript@NScript@@UAE@XZ.
//   Ego_r.pdb (release) has no out-of-line ??1 for the entity, and retail calls ~CEntityScriptBase directly: the entity destructor is implicit.
//   Was a free __fastcall CLandscapeBackgroundPatch_vector_deleting_destructor stand-in (catalog CVtable_012d5a94).
// Shape: the deleting destructor is emitted with the vtable, i.e. in the TU that constructs the entity: the class model and the PDB Alloc
//   of the landed row 0x00D90E90 (rebuild/src/compiled/00/d9/NScriptCQHobbeCaveScriptCBanditSacrifice_Alloc_00d90e90.cpp), its user-declared destructor dropped. The Alloc body is not part of this graded body.
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
class CQ_HobbeCaveScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    class CBanditSacrifice : public CEntityScriptBase {
    public:
        CQ_HobbeCaveScript* ParentClass;                // +0x14
        CQ_SunnyvaleMasterData* PMasterData;            // +0x18
        long CurrentAIState;                            // +0x1C
        long PreviousAIState;                           // +0x20
        long BanditHealthID;                            // +0x24
        CScriptThing ImprisonedFX;                      // +0x28 CScriptThing (Ego_r)
        bool MadeMainChamberHobbesComment;              // +0x34
        long FollowingCommentsSaid;                     // +0x38
        CBanditSacrifice(CQ_HobbeCaveScript& parent, CScriptDataBase* data, const CScriptThing& me)
            : CEntityScriptBase(*parent.Game, me), ParentClass(&parent)
        {
            PMasterData = (CQ_SunnyvaleMasterData*)data;
        }
        static CCountedPointer<CEntityScriptBase> __fastcall Alloc(CQ_HobbeCaveScript& parent, CScriptDataBase* data, const CScriptThing& me);
    private:
    virtual void Main();                                 // PDB ?Main@<entity>@..@@EAEXXZ (slot 1)
public:
    virtual CScriptBase* GetParentScript() const;        // PDB ?GetParentScript@<entity>@..@@UBEPAVCScriptBase@@XZ (slot 3)
};
};

CCountedPointer<CEntityScriptBase> __fastcall CQ_HobbeCaveScript::CBanditSacrifice::Alloc(CQ_HobbeCaveScript& parent, CScriptDataBase* data, const CScriptThing& me)
{
    return CCountedPointer<CEntityScriptBase>(new CBanditSacrifice(parent, data, me));
}
}