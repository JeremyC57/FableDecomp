# AUTH10: Account A authoring agent (2026-11-22)

Owner orders: do not park. Keep working until each row matches. Stalls go to the advisor through the coordinator: write ESCALATE.md
for the row and report. Locks read AUTH10 (`$S/w22/nflocks/<addr>/owner`).
`S=/tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad`. Outputs go ONLY under `$S/w22/auth10/<addr>/`.
Never write into /home/user/FableDecomp. Be frugal with context: residue counts and differing hunks only.

## Read first
1. `$S/w22/auth1/BRIEF.md`: Paths, Tools, Hard rules, Hand-back format. Outputs move to `$S/w22/auth10/`.
2. Rules: CLAUDE.md, `docs/pipeline/ASM_RULES.md`, `tools/decomp_pipeline/linux_lane/lane_tools/chunk_pool/EXTRA_RULES.md`
   (names right the first time; shared headers; pick-any new-row rule 2026-11-21).
3. VC10 evidence lane: docs/BUILDING.md "VC10 SP1", the odall10.py docstring (`--lib`, `--stl`), FINDINGS_LOG 2026-11-21/22 VC10 entries.
   Evidence only. Rows are graded with VC7.1 3077 (rg1.sh / q.py / vland.py --dry).
4. PDBs on the lane (never copy them into the repo):
   - `$S/dl_debug/files/Ego_d.pdb` (VC10 debug game): line tables, S_BPREL32 locals, blocks.
   - `$S/dl_debug/files2/Ego_r.pdb` (VC10 release game): names, types, inline sites.
   - `$S/dl_debug/files2/FableWin.pdb` (VC10 debug editor): names, code of shared library functions.
   Ego_r and FableWin need `rebuild/candidates/manual/pending/2026-11-22_codex_pass27/tools/repack4k.py` before llvm-pdbutil.
   Use them for names (two sources) and for locals and line sizes.

## Rows, in this order (one to a terminal state before the next)
AUTH9 landed 0x00763060 and 0x00B30B50 (`batches/2026-11-22_landing_auth9_*`); do not redo them. Its hand-back is in
`rebuild/candidates/manual/pending/2026-11-22_auth9/`; its working files are in `$S/w22/auth9/`, including `tools/leafrel.py` (for every
relocation a leaf uses: the retail target and a comparison of the copies the object defines; read it before use).
1. **0x008E18D0** NCreatureMode::CModeThunderLightningStorm::UpdateLightningBeams. Start from AUTH9's `pending/2026-11-22_auth9/008e18d0/HANDOVER.md`
   (best b0.cpp DIFFER(2045v2073); the overlay `hdr.diff` is AUTH9's add-only IMPLICIT_DTOR opt-in re-based on HEAD). Next steps it names: the
   opt-ins the landed sibling 0x007A1380 has and b0 lacks, then a bounded inl.py hunt on the loop-2 lookup, each choice backed by Ego_d line sizes.
2. Then **0x00CF4EC0** CFanMarker::Main, **0x00B4D370** UpdateShadowSpace, **0x00BD3070** RenderMeshPrimitive, **0x00502E90**
   PopulateRegionWithVillagers, **0x0093D270** CAIStateGroup_MeleeCombatNearbyBase::IsRunnable (`?IsRunnable@CAIStateGroup_MeleeCombatNearbyBase@@MAE_NXZ`,
   both PDBs). History: `pending/2026-11-21_auth1..auth5/` and UNMATCHED_ROWS.
Out of scope: 0x00A4EFC0 (waits on an advisor ruling) and every row reserved for Codex pass 27.

## Hand-back, per row (`$S/w22/auth10/<addr>/`)
Source, test, NOTES.md (evidence for every name, descriptive names marked), MATCHES.tsv (grade line), STATUS.tsv, LOG.md,
NAME_FIXES.tsv / LABELS.tsv when needed, and any header change as a patch file with the equivalence proof (BUILDING: pin_asserts,
assert_names_check, equiv_check). Keep STATUS.tsv current; report briefly when a row reaches a terminal state or context gets heavy.
