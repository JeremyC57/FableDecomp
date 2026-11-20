# 2026-11-20 landing nf114: CUIScreenUnitConverter private statics, first hand-back (8 rows)

Records: `naming/2026-11-20_agentNF114/` (selfcheck 0 problems). 18 entries: 3 done (q11634 q11637 q11638: IsUsingRelativeCoords and the two
default dimensions now bind only `@@0`), 4 blocked (q4447 port after 3 forms, DIFFER 423v423 on one register swap in the C2DBoxI copy;
q6212 authoring, the Leaf-less 0x00644627; q11635 / q11636 authoring: only 0x0040F0E0 ResetDimensions still writes the invented g_dimW / g_dimH,
and it must be re-authored as the header's `?ResetDimensions@CUIScreenUnitConverter@NUISystem@@SIXXZ` member), 11 open (probe in work/probe3.tsv:
q4449 11 of 55 binders wrong, q4524 17 of 73, q4452 5 of 120).

## Rows (8): 8 RELOCATION_MATCH, every test PASS (`logs/recheck.log`); no self rename
- 0x00531EC0, 0x0060EEB0, 0x0061E6D0: `..._INLINE` -> `FABLE_RETAIL_CUISCREENUNITCONVERTER_ORIGINAL_DIMENSIONS` (they read the statics only inside
  the class's inline members).
- 0x004299A8, 0x0052E530, 0x0052E580, 0x0052F5C0: in-place respelling inside their existing local models (private statics; ScreenWidth /
  ScreenHeight -> DefaultBackBufferWidth / Height; 0x0052F5C0's invented ConvertWidth / ConvertHeight -> the PDB ConvertX / ConvertY).
- 0x00543EE0: the invented g_bScreenSizeOverride / g_Override* / g_Display* externs replaced by the header's public inline ConvertX / ConvertY.
- Header: engine/NUISystem_CUIScreenUnitConverter.h, no new macro: `ORIGINAL_DIMENSIONS` arms added to the inline ConvertX / ConvertY bodies of the
  existing CONVERTXY_INLINE / VECTOR_INLINE_CONVERTXY branches; default path token-identical, pin_asserts 0, assert_names_check 0.
- Needs a header claim (not applied): the header's public `@@2` INLINE block; 7 landed rows still define INLINE but none references the data.

## Checks
bindings_multi_check 1 (NullCoord); funclet_includes_check 1355 / 0. Includers + exe-scan referencing rows: 35 good / 0 bad. No-new-split gate:
0 created; table cross-check: the five statics go from 3-4 spellings to 1 (0x013B8768, 0x01375CD4, 0x01375CD8) or 2 (0x013B876C / 0x013B8770:
only 0x0040F0E0's g_dimW / g_dimH left). The gate's section-2 parser now accepts callers without `0x` (GOTCHAS 2026-11-20; nf111 follow-up NF117).
