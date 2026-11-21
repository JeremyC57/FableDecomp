# 0x00502E90 CWorldMap::PopulateRegionWithVillagers -- p25S (2026-11-21)
Base: acctC wfE10 draft (pending/2026-11-16_acctC_open/wfE10/00502e90.cpp) = W/src/502/a0.cpp, overlay W/hdr50 (only 6 headers still
needed: CShopDef, CTCBuyableHouse, CTCVillage, CThingAICreature, CThingBuilding, CThingCreatureDef; every other wfE10 opt-in is in HEAD now).
Regrade on HEAD: DIFFER(13587v13622), reproduces acctC.

## Measured: retail TU layout is NOT c2 compile order (refutes the layout -> budget-path inference)
- world_map.cpp in retail: all source functions first (0x4FA9A0..0x50F820), all header-inline / template COMDATs after
  (0x50F820..0x51CDC0, then CWorldMap in-class members). Source functions are NOT in source order: by Ego_d first lines they form
  ascending runs (1204..4039 | 741 4017 | 741..5445 | 957..3324 | 1795..4227 | 1112..4323 | 1998..3910 | 186..4482 | 1104..2163 ...),
  a merge of source-ordered runs (hash-bucket-like). W/tmp/tu_order.txt.
- Our VC7.1 (same 3077 c2): obj section order = compile order = callees first (inline callees before every caller, source functions
  in source order); PCH (/Yc,/Yu), /Zi, /Z7, /Gy, /Gy-, /Ob1, /Gm, /EHsc /GR, a 3000-function TU: none changes that (minis W/tmp/pch).
  Caveat: with /O2 /Gy- the obj still had one .text section per function, so a true non-COMDAT build was not tested.
  link 7.1 keeps obj section order.
- Emulation (diagnostic hook tools/dfr/defer3.py: after the sort at 0x1070CCB6, move the TU's 5 source functions to the head of
  the compile list, so every inline/template callee compiles after them = retail's layout taken as compile order):
  the row goes 13587 -> 13371 with 576 calls v retail 491, and the three helpers that MATCH in our callee-first build
  (GetRelevantBuilding 0x005029A0, AddToRelevantBuilding 0x00502B00, CreateRelevantPersonAndUpdateStats 0x004FF750) all break
  (DIFFER 328v337 / 729v906 / 364v336). Retail laid those helpers out BEFORE their template callees, yet their bytes need the
  callees compiled first. So retail's COMDAT placement after a function does not show the function was compiled first;
  the "budget path from layout" inference (0x00E20600's predicate dtors, the 0x008E18D0 first idea) has no support from layout alone.

## Ego_d check of the draft's devices (brief: decode against Ego_d first)
- Ego_d locals (egod_locals.txt, debug length 24594): no `listCreatures` or any other bool besides find_house / dont_populate_next_loaded_region /
  evil_apprentice / assigned_to_work_home. Ego_d records this function's locals, so the draft's fitted `bool listCreatures = true` is
  contradicted (the 0x00763060 `anchor` ruling): not landable.
- Measured only by a name grep: the draft uses none of Ego_d's local NAMES (num_*_places_left, homes_to_villagers_map, men_shuffled,
  assigned_to_work_home, evil_apprentice ...). Its block shape lines up with Ego_d in places (eight `init`/`new_person` blocks at the
  CreateRelevant sites, five fill-loop `init`s, men_shuffled = the draft's candidates copy, evil_apprentice = the draft's apprentice
  coin bool, which supports it). Next: rename to Ego_d's locals, then decode the Ego_d line table (egodlines.py) against the 9 depth-0
  regions before more device work.
- gobj on the base obj: 0x005003E0 AddToWorkBuildingList grades DIFFER(166v342), calls 10 v 5, which the draft's RELOCATION_MATCH claim
  for it does not show. That may be a leaf / length mismatch in my oracle row (I took 352 bytes from the next manifest start); unverified.
- No FableWin.pdb on the Linux lane, so I could not run the Ego_d v FableWin revision check.
