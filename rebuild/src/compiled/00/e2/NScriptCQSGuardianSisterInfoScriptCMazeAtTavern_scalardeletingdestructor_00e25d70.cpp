// NScript::CQS_GuardianSisterInfoScript::CMazeAtTavern::`scalar deleting destructor'(unsigned int flags) -- the implicit ~CMazeAtTavern (no members with destructors), i.e. ~CEntityScriptBase, then free the entity when bit 0 is set
// Leaf: ??_GCMazeAtTavern@CQS_GuardianSisterInfoScript@NScript@@UAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E25D70, 30 bytes: push esi; mov esi,ecx; call ~CEntityScriptBase 0x00F35B40; test byte [esp+8],1; je;
//   push esi; call operator delete 0x00BFE9BC; add esp,4; mov eax,esi; pop esi; ret 4.
// Naming wave 33 (family esb): slot 0 of vtable 0x012E1E28 (no RTTI: NScript TU), the vtable the landed Alloc 0x00E25C70 stores
//   (?Alloc@CMazeAtTavern@CQS_GuardianSisterInfoScript@NScript@@SI?AV?$CCountedPointer@VCEntityScriptBase@@@@AAV23@PAVCScriptDataBase@@ABVCScriptThing@@@Z, Ego_r.pdb / FableWin.pdb); FableWin.pdb ??_ECMazeAtTavern@CQS_GuardianSisterInfoScript@NScript@@UAEPAXI@Z; FableWin.pdb ??1CMazeAtTavern@CQS_GuardianSisterInfoScript@NScript@@UAE@XZ.
//   Ego_r.pdb (release) has no out-of-line ??1 for the entity, and retail calls ~CEntityScriptBase directly: the entity destructor is implicit.
//   Was a free __fastcall CLandscapeBackgroundPatch_vector_deleting_destructor stand-in (catalog CVtable_012e1e28).
// Shape: the deleting destructor is emitted with the vtable, i.e. in the TU that constructs the entity: the class model and the PDB Alloc
//   of the landed row 0x00E25C70 (rebuild/src/compiled/00/e2/NScriptCQSGuardianSisterInfoScriptCMazeAtTavern_Alloc_00e25c70.cpp), its user-declared destructor dropped. The Alloc body is not part of this graded body.
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
class CQS_GuardianSisterInfoScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    class CMazeAtTavern : public CEntityScriptBase {
    public:
        CQS_GuardianSisterInfoScript* ParentClass;      // +0x14
        CQ_SunnyvaleMasterData* PMasterData;            // +0x18
        long TalkCounter;                               // +0x1C
        bool WavedOver;                                 // +0x20
        long HitCount;                                  // +0x24
        CMazeAtTavern(CQS_GuardianSisterInfoScript& parent, CScriptDataBase* data, const CScriptThing& me)
            : CEntityScriptBase(*parent.Game, me), ParentClass(&parent)
        {
            PMasterData = (CQ_SunnyvaleMasterData*)data;
        }
        static CCountedPointer<CEntityScriptBase> __fastcall Alloc(CQS_GuardianSisterInfoScript& parent, CScriptDataBase* data, const CScriptThing& me);
    private:
    virtual void Main();                                 // PDB ?Main@<entity>@..@@EAEXXZ (slot 1)
public:
    virtual CScriptBase* GetParentScript() const;        // PDB ?GetParentScript@<entity>@..@@UBEPAVCScriptBase@@XZ (slot 3)
};
};

CCountedPointer<CEntityScriptBase> __fastcall CQS_GuardianSisterInfoScript::CMazeAtTavern::Alloc(CQS_GuardianSisterInfoScript& parent, CScriptDataBase* data, const CScriptThing& me)
{
    return CCountedPointer<CEntityScriptBase>(new CMazeAtTavern(parent, data, me));
}
}
