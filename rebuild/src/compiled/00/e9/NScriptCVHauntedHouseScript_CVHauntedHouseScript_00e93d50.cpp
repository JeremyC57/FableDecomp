// NScript::CV_HauntedHouseScript::CV_HauntedHouseScript(CScriptDataBase* data, CGameScriptInterfaceBase& game) -- script constructor: CScriptBase base, then the CLevelScriptBase members Game = &game and MasterScriptData = data, then the script's vtable
// Leaf: ??0CV_HauntedHouseScript@NScript@@QAE@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00E93D50, 48 bytes, `ret 8`. Stores the script vtable @ 0x012ECB94.
// Identity: that vtable is stored by the script's Alloc, which CScriptInfoManager::RegisterScripts (0x00CD52D0) registers as
//   "V_HauntedHouse" (site 0x00CD8B33) -> NScript::CV_HauntedHouseScript; the PDB name sets hold this constructor's mangled name (above).
//   Family F003 (76 byte-identical bodies): the manifest label (CEngineSubPrimitiveAnimatedMesh ctor) is propagated. The
//   out-of-line constructor has no caller in retail (the Alloc inlines its own copy).
// Callees: CScriptBase::CScriptBase() @ 0x00CB8110.
// Layout: CScriptBase is retail 0x40 bytes (vptr + 0x3c); CLevelScriptBase<TData> (PDB) adds Game +0x40 and MasterScriptData
//   +0x44. TData (the master script data class) enters no symbol of this function, so the base is modelled with the data
//   pointer typed as its CScriptDataBase base.
// Shape: the intermediate CLevelScriptBase vtable store is dead and dropped by VC7.1; the inline base constructor's
//   initialisers load the second argument first (Game, `mov eax,[esp+0xc]`) and store +0x40 before +0x44.
// NF336 (2026-11-24): moved off the row-local class model onto the shared engine headers (acctB Gate 0 prerequisite). The family
//   preamble below is shared by the CV_HauntedHouseScript object-mate rows (a per-row departure is marked where it occurs).
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_SUSPENDABLE_PROCESS
// FABLE_RETAIL_CLEVELSCRIPTBASE left undefined here: ~CLevelScriptBase<T> stays declared out of line (the HEAD form), so this TU emits no unreferenced ??1?$CLevelScriptBase thunk (NF320 Q2).
#define FABLE_RETAIL_CLEVELSCRIPTBASE_ORIGINAL_MEMBERS
#define FABLE_RETAIL_CENTITYSCRIPTBASE_ORIGINAL_MEMBERS
#define FABLE_RETAIL_CV_HAUNTEDHOUSE_ORIGINAL_MEMBERS
#define FABLE_RETAIL_CV_HAUNTEDHOUSE_DECLS
#define FABLE_RETAIL_CM_UNDEADHERE_MAIN
#define FABLE_RETAIL_CM_QUESTCONTROL_MAIN_PASS26
#define FABLE_RETAIL_CHHUNDEAD_MEMBERS
#define FABLE_RETAIL_CM_HEROINSIDEHOUSE_MEMBERS
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CV_HauntedHouseScript.h"
NScript::CV_HauntedHouseScript::CV_HauntedHouseScript(CScriptDataBase* data, CGameScriptInterfaceBase& game)
    : CLevelScriptBase<NScript::CQ_SunnyvaleMasterData>(data, game)
{
}
