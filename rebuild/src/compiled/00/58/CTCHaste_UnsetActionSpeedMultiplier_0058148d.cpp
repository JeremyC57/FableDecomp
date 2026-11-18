// CTCHaste::UnsetActionSpeedMultiplier(void* key) -- drop the key's multiplier (Multipliers.erase(key), the out-of-line
//   map<void*,float> tree erase 0x0058119B), then recompute the speed (the private UpdateActionSpeedMultiplier 0x00800020)
// Leaf: ?UnsetActionSpeedMultiplier@CTCHaste@@QAEXPAX@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0058148D, 27 bytes, size TU.
// NF64 (round 7, q10667 / q10668): moved onto engine/CTCHaste.h (FABLE_RETAIL_CTCHASTE: the Ego_r.pdb std::map<void*,float> Multipliers
//   on stlport/fable_map.h; FABLE_RETAIL_CTCHASTE_UNSET: the Ego_r.pdb ?UnsetActionSpeedMultiplier@CTCHaste@@QAEXPAX@Z declaration). The
//   erase now binds the STLport _Rb_tree::erase(const key_type&) spelling NF61's 0x00694020 binds at 0x0058119B, and the update the private
//   ?UpdateActionSpeedMultiplier@CTCHaste@@AAEXXZ. Was a local CTCHaste model over a CMultiplierMap stand-in.
#pragma optimize("s", on)
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCHASTE
#define FABLE_RETAIL_CTCHASTE_UNSET
#define FABLE_RETAIL_STLP_TREE_ERASE
#define FABLE_RETAIL_STLP_TREE_ERASE_KEY
#include "fable_compiler.h"
#include "fable_string.h"
#include "engine/ETCInterfaceType.h"
#include "engine/CTCHaste.h"

void CTCHaste::UnsetActionSpeedMultiplier(void* key)
{
    Multipliers.erase(key);
    UpdateActionSpeedMultiplier();
}
