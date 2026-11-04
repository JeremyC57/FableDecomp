// NScript::CGlobal_ToggleTimeDisplayScript::`scalar deleting destructor'(unsigned int flags) -- the destructor (expanded), then free the object when bit 0 is set
// Leaf: ??_GCGlobal_ToggleTimeDisplayScript@NScript@@UAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00EED090, 30 bytes, thiscall, `ret 4`; calls 00cbd510=CScriptBase::~CScriptBase ; 00bfe9bc=MSVCR71.DLL::operator_delete.
// Identity (naming wave 35, family sdd): CScriptInfoManager::RegisterScripts 00cd52d0 stores Alloc 00eed060 at 00cda044 with name "Global_ToggleTimeDisplay"; Alloc news + stores script vtable 012f57a0; PDB ?Alloc@CGlobal_ToggleTimeDisplayScript@NScript@@SIPAVCScriptBase@@; script vtable slot 0 (slot 1 pushes "Main" 0x012C2F94); PDB sdd; refs=1; was CVtable_012f57a0::`scalar deleting destructor'.
//   ??_GCGlobal_ToggleTimeDisplayScript@NScript@@UAEPAXI@Z (rebuild/candidates/manual/naming/2026-11-04_anniversary/wave3/labels_alloc.tsv); PDB: FableWin.pdb ??_GCGlobal_ToggleTimeDisplayScript@NScript@@UAEPAXI@Z; FableWin.pdb ??1CGlobal_ToggleTimeDisplayScript@NScript@@UAE@XZ.
//   Was:  (catalog stand-in CVtable_012f57a0, Leaf `vector_deleting_destructor').
// Shape: the deleting destructor is emitted with the vtable, i.e. in the TU that constructs the object (wave 33 ctor): the class model and
//   the PDB constructor ??0CGlobal_ToggleTimeDisplayScript@NScript@@QAE@AAVCGameScriptInterfaceBase@@@Z of the landed row 0x00EECEF0 (rebuild/src/compiled/00/ee/NScriptCGlobalToggleTimeDisplayScript_CGlobal_ToggleTimeDisplayScript_00eecef0.cpp), its own source unchanged; its user-declared destructor dropped (retail expands the destructor into the ??_G). The constructor body is not part of this graded body.
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptBase.h"
class CGameScriptInterfaceBase;
namespace NScript {
class CGlobal_ToggleTimeDisplayScript : public CScriptBase {
public:
    CGlobal_ToggleTimeDisplayScript(CGameScriptInterfaceBase& game);
    CGameScriptInterfaceBase* Game;         // +0x40
};
CGlobal_ToggleTimeDisplayScript::CGlobal_ToggleTimeDisplayScript(CGameScriptInterfaceBase& game) : Game(&game)
{
}
}
