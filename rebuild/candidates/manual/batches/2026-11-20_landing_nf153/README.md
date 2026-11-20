# Landing nf153 (Account A, 2026-11-20): CPlayer / CPlayerManager accessor binders (24 rows)

Agent NF153 (round 10) handed back 13 entries (6 done, 3 done-already, 1 blocked, 3 partly applied); its files are in
`naming/2026-11-20_agentNF153/`.

- 24 binder rows respelled onto the PDB accessor names other landed rows already bind; no self renames, no overlay headers (rows only
  switch on existing opt-ins). Every changed (symbol, target) pair lands on an existing spelling (NF153's census of ~900 rows).
- 0x00957F20 carries NF155's suggested change on top of NF153's version (the CThingPos stand-in dropped, `player->GetPos()`), graded
  RELOCATION_MATCH non-strict (`logs/regrade_00957f20_nf155_diff.log`; strict mode flags the row's older local models).
- Recheck (`logs/recheck.log`): 24 RELOCATION_MATCH, every test PASS. bindings_multi_check 1 (baseline); split gate 0 created.
  tablecheck: seven addresses shrink; 0x0123E7D8 shows 4 -> 5 only because the table now lists 0x009352C0's vtable stand-in, which its
  HEAD object already bound (SAME).
- Includers (`logs/includer_recheck.txt`): 231 good; 1 bad, 0x007E2A60 DIFFER(707v707), which is DIFFER with the include trees of
  6eb99a4dba and 731726ee06 too (the assert-name-sensitive row in GOTCHAS 2026-11-19 / 2026-11-20), so not caused here.
- Link (`logs/link_landing.log`): 8 LNK2005 (IS_THING_ALIVE<CThing*> x5, a std::pair copy-ctor, GetSubDef<CHeroTitleDef>,
  GetAbilityPDef); the HEAD objects of the same rows give the same 8 (`logs/link_control_head.log`). Queued as one DEBT line.
- Queued: the 0x00935160 CanFire family port (finishes q11705 / q5045 / q5062), the 0x004C7CA0 port (q4753), and NF153's NAME_FIXES.
  NF153's locks, including 0x00935160 and 0x004C7CA0, are released.
