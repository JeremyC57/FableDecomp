// NScript::Cbandit_level_cutsceneScript::Cbandit_level_cutsceneScript(CScriptDataBase* data, CGameScriptInterfaceBase& game) -- script constructor: CScriptBase base, then the CLevelScriptBase members Game = &game and MasterScriptData = data, then the script's vtable
// Leaf: ??0Cbandit_level_cutsceneScript@NScript@@QAE@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00EFA420, 48 bytes, `ret 8`. Stores the script vtable @ 0x012F69E8.
// Identity: that vtable is stored by the script's Alloc, which CScriptInfoManager::RegisterScripts (0x00CD52D0) registers as
//   "bandit_level_cutscene" (site 0x00CDA84D) -> NScript::Cbandit_level_cutsceneScript; the PDB name sets hold this constructor's mangled name (above).
//   Family F003 (76 byte-identical bodies): the manifest label (CEngineSubPrimitiveAnimatedMesh ctor) is propagated. The
//   out-of-line constructor has no caller in retail (the Alloc inlines its own copy).
// Callees: CScriptBase::CScriptBase() @ 0x00CB8110.
// Layout: CScriptBase is retail 0x40 bytes (vptr + 0x3c); CLevelScriptBase<TData> (PDB) adds Game +0x40 and MasterScriptData
//   +0x44. TData (the master script data class) enters no symbol of this function, so the base is modelled with the data
//   pointer typed as its CScriptDataBase base.
// Shape: the intermediate CLevelScriptBase vtable store is dead and dropped by VC7.1; the inline base constructor's
//   initialisers load the second argument first (Game, `mov eax,[esp+0xc]`) and store +0x40 before +0x44.
class CScriptDataBase {
public:
    virtual ~CScriptDataBase();
};
namespace NScript { class Cjoss_test_scriptsData; }
class CGameScriptInterfaceBase;
class CPersistContext;
// CScriptBase / CLevelScriptBase<T>: the retail NScript model from the shared headers (FABLE_RETAIL_NSCRIPT: CScriptBase :
//   CBaseClassNonCopyable : CBaseClass, vtable 0x012C1648, 5 slots, 0x40; CLevelScriptBase<T> Game +0x40, MasterScriptData +0x44).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CLevelScriptBase.h"
namespace NScript {
class Cbandit_level_cutsceneScript : public CLevelScriptBase<NScript::Cjoss_test_scriptsData> {
public:
    Cbandit_level_cutsceneScript(CScriptDataBase* data, CGameScriptInterfaceBase& game);
    virtual void RegisterMain();
    virtual void Main();
};
Cbandit_level_cutsceneScript::Cbandit_level_cutsceneScript(CScriptDataBase* data, CGameScriptInterfaceBase& game)
    : CLevelScriptBase<NScript::Cjoss_test_scriptsData>(data, game)
{
}
}