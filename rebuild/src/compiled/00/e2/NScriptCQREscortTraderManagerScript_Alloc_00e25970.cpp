// NScript::CQR_EscortTrader_ManagerScript::Alloc(CScriptDataBase* data, CGameScriptInterfaceBase& game) -- the script-info factory: new CQR_EscortTrader_ManagerScript(data, game)
// Leaf: ?Alloc@CQR_EscortTrader_ManagerScript@NScript@@SIPAVCScriptBase@@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E25970, 54 bytes, static __fastcall (data in ecx, the interface in edx), plain `ret`.
// Identity: CScriptInfoManager::RegisterScripts 00cd52d0 stores Alloc 00e25970 at 00cd7c0d with name "QR_EscortTrader_Manager"; Alloc news + stores script vtable 012e1d48; PDB ?Alloc@CQR_EscortTrader_ManagerScript@NScript@@SIPAVCScriptBase@@; refs=1; was _global::Alloc_CVtable_012e1d48; PDB ?Alloc@CQR_EscortTrader_ManagerScript@NScript@@SIPAVCScriptBase@@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z in Ego_r.pdb / FableWin.pdb.
// Naming wave 35: was landed as the free __fastcall stand-in `?Alloc_CVtable_012e1d48@@YIPAUCVtable_012e1d48@@PAX0@Z` (00/e2/global_AllocCVtable012e1d48_00e25970.cpp); same bytes.
// Callees: scalar operator new 0x00BFEA1A (0x58 = sizeof, Ego_r.pdb 108 - 0x14), CScriptBase::CScriptBase 0x00CB8110; the constructor is inlined
//   (CLevelScriptBase<T>(data, game): Game +0x40, MasterScriptData +0x44, then the script vtable).
// Model (NF436): the shared engine/CQR_EscortTrader_ManagerScript.h (NEW header: Ego_r.pdb layout, FableWin.pdb methods), replacing a local class that declared none of the class's own virtuals
//   and an out-of-line destructor (FableWin.pdb: compiler-generated). The user constructor is inline in the header. The vftable 0x012E1D48 now carries the FableWin.pdb overrides (RegisterMain Main Init OnPersist) and
//   this TU emits the implicit ??_G with it, as retail's TU did; FABLE_RETAIL_CLEVELSCRIPTBASE: the implicit destructor calls
//   ~CScriptBase 0x00CBD510 directly.
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#include "rebuild_abi.h"
#include "engine/CQR_EscortTrader_ManagerScript.h"
CScriptBase* FABLE_FASTCALL NScript::CQR_EscortTrader_ManagerScript::Alloc(CScriptDataBase* data, CGameScriptInterfaceBase& game)
{
    return new CQR_EscortTrader_ManagerScript(data, game);
}
