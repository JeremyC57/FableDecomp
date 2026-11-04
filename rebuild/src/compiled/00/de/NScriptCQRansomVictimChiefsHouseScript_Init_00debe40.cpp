// NScript::CQ_RansomVictimChiefsHouseScript::Init() -- reset the script state: SetChiefsHut = false
// Leaf: ?Init@CQ_RansomVictimChiefsHouseScript@NScript@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00DEBE40, 5 bytes, thiscall, plain `ret`.
// Identity: outer script vtable 012ddba8 slot 3 (slot 1 RegisterMain pushes "Main" 0x012C2F94); enclosing 00debef0 (slot 2) registers 1 entity scripts whose PDB Alloc outer is CQ_RansomVictimChiefsHouseScript; PDB ?Init@CQ_RansomVictimChiefsHouseScript@NScript@@; refs=1; was sub_00debe40 (wave3/labels_alloc.tsv); PDB ?Init@CQ_RansomVictimChiefsHouseScript@NScript@@UAEXXZ in FableWin.pdb.
// Naming wave 35: was landed as the gap-scan stand-in T::Clear() on a `struct T` pad model (00/de/gapscan_sub00debe40_00debe40.cpp, `?Clear@T@@QAEXXZ`); same bytes.
// Model: engine/CLevelScriptBase.h (Game +0x40, MasterScriptData +0x44); the members this body stores are named from
//   Ego_r.pdb CQ_RansomVictimChiefsHouseScript at +0x14 (the retail script base is 0x14 smaller), the rest is not modelled here.
#define FABLE_RETAIL_NSCRIPT
#include "engine/CLevelScriptBase.h"
namespace NScript {
class CQ_SunnyvaleMasterData;
class CQ_RansomVictimChiefsHouseScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    virtual void Init();                          // slot 3
    bool SetChiefsHut;                  // +0x48 (Ego_r.pdb +0x5c)
};
void CQ_RansomVictimChiefsHouseScript::Init()
{
    SetChiefsHut = false;
}
}
