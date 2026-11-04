// NScript::CHeroBoastsScript::`scalar deleting destructor'(unsigned int flags) -- the destructor (expanded), then free the object when bit 0 is set
// Leaf: ??_GCHeroBoastsScript@NScript@@UAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00CE6C80, 40 bytes, thiscall, `ret 4`; calls 0099eae0=CCharString::~CCharString ; 00cbd510=CScriptBase::~CScriptBase ; 00bfe9bc=MSVCR71.DLL::operator_delete.
// Identity (naming wave 35, family sdd): CScriptInfoManager::RegisterScripts 00cd52d0 stores Alloc 00ce6c40 at 00cd53df with name "HeroBoasts"; Alloc news + stores script vtable 012c3688; PDB ?Alloc@CHeroBoastsScript@NScript@@SIPAVCScriptBase@@; script vtable slot 0 (slot 1 pushes "Main" 0x012C2F94); PDB sdd; refs=1; was CVtable_012c3688::`scalar deleting destructor'.
//   ??_GCHeroBoastsScript@NScript@@UAEPAXI@Z (rebuild/candidates/manual/naming/2026-11-04_anniversary/wave3/labels_alloc.tsv); PDB: Ego_r.pdb / FableWin.pdb ??_ECHeroBoastsScript@NScript@@UAEPAXI@Z; FableWin.pdb ??1CHeroBoastsScript@NScript@@UAE@XZ.
//   Was:  (catalog stand-in CVtable_012c3688, Leaf `scalar_deleting_destructor').
// Shape: the deleting destructor is emitted with the vtable, i.e. in the TU that constructs the object (wave 33 ctor): the class model and
//   the PDB constructor ??0CHeroBoastsScript@NScript@@QAE@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z of the landed row 0x00CE1A00 (rebuild/src/compiled/00/ce/NScriptCHeroBoastsScript_CHeroBoastsScript_00ce1a00.cpp), its own source unchanged; its user-declared destructor dropped (retail expands the destructor into the ??_G). The constructor body is not part of this graded body.
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
class CHeroBoastsScript : public CScriptObject {
public:       // vtable 0x012C3688
    CCharString   m_name;                   // +0x48
    virtual void v00();
    CHeroBoastsScript(CScriptDataBase* data, CGameScriptInterfaceBase& game);
};
}
using NScript::CHeroBoastsScript;

NScript::CHeroBoastsScript::CHeroBoastsScript(CScriptDataBase* data, CGameScriptInterfaceBase& game)
    : CScriptObject((void*)data, (void*)&game)
    , m_name()
{
}