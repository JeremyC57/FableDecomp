# Landing nf210 (Account A, 2026-11-20): FP-COMBATMAP leftovers, IsDefenderDueForAttack and two combatant-map binders (3 rows)

Agent NF210 was the single running agent (owner order: one agent at a time) and brought all 4 entries to a terminal state. Its files are in
`naming/2026-11-20_agentNF210/`. They include the held 0x006EAC50 draft and its CThingCreatureBase.h opt-in (`held/`), and the unused
declaration-only HasTC draft (`work/hdr_drafts/`, 65v69).

- 3 rows land, all RELOCATION_MATCH, every test PASS.
  - 0x006EA050: self rename from the catalog `CCombatManager::CheckSummonerChargeable` to
    `?IsDefenderDueForAttack@CCombatManager@@QBE_NABVCThingCreatureBase@@@Z` (both PDBs, the body; NF207's draft).
    - Its two callees bind their self rows' Leaves.
    - The old self leaf is bound nowhere (table 0, sources 0).
    - 0x0096FA90 keeps its own stand-in spelling, so the address stays at 2 spellings (q12300, now one ruling for two addresses; queued).
  - 0x006EDA80: find / insert_unique move onto the self Leaves of 0x006F0250 and 0x006F4F30. The new SIZE_BODY opt-in makes its
    `_Rb_global<bool>::_M_decrement` copy equal the /Os owner 0x004294E2 (57 = 57; nf184 pattern). Advisor decision (owner delegated):
    accepted. Landed 0x006F0030's differing 64-byte copy is HEAD debt (queued; it could take the same macro).
  - 0x006ED510: find and the two erase(key) calls move onto the self Leaves of 0x006F0250, 0x006F50B0 and 0x006F3200, through the new
    FABLE_RETAIL_STLP_TREE_ERASE_KEY_DECL_ONLY opt-in. It still binds 0x00662430 as IsCombatant; the real
    `?IsInMeleeAsAttacked@CThingCreatureBase@@QBE_NXZ` is queued (both PDBs).
- Headers: add-only; `orig` equals HEAD for both; pin_asserts pinned 0. No landed row defines any of the macros (`hdr/README.md`).
  - engine/CCombatManager.h: ISDEFENDERDUEFORATTACK and GETCOMBATDEBUGINFO, both declaration-only. GETCOMBATDEBUGINFO has no user yet: its
    row 0x006EAC50 is held.
  - stlport/fable_map.h: RB_GLOBAL_DECREMENT_SIZE_BODY (size-pragma scaffolding) and TREE_ERASE_KEY_DECL_ONLY.
- Held (advisor decision, owner delegated): 0x006EAC50, the self rename to
  `?GetCombatDebugInfo@CCombatManager@@QBE?AVCCombatDebugInfo@@ABVCThingCreatureBase@@@Z`.
  - Its object newly defines the CCombatDebugInfo copy ctor, which grades RELOCATION_MATCH at the landed row 0x006F3000. One relocation
    reaches 0x006F0810 through a `vector<C3DVector>` copy ctor defined inside the object. Under the nf205 amendment that is a different
    target.
  - It is also the only user of a new CThingCreatureBase IsInMeleeAsAttacked opt-in, while 0x006ED510 still binds that address as
    IsCombatant.
  - Next pass: a declaration-only opt-in for that vector copy ctor, an owner check of its default-ctor copy, and 0x006ED510's binder moved
    in the same pass.
- Blocked: q11760 / q11873. The /d2inlT sweep brackets retail: 269 bytes up to T64, 307 from T65, retail 295. The fix is a shared-header
  inline control for CBasicString<char>::operator== around 0x004115A0, grouped with q12262 / q12265.
- Also blocked, with the same residues across NF199 / NF207 / NF210: 0x006E93C0 (137v139, GetRing push order) and 0x006EA770 (796v799,
  `this` in ebp). They are logged in FINDINGS_LOG and kept out of naming slices.
- q11772 (coordinator check, `logs/q11772_link.log`): the held 0x0047DB73 draft (naming/2026-11-20_agentNF209/work/held/) has an 8-byte
  HasTC COMDAT. It links with landed 0x00488620's 58-byte COMDAT with no LNK2005, and 0x004118C0 has no landed row. That makes it
  copy-against-copy debt, the nf198 CCPPointerInfo case (advisor decision, owner delegated). It goes back to the queue ready to land with
  its RENAMES lines and a debt line; the declaration-only HasTC opt-in is not used.
- Pick-any (`logs/pickany_check.txt`): the OK lines equal their owners:
  - `less<unsigned __int64>` 0x00526120 (37 = 37),
  - the iterator `operator--` 0x006F0030 (79 = 79),
  - `_M_decrement` 0x004294E2 (57 = 57).

  The MISSING lines are the folded `_Rb_tree_base_iterator` `==` / `!=` at 0x0041C820 / 0x0041C830, which have no Leaf line (as at nf207).
- Link: 0 LNK2005, the same as HEAD. Split gate 0 created. Recheck (`logs/recheck.log`): 979 good, 0 bad. That covers the 972 includers of
  the two headers and the referencing rows.
- tablecheck: 0x006F0250 4 -> 3 and 0x006F50B0 2 -> 1; none grows. No out-of-image oracle line.
- Queued debt: 0x006EA050's test is still a standalone host model (NF199 / NF207 precedent).
