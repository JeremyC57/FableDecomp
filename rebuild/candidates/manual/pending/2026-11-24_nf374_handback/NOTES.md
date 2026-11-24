# NF374: T1 binder respells (Account A), hand-back notes

306 targets (TARGETS.tsv; fix kinds: other 209, signature 62, standin_alias 35, access 0; the brief said 39 / 6).
417 binder lines (work/dm.tsv). Outcome per line: 33 done, 222 port, 160 held, 2 identity.
Per target (STATUS.tsv): done 14, done-partial 12, skipped-port 157, skipped-held 121, skipped-identity 2.
Status is computed from the table after every proposed change (work/all_tc.tsv, applied in memory; tools/status374.py). A target is `done` only when no non-PDB spelling is left at it.
Compiles: about 126 (budget about 120; stopped compiling at the limit). Nothing was written to the repo; no git was run.

## Batches

### batches/b1: 28 ACCEPT binder rows (land_scripted form)
004671ca 0046abd6 006c88d0 006e1620 00703590 00733910 00739550 007864c0 00799e30 007bb150 007e1e30 007f90b0 007fdeb0 0082d080
00835c20 0089f4a0 009224f0 00922750 0093d800 00950950 0095d200 009c5650 00a25d20 00a99510 00ab69f0 00cb8960 007e7760 00714d30

Each row passed every gate:
- vland HEAD and new grade MATCH or RELOCATION_MATCH with the test PASS, at one LANDVERIFY_WORK path. The grade lines are in logs/.
- Claims: no ACCEPT row is in T3_PORT_QUEUE or in an open COORDINATION_CLAIMS line. Simulated with land_scripted's own newest-line rule.
- objsec374 (rename-aware): ALL_EQ except the intended renames (evidence/objsec/<a>.renames.txt). `.drectve` is byte-equal and the COMDAT sets are the same.
- pk_all and pickany_vs_retail: HEAD equals new under the renames (evidence/pk_gate, evidence/pkdiff_all.txt: 35 of 35 SAME, b1 plus b1_hold). reachflip: 35 NEQ pairs, 0 flips (evidence/reachflip_all.txt).
- /FORCE link of the binders plus the HEAD callee rows (../../evidence/link_b1.txt, link_b2.txt): 0 duplicates, and nothing HEAD resolved becomes unresolved.
  Three names stay unresolved after, each only on the PDB name with no landed owner row (NF302 Q2):
  - DrawAddWeaponTrails (0x0076E6A0)
  - CThingPhysical::DrawPreparePrimitivesForRendering UAE (0x006A64D0; this one was already unresolved at HEAD)
  - SaveVerts<CWaterFanBaseVertex>, but 00bfc770 is in b1_hold, not b1.
- Names: every new spelling is in the Ego_r and FableWin name sets, or is the self row's Leaf. Sources are in each RESULTS reason.

Read each row's RESULTS reason for its rulings. These rows rely on a ruling:

| Row | Ruling or rule |
|---|---|
| 0082d080 | NF299 Q3: virtual on the local model, no vftable emitted |
| 00714d30 | NF300 Q2: nested re-scope |
| 007e7760 | Nested forward declaration only |
| 009224f0, 00922750, 00950950, 007bb150 | NF300 Q1: enum header, with the literal written as its enumerator. 009224f0's literal 4 is AI_ACTION_IN_PROGRESS; the old local name FINISHED=4 contradicted the PDB enum, where FINISHED=2 |
| 007bb150 | NF304 Q6: the own self also moves to its PDB spelling (private, EOpinionDeedType). No landed row binds the old self |

**TABLE_CHANGES.tsv** (21 lines: delete 9, replace 8, add 4)
- `apply_table.py --dry`: 0 misses, multi-target before 1 and after 1, no new multi-target symbol (../../evidence/apply_table_dry.txt).
- Every new spelling has exactly one table target after the changes.

**HAND_TABLE_CHANGES.tsv** (7 rewitness lines: apply by hand)
- land_scripted keeps only lines whose witness is a landed run row.
- Each new witness's HEAD object has the old symbol as UND (../../evidence/rewitness_proof_b1.txt):
  0046b0d6 00786700 00752640 007fdf60 0052ac10 00950c30 00a9b740.
