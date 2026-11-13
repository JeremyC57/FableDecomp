# 2026-11-13 landing fy -- the acctB hand-over: 138 of 140 matched rows landed (441,890 bytes)

Source: `pending/2026-11-13_acctB_FINAL/` (branch `FullDecomp-B`, merged into `FullDecomp`; it only touched pending folders). The lane
had matched 140 of its 189 reserved rows against its OWN header overlays, taken at older states of `rebuild/include`, while this branch
kept changing the same headers. `LANDED.tsv` lists every row with its length, route and compiler flags.

## How they were landed
1. **Baseline:** all 140 rows re-graded here against their own overlays: 140 of 140 match (the hand-over index carried length
   estimates; 25 oracle lengths were corrected from the rows' `// Retail:` lines and the grade).
2. **Gated driver** (`lane_tools/lane_merge/acctb_land_v2.py`), one row at a time: merge only the headers the row includes into today's
   headers, refuse anything that changes a default path, recheck the landed files that use a changed opt-in, strict lander. 79 rows.
3. **Hand merges** for the 61 rows the automatic merge could not do (both lanes changed the same line, wrote the same header, or used
   one opt-in name for different content): five Opus agents, each in a PRIVATE copy of the headers (never the repository), staged 59
   rows; each group's headers were then merged into the repository under the same gates and its rows landed with the strict lander
   (`acctb_integrate.py`). Agent reports: `agent_reports/` (mesh, render) and the review notes below (ai, game, open).
4. **Whole-tree proof** (`equiv_check.py`, `recheck_rows.py`): all 56,185 landed rows were preprocessed with gcc cpp against the headers
   as they were before this landing (commit 0e200d83d) and as they are now. 56,016 earlier rows see token-identical compiler input,
   so their bytes cannot have changed. The 31 earlier rows whose input changed and all 138 new rows pass a VC7.1 recheck (byte match
   and behaviour test): 169 of 169 (`logs/final_recheck_good.txt`, `logs/equivalence_changed_input_rows.tsv`).

## What the proof caught (all repaired by hand, commit c1bfaece6)
The automatic merges had damaged seven shared headers in ways a one-opt-in-at-a-time check cannot see:
CTCHeroReceiveItems.h (DISPLAYINGITEMS members moved out, broke 0x00492973), CRightHandedSet.h (GetRight declared twice for 0x00BC2480),
CQ_HobbeCaveScript.h (a class lost its closing brace, broke 0x00D91960), CTCHeroStats.h (RENTMONEY body moved under a `_B` copy, broke
the just-landed 0x006ACA30), CEngineResourceManager.h (a stray `#endif` and a missing one that balanced by accident, broke 0x00B3C5D0),
CQuaternion.h and CTCInventoryQuests.h (two emptied opt-in blocks without a landed user). Two agents had also reported duplicate
`Rotate` declarations in C2DVector.h / C3DVector.h and a duplicate `Reset` in CIVCountedPointer.h (0x00A87700, 0x00A69B70 broken);
their merged headers fixed those. A checkpoint commit taken while the driver was in the middle of a row had also recorded headers
of a row that then failed (removed in 1dc81baa0). Lesson: the gates need the whole-tree equivalence check, not per-opt-in checks.

## Not landed (2)
0x005CE0E6 CTCInventoryStats::ConstructPersonalityList and 0x005D1F0C ConstructPersonalityListPC match only through
`CTCHeroStats::GetNumberOfBuildingsOwned()`, which the lane's own header calls "FITTED, descriptive name (in no PDB)". Left parked
(`pending/2026-11-13_acctB_parked/`, merge work kept in the lane folders) until that helper has evidence or the rows are respelled.

## Review items carried forward
- **`_B` opt-in names (26 in the headers).** The driver renamed an incoming opt-in whenever this branch already used the name. Most are
  plain aliases (`#if defined(X) || defined(X_B)`); respell those rows to `X` and drop the alias. Three select different content and
  need descriptive names: C3DVECTOR_MAGNITUDE2D_B (two-statement body), CAISTATEGROUP_JACKDRAGON_B / _B2 (extra declarations),
  CRGBCOLOUR_GETD3DCOLOR_B (loads the dword; this branch's packs four bytes) and CMATRIX3X4_SETMULTIPLY_B.
- **Fitted constructs the lane flagged itself:** 0x00563920 `if (frame)` (Inline-split note), 0x006D1250 locals and a noinline device,
  CMatrix3x4 MULTIPLYNOTRANSLATION ("FITTED, needs a ruling"), 0x00B5FAB0 `volatile` (owner ruling 2026-11-13; `// Retail-volatile:`
  line checked against the oracle: without it the row differs at the same size).
- **One retail class modelled twice:** CAIStateGroupBase::CAIFilterBase (FINDAGUARD and FILTER opt-ins); CInventoryDef FULL / ASSIGN.
- **Wrong parameter type in an older declaration:** C3DMeshStats DUMMYOBJECTS / C3DMesh2 HELPERPOINTS declare the out parameter of
  GetDummyObjectPositionAnimated as `C3DVector&`; both PDBs say `C3DDummyObject&`. Queued.
- **Made-up names to check against the rule:** five render-switch globals `g_particleMesh..._0139b7a0..b0` and a row-local
  `GFDebugPDynamicCast` template (0x00BC25F0 / 0x00BB8EF0).
- **Local models fixed at landing:** 0x00BB5040 (CEngineClothRenderer) and 0x004959E0 (CFrontendGameComponent) now use the shared
  headers. The empty `class CThingPlayerCreature : public CThing { };` stub in three rows is the spelling 540 landed rows use.
- **Lander warnings:** RELOC_CONFLICT on GetExtendedSightRadius (0x00744950 here, 0x007448F0 in landed 0x00864620: the landed row
  binds the name to GetSightRadius) and on `_Rb_tree<long>::find` (0x00616580 / 0x00B2EB20); STANDIN_LEAF on 0x00A079B0 was a
  catalogue name without its method (fixed). Many RELOC_NAME warnings are manifest labels the lane's NAME_FIXES correct.
- **Name fixes:** `NAME_FIXES_acctB.tsv` (420 de-duplicated lines from the lane's folders) is queued under tag `land1113fy`;
  `MANIFEST_NAME_SYNC.tsv` lists the 31 manifest rows respelt to the names they landed with.

Checks: bindings_multi_check at baseline; funclet_includes_check 1317 checked, 0 broken.
