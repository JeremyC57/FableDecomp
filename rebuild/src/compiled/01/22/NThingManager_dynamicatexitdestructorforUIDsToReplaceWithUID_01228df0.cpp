// NThingManager::`dynamic atexit destructor for 'UIDsToReplaceWithUID'' -- the inline ~map: erase every node (the tree's _M_erase 0x00527950 from
//   the root), relink the header, zero the count and free the header node
// Retail: Fable.exe (TLC, VC7.1) @ 0x01228DF0, 84 bytes.
// Leaf: _$E2
// Identity (NF92, q11218 catalog fix): the static at 0x013B8A8C is NThingManager::UIDsToReplaceWithUID, Ego_d.pdb PUB32
//   ?UIDsToReplaceWithUID@NThingManager@@3V?$map@_K_KU?$less@_K@std@@V?$allocator@U?$pair@$$CB_K_K@std@@@2@@std@@A (a namespace-scope
//   std::map<unsigned __int64, unsigned __int64>: the 16-byte value gives the 0x20-byte tree node the initializer allocates). Ego_r.pdb +
//   FableWin.pdb name NThingManager::`dynamic initializer for 'UIDsToReplaceWithUID'' and its `dynamic atexit destructor`; Ego_r runs it
//   right after AttitudeSymbols and REACTION_MANAGER_DEF_INSTANTIATION_NAME (0x0100C9E0 / 0x0100CB00 / 0x0100CB20), as retail's .CRT$XCU runs
//   0x0121A630 (AttitudeSymbols, 0x013B8A54) -> 0x0121A650 (the CCharString at 0x013B8A7C) -> 0x0121A670; the atexit thunks follow in the
//   same order (0x01228DD0 / 0x01228DE0 / 0x01228DF0), and 0x013B8A8C sits in the thing-manager .bss block. The landed catalog called
//   0x0121A670 `CThing::$E_TCsToBeRemoved` (wrong: that list is 0x013B8A24, initializer 0x0121A440) and 0x01228DF0
//   `unattributed::StaticTreeDeleter013B8A8C`; both rows modelled the tree locally with unproven key / value types.
// Model: stlport/fable_map.h (the NF89 0x0121F0E0 / 0x01229BE0 two-row recipe: the static defined here, the compiler emitting both functions).
//   No header declares NThingManager::UIDsToReplaceWithUID, so the row defines the namespace-scope global itself.
#define FABLE_RETAIL_STLP_MAP
#include "fable_compiler.h"
#include "rebuild_abi.h"
#include "stlport/fable_containers.h"
#include "stlport/fable_map.h"

namespace NThingManager {
std::map<unsigned __int64, unsigned __int64> UIDsToReplaceWithUID;   // 0x013B8A8C
}
