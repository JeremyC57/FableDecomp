// NScript::CExpression_WaitScript::`scalar deleting destructor'(unsigned int flags) -- the destructor (expanded), then free the object when bit 0 is set
// Leaf: ??_GCExpression_WaitScript@NScript@@UAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00EEA770, 30 bytes, thiscall, `ret 4`; calls 00cbd510=CScriptBase::~CScriptBase ; 00bfe9bc=MSVCR71.DLL::operator_delete.
// Identity (naming wave 35, family sdd): Anniversary wave 2026-11-04 (high): vtable 0x012F55F8 slot 0; Anniversary RTTI NScript::CExpression_WaitScript (string-literal match, validated 257/258); PDB name set has the sdd member of NScript::CExpression_WaitScript; slot layout from named TLC siblings; function referenced by this vtable only.
//   ??_GCExpression_WaitScript@NScript@@UAEPAXI@Z (rebuild/candidates/manual/naming/2026-11-04_anniversary/labels_high.tsv); PDB: FableWin.pdb ??_GCExpression_WaitScript@NScript@@UAEPAXI@Z; FableWin.pdb ??1CExpression_WaitScript@NScript@@UAE@XZ.
//   Was:  (catalog stand-in CVtable_012f55f8, Leaf `vector_deleting_destructor').
// Shape: the deleting destructor is emitted with the vtable, i.e. in the TU that constructs the object (wave 33 ctor): the class model and
//   the PDB constructor ??0CExpression_WaitScript@NScript@@QAE@AAVCGameScriptInterfaceBase@@@Z of the landed row 0x00EEA430 (rebuild/src/compiled/00/ee/NScriptCExpressionWaitScript_CExpression_WaitScript_00eea430.cpp), its own source unchanged; its user-declared destructor dropped (retail expands the destructor into the ??_G). The constructor body is not part of this graded body.
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptBase.h"
class CGameScriptInterfaceBase;
namespace NScript {
class CExpression_WaitScript : public CScriptBase {
public:
    CExpression_WaitScript(CGameScriptInterfaceBase& game);
    CGameScriptInterfaceBase* Game;         // +0x40
};
CExpression_WaitScript::CExpression_WaitScript(CGameScriptInterfaceBase& game) : Game(&game)
{
}
}
