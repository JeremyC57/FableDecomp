// NScript::CTestQuestCard1Script::Alloc(CScriptDataBase* data, CGameScriptInterfaceBase& game) -- the script-info factory: new CTestQuestCard1Script(data, game)
// Leaf: ?Alloc@CTestQuestCard1Script@NScript@@SIPAVCScriptBase@@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00EEF540, 54 bytes, static __fastcall (data in ecx, the interface in edx), plain `ret`.
// Identity: CScriptInfoManager::RegisterScripts 00cd52d0 stores Alloc 00eef540 at 00cdad95 with name "TestQuestCard1"; Alloc news + stores script vtable 012f5a78; PDB ?Alloc@CTestQuestCard1Script@NScript@@SIPAVCScriptBase@@; refs=1; was _global::Alloc_CVtable_012f5a78; PDB ?Alloc@CTestQuestCard1Script@NScript@@SIPAVCScriptBase@@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z in Ego_r.pdb / FableWin.pdb.
// Naming wave 35: was landed as the free __fastcall stand-in `?Alloc_CVtable_012f5a78@@YIPAUCVtable_012f5a78@@PAX0@Z` (00/ee/global_AllocCVtable012f5a78_00eef540.cpp); same bytes.
// Callees: scalar operator new 0x00BFEA1A (0x4C = sizeof, Ego_r.pdb 96 - 0x14), CScriptBase::CScriptBase 0x00CB8110; the constructor is inlined
//   (CLevelScriptBase<T>(data, game): Game +0x40, MasterScriptData +0x44, then the script vtable).
// Model (NF436): the shared engine/CTestQuestCard1Script.h, replacing a local class that declared none of the class's own virtuals
//   and an out-of-line destructor (FableWin.pdb: compiler-generated). The user constructor is defined `inline` here (FableWin.pdb
//   declares it; Alloc expands it, 0x00D80C00 pattern). The vftable 0x012F5A78 now carries the FableWin.pdb overrides (RegisterMain Main) and
//   this TU emits the implicit ??_G with it, as retail's TU did; FABLE_RETAIL_CLEVELSCRIPTBASE: the implicit destructor calls
//   ~CScriptBase 0x00CBD510 directly.
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#include "rebuild_abi.h"
#include "engine/CTestQuestCard1Script.h"
inline NScript::CTestQuestCard1Script::CTestQuestCard1Script(CScriptDataBase* data, CGameScriptInterfaceBase& game) : CLevelScriptBase<CEmptyData>(data, game)
{
}
CScriptBase* FABLE_FASTCALL NScript::CTestQuestCard1Script::Alloc(CScriptDataBase* data, CGameScriptInterfaceBase& game)
{
    return new CTestQuestCard1Script(data, game);
}
