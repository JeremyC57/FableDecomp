# 2026-11-20 landing nf112: PRIORITY slice 2 remainder, first hand-back (20 rows)

Records: `naming/2026-11-20_agentNF112/` (selfcheck 0 problems). 16 entries (NF108's leftovers): 1 done (q4769), 1 done-already (q6230),
1 rejected (q6210: 0x00514E20 is CArray<unsigned char>(unsigned int), not vector<unsigned char>(size); FableWin has both, the vector one is
the twin 0x005136B0), 2 blocked (q5205 family, 3 rows left: 0x0040F7E0 held ready, 0x00816000 DIFFER 332v324 on the CPersistTraits<CDefString>
DefaultVal store, 0x00816150; q6212: the Leaf-less 0x00644627 placeholder), 11 open.

## Rows (20): 20 RELOCATION_MATCH, every test PASS (`logs/recheck.log`)
14 ports (13 onto stlport/fable_algorithm.h for q6212's for_each binders; 0x00516D90 onto engine/CArray.h), 4 in-place respellings inside
existing local models (q5205 binders; 0x00686240 keeps its pre-existing strong same-TU copy of 0x0040F7E0, respelled in place), and 2 self
re-lands: 0x0041CC14 / 0x0041CC2B (q4769) are now the header's inline static getters GetBackBufferWidth / GetBackBufferHeight
(`FABLE_RETAIL_CUISCREENUNITCONVERTER_ORIGINAL_DIMENSIONS`), replacing an invented free CTCLook stand-in that read invented globals.
- **CUIScreenUnitConverter statics are private (`@@0`): two sources.** Ego_d PUB32 (NF112) and FableWin.pdb's class record
  (`LF_STMEMBER IsUsingRelativeCoords / BackBufferWidth / BackBufferHeight / DefaultBackBufferWidth / DefaultBackBufferHeight, attrs = private`,
  checked by the coordinator). This settles the HANDOFF "chunk 733" access question.
- **Flags carried into CompilerFlags:** 0x0041CC14, 0x0041CC2B `/Os` (the old rows used `#pragma optimize("s",on)`).

## Checks
bindings_multi_check 1 (NullCoord); funclet_includes_check 1355 / 0; no header changed. Exe-scan referencing rows: 52 good, 1 pre-existing
DIFFER. No-new-split gate (before set compiled from HEAD at HEAD flags): 0 created. Existing splits left: the five statics are still bound
`@@2` (and as ScreenWidth / ScreenHeight / g_dimW / g_dimH) by older rows: queued for respelling to `@@0` now that the access is settled.
