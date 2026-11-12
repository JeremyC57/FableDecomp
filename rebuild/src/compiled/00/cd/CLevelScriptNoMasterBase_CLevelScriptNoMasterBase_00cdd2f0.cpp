// CLevelScriptNoMasterBase::CLevelScriptNoMasterBase(CGameScriptInterfaceBase& game) -- CScriptBase(), Game = &game (+0x40), then
// this class's vtable 0x012C2F50
// Leaf: ??0CLevelScriptNoMasterBase@@QAE@AAVCGameScriptInterfaceBase@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CDD2F0, length 27 bytes, `ret 4`; unreferenced (an /OPT:NOREF-kept COMDAT: every
//   derived script inlines it, e.g. CGlobal_ToggleTimeDisplayScript::Alloc 0x00EED060 `call CScriptBase ; mov [esi+40h],edi`).
// Identity: FableWin.pdb ??0CLevelScriptNoMasterBase@@QAE@AAVCGameScriptInterfaceBase@@@Z is the one CScriptBase-derived
//   constructor taking only the game interface whose class adds nothing but Game (Ego_r.pdb record: CGameScriptInterfaceBase* Game
//   right after CScriptBase); its vtable 0x012C2F50 keeps CScriptBase's empty RegisterMain / Main / Init / OnPersist (it is not
//   abstract), and its deleting destructor 0x00CDD310 calls ~CScriptBase directly (implicit destructor, FableWin ??1 UAE).
// Callee: CScriptBase::CScriptBase() 0x00CB8110.
#define FABLE_RETAIL_NSCRIPT
#include "engine/CLevelScriptNoMasterBase.h"

CLevelScriptNoMasterBase::CLevelScriptNoMasterBase(CGameScriptInterfaceBase& game)
    : Game(&game)
{
}
