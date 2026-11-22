# AUTH11 (Account A authoring agent, 2026-11-22): the two bounded steps of the 2026-11-22 advisor rulings

Brief: `BRIEF.md`; state: `STATUS.tsv`. Grader outputs and objects are left out.
- **0x00A4EFC0: to the owner** (`00a4efc0/OWNER_PACKET.md`).
  - Check (`CHECK.md`): Ego_d's AllocateType local `overflow_check` is not assert-only (line 25, VC10-calibrated), and FableWin.pdb
    records it in all 12 instantiations. Ego_r's release record optimised it away.
  - The pre-registered test c1 (b3 + Ego_d's exact AllocateType shape, opt-in in `hdr.diff`) stays at residue 16 with no flip. The three
    users break as they did under AUTH2's V13 (35 / 15 / 9), so acceptance is not met.
  - b3 stays the recorded best text.
- **0x008E18D0: measured** (`008e18d0/MEASURE.md`).
  - GetTC wrapper calibrated on Ego_d's 74 B. 47 of 54 Ego_d lines equal in e5, against 37 in e4.
  - Corrections C1-C5 (GetParentThing reads, storm-rotation lines, `rhs` order, the VC6 for-scope idiom, an Ego_d GetTC wrapper opt-in).
  - The idiom moves the inline f at every site, but the main contradiction remains.
  - The one stock grade: DIFFER(1948v2073). It cannot be ranked against e4 (different configuration, bundled corrections).
