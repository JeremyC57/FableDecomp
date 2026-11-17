// CTCHaste::SetActionSpeedMultiplier(void* key, float multiplier) -- insert (key, multiplier) into Multipliers (+0x18; map::insert -> _Rb_tree::insert_unique 0x00580D21, the result in a 16-byte frame local), then UpdateActionSpeedMultiplier 0x00800020
// Leaf: ?SetActionSpeedMultiplier@CTCHaste@@QAEXPAXM@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005812AE, length 49 bytes; frame-pointer size TU (the 0x0058xxxx module), `leave ; ret 8`.
// Identity (NF61, round 7, q10575): Ego_r.pdb / FableWin.pdb ?SetActionSpeedMultiplier@CTCHaste@@QAEXPAXM@Z, the inline member that
//   engine/CTCHaste.h already spells (map<void*, float> Multipliers +0x18, then the private UpdateActionSpeedMultiplier 0x00800020,
//   which the landed callee row carries); Ego_r keeps an out-of-line copy and this is retail's (unreferenced COMDAT: no call or data
//   reference). The body inserts into exactly NF58's map<void*, float> instance (0x00580D21, Ego_r CTCHaste::Multipliers). Was the
//   stand-in CBlockTable_005812ae::GetBlockData(long, long) with a local tree model (the chunk label
//   "CEngineStaticMapFileBlockManager::GetBlockData" was propagated). Header: overlay opt-in
//   FABLE_RETAIL_CTCHASTE_SETACTIONSPEEDMULTIPLIER_OUTOFLINE declares the member so this row can define retail's copy.
#pragma optimize("y", off)
#pragma optimize("s", on)
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCHASTE
#define FABLE_RETAIL_CTCHASTE_SETACTIONSPEEDMULTIPLIER_OUTOFLINE
#include "engine/ETCInterfaceType.h"
#include "engine/CTCHaste.h"

void CTCHaste::SetActionSpeedMultiplier(void* key, float multiplier)
{
    Multipliers.insert(std::map<void*, float>::value_type(key, multiplier));
    UpdateActionSpeedMultiplier();
}
