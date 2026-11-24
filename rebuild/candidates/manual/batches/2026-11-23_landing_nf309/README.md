# Landing nf309 (Account A, 2026-11-23): NF306's leftovers + uncovered T1 / T2 entries (28 rows, 28 table lines)

Hand-back: `pending/2026-11-23_nf309_handback/` (`reverted/` keeps the withdrawn unit 5 and three reverted respells); helpers under
`naming_batch/agent_tools/nf309/` (its stay-off tool now also reads the target lists in column 3 of T3_PORT_QUEUE.tsv). All 198 entries have a
STATUS line: 16 done, 9 done-partial, 1 done-equivalent, 72 skipped-port (62 by census), 66 skipped-held, 34 open; no header, flag,
catalog or test change.

- Unit 1: 0x00694020 UpdateActionMovement protected; 0x00BF9AA0 / 0x00B662F0 / 0x00845900 / 0x004D4A99 pointer parameters onto the PDB
  reference / const forms. Unit 2: virtual Initialise (0x0078B410 + the owner 0x00694D10), virtual WorldUpdate on the local CThing and its
  overrides (0x0082D810, 0x00704150, 0x00836260; NF303 Q1). Unit 3: virtual ChangeState (0x005469E0, 0x00547C90), 0x0053EBF0 into
  `NUISystem`, 0x00543DF0 `unsigned long`. Unit 4: 0x00F3AFC0 Compress, 0x009261F0 protected `GetPlayerManager() const` by reference,
  0x009BA110 SkipUntilString. Unit 6: 0x008728C0 GetCamTools, 0x006BA030 WriteCompressed, 0x00BF8030 / 0x00BF84C0 SetBlockToHighPriority
  + GetDesiredLODBand, 0x007E7AC0 PlaySoundW, 0x005EE887 SetWill. Unit 9: 0x00BAD040 AddEnableConsoleVariable, 0x009F4910 protected
  Initialise, 0x006AD970 / 0x006A8560 / 0x008326A0 `IsActionListEmpty() const`. Seven rows also had old callee names in comments updated.
- Proof: objsec equal apart from the intended renames, no new vtable / RTTI symbol; links: nothing HEAD resolved left unresolved, no
  duplicates (new unresolved spellings only at the stand-in owners 0x00838BA0 / 0x008FCF90, queued); every binder of every deleted or
  replaced symbol (tracker list, exe calls, absolute and vtable references, the rows NF307 / NF308 landed mid-run) moved or is a stale-line
  witness (`evidence/delete_audit.txt`). Staleness: all 28 orig/ copies equal HEAD at landing (after nf310 / nf311); the agent had released
  its locks before hand-back, so the coordinator held them for the landing.
- Table: 22 delete (5 table-only stale lines: entries 24, 26, 47, 85, 166, each proved from the witness's object), 5 replace, 1 rewitness.
  The NF299-landing rewitness `WriteData` onto 0x006BA030 is now deleted: that row binds WriteCompressed, as the six other callers do.
- Rulings: Q1 entry 169 (DepopulateLevel, `reverted/00dc0300.cpp`) goes to the continuation now that NF307 has landed. Q2 an `#ifdef` opt-in
  that gives an existing shared-header line its PDB access (`#ifdef` new / `#else` old, the NF300 RegisterScreen shape) plus a friend naming
  the real class is acceptable with equiv_check and a planted control (continuation). Q3 a self const fix that forces const onto same-TU
  stand-ins is T3 (NF305 Q1). Q4 yes: a table-only delete of a stale line proved from the witness's object is `done`. Q5 nested types that
  only a shared header can declare (the DeleteData pattern, 78 / 82) are a header-lane / T3 task. Q6 unit 5 waits for the deferred-12 T3 unit.
- Slips noted: one precheck in the same call as its grade (grade rerun later); entry 169 caught late (target not screened against
  stay-off); a `>>` that clang accepted and VC7.1 rejected; the first name census read the wrong RENAMES column (fixed).
- Queued: 72 T3 ports + the unit-5 pointer; NAME_FIXES_QUEUE: 0x00838BA0, 0x008FCF90, 0x009D54E0.

## Recheck
`logs/recheck.log` (landed tree): WINS 28, 0 DIFFER, 0 RELOC_CONFLICT.
