// NScript::CQ_BanditCampHoldingScriptScript::Alloc(CScriptDataBase* data, CGameScriptInterfaceBase& game) -- the script-info factory: new CQ_BanditCampHoldingScriptScript(data, game)
// Leaf: ?Alloc@CQ_BanditCampHoldingScriptScript@NScript@@SIPAVCScriptBase@@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00D12C50, 54 bytes, static __fastcall (data in ecx, the interface in edx), plain `ret`.
// Identity: CScriptInfoManager::RegisterScripts 00cd52d0 stores Alloc 00d12c50 at 00cd5b35 with name "Q_BanditCampHoldingScript"; Alloc news + stores script vtable 012c9e14; PDB ?Alloc@CQ_BanditCampHoldingScriptScript@NScript@@SIPAVCScriptBase@@; refs=1; was _global::Alloc_CVtable_012c9e14; PDB ?Alloc@CQ_BanditCampHoldingScriptScript@NScript@@SIPAVCScriptBase@@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z in Ego_r.pdb / FableWin.pdb.
// Naming wave 35: was landed as the free __fastcall stand-in `?Alloc_CVtable_012c9e14@@YIPAUCVtable_012c9e14@@PAX0@Z` (00/d1/global_AllocCVtable012c9e14_00d12c50.cpp); same bytes.
// Callees: scalar operator new 0x00BFEA1A (0x4C = sizeof, Ego_r.pdb 96 - 0x14), CScriptBase::CScriptBase 0x00CB8110; the constructor is inlined
//   (CLevelScriptBase<T>(data, game): Game +0x40, MasterScriptData +0x44, then the script vtable).
// Model (NF436): the shared engine/CQ_BanditCampHoldingScriptScript.h (NEW header: Ego_r.pdb layout, FableWin.pdb methods), replacing a local class that declared none of the class's own virtuals
//   and an out-of-line destructor (FableWin.pdb: compiler-generated). The user constructor is inline in the header. The vftable 0x012C9E14 now carries the FableWin.pdb overrides (RegisterMain Main Init OnPersist) and
//   this TU emits the implicit ??_G with it, as retail's TU did; FABLE_RETAIL_CLEVELSCRIPTBASE: the implicit destructor calls
//   ~CScriptBase 0x00CBD510 directly.
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#include "rebuild_abi.h"
#include "engine/CQ_BanditCampHoldingScriptScript.h"
CScriptBase* FABLE_FASTCALL NScript::CQ_BanditCampHoldingScriptScript::Alloc(CScriptDataBase* data, CGameScriptInterfaceBase& game)
{
    return new CQ_BanditCampHoldingScriptScript(data, game);
}
