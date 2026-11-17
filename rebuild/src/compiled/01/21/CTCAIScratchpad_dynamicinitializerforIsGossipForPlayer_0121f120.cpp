// dynamic initializer for 'CTCAIScratchpad::IsGossipForPlayer' (0x013BAE38) -- construct the static STLport map<CCharString, bool> (0x18-byte header node from malloc, empty, self-linked) and register its atexit destructor 0x01229C40
// Leaf: _$E1
// Retail: Fable.exe (TLC, VC7.1) @ 0x0121F120, 60 bytes.
// agentNF89 (q11172 / q11173): the static map is CTCAIScratchpad::IsGossipForPlayer (engine/CTCAIScratchpad.h FABLE_RETAIL_CTCAISCRATCHPAD_GOSSIP_BOOL_MAPS;
//   Ego_r.pdb `dynamic initializer for 'CTCAIScratchpad::IsGossipForPlayer'` / `dynamic atexit destructor for ..`, Ego_d.pdb public
//   ?IsGossipForPlayer@CTCAIScratchpad@@2V?$map@VCCharString@@_N...@std@@A, FableWin.pdb accessor types map<CCharString, bool>), defined here on
//   stlport/fable_map.h as the landed Gossip pair 0x0121F160 / 0x01229CA0: VC7.1 emits the dynamic initializer $E1 (the inline map
//   constructor, then atexit($E2)) and the atexit destructor $E2 (the inline ~map with the out-of-line _M_erase 0x007ACC50, then free of
//   the header node) from this one definition. Was the g_tree_013bae38 (CRetailTree_node0x18 stand-in).
#include "fable_compiler.h"
#include "rebuild_abi.h"
#include "stlport/fable_containers.h"
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCAISCRATCHPAD_GOSSIP_BOOL_MAPS
#include "engine/ETCInterfaceType.h"
#include "engine/CTCAIScratchpad.h"

std::map<CCharString, bool> CTCAIScratchpad::IsGossipForPlayer;   // 0x013BAE38
