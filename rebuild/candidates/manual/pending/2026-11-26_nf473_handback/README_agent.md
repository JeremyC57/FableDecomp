# NF473 hand-back (2026-11-26): 8 of the 9 held b4 units land with their slot-target respells; T3 nf319 / nf322 / nF325 / nf324 / nf308 resolved

No repo writes, no git, no `wineserver -k`. Locks: LOCKS.txt (NF472 / COORD_HOLD_NF472 / COORD_HOLD_NF471 locks taken over, plus the slot targets and 6 new).

## Land order
1. **batches/b0_hdr** (1 header): engine/CAIStateGroup_MinionKidCalledToMother.h, nested opt-in `_FINDCALLHOMEEVENT` (private const-return FindCallHomeEvent, both PDBs).
   Add-only plus one pinned assert (FABLE_STATIC_ASSERT line 18 -> FABLE_STATIC_ASSERT_AT(18, ..)); g++ -E -P identical for every includer configuration.
2. **batches/bS1_slot_units** (28 rows, 8 units, all ACCEPT). Grade with `FABLE_HDR_OVERLAY=$W/ov/inc` (ov/inc = the b0_hdr header; ov/src/compiled = the 8
   owner ctor texts the thin ??_G rows include) or after b0_hdr and the owners land. Units (UNITS.tsv): StandStill, MinionKidCalledToMother (+ 0x0090EE20 self,
   0x0090EEC0 / 0x009009EA callee-only), WanderAroundInitialPos, CreatureGoUnderground, Freeze (2 slot rows), KrakenTentacleIdle / Rise / Attack.
   The 16 owner / thin rows are NF471's b4 texts, unchanged (orig == HEAD checked). Apply TABLE_CHANGES (7 replaces), SELF_SYMBOLS (28), CATALOG_CHANGES (8 thin relabels).
3. **batches/bS2_minionwander_HOLD** (3 rows HOLD, libcp; alt/ holds the RELOCATION_MATCH header port as evidence).
4. **batches/probe_task2_buyfromseller**: task 2 not landable without ruling r473e (README there). bU3 stays held.

## Gates (bS1)
- vland HEAD v new (final run, all 28): 28 / 28 RELOCATION_MATCH PASS both; per-row WARN never grows (79 -> 44 total; STANDIN_LEAF on the 8 thin rows goes with the catalog relabel).
- objproof: every slot / callee row ALL_EQ under its renames (0x00920710: leaf .text identical, plus the inline GetCreature pick-any COMDATs the landed 0x00920730 also emits); .drectve unchanged in all 28 (no libcp); owners ??_7 4 -> 56 bytes, slots 11 / 12 name the PDB selves.
- link (20 objects, thin rows excluded): duplicates 0 -> 0; unresolved: the old stand-ins gone; new = base / sibling slot selves and callees that are landed selves (0x0085F120, 0x00924420, 0x009244D0, 0x00920730, 0x00911A90, 0x0099A2E0, 0x0099EAE0, base slots), plus the data global renamed 3EA -> 3_NA.
- census (evidence/fullcensus.tsv): every retail reference to the 12 respelt addresses (vftable slots via their owner objects): 20 OK, 0 BAD, 0 row-less.
- sandbox: apply_table --dry 7 replace, 0 misses; bindings_multi 1 -> 1; descriptive_check 2030, 0 FAIL; s_unit_check 189, 0 failed; repo table unchanged.
- tracker dry run: 20 rows -> agree_pdb (17 rtti_identified, 2 split_unknown 0x0090EE20 / 0x0128B0C0, 1 split_fixable 0x0085F120); 0x0097E200 stays split_fixable with fewer names (6 -> 5); none worse.

## Not done
- Task 3 (NF467 b3 rework) not started. Task 2 probe only (r473e).
- NAME_FIXES.tsv: 0x008FEB21 ??_G of CIntelligentPointer<const CMessageEvent>, the 0x009009EA sibling member vtables, 0x0092BF30.
- Known debt kept: NEQ pick-any ??_G?$CIntelligentPointer@$$CBVCMessageEvent@@@@ copies in 0x0090EEC0 / 0x009009EA (30-byte /O2 v retail's 28; same bytes as HEAD's stand-in-named copies).

## Units and the rulings they depend on (units are independent; land any subset)
| unit (bS1) | rows | depends on |
|---|---|---|
| StandStill | 0x0091BCF0, 0x008FDE1B, 0x00900484 | nothing |
| KrakenTentacleIdle / Rise / Attack | 0x009250F0 / 0x00925170 / 0x009251E0 + owners / thins | r473a (kept action / CAIBrain local models) |
| WanderAroundInitialPos | 0x00911A00, 0x008FD757, 0x008FF704 | r473a (kept CThingAICreature view) |
| MinionKidCalledToMother | 0x0090EEB0, 0x0090EE20, 0x0090EEC0, 0x009009EA, 0x008FD459, 0x008FEF6F (+ b0_hdr) | r473a (0x0090EE20 / 0x0090EEC0 local models edited for access / const only) |
| CreatureGoUnderground | 0x00920710, 0x008FE30C, 0x0090101E | r473b (reinterpret_cast to const CThing&) |
| Freeze | 0x009224E0, 0x009224F0, 0x008FE593, 0x0090194E | r473c (0x013BB68C bool retype) |
Each unit's TABLE_CHANGES / SELF_SYMBOLS / CATALOG lines are keyed by its rows' addresses (witness column).
