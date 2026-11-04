// NScript::CQS_GuardianTrophyDealerInfoScript::Init() -- reset the script state: PieceOver = false
// Leaf: ?Init@CQS_GuardianTrophyDealerInfoScript@NScript@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E27870, 5 bytes, thiscall, plain `ret`.
// Identity: outer script vtable 012e2018 slot 3 (slot 1 RegisterMain pushes "Main" 0x012C2F94); enclosing 00e27920 (slot 2) registers 1 entity scripts whose PDB Alloc outer is CQS_GuardianTrophyDealerInfoScript; PDB ?Init@CQS_GuardianTrophyDealerInfoScript@NScript@@; refs=1; was sub_00e27870 (wave3/labels_alloc.tsv); PDB ?Init@CQS_GuardianTrophyDealerInfoScript@NScript@@UAEXXZ in FableWin.pdb.
// Naming wave 35: was landed as the gap-scan stand-in T::Clear() on a `struct T` pad model (00/e2/gapscan_sub00e27870_00e27870.cpp, `?Clear@T@@QAEXXZ`); same bytes.
// Model: engine/CLevelScriptBase.h (Game +0x40, MasterScriptData +0x44); the members this body stores are named from
//   Ego_r.pdb CQS_GuardianTrophyDealerInfoScript at +0x14 (the retail script base is 0x14 smaller), the rest is not modelled here.
#define FABLE_RETAIL_NSCRIPT
#include "engine/CLevelScriptBase.h"
namespace NScript {
class CQ_SunnyvaleMasterData;
class CQS_GuardianTrophyDealerInfoScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    virtual void Init();                          // slot 3
    bool PieceOver;                     // +0x48 (Ego_r.pdb +0x5c)
};
void CQS_GuardianTrophyDealerInfoScript::Init()
{
    PieceOver = false;
}
}
