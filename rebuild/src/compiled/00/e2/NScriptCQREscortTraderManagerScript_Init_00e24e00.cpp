// NScript::CQR_EscortTrader_ManagerScript::Init() -- reset the script state: Activated = false
// Leaf: ?Init@CQR_EscortTrader_ManagerScript@NScript@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E24E00, 5 bytes, thiscall, plain `ret`.
// Identity: CScriptInfoManager::RegisterScripts 00cd52d0 stores Alloc 00e25970 at 00cd7c0d with name "QR_EscortTrader_Manager"; Alloc news + stores script vtable 012e1d48; PDB ?Alloc@CQR_EscortTrader_ManagerScript@NScript@@SIPAVCScriptBase@@; script vtable slot 3 (slot 1 pushes "Main" 0x012C2F94); PDB Init; refs=1; was sub_00e24e00 (wave3/labels_alloc.tsv); PDB ?Init@CQR_EscortTrader_ManagerScript@NScript@@UAEXXZ in Ego_r.pdb / FableWin.pdb.
// Naming wave 35: was landed as the gap-scan stand-in T::Clear() on a `struct T` pad model (00/e2/gapscan_sub00e24e00_00e24e00.cpp, `?Clear@T@@QAEXXZ`); same bytes.
// Model: engine/CLevelScriptBase.h (Game +0x40, MasterScriptData +0x44); the members this body stores are named from
//   Ego_r.pdb CQR_EscortTrader_ManagerScript at +0x14 (the retail script base is 0x14 smaller), the rest is not modelled here.
#define FABLE_RETAIL_NSCRIPT
#include "engine/CLevelScriptBase.h"
namespace NScript {
class CQ_SunnyvaleMasterData;
class CQR_EscortTrader_ManagerScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    virtual void Init();                          // slot 3
    unsigned char _pad48[0x50 - 0x48];
    bool Activated;                     // +0x50 (Ego_r.pdb +0x64)
};
void CQR_EscortTrader_ManagerScript::Init()
{
    Activated = false;
}
}
