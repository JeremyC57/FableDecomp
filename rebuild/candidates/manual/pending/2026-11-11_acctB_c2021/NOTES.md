# Hand chunk 2021 (CEngineMap .. CEngineMap): 2 of 2 byte-exact

Both rows RELOCATION_MATCH under VC7.1 RTM 3077 with `FABLE_HDR_OVERLAY=/opt/fable_lane/w22/agent2021/hdr`, behaviour tests PASS
under g++ (tall.sh) and VC7.1 (tall.py), read-only vland: both RELOCATION_MATCH + behaviour PASS, no relocation-identity finding.
Grades: 0x00B3F5E0 3, 0x00B40BD0 9 (all in pass 1; tmp/grades.log). Free probes on top: 3 c2prio traces, 5 inline traces,
~10 scheduler-trace compiles (tools/sched.sh, tools/schedv.py), 1 c2prio variant-header trace.
Row extents checked: 0x00B3F5E0 + 1624 = 0x00B3FC38 (`ret 0Ch` at 0x00B3FC35, PeekInterpolatedMapNormal at 0x00B3FC40 after int3);
0x00B40BD0 + 1736 = 0x00B41298 (`ret 8` at 0x00B41295, PeekHasWater at 0x00B412A0 after int3). No SPLITS.tsv needed.

## What matched (2)
- **CEngineMap** (shared header engine/CEngineMap.h + new opt-in `FABLE_RETAIL_CENGINEMAP_PEEK_BODIES`, hdr/README.md)
  - 0x00B3F5E0 `C3DVector CEngineMap::PeekMapNormal(long x, long y) const` -- quantised heights of the four neighbours (header inline
    PeekLandscapeHeight expanded four times), `C2DVector dx(left - right, 2.0f)`, `dy(down - up, 2.0f)`, both normalised out of
    line (0x00A14540), `C3DVector normal(dx.X, dy.X, dx.Y * dy.Y); normal.Normalise(); return normal;`. Matched on the first grade
    once the Peek bodies read `g_pEngineWorldMap` in place. `// Flags: /d2inlT444` (inline-budget device; see below).
    `// Retail-asm:` lines cite the four expanded GFFloatToLongNear sites (0x00B3F739, 0x00B3F89D, 0x00B3F9FA, 0x00B3FB3F).
  - 0x00B40BD0 `float CEngineMap::PeekWaterDepth(long x, long y) const` -- per theme layer 0..2 (stop at the first theme <= 0):
    def class index (0x009ACCA0), blend (layer 2: `255 - blend1 - blend0` as a byte), `(blend * PeekTheme(index).WaterHeight) *
    (1.0f / 255.0f)` summed. Levers, in order: nested ifs with one `return depth` (retail's shared `fld [esp+18h]` exit and the
    `return 0.0f` block at the end); one index local per layer (separate frame slots); the header bodies reading
    `g_pEngineWorldMap` in place instead of a `worldMap` local (the first expansion's MapWidth CSE then spills to [esp+38h] as
    retail; found with c2prio: the local made copy 1's W candidate split into two copy-forwarded pieces = rematerialised loads,
    the global made it a register-plus-spill candidate like copies 2 and 4); `(blend * WH) * k` grouping (one PAREN tuple: c2's
    list scheduler then issues the layer-1 `fmul k` before the two copy stores, measured with the scheduler trace, picks
    `fmul#9 mov#14 mov#16 0x269 fmul#11 mov#15 mov#17`). `// Flags: /d2inlT576`.

