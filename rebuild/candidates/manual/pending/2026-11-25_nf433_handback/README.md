# NF433 hand-back (2026-11-25): CGameScriptInterface round 2

This is round 2 of NF431's family. `NOTES.md` has the per-row recipe, the gates and the tool fixes.

## Landed: 28 rows (`batches/2026-11-24_landing_nf433_cgsi_b1`)
- **24 body ports.** Each RTTI-placed slot is now the real `CGameScriptInterface::<Method>` on `engine/CGameScriptInterface.h`. Callee names and argument order come from FableWin's /Od body, checked against retail's targets.
- **The 4 CWorld accessor debt rows** (0x0088F450, 0x0088F950, 0x0088E380, 0x0088F460) now call FableWin's inline CWorld accessors. Their selves and tests are unchanged.
- **One baked constant.** 0x0088F810 spells its string literal instead of the made-up `kName_0125fd40`.
- **Grades** are unchanged (10 MATCH, 18 RELOCATION_MATCH). Every test passes, and every mutant fails its test.
- **Name gate:** PASS on row and test.
- **No vftable emitted:** no `??_7` / `??_R` in any object.
- **Copies:** pk_all reaches no differing copy (0 NEQ).
- **Link:** /FORCE duplicates stay at 0.
- **Header opt-ins:** 15, all add-only, across 11 headers.
- **Equivalence:** the coordinator reran equiv_check copy against copy on the tree at landing time; 0 of 57,160 rows differ.
- **Table:** 9 adds and 14 deletes, all stand-in callee lines replaced by PDB spellings.
- **Released T3 lines:**
  - 18 nf324 port lines;
  - 0x0088F810's nf320 / nf322 / nF325 / nf326 lines;
  - the CWorld half of the nf431 debt line. Its IsInLimbo half stays open.

## Residue (`RESIDUE.tsv`, `T3.txt`; nf433 T3 family line)
| Group | Rows | What it needs |
|---|---:|---|
| TC family | 34 | GetTC / PeekTC map expansion and the TC class headers |
| Body ports not attempted | 12 | same recipe |
| Research | 6 | see the list below |
| On other T3 lines | 4 | |
| NF431's T1 caller respells | 8 | |
| NF431's tc_query rows | 5 | |

The six research rows:
- 0x006E72A0 can be ported once `CClassFactory.h` declares `Validate`.
- 0x00890960 needs the PDB data name of the static table at 0x01264B00.
- 0x006E7300 and 0x0088ECE0 need the camera class headers.
- 0x0088F980: retail inlines `SetGuildSealRecallPos`, which the header declares out of line.
- 0x0088FE90: retail multiplies by a float reciprocal where FableWin divides by 24.0.

## Name fixes (queued)
- 0x004349D0 should be `?EndLetterBoxMode@CDisplayEngine@@QAEXXZ`; it is the stand-in `?Deactivate@T@@QAEXXZ` now.
- 0x00A36FE0 is `?Validate@CClassFactory@@QAEXXZ`. Retail has no ICF, so the `InterruptAllNavigators` line there is debt.
- Five callers still use stand-in spellings, and 0x00654593 spells `EMiniGameType` as `long`.
