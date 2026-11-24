# AUTH9: Account A authoring agent (2026-11-22)

Owner orders: do not park. Keep working until each row matches. Stalls go to the advisor through the coordinator: write ESCALATE.md
for the row and report. Locks read AUTH9 (`$S/w22/nflocks/<addr>/owner`).
`S=/tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad`. Outputs go ONLY under `$S/w22/auth9/<addr>/`.
Never write into /home/user/FableDecomp. Be frugal with context: residue counts and differing hunks only.

## Read first
1. `$S/w22/auth1/BRIEF.md`: Paths, Tools, Hard rules, Hand-back format. Outputs move to `$S/w22/auth9/`.
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
1. **0x00763060 NShape::FractureShape** (1146 B). Codex pass 26 row 0x007634E0
   (`rebuild/src/compiled/00/76/NShapeCBasicShape_UpdateSubdivisions_007634e0.cpp`) carries a full FractureShape whose VC7.1 copy equals
   retail byte for byte. AUTH8 showed it is Ego_d-exact on lines 195-241 and all locals (`$S/w22/auth8/RESULT.md`). Make it its own row:
   - source: one definition. Either move the body into the new row and make 0x007634E0 call or declare it (link check both), or follow
     EXTRA_RULES for a strong TU-mate copy;
   - grade it as a leaf against 0x00763060 (MATCH or RELOCATION_MATCH);
   - write a behaviour test from Codex's fixture (the 0x007634E0 test);
   - recheck 0x007634E0 after the change.
   History: `pending/2026-11-21_auth5/`, `_auth6/00763060/`, `batches/2026-11-22_landing_p26/README.md` "FractureShape".
2. **0x00B30B50 CEngineCamera::SetCameraState** (1552 B catalog, 1499 B copy). It matches inside
   `rebuild/src/compiled/00/b3/CEngineCamera_ProcessDebugScript_00b31160.cpp` (p26). Same treatment as row 1: own row, own test,
   recheck 0x00B31160.
3. Then the remaining Account A authoring rows: **0x008E18D0** UpdateLightningBeams, **0x00CF4EC0** CFanMarker::Main,
   **0x00B4D370** UpdateShadowSpace, **0x00BD3070** RenderMeshPrimitive, **0x00502E90** PopulateRegionWithVillagers,
   **0x0093D270** CAIStateGroup_MeleeCombatNearbyBase::IsRunnable. Their history is in `pending/2026-11-21_auth1..auth4/`
   and UNMATCHED_ROWS. 0x0093D270's UNMATCHED name column holds a mangled name: report the correct catalog name.
Out of scope: 0x00A4EFC0 (waits on an advisor ruling) and every row reserved for Codex pass 27.

## Hand-back, per row (`$S/w22/auth9/<addr>/`)
Source, test, NOTES.md (evidence for every name, descriptive names marked), MATCHES.tsv (grade line), STATUS.tsv, LOG.md,
NAME_FIXES.tsv / LABELS.tsv when needed, and any header change as a patch file with the equivalence proof (BUILDING: pin_asserts,
assert_names_check, equiv_check). Keep STATUS.tsv current; report briefly when a row reaches a terminal state or context gets heavy.
