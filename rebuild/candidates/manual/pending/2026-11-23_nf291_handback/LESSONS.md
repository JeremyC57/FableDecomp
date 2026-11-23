# NF291 lessons (2026-11-23)

- **Read the full table lines, not ENTRIES' `spellings` column.** The tracker cuts that column at about 160 characters. tri3 then prints a long PDB
  spelling as an `S` stand-in, and objbind tests the truncated string, so `standin_in_obj` is always False for long symbols.
  `tools/tbl.py` prints each entry's reloc_bindings.tsv lines with PDB / SI marks. Use it before treating an `S` line as live or stale.
- **Most T1 standin_alias tails are ports.** Of 110 entries, only one binder had its stand-in on a struct that derives a real header class.
  The rest hold the stand-in in other places:
  - `_Retail` / `_Calls` local structs (no base);
  - element stand-ins (`CElementNN_`, `CSecond_`, `CMember_`);
  - free fastcall destructor helpers on raw offsets;
  - same-TU stand-in bodies;
  - stand-in namespaces (`X_<addr>`, `FableHeap_<addr>`, `N_<addr>`).
  Classify them by reading the struct definition line: grep `^(struct|class) X(_Retail|_Calls|Methods)` and check for a `: RealClass` base.
- **A header can declare a member in only one branch.** Check which branch the binder compiles, and whether that branch is generated do-not-edit:
  - CTCQuestCard.h's flat branch is hand-corrected, the landed default, and has all asserts `_AT`, so an add-only opt-in is fine there;
  - CWorldMap.h's flat branch carries the GENERATED do-not-edit marker, so it is T3.
- **A `_quarantine/` include is a generated, single-user header.** Its class lacks the PDB methods. Moving the binder to the engine header is a port.
- **Check T3_PORT_QUEUE.tsv for witness binders too, not only ENTRIES targets.** Entry 66's only witness (0x007F53A0) is queued there. Leave it
  uncompiled and propose a CONDITIONAL table line from source evidence.
- **The planted-token control for a flat-branch opt-in flags only the flat-branch users.** That was 10 of 1,407 rows here, a small but non-zero set,
  which shows the equivalence check sees the branch.
