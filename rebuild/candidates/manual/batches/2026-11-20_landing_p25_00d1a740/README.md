# Landing p25 00d1a740: NScript::CQ_BountyHuntScript::CBH_Hostage2::Main (Account B, 2026-11-20)

One of the 25 parked rows Account A reserved for Account B. Written by B's Opus author p25A and checked by B's coordinator: purity,
labels, add-only overlay, hdrcheck2 default=same, then strict vland `--land`. Grade: RELOCATION_MATCH, test PASS.

Author's notes: author_README.md (measurements, `// Inferred:` / `// Bracket:` lines, header opt-ins, mutants).

## Device ablation (2026-11-20, advisor requirement)
Each device was removed in turn, with the others kept:
- FABLE_RETAIL_CWIDESCREENMAGICPAUSEENTITIES_FORCEINLINE_CTOR: still RELOCATION_MATCH and test PASS without it. **Dropped from the landed row.**
  The define line is now a comment, so the line count is unchanged; the recheck is RELOCATION_MATCH PASS.
- FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTSCRIPTEDTHINGBASE_FORCEINLINE_ASSIGN: needed (DIFFER 2707v2743). No flag replaces it: S15+T90/T120/T200
  give 2707; S20 2755; S30 / S40+T200 2787.
- FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTMOVIEBASE_FORCEINLINE_CTOR: needed (DIFFER 2755v2743; S20 2787, S15+T120 2755 without it).
- /d2inlS15: needed (DIFFER 2706v2743).
