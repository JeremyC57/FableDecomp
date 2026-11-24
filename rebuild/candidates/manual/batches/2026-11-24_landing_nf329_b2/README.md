# Landing nf329 b2 (Account A, 2026-11-24): 75 CCombatActionBase-family rows on 34 new generated opt-in class models; NF329 complete

NF329 continued (rulings and route: `batches/2026-11-24_landing_nf329_b3/README.md`; hand-back `pending/2026-11-24_nf329_handback/`).
- **Headers first:** 34 headers, the same form and checks as b1 (`HEADERS.tsv`, `PROBES.tsv`, `EQUIV_CHECK.tsv`, `EQUIV_CONTROL.tsv`).
  - `hdrcheck2` printed default=same for all 118 NF329 headers.
  - After this batch every header in the repo equals the hand-back's `hdr/`.
- **Rows:** 75 rows, rechecked by `land_scripted.py --release-t3 nf324`; 75 nf324 T3 lines are closed.
- **D-f:** 0x008CA950's landed source carried a retail byte-dump comment, so the hand-back left out its HEAD entry. The coordinator rebuilt
  that entry from the repo. Its diff against the new source is only the rename, the header move and the dropped comment. The landed row no
  longer carries the comment.
- **Includers:** the 18 pre-existing includers recheck PASS on the full tree (`logs/includer_recheck.log`).

## NF329 in total
- 301 rows over 4 batches (b3 60, b4 86, b1 80, b2 75), 118 headers; 0 held, 0 recheck failures.
- 301 nf324 T3 lines closed.
- **Queued:**
  - the 95 thin-remainder Q1 rows (CCreatureActionBase, CSubDefClassBase, ...). Several chain onto the b1 / b2 models; they keep their
    nf324 T3 lines;
  - a neutral `FABLE_RETAIL_CCOMBATACTIONBASE` guard widening (NF322 Q1 form), to replace the `FABLE_RETAIL_KRAKEN_ROAR` selection in
    the CA `// Needs` lines (D-d);
  - D-e: six CA headers also sit in parked pending overlays (hand732, handH40i, `2026-11-13_acctB_parked`), which must rebase onto the
    landed headers.
