# NF396 hand-back (2026-11-24): T3 nf391 q11 (0x004DF554 and eight more NF391 STL identities)

## Summary
- 6 of 9 rows respelt to their compiled repo-STLport spelling, every gate clean: b1 0x004DF554, b2 0x005793E8, b4 0x006553D0 +
  0x006553FD, b5 0x006C2C00 ACCEPT; b3 0x004E1BD2 HOLD_RULING Q1 only for its contested table line (ACCEPT if the line is dropped). 3 held: 0x006CE220 (identity pair, Q3), 0x004E35D4 (its ~pair copy is NEQ at 0x004DD728:
  port first), 0x0057FEFB (R1 residue, Q2). Row texts of the held three are in held/ (all leaf RELOCATION_MATCH in evidence compiles).
- About 69 VC7.1 compiles: 21 evidence compiles (3 SRC_FAIL, 2 callee probes, 2 vector probes), 26 in the vland pairs a / c / d / e / f,
  22 census objects.
- Locks: kept on the 6 respelt rows; released on the 3 held rows and the 22 census rows.

## Function starts (all nine)
Every address is a catalog start and a direct E8/E9 target (evidence/rawref_9.txt): 0x004DF554 only by the `jmp` at 0x004DF54F (the
5-byte ~vector thunk right before it); the others by calls. The `abs @00427e19` hit for 0x005793E8 is the rel32 field of an unrelated
call (decoded as `call 0x42d5b1`), not a reference. None is a tail fragment; the unaligned ones are /Os-packed COMDATs.

## Two sources per row (re-check)
- 0x004DF554: 0x004DF54F (`jmp 0x004DF554`) <- ??_GCObstacleNavigatorExternalInterface 0x004DF507 (vftable 0x01241D04) on +8 =
  Ego_r.pdb DummyAgentList vector<CSteeringAgent const*>; FableWin ~CObstacleNavigatorExternalInterface (tc_register.obj) calls
  ~vector<CSteeringAgent const*> (evidence/fw_callers2.txt; the first row text said ??_G and was corrected, vland pair f). The thunk
  settles ~_Vector_base.
- 0x005793E8: sole caller 0x005796AB (`call clear; free(header)`), which the table binds as ??1?$_Rb_tree<EHeroTrainableStatType,
  pair<..., NUISystem::CComponent*>> (witness ~CTCInventoryExperience); class part of the new spelling equals that line
  (evidence/classpart_005793e8.txt). FableWin destroys that map in tc_inventory_experience.obj (evidence/fw_callers.txt).
- 0x004E1BD2: sole caller 0x004E350F <- ~CTCCreatureGroup 0x004E5C71 on +0x18 (Ego_r.pdb Members map<unsigned __int64, CGroupMemberData>
  +24); FableWin ~CTCCreatureGroup (tc_register.obj).
- map::clear v _Rb_tree::clear (rulings R2 / R6 spirit): for both tree clears the only caller is a tree destructor body
  (clear + header free). A destructor calls the tree's own `clear()` (STLport `~_Rb_tree() { clear(); }`); map::clear is never called
  from a destructor, so the caller path proves _Rb_tree::clear. Not a hold.
- 0x006553D0 / 0x006553FD (byte twins): pop_front 0x00655A86 / 0x00655AA4 (each called once) <- DrawStatBarOnLeft of CMultiStatBar
  (vftable 0x0125A758 slot 0) / CAutoPickupBar (0x0125A768 slot 0) by RTTI COL (evidence/rtti_popfront.txt); FableWin's two
  DrawStatBarOnLeft call list<CStatUpdate>::pop_front / list<CAutoPickup>::pop_front respectively (evidence/fw_callers.txt).
- 0x006C2C00: called by the byte-identical 25-byte twins 0x006C2CE0 (no reference) / 0x006C2D20 (<- CLevelLoader ??_G 0x006C2E90 on
  +0x14 = ObjectList list<CCountedPointer<CLevelLoader::CObject>>); FableWin ~CLevelLoader (level_loader.obj). Either twin's body is the
  ~_List_base text, so the callee is _List_base::clear.
