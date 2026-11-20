# NF167 notes (TC component naming, CTC A-H), 2026-11-20

## Counts
- Entries (25): done 11 (q4151 q4152 q4475 q5043 q5093 q5124 q5125 q6155 q11690 q11700 q11926); done-already 3 (q4717 q4947 q5098);
  deferred 2 (q6190: self row 0x0055F710 held by acctB; q11987: self row 0x006E11F0 held by NF166); blocked 9 (q4949 q4988 q11669
  q11687 q11689 q11702 q11768 q11985 q11986); rejected 0 as entries (REJECTED.tsv records the public access letter of the q4151 / q4152 claims); open 0.
- Rows (STATUS.tsv, 72 lines): ported 23 (all in rows/; recheck_final.log: 23/23 RELOCATION_MATCH + PASS); not-a-binder 19; blocked 30
  (22 of them are the q11669 mode family).

## Decisions for the coordinator
1. **Access letter, q4151 / q4152.** The queue claims `@@2MB` / `@@2JB` (public static). Ego_d.pdb PUB32 mangles both as private
   `@@0MB` / `@@0JB`, the shared headers declare them private, and only members read them. I respelt them to the private spelling
   (rows 0x0093A8C0, 0x0121F260, 0x0073A760, 0x0121F7E0) and recorded the public claim in REJECTED.tsv. Please confirm the private letter.
2. **Needs header claim (q11687).** The shared engine/CTCHeroRangedAttacks.h inline `GetShotBuildupTime` body (`double time = AimingTimer;
   return time / fps;`, used by landed 0x0078A390) has a local that Ego_d does not list. Changing an existing shared-header member body
   is a coordinated header item, so I did not apply it. It unblocks nothing by itself (0x0078A390 already matches); it is a fidelity item.
3. **0x007E2A60** probes DIFFER (707v707) at HEAD with its catalog flags (seen in the q4717 probe; its binding is right; not changed).

## Casts and model changes the coordinator should know about
- No stand-in type was cast to a real class. 0x008A7F60 keeps HEAD's `static_cast<...*>(void*)` on the component-map value; it now targets
  the real `CTCBob` (engine/CTCBob.h) instead of the row-local stand-in `CTCBobNative`. The stand-in struct is deleted, not renamed.
- In-place respells inside pre-existing local models (no new class token): 0x0121F260, 0x0073A760, 0x0073AC50, 0x0078A5D0, 0x0077A130,
  0x00836260, 0x007E4D70, 0x007E7580, 0x007E00D0. For 0x007E00D0, the row's own local `CWorldMap` model gained ten dummy virtual slots
  plus `IsMapLoaded` at slot 10, and the CRegionManager stand-in struct was removed. No vtable is emitted, and the defined symbols are
  identical to HEAD. Also seen there (not changed, not an entry): 0x00686D30 is still bound as the stand-in `CTCTeleporter::GetWorldMap`.
  The fix is in NAME_FIXES.tsv.

## Self renames (file moves at landing)
- 0x006A7550: `std::_Cons_val<...>` -> `CThingFilter_EnemyCreature::operator()(const CThing&) const` (row + test rewritten on the shared
  headers; the test now links the row object). Twin 0x006A7560 (the const-pointer overload) is in NAME_FIXES.tsv; it is not one of my rows.
- 0x0057A690: leaf only, the file name stays `CTCHeroStats_GetMoralityPercentage` (row + test on engine/CTCHeroStats.h).
- 0x00789FD0: leaf only (ContinueAimingRangedWeapon, member of CTCHeroRangedAttacks).
- 0x0073AC50: QAE -> UAE (virtual).
- 0x0077A130: QAE PAV -> AAE AAV (private, CThing&).
- None of these has an EH funclet row (grep of rebuild/src for parent references: none).

## Header opt-ins (hdr/README.md)
- CTCDrunkenness.h `FABLE_RETAIL_CTCDRUNKENNESS_DRANKFRAMES`;
- CTCCarryable.h `FABLE_RETAIL_CTCCARRYABLE_GETCARRYSLOTPASSIVE`;
- CTCEnemy.h `FABLE_RETAIL_CTCENEMY_ARETHINGSENEMIES`;
- CTCCreatureOpinionOfHero.h `FABLE_RETAIL_CTCCREATUREOPINIONOFHERO_CLEARPENDINGDEEDS`.

All four are guarded opt-ins. pin_asserts was re-run after each one. No landed includer is affected.

## Name splits (callers.py over reloc_bindings, 2026-11-20)
No new split. Two self renames leave a split that already existed at HEAD and make it narrower:
- 0x006A7550: at HEAD it was bound three ways: self `?Cons_val@@...`, 0x007F4020 ABV, and 0x007F6DF0 `QBE_NPAVCThing`. After this hand-back
  the self row and 0x007F4020 bind ABV. 0x007F6DF0 still binds the wrong PAV spelling (NAME_FIXES).
- 0x0057A690: at HEAD it was bound three ways: self `YIM` free function, 0x0057A6B8 QBE, and 0x0057C2E6 QAE. After this hand-back the self
  row and 0x0057A6B8 bind QBE. 0x0057C2E6 still binds the non-const spelling (NAME_FIXES).
- 0x0073AC50: no landed row binds the old QAE spelling. 0x00789FD0: no landed binder. 0x0077A130: see RENAMES evidence.

## Blocked (rows left)
- q4949 / q4988 AreThingsEnemies, 2 rows left:
  - 0x0064A1AE: 2 forms failed, both C2953 / C2440. Its local CThing has no CBaseObject base, and re-basing it would be a new model.
    Draft: work/0064a1ae.cpp.
  - 0x00807B70: whole-TU namespace N00807b70.
- q11669: 22 mode-row binders (family; list in APPLIED.tsv). Each models the mode hierarchy locally, so this needs a family port onto
  engine/CTCCreatureModeManager.h. The self row is done.
- q11687: 0x00899C10, a legacy free function; needs a self rename + port.
- q11689: the self row 0x00789E60, a stand-in CTCHeroRangedAttacksMethods. This is a pre-existing split; the binders are now on the PDB spelling.
- q11702: 0x008A0720 (CTCFadeOutAndIn_Retail stand-in).
- q11768: 0x0089A070 / 0x0089A0E0. Both reach RELOCATION_MATCH on CTCHeroStats.h + CVectorMap.h (drafts in work/), but their tests mock
  stand-in classes and LINK_FAIL; they need the tests rewritten. The other REFS rows were not re-probed.
- q11985 / q11986: 0x008F7BD0, whole-TU namespace NFoot008f7bd0, not attempted.

## Names
- No descriptive (made-up) names were introduced. All names come from PDB spellings with two sources.

## Rule slips
- I ran HEAD-source diagnostic grades (tools/hd.sh into pH) beside the real grades. They are counted in grades_used.
- The first q6155 probe silently skipped 0x00774D20 (probe.py resume). It was re-probed: the binding is right.
- 0x0064A1AE was marked blocked after 2 forms, not 3. A third form would need a new local CThing base, which is not allowed.
- q11768: the other REFS rows were not re-probed.
