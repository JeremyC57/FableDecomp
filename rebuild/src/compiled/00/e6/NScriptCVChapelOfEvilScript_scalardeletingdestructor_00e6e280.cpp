// NScript::CV_ChapelOfEvilScript::`scalar deleting destructor'(unsigned int flags) -- the destructor (expanded), then free the object when bit 0 is set
// Leaf: ??_GCV_ChapelOfEvilScript@NScript@@UAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E6E280, 43 bytes, thiscall, `ret 4`; calls 0099eae0=CCharString::~CCharString ; 00cbd510=CScriptBase::~CScriptBase ; 00bfe9bc=MSVCR71.DLL::operator_delete.
// Identity (naming wave 35, family sdd): outer script vtable 012e8080 slot 0 (slot 1 RegisterMain pushes "Main" 0x012C2F94); enclosing 00e693f0 (slot 2) registers 3 entity scripts whose PDB Alloc outer is CV_ChapelOfEvilScript; PDB ??_G/??1CV_ChapelOfEvilScript@NScript@@; refs=1; was CVtable_012e8080::`scalar deleting destructor'.
//   ??_GCV_ChapelOfEvilScript@NScript@@UAEPAXI@Z (rebuild/candidates/manual/naming/2026-11-04_anniversary/wave3/labels_alloc.tsv); PDB: Ego_r.pdb / FableWin.pdb ??_GCV_ChapelOfEvilScript@NScript@@UAEPAXI@Z; FableWin.pdb ??1CV_ChapelOfEvilScript@NScript@@UAE@XZ.
//   Was:  (catalog stand-in CVtable_012e8080, Leaf `scalar_deleting_destructor').
// Shape: the deleting destructor is emitted with the vtable, i.e. in the TU that constructs the object (wave 33 ctor): the class model and
//   the PDB constructor ??0CV_ChapelOfEvilScript@NScript@@QAE@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z of the landed row 0x00E68F70 (rebuild/src/compiled/00/e6/NScriptCVChapelOfEvilScript_CV_ChapelOfEvilScript_00e68f70.cpp), its own source unchanged; its user-declared destructor dropped (retail expands the destructor into the ??_G). The constructor body is not part of this graded body.
class CScriptDataBase;
class CGameScriptInterfaceBase;
#include "fable_string.h"

// CScriptBase: the retail NScript model from the shared headers (FABLE_RETAIL_NSCRIPT: CScriptBase :
//   CBaseClassNonCopyable : CBaseClass, vtable 0x012C1648, 5 slots, 0x40).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptBase.h"

struct __declspec(novtable) CScriptObject : CScriptBase {   // abstract: no vtable store of its own
    void* m_pThing;                         // +0x40
    void* m_pOwner;                         // +0x44
    CScriptObject(void* owner, void* thing) : CScriptBase(), m_pThing(thing), m_pOwner(owner) { }
};

namespace NScript {
class CV_ChapelOfEvilScript : public CScriptObject {
public:       // vtable 0x012E8080
    unsigned char _pad[0x88 - 0x48];        // +0x48..+0x88
    CCharString   m_name;                   // +0x88
    virtual void v00();
    CV_ChapelOfEvilScript(CScriptDataBase* data, CGameScriptInterfaceBase& game);
};
}
using NScript::CV_ChapelOfEvilScript;

NScript::CV_ChapelOfEvilScript::CV_ChapelOfEvilScript(CScriptDataBase* data, CGameScriptInterfaceBase& game)
    : CScriptObject((void*)data, (void*)&game)
    , m_name()
{
}