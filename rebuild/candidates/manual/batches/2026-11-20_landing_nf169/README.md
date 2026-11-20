# Landing nf169 (Account A, 2026-11-20): creature action / AI state group names (26 rows)

Agent NF169 (round 10; it took up NF164's unfinished entries) handed back early (heavy context) with 22 entries: 8 done,
6 blocked, 8 open (q4967 / q4968 partly respelled). Its files are in `naming/2026-11-20_agentNF169/`. The 8 open entries go back
to the queue unclaimed.

- 26 rows. Self renames (7): CAIBrain::GetCurrentStateGroupScriptPriority / IsUpdateZoneActiveThisFrame (0x0088B760 / 0x0088B810),
  CCombatManager::DeregisterAttackingCombatant 0x006ED510 (with its four binders, one family), CAIStateGroup_MeleeCombatNearbyAttack-
  WhenVeryClose::IsStillRunnable 0x0093DC10, CAIStateGroup_MazeRangedCombat::IsStillRunnable 0x00954220, CAIStateGroupBase::
  SetUpPreCalcNav 0x0097F5B0, CAIStateGroup_MinionSitAtSchool::InitCreaturePosition 0x0090FC50. Every landed referrer of these
  addresses ends on the new spelling.
- 0x0095EDF0 / 0x0094F000 moved onto the shared CIntelligentPointer<const CThing>, so they emit its template COMDATs. Linked with
  owner 0x00930D20 there is no clash.
- Header: engine/CAIStateGroup_MeleeCombatNearbyAttackWhenVeryClose.h opt-in (declaration-only, `orig` equals HEAD).
- Recheck (`logs/recheck.log`): 1 MATCH + 25 RELOCATION_MATCH, every test PASS. rebind multi-check 1 (baseline); split gate 0
  created; includers 63 good, 0 bad. tablecheck: no address gains a spelling; five shrink.
- Link (`logs/link_landing.log`): 2 LNK2005, the same pairs as HEAD (`logs/link_control_head.log`). DeregisterAttackingCombatant
  clashes between 0x006ED8C0 (a same-TU noinline copy) and the self row 0x006ED510. At HEAD the clash is the same, under the old
  spelling, so it stays as DEBT and does not hold the family.
- Queued: the open `_quarantine` header question (q11715, 0x006EAC50), the 0x006ED8C0 port, and the InitCreaturePosition family
  (q11937-q11940).
