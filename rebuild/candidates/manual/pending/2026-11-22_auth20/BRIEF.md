# AUTH20: Account A authoring agent (2026-11-22; continues AUTH20)

Owner orders: do not park. Keep working until each row matches. Stalls go to the advisor through the coordinator: write ESCALATE.md
for the row and report. Locks read AUTH20 (`$S/w22/nflocks/<addr>/owner`).
`S=/tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad`. Outputs go ONLY under `$S/w22/auth20/<addr>/`.
Never write into /home/user/FableDecomp and never run git, not even read-only (`git log` / `git show` included: AUTH18 slipped once). Use the files on disk. Wait on jobs by PID (kill -0), never pgrep -f. Be frugal with context:
residue counts and differing hunks only.

## Read first
1. `$S/w22/auth1/BRIEF.md`: Paths, Tools, Hard rules, Hand-back format (outputs move to `$S/w22/auth20/`).
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

## Rows, in this order (one to a terminal state before the next)
1. **0x00B4D370** CEngineShadowRenderer::UpdateShadowSpace: RESUME from AUTH19 (`$S/w22/auth19/00b4d370/HANDOFF.md`, then NOTES.md / LOG.md).
   Start from `best_evidence.cpp` (p7, DIFFER(7162v7176), the Ego_d / FableWin-faithful port; nm2.py 149); `best.cpp` (p5, 7176v7176,
   nm2 110) is the lowest residue but less faithful. Overlay `$S/w22/auth19/00b4d370/ov/` and the three patches there; flags `/d2inlT314`
   (as the landed sibling 0x00B4FC40). AUTH19's tools (nm2.py, lc.py, blk.py, g.sh) are yours. The advisor told AUTH19 not to escalate
   while the row is still moving. HANDOFF.md's order: the remaining /Od line sizes first (532, 546, 590, 644), then ONE pre-registered
   candidate, only then a c2 id trace. Copy what you reuse into `$S/w22/auth20/00b4d370/`; do not edit AUTH19's folder.
   Header note: `hdr_GFLimit.patch` (`FABLE_RETAIL_GFLIMIT_UPPER_LOWER`) cannot coexist with engine/CAnimSet.h's own GFLimitLower: say
   which includers that rules out when you hand back.
2. **0x00BD3070** CEnginePrimitiveManagerAnimatedMeshesBase::RenderMeshPrimitive.
3. **0x00502E90** CWorldMap::PopulateRegionWithVillagers, DIFFER(13587v13622).
Method: once a residue is pinned to one construct, pre-register the single candidate that would explain it, grade it once on the REAL
row, and report it as an oracle if it contradicts the PDBs. If a row stops moving, write ESCALATE.md and report rather than spending
context; the coordinator takes it to the advisor.
History: UNMATCHED_ROWS.tsv lines, `pending/2026-11-21_auth1..auth5/`, `pending/2026-11-22_auth10/STATUS.tsv`, acctC round-six drafts.
Out of scope: everything reserved for Codex pass 27 and every other row.

## Hand-back, per row (`$S/w22/auth20/<addr>/`)
Source, test, NOTES.md (evidence for every name, descriptive names marked), MATCHES.tsv (grade line), STATUS.tsv, LOG.md,
NAME_FIXES.tsv / LABELS.tsv when needed, and any header change as a patch file with the equivalence proof. Keep
`$S/w22/auth20/STATUS.tsv` current (address, state, best_grade, grades_used, best_source, note). Report briefly when a row reaches a
terminal state (match, ESCALATE.md, or a TERMINAL.md proof) or when context gets heavy, so the coordinator can land it and restart you.
