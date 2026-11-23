# Landing nf296 (Account A, 2026-11-23): T1 "other" made-up leaf names, NF292's continuation (75 rows + 2 tests)

Hand-back: `pending/2026-11-23_nf296_handback/` (`TABLE_CHANGES_applied.tsv` is what landed); helpers under
`naming_batch/agent_tools/nf296/`, among them dcalls.py / rcalls.py / dchk.py: donor-build (FableWin.exe) forward callees, reverse callers
and a per-binder second-source check, by name only. 60 of 894 entries reached: 33 done, 18 done-partial, 9 skipped-held; 834 go to NF299.
No header, flag or catalog change.

- 75 rows: each binder's local-model declaration and its calls take the PDB name in place of the made-up one (`const` where the PDB is
  const; private / protected on a single declaration line; 0x006CD970's second parameter `int` -> `bool` per the PDB). Two tests change with
  their rows (0x006CD970, 0x00892F10: they link the row and defined the callee stub under the made-up name).
- Proof: every object identical to HEAD under an explicit old-to-new symbol map; each new symbol exactly the entry's PDB spelling. Links:
  the made-up names HEAD left unresolved are gone, no new duplicates; every reference still unresolved has a reason (owner not landed, not
  linked, or defining another name). Second source for every name: both PDB name sets plus the donor FableWin.exe showing the binder's
  counterpart calling that function (a guard against identical-code folding on 4-17 byte targets); entries 355 / 647 / 649 are FableWin-only,
  with the donor caller plus the retail call site.
- tblafter.py moved entry 616 to partial (0x007AA9E0 binds the PDB name on the wrong class, CThing).
- Table: 53 deleted, 22 added, 5 rewitnessed. The coordinator compiled the six CONDITIONAL rewitness witnesses
  (`logs/witnesses_head.log`): five bind exactly the spelling; 0x00833010 binds `?OnNewBrain_008018b0@CTCIdleScheduler@@QAEXXZ` (its own
  line), not `?Reset@CTCIdleScheduler@@QAEXXZ`, so that line became a delete (no binder left for it at 0x008018B0).
- Queued T3: wrong-scope binders (local CThing, outside NGraphicAppearance, a derived class), stand-in classes / free stand-ins,
  signature ports; tiny shared owner bodies 0x006925A0 / 0x00662170 / 0x00661E70 / 0x006621A0 / 0x0079DCB0 / 0x004BFFC0 (identity research,
  no guessed names); owner self ports 0x00A67770 / 0x00CB7D60 / 0x0073A840 / 0x005DAA00; 0x0082FE00 (identity). 0x0077CCA0 (access) goes
  to NF299 first.

## Recheck
`logs/recheck.log` (landed tree): 75 RELOCATION_MATCH PASS (with the two changed tests), 0 DIFFER, 0 RELOC_CONFLICT.
