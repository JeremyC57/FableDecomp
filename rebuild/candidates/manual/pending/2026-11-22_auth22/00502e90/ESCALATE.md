# ESCALATE 0x00502E90 CWorldMap::PopulateRegionWithVillagers (13622 B) -- AUTH22, 2026-11-22

## State
- Best landable-shape text: `b3.cpp` (this folder), STOCK flags, overlay `hdr/` (the six p25S opt-in headers re-ported add-only onto
  today's headers; `hdr.diff`; pin_asserts: 0 pinned, nothing moved). **DIFFER(13606v13622)**, 269 unmasked hunks.
  It carries NO device: no `#pragma inline_depth`, no `/d2inl*`, no fitted `listCreatures` local.
- For comparison, the recorded draft a0.cpp (wfE10-15 / p25S; nine inline_depth(0) regions + the Ego_d-contradicted `bool listCreatures`)
  regrades DIFFER(13587v13622), 285 unmasked hunks. The same draft with the devices simply removed (b0.cpp) is DIFFER(13971v13622).
- Grades used: 4 (a0 regrade, b0, b1, b3). Readouts (c2 inline trace, not grades): b0, b1, b2. Helper check (not a row grade):
  the in-file CreateRelevantPersonAndUpdateStats with its dead arm still grades RELOCATION_MATCH at 0x004FF750 (gobj on b1.f.obj).

## New evidence (not in any earlier note)
1. **Ego_d line table, decoded against a VC10 /Od build of the draft** (`egod_lines.txt`, 555 records, source lines 4482..5423;
   ours: `b0_od10.txt`, tool odall10.py; `olist.py` prints our per-line sizes with text). The work-place blocks align line for line
   (62 DisplayProgress, 107 init ctor, 7, 23, 9, 20, 102 Create, 21, ...). Ego_d has ~35 debug-only lines of 185..533 B that our
   text has nothing for; they are ASSERT / message macro lines (one-assert unit ~197..219 B):
   helper 4232 (284); 4487 (481, right after GetSubDef), 4608 / 4613 / 4620 (197 each: inside the three guard-arm `if`s; the third
   is `if (person->IsUnderScriptSupervision()) <msg>`, which is why retail keeps the bare call 0x004C7E90), 4696 (210, before
   `num_barman_places_left = 1`), 4759 (219, shop block after the break), 4772 (533, the shop GetDef line), 4801 (219, market block
   after the break), 4898 (197, inside `if (num_home_places_left < 0)` before the return), 4929 (332, loop top), 4946 / 4972 /
   4993 / 5000 (218, before the four unemployed / child Creates; male: after `init`, female: BEFORE `init`), 4950 / 4977 / 5005 /
   5013 (210, after them), 5270, 5318 (220), 5343 / 5394 / 5415 (517), 5355 / 5371 / 5381 / 5387 (218), 5410 (400).
2. **Stock call list.** With every device removed (b0) the row's call list differs from retail at only four places: loop-1
   `list<CThingAICreature*>::push_back` x2 (retail calls 0x004AC4B0, ours expands, score 50), the teacher / shop Create cross-jump,
   the servant CreateRelevant site (retail calls; ours expands at g 8515, no penalty, score 29) and the female push_back. The
   apprentice call, the five fill-loop init ctor calls, the vector copy and the three SetHomeBuilding calls are right at stock.
3. **The servant / apprentice calls are the a+g>9000 step penalty, reached by dead assert IL.** Putting FABLE_RETAIL_ASSERT dead arms
   at the Ego_d positions before the servant block (11 arms, list in b3.cpp line 2) moves g at the servant site from 8515 to 8974
   (a = 99): penalty 169, score 152, called; apprentice called; the six earlier sites stay expanded (games site g 8409). The rule
   is confirmed on this row's own trace: at g 8974 CreateRelevant (a 99) and AddToWorkBuildingList (a 60) are penalised, the init
   dtor (a 6) and ctor (a 17, g 8981) are not. Window for the pre-servant extra IL: +387..+931 (ours +459).
4. Apprentice: Ego_d line 4889 is ONE 184-B statement and the block has no index locals (init, new_person, evil_apprentice only):
   the two indices are ternary arguments. b3 uses that form; bytes equal the fitted if/else form (replaces a FITTED item).
5. Other places where the draft contradicts Ego_d's locals / lines (NOT changed in b3, each needs its own grade):
   no `long property` local in the buildings loop (Ego_d re-reads `sim_building_def->Property` on lines 4663..4719); no
   `bool initiallyEmpty` (line 4635 is one `if`, locals ptc_buyable_house / sim_building_def); `ptc_thing_owner = NULL` initialisers
   (4683 / 4702, 10 B each); fill loop is `while (num_home_places_left > 0)` with `males_left / females_left / children_left`
   computed per iteration (4909..4923), `ret = NULL` (4926) and `num_warehouse_workers_left_last` (4927), DisplayProgress 11 first;
   the declaration order of the function-level locals differs (4496..4533: maps, `men, women, children` on one line (27 B),
   10 zero inits, weights, 5 zero inits, guards / ghosts, hand_placed_villagers). Loop 1 (4541..4623) is a different revision
   (find_house logic that retail does not have): the Ego_d rule cannot be applied to it as it stands.

## The remaining contradiction (the question)
With c2's trace on b1/b3 (`b1.f.json`; `python3 $S/w22/auth22/dt.py b1.f.json`):
- vector push_back cost c1 = 39 is pinned from below (>= 38) by the helper's own out-of-line body 0x004FF750, where retail calls it
  at f 0.125 (39 x 1.97 = 77 >= T 74).
- Fill loop, retail: male push_back called, female expanded, child called. Unpenalised scores are 75.6 / 59 / 95 (a perfect fit to
  T = 74); with the penalty they are 126 / 99 / 159 (all called). So retail's female site is unpenalised: g <= 8983 there (a = 17).
- But the servant site must be penalised (g >= 8902), and c2's g only grows: servant -> female is +460 in our text (apprentice
  loop +77, the early-return destructor block at `if (num_home_places_left < 0) return;` +288, fill-loop head +95).
  8902 + 460 > 8983. The two retail facts cannot both come from the penalty unless that stretch is <= 81 in retail.
