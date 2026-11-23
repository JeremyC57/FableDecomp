# Landing nf316 (Account A, 2026-11-23): NF312's held T1 signature entries (14 rows, 6 table lines)

Hand-back: `pending/2026-11-23_nf316_handback/`; helpers under `naming_batch/agent_tools/nf316/` (mkoff.py also reads T3_PORT_QUEUE column 3
and the neighbours' rows/; recheck.sh changes to the agent's work folder first). 29 entries: 5 done, 1 done-partial, 2 skipped-port,
21 skipped-held (12 on NF314 / NF315 witnesses, 1 on 0x005296B0, 8 on T3 queue rows), no rejects; no header or test change.

- Entry 1: 0x00522880 declares `const CThing* GetInventoryOwner() const`. Entry 2 (partial): 0x00846100 takes `const C3DVector&` for
  ApplyAcceleration; the self 0x006B0210 still defines `CTCPhysicsBase_Methods::ApplyAcceleration` (NF302 Q2; NAME_FIXES), and the
  forwarder 0x006A4DC0 (`?Run_006b0210@Sub@@`) is probably CThingPhysical::ApplyAcceleration (NOTES only). Entry 5: 0x00969470 takes the PDB
  reference parameters for GetEmptyJugSlot. Entry 10: table only (0x005800D6 already binds the ABV filter). Entry 11: the self 0x0051E210
  RemapUIDForSerialisation onto `QBE_K_N0K_K@Z` (const, `unsigned long level`, full Leaf; NF304 Q6) with all 9 landed binders of the old
  QAE `long` form (0x006CA120, 0x00729CF0, 0x0075D320, 0x00779430, 0x0077B0D0, 0x007C9770, 0x007E8460, 0x007EE3D0, 0x00800A60); the 12
  other exe callers already bound the PDB form and now resolve. Entry 13: 0x0040D350 `protected: void LoadGameState(CAFile& file)`.
- Proof: RELOCATION_MATCH at HEAD and new with 39 caller controls compiled; objsec ALL_EQ (`.drectve` byte-equal); only the respelled UND
  symbols change (plus entry 11's self DEF); links: no duplicates, nothing HEAD resolved left unresolved; both PDB name sets give all six
  spellings. Staleness: all 14 orig/ copies equal HEAD at landing.
- Table: 4 deletes, 2 replaces.
- Rulings: Q1 the 12 witness-held entries are taken after NF314 / NF315 land. Q2 entry 2's binder lands alone (NF302 Q2). Q3 a mention in
  T3_PORT_QUEUE's reason column is not a reservation: only an address in column 2, or listed in a unit's column 3, is.
- Slip noted: one vland run started with the repo root as its working directory (a failed sed); it wrote nothing into the repo (checked).
- Queued: 2 T3 ports; NAME_FIXES_QUEUE: 1 line.

## Recheck
`logs/recheck.log` (landed tree): WINS 14, 0 DIFFER, 0 RELOC_CONFLICT.
