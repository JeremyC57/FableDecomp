# b27 audit 0x00D8A240 WatchForBooty
- Identity: FableWin.pdb 0x037BA020 + Ego_r.pdb 0x00DAF540 (register_scripts.obj) ?WatchForBooty@CQ_HerosOldHouseScript@NScript@@QAEXXZ; labels accepted; Main 0x00D89E60 spawns "WatchForBooty" -> 0x00D8A240. Ego_d types.txt lists the method.
- Ego_d locals (egod_locals.py 'NScript::CQ_HerosOldHouseScript::WatchForBooty', debug len 833): this, HiddenBooty (CScriptThing 0xBCD4) bprel -28, _Fail_Val (TRetVoid 0xFA51) bprel -13.
  -> README claim "Ego_d.pdb has no record" is WRONG (they queried without NScript::). Source uses `booty` + TRetVoid() temporaries: locals mismatch.
  -> `TRetVoid _Fail_Val;` + `return _Fail_Val.Return();` never tried (no hit in NOTES/attempts/variants). GOTCHAS 138/311/312 say it changes exit layout.
- Precedent: 174 d2inl flags in build_candidates.ps1; d2inlT57 already landed on NScriptCQHeroSoulsArenaScript_WatchForRoundsInArena_00d7fe90.
T0..70 sweep on v1: only T57..T63 RELOCATION_MATCH (sweep_v1.txt)
- Grades (qa.py = q.py copy reading b27 oracle.tsv, same bytes as $S/batch/hand2c_oracle.tsv; work dir in audit): b27 src T57 RM, T56 DIFFER(561), T63 RM, T64 DIFFER(655), none DIFFER(662).
  v1 (Ego_d locals) identical window; v2 (final return via _Fail_Val) also RM at T57.
- No-flag vs retail: first 128 insns identical; diff only in return tails (3 vs 2 `dec [eax]` release copies, jmp vs call) = inline decision.
- Ego_d decl order check: 0x00D58490 landed `TRetVoid _Fail_Val;` first, Ego_d bprel -13 (highest) -> top placement confirmed.
- Overlay: additive (WATCHFORBOOTY opt-in decl only) + 3 asserts pinned 74/75/77; hdrcheck2 default=same[0] NSCRIPT=same[475].
- Includers 0x00D8BCD0 / 0x00D8ABF0 (no flags): RELOCATION_MATCH with overlay and without.
- check_portable_spellings clean; no asm/volatile/macros in src.
- Fixed: test PASS, strict vland RM PASS, mutants 33 KILLED / 1 equivalent SURVIVED / 1 LINK_FAIL.
