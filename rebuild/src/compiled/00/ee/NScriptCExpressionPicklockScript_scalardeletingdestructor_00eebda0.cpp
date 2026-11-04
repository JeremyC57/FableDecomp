// NScript::CExpression_PicklockScript::`scalar deleting destructor'(unsigned int flags) -- the destructor (expanded), then free the object when bit 0 is set
// Leaf: ??_GCExpression_PicklockScript@NScript@@UAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00EEBDA0, 30 bytes, thiscall, `ret 4`; calls 00cbd510=CScriptBase::~CScriptBase ; 00bfe9bc=MSVCR71.DLL::operator_delete.
// Identity (naming wave 35, family sdd): CScriptInfoManager::RegisterScripts 00cd52d0 stores Alloc 00eebd70 at 00cd9e79 with name "Expression_Picklock"; Alloc news + stores script vtable 012f56d0; PDB ?Alloc@CExpression_PicklockScript@NScript@@SIPAVCScriptBase@@; script vtable slot 0 (slot 1 pushes "Main" 0x012C2F94); PDB sdd; refs=1; was CVtable_012f56d0::`scalar deleting destructor'.
//   ??_GCExpression_PicklockScript@NScript@@UAEPAXI@Z (rebuild/candidates/manual/naming/2026-11-04_anniversary/wave3/labels_alloc.tsv); PDB: FableWin.pdb ??_GCExpression_PicklockScript@NScript@@UAEPAXI@Z; FableWin.pdb ??1CExpression_PicklockScript@NScript@@UAE@XZ.
//   Was:  (catalog stand-in CVtable_012f56d0, Leaf `vector_deleting_destructor').
// Shape: the deleting destructor is emitted with the vtable, i.e. in the TU that constructs the object (wave 33 ctor): the class model and
//   the PDB constructor ??0CExpression_PicklockScript@NScript@@QAE@AAVCGameScriptInterfaceBase@@@Z of the landed row 0x00EEB780 (rebuild/src/compiled/00/ee/NScriptCExpressionPicklockScript_CExpression_PicklockScript_00eeb780.cpp), its own source unchanged; its user-declared destructor dropped (retail expands the destructor into the ??_G). The constructor body is not part of this graded body.
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptBase.h"
class CGameScriptInterfaceBase;
namespace NScript {
class CExpression_PicklockScript : public CScriptBase {
public:
    CExpression_PicklockScript(CGameScriptInterfaceBase& game);
    CGameScriptInterfaceBase* Game;         // +0x40
};
CExpression_PicklockScript::CExpression_PicklockScript(CGameScriptInterfaceBase& game) : Game(&game)
{
}
}
