# Anniversary naming wave (2026-11-04)

Source analysis: `docs/re_reference/FABLE_ANNIVERSARY.md`. `candidate_names.tsv` / `vt_confirmations.tsv` come from the static survey of
the owner-supplied Fable Anniversary exe; `tools/` holds the survey scripts and `anwave.py` (this wave).

## Verification rule (owner: "verify as best you can any names you use from anniversary")
An Anniversary-derived name lands only if all hold:
1. The Anniversary RTTI match (string literals of the vtable's own slot functions, validated 257/258 on named TLC vtables) names this
   TLC vtable.
2. Our own PDB name sets (FableWin.pdb + Ego_r.pdb) contain that exact class with its nesting AND the method assigned to the slot
   (`??_G` / `??1` for slot 0).
3. The slot layout agrees with TLC classes already named by other evidence: def classes 0 sdd / 18 Transfer / 19 Copy / 20
   GetSizeofClass (RTTI CControlsDef, CFrontEndDef, NUISystem::CUIIconsDef ...; GetSizeofClass bodies checked as `mov eax,imm32; ret`);
   level scripts 0 sdd / 1 RegisterMain / 2 Main / 3 Init (landed CV_HeroDuelScript); entity scripts 0 sdd / 1 Main / 2 Init /
   3 GetParentScript / 4 OnPersist / 5 OnPredicateFail (landed CV_MurderTwistFinalScript sheriff + guard).
4. The function address is referenced by exactly one dword in the exe (never a shared base-class slot).
The survey's own slot counts ran past the end of each vtable into the next one; they are not used.

## High-confidence wave (applied)
- 17 classes, `labels_high.tsv` = `ghidra_out/labels_anniversary_2026-11-04_high.tsv`, 63 function names; report
  `logs/wave_high_report.txt` (every slot with its checks; shared slots skipped). `apply_labels.py`: 45 catalog and 53 manifest
  renames (`logs/apply_labels_high.log`); 10 of the 63 are functions not yet in the manifest.
- Every Transfer / RegisterMain / Main that already carried a class name agreed with the Anniversary class (e.g. 0x00F261F0
  CArenaWaveDef::Transfer, 0x00DDDE20 CQ_PrisonWardenGameScript::Main, 0x00EE0350 CTH_SnowTroll::OnPredicateFail).
- Two propagated labels were wrong: 0x00EE36A0 "CStopDrawing::Main" is CV_TravellingHeroesScript::CTH_SnowTroll::Main (referenced
  only by that vtable, whose slot 5 is already CTH_SnowTroll::OnPredicateFail); 0x00DCDED0 "CGuardTeamMember::OnPredicateFail" is
  CCrateTeamMember's (refs=1, identical body).
- 45 of the 63 are landed rows whose sources still spell the old placeholder names: queued PRIORITY `land0411k` in
  `naming/NAME_FIXES_QUEUE.tsv` (re-model onto the nested classes, respell callers).

## Confirmations
`vt_confirmations.tsv`: 667 RTTI-less TLC vtables whose existing catalog class the Anniversary RTTI independently confirms. They are a
second source for those class names under the naming rule; nothing to apply.

## Next (agents running when this was written)
- Medium (220, CSpawnedFunc arity) + low (44, catalog disagreements, FireHeart first) under the same rule: `$S/anniv/wave2/`.
- Instead of a BSim/Version Tracking diff for the ~690 remaining placeholders: TLC-internal naming from the quest scripts' entity
  registrations (name string + ClassAllocFunc -> Alloc -> ctor -> vtable, checked against the PDB's `?Alloc@<Inner>@<Outer>@NScript@@`),
  with Anniversary as a cross-check only: `$S/anniv/wave3/`.

## Recheck of the high-confidence wave (owner question, 2026-11-04)
Wave 2 showed the order matcher misnamed neighbouring vtables, and the high-wave check (PDB method for the class + slot layout +
refs=1) cannot tell sibling entity classes apart, so every high-wave class is being re-proven from TLC alone:
- **Def classes (5): confirmed.** The field-name strings each class's Transfer persists match that class's Ego_r members and no
  other class: CArenaWaveDef 3/3 (next best 2), CFeatDef 15/15 (1), CHeroSoulsCreatureDef 3/3 (2), CRegionScriptDef 4/4 (0),
  CScriptDef 92/92 (0).
- **Script classes (12):** re-proven through the parent script's entity registration (name string -> Alloc -> vtable) or the
  script registry for the 3 level scripts; result in `$S/anniv/wave3/recheck_high.tsv`, applied or reverted with wave 3.
