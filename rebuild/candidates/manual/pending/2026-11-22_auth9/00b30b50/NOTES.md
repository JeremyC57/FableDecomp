# 0x00B30B50 CEngineCamera::SetCameraState: RELOCATION_MATCH (1499 B code, catalog span 1552), no flags, test PASS (AUTH9, 2026-11-22)

Source `CEngineCamera_SetCameraState_00b30b50.cpp`, test `CEngineCamera_SetCameraState_00b30b50_test.cpp` (vt.py PASS). Log: LOG.md.

## Route
- Text: Codex pass 26's SetCameraState from 0x00B31160's TU, verbatim, with that row's header opt-ins (all already in the shared headers;
  no header change). ProcessDebugScript-only opt-ins, includes and constants dropped (regraded: same bytes).
- The new row needs no TU-mate (it calls only SetupGamut / RefreshD3DTransforms, declared). The reverse is not true: 0x00B31160 with
  SetCameraState declaration-only measured DIFFER(886v886) (one swapped instruction pair at 0x00B313A4), so 0x00B31160 keeps its copy as a
  same-TU strong TU-mate (GOTCHAS nf96a); the full link gets an LNK4006 with identical bytes.

## Checks
- Leaf relocations (`../tools/leafrel.py`): externals ?GPVideoOptions@NEngineInternal@@3PBVCEngineVideoOptions@@B (BOUND 0x01436EB0),
  ?RefreshD3DTransforms@CEngineCamera@@QAEXXZ (BOUND 0x00B2FC50), ?SetupGamut@CEngineCamera@@AAEXXZ at 0x00B2FD60 (no binding line yet:
  first witness; labels_landing_2026-11-07t.tsv names 0x00B2FD60 CEngineCamera::SetupGamut from Ego_r + FableWin, exact spelling in both
  name lists); float literals 0.5 / 1.0 / 0.0 / 0.75. PROBLEMS 0. Pick-any: no function COMDAT in the object maps to another address.

## Names
- Leaf: exact string in ghidra_out/egor_pdb_names.tsv and fablewin_pdb_names.tsv. Callees as above (two PDBs each). Parameter names
  (desc, rebuild_gamut, refresh_d3d) and the locals aspect_size / horiz_fov are Codex's; not re-measured against Ego_d here. No descriptive
  names, no NAME_FIXES.
- Side finding (not applied, not a landed name): ghidra_out/labels_forge_autoname4.tsv labels 0x00B2FD60 `CPlane_InitializeFromCorners`
  (autoname); the PDB name is CEngineCamera::SetupGamut.

## Test
Self-contained, the row text verbatim (/Od), members through `private public`, SetupGamut / RefreshD3DTransforms counted stubs. Cases:
StateValid false (camera memory untouched, no calls); no video options (4:3 window, FOV factor 1); 16:9 options (factor 1.09375, aspect
16/9); a zero aspect component ignored; Use2DFOV (both scales from their own FOVs). Each checks the window fields, view transform
translation and PostScale, both inverses (round trip of a point), z-buffer factor / offset, projection matrix and the two flag-gated calls.

## 0x00B31160 (recheck, `../00b31160/`)
P1.patch: only the comment over its SetCameraState copy, now a Fidelity TU-mate line. RELOCATION_MATCH; its copy at 0x00B30B50
RELOCATION_MATCH; pick-any 1 EQ; repo test PASS. git apply --check OK.
