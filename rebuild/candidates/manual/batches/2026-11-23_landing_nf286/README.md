# Landing nf286 (Account A, 2026-11-23): T1-access + T2, NF283's continuation (52 rows)

First batch after the 2026-11-23 container restart, redone from the repo tree on the rebuilt lane. Hand-back:
`pending/2026-11-23_nf286_handback/`; the agent's helpers (nf.py: census, source census, prechecks, grades, objsec proof, link, xr, payload)
are committed under `tools/decomp_pipeline/linux_lane/lane_tools/naming_batch/agent_tools/nf286/`. No header, flag, test or catalog change.

- ENTRIES 1-60 of 144: 49 done, 2 done-already, 5 rejected, 2 decisions, 2 skipped (precheck); 61-144 go to NF288.
- 52 rows changed, all zero line delta, in place in each row's own local model. objsec HEAD v new ALL_EQ for all 52 (`.drectve` included),
  each object's symbol changes = its RENAMES lines exactly; six table-emitting rows, all tables byte- and relocation-identical; links: HEAD
  leaves the wrong spellings unresolved, the new sets resolve them, duplicate counts equal. 9 self-leaf renames, 53 caller respellings.
- The 12 precheck retries (D1 `cl /Zs`): entries 1-8 and 12 done, including the three cloth `InitialiseSimulation` signature splits
  (`unsigned int` -> `unsigned long`, both PDBs and the self rows K); 9 rejected (same-TU strong copy, D3); 10 / 11 skipped again (their
  unchanged tests fail clang in their own lines; ruling D1b for NF288: an unchanged landed test may take `cl /Zs`).
- Accepted wider fixes (rulings): entry 23 0x0042E3EE drops `virtual` from its own declaration (both PDBs private non-virtual; no table
  emitted; no absolute reference to the address in Fable.exe), entry 43 0x00B63010 widens private -> public (both PDBs), entry 1
  0x00A5B710 / 0x00A5B760 keep their row-local `typedef unsigned short wchar_t` (VC7.1's default; NOWCHAR precheck).
- Rejected / T3: 9 (same-TU copy), 13 / 21 (CKeyboardDX / CMouseDX ctor binders emit their own tables), 14 (declaration from a header under
  `rebuild/include/candidates`), 50 (free stand-in binder, would need a stand-in friend); decisions 40 / 42 (script base class virtuals,
  binders emit a table) -> T3. Added to `naming/T3_PORT_QUEUE.tsv`.
- Table: 20 lines deleted, 25 respelled (stale witnesses for entries 2, 27, 28, 41, 44 included). No symbol gains a second target.
- NAME_FIXES (13, queued `land1123nf286`), first among them a behaviour finding: 0x00A64230 ReadFromParserAsBool relocates
  `CParsedItem::GetSymbol` 0x009B9870 at both sites where retail calls `GetString` 0x009B9880 (its local model declares only GetSymbol):
  NF288's first entry.

## Recheck
`logs/recheck.log` (landed tree): 52 RELOCATION_MATCH PASS, 0 DIFFER, 0 RELOC_CONFLICT. Rule slip: GOTCHAS.md not re-read word for word in
the restarted run (the NF288 prefix repeats it).
