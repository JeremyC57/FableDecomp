# 2026-11-10 landing n -- chunk 1001 (libpk gap-scan rows, split), 172 functions

Source: `pending/2026-11-10_linux_hand1001/` (one Opus expert agent on 38 gap-scan rows). Each row held several functions
separated by int3 padding; the agent split all 38 into 172 functions (SPLITS.tsv, every start proven by its own `.rdata`
vtable cell) and matched every one. All 172 landed (vland --land: 172 MATCH, tests PASS, no warnings).

- 145 `PKCTRList<T>` forwarders (slots 11..15 of 29 abstract list vtables: GetFirst / GetLast / GetItemByIndex / Reserve /
  GetCount on the +4 container). Class name from libpk's own trace string ("libpk\PKCTRList.h(39)"); T = CListItem for
  0x012B1BC0, the other 28 item types unproven (`CUnprovenListItem_<vtable>`).
- 24 item slots (Validate / Attach / Detach) of the eight key-1 / key-2 item classes.
- 3 file-handle forwarders of CVtable_012b4f60 (GetSize / Read / GetUnitSize).

New shared headers (no existing header changed): `rebuild/include/libpk/PKCTRList.h`, `libpk/CItem.h`, `libpk/CVtable_012b4f60.h`.
Coordinator changes: 134 new starts added to `rebuild/manifest/functions.tsv` (agent_source gapscan, the split evidence) and the
38 original rows recorded in `BOUNDS_FIX.tsv` (action split). Names: `ghidra_out/labels_landing_2026-11-10n.tsv` (descriptive,
libpk has no PDB). Queued (`land1011n`): 0x00C33050 Close, 0x00C1B930 DestroyHandle, 0x00C1F720 CKeyedItem ctor and three vtable
spellings. `bindings_multi_check.py` prints 1, unchanged.
