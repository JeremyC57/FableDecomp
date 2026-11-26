# NF471 hand-back (2026-11-26): T3 line 651 (nf303 0x008FD288) resolved; bR1 unit complete; b4 re-gated

No repo writes, no git, no `wineserver -k`. Locks: NF470's 66 taken over plus 35 new claims (LOCKS.txt, 101 addresses, all NF471).

## Land order
1. **batches/b0_hdr** (6 headers): NF469's 3 (MinionKidCalledToMother, MinionWander, StandStill; orig == repo) +
   JackDragon `_MEMBERS_ONLY` view, RangedCombatUsePreferredFirePoint `_MEMBERS_ONLY` view, WanderAroundShops retail opt-in (r470e).
   Add-only; g++ -E -P identical (evidence/hdr_equiv_nf471.txt); 4 moved RangedCombat asserts pinned (pin_asserts_nf471.txt). Coordinator equiv.
2. **batches/bR1_008fd288_unit** (38 rows, all ACCEPT, land as one unit with REL=nf303): replaces NF470's 4-row unit (its rows are included
   unchanged). Thin rows include owner texts of this unit: grade with FABLE_HDR_OVERLAY=<inc with b0_hdr> and the owners at their paths.
3. **batches/b4_regate** (NF469 b4_aisg_ctor_fold re-gated, 40 rows): 22 ACCEPT (11 units), 18 HOLD r467b (9 units: slot-row respells first).
   rows / orig / metadata copied from NF469 unchanged; RESULTS rewritten. SELF_SYMBOLS / TABLE / CATALOG files cover all 40: apply only ACCEPT rows.

## T3 line 651
All six binders resolved (T3_RESOLVED.tsv): each is the implicit `~CAIStateGroup_<X>`, now a thin row of its callee-respelt `??_G` owner
(r470a), copy EQ retail. Caller census (evidence/fullcensus.tsv): every retail reference -> its landed row's relocation = the real name, 0 BAD
(2 row-less sites). Emitter census (whole symbol tables, 546 HEAD objects + the unit): HEAD had 9 NEQ ~UsesSeat, 11 NEQ ~EnemyCreatureBase,
1 NEQ ~WanderAroundShops (local models) -> 20 more rows made declaration-only (r469a) in the unit; after: every copy of the six names and of
~CAIStateGroupBase is EQ pick-any, no strong definer.

Untabled binders: 175 retail references to 0x008FD288 (evidence/refs_008fd288.tsv); 4 more landed rows bind it under stand-in names with NO reloc_bindings line
(0x008FDA24, 0x008FECEC, 0x00900533, 0x00913490: evidence/untabled_008fd288_binders.tsv). Outside line 651's six-line subject; coordinator's call on REL=nf303.

## Gates (bR1 unit)
- vland HEAD v new: 38/38 RELOCATION_MATCH PASS both; no row's WARN grows (81 -> 61); 0 leaf fallbacks on the new side.
- objproof: intended changes only (evidence/objproof.txt), .drectve unchanged in all 38 (the full JackDragon / RangedCombat views added libcp: hence the _MEMBERS_ONLY views).
- link, full chain (unit + b4 owners + 82 other HEAD emitters; evidence/link_fullchain*.txt): duplicates 114 -> 12 (the 12 = pre-existing strong ??0 local-model collisions, same on both sides); 0 duplicates of ??1CAIStateGroupBase / binder names; watched unresolved 8 -> 4 (stand-in names gone). All names: 57 -> 61; the 44 new are b4 vftable slot names (24 have landed selves, 7 resolve via exact // Leaf: spellings, 13 belong to the r467b HOLD units: b4_regate/evidence/link_new_unresolved_classified.tsv); the 40 gone are stand-ins / ?v00@ placeholders.
- sandbox: apply_table --dry 19 replaces 0 misses; bindings_multi 1 -> 1; descriptive_check 0 FAIL; s_unit_check 3 FAIL = r470b false positive.
- tracker (dry): 7 -> agree_pdb (0x008FD288, 0x008FD2CA, 0x008FD544, 0x008FD769, 0x0090010E, 0x009006DD, 0x00900F43); names down at 0x0099EAE0 / 0x00A01C10; none worse.
- With b4: +20 rtti_identified -> agree_pdb (11 in ACCEPT units); 20 vftables 4 -> 56 bytes, ??_G / ??1CAIStateGroupBase copies EQ.

## Not done / wave 2
- r467b / r469c slot-row respells for b4's 9 held units (not started).
- 0x008FD56A self respell to ??1CAIStateGroup_MinionSitOnSeatBase, 0x008FDA24 to ??1?$CAIStateGroup_WorkBase@VCAIStateGroupBase@@@@ (NAME_FIXES.tsv).
- 0x009000BC header ctor port (r471a). Pre-existing divergent pick-any copies seen in the census (no retail-bound name):
  ~CIntelligentPointer<CThing> 5 v 11 bytes, <CThingCreatureBase> 5 v 11, <CThingGameObject> 5 v 7, ~WorkBase<CAIStateGroupBase> 5 v 19.
