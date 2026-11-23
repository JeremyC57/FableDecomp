# Landing flatten switch 1 (Account A, 2026-11-23): 51 flattened rows onto their class headers through existing class-model switches

Script: `tools/decomp_pipeline/linux_lane/lane_tools/flatten_pilot.py --switch` (journal: `docs/journal/2026-11/2026-11-23_naming_remainder.md`,
section "Header route"). Owner question answered by the advisor: the header route next, as a bounded pilot. It became the existing-switch
route: **no header was edited.**

- **What a row becomes.** A flattened `R __fastcall Class_Method(const Class* p)` row now does the following:
  - defines the class-model switch its header already declares the member under: `FABLE_RETAIL_CCREATUREACTIONBASE`,
    `FABLE_RETAIL_CIENGINE` or `FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS` (plus one `FABLE_RETAIL_NSCRIPT`);
  - includes the header with the include lines of the smallest landed precedent that defines a member of that class under the same switch
    (named in the row);
  - defines `R Class::Method() [const]` with the header's own return type, keeping its body and every comment.
- **Proof per row**, computed by the script and recomputed at landing:
  - **Bytes.** HEAD and new objects equal section by section under the one rename (objsec ALL_EQ). For the 33 CCreatureActionBase rows
    the one difference allowed is `.drectve` plus exactly `/DEFAULTLIB:"libcp"`, with equal undefined-symbol sets. That is the coordinator
    ruling in the standing brief (advisor-reviewed), limited to this scripted route; the T1 agent standard is unchanged.
  - **Symbol.** The graded symbol is the PDB symbol; landing confirmed 51 of 51 from the landed tree's objects.
  - **Grade.** MATCH / RELOCATION_MATCH, test PASS.
- **Identity.** Every row is virtual. The retail RTTI vftable placement (`ghidra_out/labels_rtti_port.tsv`) names exactly this PDB symbol at
  this address and no other. That is the second source beside the flattened label, and the script requires it. The same guards as pilot 1
  ran, plus the source-level guard:
  - no other landed source defines or Leafs the member;
  - no file is named for it at another address;
  - the table does not bind it elsewhere.
- **Reservations.** None of the 51 addresses is in a T3_PORT_QUEUE address or unit column, in COORDINATION_CLAIMS.tsv, in
  docs/COORDINATION.md, or under a lane lock. They were locked as COORD for the landing.
- **Counts, run sw4 (`RESULTS.tsv`, all four classes):**

  | Class | Generated | Accepted | Rejected |
  |---|---:|---:|---:|
  | CCreatureActionBase | 37 | 33 | 4 |
  | CIEngine | 9 | 9 | 0 |
  | CAIStateGroupBase | 9 | 8 | 1 |
  | CGameScriptInterface | 25 | 1 | 24 |
- **Rejects, queued in T3_PORT_QUEUE.tsv as `flatten-switch1`:**
  - 4 LINK_FAIL: identical bytes, but the test calls the old flattened name, so the tests move with the rows.
  - 25 SRC_FAIL: the body needs local helper types or globals the switch route drops. That covers 24 of the 25 CGameScriptInterface rows,
    whose bodies are real code; the stop rule applied to that class.
  - The 35 skips whose body uses the local model's members are ports onto the header's member names (not queued here; the script reports
    them).
- **Table.** No lines: none of the rows had a table binder.

## Recheck
`logs/recheck.log` (landed tree, 51 rows): 51 MATCH / RELOCATION_MATCH PASS.
