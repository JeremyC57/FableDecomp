# NF207 notes (round 10, family pass FP-COMBATMAP; handed back for context after 0x006EDA80)

Counts (from APPLIED.tsv / STATUS.tsv / recheck_final.log):
- 2 entries: 0 done, 2 blocked (q4389 partial; q12300 decision), 0 open.
- 11 rows in rows.txt: 3 ported (0x006E9310, 0x006E9700, 0x006ED270; RELOCATION_MATCH + PASS), 1 not-a-binder (0x006F0250, the
  entry's self row), 7 blocked (0x006E93C0, 0x006EA050, 0x006EA770, 0x006EDA80, 0x006ED510, 0x006EAC50, 0x0096FA90).
- Overlay: engine/CCombatManager.h, one add-only opt-in (FABLE_RETAIL_CCOMBATMANAGER_ISATTACKERON). hdr/README.md.

## Self rename (q12300)
- 0x006E9310: ?IsCreatureFightingThing_006e9310@CCombatManager_Retail@@QAE_NABUCThingCreatureBase_Retail@@PBUCThing_Calls@@@Z
  (HEAD object; the HEAD row had no Leaf line) -> ?IsAttackerOn@CCombatManager@@QBE_NABVCThingCreatureBase@@0@Z (Ego_r 0x00712570 +
  FableWin 0x02412B50 + body). The row is ported whole onto engine/CCombatManager.h + engine/CThingCreatureBase.h (+ CCombatantInfo.h
  through it): the stand-in classes CCombatManager_Retail / CThingCreatureBase_Retail / CCombatantInfo_Retail / CThing_Calls are gone,
  no local model is added. The old leaf is bound by no landed row (reloc_bindings + source grep: only comments in 0x006E9270 /
  0x006E92C0 mention the old name).
- The file moves (self rename): rebuild/src/compiled/00/6e/CCombatManager_IsCreatureFightingThing006e9310_006e9310.cpp. No landed
  row or funclet #includes it (grep of the file name under rebuild/src and rebuild/tests: none but its own build entry).
- Test unchanged: a standalone host model of stand-in names (does not link the row), as NF199 left its standalone tests.

## Splits
- No address grows a spelling.
  - 0x006F0250 loses `?find@CRetailRbMap_UID_CombatantInfo@@QAEPAUnode@1@AB_K@Z` at 0x006E9310 / 0x006ED270 and
    `?find@CRetailRbMap_UID_CombatantInfo@@QBEPAUnode@1@AB_K@Z` at 0x006E9700; both stand-ins stay bound by blocked rows
    (0x006EA050, 0x006EDA80; 0x006E93C0).
  - 0x006F4F30 loses `?insert@CRetailRbMap_UID_CombatantInfo@@QAE?AUSResult@1@ABUSCombatantValue@@@Z` at 0x006ED270 (still bound by
    0x006EDA80).
- gate.py counts callers only: it may print CREATED / GREW at 0x006F0250 and 0x006F4F30 for these moves. Both new caller spellings are
  the self rows' own Leaves (0x006F0250 `??$_M_find@_K@..`, 0x006F4F30 `?insert_unique@?$_Rb_tree@_K..`), so the self-vs-callers split
  narrows (GOTCHAS nf171b line). 0x006E97F0 is untouched (the 0x006EA050 draft that would move it is not handed back).
- Recorded split (not created, not grown): 0x006E9310 keeps two spellings, IsAttackerOn (self) and the caller stand-in
  ?IsCreatureFightingThing@CCombatManager@@QBE_NABVCThingCreatureBase@@PBVCThing@@@Z (0x0096FA90). Before: the old self stand-in +
  the same caller stand-in (2 -> 2).

## Pick-any copies (advisor rule; cmpbody against the owner's object)
- _M_find<unsigned __int64> (owner 0x006F0250): 0x006E9700, 0x006ED270 define it (MFIND_NOINLINE): 84 bytes, bytes equal True,
  relocs equal True. 0x006E9310 declares it only (MFIND_DECL).
- ??R?$less@_K@std@@QBE_NAB_K0@Z (table 0x00526120): 0x006E9700, 0x006ED270: 37 bytes, equal / equal.
- ??9_Rb_tree_base_iterator@std@@QBE_NABU01@@Z (table 0x0041C830, witness 0x0041E6D3): 0x006E9310. The 0x0041C830 owner row defines
  only the stand-in ?NotEqual@CP@@ (no table ownership of ??9); the 22-byte copy is byte-identical to the retail oracle of
  0x0041C830 (no relocations).
- ??8_Rb_tree_base_iterator@std@@QBE_NABU01@@Z (table 0x0041C820, witness 0x00CBFB7D): 0x006ED270. The 0x0041C820 row is
  CPixelFormat::IsEquivalent (no Leaf line); the 16-byte copy is byte-identical to the retail oracle (no relocations).
- Everything else new is a header inline with no table line and no landed Leaf (iterator ctor / -> / *, map and tree find / end /
  insert, pair<const u64,CCombatantInfo> ctor / dtor, _Select1st, _S_key / _S_left / _S_right / _S_value / _M_root,
  CThing::GetUniqueID).
- Blocked 0x006EDA80 (not handed back): its matching form newly defines ?_M_decrement@?$_Rb_global@_N@std@@... 57 bytes v the /Os owner
  0x004294E2 (64 bytes). The landed 0x006F0030 row already emits that 57-byte copy at HEAD (debt, queue it).

## Decision items (coordinator)
1. 0x0096FA90 CAIStateGroup_SummonerCharge::IsRunnable binds 0x006E9310 and 0x006EA050 through a local CCombatManager with the enemy
   as a CThing*. Both PDB spellings take const CThingCreatureBase& (IsAttackerOn's second argument; IsDefenderDueForAttack), so the
   caller needs a CThing -> CThingCreatureBase downcast. Not added (no cast of a stand-in to a real type).
2. 0x006EA050 identity: ?IsDefenderDueForAttack@CCombatManager@@QBE_NABVCThingCreatureBase@@@Z (both PDBs + body; NAME_FIXES.tsv).
   Draft work/006ea050_b.cpp grades RELOCATION_MATCH (2 grades) with an `// Inferred:` signed __int64 id local (retail keeps the id
   in ebx:ebp and re-stores it before each find; an unsigned local is passed by address: DIFFER 92v106). Its opt-in
   (FABLE_RETAIL_CCOMBATMANAGER_ISDEFENDERDUEFORATTACK, both overloads) is in work/hdr_drafts/engine/CCombatManager.h. Landing it would
   also move 0x006E97F0 off ?IsDefenderDueForAttack@CCombatManager_Retail@@QAE_NPAUCCombatantInfo_Retail@@@Z onto its self Leaf.
3. 0x006EDA80: rule on the pick-any _M_decrement copy (HEAD debt at 0x006F0030), or try an inline explicit specialization
   _Rb_global<bool>::_M_decrement under optimize("s") (nf184 SIZE_BODY pattern) in fable_map.h.
- (Advisor note seen, not applied: 0x006ED270 and 0x006EDA80 are not a must-move-together family under the no-split rule: each moves
  its own bindings onto spellings the self rows already define; 0x006ED270 is handed back alone.)

## 0x006E93C0 diagnosis
- NF195's "frame 10h v 8" does not reproduce under MFIND_NOINLINE (_a: `sub esp,8` both sides, 137v139). The residue is the
  GetRing argument: retail pushes the ring index before the victim lookup (0x006E940E), every std::map spelling evaluates the lookup
  first and keeps the index in edi. The family brief still carries the stale frame diagnosis.

## Access fixes found (NAME_FIXES, not applied)
- 0x006ED270 RegisterAttackedCombatant and 0x006EDA80 RegisterAttackingCombatant are private (AAE) in both PDBs; the landed objects
  define QAE spellings.

## Descriptive names
- None. Every respelling moves a binder onto a spelling a self row already defines (0x006F0250 D1 _M_find, 0x006F4F30 insert_unique);
  the one self rename is a PDB name (two PDBs + body).

## Local models / flags
- 0x006E9700 / 0x006ED270 keep their HEAD local models (CThingCreatureBase, CCombatantInfo, CCombatManager, ...) in place: no class
  token added (the SHARED_HEADER_AVAILABLE warnings in b3.log are HEAD's). 0x006E9700's local GetCombatantInfo is unchanged; only the
  victim lookup statement now calls it. 0x006ED270 drops its local namespace std { __false_type; __destroy_aux } for the header
  opt-in FABLE_RETAIL_STLP_DESTROY_AUX_FASTCALL_DECL (already used by landed 0x006EDA80).
- No // Flags: lines added; no new local model.

## Locks
- All rows touched are in rows.txt (locked NF207). No new lock claimed. Locks still held on rows NOT changed (blocked; the coordinator
  may release them): 0x006E93C0 0x006EA050 0x006EA770 0x006EAC50 0x006ED510 0x006EDA80 0x0096FA90 0x006F0250.
