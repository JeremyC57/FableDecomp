# 2026-11-11 landing ck -- family FAM05: 136 landed rows respelled to the virtual names FAM03 proved (FAM04's remainder)

Source: `naming/2026-11-11_agentFAM05/` (STATUS, NOTES, LESSONS, RENAMES, NAME_FIXES, LEFT, hdr README).

**Verification**
- vland --recheck-landed ran in two parts: all 136 grade MATCH (68) or RELOCATION_MATCH (68).
- Running every test exe found its catalog PassPattern in 136 of 136.
  - 8 tests were replaced: their catalog tests linked the old symbols, or were fastcall/thiscall shims that never tested the row and do not build under g++.
  - The 5 replaced tests whose old catalog PassPattern was custom were reset to `<addr>_TEST PASS`: 0x00569B10, 0x005E9820, 0x005E9A70, 0x007E35D0, 0x00905420.
- Bindings were re-derived (-29 / +30 lines). bindings_multi_check is at its NullCoord baseline.

**What was renamed**
- Type and member getters: 50.
- Persist bodies (OnSerialise and TransferableVector::Resize; string keys read from Fable.exe): 21.
- Lifecycle overrides and forwards: 41.
- One-offs: 24.

**Headers**
- 33 headers carry `#ifdef` opt-ins, mostly `_VSLOTS3`. hdrcheck2 reports default=same, and every header is a line-superset of the repo copy.
- FABLE_RETAIL_CTCCAMERAPOINTDEFINITIONBASE_VTABLE declares the virtuals in true slot order.

**Left over and queued**
- 29 rows are left (LEFT.tsv): 20 complex bodies, and 9 that need unproven callee names or large class models.
- Name fixes are queued as land1011ck (29 entries; also in NF19's queue).
- Labels: ghidra_out/labels_landing_2026-11-11ck.tsv.
- Made-up names (s_MaxFrames and parameter names) are listed in NOTES.md.
