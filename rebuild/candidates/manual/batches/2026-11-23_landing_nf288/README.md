# Landing nf288 (Account A, 2026-11-23): T1-access + T2, NF286's continuation (70 rows + 1 test)

Hand-back: `pending/2026-11-23_nf288_handback/`; helpers (bind.py: what each census row's HEAD object binds at an address) under
`naming_batch/agent_tools/nf288/`. ENTRIES 1-60 of 87 reached: 45 done, 6 done-partial, 3 rejected (T3), 4 decisions, 1 skipped-precheck,
1 skipped-held; 61-87 go to NF290. No header, flag or catalog change.

- **0x00A64230 ReadFromParserAsBool (NF286's behaviour finding):** its local CParsedItem declared only GetSymbol, so both retail
  GetString (0x009B9880) call sites bound `?GetSymbol@CParsedItem@@QBEABVCCharString@@XZ`. It now declares and calls GetString; its test
  (a copy of the row with stubs) changed to match. Census: the other 9 rows calling 0x009B9880 bind GetString, all 8 calling 0x009B9870 bind
  GetSymbol; no table change needed.
- 70 rows changed in place, zero line delta, in each row's own local model: 14 self renames, 60 caller respellings. objsec HEAD v new equal
  for 70/70 (`.drectve` included), symbol changes 1:1 with RENAMES; 14 table-emitting objects with identical tables; links: the new sets leave
  unresolved only what HEAD left, duplicates equal except one HEAD duplicate gone (entry 19).
- Wider kinds accepted: entry 7 0x007E7AC0 `const` removed (both PDBs non-const), 32 0x00753C70 private + const, 60 0x00830360 `protected:`
  on a base declaration the derived class calls, 19 a local stand-in's `operator[]` made const (bytes equal; only the stand-in's name changes).
- Rejected (T3): 12 0x0055FB80 (0x00560D00 holds a strong same-TU private copy), 23 0x005FC840 (const cascades into
  TeleportHeroToTeleporter, whose self row 0x005FBD20 is a stand-in signature), 44 0x0066BC10 (strong public copy in 0x0066BDF0). Decisions
  -> T3: `virtual` entries 20 / 43 / 47 / 49 (binders carry or emit a table). Held: entry 8 0x006D5260, queued with 0x006D5850 as one unit.
  Partial (stand-ins left, NAME_FIXES): entries 10, 11, 17, 19, 35, 42.
- Table: 18 lines deleted, 26 respelled; no symbol gains a second target. NAME_FIXES 12, queued `land1123nf288`.

## Recheck
`logs/recheck.log` (landed tree): 1 MATCH + 69 RELOCATION_MATCH PASS, 0 DIFFER, 0 RELOC_CONFLICT. Rule slips: one `sleep 0`, clang
prechecks in the foreground (under `timeout 120`); the NF290 prefix settles both.
