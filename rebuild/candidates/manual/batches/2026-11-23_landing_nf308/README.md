# Landing nf308 (Account A, 2026-11-23): T1 "other" class / namespace / form, NF305's continuation (10 rows, 8 table lines, 1 catalog Module)

Hand-back: `pending/2026-11-23_nf308_handback/`; helpers under `naming_batch/agent_tools/nf308/` (new: stalescreen.py screens census-class
entries for stale witnesses; namecensus308.py counts one-line `{return ...}` definitions). 118 of 307 entries with a verdict (1-104 plus
E2 pull-aheads): 5 done, 1 done-partial, 2 done-already, 1 done-equivalent, 86 skipped-port, 23 skipped-held, no rejects; no header or test change.

- 0x007032F0: its one-member local CTCGraphicAppearanceSprite into `NGraphicAppearance` (NF300 Q2). 0x008CC8E0 SetLungedThisFrame and
  0x007FD580 GetLineThingSearchArea declarations into their PDB namespaces. 0x0064F6DC + 0x0064499C (no table line; found by the exe
  call-site census): GFAnchorToTopRight into `NPlayerGui`. Owner 0x009E9F20 + 0x0049E8C0: the free StopProgress into `NProgressDisplay`
  (owner Leaf full; catalog Module `_global` -> `NProgressDisplay` in `rebuild/build_candidates.ps1`, matching `manifest/functions.tsv`; file
  names kept). Owner 0x006EA2C0 + 0x006EB390 / 0x006EB600: PerformSequenceReset private (AAE, full Leaf; NF302 Q5).
- Proof: RELOCATION_MATCH at HEAD and new; objsec ALL_EQ with only the listed rename, COMDAT sets SAME; link HEAD v new: every reference
  HEAD resolved stays resolved, the moved ones resolve, no duplicate. Staleness: all 10 orig/ copies equal HEAD at landing (after nf306 / nf307).
- Table: 5 delete (stale witnesses for entries 19 and 99: 0x00ED4830 / 0x00ED41D0 / 0x00AE2CB0 already bind the PDB forms, compiled at
  landing), 3 replace. Every delete checked against every exe call site and absolute reference (none virtual).
- Rulings (standing brief): Q1 LSocket entries 61-71 are one T3 FAMILY UNIT. Q2 a re-scope that renames an emitted vftable (including a
  multiple-inheritance `6B<base>@@` one) is a port, no per-case decision. Q3 a table line may carry only the spelling its witness object
  binds: a Dinkumware `U?$char_traits` line whose witness binds the STLport `V` form is respelled after compiling the witness (continuation).
  Q4 the 0x009E9F20 own-scope move lands (NF301 in-place namespace move + NF304 Q6, all binders moving together). Q5 entry 179 goes to the
  continuation now that NF307 has landed.
- Slips noted: one `git -C /dev/null status` (failed, touched nothing; agents never run git); one precheck before the lock, redone after it.
- Queued: 75 T3 ports + the LSocket family unit (11 entries); NAME_FIXES_QUEUE: 0x0097E330 (owner, with entry 179), 0x004BFA50 (SetScale binder).

## Recheck
`logs/recheck.log` (landed tree, 10 rows + witnesses 0x00ED4830 / 0x00ED41D0 / 0x00AE2CB0): 13 RELOCATION_MATCH PASS, 0 DIFFER, 0 RELOC_CONFLICT.
