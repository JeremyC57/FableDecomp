# Chunk 875 header overlay (hdr/engine/)

Copies of `rebuild/include/engine/<X>.h` with ADDITIVE `#ifdef FABLE_RETAIL_*` opt-ins only; the default path (no new macro
defined) is byte-for-byte the repo header. Every copied header's includes resolve through the include path (`engine/...`,
`stlport/...`, `rebuild_abi.h` at rebuild/include), so no sibling copy is needed (include closure checked).
`hdrp/` is a probe-only overlay (PROBE_* macros) and is NOT for hand-over.

| Header | Opt-in | What it adds | Evidence |
|---|---|---|---|
| C2DVector.h | `FABLE_RETAIL_C2DVECTOR_ASSIGN3D` (nested in `_FROM3D`) | `void operator=(const C3DVector&)` (X, Y copied as dwords) | FableWin.pdb `??4C2DVector@@QAEXABVC3DVector@@@Z`; 0x00A1EDB3 `GoalPosition = destination` stores straight into the member (no frame temp), 0x00A1FA9A `LastWayPointVisited = position` |
| C2DVector.h | `FABLE_RETAIL_C2DVECTOR_ISNONZERO_FORCEINLINE` (nested in `_NORMALISE`) | `IsNonZero` same body, `__forceinline` under `_MSC_VER` | no out-of-line IsNonZero anywhere in Fable.exe; every use expanded (IsRouteClear 0x00A1A321, both expansions in 0x00A1E5C0); VC7.1 calls a plain-inline copy out of line from IsRouteClear-sized callers. Inline-budget device per the owner's 2026-11-08 rule (no flag reaches it: /d2inlS10/15/30, /d2inlT80/150 tried) |
| C2DLineF.h | `FABLE_RETAIL_C2DLINEF_SQUAREDDIST` | inline `C2DLineF(const C2DVector&, const C2DVector&)` and `float GetSquaredDist(const C2DVector&)` decl (0x00A57770) | FableWin.pdb `??0C2DLineF@@QAE@ABVC2DVector@@0@Z`; Ego_r + FableWin `?GetSquaredDist@C2DLineF@@QAEMABVC2DVector@@@Z` (landed row 0x00A57770) |
| CNavRegion.h | `FABLE_RETAIL_CNAVREGION_ISCONNECTED` | `bool IsConnectedToRegion(const CNavRegion&) const;` (0x00A77780) | Ego_r + FableWin `?IsConnectedToRegion@CNavRegion@@QBE_NABV1@@Z` |
| CNavigatorManager.h | `FABLE_RETAIL_CNAVIGATORMANAGER_ISACTIONPOINT` (nested) | `bool IsActionPoint(const C2DVector&) const;` (0x00A150A0) | Ego_r + FableWin `?IsActionPoint@CNavigatorManager@@QBE_NABVC2DVector@@@Z` |

Only the parked row 0x00A1E5C0 (differ/) uses these opt-ins; no matched row depends on them.
