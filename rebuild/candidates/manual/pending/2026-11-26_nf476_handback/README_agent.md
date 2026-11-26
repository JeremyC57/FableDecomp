# NF476 hand-back (2026-11-26): MinionReactToAlert lands with the whole T3 nf374 0x008FCEC0 line

No repo writes, no git, no `wineserver -k`. Base: the working tree as read (HEAD plus NF475's landing); every `orig/` file is cmp-equal to the tree.

## Land order
1. **batches/b0_hdr** (3 headers, add-only): MinionFaceHero, BanditKingLunge and BanditSitRoundCampfire.
   - Each gets a nested `FABLE_RETAIL_CAISTATEGROUP_<X>_INITIALISE` slot-1 declaration in the PDB spelling. The first two have an `#error` without their view; BanditSitRoundCampfire has no flat branch, so no `#error` is possible.
   - The 3 moved size asserts are pinned to their original lines. `g++ -E -P` is identical for all 11 HEAD includers (evidence/hdr_equiv.txt).
2. **batches/b1** (7 rows, 1 unit, all ACCEPT). Grade with `FABLE_HDR_OVERLAY=<b0_hdr/hdr>`, or after b0_hdr lands. `ov_src/` holds the ctor text the thin row includes, which is NF475's bA text unchanged.
   - **MinionReactToAlert:** NF475's 3 rows (0x008FD50D ctor, 0x008FF19C thin `??_G`, 0x0090F0D0 slot 1), text unchanged.
   - **The other binders on the nf374 line:** 0x00902045 MinionFaceHero, 0x0090F690 MinionSitOnSeat and 0x009612E0 BanditKingLunge.
   - **The other long-priority emitter:** 0x0093A500 BanditSitRoundCampfire.
   - Each binder's self is respelt to its PDB `...JW4EScriptAIPriority@@...` virtual on the class header's retail view, so each one binds the landed base self. The local models of headered classes are gone, there is no new `reinterpret_cast`, and there is no `__asm`.
   - **Apply:** SELF_SYMBOLS (7; 0x0090F690 is a new line), CATALOG_CHANGES / CATALOG_MANGLED (the 0x008FF19C relabel), TABLE_CHANGES (2 delete, 2 rewitness).
   - **Replaces NF475 bA's TABLE_CHANGES line for this unit.** Apply b1's table, not bA's. The UAE..JJ line is rewitnessed to 0x00957940, which stays an emitter whatever happens to b1_hold.
3. **batches/b1_hold** (2 rows, HOLD census BAD): 0x0091D820 and 0x009705D0. Both ported and gated with b1 (RELOCATION_MATCH PASS on both sides, objproof ALL_EQ), but other landed rows bind stand-ins of their selves:
   - **0x0091D820:** 0x0095A890 binds `?Initialise@CAIStateGroup_EnemyCreatureBase@@QAE..JJ..@Z`, and 0x0096A250 binds `?base_init@@YIXPAUC@@@Z`.
   - **0x009705D0:** the dtor row 0x00902F71 emits an 8-byte local `??_7CAIStateGroup_CreatureUseCoverBase@@6B@` with `??_E` in slot 1, where retail's slot 1 is 0x009705D0.
   - These are HEAD debt. Each row lands once its binders are respelt.

## Gates (b1)
- **vland HEAD v new** (run on 9 rows, the 2 b1_hold rows included): 9 / 9 RELOCATION_MATCH PASS on both sides, tests included. WARN 22 → 1; the 1 is STANDIN_LEAF on the thin row and goes away with the relabel.
- **objproof** (evidence/objproof.txt):
  - The 6 respelt rows other than 0x00902045 are ALL_EQ under their 2 renames.
  - **0x00902045:** the leaf section's bytes are equal and every relocation has the same offset and type (evidence/leafproof_00902045.txt). Its pick-any COMDATs are now the template's:
    - `??0?$CIntelligentPointer@$$CBVCThingPlayerCreature@@@@QAE@PBV..@Z` is EQ at its landed owner 0x006892A0 (pk_all);
    - `??_G?$...` is EQ at 0x0048A4D0 (checked by hand; that address has no row);
    - the rest are NOADDR.
  - **0x008FD50D / 0x008FF19C:** the expected full-table change. slotchk: class_bad=0, and the only misspelt slot is slot 0, which this unit's relabel fixes.
  - `.drectve` is identical on all 18 objects (post-`_USE_ANSI_CPP`, both sides compiled after the change).
- **0x00902045 row-end `#pragma optimize("", on)`** (R-NF409-2, marked `Fidelity: scaffolding, not retail`).
  - The row's `optimize("y", off)` would otherwise give the template-instance ctor copy a frame pointer, so it would be NEQ at 0x006892A0. With the reset it is EQ.
  - The leaf stays RELOCATION_MATCH.
  - The owner row has no CompilerFlags override and no size pragma.
- **Link** (8 objects without the thin row, plus HEAD objects of 0x008FCEC0, 0x0090F1C0 and the 10 base slot targets):
  - duplicates 0 → 0;
  - unresolved names 9 → 5. The one new name is `?IsRunnable@CAIStateGroup_MinionReactToAlert@@UAE_NXZ`, whose target 0x0090F1B0 has no row.
- **Census:** evidence/fullcensus.tsv, 0 BAD for the 7 rows. Inputs in evidence/census_init.tsv: every landed retail caller of 0x008FCEC0 (38 code sites) plus every vftable witness of the classes touched, compiled from HEAD.
  - All their tables are 4 bytes (no slot 1), so no table names a respelt self.
  - Remaining stand-in emitters: QAE..JJ in 0x00902233, 0x00911F20, 0x00921270 and 0x00953FE0; UAE..JJ in 0x00957940 and 0x00957BB0.
  - The CAIStateGroupBaseI and `@CAIStateGroupBase_Initialise@28` lines had only b1 emitters (census plus source grep), so they are deleted.
  - The 2 held rows have BAD lines: see b1_hold.
- **Sandbox** (overlay):
  - apply_table --dry: 0 misses;
  - bindings_multi 1 → 1;
  - descriptive_check: 2030, 0 FAIL;
  - s_unit_check: 205, 0 failed;
  - the repo table is unchanged.
- **Tracker dry run** (measured with the 2 b1_hold rows in; without them it is 6 rows and 0x0091D820 unchanged): 7 rows move rtti_identified → agree_pdb. 0x008FCEC0 (7 → 5 names) and 0x0091D820 (4 → 3 names) stay split_fixable with fewer names. Nothing gets worse.

## Not done (rows untouched; locks restored)
- **IdleAtHome** (item 2): not started. Binders 0x00917330 and 0x0090FC50 need CThingAICreature.h / CAIBrain.h views and a `GetPHome` opt-in; 0x0090FC50 also has local CTCVillage / CThingCreatureHome / base models. Still HOLD on nf305 line 1339.
- **bH_hold's 4 units** (item 3): not started; NF475's bH_hold is unchanged. HOLD r467b.
- **MinionWander / CreatureGoUnderground** (the coordinator's add-on): not started. The locks are back to COORD_HOLD_NF473.

## Notes
- **0x00902045's pick-any record:** b1 evidence/pk/pk_00902045_noreset.txt (without the reset, the ctor copy is NEQ at 0x006892A0) and pk/pk_00902045.txt (with it, EQ).
  - The reset also changes `??4` (29 → 26 bytes) and `SetPointer` (9 → 5 bytes). Both are NOADDR: no retail address or row was found.
  - The reset's cover under R-NF409-2 for those two NOADDR copies is a ruling request (r476a).
- **Comment edits after vland:**
  - The headers' three asserts were pinned after vland ran. The three header rows were recompiled against the final b0_hdr, and their object summaries equal the vland objects.
  - 0x00902045's evidence pointer comment was edited after vland. It is a comment only; the recompile above covers it.
- **Tests:** all HEAD tests unchanged; they PASS in the new payload. 0x009612E0's test is a standalone g++ model and still spells the old free stand-ins.
- **Claims:** COORDINATION_CLAIMS.tsv has one hit only, NF429's stale CAIStateGroup_* family claim (r475 note). T3: only nf374 line 3446 (was 3451) names b1 rows, and b1 resolves it.
