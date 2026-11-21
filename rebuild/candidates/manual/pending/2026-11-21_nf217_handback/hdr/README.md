# NF217 header overlay (2026-11-21)

## engine/C3DMeshObject.h
Base: the repo working-tree copy at the start of the run (`orig/engine/C3DMeshObject.h`). No other agent overlays it (AUTH4's
`$S/w22/auth4/hdr/` has no copy; no pending hand-back carries one).

Added (add-only, each block wholly inside its own `#ifdef`):
- `FABLE_RETAIL_C3DMESHOBJECT_GETNAMES` (q12368 / q12369 / q12370; rows 0x00A35B30 / 0x00A35AF0 / 0x00A35B50): an include of
  `stlport/fable_containers.h` after `fable_string.h`, and the three public const declarations GetAllHelperPointNames /
  GetAllDummyObjectNames / GetDummyObjectNamesStartingWith over `std::vector<CCharString>&` (both PDBs; addresses in the block comment),
  placed after the GetPositionMatrixFromRHSet block, before `protected:`. Declarations only, no layout change.

Default path: token-identical to the base (the diff is only the two `#ifdef` blocks). pin_asserts.py: `pinned 0` (the one assert is
already `FABLE_STATIC_ASSERT_AT(47, ...)`). Landed includers (`grep -rl C3DMeshObject.h rebuild/src/compiled`): 0x0075B000, 0x00724630,
0x00C15A70, 0x00A36100, 0x004C0560 (and rebuild/include/engine/NGraphicAppearance_CTCGraphicAppearance_HelperPoint.h); none defines
`FABLE_RETAIL_C3DMESHOBJECT_GETNAMES`, so the opt-in changes none of them.
