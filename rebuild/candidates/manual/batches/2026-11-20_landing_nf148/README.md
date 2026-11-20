# Landing nf148 (Account A, 2026-11-20): CMemoryDataInputStream and ThrowShotObject one-definition follow-ups (5 rows)

Agent NF148 (round 10) handed back q11817 and q11785; the agent's files are in `naming/2026-11-20_agentNF148/`.

| Row | Change | Grade |
|---|---|---|
| 0x00994930 | self leaf ??0CMemoryDataInputStream@@QAE@PBEJ@Z -> PBXK (Ego_r.pdb + FableWin.pdb) | RELOCATION_MATCH PASS |
| 0x00BF3B60 | drops its strong local CMemoryDataInputStream(const void*, unsigned long) definition (retail calls 0x00994930 out of line twice); `// Leaf:` line added | DIFFER 809v1480 -> 825v1480, PASS (accepted: not worse, 47 -> 55 exact instruction matches, the two retail calls now exist) |
| 0x009551A0 | defines the new _CLONE / _INITIALISE opt-ins, so its vtable COMDAT has the ThrowShotObject slots 11 / 12 | RELOCATION_MATCH PASS |
| 0x009566C0 | nf145's header port + _CLONE / _INITIALISE (one vtable: identical COMDAT copies) | RELOCATION_MATCH PASS |
| 0x00956960 | ported onto the shared headers; self leaf -> ?Clone@CCreatureAction_ThrowShotObject@@UBEPAVCCreatureActionBase@@XZ (Ego_r.pdb + FableWin.pdb) | RELOCATION_MATCH PASS |

- Headers: engine/CCreatureAction_ThrowShotObject.h (two add-only nested opt-ins, its assert pinned at its old line) and
  engine/CCombatActionBase.h (add-only). Prep: hdrcheck2 default path and every existing macro unchanged; overlaid headers' `orig` equal HEAD.
- 0x009566C0 changed in nf145 after NF148's claim; NF148's copy is nf145's version plus its two defines (diff checked).
- Recheck (`logs/recheck.log`): 4 RELOCATION_MATCH + the accepted DIFFER; rebind, bindings_multi_check 1 (baseline); split gate 0 created
  (`logs/split_gate.txt`). Includers of the two headers: 166 good, 3 bad, all DIFFER at the same sizes in an earlier landing run
  (0x00BDC510, 0x00BE7D70, 0x00BE8920; July landscape rows), so not caused here (`logs/includer_recheck.txt`).
- Symbol diff (`logs/symbol_diff.txt`): new definitions are the self leaves, the implied ??_G COMDAT and one anonymous-namespace
  scaffolding helper. Link of the five objects with 0x009FF590: 0 LNK2005 (`logs/link_landing.log`).
- NF148's NAME_FIXES.tsv (three ThrowShotObject vtable slots) appended to the queue.