## Inline budget (both rows)
PeekThemeId / PeekThemeBlend / PeekLandscapeHeight are plain header inlines with COMDAT copies in this TU's inline run (0x00B43720 /
0x00B43870 / 0x00B439C0). Retail expands every site in this TU, but their plain-inline scores (inline trace, c2.dll 0x1078D1AA) are
403 (f=1) .. 575 (f=0.25) for PeekThemeId / Blend (c1 308) and 443 (f=1) for PeekLandscapeHeight (c1 345), above the default budget.
No control flow raises a straight-line site above f=1. __forceinline also matches both rows (graded: 0x00B3F5E0 g1, 0x00B40BD0 g7)
but is ruled out by another TU: CWaterPatchMesh::GetWaterType 0x00BEAB40 calls 0x00B43720 three times inside its loops
(0x00BEAB8A / 0x00BEAB9B / 0x00BEABAE), which a forced inline would have expanded. So the bodies are plain inline and each row carries
the smallest `/d2inlT<N>` (owner device 2026-11-08): T444 (PeekMapNormal), T576 (PeekWaterDepth, T575 calls the two f=0.25 sites:
graded 1169 bytes). Both rows also match at a common T576 (graded), and the parked sibling 0x00B41770 needed T301: one TU-wide
inline setting >= 576 is consistent with all three.

## Names (all two-source; none descriptive except the established data-symbol spellings)
- Rows: `?PeekMapNormal@CEngineMap@@QBE?AVC3DVector@@JJ@Z`, `?PeekWaterDepth@CEngineMap@@QBEMJJ@Z` (Ego_r + FableWin name sets, Ego_r
  donor order = retail order, callers: 0x00B3FC40 for the first; 0x00B416A0 / 0x00B418A4 / 0x00B41A96 / 0x00B41D3C for the second).
- Callees: PeekThemeId / PeekThemeBlend / PeekLandscapeHeight (Ego_r + FableWin); `?GetDefClassIndexFromGlobalIndex@CDefinitionManager@@QBEJJ@Z`
  0x009ACCA0 and `?Normalise@C2DVector@@QAEAAV1@XZ` 0x00A14540 (landed bound spellings, both PDBs); inline FableWin
  `?PeekTheme@CEngineLandscapeRenderer@@QBEABUCLandscapeTheme@@K@Z` (struct key U as the PDB mangles it; header struct CLandscapeTheme).
- Globals: `g_pEngineWorldMap` 0x01436E8C, `g_pEngineDefinitionManager` 0x01436E28, `g_LandscapeRenderer_01436EA8` 0x01436EA8 -- the
  spellings CEngine::Initialise 0x00B26360 (landed) binds; no PDB data-symbol dump exists, so they stay the established
  descriptive data names (EXTRA_RULES 2026-11-01). Float constants are compiler literals (__real@...), as in retail (merged pools).
- Descriptive names: none new. Local variable names (left/right/down/up, dx/dy, theme0..2, index0..2, blend0..2, depth) are mine.

## Header debt / review
- hdr/engine/CEngineMap.h (opt-in `FABLE_RETAIL_CENGINEMAP_PEEK_BODIES`) and hdr/engine/CEngineLandscapeRenderer.h (opt-in
  `FABLE_RETAIL_CENGINELANDSCAPERENDERER_PEEKTHEME`) must be merged into rebuild/include before landing (both are additive opt-ins).
- The parked siblings 0x00B43720 / 0x00B43870 / 0x00B439C0 (compensation-block residue) and 0x00B41770 (MapWidth CSE slot residue,
  the same symptom as this chunk's first expansion) all copied `g_pEngineWorldMap` into a `worldMap` local; reading the global in
  place was this chunk's lever for exactly that MapWidth spill. Worth one regrade each with the new opt-in (not done: no oracle rows
  for them in $S/batch).
- 0x01231724 (1/255) is the float literal pool __real@3b808081 (also used by this row); the landed 0x00B45460 binds it as a named
  `?g_OneOver255@@3MB` and another row as `?g_fReciprocal255_01231724@@3MA`: literal pools, not data symbols (review, not a NAME_FIX).
- Objects also emit the inline COMDATs (PeekLandscapeHeight 350 bytes here vs retail's 356 at 0x00B439C0; PeekWidth/Height/WorldX/Y,
  C3DVector::Normalise): check COMDAT selection at link.

## Parked for Fable 5.1 (0)

## Skipped (0)

## Label corrections
- None: the catalog labels PeekMapNormal / PeekWaterDepth are right.
