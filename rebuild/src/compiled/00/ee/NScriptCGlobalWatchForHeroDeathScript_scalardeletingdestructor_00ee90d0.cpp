// NScript::CGlobal_WatchForHeroDeathScript::`scalar deleting destructor'(unsigned int flags) -- the destructor (expanded), then free the object when bit 0 is set
// Leaf: ??_GCGlobal_WatchForHeroDeathScript@NScript@@UAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00EE90D0, 30 bytes, thiscall, `ret 4`; calls 00cbd510=CScriptBase::~CScriptBase ; 00bfe9bc=MSVCR71.DLL::operator_delete.
// Identity (naming wave 35, family sdd): CScriptInfoManager::RegisterScripts 00cd52d0 stores Alloc 00ee90a0 at 00cd9a4a with name "Global_WatchForHeroDeath"; Alloc news + stores script vtable 012f5470; PDB ?Alloc@CGlobal_WatchForHeroDeathScript@NScript@@SIPAVCScriptBase@@; script vtable slot 0 (slot 1 pushes "Main" 0x012C2F94); PDB sdd; refs=1; was CVtable_012f5470::`scalar deleting destructor'.
//   ??_GCGlobal_WatchForHeroDeathScript@NScript@@UAEPAXI@Z (rebuild/candidates/manual/naming/2026-11-04_anniversary/wave3/labels_alloc.tsv); PDB: FableWin.pdb ??_GCGlobal_WatchForHeroDeathScript@NScript@@UAEPAXI@Z; FableWin.pdb ??1CGlobal_WatchForHeroDeathScript@NScript@@UAE@XZ.
//   Was:  (catalog stand-in CVtable_012f5470, Leaf `vector_deleting_destructor').
// Shape: the deleting destructor is emitted with the vtable, i.e. in the TU that constructs the object (wave 33 ctor): the class model and
//   the PDB constructor ??0CGlobal_WatchForHeroDeathScript@NScript@@QAE@AAVCGameScriptInterfaceBase@@@Z of the landed row 0x00EE8E80 (rebuild/src/compiled/00/ee/NScriptCGlobalWatchForHeroDeathScript_CGlobal_WatchForHeroDeathScript_00ee8e80.cpp), its own source unchanged; its user-declared destructor dropped (retail expands the destructor into the ??_G). The constructor body is not part of this graded body.
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptBase.h"
class CGameScriptInterfaceBase;
namespace NScript {
class CGlobal_WatchForHeroDeathScript : public CScriptBase {
public:
    CGlobal_WatchForHeroDeathScript(CGameScriptInterfaceBase& game);
    CGameScriptInterfaceBase* Game;         // +0x40
};
CGlobal_WatchForHeroDeathScript::CGlobal_WatchForHeroDeathScript(CGameScriptInterfaceBase& game) : Game(&game)
{
}
}
