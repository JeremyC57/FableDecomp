// NScript::CGlobal_DebugCycleThroughSpeechScript::CGlobal_DebugCycleThroughSpeechScript(CGameScriptInterfaceBase& game) -- CScriptBase(), Game = &game, then the script vtable 0x012F54B4
// Leaf: ??0CGlobal_DebugCycleThroughSpeechScript@NScript@@QAE@AAVCGameScriptInterfaceBase@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00EE90F0, 27 bytes, thiscall, `ret 4`; calls CScriptBase::CScriptBase 0x00CB8110.
// Identity: CScriptInfoManager::RegisterScripts 00cd52d0 stores Alloc 00ee9450 at 00cd9ae3 with name "Global_DebugCycleThroughSpeech"; Alloc news + stores script vtable 012f54b4; PDB ?Alloc@CGlobal_DebugCycleThroughSpeechScript@NScript@@SIPAVCScriptBase@@; out-of-line copy of the ctor the Alloc inlines: base CScriptBase ctor 0x00CB8110 on ecx, stores the script vtable, ret 4 = PDB arity; PDB ??0CGlobal_DebugCycleThroughSpeechScript@NScript@@QAE@AAVCGameScriptInterfaceBase@@@Z; was CVtable_012f54b4::CVtable_012f54b4 (wave3/labels_alloc.tsv); PDB ??0CGlobal_DebugCycleThroughSpeechScript@NScript@@QAE@AAVCGameScriptInterfaceBase@@@Z in FableWin.pdb.
// Naming wave 35: was landed as the stand-in `??0CVtable_012f54b4@@QAE@PAX@Z` (00/ee/CVtable012f54b4_CVtable012f54b4_00ee90f0.cpp) on a CBase_00cb8110 base; same bytes.
// Model: engine/CScriptBase.h; a global script has no master data (PDB ctor takes only the interface): Game +0x40.
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptBase.h"
class CGameScriptInterfaceBase;
namespace NScript {
class CGlobal_DebugCycleThroughSpeechScript : public CScriptBase {
public:
    CGlobal_DebugCycleThroughSpeechScript(CGameScriptInterfaceBase& game);
    virtual ~CGlobal_DebugCycleThroughSpeechScript();    // ??1CGlobal_DebugCycleThroughSpeechScript@NScript@@UAE@XZ
    CGameScriptInterfaceBase* Game;         // +0x40
};
CGlobal_DebugCycleThroughSpeechScript::CGlobal_DebugCycleThroughSpeechScript(CGameScriptInterfaceBase& game) : Game(&game)
{
}
}
