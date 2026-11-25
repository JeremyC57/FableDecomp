# NF442 hand-back (2026-11-25): T1 list A caller respells, round 2

`NOTES.md` has NF442's method and gates, and `RESIDUE.tsv` (1,576 lines) has what is left with reasons.

## Landed: 98 caller rows in 7 batches (p1, b1, q2b, b2, b3, q2c, b4)
- **What changed:** 104 callee rebinds onto 43 T1 list-A targets. No header changes, and no row's own self changes.
- **Rulings used:**
  - NF438 q1: friend lines;
  - NF438 q2: test stubs respelt with the row;
  - r2: a qualified destructor call;
  - r3: bodiless classes, booked as header debt for 32 rows and 16 classes in `T3.txt`.
- **Cross-batch rewitness lines:** 9, applied afterwards.
- **Tracker:** 20 addresses split_fixable → agree_pdb, none worse.
- **Coordinator checks:** 0 HEAD-payload mismatches before landing; baselines unchanged; descriptive_check 0 FAIL.

## Not applied yet
- **`STALE_TABLE.tsv` (52 table-only deletes, 27 targets).** The advisor (owner delegated) asked for a witness check first. For each line, take the witness the table names now, since NF443's rewitness lines landed after NF442's census, and confirm that row is in NF442's compiled set. Then apply the file as its own step with its own tracker diff.
- **q442a, `tplalt_hold/` (29 rows).** VC7.1 adds one `/alternatename` token per member-template callee. Grant if a dump shows the token is inert:
  - the relocation carries the target's self name, not the alias;
  - each callee name is defined by its target's own object;
  - the alias name is bound or defined nowhere;
  - the `.drectve` delta is exactly one token per rebound callee, with every other section equal.
- **q442b (107 lines):** port work, not a ruling. These rows define a local body for the stand-in callee.
- **Other holds:**
  - b3_hold (8 rows): 6 destructors named only in FableWin; 2 rows whose targets 0x004E0D7E / 0x004E424B share one spelling.
  - p1_hold / q2b_hold (3 rows): the test-side SHARED_HEADER_AVAILABLE would make name_gate worse by one.
- **Next:** a CThingSearchTools.h header batch, a protected `PeekClassList() const`, would open 51 lines.