- 0x004E35D4 / 0x0057FEFB: _M_erase; callers = recursion + one 41-byte clear each (0x004E4108 / 0x0058060D); callees ~pair 0x004DD728
  (table) / ~CCharString 0x0099EAE0 (the ~pair inlined; MirroredCriminalStatus is the PDB U-key struct, trivially destructible).
- 0x006CE220: see Q3 (bytes cannot split ~vector from ~_Vector_base; held/evidence/vecloc_objdump.txt).

## Forms
- Explicit instantiation of the single member on the shared headers (the nf391_b4 / 0x00579EA3 / 0x00527950 forms); for
  ~_Vector_base the destructor alone (`template std::_Vector_base<...>::~_Vector_base();`), which avoids the two constructor COMDATs the
  0x004D9C5D whole-class form would add. Tree clears use FABLE_RETAIL_STLP_TREE_MERASE_DECL_ONLY (no _M_erase copy).
- Header sets were trimmed to what the include chain needs (tools/trimdefs.py, clang greedy, then a VC7.1 recompile proved the object
  COMDAT-identical to the full define set: 004e1bd2 p3/p4, 006553d0 p1/p2). CAutoPickupBar has no shared header: a local model of the
  PDB nested 12-byte record, as the landed 0x006554BD. CGroupMemberData.h needs the CThingAICreature chain for its inline ctor.
- No header edits.

## Gates (ACCEPT rows)
- vland --recheck-landed HEAD and new at one LANDVERIFY_WORK path per pair: f (004df554), a (005793e8), c (004e1bd2), d (006c2c00),
  e (006553d0 006553fd): all RELOCATION_MATCH PASS both sides, tests unchanged and PASS. WARN STANDIN_LEAF lines are the catalog labels
  (still the old stand-in names; non-blocking, catalog relabel is queued with the selves).
- R1 / R10 tiers (batches/bN/evidence/scan.tsv, tools/tier396.py): b1 3x TIER1 (004df365, shared header via
  engine/CNavigatorExternalInterface.h -> stlport); b2 / b3 no extra COMDAT; b4 per row TIER1 _List_iterator_base (005eeff2),
  TIER2_NOT_UNIQUE _List_iterator ctor + node deallocate, TIER2_SMALL 1-byte _Destroy; b5 TIER1 ~CCountedPointer<CObject> (00500eb0),
  TIER2_NOT_UNIQUE ??_G + node deallocate. No residue.
- pk_all + pickany_vs_retail on HEAD, new, and new against the TABLE_CHANGES table (work/troot): no NEQ anywhere; reach flips 0
  (evidence/reachflip_all.txt).
- /FORCE link HEAD v new (evidence/link_final.txt; 6 rows + 22 census objects): duplicates 5 -> 3 (the two stand-in duplicates gone;
  the 3 left are between census objects, the same in both sets); the only new unresolved names are the
  two STLport _M_erase callees, whose HEAD stand-ins (AutoTinyFreeListAllocation_*) were unresolved at HEAD too. Nothing HEAD resolved
  becomes unresolved.
- apply_table --dry (evidence/apply_dry_*.txt): b2 / b3 one add each, 0 misses, multi-target 1 -> 1, no new.
- Census by header reach (tools/reachset.py, evidence/census/): the 1019 landed rows whose source or include closure names any of the
  instance types or old stand-in names; clang prefilter (tools/clcensus.py) then VC7.1 of every definer of a new name (004df365,
  006cc1e0, 006cd680, 0057dc22, 005812df, 00500eb0, 00501150, 00501990, 006c2ba0, 005eeff2) and of the 12 rows clang could not compile
  (004e9ad3 004ebca9 004ebf32 0058578b 00599e3f 005aecc0 005f519f 005f900a 006cc800 008127f0 009c46b0 009c47a0; all RELOCATION_MATCH /
  MATCH at HEAD; evidence/census/vc71_census_hits.txt). 00402510 (also clang-fail) is reserved by Account B and was not compiled.
  New selves: only 0x004DF554's has other definers (004df365 Any EQ, 006cc1e0 local model Any EQ). All selves are Any.
  Removed names: no landed row binds any old self or old callee name except the batch rows themselves; ?OnReadFinished@@YIXPAUS@@@Z,
  ?OnKill@@YIXPAUAutoTinyOwnedList@@@Z, ?clear@CRetailList_CountedPointer@@QAEXXZ keep other definers; 0x00655125 is now the only
  definer of ?std_list_node_unlink_and_free...; ??1CCountedPointer@@QAE@XZ (HEAD 006c2c00's local copy, bound at 0x0048C5C0 by
  00715110) keeps 11 other definers.
