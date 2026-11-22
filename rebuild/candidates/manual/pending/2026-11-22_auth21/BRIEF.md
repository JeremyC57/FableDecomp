# AUTH21: Account A authoring agent (2026-11-22; continues AUTH21)

Owner orders: do not park. Keep working until each row matches. Stalls go to the advisor through the coordinator: write ESCALATE.md
for the row and report. Locks read AUTH21 (`$S/w22/nflocks/<addr>/owner`).
`S=/tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad`. Outputs go ONLY under `$S/w22/auth21/<addr>/`.
Never write into /home/user/FableDecomp and never run git, not even read-only (`git log` / `git show` included: AUTH18 slipped once). Use the files on disk. Wait on jobs by PID (kill -0), never pgrep -f. Be frugal with context:
residue counts and differing hunks only.

## Read first
1. `$S/w22/auth1/BRIEF.md`: Paths, Tools, Hard rules, Hand-back format (outputs move to `$S/w22/auth21/`).
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

## Rows, in this order (advisor ruling 2026-11-22, ADVISOR_DECISIONS_B.md last entry)
1. **0x00B4D370** CEngineShadowRenderer::UpdateShadowSpace, ONE bounded item, then move on. AUTH20's packet: `$S/w22/auth20/00b4d370/`
   (ESCALATE.md, NOTES.md, LOG.md; p9 = best_evidence.cpp, the row's record DIFFER(7162v7176); p5 = best.cpp, 7176v7176, a bracket only;
   flags `/d2inlT314`, overlay `ov/`). The whole 14-B gap is the frame (0x3c0 v retail 0x3d0): retail puts the last PostMultiply's 64-B
   result over the dead local_world_to_base_shadow_space home (0x100..0x13F). Check p5's frame size and where IT puts that result.
   If p5 has retail's placement, the p5-v-p9 text difference at that site is your one candidate: pre-register it, grade it once on p9, report.
   If p5 reaches 7176 another way, record how; the placement stays open. Do not chase the x87 operand-order sites (a tools agent, C2T1, is
   rebuilding the c2 comparator readout for those; do not touch `$S/w22/c2t1/`). Line 590 is queued for Codex.
2. **0x00BD3070** RenderMeshPrimitive: resume from `$S/w22/auth20/00bd3070/` (AUTH1's b6.cpp + overlay, DIFFER(11288v11288) at stock,
   ~4 s a grade). Move the ~15 row-local FABLE_FORCEINLINE definitions into their shared headers as add-only opt-ins (equivalence proof),
   then regrade. If the rest is c2 numbering ties, write ESCALATE.md and move on.
3. **0x00502E90** CWorldMap::PopulateRegionWithVillagers, DIFFER(13587v13622).
Copy what you reuse into `$S/w22/auth21/<addr>/`; never edit AUTH19 / AUTH20 folders. Disk is tight (~3.5 GB free): delete your own
/Od and object scratch as you go (literal paths).
Method: once a residue is pinned to one construct, pre-register the single candidate that would explain it, grade it once on the REAL
row, and report it as an oracle if it contradicts the PDBs. If a row stops moving, write ESCALATE.md and report rather than spending
context; the coordinator takes it to the advisor.
History: UNMATCHED_ROWS.tsv lines, `pending/2026-11-21_auth1..auth5/`, `pending/2026-11-22_auth10/STATUS.tsv`, acctC round-six drafts.
Out of scope: everything reserved for Codex pass 27 and every other row.

## Hand-back, per row (`$S/w22/auth21/<addr>/`)
Source, test, NOTES.md (evidence for every name, descriptive names marked), MATCHES.tsv (grade line), STATUS.tsv, LOG.md,
NAME_FIXES.tsv / LABELS.tsv when needed, and any header change as a patch file with the equivalence proof. Keep
`$S/w22/auth21/STATUS.tsv` current (address, state, best_grade, grades_used, best_source, note). Report briefly when a row reaches a
terminal state (match, ESCALATE.md, or a TERMINAL.md proof) or when context gets heavy, so the coordinator can land it and restart you.
