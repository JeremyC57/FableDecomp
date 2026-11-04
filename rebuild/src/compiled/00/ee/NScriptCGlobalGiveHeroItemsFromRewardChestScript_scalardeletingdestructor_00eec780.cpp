// NScript::CGlobal_GiveHeroItemsFromRewardChestScript::`scalar deleting destructor'(unsigned int flags) -- the destructor (expanded), then free the object when bit 0 is set
// Leaf: ??_GCGlobal_GiveHeroItemsFromRewardChestScript@NScript@@UAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00EEC780, 30 bytes, thiscall, `ret 4`; calls 00cbd510=CScriptBase::~CScriptBase ; 00bfe9bc=MSVCR71.DLL::operator_delete.
// Identity (naming wave 35, family sdd): CScriptInfoManager::RegisterScripts 00cd52d0 stores Alloc 00eec750 at 00cda0dd with name "Global_GiveHeroItemsFromRewardChest"; Alloc news + stores script vtable 012f5730; PDB ?Alloc@CGlobal_GiveHeroItemsFromRewardChestScript@NScript@@SIPAVCScriptBase@@; script vtable slot 0 (slot 1 pushes "Main" 0x012C2F94); PDB sdd; refs=1; was CVtable_012f5730::`scalar deleting destructor'.
//   ??_GCGlobal_GiveHeroItemsFromRewardChestScript@NScript@@UAEPAXI@Z (rebuild/candidates/manual/naming/2026-11-04_anniversary/wave3/labels_alloc.tsv); PDB: FableWin.pdb ??_ECGlobal_GiveHeroItemsFromRewardChestScript@NScript@@UAEPAXI@Z; FableWin.pdb ??1CGlobal_GiveHeroItemsFromRewardChestScript@NScript@@UAE@XZ.
//   Was:  (catalog stand-in CVtable_012f5730, Leaf `vector_deleting_destructor').
// Shape: the deleting destructor is emitted with the vtable, i.e. in the TU that constructs the object (wave 33 ctor): the class model and
//   the PDB constructor ??0CGlobal_GiveHeroItemsFromRewardChestScript@NScript@@QAE@AAVCGameScriptInterfaceBase@@@Z of the landed row 0x00EEC3E0 (rebuild/src/compiled/00/ee/NScriptCGlobalGiveHeroItemsFromRewardChestScript_CGlobal_GiveHeroItemsFromRewardChestScript_00eec3e0.cpp), its own source unchanged; its user-declared destructor dropped (retail expands the destructor into the ??_G). The constructor body is not part of this graded body.
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptBase.h"
class CGameScriptInterfaceBase;
namespace NScript {
class CGlobal_GiveHeroItemsFromRewardChestScript : public CScriptBase {
public:
    CGlobal_GiveHeroItemsFromRewardChestScript(CGameScriptInterfaceBase& game);
    CGameScriptInterfaceBase* Game;         // +0x40
};
CGlobal_GiveHeroItemsFromRewardChestScript::CGlobal_GiveHeroItemsFromRewardChestScript(CGameScriptInterfaceBase& game) : Game(&game)
{
}
}
