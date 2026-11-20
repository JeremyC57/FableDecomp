# 2026-11-20 landing nf109: GameCODA follow-ups, the CFloatDiffuser family and the healed DSP splits (18 rows)

Records: `naming/2026-11-20_agentNF109/` (selfcheck 0 problems). 18 entries: 15 done, 1 blocked, 2 open.
- Done, self rows ported (q11600..q11606): 0x00C74CE0, 0x00C74D30, 0x00C756C0, 0x00C75A20, 0x00C75AB0, 0x00C76E90, 0x00C770B0 move to their
  GameCODA 2.10 CodeView spellings; their outside binders (NF105 rows) already bound those, so the existing splits are healed.
- Done, q11607 (0x00C75000 `?Release@CFloatDiffuser@@QAEXXZ`): an 11-row family moved together: the self row, in-place member respellings in
  0x00C6A3D0 / 0x00C6A780 / 0x00C6A8B0 / 0x00C6A650 (existing local models, advisor rule), and the CFloatDiffuser / CFloatAllpassSection ctor,
  dtor and `??_G` rows (0x00C75410, 0x00C75440, 0x00C754A0, 0x00C74F60, 0x00C75400, 0x00C75470).
- Done, catalog-only (q11593..q11599): parked rows, two sources re-verified; the manifest names are left for Account B to apply at authoring.
- Blocked q11517 (`m_pDSP`, closure of 33 rows); open q11584 (SetReflectionsPan / SetReverbPan / CSalSwReverb::Update findings queued
  q11618..q11620) and q11559 (folded dtor: needs the ctor or RTTI as the discriminating source).

## Rows (18): 1 MATCH + 17 RELOCATION_MATCH, every test PASS; 19 EH funclets of 0x00C6A780 / 0x00C6A650 RELOCATION_MATCH PASS (`logs/recheck.log`)
Headers: 7 add-only `FABLE_RETAIL_*` opt-ins in CFloatFilterParams.h, CFloatLongReverb.h, CFloatLongReverbModule.h, CFloatAllpassSection.h,
CFloatDiffuser.h (`naming/.../hdr/README.md`); no landed includer defines them.

## Checks
assert_names_check 0; bindings_multi_check 1 (NullCoord); funclet_includes_check 1355 / 0. Includers + renamed + exe-scan referencing rows: 53 good,
1 funclet graded without its offset (good with it). No-new-split gate (before set compiled from HEAD; self old leaf from the HEAD Leaf line,
else now the HEAD object's defined symbol): 0 created. 0x0143CCDC stays an existing split (q11517).
