# Landing nf297 (Account A, 2026-11-23): T1 signature, NF293's continuation (51 rows + 2 tests, 1 header opt-in)

Hand-back: `pending/2026-11-23_nf297_handback/`; helpers (NF293's set plus show.py) under `naming_batch/agent_tools/nf297/`. 71 of 348
entries reached: 27 done, 6 done-partial, 38 skipped-port, 0 rejected; the rest (and 24 to re-open) go to NF300.

- **Save opt-in (entry 1):** `FABLE_RETAIL_CUSERPROFILEMANAGER_SAVE_PUBLIC` in `fable_user_profile.h` (add-only, declaration only), used by
  0x0040D400, its test, and 0x0040C790 (a second AAE binder the table did not list; its HEAD object bound `?Save@...@AAEXXZ` through the main
  class branch: the one macro sits in both mutually exclusive branches, ruling Q1). equiv_check over all 57,146 landed rows: 0 differ; the
  planted control flags all 6 includers in both branches.
- **Signature respells:** pointer -> reference where the argument already had the class type; signed / unsigned swaps receiving literals or
  same-size values; `private:` / `protected:`, `const`, `virtual ~` on already-polymorphic models; return types both PDBs give (void -> bool,
  pointer -> reference); a real-class friend at 0x00B937A0; two layout-keeping vptr cases (0x0090F830, 0x00581765). 0x004022B0's test stub
  retyped to `unsigned long` (it defines its own `IsTextureBitDepthSupported` stub).
- Proof: objsec HEAD v new ALL_EQ for every row (`.drectve`, tables); every HEAD and new object MATCH / RELOCATION_MATCH; links: no
  duplicates, nothing HEAD resolved left unresolved. 11 self renames, 46 caller lines.
- Q2: entries 20 / 78 (binders 0x00450BFF / 0x00446BE0 define a derived destructor over the base model; a virtual base dtor would turn
  theirs virtual, QAE -> UAE, a table changes) are T3 ports.
- Table: 24 deleted, 7 respelled. NAME_FIXES 11, queued `land1123nf297`.

## Recheck
`logs/recheck.log` (landed tree): 51 RELOCATION_MATCH PASS (with the two changed tests), 0 DIFFER, 0 RELOC_CONFLICT.
