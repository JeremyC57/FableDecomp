# Landing nf290 (Account A, 2026-11-23): the T1-access tail and the first T1 signature entries (58 rows)

Hand-back: `pending/2026-11-23_nf290_handback/`; helpers under `naming_batch/agent_tools/nf290/`. ENTRIES 1-60 of 445 reached (1-28 NF288's
leftovers, 29-60 the first signature entries): 29 done, 8 done-partial, 1 done-already, 3 rejected, 16 skipped-port, 3 decisions; 61-445
go to NF293. No header, flag, catalog or test change.

- 58 rows changed in place: access labels and `const`; pointer -> reference with the call sites; `void*` -> `const void*`; a `void` return ->
  `bool`; `friend class CBankFileAsyncEntry` naming the real caller of private constructors (entries 55 / 56); one PDB-typed `static_cast` from
  the row's own PDB `void*` parameter (entry 35, the form NF93 already landed in that row). Every respelled name is the exact string in both
  PDB name sets. objsec HEAD v new ALL_EQ for all (`.drectve` included; 0x0087E250 / 0x0087E450 differ only by the compile-path
  anonymous-namespace hash, equal with it mapped). Links: unresolved after = a subset of HEAD's; no new duplicate; one HEAD duplicate gone
  (StripUnusedVerts v 0x00F3D830).
- Found on the way: binder 0x006292D0 bound `?SetAsPaused@CWorld@@QAEX_N@Z`, exactly the public symbol libpk row 0x00C1C620 (the steering
  library's own CWorld) defines, so a whole-program link would have sent the call to the wrong function; it now binds the protected PDB
  spelling.
- Dropped (rulings): entry 7 (0x006AC390 + 0x00805890) as a unit: making the self const respells the strong same-TU stand-in
  CInterfaceMap::LowerBound 0x004365B0 QAE -> QBE (T3). Selves 0x0049D8F0 SetAsPaused and 0x0093E760 SetAsLookAtThing: shared headers
  define their PDB symbols inline (emitted as pick-any copies by 0x004A3740 / 0x00972CC0), so a strong self definition would duplicate
  them; only their binders moved (T3 scaffolding port for the selves).
- Rejected (T3): entries 2 / 6 / 31 (`virtual` with table effects; entry 2 re-opened for NF293 under R1 if its TU emits no table);
  16 skipped-port (stand-in parameter / return types, a binder passing `long` for `const CCharString&`, an overload `const` would switch, a
  base-to-derived cast, a stand-in ctor, a literal for the EMovementType enum, a namespace move, an STL stand-in class key).
- Table: 18 lines deleted, 17 respelled (entry 7's two lines withheld). NAME_FIXES 9, queued `land1123nf290`.

## Recheck
`logs/recheck.log` (landed tree): 58 RELOCATION_MATCH PASS, 0 DIFFER, 0 RELOC_CONFLICT.
