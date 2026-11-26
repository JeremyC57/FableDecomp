# NF470 hand-back (2026-11-26): continues NF469 (CAIStateGroup chain)

No repo writes, no git, no `wineserver -k`. Locks: NF469's 74 taken over (all still NF469), plus 0x008FD2CA / 0x008FEB97 / 0x008FEBC5 claimed (LOCKS.txt, 77).

## Land order
**Do not land NF469's `bR1_008fd288_thin`.** Its unchanged behaviour test LINK_FAILs against the thin object. bR1_008fd288_unit supersedes it and carries the replacement test.

1. **bR0_basedtor_declonly (11): ACCEPT** (r469a full gates, all passed).
2. **bR1_008fd288_unit (4): HOLD** T3 line 651. 1 of 6 binders ported (0x008FD2CA pilot); 5 open, route per binder in T3_RESOLVED.tsv.
3. **b0_hdr (3) + b4_aisg_ctor_fold (40)** (NF469 folder, unchanged): HOLD, they wait on step 2. Not re-gated: the full-chain link and the r467b slot respells were not reached.

## bR0 gates (batches/bR0_basedtor_declonly/evidence)
- vland HEAD v new: 11/11 RELOCATION_MATCH, behaviour PASS both sides, WARN identical (vland_pair.tsv).
- objproof: only the ??1CAIStateGroupBase COMDAT removed (0x00902FB9 also loses its base ??_GCAIStateGroupBase / ??_7CAIStateGroupBase); .drectve unchanged (objproof_nf470.txt, identical to NF469's).
- link with HEAD 0x008FD288: duplicates 11 -> 0; unresolved 14 -> 14, same set (link.txt).
- sandbox (apply_table --dry, empty TABLE_CHANGES): 0 misses; bindings_multi_check 1 -> 1; descriptive_check 2,030 / 0 FAIL; s_unit_check 0 failed; tracker: 0 addresses moved.
- T3 / claims screen: no line (T3_SCREEN.tsv).

## T3 line 651 (nf303 0x008FD288): the six binders are implicit derived destructors
Each binder is the compiler-generated `~CAIStateGroup_<X>` that tail-jumps to 0x008FD288. Two sources for each:
- it is the slot-0 ??_G callee of the class's RTTI vftable;
- both PDBs flag the destructor `compiler-generated`, with Ego_r members at exactly the destroyed offsets.

The classes: GoToBed, UsesSeat, EnemyCreatureBase, WanderAroundShops, JackDragon, RangedCombatUsePreferredFirePoint. Full table, callers (24 retail sites, 22 landed rows), owners and HEAD emitters are in T3_RESOLVED.tsv.

All six are fixable, not "cannot be fixed". p1/p5 probes: today's shared headers give an implicit copy equal to retail for GoToBed, UsesSeat, EnemyCreatureBase and RangedCombat. Two exceptions:
- JackDragon's header wrongly declares the destructor (r470d).
- WanderAroundShops has no retail class in its header (r470e).

## Pilot: 0x008FD2CA
- **0x008FEB97 / 0x008FEBC5** (??_G stand-ins): callee-only respell onto engine/CAIStateGroup_GoToBed.h (and MinionSleep.h). The leaf symbol is unchanged; a `// Leaf:` line was added.
- **0x008FD2CA**: thin row of 0x008FEB97.
- **Grades:** all RELOCATION_MATCH PASS; WARN 5 -> 0 (0x008FD2CA), 1 -> 0 (0x008FEBC5).
- **Copies:** ??1CAIStateGroup_GoToBed 27 bytes, sel 2, EQ retail; ??1CAIStateGroupBase EQ; ??1?$CIntelligentPointer@VCThing@@ identical to the HEAD copies.
- **Emitter census:** 0 other emitters (evidence/emitters_gotobed.tsv).
- **Link (pilot):** duplicates 1 -> 0; unresolved 10 -> 6; only ??1CBaseIntelligentPointer is new: the bound real name of 0x00A01C10 replacing ??1MemberB, unresolved only because 0x00A01C10's object is not in the subset link.
- **0x008FD288's behaviour test LINK_FAILs** against the thin object (NF469 never ran vland). Replaced by a self-contained test with the implicit destructor (tests/008fd288_test.cpp): PASS.
- **Sandbox (bR0 + unit):** apply_table --dry 0 misses (5 replaces); multi 1 -> 1; descriptive 0 FAIL; s_unit_check 1 failed (r470b, a tool false positive).
- **Tracker:** 0x008FD2CA split_unknown -> agree_pdb; name counts down at 0x008FD288 (7 -> 6), 0x0099EAE0 (56 -> 55) and 0x00A01C10 (17 -> 16); no address worse.

## Not done
- **Binders 0x008FD544 / 0x008FD769 / 0x00900F43:** EQ from today's headers, so the same pattern as the pilot applies.
  - 0x008FD544 needs 4 callers respelt, plus the two NEQ HEAD emitters 0x008FFF74 / 0x009001F2 made declaration-only.
  - 0x008FD769 needs no caller change, but an emitter scan of 108 sources.
- **Binders 0x0090010E / 0x009006DD:** need header rulings (r470e / r470d).
- **Step 4:** b4 re-gate, full-chain link and the r467b slot respells.
- **r467d** (CInputProcess opt-in) not started.

## Tool notes
- tools/gobj.py: HEAD + new objects.
- tools/cc.py: probe compile.
- tools/symdis.py: COMDAT disassembly.
- tools/t3screen.py: T3 / claims screen.
- A thin row's include must resolve to the new owner text: grade with FABLE_HDR_OVERLAY=work/ov/inc, with work/ov/src/compiled/<path> holding the new owner.
