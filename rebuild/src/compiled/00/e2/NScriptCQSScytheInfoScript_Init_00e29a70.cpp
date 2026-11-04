// NScript::CQS_ScytheInfoScript::Init() -- reset the script state: MissionSucceeded = false
// Leaf: ?Init@CQS_ScytheInfoScript@NScript@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E29A70, 5 bytes, thiscall, plain `ret`.
// Identity: Anniversary wave 2026-11-04 (high): vtable 0x012E22C8 slot 3; Anniversary RTTI NScript::CQS_ScytheInfoScript (string-literal match, validated 257/258); PDB name set has the Init member of NScript::CQS_ScytheInfoScript; slot layout from named TLC siblings; function referenced by this vtable only (2026-11-04_anniversary/labels_high.tsv); PDB ?Init@CQS_ScytheInfoScript@NScript@@UAEXXZ in Ego_r.pdb / FableWin.pdb.
// Naming wave 35: was landed as the gap-scan stand-in T::Clear() on a `struct T` pad model (00/e2/gapscan_sub00e29a70_00e29a70.cpp, `?Clear@T@@QAEXXZ`); same bytes.
// Model: engine/CLevelScriptBase.h (Game +0x40, MasterScriptData +0x44); the members this body stores are named from
//   Ego_r.pdb CQS_ScytheInfoScript at +0x14 (the retail script base is 0x14 smaller), the rest is not modelled here.
#define FABLE_RETAIL_NSCRIPT
#include "engine/CLevelScriptBase.h"
namespace NScript {
class CQ_SunnyvaleMasterData;
class CQS_ScytheInfoScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    virtual void Init();                          // slot 3
    bool MissionSucceeded;              // +0x48 (Ego_r.pdb +0x5c)
};
void CQS_ScytheInfoScript::Init()
{
    MissionSucceeded = false;
}
}
