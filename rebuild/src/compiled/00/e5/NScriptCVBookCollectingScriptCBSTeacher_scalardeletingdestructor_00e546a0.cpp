// NScript::CV_BookCollectingScript::CBS_Teacher::`scalar deleting destructor'(unsigned int flags) -- the implicit ~CBS_Teacher (its members' destructors, then ~CEntityScriptBase), then free the entity when bit 0 is set
// Leaf: ??_GCBS_Teacher@CV_BookCollectingScript@NScript@@UAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E546A0, 40 bytes, thiscall, `ret 4`; calls 007e74d0=CScriptGameResourceObjectScriptedThingBase::~CScriptGameResourceObjectScriptedThingBase ; 00f35b40=CEntityScriptBase::~CEntityScriptBase ; 00bfe9bc=MSVCR71.DLL::operator_delete.
// Naming wave 33 (family esb): slot 0 of vtable 0x012E5F40 (no RTTI: NScript TU), the vtable the landed Alloc 0x00E54580 stores
//   (?Alloc@CBS_Teacher@CV_BookCollectingScript@NScript@@SI?AV?$CCountedPointer@VCEntityScriptBase@@@@AAV23@PAVCScriptDataBase@@ABVCScriptThing@@@Z, Ego_r.pdb / FableWin.pdb); FableWin.pdb ??_GCBS_Teacher@CV_BookCollectingScript@NScript@@UAEPAXI@Z; Ego_r.pdb ??_ECBS_Teacher@CV_BookCollectingScript@NScript@@UAEPAXI@Z; Ego_r.pdb ??1CBS_Teacher@CV_BookCollectingScript@NScript@@UAE@XZ; FableWin.pdb ??1CBS_Teacher@CV_BookCollectingScript@NScript@@UAE@XZ.
//   Ego_r.pdb (release) has no out-of-line ??1 for the entity, and retail calls ~CEntityScriptBase directly: the entity destructor is implicit.
//   Was a free __fastcall CLandscapeBackgroundPatch_vector_deleting_destructor stand-in (catalog CVtable_012e5f40).
// Shape: the deleting destructor is emitted with the vtable, i.e. in the TU that constructs the entity: the class model and the PDB Alloc
//   of the landed row 0x00E54580 (rebuild/src/compiled/00/e5/NScriptCVBookCollectingScriptCBSTeacher_Alloc_00e54580.cpp), its user-declared destructor dropped. The Alloc body is not part of this graded body.
class CGameScriptInterfaceBase;
class CScriptDataBase;

// 0x00CDEE00 is CCountedPointer<T>::DeleteData (engine/CCountedPointer.h; reloc_bindings binds both spellings)

#include "engine/CCountedPointer.h"


// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
class CScriptGameResourceObjectScriptedThingBase {      // 16 bytes (Ego_r): vptr, +4, PImp +8
public:
    virtual ~CScriptGameResourceObjectScriptedThingBase();
    unsigned char _base04[4];
    CCountedPointer<CScriptGameResourceObjectScriptedThingBase> PImp;   // +0x08
    CScriptGameResourceObjectScriptedThingBase();       // 0x007E72A0
};

// CEntityScriptBase: the retail model from engine/CEntityScriptBase.h (vtable 0x012C3224, 7 slots, 0x14: Game +0x04, Me +0x08).
#include "engine/CEntityScriptBase.h"

// CScriptBase / CLevelScriptBase<T>: the retail NScript model from the shared headers (FABLE_RETAIL_NSCRIPT: CScriptBase :
//   CBaseClassNonCopyable : CBaseClass, vtable 0x012C1648, 5 slots, 0x40; CLevelScriptBase<T> Game +0x40, MasterScriptData +0x44).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CLevelScriptBase.h"

namespace NScript {
class CQ_SunnyvaleMasterData;
class CV_BookCollectingScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    class CBS_Teacher : public CEntityScriptBase {
    public:
        CV_BookCollectingScript* ParentClass;           // +0x14
        CQ_SunnyvaleMasterData* PMasterData;            // +0x18
        long BadBooksForHat;                            // +0x1C
        long GoodBooksForHat;                           // +0x20
        long BooksWanted;                               // +0x24
        long BooksAccepted;                             // +0x28
        long BooksComment;                              // +0x2C
        long MoralityReward;                            // +0x30
        CScriptGameResourceObjectScriptedThingBase seh_me;// +0x34
        CBS_Teacher(CV_BookCollectingScript& parent, CScriptDataBase* data, const CScriptThing& me)
            : CEntityScriptBase(*parent.Game, me), ParentClass(&parent)
        {
            PMasterData = (CQ_SunnyvaleMasterData*)data;
        }
        static CCountedPointer<CEntityScriptBase> __fastcall Alloc(CV_BookCollectingScript& parent, CScriptDataBase* data, const CScriptThing& me);
    private:
    virtual void Main();                                 // PDB ?Main@<entity>@..@@EAEXXZ (slot 1)
public:
    virtual CScriptBase* GetParentScript() const;        // PDB ?GetParentScript@<entity>@..@@UBEPAVCScriptBase@@XZ (slot 3)
};
};

CCountedPointer<CEntityScriptBase> __fastcall CV_BookCollectingScript::CBS_Teacher::Alloc(CV_BookCollectingScript& parent, CScriptDataBase* data, const CScriptThing& me)
{
    return CCountedPointer<CEntityScriptBase>(new CBS_Teacher(parent, data, me));
}
}