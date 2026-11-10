# 2026-11-10 landing bd -- agentNF6: /Zc:wchar_t removed from landed rows, Codex pass 19 / NF5 name-fix queue

Source: `naming/2026-11-10_agentNF6/` (QUEUE / STATUS / NOTES / LESSONS / RENAMES / NAME_FIXES).
203 landed rows rechecked (vland --recheck-landed): 203 wins, tests PASS (0x00891720 prints its own PassPattern
`CHANGE_HERO_HEALTH_BY_PASS`; the recheck harness looks for `<addr>_TEST PASS`, so it shows FAIL there only).
- Job 1, owner wide-text rule (2026-10-29): `/Zc:wchar_t` is gone from every landed row. 12 rows by NF6; the three it could not touch
  (0x0099BA70, 0x00A0CDD0, 0x007E5570) were done by the coordinator (flag dropped, 0x0099BA70 leaf `PB_W` -> `PBG` and its
  FABLE_RETAIL_STLP_NATIVE_WCHAR_TRAITS define removed). Byte-neutral throughout; build_candidates.ps1 CompilerFlags updated (8 rows).
  A `/`-prefixed note on a `// Flags:` line is parsed as a flag: notes moved off those lines.
- Job 2, queue land1011ba/bb (149): 37 applied, 112 partly (verified; binders left per STATUS.tsv), 0 rejected; 188 rows respelled across
  31 target symbols. 0x00662000 renamed CThingCreatureBase::GetCreatureType (was GetMovementMode).
- Job 3 (large binder groups) not reached; ranked in NOTES.md. Left for rewrite: 0x006622A0 IsPerformingAnyAction (`mov eax,1` bool),
  0x007AED10 GetBossPhase (engine/CTCBoss.h hangs cl.exe), ~25 self-misspelled rows on private models.
reloc_bindings.tsv: rows' bindings recomputed (rebind.py); bindings_multi_check at baseline. Queued land1011bd: 2 new fixes.
