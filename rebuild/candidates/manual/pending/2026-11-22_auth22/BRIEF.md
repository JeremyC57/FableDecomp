# AUTH22: Account A authoring agent (2026-11-22; Fable 5.2 authoring trial)

Owner orders: do not park. Keep working until each row matches. Stalls go to the advisor through the coordinator: write ESCALATE.md
for the row and report. Locks read AUTH22 (`$S/w22/nflocks/<addr>/owner`).
`S=/tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad`. Outputs go ONLY under `$S/w22/auth22/<addr>/`.
Never write into /home/user/FableDecomp and never run git, not even read-only (`git log` / `git show` included: AUTH18 slipped once). Use the files on disk. Wait on jobs by PID (kill -0), never pgrep -f. Be frugal with context:
residue counts and differing hunks only.

## Read first
1. `$S/w22/auth1/BRIEF.md`: Paths, Tools, Hard rules, Hand-back format (outputs move to `$S/w22/auth22/`).
2. Rules: CLAUDE.md, `docs/pipeline/ASM_RULES.md`, `tools/decomp_pipeline/linux_lane/lane_tools/chunk_pool/EXTRA_RULES.md`
   (names right the first time; shared headers; pick-any new-row rule).
3. Advisor rulings: `docs/journal/2026-11/2026-11-20_ADVISOR_DECISIONS_B.md` (a forced c2 decision is an oracle, not a grade;
   pre-register a candidate set, grade once). Inline model: `rebuild/candidates/manual/pending/2026-11-22_auth14/` (always-inline iff
   cost-4 <= S, default S 9; a scored site expands iff score < T, default T 74).
4. Lessons from this week, apply them first:
   - Grade at stock flags (`/Gr` or the TU default) BEFORE any `/d2inl*`: 0x008E18D0 matched at the default threshold once its text was
     right; the earlier T65 premise was wrong (AUTH15, `batches/2026-11-22_landing_auth15_updatelightningbeams/`).
   - Many residues close with a shared-header opt-in that changes inline cost (an implicit dtor, a decl-only ctor, a release-form debug
     check in `if (0)`, e.g. CArray.h `FABLE_RETAIL_CARRAY_DEBUGGER_BOUNDS_PASS27` for 0x00A4EFC0). Opt-ins are new macros in the shared
     header, default off; prove them with pin_asserts / assert_names_check / equiv_check (lane_merge README).
   - A terminal (A) ruling (proven per-callee inline tie) is the right outcome when no text change moves it: 0x00CF4EC0
     (`pending/2026-11-22_auth16/00cf4ec0/TERMINAL.md`) is the model for writing one.
5. PDBs on the lane (never copy them anywhere in the repo or a package):
   `$S/dl_debug/files/Ego_d.pdb` (VC10 debug: line tables, S_BPREL32 locals, blocks), `$S/dl_debug/files2/Ego_r.pdb` (VC10 release: names,
   types, inline sites), `$S/dl_debug/files2/FableWin.pdb` (VC10 debug editor). Ego_r / FableWin need
   `rebuild/candidates/manual/pending/2026-11-22_codex_pass27/tools/repack4k.py` before llvm-pdbutil. Symbol dumps already made:
   `$S/codex27/pdbx/*.symbols.txt`. VC10 lane (`odall10.py`) is evidence only; grade with VC7.1 3077 (rg1.sh / q.py / vland.py --dry).

## Owner trial
The owner moved hand authoring to Fable 5.2 "to see if match rate improves" (`rebuild/candidates/manual/pending/AUTHORING_MODEL_TRIAL.md`,
which has the Opus baseline). Same rules and lane as every AUTH agent. Count your grades per row; the coordinator scores the run there.

## Rows, in this order (one to a terminal state before the next)
1. **0x00502E90** CWorldMap::PopulateRegionWithVillagers, DIFFER(13587v13622). Not started by AUTH1-AUTH21; history in UNMATCHED_ROWS.tsv
   (acctC round-six draft), `pending/2026-11-21_auth1/00502e90/`, and `$S/w22/auth21/00502e90_HANDOFF.md`.
2. Five parked near misses nobody worked this week (closest_version in UNMATCHED_ROWS.tsv is the best draft on record):
   - **0x0066B370** CTCSoundPlayer::PlayAllSoundEvents, DIFFER(1805v1808)
   - **0x00A68DE0** CVolumeTexture::LoadFromDataStream, DIFFER(923v919)
   - **0x00839420** NThingHelpers::CreateEffectOnHit, DIFFER(1304v1308)
   - **0x00651273** NPlayerGui::CDrawQuestInfo::CQuestInfoElementMiniGameBar vfunc_0 (name it from the PDBs / vtable), DIFFER(1677v1681)
   - **0x00816410** NHitLocations::CPhysicalPrimitiveCylinder::GetClosestPointToPos2D, DIFFER(1169v1174)
   For each: re-grade the recorded draft first (it may be stale against today's headers), then port it onto the shared headers and the
   PDBs' locals / line sizes (Ego_d, FableWin) before any flag work.
Out of scope: 0x00B4D370 and 0x00BD3070 (they wait on the C2T1 comparator readout; do not touch `$S/w22/auth21/`, `$S/w22/c2t1/`),
every Codex pass 27 row, and every other row.
Disk is tight (~3.4 GB free): delete your own /Od and object scratch as you go (literal paths).
Method: once a residue is pinned to one construct, pre-register the single candidate that would explain it, grade it once on the REAL
row, and report it as an oracle if it contradicts the PDBs. If a row stops moving, write ESCALATE.md and report rather than spending
context; the coordinator takes it to the advisor.
History: UNMATCHED_ROWS.tsv lines, `pending/2026-11-21_auth1..auth5/`, `pending/2026-11-22_auth10/STATUS.tsv`, acctC round-six drafts.
Out of scope: everything reserved for Codex pass 27 and every other row.

## Hand-back, per row (`$S/w22/auth22/<addr>/`)
Source, test, NOTES.md (evidence for every name, descriptive names marked), MATCHES.tsv (grade line), STATUS.tsv, LOG.md,
NAME_FIXES.tsv / LABELS.tsv when needed, and any header change as a patch file with the equivalence proof. Keep
`$S/w22/auth22/STATUS.tsv` current (address, state, best_grade, grades_used, best_source, note). Report briefly when a row reaches a
terminal state (match, ESCALATE.md, or a TERMINAL.md proof) or when context gets heavy, so the coordinator can land it and restart you.
