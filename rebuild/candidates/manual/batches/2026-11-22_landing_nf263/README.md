# Landing nf263 (Account A, 2026-11-22): the GB unit of L129: three class operator deletes on their PDB placement forms (5 rows)

Agent NF263 (one L129 unit; the rest planned; 0x009B0AC0 not started). Hand-back: `pending/2026-11-22_nf263_handback/`.
- **Self renames** (July placeholders `?fn_00bf36a0@@YIPAXH@Z` / `?fn_00bf36e0@@YIPAXH@Z`, catalog `CEngineSubPrimitiveBase::operator_new`):
  0x00BF36A0 -> `??3CEngineLandscapePatch@@SIXPAXPAVCMovableResource@@@Z`, 0x00BF36E0 -> `??_VCEngineLandscapePatch@@SIXPAXPAVCMovableResource@@@Z`,
  0x00BF9920 -> `??3CLocalDetailPrimitiveMesh@@SIXPAXPAVCMovableResource@@@Z` (both PDBs; every retail caller zeroes `edx` first: 0x00BDD414 /
  0x00BDD431 / 0x00BDFD6F; retail's allocator quartets ??2 / ??3 / ??_U / ??_V follow Ego_r's order; siblings 0x00BF36C0 (??_U) / 0x00BF9900
  (??2) already use the placement form). At HEAD 0x00BF9920 also defined 0x00BF36A0's symbol; gone. Each on the landed GPLANDSCAPERENDERER
  opt-in and CEngineLandscapeMemoryManager.h's retail branch, the class modelled locally in the siblings' shape (CEngineLandscapePatch's
  only header is in `_quarantine/`; CLocalDetailPrimitiveMesh has none); `.text` identical to HEAD. Catalog / Module / files respelled;
  tests rewritten on the shared headers (they link the row object; PassPatterns kept).
- **Binders:** 0x00BDD3E0's header-less local model respelled in place (`long` -> `CMovableResource*`; every section and defined symbol
  identical to HEAD; vtcheck / pk.py unchanged); 0x00BDFD60's free stand-in `CLocalDetailPrimitiveMesh_dealloc` -> the class `operator delete`
  (full Leaf line added).
- Census: 0x01436EA8 loses `?g_00bf36a0` / `?g_00bf36e0` (table lines 14 -> 12; 12 stand-in spellings left on 22 rows); 0x00BD9B90 loses
  `?m@That2` (2 -> 1); 0x00BF36A0 2 -> 1, 0x00BF36E0 2 -> 1, 0x00BF9920 3 -> 1. Table: 6 deleted (one stale at HEAD), 3 added. Link: 0
  duplicates (control 1).
- **Recheck** (`logs/recheck.log`, landed tree): the 5 changed rows + 0x00BF36C0, 0x00BDE600, 0x00BF5D30, 0x00BF9900, 0x00BD9B90 (10 rows): 10
  RELOCATION_MATCH PASS, 0 RELOC_CONFLICT.

## Coordinator rulings on NF263's questions
- **D1:** the PDB placement forms, not the brief's one-parameter `...SIXPAX@Z`: accepted. Those plain forms are separate retail functions at
  0x00BDBF70 / 0x00BDBF80 (`xor edx,edx ; jmp` to the placement form), so the brief's "same bytes" was wrong (REJECTED.tsv).
- **D2:** 0x00BDFD60's forward-declared `struct CLocalDetailPrimitiveMesh;` keeps `struct` so its stand-in self leaf (`PAU...`) does not change;
  the `class` key (retail's) is queued together with that row's self rename.
- **D3:** queued: 0x00BDD3E0's cdecl in-class forwarders -> `__fastcall` (the FableWin spelling with the 0x00BDBF70 / 80 bytes); catalog
  names for 0x00BF36C0 / 0x00BF9900 (both `_Cons_val`); catalog entries for the five uncatalogued neighbours; 0x00BF36C0 / 0x00BF5D30 off
  their row-local CEngineLandscapeMemoryManager models.
Unblocked for the next pass: 0x00BE5C30 + 0x00BE6090 with 0x00BDA110 (`?DeleteTexture@CEngineLandscapeMemoryManager@@QAEXPAVCTexture@@@Z`:
Ego_r 009c26a0 + FableWin 02cb7490; needs an add-only declaration opt-in in CEngineLandscapeMemoryManager.h with equiv_check).
Slips (NF89 LESSONS read after grading; the recheck's `timeout 900` inside its script, not on the launch line): noted.