- Dry run: 0 misses. The combined dry run (b1 + hand + table_only, work/all_tc.tsv) also has 0 misses and no new multi-target symbol.

**Lines kept on purpose.** Six old lines stay. Each one's witness moves to the PDB name when b1 lands, so each becomes a stale-witness line (ruling 3). Another row still binding the old form is proven for none of them:

| Binder | Old line kept | Why |
|---|---|---|
| 00733910 | GetCombatAnimationSet QAE | 00733950 is T3 and also declares it; not compiled |
| 00739550 | SetPItem PBX | 00740450 compiled and does NOT bind it; other textual declarers unverified |
| 009224f0 | CAIStateGroupBase::AIAction_DoNothing QAEJ | 0090eec0 compiled and does not bind it; other declarers unverified |
| 00ab69f0 | UpdateEvents | 00ab6e40 declares it (unverified); the new UpdateKeyEvents line is added |
| 007e7760 / 00714d30 | Struct and NTCScriptedControl forms of AddAction | 00903ca0 / 00904cd0 (T3) and siblings declare them; the new nested form is added |

A coordinator census (`ref_scan.py`) can turn these into deletes or rewitnesses later. For 00739550 and 009224f0, the only candidate compiled did not bind the old form.

**Proven, unattempted binders of old spellings.** These seven rows are invisible in TARGETS, because the table keeps one witness per pair. Each one's HEAD object has the old symbol as UND, which is why HAND_TABLE_CHANGES rewitnesses the old line to them:
- 0046b0d6 (CDefString::TransferBinaryIn by CMemoryDataInputStream&)
- 00786700 (C3DSkeletalMorphParameter ctor by pointer)
- 00752640 (CAnimationSet::GetAnimation)
- 007fdf60 (SetPItem(CThing*))
- 0052ac10 (CThingPhysical(long))
- 00950c30 (CAIStateGroup_JackDragon::AIAction_DoNothing)
- 00a9b740 (non-virtual ~CTemporaryBuffer)

Their targets therefore stay `done-partial` (00a455a0, 005dc340, 00a98910, and others). They are candidates for a next T1 pass. None is in T3, NF373's set or a claim.

### batches/table_only: stale table lines (no source change)
These HEAD objects already bind the PDB form; the table line names an old or wrong spelling.

| Witness | Change | Note |
|---|---|---|
| 004d297b | Delete `??0CTCActivationTrigger@@QAE@PAUCThing@@@Z -> 00723fd0` | 00723fd0 is the CTCPhysicsStandard ctor. Its PDB line exists, witnessed by 00724f50 |
| 004de9a1 | Delete plain `?Transfer@CPersistContext@@QAEXPBDAAJABJ@Z -> 00410be0` | The object binds the template `??$Transfer@J@...`, already a line, witnessed by 00d87c60. The plain name is only its /alternatename fallback |
| 00ae2cb0 | Replace ApplyDragging onto the PDB form (AAE, const&) | |
| 00ae2cb0 | Replace ApplyForces at 00adf760 onto the PDB form | Same witness and proof. Not a T1 target; offered alongside |

Dry run: 0 misses. Apply by hand (`apply_table.py batches/table_only/TABLE_CHANGES.tsv`).

00ae2cb0 also still witnesses `?ExecuteLooped@CParticleProgram@@QAEXPAUCParticleSimulation@@J@Z -> 00ae2510`, but its HEAD object no longer references ExecuteLooped. Left alone; it needs another witness.

