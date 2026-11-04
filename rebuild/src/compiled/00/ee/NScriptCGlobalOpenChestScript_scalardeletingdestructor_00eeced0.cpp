// NScript::CGlobal_OpenChestScript::`scalar deleting destructor'(unsigned int flags) -- the destructor (expanded), then free the object when bit 0 is set
// Leaf: ??_GCGlobal_OpenChestScript@NScript@@UAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00EECED0, 30 bytes, thiscall, `ret 4`; calls 00cbd510=CScriptBase::~CScriptBase ; 00bfe9bc=MSVCR71.DLL::operator_delete.
// Identity (naming wave 35, family sdd): CScriptInfoManager::RegisterScripts 00cd52d0 stores Alloc 00eecea0 at 00cda176 with name "Global_OpenChest"; Alloc news + stores script vtable 012f5760; PDB ?Alloc@CGlobal_OpenChestScript@NScript@@SIPAVCScriptBase@@; script vtable slot 0 (slot 1 pushes "Main" 0x012C2F94); PDB sdd; refs=1; was CVtable_012f5760::`scalar deleting destructor'.
//   ??_GCGlobal_OpenChestScript@NScript@@UAEPAXI@Z (rebuild/candidates/manual/naming/2026-11-04_anniversary/wave3/labels_alloc.tsv); PDB: FableWin.pdb ??_ECGlobal_OpenChestScript@NScript@@UAEPAXI@Z; FableWin.pdb ??1CGlobal_OpenChestScript@NScript@@UAE@XZ.
//   Was:  (catalog stand-in CVtable_012f5760, Leaf `vector_deleting_destructor').
// Shape: the deleting destructor is emitted with the vtable, i.e. in the TU that constructs the object (wave 33 ctor): the class model and
//   the PDB constructor ??0CGlobal_OpenChestScript@NScript@@QAE@AAVCGameScriptInterfaceBase@@@Z of the landed row 0x00EEC7A0 (rebuild/src/compiled/00/ee/NScriptCGlobalOpenChestScript_CGlobal_OpenChestScript_00eec7a0.cpp), its own source unchanged; its user-declared destructor dropped (retail expands the destructor into the ??_G). The constructor body is not part of this graded body.
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptBase.h"
class CGameScriptInterfaceBase;
namespace NScript {
class CGlobal_OpenChestScript : public CScriptBase {
public:
    CGlobal_OpenChestScript(CGameScriptInterfaceBase& game);
    CGameScriptInterfaceBase* Game;         // +0x40
};
CGlobal_OpenChestScript::CGlobal_OpenChestScript(CGameScriptInterfaceBase& game) : Game(&game)
{
}
}
