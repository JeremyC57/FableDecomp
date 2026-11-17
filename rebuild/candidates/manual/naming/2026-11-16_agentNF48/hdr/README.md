# NF48 header overlay (round 7)

Two full copies of repo headers, each with `#ifdef` opt-ins only. Without the opt-in macros both files are token-identical to the
repo copies (`diff repo overlay | grep '^<'` is empty; re-diffed just before hand-back).

## stlport/fable_containers.h (copy of rebuild/include/stlport/fable_containers.h)
In `class vector`, the destructor line `~vector() { _Destroy(this->_M_start,this->_M_finish); }` is wrapped:
- `FABLE_RETAIL_STLP_VECTOR_DTOR_OUTOFLINE`: `~vector();` (declaration only). For TUs that call the out-of-line COMDAT at every
  site: ~vector<CScriptThing> 0x008AC970 from 0x00D2D900 / 0x00D61C00 / 0x00DC2540 (deleting destructors) and 0x00DC8890.
- `FABLE_RETAIL_STLP_VECTOR_DTOR_FORCEINLINE`: the same body with `FABLE_FORCEINLINE` (an inline control under the owner's
  2026-11-08 inline-budget decision; same pattern as FABLE_RETAIL_CARRAY_SIZE_CTOR_FORCEINLINE). Used by 0x00EE6A40, 0x00EDD060,
  0x00D18B20 (with a scoped inline_depth(0) region for the one out-of-line exit) and 0x00CB5B90.
- Otherwise the original line.

Include order: a row using these opt-ins includes `stlport/fable_containers.h` itself before any repo header that includes it
(NF41 lesson: a repo header's quoted include resolves to the repo copy first; the include guard then keeps the overlay copy).

## engine/CTCAIScratchpad.h (copy of rebuild/include/engine/CTCAIScratchpad.h)
- `FABLE_RETAIL_CTCAISCRATCHPAD_GOSSIP_MAPS`: `#include "stlport/fable_map.h"` after the existing includes, and at the top of
  the class the three static data members `Gossip`, `GossipVillages`, `GossipFactions`, each
  `std::map<CCharString, std::vector<CCharString> >` (Ego_r.pdb `dynamic initializer for 'CTCAIScratchpad::Gossip'` / ...;
  retail 0x013BAE44 / 0x013BAE50 / 0x013BAE5C; value type from CGameScriptInterface::GetGossip* in engine/CGameScriptInterface.h).
  Declarations only. Used by 0x008AA010 / 0x008AA270 / 0x008AA4D0. Includers must define FABLE_RETAIL_CTCBASE,
  FABLE_RETAIL_DEF_CLASS_BASE, FABLE_RETAIL_CCHARSTRING and include engine/ETCInterfaceType.h first (the header's existing needs).

## Unrelated includers rechecked under the overlay (probes)
- 0x006B3B80 CEnvironment::LightingFlash (includes stlport/fable_containers.h): RELOCATION_MATCH.
- 0x007ACDD0 CTCAIScratchpad::IsComponentSerialisable (includes engine/CTCAIScratchpad.h): MATCH.

Macro names are specific to the member (STLP_VECTOR_DTOR_*, CTCAISCRATCHPAD_GOSSIP_MAPS) so the coordinator can merge them with
NF47's engine-header opt-ins.
