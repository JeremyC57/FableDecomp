# Landing auth26 unit 1 (2026-11-25): 0x005DCC00 CAnimationSet::GetRandomAnim plus its two vector helpers

AUTH26 hand-authored this unit under the owner's orders: no switching, no re-parking, hand authoring, and names and headers right as authored. Agent files are in `pending/2026-11-25_auth26/005dcc00/`, covering the LOG, NOTES, the gate runs and the unit README.

## Rows
| row | what | grade | test |
|---|---|---|---|
| 0x005DCC00 `?GetRandomAnim@CAnimationSet@@QBE...` | new row, parked since 2026-11-10 at DIFFER(278v264) | RELOCATION_MATCH 264 B, strict headers (`logs/vland_land_005dcc00.log`) | PASS: 10 cases, linked to the row object; 3 mutants fail it |
| 0x005E15E0 `vector<const pair<unsigned long,CAnimationEntry>*>::reserve` | self respell from `vector<const CAnimationEntry*>` | RELOCATION_MATCH 179 B (HEAD also RELOCATION_MATCH) | PASS (new test on the real element type) |
| 0x005E1500 the same vector's `_M_insert_overflow` | self respell from the stand-in `CUnprovenElement_005e1500` | RELOCATION_MATCH 216 B (HEAD also RELOCATION_MATCH) | PASS |

## How it matched
- **Register-allocation residue:** closed by making the two vector helpers' bodies visible (still out of line).
- **Statement order:** taken from FableWin.exe's /Od build of the same function.
- **Asserts:** written as `FABLE_RETAIL_ASSERT`.
- **Opt-ins:** existing stlport opt-ins only (`FABLE_RETAIL_STLP_VECTOR_OVERFLOW_OUTOFLINE`, `_RESERVE`, `_ALLOCATE_AND_COPY`, `_C`, as on 0x0077E580). There are no header changes and no flags.

## Names
**Element type evidence:**
- Ego_d.pdb: both retail callers (0x005DCC00 and GetRandomLoadedAnim 0x005DCD10) declare `found_anims` as the CVectorMap const_iterator vector (type 0x3AEEC).
- Ego_r.exe: both functions call only that instance.
- Retail: the `const CAnimationEntry*` instance's overflow is a different function, 0x005E1420.

**Gate:** `name_gate.py` passes for all three. Row 1 failed 2 before the respells and passes after (`logs/NAME_GATE_*`).

**Bindings:** `reloc_bindings.tsv` had no line at either helper. The new row adds the two first witnesses, under the corrected spellings.

**Link check (`logs/LINK_CHECK.txt`):** the three objects link with no duplicates, and the control fires. The pick-any copies in 0x005DCC00's object are byte- and relocation-identical to the owner rows' objects and to retail.

## Landing
- **Respells:** `nf352_land.sh` (land_scripted); orig equals the repo.
- **Catalog:** both relabelled; 0x005E15E0's Module and file name moved to the real element type.
- **New row:** `vland.py --land` with `LANDVERIFY_STRICT_HEADERS=1`.
- **Records:** self_symbols lines added for all three; 0x005DCC00 removed from UNMATCHED_ROWS.tsv.
