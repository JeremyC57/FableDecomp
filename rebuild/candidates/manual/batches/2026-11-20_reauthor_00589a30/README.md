# Re-author: 0x00589A30 CTCInventory::OpenPCMapMenu, a landed row that had regressed (Account B, 2026-11-20)

The landed row had graded DIFFER since ce3b25d5d (2026-11-13), which moved the GFDebugPDynamicCast template from CThing.h into its own header.
Where an inline function is defined is compiler input (GOTCHAS). The only difference was one swapped pair of register reloads after the inline
`repe cmpsb`, a shift in c2 symbol numbering.

- **Fix:** the row is rewritten in Ego_d's text, which matched on the first compile on today's headers:
  - Ego_d's names;
  - the search loop's condition carries `&& !pmap_list`;
  - `if (pmap_list) { Initialise; push_back; }`, then the FindChild part inside a second `if (pmap_list) { }`;
  - two scroll locals, each reused for two buttons.
- **No** `// Inferred:` line, header change or name fix.
- **Grade:** RELOCATION_MATCH, test PASS, strict vland `--land` with no overlay.
- **Review:** written by Account B's Opus follow-up author and checked by Account B's coordinator. It was not independently audited (owner
  wind-down). It replaces a landed text that no longer matched.
- **Test limitation (pre-existing):** the landed test is a host model that does not link the row object. It was updated to the new control
  flow, but it cannot kill row mutants. A real-object test (10 externals, listed in author_README.md) is queued.
