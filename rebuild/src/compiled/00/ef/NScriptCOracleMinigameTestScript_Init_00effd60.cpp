// NScript::COracleMinigameTestScript::Init() -- reset the script state: MissionSucceeded = false
// Leaf: ?Init@COracleMinigameTestScript@NScript@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00EFFD60, 5 bytes, thiscall, plain `ret`.
// Identity: outer script vtable 012f70a0 slot 3 (slot 1 RegisterMain pushes "Main" 0x012C2F94); enclosing 00effe10 (slot 2) registers 3 entity scripts whose PDB Alloc outer is COracleMinigameTestScript; PDB ?Init@COracleMinigameTestScript@NScript@@; refs=1; was sub_00effd60 (wave3/labels_alloc.tsv); PDB ?Init@COracleMinigameTestScript@NScript@@UAEXXZ in FableWin.pdb.
// Naming wave 35: was landed as the gap-scan stand-in T::Clear() on a `struct T` pad model (00/ef/gapscan_sub00effd60_00effd60.cpp, `?Clear@T@@QAEXXZ`); same bytes.
// Model: engine/CLevelScriptBase.h (Game +0x40, MasterScriptData +0x44); the members this body stores are named from
//   Ego_r.pdb COracleMinigameTestScript at +0x14 (the retail script base is 0x14 smaller), the rest is not modelled here.
#define FABLE_RETAIL_NSCRIPT
#include "engine/CLevelScriptBase.h"
namespace NScript {
class CQ_SunnyvaleMasterData;
class COracleMinigameTestScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    virtual void Init();                          // slot 3
    bool MissionSucceeded;              // +0x48 (Ego_r.pdb +0x5c)
};
void COracleMinigameTestScript::Init()
{
    MissionSucceeded = false;
}
}
