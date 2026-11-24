# Landing nf335 b1 (Account A, 2026-11-24): 22 NF324 Q2 rows onto their PDB members, with their moved tests

Agent NF335 took NF324 Q2: 81 rows whose tests call the flattened name. It moved 22 rows; the other 59 stay on their nf324 T3 lines with a
measured reason each (`pending/2026-11-24_nf335_handback/STATUS.tsv`, NOTES.md first). The 59 are 34 Copy thunks and 1 held row: 0x0046F49F,
whose operator= row already defines the PDB `Copy`. The remaining 24 are other ports. Tools: `naming_batch/agent_tools/nf335/`.
- **Rows:** each moves from its flattened self to the PDB member, 11 by the local route and 11 by the switch route. There are no header changes.
  - HEAD and new were graded at one work path: objcheck ALL_EQ, except 0x004AE890, which is ALL_EQ plus exactly libcp (NF318 Q1).
  - The graded symbol is the PDB symbol for every row.
  - `land_scripted.py --release-t3 nf324` rechecked all 22 on the landed tree: 11 MATCH PASS and 11 RELOCATION_MATCH PASS. Their nf324
    T3 lines are closed (`T3_CLOSED.tsv`).
- **D1, tests:** land_scripted neither copies nor restores tests. The coordinator re-diffed the agent's `orig/` rows and tests against the
  repo, then copied the 22 moved tests over the landed ones before the landing; with no row held, none needed restoring. For every row,
  the control (the HEAD source with the moved test) fails to link.
- **D2:** 0x00697700's landed source carries a retail byte-dump comment, so its HEAD entry was rebuilt from the repo (D-f).
- **D3, accepted:** the test-local names `<Class>Layout` and `CGraphicDataBank_TestCaller` are made-up descriptive names. They are used only
  in tests and bind to no retail address.
- **D4, accepted:** the test-only changes are:
  - real zero-initialised storage in place of fake object addresses, which removes UB;
  - header switches defined in two tests;
  - one stub member on a class model.

  Each call goes through the ruling's cast form, `((Class*)(&obj))->Class::Method(args)`.
- **Debt, pre-existing:** the landing recheck shows two RELOC_NAME warnings for made-up callees that this landing did not change, now T3 `nf335`:
  - 0x004644FA's `base_clone` at 0x00463950, labelled CThingBaseDef::operator=;
  - 0x006A40B0's `_JR_Helper@8` at 0x00687D50, labelled IsGameActionEventInQueue.
- **Copy thunks, port idea for the 34:** `*this = *(const Class*)other;` with operator= out of line, landed as a unit with each class's
  operator= row (NOTES.md).
