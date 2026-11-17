// atexit destructor for 'CTCAIScratchpad::ActiveGossipCategories' (0x013BAE2C) -- clear the static STLport map<CCharString, bool> (recursive _M_erase 0x007ACC50 from the root, header relinked, size zeroed) and free its header node
// Leaf: _$E2
// Retail: Fable.exe (TLC, VC7.1) @ 0x01229BE0, 84 bytes.
// agentNF89 (q11172 / q11173): the static map is CTCAIScratchpad::ActiveGossipCategories (engine/CTCAIScratchpad.h FABLE_RETAIL_CTCAISCRATCHPAD_GOSSIP_BOOL_MAPS;
//   Ego_r.pdb `dynamic initializer for 'CTCAIScratchpad::ActiveGossipCategories'` / `dynamic atexit destructor for ..`, Ego_d.pdb public
//   ?ActiveGossipCategories@CTCAIScratchpad@@2V?$map@VCCharString@@_N...@std@@A, FableWin.pdb accessor types map<CCharString, bool>), defined here on
//   stlport/fable_map.h as the landed Gossip pair 0x0121F160 / 0x01229CA0: VC7.1 emits the dynamic initializer $E1 (the inline map
//   constructor, then atexit($E2)) and the atexit destructor $E2 (the inline ~map with the out-of-line _M_erase 0x007ACC50, then free of
//   the header node) from this one definition. Was the CRetailTree / SNode stand-in (_unattributed::Database_Array_BuildDeleter1_01229be0).
#include "fable_compiler.h"
#include "rebuild_abi.h"
#include "stlport/fable_containers.h"
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCAISCRATCHPAD_GOSSIP_BOOL_MAPS
#include "engine/ETCInterfaceType.h"
#include "engine/CTCAIScratchpad.h"

std::map<CCharString, bool> CTCAIScratchpad::ActiveGossipCategories;   // 0x013BAE2C
