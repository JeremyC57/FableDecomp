// NScript::CV_GuildMasterScript::`scalar deleting destructor'(unsigned int flags) -- the destructor (expanded), then free the object when bit 0 is set
// Leaf: ??_GCV_GuildMasterScript@NScript@@UAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E92940, 40 bytes, thiscall, `ret 4`; calls 0099eae0=CCharString::~CCharString ; 00cbd510=CScriptBase::~CScriptBase ; 00bfe9bc=MSVCR71.DLL::operator_delete.
// Identity (naming wave 35, family sdd): outer script vtable 012ec138 slot 0 (slot 1 RegisterMain pushes "Main" 0x012C2F94); enclosing 00e90830 (slot 2) registers 1 entity scripts whose PDB Alloc outer is CV_GuildMasterScript; PDB ??_G/??1CV_GuildMasterScript@NScript@@; refs=1; was CVtable_012ec138::`scalar deleting destructor'.
//   ??_GCV_GuildMasterScript@NScript@@UAEPAXI@Z (rebuild/candidates/manual/naming/2026-11-04_anniversary/wave3/labels_alloc.tsv); PDB: FableWin.pdb ??_GCV_GuildMasterScript@NScript@@UAEPAXI@Z; Ego_r.pdb ??_ECV_GuildMasterScript@NScript@@UAEPAXI@Z; FableWin.pdb ??1CV_GuildMasterScript@NScript@@UAE@XZ.
//   Was:  (catalog stand-in CVtable_012ec138, Leaf `scalar_deleting_destructor').
// Shape: the deleting destructor is emitted with the vtable, i.e. in the TU that constructs the object (wave 33 ctor): the class model and
//   the PDB constructor ??0CV_GuildMasterScript@NScript@@QAE@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z of the landed row 0x00E90750 (rebuild/src/compiled/00/e9/NScriptCVGuildMasterScript_CV_GuildMasterScript_00e90750.cpp), its own source unchanged; its user-declared destructor dropped (retail expands the destructor into the ??_G). The constructor body is not part of this graded body.
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
class CV_GuildMasterScript : public CScriptObject {
public:       // vtable 0x012EC138
    unsigned char _pad[0x50 - 0x48];        // +0x48..+0x50
    CCharString   m_name;                   // +0x50
    virtual void v00();
    CV_GuildMasterScript(CScriptDataBase* data, CGameScriptInterfaceBase& game);
};
}
using NScript::CV_GuildMasterScript;

NScript::CV_GuildMasterScript::CV_GuildMasterScript(CScriptDataBase* data, CGameScriptInterfaceBase& game)
    : CScriptObject((void*)data, (void*)&game)
    , m_name()
{
}