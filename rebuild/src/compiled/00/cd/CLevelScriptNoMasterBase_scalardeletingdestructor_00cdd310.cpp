// CLevelScriptNoMasterBase::`scalar deleting destructor'(unsigned int flags) -- the implicit destructor (~CScriptBase out of line),
// then free this when bit 0 of flags is set; returns this
// Leaf: ??_GCLevelScriptNoMasterBase@@UAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CDD310, 30 bytes, thiscall, `ret 4`.
// Identity: slot 0 of vtable 0x012C2F50, the vtable CLevelScriptNoMasterBase::CLevelScriptNoMasterBase 0x00CDD2F0 stores (FableWin.pdb
//   ??0CLevelScriptNoMasterBase@@QAE@AAVCGameScriptInterfaceBase@@@Z; see engine/CLevelScriptNoMasterBase.h); it calls
//   CScriptBase::~CScriptBase 0x00CBD510 with no vptr store of its own (the implicit destructor). FableWin.pdb spells the debug
//   build's slot ??_E; the retail body tests bit 0 only (scalar form).
// Shape: `new` of the class with the constructor defined inline is this TU's emitter of the vtable and so of ??_G (descriptive
//   emitter name, not graded).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CLevelScriptNoMasterBase.h"

inline CLevelScriptNoMasterBase::CLevelScriptNoMasterBase(CGameScriptInterfaceBase& game)
    : Game(&game)
{
}
CLevelScriptNoMasterBase* NewLevelScriptNoMasterBase(CGameScriptInterfaceBase& game)
{
    return new CLevelScriptNoMasterBase(game);
}
