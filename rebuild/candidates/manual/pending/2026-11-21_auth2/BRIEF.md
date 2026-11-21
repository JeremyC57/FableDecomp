# AUTH2: Account A authoring agent, round 2 (2026-11-21)

You continue AUTH1's work on three rows. Owner orders, unchanged: "It should not park functions, but keep working until they match";
stalls go to the advisor through the coordinator.

## Read first
1. `$S/w22/auth1/BRIEF.md`: ALL of its Paths, Tools, Read-first, Hard rules and Hand-back sections apply to you, with `auth1` -> `auth2`
   for YOUR outputs (work dir `$S/w22/auth2/`; you may read and reuse everything under `$S/w22/auth1/`, never edit it).
   `S=/tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad`.
2. For each of your rows: `$S/w22/auth1/<addr>/ESCALATE.md` and `LOG.md` (AUTH1's state, what it tried, its question). AUTH1's tools:
   `$S/w22/auth1/nd.py`, `g.sh`, `egl.py`, `odl.py`, `mini.py`, `c2x/`. Committed copy: `rebuild/candidates/manual/pending/2026-11-21_auth1/`.
3. `docs/journal/2026-11/2026-11-20_ADVISOR_DECISIONS_B.md` binds wherever it rules on your question, and so does the per-block Ego_d
   revision test (`git -C /home/user/FableDecomp show a6ca114f6e`). If an instruction below conflicts with them, they win: say so in LOG.md.

## Context budget (AUTH1 ran out)
Do not re-baseline anything AUTH1 already measured. Grade with `nd.py`-style residue counts and print only the differing hunks, never
whole disassemblies. Write findings to LOG.md as you go and keep STATUS.tsv current, so a restart loses nothing. If your context gets
heavy, finish the current step, update the files and return.

## Rows and the advisor's instructions (2026-11-21, owner delegated; provisional where ADVISOR_DECISIONS_B is silent)
Locks: `$S/w22/nflocks/{00a4efc0,0093d270,00c73050}/owner` now read AUTH2.

### 1. 0x00A4EFC0 C3DAnimationSequenceData::InitialiseFromFileData (DIFFER 3491v3491, one residue; best `$S/w22/auth1/00a4efc0/e1.cpp`, /Gr, overlay `$S/w22/auth1/00a4efc0/hdr`)
- The `FABLE_RETAIL_CSIMPLEMEMORYPOOL_ALLOCATETYPE_EGOD` opt-in is acceptable only if it is the REAL AllocateType text, not a fit. Test:
  define the macro in 3-5 LANDED rows that expand AllocateType (grep rebuild/src/compiled) and regrade them (q.py with your overlay). If any
  of them breaks, the text is fitted: reject it and find the text that holds for all of them.
- The dead assert's condition: use the text whose /Od size equals Ego_d's line (`odl.py` against `egl.py`), under ADVISOR_DECISIONS_B's
  fitted-assert-arm ruling as written.
- Temp-count dial: changing a helper's text because its Ego_d calibration FAILED is legitimate; adding a symbol only to move c2's count is
  a device (not allowed). So calibrate EVERY expanded helper of this row at /Od against Ego_d now (do not wait for FableWin) and fix
  whichever fails; then regrade.

### 2. 0x0093D270 CAIStateGroup_MeleeCombatNearbyBase::IsRunnable (DIFFER 305v295; best `$S/w22/auth1/0093d270/e1.cpp`)
- First: grep the getter (GetProximityForNavigation and its class) in `ghidra_out/egor_pdb_names.tsv` (Ego_r), not just FableWin / Ego_d.
  If Ego_r spells the return as a const reference (`ABM` rather than `M`), the reference text has a PDB source: use it (the shared header
  gets an add-only member-specific opt-in) and no rule question remains.
- Then (bounded measurement): run p25R's `c2path.py` (find it: `git -C /home/user/FableDecomp log --all --oneline -S c2path | head`, commit
  0163501389 area, or `tools/decomp_pipeline/linux_lane/lane_tools/c2probe/`) on the m2 mini (`$S/w22/auth1/mini/m2.cpp`) to see whether the
  budget path (callee compiled after caller) drops the return-value home. If it does, look for a natural TU construct that orders it so.

### 3. 0x00C73050 Audio_PlaySourceAndProcessQueue (DIFFER 672v672, one register; best `$S/w22/auth1/00c73050/b0.cpp`, /d2inlT150)
- Two links of AUTH1's causal chain are inferred. FIRST use the c2 hook (`$S/w22/auth1/c2x/`) to force #32 (item) to be coloured before #9
  (queue) and check the output equals retail. Only if it does, hunt for a natural text.
- This is not the 0x005BFD5E case (there, no perturbation moved the bytes; here the cause has a name): keep working.
- Check whether Ego_d has a line table for this function (`egl.py`). That settles the name's second source (catalog
  Audio_PlaySourceAndProcessQueue v p25N's CAudioSource::UpdateStream) and how constrained the text is. A redundant read is allowed only
  under the existing ruling's own conditions (ADVISOR_DECISIONS_B).
- Keep `/d2inlT150` as the base with an `// Inline-budget:` line (0x005414A0 precedent; the 2026-11-08 rule prefers a flag to FORCEINLINE).

## Order and stall rule
Work 0x00A4EFC0 first, then 0x0093D270, then 0x00C73050. Same stall rule as AUTH1 (25 grades with no residue reduction, or every idea
left contradicts a rule or Ego_d): write `$S/w22/auth2/<addr>/ESCALATE.md` and move on. Report when every row is matched or escalated.
Hand-back as in AUTH1's brief (STATUS.tsv, MATCHES.tsv, per-row source + `_test.cpp` + NOTES.md, per-row `hdr/`).
Never write into /home/user/FableDecomp (absolute output paths under `$S/w22/auth2/` only).
