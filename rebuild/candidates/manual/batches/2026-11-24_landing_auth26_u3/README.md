# Landing auth26 row 3 (2026-11-25): 0x006C0B20 CTCBuyableHouse::FindPosToCreateNewVillager

AUTH26 hand-authored this row (its continuation session, after AUTH26's context filled) under the owner's orders: no switching or re-parking, hand authoring, names and headers right as authored. Agent files: `pending/2026-11-25_auth26/006c0b20/` (LOG, NOTES, g0 / g1, the three mutants, NAME_GATE).

| row | grade | test |
|---|---|---|
| 0x006C0B20 `?FindPosToCreateNewVillager@CTCBuyableHouse@@AAE_NAAVC3DVector@@@Z`, parked since 2026-11-11 at DIFFER(383v368) | RELOCATION_MATCH 368 B, strict headers, matched on grade 2 (`logs/vland_land_006c0b20.log`) | PASS: 8 cases (A-H) worked out from the disassembly, linked to the row object; mutants m1-m3 fail it |

## How it matched
- FableWin.pdb's /Od build of the function (569 B, the same as Ego_d's code size) gave every callee's name and the statement order. Its pieces fit Ego_d's line table 937-958, and the source uses Ego_d's locals as they are (`ret_pos`, `phero`, `entrances`, `it` / `end_it` in an inner block).
- g0 wrote those statements on the shared headers (engine/CTCBuyableHouse.h, engine/CThingSearchTools.h), with the GetAllThings template body in the file and its filter call out of line; the function matched on that first grade. g1 added three existing opt-ins so the file's copy of GetAllThings also equals retail 0x006C1DD0.
- The distance constant (400.0f, retail 0x0125D824) is behind a relocation the grader masks: a 20.0f mutant (m1) also grades RELOCATION_MATCH. The behaviour test and the pick-any byte compare are what pin it.

## Names and headers
- `name_gate.py`: PASS (rerun by the coordinator on the landing text). Every name is the exact PDB spelling from two sources (NOTES.md table); no descriptive names, no local classes, no typed-in addresses.
- The 8 copies in the object that landed rows bind to retail addresses are byte- and relocation-equal to retail (NOTES.md).
- Header: one add-only opt-in, `FABLE_RETAIL_CTCBUYABLEHOUSE_FINDPOSTOCREATENEWVILLAGER` in engine/CTCBuyableHouse.h (declares the private member only; default path unchanged; pin_asserts pinned 0). The 38 landed includers regrade RELOCATION_MATCH / PASS on it (`logs/includer_recheck_CTCBuyableHouse.log`).
- Name fix queued, not applied: landed 0x006C1DD0's self is a made-up class spelling; T3 line `auth26 0x006C1DD0 respell`.

## Landing
`vland.py --land` with `LANDVERIFY_STRICT_HEADERS=1`; self_symbols line added; 0x006C0B20 removed from UNMATCHED_ROWS.tsv.
