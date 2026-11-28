#ifndef FABLE_ENGINE_CLEVELSCRIPTNOMASTERBASE_H
#define FABLE_ENGINE_CLEVELSCRIPTNOMASTERBASE_H
// NEW header (agentFAM08). CLevelScriptNoMasterBase : CScriptBase -- the base of the scripts that keep no master data (the
// CGlobal_* / CExpression_* scripts): FableWin.pdb ??0CLevelScriptNoMasterBase@@QAE@AAVCGameScriptInterfaceBase@@@Z,
// ??1CLevelScriptNoMasterBase@@UAE@XZ (implicit). Ego_r.pdb record: sizeof 0x58 = CScriptBase 0x54 + CGameScriptInterfaceBase* Game
// +0x54; retail CScriptBase is 0x40, so Game +0x40 and sizeof 0x44 (the derived CGlobal_ToggleTimeDisplayScript::Alloc 0x00EED060
// stores its Game argument at +0x40 after CScriptBase(), its own TimeDisplayActive following).
// Retail constructor 0x00CDD2F0 stores vtable 0x012C2F50 (no RTTI): slot 0 the deleting destructor 0x00CDD310 (calls ~CScriptBase:
// the destructor is implicit), slots 1..4 CScriptBase's RegisterMain / Main / Init / OnPersist (0x00CBD4B0 .. 0x00CBD4E0).
// Needs FABLE_RETAIL_NSCRIPT (engine/CScriptBase.h).
#include "engine/CScriptBase.h"
class CGameScriptInterfaceBase;
class CLevelScriptNoMasterBase : public CScriptBase {
public:
    CLevelScriptNoMasterBase(CGameScriptInterfaceBase& game);
    CGameScriptInterfaceBase* Game;                                      // +0x40
};
FABLE_STATIC_ASSERT(sizeof(CLevelScriptNoMasterBase) == 0x44);
#endif