- The other reading (no penalty anywhere before the female site, i.e. no dead-assert IL) needs the servant and apprentice sites to
  score >= 74 on merit (c1 >= 105: only a non-constant 12th argument does that, which is the listCreatures device and which
  Ego_d contradicts: both lines are 102 B like the literal-true sites) AND the male / female "unemployed" init ctors (f 0.198 /
  0.321, unpenalised scores 65 / 53) to be called on merit (ctor c1 in 58..103, ours 41).
Question for the advisor: which side gives? (a) is the 288-unit destructor block between the apprentice loop and the fill loop
real in retail's IL (retail does have a second destructor copy at 0x005048C6..), or does retail's early return reach it differently
(`goto` a common exit, a scoped block), (b) may the female site be closed by a frequency change instead (it needs f >= ~0.5,
ours 0.214: Ego_d lines 4977..4982 show `ASSERT(ret); if (ret) women.push_back(ret); num_women++;`, same shape as the male arm), or
(c) is this row terminal (A) on that one site once the Ego_d port in item 5 is done?
Loop-1 list push_back (retail calls x2 at f 0.281, c1 37, score 50; the four later sites at f 8..40 are expanded in both builds):
a uniform cost in 55..87 would fit all six; FABLE_RETAIL_STLP_LIST_CONSTRUCT_HELPER (STLport's `_Construct` text) leaves c1 at 37
(readout b2, negative).

## Next steps if the row stays open
1. Port item 5 (Ego_d locals and declaration order): the frame-slot order residue and the adults-loop RA are id-order effects and
   the draft's declaration order is not Ego_d's.
2. Add the remaining Ego_d assert lines (4898 onward) only together with a readout: SetHomeBuilding at 0x005052AC is expanded at
   score 72 (T 74), so later extra IL can flip it.
3. Then the female site per the advisor's answer.
Files: b3.cpp (best), b0.cpp (device-free baseline), b1.cpp (asserts only), *.out (q.py), b1.f.json (trace), egod_lines.txt,
b0_od10.txt, olist.py, hdr/, orig/, hdr.diff, LOG.md. Tools: `$S/w22/auth22/cc.py` (call list by retail target), `dt.py`, `rs.py`.

## Addendum (closing checks)
- Grades: 4 distinct texts (a0, b0, b1, b3) + 1 identical recheck of b3 after the comment-line edit = 5 q.py runs.
- p25S's open item is closed: the three in-file same-TU helpers grade RELOCATION_MATCH from b1.f.obj (gobj.py) at 0x005029A0
  GetRelevantBuilding (337 B), 0x005003E0 AddToWorkBuildingList (166 B; p25S's "166v342" used a wrong length, the catalog row is 166)
  and 0x00502B00 AddToRelevantBuilding (906 B); CreateRelevantPersonAndUpdateStats 0x004FF750 with its dead arm also RELOCATION_MATCH.
- `$S/w22/auth21/00502e90_HANDOFF.md` (152 bytes) was NOT read: access to that folder is denied to this agent. If it holds a pointer
  that changes the above, the coordinator should pass it on.
