# NF448 hand-back (2026-11-25): NF442 follow-ups

`NOTES.md` has NF448's method and every gate.

## 1. Stale table lines: 50 deleted (committed 047641e2f6)
- **Check per line:** NF448 took the current table witness and compiled every landed object that could emit the spelling from its current repo source (2,600+ objects). A line was kept only when no object binds or defines the spelling.
- **Dropped 2 lines** (`evidence/STALE_TABLE_DROPPED.tsv`):
  - `??4?$CCountedPointer@UCScriptThingImp@@@@...` at 00704580, still bound by 13 objects;
  - `??0CDefClassBase@@QAE@XZ` at 00430370, still defined in two objects.
- **Tracker:** 26 targets split_fixable → agree_pdb, 19 partly, none worse.

## 2. q442a: 27 of 29 rows landed (`batches/..._nf448_ta27`)
- **Ruling:** advisor condition, owner delegated. A row may land when its `/alternatename` token is inert:
  - (a) the relocation carries the target's self name;
  - (b) each callee is defined by its target's own object;
  - (c) the alias is bound or defined nowhere;
  - (d) the `.drectve` delta is exactly one token per rebound callee.
- **Held, failing (c)** (verdict stays HOLD):
  - 0x00921630: its alias is still bound at 0x0056D050 and defined by landed 0x00963640 / 0x0097B900;
  - 0x008B74D0: its alias is defined by landed 0x007F0B10, 0x0080F270, 0x0080F5E0, 0x0080F870, 0x0080FAD0.
- **Table:** the `GetPFirstThing` rewitness that belongs to 0x00921630 stays out. The `GetSubDef<CChestDef>` rewitness was applied.

## 3. CThingSearchTools.h opt-in + 48 callers (`batches/..._nf448_h1_hdr`)
- **Header:** add-only `FABLE_RETAIL_CTHINGSEARCHTOOLS_PEEKCLASSLIST`, which adds a protected `PeekClassList() const`.
- **Callers:** 48 rows rebound from the stand-in at 0x0049C770.
- **Table:** a hand rewitness to 0x0048D400 was applied, because 8 other objects still bind the stand-in.
- **Left for a later batch:** 006cd3b0, 00914b20 and 0093f8a0. They are named only in T3 nf378 reason text.
- **Tracker (2 + 3 together):** 17 split_fixable → agree_pdb, none worse.

## Rows that did not match when compiled from the repo (recheck item)
- DIFFER: 0x0072A680 (75v54), 0x00BE7D70 (1437v2983), 0x00BE6F70 (198v2654), 0x00BE8920 (328v365), 0x01214190 (95v8).
- No object: 0x00C069D0, the LZO vendor .asm row (expected).

These were landed rows. Regrade them with their catalog flags before trusting their grades.

- **equiv_check (h1_hdr header):** 57,160 rows checked; 48 differ, all of them h1_hdr rows that opt in. 0 differ outside the batch.
