# Landing nf312 (Account A, 2026-11-23): T1 signature, NF310's continuation (14 rows, 16 table lines)

Hand-back: `pending/2026-11-23_nf312_handback/`; helpers under `naming_batch/agent_tools/nf312/` (new: whose.py, spells.py, portcensus.py,
rdis.py, a widened mkoff.py). All 85 entries classified: 12 done, 4 done-partial (table only), 1 done-equivalent, 38 skipped-port (14 by
census, all opened), 30 skipped-held, no rejects; no header or test change.

- Binder respells: 0x007990C0, 0x009650C0, 0x00405350, 0x009432C0, 0x00EFB8C0, 0x00DF2AB0 / 0x00D89380 / 0x00DF2380, 0x00DFE0F0,
  0x006C0810 (WifeLivingHere typed `CIntelligentPointer<CThing>` as in the Ego_r layout), 0x008D66E0 (its local CThing out of NCreatureMode,
  plus ETargetingType / TARGETING_NULL). Self fixes (NF304 Q6): 0x0088F400 drops `__fastcall` on itself and its callee (UBE / QAE; tail jump
  unchanged, nothing landed binds the old spelling), 0x006A7B20 takes `const CThing&` with its binder 0x006698D0 (pick-any Bind2 /
  OverlappingSphere copies now on the PDB names, bytes and relocations equal to landed 0x00669110's: NF304 Q1 / NF310 D1).
- Proof: RELOCATION_MATCH at HEAD and new; objsec ALL_EQ (`.drectve` included); symbol diffs only the respelled names; links: no new
  duplicate, nothing newly unresolved, every PDB name HEAD left unresolved now resolves. Staleness: all orig/ copies equal HEAD at landing.
- Table: 8 deletes (3 stale lines: entries 78, 83, 84), 7 replaces, 1 rewitness: GFROR13 `YIKK` onto 0x008ED590, whose object imports it
  (compiled at landing, `logs/witness_008ed590.log`; that row is DIFFER in the committed parity gate already, unrelated to the binding).
  The rewitness came in as `old -> new` in the witness column and apply_table.py wrote it verbatim; the line was corrected and apply_table.py
  now takes the new side and rejects any witness that is not 8 hex digits (GOTCHAS).
- Rulings: D1 lands (above). D2 the PDB-const slot 93 respell lands (R1). D3 dropping `__fastcall` on a self and its callee is an in-place
  self fix when objsec is ALL_EQ and no landed row binds the old spelling. D4 re-scoping a local model out of a namespace onto the PDB global
  is the NF300 Q2 shape. D5 an object's UND symbol is enough proof of binding for a rewitness, whatever the row's grade.
- Also: an empty `D:\Documents\FableTLC\rebuild\build\landverify` directory tree dated 00:30 (before any agent, a Windows path from a tool)
  sat in the repo root, untracked; the coordinator removed it.
- Queued: 38 T3 ports + 0x006689E0 (nf86 / nf88 pick-any) + the `candidates/object_family_compiled.h:21` GFROR13 header line;
  NAME_FIXES_QUEUE: GetBoundingInfo 0x006AF640, GFROR13, the 0x009432C0 CrouchAndTalk / SitAndTalk label pair.

## Recheck
`logs/recheck.log` (landed tree, 14 rows + witness 0x008ED590): WINS 14, 0 RELOC_CONFLICT; 0x008ED590 DIFFER(141v166) as in the committed gate.
