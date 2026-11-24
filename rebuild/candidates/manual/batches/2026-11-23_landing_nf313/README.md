# Landing nf313 (Account A, 2026-11-23): the T1 "other" class / namespace tail + 17 uncovered entries (22 rows, 10 table lines)

Hand-back: `pending/2026-11-23_nf313_handback/` (`unreached.txt`: 24 in-place entries not reached or waiting on a ruling, NOT T3); helpers
under `naming_batch/agent_tools/nf313/` (README_NF313.md; delchk.py checks a delete line against every exe call site). All 69 entries have a
STATUS line: 10 done-partial (the 22 rows), 28 skipped-port, 31 skipped-held (7 tie / off-limits, 24 unreached), no rejects; no header,
test, flag or catalog change.

- Entry-18 unit (NF311 Q1): owner 0x0097E330 SetPushableByHero QAE -> IAE (`protected:` + full Leaf) with its four QAE binders 0x00967930 /
  0x00967B40 / 0x00968BC0 (NF308's respells) and 0x009689C0 line 73 (re-derived after nf310); 0x00944DB0 keeps its made-up
  CAIStateGroupBaseI line (done-partial).
- Batch A, binder-only (the owner's HEAD object already defines the PDB form; every landed binder of each stand-in fixed): CalculateMatrix
  (0x00B242A0, 0x00BACBD0, 0x00BC5FC0), SetMainAnim (0x00861A80), the CEngineInternalPrimitiveBase ctor (0x00B3A910), SetText 6-arg
  (0x006CEF20, 0x00729B20, 0x007800B0, 0x008139C0, 0x00849380) and 9-arg (0x007DF4B0), AddObject (0x009D5A90), GetRegionNumberFromName
  const (0x0088E0B0), PerformSequenceReset private AAE (0x006EA450, 0x006EABB0).
- Batch M: AIAction_MinionWander in 0x0092A8A0 / 0x0092B3E0 onto the PDB IAE signature (shared engine/EMovementType.h, literal 3 ->
  AI_MOVEMENT_WALKING, EAIActionReturn return, CThingBuilding* parameter; `evidence/ppdiff_M.txt`).
- Proof: RELOCATION_MATCH at HEAD and new; objsec ALL_EQ under exactly one rename (`.drectve` included), tables and COMDAT sets unchanged;
  links with the owners' HEAD objects: each stand-in unresolved at HEAD, every moved reference resolves, nothing HEAD resolved lost, 0
  duplicates; no includers. Staleness: all orig/ copies equal HEAD at landing (after nf309).
- Table: 10 deletes, one per stand-in spelling, each checked against every exe call site and the shared-header declarations.
- Rulings: Q1 the entry-18 unit lands. Q2 Batch A's call-site edits (`&x` -> `x`, `p` -> `*p`) are the normal shape of a pointer ->
  reference fix; Batch M's access label, return type and parameter type are PDB-given and byte-equal (R1): both land. Q3 a `typedef long E`
  row whose own self is a virtual override may retype its self only when every landed vftable and binder of the old self spelling moves in
  the same landing (NF304 Q6); otherwise a port. Q4 the SetPItem `PAVCThing` line: object-scan its 10+ declaring rows at the next landing and
  rewitness onto one that binds it, delete if none (continuation). Q5 the NOTES Q5 in-place candidates go to the continuation. Q6 the
  CTCBase(CThing*) pointer-form ctor (58 sources) is one T3 FAMILY UNIT. Q7 LSocket / LHNetworkLib is the NF308 family unit already queued.
- Slip noted: one payload build piped into `head` (broken pipe); the agent killed its own PID and rebuilt.
- Queued: 28 T3 ports + the CTCBase family unit; NAME_FIXES_QUEUE: 1 line.

## Recheck
`logs/recheck.log` (landed tree): WINS 22, 0 DIFFER, 0 RELOC_CONFLICT.
