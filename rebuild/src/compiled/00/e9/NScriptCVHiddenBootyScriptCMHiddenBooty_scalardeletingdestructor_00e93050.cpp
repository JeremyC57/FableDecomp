// NScript::CV_HiddenBootyScript::CM_HiddenBooty::`scalar deleting destructor'(unsigned int flags) -- the implicit ~CM_HiddenBooty (its members' destructors, then ~CEntityScriptBase), then free the entity when bit 0 is set
// Leaf: ??_GCM_HiddenBooty@CV_HiddenBootyScript@NScript@@UAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E93050, 40 bytes, thiscall, `ret 4`; calls 0099eae0=CCharString::~CCharString ; 00f35b40=CEntityScriptBase::~CEntityScriptBase ; 00bfe9bc=MSVCR71.DLL::operator_delete.
// Naming wave 33 (family esb): slot 0 of vtable 0x012ECAE8 (no RTTI: NScript TU), the vtable the landed Alloc 0x00E92F70 stores
//   (?Alloc@CM_HiddenBooty@CV_HiddenBootyScript@NScript@@SI?AV?$CCountedPointer@VCEntityScriptBase@@@@AAV23@PAVCScriptDataBase@@ABVCScriptThing@@@Z, Ego_r.pdb / FableWin.pdb); Ego_r.pdb ??_GCM_HiddenBooty@CV_HiddenBootyScript@NScript@@UAEPAXI@Z; FableWin.pdb ??_GCM_HiddenBooty@CV_HiddenBootyScript@NScript@@UAEPAXI@Z; FableWin.pdb ??1CM_HiddenBooty@CV_HiddenBootyScript@NScript@@UAE@XZ.
//   Ego_r.pdb (release) has no out-of-line ??1 for the entity, and retail calls ~CEntityScriptBase directly: the entity destructor is implicit.
//   Was a free __fastcall CLandscapeBackgroundPatch_vector_deleting_destructor stand-in (catalog CVtable_012ecae8).
// Shape: the deleting destructor is emitted with the vtable, i.e. in the TU that constructs the entity: the class model and the PDB Alloc
//   of the landed row 0x00E92F70 (rebuild/src/compiled/00/e9/NScriptCVHiddenBootyScriptCMHiddenBooty_Alloc_00e92f70.cpp), its user-declared destructor dropped. The Alloc body is not part of this graded body.
class CGameScriptInterfaceBase;
class CScriptDataBase;

// 0x00CDEE00 is CCountedPointer<T>::DeleteData (engine/CCountedPointer.h; reloc_bindings binds both spellings)

#include "engine/CCountedPointer.h"


// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
class CCharString {                                     // 4 bytes
public:
    void* PStringData;
    CCharString();                                      // 0x0099E4B0
    ~CCharString();
};

// CEntityScriptBase: the retail model from engine/CEntityScriptBase.h (vtable 0x012C3224, 7 slots, 0x14: Game +0x04, Me +0x08).
#include "engine/CEntityScriptBase.h"

// CScriptBase / CLevelScriptBase<T>: the retail NScript model from the shared headers (FABLE_RETAIL_NSCRIPT: CScriptBase :
//   CBaseClassNonCopyable : CBaseClass, vtable 0x012C1648, 5 slots, 0x40; CLevelScriptBase<T> Game +0x40, MasterScriptData +0x44).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CLevelScriptBase.h"

namespace NScript {
class CQ_SunnyvaleMasterData;
class CV_HiddenBootyScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    class CM_HiddenBooty : public CEntityScriptBase {
    public:
        CV_HiddenBootyScript* ParentClass;              // +0x14
        CQ_SunnyvaleMasterData* PMasterData;            // +0x18
        bool HasHeroGotAllClues;                        // +0x1C
        bool HasHeroGotAnyClues;                        // +0x1D
        bool PlayedIntroduction;                        // +0x1E
        CCharString ThingFromWhichHeroMightBeRewardedFrom;// +0x20
        CM_HiddenBooty(CV_HiddenBootyScript& parent, CScriptDataBase* data, const CScriptThing& me)
            : CEntityScriptBase(*parent.Game, me), ParentClass(&parent)
        {
            PMasterData = (CQ_SunnyvaleMasterData*)data;
        }
        static CCountedPointer<CEntityScriptBase> __fastcall Alloc(CV_HiddenBootyScript& parent, CScriptDataBase* data, const CScriptThing& me);
    private:
    virtual void Main();                                 // PDB ?Main@<entity>@..@@EAEXXZ (slot 1)
public:
    virtual CScriptBase* GetParentScript() const;        // PDB ?GetParentScript@<entity>@..@@UBEPAVCScriptBase@@XZ (slot 3)
};
};

CCountedPointer<CEntityScriptBase> __fastcall CV_HiddenBootyScript::CM_HiddenBooty::Alloc(CV_HiddenBootyScript& parent, CScriptDataBase* data, const CScriptThing& me)
{
    return CCountedPointer<CEntityScriptBase>(new CM_HiddenBooty(parent, data, me));
}
}