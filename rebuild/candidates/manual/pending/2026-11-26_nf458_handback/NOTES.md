# NF458 (2026-11-26): NEXT_CANDIDATES stale lines (A3 continuation), rulings r453a-e as NF453 applied them

## State / resume point
- **Final base: repo HEAD 5288bedbe6 (NF457 landed during the run; started at 90d89c9417).** work/base/ holds the table / self_symbols /
  descriptive_names copied at 5288bedbe6 (work/base_90d89c94/ = the first snapshot). Repo table sha1 2f9584397da7..., unchanged during the
  final track run.
- **Census refresh:** NF453's work/cache_*.tsv copied. Drift: 4 rows changed at 00:11 (007bb800 007c78c0 007e7ed0 00bdfd80 -> cache_9e.tsv),
  then NF457's 38 rows (-> cache_9f.tsv). drift458.py: 0 drift at 5288bedbe6. No header / build_candidates.ps1 change after 22:46 (NF453 handled those).
- **Lines:** prep458.py (NEXT_CANDIDATES vs today's table) -> work/lines.tsv, work/refs.tsv: 490 lines, 437 targets, 11,692 referencing rows.
  classify458.py -> work/verdicts.tsv (identical at both bases): 274 DELETE, 158 REWITNESS, 41 HOLD_WITNESS_NO_PAIR_AT_T,
  8 HOLD_MULTI_WITNESS, 4 HOLD_WITNESS_GRADE, 1 HOLD_HELD_ELSEWHERE, 4 SKIP_LOCKED (CODEX29 x2, COORD_HOLD_NF453 x2), 0 pending
  (scanner live: 105 unapplied pending-hand-back lines, none at a candidate (symbol, target)).
- **Evidence:** all 8,330 rows (every row referencing a candidate target, each witness / new witness / target row) recompiled fresh with
  evid453 (verify_and_land.parity_of + reloc_identity.check): work/evid_n_* (3,106), evid_r_* (5,224), evid_s (4 NF457 rows).
  NF453's older evid lines were NOT reused (104 of them differ from a fresh compile, all only by build-path anonymous-namespace hashes).
  Every evid row's source sha1 equals the repo source at 5288bedbe6.
- **finalize458.py** (final453 + rebuild453 in one pass, plus: hold a live spelling that two replaces would write at two targets -> none occurred) -> batches/nf458_all/TABLE_CHANGES.tsv: **107 delete (106 live spelling already in the table at t,
  1 a second stale line covered by a replace), 105 replace (r453b), 25 rewitness = 237 lines.**
- **Checks (shadow of 5288bedbe6, tools/track458.sh; HEAD shadow rebuilt back to back):** apply --dry 0 misses, multi-target 1 -> 1, new [];
  descriptive_check 2,022 lines 0 FAIL (REGISTRY_REMOVE.tsv empty); EVCHECK 237/237 PASS (every referencing row re-derived, no binder
  of a deleted / replaced stale spelling, every replace witness binds its live spelling, every new witness binds and old witness does not).
- **Tracker (TRACKER_EFFECT.tsv, v the 5288bedbe6 table):** +57 done: 23 split_unknown -> agree_std, 19 split_fixable -> agree_pdb,
  11 standin -> agree_std, 2 standin -> agree_pdb, 1 agree_unverified -> agree_pdb, 1 agree_unverified -> agree_std. Also 2 agree_std -> agree_pdb,
  14 split_unknown -> agree_unverified, 2 standin -> agree_unverified, 3 split_unknown -> split_fixable, 3 split_unknown -> standin;
  18 split_fixable + 13 split_unknown keep their status with fewer spellings. No address leaves the tracker; no agree -> worse move.
- **3 split_unknown -> standin** (00c2c7b0, 00c705b0, 00c70f80): the deleted line was the only non-self spelling and is bound by no
  object; what is left is the landed stand-in self (`CRegistryLayer_00c2c7b0`, `CVtable_012bedcc/..de0` + `SUnprovenImpulse52`). The
  deleted `SVoiceTap` parameter spelling at 00c705b0 / 00c70f80 read better than the live `SUnprovenImpulse52`; it was not live anywhere.
- **Held (batches/HELD_LINES.tsv, 253):** 96 NOT_STALE_EVID (reloc_identity.check shows the table witness still binds: census false
  positives), 87 HOLD_r453d_UNCHECKED_REF (52 delete, 35 rewitness), 41 HOLD_WITNESS_NO_PAIR_AT_T, 9 HOLD_ANON_HASH, 8 HOLD_MULTI_WITNESS,
  4 HOLD_WITNESS_GRADE, 4 SKIP_LOCKED, 2 HOLD_EVID_REWITNESS, 1 HOLD_HELD_ELSEWHERE, 1 HOLD_IDENTITY_FIX.
- **IDENTITY_FIXES.tsv (1):** `?IsPlayingVirtual@CSoundEvent@@QAEJXZ` is live at 00c20f40 (witness 00c12680) but bound in the table only at
  00c207a0; the stale `?IsPlayingVirtual@CSoundEvent_00c12680@@QAEJXZ` line at 00c20f40 is held and the 00c207a0 line untouched.
- **NAME_FIXES.tsv (r453e):** 31 lines at 26 addresses where a live spelling this batch writes or keeps differs from naming_status's
  pdb_spelling (anonymous-namespace hashes normalised), e.g. 00cb7e50 `?Add@CQ_*Script@NScript@@...` vs PDB `?AddSpawnedFunction@CScriptBase@@...`,
  00af7d10.. `?GetDrawableObject@@...` vs PDB `?Alloc@CPSC*@NParticleEngine@@...`, 013b8790 `?g_pGameUI@@3PAUCGameUI@@A` vs the PDB singleton.

## Rulings needed
1. **r453d and relocation-free MATCH rows:** 62 of the 87 r453d holds are blocked only by referencing rows that grade MATCH (byte-exact)
   in evid AND whose census object has the leaf found by name (MATCH/LEAF) with an empty relocation list for that section (their retail
   bytes merely hold a constant equal to / within 0x80 of the target). Checked over every referencing row, not the 10 listed. Such a row
   binds nothing at the target, so it cannot keep a stale spelling alive. Ruling asked: count them as re-derived. Not applied here.
   (An empty pair set alone is not enough: e.g. 00557c10 is RELOCATION_MATCH with no pairs; those stay held.)
2. **8 two-witness lines (HOLD_MULTI_WITNESS):** the table has (s, t) twice with different witnesses; the census says one witness is stale.
   apply_table keys on (symbol, target), so it cannot drop one witness line (a rewitness would make both lines identical). Needs either a
   witness-keyed delete in apply_table or a ruling to leave them.

## Other observations
- **Standin -> agree moves (15, 13 of them inside the +57):** all are `replace` lines at non-landed targets where a stand-in table spelling
  gives way to the witness's live spelling, mostly libpk / audio vtables (e.g. 012b8fc4 `??_7CVtable_012b8fc4` -> `??_7CAudioRingBuffer`,
  012c1648 `??_7CVtable_012c1648` -> `??_7CScriptBase` = PDB) and 00accbf0 (-> the PDB `PerformSubContraction`). The libpk vtable names
  come from landed code (no RTTI there), so their agree_std is only as good as those names (cf. NF453's 0x012B2F60 note).
- **Funclet selves:** 8 live spellings written or kept here are also the self symbol of an unwind funclet row (e.g. `?Shutdown@CSoundEventManager@@QAEXXZ`
  at 012116d7 for the replace at 00c123b0, `?Wait@PKCSemaphore@@QAEXXZ` at 01213512). Funclet rows carry their parent's name as self
  (convention), so this is not a 0x004ADFF0-style identity clash; no hold.
- **NF457 overlap:** none (its hand-back has no table file; none of its 52 addresses is a TABLE_CHANGES target or witness).
- **Name queue:** the landed stand-in selves at 00c705b0 / 00c70f80 carry the placeholder `SUnprovenImpulse52` (an `Unproven` name in landed
  code); the stale `SVoiceTap` spelling deleted there may be the better candidate for a respell.
- **Identity fix sides:** finalize holds the stale line; the other side (00c207a0's line) is not in this batch, so nothing else needed holding.
- **-I disclosure:** census453 / evid453 / need458 / finalize458 import repo modules (verify_and_land, reloc_identity) and call clean.py
  without -I, as NF453 did; all ran from my work dir with explicit paths. Repo files I did not write were run with -I (apply_table,
  descriptive_check, naming_status).
- **Last freshness check (00:5x UTC):** HEAD 5288bedbe6, drift 0, the three oracle files equal to work/base, no header /
  build_candidates.ps1 / auto-re-candidates.tsv newer than 00:29.

## Locks
222 held as NF458 (work/locks_held.txt): every TABLE_CHANGES target and new witness. 279 candidate locks taken at the start were released
(work/locks_released.txt, written `released`).

## Tools (tools/)
NF453 copies repointed to agentNF458 (nf, census453, evid453, mkshadow, recdiff453, ...). New: drift458, prep458, classify458 (pending scan
over every hand-back table file, own lock name, GONE / HOLD_MULTI_WITNESS), need458, run_evid458(b), finalize458, evcheck458, track458, namefix458.
No git was run; HEAD was read from .git/refs; naming_status ran in shadows with the no-op git shim.
