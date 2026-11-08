# WORKLOG chunk 882 -- 0x0061E6D0 CTCInventoryQuests::ConstructQuestListPC (12,356 bytes)

## Decoded phases (retail)
1. bucket quests into sorted_quests[3] (core / optional-non-vignette / vignette; type!=0 -> [0]); loops written with `!=`.
2. misc def (GetDef same-TU body), text bank, hero stats (GetTC 4).
3. frame (if/else FindChild, tail-merged), PC_WORLD_MAP + zoom (ConvertVector inline at site 1).
4. description viewport, InitialiseOffsets, movie viewport.
5. outer k / inner j loop (start + count cached in locals): entry, close action, region, minimap, markers,
   AvailableQuests / DisplayQuests, icon (FindIndexBySymbol), title, icon, description text, boasts, rewards,
   sub list, take / boast / pop / delete / teleport actions (type 0) or zoomed-region entry (type 1/2),
   CUIStateDef inserted into the world map's states.
6. SetDefaultChild, inventory-map region markers (set<long> + vector<CRegion>), AddNonScrollingChild(world map).

## Grades (sdiff = structural +/- lines, stack offsets normalised)
- v1 first compile DIFFER(12919v12356)
- v2 TREE_CONSTFIND + first loop `!=`: 12932
- v4 frame if/else, inner-loop cached begin/count, zoom cast early: 12900, sdiff 871
- v6 nested take-condition ifs (tail-merged else), objective nested if, named `started`: 12756, sdiff 824

## Open residues
- inline budget: push_back site 2 inlined (retail calls), heroStats LowerBound inlined (retail calls),
  late CCountedPointer / CAction ctors and 2nd ConvertVector inlined (retail calls). Ours inlines MORE.
- register allocation (zero register edi in retail).
- frame size 0x41c retail vs 0x44c ours.

## Next idea
- find what retail inlines that we call (budget consumer), frame size, CCharString== operand order.

## Session 2 (grades v9..v27)
- v9 (v6 + SetDefaultChild if/else `0L`, no in-loop inventory_map init): sdiff 806.
- /d2inlS0..S20 no effect anywhere; /d2inlT changes unrelated sites first (non-monotonic); /d2inlcrmax/userinl/nlw/fcsa/fcsw no effect.
- First loop: `if (quests.size() != 0) { for (; i != quests.size(); ++i) ... }` with `unsigned long i = 0;` declared before the guard
  (v21): the first loop (push_back sites 2-4 called, 1/5 expanded), the heroStats LowerBound call and the prologue registers all
  match retail; sdiff 602 (from 806). `for(;;)`+`++i` in body / do-while made the loop body hotter (all sites expanded).
- Inline budget proven whole-function / source-order: 4 extra dummy inline CCountedPointer<CAction>(new CAction) statements before the
  per-quest loop (diagnostic x4) turn the late sites (take_boast_entry / teleport ctor, zoom entries, ConvertVector #2) into calls.
  So retail spends MORE budget before the delete action than our model does (same call set otherwise).
- `#pragma inline_depth(0)` round delete/take_boast_entry/teleport/zoom entries/ConvertVector #2 (v26): 12388v12356, bcalls 2
  (the ConvertVector statement also outlines misc_def-> and the C2DVector copy); sdiff 469.
- Accessors GetGoldReward/GetRenownReward/GetScriptQuestName/GetRenownLevel (FableWin.pdb inline): identical bytes (kept, v12).
- v28 (`unsigned long j = 0;` before the inner loop): j store order fixed, 467.
- v39/v41/v42 (`state_def.Position += origin;`, the three state fields 0.5 / 3 / 0x67 in that order, final-block loop reuses `i`):
  12357v12356, 467 -- BEST, now differ/0061e6d0.cpp.
- Rejected: switch for the type chain (783), `inventory_map = 0` in the loop (650: the loop-top zero registers go), boast compare
  spellings (no effect on the ecx/eax this/other swap), boast loop `++` in body, j-loop guard / do-while (identical).
## Current best: differ/0061e6d0.cpp DIFFER(12357v12356), sdiff 467. Next idea: see differ line 3.
