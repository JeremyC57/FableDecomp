# Landing nf155 (Account A, 2026-11-20): CThingCreatureBase / CThing / CThingManager names (18 rows)

Agent NF155 (round 10) handed back 18 entries (6 done, 3 done-already, 6 blocked, 3 open); its files are in `naming/2026-11-20_agentNF155/`.

- 18 rows: 5 self renames (RENAMES section 1: the GetDefHover{Slow,Medium,Fast}Speed trio on the shared header, and 0x00692930 /
  0x00692990 off the free `Dest_val` stand-ins onto CCreatureActionBase::GetPlayer / PeekThingSearchTools) and 13 binders.
  0x00692990 lands although 0x008C2240 still binds a CCombatActionBase stand-in: its old leaf is bound by no landed row and its new leaf
  already is (advisor decision, owner delegated; the address's spellings go 3 -> 2).
- NF155's suggested 0x00957F20 change landed with nf153.
- Headers: engine/_quarantine/CThing.h (FABLE_RETAIL_QUARANTINE_CTHING_GETPOS; the 26 removed lines are asserts re-pinned at their old
  lines) and engine/CCreatureActionBase.h (two declaration opt-ins), both add-only; `orig` equals HEAD.
- Recheck (`logs/recheck.log`): 3 MATCH + 15 RELOCATION_MATCH, every test PASS. rebind multi-check 1 (baseline); split gate 0 created;
  includers 399 good, 0 bad. tablecheck: six addresses shrink; 0x013BAB2C shows 1 -> 2 only because the table now lists a stand-in
  0x008330D0's HEAD object already bound (SAME).
- Link (`logs/link_landing.log`): 6 LNK2005, all present at HEAD, which had 8 (`logs/link_control_head.log`): the landing removes two
  (CTCPhysicsBase_GetAirResistance, Dest_val) and adds none. Queued as DEBT.
- 30 unattempted port rows and 4 open rows are queued for a new claim (NF155 STATUS.tsv).