- Existing debt seen (not holds, R10): 006cc1e0 / 006cd680 (local models) define a different deallocate<const CSteeringAgent*>;
  006c2ba0 (local model) a different node deallocate for list<CCountedPointer<CObject>>.

## Table changes (b3's line is contested: Q1)
- b2: add the STLport _M_erase of map<EHeroTrainableStatType, NUISystem::CComponent*> -> 00578ff5 (witness 005793e8). Body proof:
  probe/merase_00578ff5.cpp RELOCATION_MATCH at 0x00578FF5.
- b3: add the STLport _M_erase of map<unsigned __int64, CGroupMemberData> -> 004dfb8f (witness 004e1bd2); probe RELOCATION_MATCH at
  0x004DFB8F. Second name at that address (Q1). Without the line the row still lands (its call is unbound, as at HEAD).
- No line bound the nine addresses before; the only line witnessed by a changed or held row at another address is
  ?Cleanup_004dd728@SubObject@@QAEXXZ -> 004dd728 (witness 004e35d4, held): delete it when 0x004E35D4 lands.

## Holds
- 0x004E35D4: v2 (header defaults) DIFFER 55v53 (VC7.1 calls ??_G?$pair(0) instead of ??1); v3 (+CENGINEPRIMITIVEHANDLE,
  _DTOR) RELOCATION_MATCH, ~pair copy 33 v 90 NEQ at 0x004DD728; v4 (+_FORCEINLINE_CLEAR) RELOCATION_MATCH, ~pair 66 v 90 NEQ, leaf-
  referenced -> hold (held/evidence/N3_*, N4_*). Retail keeps an unreferenced ??_G?$pair at 0x004DFC0B that calls ??1 0x004DD728.
- 0x0057FEFB: Q2.
- 0x006CE220: Q3 (two more probes: a local vector, and an early return inside a loop; both inline the destructor fully, so neither
  shows which name retail's out-of-line call binds; the constructor 0x006CE210 has the same twin question).

## Deviations
- prec.sh's first version did not stop on a failed clang precheck and did not source env.sh: 3 evidence compiles returned SRC_FAIL /
  never ran (counted above). Fixed in tools/prec.sh.
- Batch subfolders regenerated once with the toolkit clean.py (my own scratch only).
- No git command, no repo write.

## Late addendum (Q2 support)
- No-compile scan with tools/destroyscan.py: positive control over all .text finds 196 `mov ecx,[esp+4]` + call / jmp /
  push 0 sites; the VA mapping was checked at 0x0057FEFB (53 56 8b 74 24 0c ...). In 0x0057D000..0x00582000 it finds 0 such
  sites (evidence/destroy_shape_scan_0057f000.txt). This tests one shape (the 9-byte `mov ecx,[esp+4]; jmp ~pair` the
  shared header emits) in one region; it supports Q2 but does not prove no copy of another shape exists.
- The directory `D:\Documents\FableTLC/` in this agent dir is an empty directory tree (a copied tool's Windows default
  path); not a deliverable.
- Hygiene: one `cp -r tools tools_snapshot_check` + `rm -rf tools_snapshot_check` (fixed literal path, scratch copy of my
  own tools dir) was run outside clean.py; nothing else was removed.
