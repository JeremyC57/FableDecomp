# Landing nf207 (Account A, 2026-11-20): family pass FP-COMBATMAP, CCombatManager map binders (3 rows)

Agent NF207 ran the CCombatManager family pass (unit = row) and handed back at a row boundary (context) with 3 rows ported, the self row
0x006F0250 not-a-binder, and 7 rows blocked. Its files are in `naming/2026-11-20_agentNF207/`, with the ready 0x006EA050 draft
(`work/006ea050_b.cpp`) and draft opt-ins (`work/hdr_drafts/`). q4389 and q12300 stay blocked under the family pass, with each remaining
row's next step in their queue notes, and are unclaimed for the next single-agent slice.

- 3 rows land, all RELOCATION_MATCH, every test PASS.
  - 0x006E9310: self rename from the stand-in `CCombatManager_Retail::IsCreatureFightingThing_006e9310` to
    `?IsAttackerOn@CCombatManager@@QBE_NABVCThingCreatureBase@@0@Z` (q12300; both PDBs, the Ego_r order IsAttacked / IsAttacker /
    IsAttackerOn = 0x006E9270 / 0x006E92C0 / 0x006E9310, and the body). The whole row is on engine/CCombatManager.h, CThingCreatureBase.h
    and fable_map.h. No row binds the old leaf; its caller 0x0096FA90 keeps a separate stand-in spelling (2 spellings before and after)
    until a cast decision.
  - 0x006E9700: the stand-in map becomes `std::map<unsigned __int64, CCombatantInfo>` (MFIND_NOINLINE).
  - 0x006ED270: the find / insert stand-ins become std::map / std::pair; it binds the 0x006F0250 and 0x006F4F30 self Leaves exactly.
- Header: engine/CCombatManager.h FABLE_RETAIL_CCOMBATMANAGER_ISATTACKERON, add-only and declaration-only; `orig` equals HEAD; pinned 0;
  none of the 37 landed includers defines it.
- Split gate (`logs/split_gate.txt`): it reports 1 created at 0x006F4F30. The only table spelling there is exactly the self row's own
  Leaf (insert_unique), and 0x006ED270 is now its witness. The gate counts callers only, so this is the nf171b case, not a split.
- Pick-any (`logs/pickany_check.txt`): `less<unsigned __int64>` equals the owner 0x00526120 (37 = 37) in both rows. The MISSING lines are
  the folded `_Rb_tree_base_iterator` comparators 0x0041C830 / 0x0041C820, which have no Leaf line (as at nf177 / nf180 / nf185). The agent
  compared those copies with the retail bytes: equal.
- Link: 1 LNK2005, the same as HEAD. Recheck: 34 good, 0 bad.
- Queued: RegisterAttackedCombatant / RegisterAttackingCombatant are private (AAE) in both PDBs. Decision batch: 0x0096FA90's CThing ->
  CThingCreatureBase downcast.
