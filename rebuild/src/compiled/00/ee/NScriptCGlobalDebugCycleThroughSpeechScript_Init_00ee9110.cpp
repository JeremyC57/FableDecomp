// NScript::CGlobal_DebugCycleThroughSpeechScript::Init() -- empty: retail is a lone `ret`
// Leaf: ?Init@CGlobal_DebugCycleThroughSpeechScript@NScript@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00EE9110, 1 bytes, thiscall, plain `ret`.
// Identity: CScriptInfoManager::RegisterScripts 00cd52d0 stores Alloc 00ee9450 at 00cd9ae3 with name "Global_DebugCycleThroughSpeech"; Alloc news + stores script vtable 012f54b4; PDB ?Alloc@CGlobal_DebugCycleThroughSpeechScript@NScript@@SIPAVCScriptBase@@; script vtable slot 3 (slot 1 pushes "Main" 0x012C2F94); PDB Init; refs=1; was sub_00ee9110 (wave3/labels_alloc.tsv); PDB ?Init@CGlobal_DebugCycleThroughSpeechScript@NScript@@UAEXXZ in FableWin.pdb.
// Naming wave 35: was landed as the gap-scan stand-in T::Hook() on a `struct T` pad model (00/ee/gapscan_sub00ee9110_00ee9110.cpp, `?Hook@T@@QAEXXZ`); same bytes.
// Model: engine/CScriptBase.h + Game +0x40 (PDB ctor CGlobal_DebugCycleThroughSpeechScript(CGameScriptInterfaceBase&): no master data); Init declared public virtual as the PDB spells it.
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptBase.h"
class CGameScriptInterfaceBase;
namespace NScript {
class CGlobal_DebugCycleThroughSpeechScript : public CScriptBase {
public:
    virtual void Init();                          // slot 3
    CGameScriptInterfaceBase* Game;         // +0x40 (the ctor's CGameScriptInterfaceBase&)
};
void CGlobal_DebugCycleThroughSpeechScript::Init()
{
}
}