### batches/b1_hold: 7 compiled rows, verdict HOLD (not REJECT; nothing for land_scripted)
- **006c7960: NF373 overlap.** The edited row binds CCharString::operator+= (`??YCCharString@@QAEABV0@PBD@Z`) at 0x0099F100, which is one of NF373's units in the coordinator's message. Otherwise clean. Re-run after NF373 lands.
- **00485b44, 00800d30: port (NF303 Q2).** The PDB spelling is a member-template Transfer, and declaring it adds a /alternatename line to `.drectve`.
- **00757f90: port.** EBodyLocation is only in engine/CWoundMorphs.h, which adds `/DEFAULTLIB:"libcp"`. It needs an enum-only header (QUEUE_PROPOSALS).
- **00939e70, 00bfc770: RULING NEEDED.** The rows already carry /alternatename lines, and the respell renames both sides of each line consistently. `.drectve` is equal under the rename but not byte-equal. Is that acceptable?
  - 00bfc770 also depends on NAME_FIXES.tsv: 0x00BFD0B0 defines the CFanBaseVertex spelling, which 0x00BFCF90 also defines. It is SaveVerts<CWaterFanBaseVertex>.
- **00ad77f0: RELOC_CONFLICT.** The class-key respell (allocator / set / map) gives the set copy ctor a spelling already bound at 0x00513700 (witness 0051c470). One symbol would have two targets.

## Rulings needed
1. **`.drectve` /alternatename lines** (00939e70, 00bfc770): may a binder respell rename the symbols inside existing alternatename lines when both sides move consistently and no line is added?
2. **00ad77f0 set copy-ctor RELOC_CONFLICT:** is 0x00513700 or the 00ad77f0 callee target the true owner? This is an identity question for the STLport set instance.
3. **Stale witness lines left in the table:**
   - `?FinishAction@CCombatActionBase@@UAEXXZ -> 00859a20` (witness 0085c120). The HEAD object binds MAE, but 27 landed rows declare UAE (work/refdecl_stale.tsv).
   - `?GiveTutorialForCategory@CTCHero@@QAE_NJ_N@Z -> 006c9fa0` (witness 005ee418). 0057c2e6 (T3) still declares it.
   - The six b1 "kept" lines above (GetCombatAnimationSet QAE, SetPItem PBX, CAIStateGroupBase::AIAction_DoNothing QAEJ, CKeyboardDX::UpdateEvents, and the two old AddAction forms) become the same kind of line once b1 lands.
   - Should these lines be rewitnessed (a census picks the witness) or kept until the declarers move?
4. **Identity 009425f0 / 00942c70** (binder 00942ee0): MoveAndFace and StandAndTalk on TellStory or WorkAtSchool? Which class owns these bodies is unproven.

## Hand steps for the coordinator
- Land batches/b1 with land_scripted. No T3 release is needed: no ACCEPT address is in T3_PORT_QUEUE, and all NF374 locks are released (evidence/locks_final.txt: 88 locks, all `released`).
- QUEUE_PROPOSALS.tsv: 178 lines (141 standin_alias port, 35 port, 2 identity), addressed by target.
  - No address is already in T3_PORT_QUEUE (column 2 or a unit column), in NF373's set, or flagged TGT: in screen.
  - No unit column names an ACCEPT row.
  - The witnesses in HAND and table_only pass the same check.
- Then apply batches/b1/HAND_TABLE_CHANGES.tsv, keeping only the lines whose row (named in each note) actually landed. Then apply batches/table_only/TABLE_CHANGES.tsv.
- The `add ?AddAction@CTCScriptedControl@@QAEXPAVCActionBase@1@@Z -> 007137d0` line is witnessed by 007e7760 only. If 007e7760 is held while 00714d30 lands, add that line by hand with witness 00714d30.
- NAME_FIXES.tsv: 0x00BFD0B0 respell (self row plus its file name). This blocks 00bfc770.

## NF373 overlap (coordinator message)
- Checked every changed row's object against NF373's extended set (work/nf373_all.txt, 26 addresses): only 006c7960 binds one (0x0099F100), so it is held.
- Two triage lines name NF373 rows as binders, 006d5260 and 006929e0. Both were already off-limits (T3 + LOCK=NF373) and were not touched.
- No b1 new object contains any of the 39 symbols of NF373's units (work/nf373_syms.txt; grep over every new object).

## Not attempted
- 0x00BF6E20 via binder 00bf8c80: the compile budget was spent. The binder forwards its own (long, long, long) args, so its own ctor needs the PDB types too.
- The stand-in lines (185) were classified by census only (NF299 Q4) and queued as `standin_alias port`.
