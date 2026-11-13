# acctB hand-back, near-miss retry of chunk 2041 (1 previously parked row: matched)

The authoring agent parked this row 4 instructions from retail; a near-miss agent (Opus 5.5) continued in the same
directory and closed it. Re-verified by the acctB local coordinator. Nothing is landed.

| address | result | notes |
|---|---|---|
| 0x006AB820 `?ResolveFacingDirection@CThingPlayerCreature@@IAEXXZ` | RELOCATION_MATCH 2328v2328, test PASS (g++ and VC7.1) | **supersedes `2026-11-12_acctB_c2041/rows/parked/006ab820.cpp`** (was DIFFER 2328v2328, 652 of 656 instructions). 9 grades in all (7 authoring + 2); the row source is unchanged except comments -- the fix is in a shared header |

Coordinator re-verification (cl 13.10.3077 under Wine, `FABLE_HDR_OVERLAY=hdr/`): `coordverify/` (grade, both test runs,
read-only vland: 1 win, no RELOC_IDENTITY / RELOC_CONFLICT / NOT_GENUINE; one `WARN RELOC_NAME`: `DELTA_NOTIONAL_ZERO`
0x0129BA3C is labelled `EPSILON` in the catalog). Purity and padded-struct greps are clean.

## The header change this row needs (review before landing)

`hdr/engine/C3DVector.h` changes the body of the shared inline `C3DVector::InitialiseFromAngles` inside the existing
`FABLE_RETAIL_C3DVECTOR_ANGLE_BODIES` branch (one body for all rows, not a per-row opt-in):

```cpp
float cosYZ=GFCos(angleYZ);
X=GFSin(angleXY)*(cosYZ*length);
Y=GFCos(angleXY)*(cosYZ*length);
Z=GFSin(angleYZ)*length;
```

in place of the hand-expanded `(float)(sin(angle*(double)6.2831855f)*(cosYZ*length))` lines. The conversions back to
float are implicit, so there is no `(float)(...)` PAREN tuple between the Y `fmul` and its `fstp`; that tuple was the
whole residue of this row (0x006ABB81..0x006ABB8F).

Evidence that this is the original body:
- FableWin.pdb: `?GFSin@@YINM@Z` 0x01DCB5D0 is the COMDAT directly after `?InitialiseFromAngles@C3DVector@@QAEABV1@MMM@Z`
  0x01DCB530, i.e. InitialiseFromAngles is where GFSin was first instantiated (GFCos 0x01D9E180 came from an earlier TU).
- Every user matches under it. **Regraded by the coordinator under `hdr/`, all RELOCATION_MATCH:** the four landed
  header users 0x00A22CA0, 0x00A0C570, 0x00746EC0, 0x008BD3F0 (all the header users in `rebuild/src`) and the pending
  same-TU row 0x006ACA30 (`2026-11-12_acctB_c2039`).
- Retail keeps an unreferenced, uncatalogued out-of-line copy of InitialiseFromAngles at 0x0068D4C0 (69 bytes; in
  `LABELS.tsv`). Both the old and the new text grade RELOCATION_MATCH against it (agent's measurement), so it confirms the
  expression tree but does not choose between the spellings.
- Rejected variant (agent): `GFSin(xy)*cosYZ*length` without the inner parentheses matches this row but breaks
  0x00A14040, 0x00A14260 and 0x00CBE693.

**Not regraded** (they need their own overlays): the pending rows that define `FABLE_RETAIL_C3DVECTOR_ANGLE_BODIES` --
0x007D3820, 0x007F26B0 and the parked 0x0097BDA0. By grep none of them calls InitialiseFromAngles or sin / cos; please
regrade them after merging the header.

## Proposals for landed rows (`proposals/`; optional, nothing in the repo was edited)

Four landed rows carry a LOCAL copy of the old body text: 0x00A14040, 0x00A14260, 0x00A0C450, 0x00CBE693. They match
with either text. To keep one text everywhere the agent re-spelled the local copies with GFCos / GFSin; each proposal
grades RELOCATION_MATCH (`*.grade`) and the unchanged landed tests pass under g++ (`TEST_RESULTS.txt`). I did not
regrade the proposals myself.

## Other review items (carried over from `2026-11-12_acctB_c2041/README.md`)

- `GFGetRoundedSignedAngle` is `FABLE_FORCEINLINE` in the `GFAngles.h` overlay with an `// Inline-budget:` line in the
  row (plain-inline cost 87, score 377).
- `NAME_FIXES.tsv`: 0x006A99D0 is `?GetMaxTurnSpeed@CThingPlayerCreature@@IBEMXZ` (landed spells it `QAEMXZ`).
- Landed 0x006A9690 names physics slot 74 `SetRHSet` in its local model (it is slot 75); catalog 0x006A9520 is probably
  `CThingPlayerCreature::GetTargetPosition` (one source).

## How to land

`rows/006ab820.cpp` + `rows/006ab820.test.cpp` with `hdr/engine/` (it is c2039's overlay plus this row's files, so it
also serves 0x006ACA30), the C3DVector.h body change merged into `rebuild/include/engine/C3DVector.h`, then
`payload.json` through your usual vland. `LESSONS.md`: a residue that shows up in only one user of a shared inline can
mean the shared body is fitted -- look at the FableWin COMDAT right after the inline's debug copy, it names the helper
the original body called.
