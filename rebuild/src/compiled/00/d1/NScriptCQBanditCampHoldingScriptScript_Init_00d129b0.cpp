// NScript::CQ_BanditCampHoldingScriptScript::Init() -- reset the script state: PathReached = false
// Leaf: ?Init@CQ_BanditCampHoldingScriptScript@NScript@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00D129B0, 5 bytes, thiscall, plain `ret`.
// Identity: CScriptInfoManager::RegisterScripts 00cd52d0 stores Alloc 00d12c50 at 00cd5b35 with name "Q_BanditCampHoldingScript"; Alloc news + stores script vtable 012c9e14; PDB ?Alloc@CQ_BanditCampHoldingScriptScript@NScript@@SIPAVCScriptBase@@; script vtable slot 3 (slot 1 pushes "Main" 0x012C2F94); PDB Init; refs=1; was sub_00d129b0 (wave3/labels_alloc.tsv); PDB ?Init@CQ_BanditCampHoldingScriptScript@NScript@@UAEXXZ in FableWin.pdb.
// Naming wave 35: was landed as the gap-scan stand-in T::Clear() on a `struct T` pad model (00/d1/gapscan_sub00d129b0_00d129b0.cpp, `?Clear@T@@QAEXXZ`); same bytes.
// Model: engine/CLevelScriptBase.h (CLevelScriptBase<CQ_SunnyvaleMasterData>, the quest-script default) (Game +0x40, MasterScriptData +0x44); the members this body stores are named from
//   Ego_r.pdb CQ_BanditCampHoldingScriptScript at +0x14 (the retail script base is 0x14 smaller), the rest is not modelled here.
#define FABLE_RETAIL_NSCRIPT
#include "engine/CLevelScriptBase.h"
namespace NScript {
class CQ_SunnyvaleMasterData;
class CQ_BanditCampHoldingScriptScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    virtual void Init();                          // slot 3
    bool PathReached;                   // +0x48 (Ego_r.pdb +0x5c)
};
void CQ_BanditCampHoldingScriptScript::Init()
{
    PathReached = false;
}
}
