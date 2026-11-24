# Landing nf346 b1, b2, b3, b3h, p1 (Account A, 2026-11-24): the GetDef<T> REREAD port (T3 nf341 D3, agent NF346)

## What changed
64 landed rows each gain one line, `#define FABLE_RETAIL_CDEFPOINTER_SETDYNAMICCAST_REREAD`, right after their
`#define FABLE_RETAIL_CDEFINITIONMANAGER_GETDEF_GLOBAL_BODY`. No header changes.

The define is the existing CDefPointer.h opt-in for retail's SetDynamicCast form (owner 0x0066C340). The same fix landed in
nf341_r2. It turns 69 leaf-called GetDef<T> pick-any copies from NEQ to EQ to retail. NF346's
`pending/2026-11-24_nf346_handback/evidence/fixed_by_copy.tsv` lists them by retail address.

The batches:

| Batch | Rows | Copies fixed |
|---|---|---|
| b1 | 21 | 24 |
| b2 | 20 | 21 |
| b3 + b3h | 19 + 1 | 20 |
| p1 | 3 | 4 |

0x008C64F0 was first held by a stale NF101 claim from 2026-11-19. That claim is now closed, and the row landed alone as b3h.

## Gates (per row, NF346 evidence in each batch dir; land_scripted recheck from the repo: 0 failed)
- **Grade:** RELOCATION_MATCH PASS before and after.
- **Pick-any:** both checkers, HEAD against new. No copy turned NEQ, none newly NEQ, and no leaf-called NEQ GetDef copy is left in the
  ACCEPT rows. Each batch's reversed control fails.
- **objsec:** the leaf section is identical in every row. The only other changes are:
  - the GetDef copies turning EQ;
  - their no-address SetDynamicCast helper bodies;
  - 33 dropped no-address Assign helpers. No reloc_bindings line names them, and nothing references them in 369 objects
    (`evidence/helper_refs_scan.txt`).

## Coordinator ruling Q1 (p1: 0x0069ADC0, 0x0082D3B0, 0x008338D0)
In these three rows the CCharString-overload GetDef copies stay NEQ: 0x004AB990 ×2 and 0x006D3E80. They were NEQ at HEAD, and the
define leaves their bytes unchanged. Under the NF340 row-change gate that is debt, so the rows land. The copies are queued as
T3 nf346.

## Left
- **40 rows skipped.** Retail's GetDef copies there are the 98 B size-mode body; REREAD alone gives 108 B. They are queued as T3 nf346
  `port (size-mode copies)`, and agent NF349 tries the size-pragma scaffolding.
- **0x00821A80:** its 0x0042B144 copy is NEQ, queued as T3.
- **Not compiled:** 12 excluded rows (T3, or Account B claims) and B's 0x00839940, which already landed with the define.
