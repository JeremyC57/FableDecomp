# Landing nf222 (Account A, 2026-11-22): the CCountedPointer<T>::ReleaseData family, whole (42 rows + 1 header opt-in pair)

Agents NF221 (ports, census) and NF222 (the open rows, the pick-any pass, the whole-family recheck). Hand-backs:
`pending/2026-11-22_nf221_handback/` and `pending/2026-11-22_nf222_handback/`. Entries q12226-q12229 and q12402 (q12403 landed as nf221).

- **42 rows land, all RELOCATION_MATCH + PASS.** Recheck (`logs/recheck.log`): 69 rows = the 42, the 4 nf221 rows, and 23 unchanged rows
  that reference family addresses. All 69 pass on both trees (HEAD control: `logs/recheck_head.log`). The RELOC_CONFLICT on
  `??1<CGraphicDataBank>` is gone.
- **25 self renames** onto FableWin.pdb names, each with a second source (RENAMES section 1). Files and catalog names move with them.
  - The 8 ReleaseData bodies were catalogued as `~CCountedPointer`; the CFont pointee is CFontBank.
  - The jmp thunks 0x004194B5 / 0x00429BF9 become `~CCountedPointer`; the ShareData, operator= and Reset stand-ins get their real T.
  - 0x0041D198 becomes SetMeshBank.
  - The AcquireFont self leaves take the const QBE spelling (Ego_d, Ego_r and FableWin agree); their only callers, 0x009E2C10 /
    0x009E2BA0, are respelled in the same landing.
- **66 caller respellings** (RENAMES section 2). Every family address now has one spelling, 0x0041947A included (census: hand-back
  `work/census_final.txt`).
- **0x005BFF07:** fully ported onto engine/CCountedPointer.h (pointees from Ego_r.pdb), as the coordinator ruled. Its `??1` copies
  equal their owners 0x005C07DF / 0x005C07FB / 0x004194B5 (5 B, 0 diffs), and the `??1<void>` copy is gone.
- **Header** engine/CCountedPointer.h: add-only opt-ins `FABLE_RETAIL_CCOUNTEDPOINTER_DELETEDATA_DECL_ONLY` (the five Reset rows) and
  `FABLE_RETAIL_CCOUNTEDPOINTER_CONVERTING_ASSIGN_DECL_ONLY` (0x0042DDB3).
  - No landed row used either macro before, so every other user preprocesses as before (pin_asserts pinned 0).
  - Each keeps a pick-any copy out of a TU where it would differ from retail: DeleteData would be a plain delete instead of the virtual
    one; the converting assign would get `push ebp`.
- **Pick-any v Fable.exe** (`logs/pickany_vs_retail.tsv`, every copy at HEAD and now):
  - 38 copies equal their owners; 0 new copies differ.
  - 14 differed at HEAD and still do (nf203 debt).
  - 142 have no retail owner. Among them, the new 5-B `??1<CFontBank>` copies: retail has no out-of-line one, and the symbol already
    had 44 / 53-B bodies at HEAD.
  - 15 `??1<T>` copies that were 44-B bodies at HEAD are now the 5-B thunks retail has.
- **Link** (`logs/link_check.txt`, 69 objects): duplicates 6 at HEAD, 1 at landing. The remaining one, `CBasicString::Compare`
  in 0x00A09F20 / 0x009F83D0, is unchanged and outside the family.
- **reloc_bindings** (`logs/TABLE_CHANGES.tsv`): 42 lines deleted and 20 added, matched by content.
  - NF222's optional `?Dtor@CManager@@QAEXXZ -> 0041e641` was held (it would add a second spelling); queued.
  - At 0x009F83D0, the 0x0042DDB3 stand-in line is replaced by the callee's self leaf OpenGraphicBank, so the count stays at 3.
- Queued (tag land1122nf222: q12551 q12552 q12553 q12554 q12555 q12556 q12557 q12558 ):
  - the three DeleteData self rows, and the GetFont / OpenGraphicBank binders still on stand-ins;
  - 0x0041E641 ~CManager's second spelling;
  - the 0x00416C8A ShareData / `??4` copy debt;
  - the unproven `mutable` on the AcquireFont Font member.
