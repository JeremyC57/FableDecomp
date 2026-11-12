# acctB hand-back, near-miss retry of chunk 2044: both animation rows matched as a pair

Chunk 2044 matched 0x00A9F540 and parked its blended twin 0x00AA0090. Two near-miss agents (Opus 5.5) continued in the
same directory; the second closed the pair. Re-verified by the acctB local coordinator. Nothing is landed.

| address | result | notes |
|---|---|---|
| 0x00AA0090 `?CalcBlendedMeshTransformations@C3DAnimationManager@@...` | RELOCATION_MATCH 2390v2390, test PASS (g++ and VC7.1) | **supersedes `2026-11-12_acctB_c2044/rows/parked/00aa0090.cpp`**. Grade `retail_asm` |
| 0x00A9F540 `?CalcMeshTransformations@C3DAnimationManager@@...` | RELOCATION_MATCH 2288v2288, test PASS (g++ and VC7.1) | **supersedes `2026-11-12_acctB_c2044/rows/00a9f540.cpp`**: same bytes, new source (helper text now identical to the blended row's). Grade `retail_asm` |

Coordinator re-verification (cl 13.10.3077 under Wine, `FABLE_HDR_OVERLAY=hdr/`): `coordverify/` (both grades, both
test runs, read-only vland: 2 wins, no warnings, no RELOC_IDENTITY / RELOC_CONFLICT / NOT_GENUINE). Purity and
padded-struct greps clean. I diffed the two row files from the first include to the row body: the helper texts and
defines are identical (one body per helper for both rows). `grades.log` has one line per grade (442).

## What closed it

The residue in 0x00AA0090 was the order of three x87 products inside the expanded `C3DKeyframe::BlendWith`, a tie that
depends on how much intermediate code the function carries. The closing change is the text of the inline `GFFastSin`:

```cpp
angle = angle * 1024.0f; long i = GFFloatToLongNear(angle - 0.5f); float frac = angle - (float)i;
i -= 256; i &= 0x3ff; float a = g_CosTable[i]; float b = g_CosTable[i + 1]; return (1.0f - frac) * a + frac * b;
```

Evidence for that text (FableWin debug sizes, reproduced by plain VC7.1 `/Od`): ours 35i/120b v FableWin 37i/121b; the
same naming on the sibling `GFFastCos` gives 32i/108b v FableWin 34i/109b, the identical +2i/+1b residual; Sin minus
Cos is 3i/12b in both builds and only the two-statement index step reproduces that. Release bytes are unchanged: the
agent regraded all nine landed `GFFastSin` users with this body (0x00A147E0, 0x00A13EB0, 0x00A145C0, 0x00AA3C50,
0x00AA3560, 0x00AA3390, 0x00A88D10, 0x00A22590, 0x00B05620) and `GFFastCos` user 0x00A14840: all RELOCATION_MATCH.
The match is not on an edge: it holds with +1 to +6 diagnostic dead floats added (diagnostics only, not in the row).

## Review items

- **`GFFastSin` is row-local text** in both rows (no shared header exists for it; the landed users carry their own
  copies with the one-expression body). If you make a shared header, this is the text with size evidence; the landed
  copies are byte-neutral either way.
- **FITTED among size-equal texts** (the retail grade decided): which reference `BlendWith` names
  (`const CQuaternion& from = Rotation;`, FableWin 48i/129b SAME) and which `PostApplyRelativeAnimationAlpha` names
  (`other_position`, 32i/88b SAME); the plain `CQuaternion::Dot` (both named-reference texts hit FableWin's 28i/73b
  but break the dot order in both rows); the named index / time / start / end / alpha locals, `bool relative`, and
  the early-return shape of the relative helpers.
- **PROVEN by size:** `GFGetInterpolatedValue` with a named delta (12i/28b), opt-in
  `FABLE_RETAIL_GFGETINTERPOLATEDVALUE_NAMED`; `SetSlerp` forced inline with `cosom = from.Dot(to)`.
- **Measured and rejected:** named-result `C4DVector` operators reproduce the FableWin sizes but change retail's
  release code (frame grows), so the opt-ins `FABLE_RETAIL_C4DVECTOR_NAMED_*` in `hdr/` are unused.
- Headers (`hdr/README.md`): the chunk 2044 overlay plus opt-ins `FABLE_RETAIL_C3DANIMATIONSTATE_ACCESSORS`
  (PDB accessors `GetAnimationIndex` / `GetAnimationTime`) and the interpolate opt-in above. Default texts unchanged.
- Asm evidence: the reached `fistp` sites are unchanged from chunk 2044 (six `GFFloatToLongNear` sites per row plus
  the floor site in `ConvertTimeToFrames`); `ASM_ATTEMPTS_TABLE.md` is the chunk 2044 table, copied here.
- Names: unchanged from `2026-11-12_acctB_c2044` (`NAME_FIXES.tsv` / `LABELS.tsv`).

## How to land

Both rows together from this folder (`rows/` + `hdr/engine/`, `payload.json`), in place of the c2044 folder's rows.
