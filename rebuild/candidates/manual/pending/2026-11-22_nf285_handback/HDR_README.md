# NF285 overlay headers

## engine/CAIStateGroup_TavernServer.h (base: orig/hdr/engine/CAIStateGroup_TavernServer.h = repo HEAD copy)
- New add-only opt-in `FABLE_RETAIL_CAISTATEGROUP_TAVERNSERVER_FLAT_METHODS`, inside the flat (`#else`, pack(1)) `struct CAIStateGroup_TavernServer`,
  after the GETNAME_ALLOC block: declarations only (no body, no virtual, no data) of the six public members GetBar 0x00966AF0,
  IsCarryingJug 0x00966CF0, GetServingPoint 0x00966DA0, FreeServingPoint 0x009670A0, FreeBarPoint 0x00967110, GetJug 0x00967180
  (Ego_r.pdb + FableWin.pdb `?<Name>@CAIStateGroup_TavernServer@@QAE_NXZ` / `QAEXXZ`; a member's own mangling carries no class key,
  so the flat struct's `struct` key does not change them). User: row 0x00967650 only.
- No existing line edited; the default path is token-identical. pin_asserts.py: 0 pinned (every assert after the block is already
  `FABLE_STATIC_ASSERT_AT`; the retail branch's line-49 assert sits above the block).
- equiv_check (evidence/equiv_tavern.txt): the header's 15 landed includers, 0 differ; the planted-token control flags 11 of them
  (the other 4 name the header only in comments). No other header includes it (rebuild/include grep: INDEX.tsv only), and no row
  `#include`s one of those 15 sources.
- Other agents' overlays: none of $S/w22/agentNF28[0-4]/hdr carries this header.
