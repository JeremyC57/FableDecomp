# 2026-11-16 landing nf35: name-fix round 2, NF35 (191 landed rows, 51 renamed)

Owner: "Continue the naming fix queue with two agents until complete". Round 2 continues NF33's half: tags land1011fj ..
land1113fz, 464 entries (NF33's carry, partial and deferred, re-screened after round 1). Brief and records:
`naming/2026-11-16_agentNF35/`.

## Queue (464 entries; APPLIED.tsv lines up with QUEUE.tsv)

| Status | Entries |
|---|---|
| done | 74 (72 binders respelled) |
| done-already | 3 |
| blocked | 46 |
| partial | 3 |
| deferred | 2 |
| rejected | 1 |
| carry | 335 |

- **blocked:**
  - port 36, of which 11 are on rows NF35 opened and 25 are classes with no shared header (a stand-in may only be respelled onto
    a shared header);
  - authoring 9;
  - ICF 1.
- **partial:**
  - 0x00A5D5F0: five binders use hand-rolled views, and 0x00B99D90 was held by NF36;
  - 0x0097F690: caller 0x00980770;
  - 0x0097D8B0: three binders declare it on a derived stand-in.
- **deferred:** 0x0049F180 (row 0x00416953) and 0x005D9930 (row 0x005E4430). Both rows were held by NF36.
- **rejected:** FormatDateTime `PA_W1H`. Retail spells wide text `unsigned short`.
- **carry:**
  - 185 likely ports (stand-in rows where engine/<Class>.h exists; several similar rows moved in one to three grades, so the
    label is unproven);
  - 44 self rows;
  - 32 template / STL / free-function claims;
  - 21 not mangled;
  - 19 data globals;
  - 15 entries with wrong binders left;
  - 12 hot targets;
  - 5 reached but unchanged, each with its reason.

## Rows (191; ten tests changed)
- **Section 1:** 51 self renames (26 file moves). Each is checked against both PDB name sets, and the shared headers already
  declare most of the new names.
- **Section 2:** 181 caller respellings.
- **Tests:** ten were respelled to the new self names, with their PassPattern lines unchanged.
- **Recheck (`logs/recheck.log`):** all 191, graded from the repo after landing: 5 MATCH and 186 RELOCATION_MATCH, every test PASS.
- **Prep checks** (`land_naming_prep.sh`): no new local model of a shared-header class and no banned token. No overlap with NF36's
  rows (row claims).
- **Self-check:** NF35 caught its own new local model in 0x009624D0 (CCreatureAction_TrollGroundPound) before handing back and
  dropped it.
- **Header:** engine/CWStringParser.h `FABLE_RETAIL_CWSTRINGPARSER_INIT`, declaring `Init(const unsigned short*)` 0x00A0C8E0 for
  row 0x0048F50C.
- **Bindings:** rebind run, bindings_multi_check at baseline, funclet_includes_check reports 1,355 checked, 0 broken.

## Also found (not changed)
- 0x00772720 declares an unused `GetStockQuantity` for 0x007726B0; nothing binds it.
- The catalog names 0x00B38DB0 RedoMatrixConversion, but it is the twin UndoMatrixConversion.
