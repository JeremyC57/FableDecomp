// NScript::CExpression_FollowScript::`scalar deleting destructor'(unsigned int flags) -- the destructor (expanded), then free the object when bit 0 is set
// Leaf: ??_GCExpression_FollowScript@NScript@@UAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00EEA410, 30 bytes, thiscall, `ret 4`; calls 00cbd510=CScriptBase::~CScriptBase ; 00bfe9bc=MSVCR71.DLL::operator_delete.
// Identity (naming wave 35, family sdd): CScriptInfoManager::RegisterScripts 00cd52d0 stores Alloc 00eea3e0 at 00cd9c15 with name "Expression_Follow"; Alloc news + stores script vtable 012f55b8; PDB ?Alloc@CExpression_FollowScript@NScript@@SIPAVCScriptBase@@; script vtable slot 0 (slot 1 pushes "Main" 0x012C2F94); PDB sdd; refs=1; was CVtable_012f55b8::`scalar deleting destructor'.
//   ??_GCExpression_FollowScript@NScript@@UAEPAXI@Z (rebuild/candidates/manual/naming/2026-11-04_anniversary/wave3/labels_alloc.tsv); PDB: FableWin.pdb ??_GCExpression_FollowScript@NScript@@UAEPAXI@Z; FableWin.pdb ??1CExpression_FollowScript@NScript@@UAE@XZ.
//   Was:  (catalog stand-in CVtable_012f55b8, Leaf `vector_deleting_destructor').
// Shape: the deleting destructor is emitted with the vtable, i.e. in the TU that constructs the object (wave 33 ctor): the class model and
//   the PDB constructor ??0CExpression_FollowScript@NScript@@QAE@AAVCGameScriptInterfaceBase@@@Z of the landed row 0x00EE9F50 (rebuild/src/compiled/00/ee/NScriptCExpressionFollowScript_CExpression_FollowScript_00ee9f50.cpp), its own source unchanged; its user-declared destructor dropped (retail expands the destructor into the ??_G). The constructor body is not part of this graded body.
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptBase.h"
class CGameScriptInterfaceBase;
namespace NScript {
class CExpression_FollowScript : public CScriptBase {
public:
    CExpression_FollowScript(CGameScriptInterfaceBase& game);
    CGameScriptInterfaceBase* Game;         // +0x40
};
CExpression_FollowScript::CExpression_FollowScript(CGameScriptInterfaceBase& game) : Game(&game)
{
}
}
