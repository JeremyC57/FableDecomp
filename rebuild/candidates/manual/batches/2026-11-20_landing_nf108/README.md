# 2026-11-20 landing nf108: PRIORITY table slice 2, first hand-back (52 rows)

Records: `naming/2026-11-20_agentNF108/` (selfcheck 0 problems). 40 entries: 20 done (q4185 q4250 q4258 q4265 q4406 q4434 q4448 q4487 q4648
q4701 q4702 q4738 q4759 q4760 q5046 q5048 q5099 q5150 q6236 q6324), 1 rejected (q4376: retail 0x0051AC70 is __fastcall, not YA; CMapDesc is
the nested CStaticMapGenerationVisibiilityInfo::CMapDesc), 3 blocked (q4453 0x00BE7D70 Leaf-less row; q4526 0x006A4B80; q4528 0x00931010 after
3 failed forms), 16 open (handed to NF112).

## Rows (52): 4 MATCH + 48 RELOCATION_MATCH, every test PASS (`logs/recheck.log`)
21 self renames (RENAMES_applied.tsv) and 54 caller respellings; 0x00502850 (CacheMaps) also moved to its protected PDB spelling so no
referencing row is left on an old spelling. 0x00890AB0 / 0x00890B00 pass `(void*)0xBEEFBEEF` / `(void*)0xCABBA9E5`: retail key immediates,
not cast addresses (NOTES).
- **Flags carried into build_candidates.ps1 CompilerFlags** (the rows' `// Flags:` lines; without them they grade DIFFER):
  0x004DA2AC, 0x004DB91F `/Os`; 0x00A92740, 0x00A91180, 0x00B2C8E0 `/d2inlS100 /d2inlT200`.
- Headers: 6 add-only opt-ins in CQuestManager.h, CWorld.h, NGraphicAppearance_CTCGraphicAppearance.h, CTCScriptedControl.h,
  CBulletTimeManager.h (`naming/.../hdr/README.md`); pin_asserts pinned 7 asserts in CBulletTimeManager.h and 1 in CTCScriptedControl.h;
  assert_names_check 0 mismatches.

## Checks
bindings_multi_check 1 (NullCoord); funclet_includes_check 1355 / 0. Includers + renamed + exe-scan referencing rows: 1411 rechecked, 1409
good, 2 pre-existing DIFFER whose sources and includes this landing does not touch (0x007E2A60, the known on-hold row; 0x00BE7D70,
FINDINGS_LOG). No-new-split gate (before set compiled from HEAD at HEAD flags): 0 created.
