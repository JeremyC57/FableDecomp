// dynamic initializer for 'CTCAIScratchpad::Gossip' (0x013BAE44) -- construct the static STLport map (0x20-byte header node from malloc, empty, self-linked) and register its atexit destructor 0x01229CA0
// Leaf: _$E1
// Retail: Fable.exe (TLC, VC7.1) @ 0x0121F160, 60 bytes.
// agentNF57 (q10430): the static map is CTCAIScratchpad::Gossip (engine/CTCAIScratchpad.h FABLE_RETAIL_CTCAISCRATCHPAD_GOSSIP_MAPS, Ego_r.pdb
//   `dynamic initializer for 'CTCAIScratchpad::Gossip'`; retail 0x013BAE44, GossipVillages 0x013BAE50, GossipFactions 0x013BAE5C), defined here
//   on stlport/fable_map.h: VC7.1 emits its dynamic initializer $E1 (0x0121F160: the inline map constructor, then atexit($E2)) and the
//   atexit destructor $E2 (0x01229CA0: the inline ~map with the out-of-line _M_erase 0x007AE0E0, then free of the header node) from
//   this one definition. Both relocations to 0x013BAE44 bind ?Gossip@CTCAIScratchpad@@2V?$map@...@std@@A (four-argument STLport map).
//   Was the CRetailTree_node0x20 g_tree_013bae44 stand-in.
#include "fable_compiler.h"
#include "rebuild_abi.h"
#include "stlport/fable_containers.h"
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCAISCRATCHPAD_GOSSIP_MAPS
#include "engine/ETCInterfaceType.h"
#include "engine/CTCAIScratchpad.h"

std::map<CCharString, std::vector<CCharString> > CTCAIScratchpad::Gossip;   // 0x013BAE44
