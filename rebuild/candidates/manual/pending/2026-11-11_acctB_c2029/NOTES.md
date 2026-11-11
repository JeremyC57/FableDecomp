# Hand chunk 2029 (CTCInventoryMap .. CAIStateGroup_MoveGoodsBase): 1 of 2 byte-exact

0x005FE090 grades RELOCATION_MATCH under VC7.1 RTM for the exact file in src/ (gall.sh with the overlay, re-run after the last header
change); its behaviour test prints `005fe090_TEST PASS` under g++ (tall.sh) and VC7.1 (tall.py). Every call target equals retail
(tools/rel.py); the one non-`same` relocation is the "" literal 0x0122D70E, which reloc_bindings.tsv binds to stand-in data names of other
rows (a plain literal COMDAT here, as chunk 2025 reported for its L"" / L"\n"). 0x00915B60 is parked in differ/ after pass 1 (7 grades):
passes 2 and 3 were NOT used because the session's context ran out, not because the row was proven unmatchable.

## Extents
- 0x005FE090: `push ebp` after int3 padding .. `ret 4` at 0x005FE81E (+3 = 1937 bytes). Correct; no SPLITS.tsv.
- 0x00915B60: `sub esp,100h` after padding .. `ret 20h` at 0x009162E8 (+3 = 1931 bytes), int3 to 0x009162F0 (Cleanup). Correct.

## What matched (1)
### CTCInventoryMap
- **0x005FE090 ConstructQuestsScreen** (RELOCATION_MATCH, plain C++, 2 grades). The console quest screen: CManager::GetInstance,
  CreateComponent("UI_QUESTS_MENU"), the past-quests title text, InitialiseOffsets(2), the UI_SCROLLING_VIEWPORT_TEXT_AREA as a non-scrolling
  child (also the viewport for the guide texts), then a current-quests and a past-quests menu entry: each entry is
  CreateMenuEntryComponent(title, layout floats, BRANCH template, "" , "PC_BUTTON_BASTARD") + AddChild(list) when the list builder
  (ConstructQuestReviewList 0x005FC2D0 / ConstructPastQuestsList 0x005FCBE0) returned one, else the plain template + MakeUnavailable; then the
  guide text (CText::SetValue of TEXT_GUI_MENUGUIDE_QUESTS_*, AddFakeChild on the viewport), the subtitle and the ring picture; both entries are
  added with AddScrollingChildAndSetOffsets and the menu is returned. Shape lessons:
  - Construction order read off the destructor order at the end (10 objects: past title, past entry, past list, text area, description,
    subtitle, ring picture, current title, current entry, current list).
  - The past entry uses the second-row layout (25, 2/3, 5) of the landed CTCInventoryWeapons::ConstructScreen 0x005C8459.
  - FABLE_RETAIL_CCOUNTEDPOINTER (existing repo opt-in: the (T*) ctor tests the member it stored): c2 then threads the ctor's null path
    straight into the else arm of the following `!list.IsNull()` test (0x005FE23C `je 0x005FE2E5`) and keeps the failed-new path's own
    `Info = 0` block (0x005FE2DF); with the default `if (data)` both paths merge and the arm prologue is hoisted above the test.
  - `description` declared before the `textBank` reference gives `mov ecx,[PMainGameComponent]; <zero stores>; mov edx,[ecx+14h]`.
  - The current-quests title re-reads PMainGameComponent (written through the accessor), the guide texts use the `textBank` reference.

## Parked for Fable 5.1 (1)
- 00915b60 CAIStateGroup_MoveGoodsBase::PlaceGoods DIFFER(1923v1931): exclusive_use piece ebp v esi (0x00915F54 / 0x00915FFA / 0x00916014),
  crate_index ebx v ebp (0x00916152 / 0x00916156), carrying ebx piece missing (0x00916158 .. 0x009161BE), vector start eax reloads v the ebx
  piece 0x009161E9 .. 0x00916254, success path exclusive_use from ebp v [esp+30h] (0x00916222); frame: scaled offset one 12-byte node v retail's
  three scalar nodes (Z [esp+34h], Y [esp+1Ch], X [esp+38h]) and slot order | tried: 7 full-row drafts (all-explicit returns p1; flag for the
  user-still-set exit p2; explicit A/B + final C p3; A explicit + B/C sharing the final p4; body in a block for `reached` p6; pre-route if/else
  falling into the final p8; + explicit B copy p12), 5 frame-packer traces, 12 c2prio traces, PRE fragment probes | next: the split cascade
  (c2prio --trace: crate_index costs ebx 800 / ebp 1400 because exclusive_use's piece #29 is allowed only ebp; in p1 the costs were 1200 / 800
  and exclusive_use re-split to an esi piece as retail) -- find the source difference that leaves ebp cheaper, then the offset-vector scalars.

### 0x00915B60 decoded (what the parked source does; all names proven, see LABELS.tsv)
Fails (false) unless the destination has a crate stack; takes the goods from the creature's carrying TC (PeekTC<const CTCCarrying>,
GetThingInPrimarySlot) and their single-user action points; waits up to 10 s (`GetFrame() - (long)(ConstantFPS * -10.0f)`) wandering
(AIAction_MinionWander) for a free action point (GetFreeActionPoint -> position, rh_set, index); the stand-off offset is the destination
physics' forward vector (GetTC<CTCPhysicsBase>, virtual GetRHSet() slot 72) times the point's 2D distance from the goods; with a route thing and
the destination more than 20 units away it first runs AIAction_MoveToPreCalcRoute; walks there (AIAction_MoveToPosProximity, 5.0); if the
destination has an exclusive-use TC whose user is set it wanders 50..99 frames (GFRandom(50) on the world seed) until free, fails if still
used, else claims it (SetUser(creature)); walks in (AIAction_MoveToPos), faces it (AIAction_LookAtPos, 17, 0.0027777f), sets DroppingGoods,
makes the goods non-solid to the creature (GetTC / AddTC "CTCSolidToThing"), plays CCreatureAction_PutGoodsDown with the animation name for
the current crate count, then on success clears the exclusive user, frees the action point and returns true. Every failure after the action
point was taken frees it (the conditional pads test the goods first: the action-point TC belongs to them).

### Pass-1 findings for 0x00915B60 (keep for the next pass)
- The block layout, the three conditional pads (registers ecx / edx / eax, laid out post-exclusive, loop-1, pre-route) and the full
  cross-jumps into 0x009162A5 are reproduced by: (a) `A` (user still set) and `B` (LookAt result != 2) as explicit `{ free; return false; }`
  copies (each consumes one eax rotation step, then is cross-jumped whole); (b) the pre-route check as `if (destination.Get() && goods.Get())
  { ...rest... } else if (!goods.Get()) return false;` falling into the final `free; return false` -- with every path to the final block
  dominated by the crate-index load, c2 hoists (PRE) the vector-start load into the LookAt loop's test block and keeps it in ebx (fragment
  probe probe/f/h1.cpp reproduces the hoist); an early predecessor of the final block stops that, as retail.
- `bool reached` declared at the top of the block after the action-point lookup (the body in `if (goods->GetTC(&action_points)) {`) gives
  retail's two 1-byte slots: reached + the first three materialised HasTC bytes at [esp+13h], the later two HasTC bytes in a slot of their own
  (frame-packer trace tools/pk). Declared at function scope it conflicts with every HasTC byte.
- Unresolved: the offset vector's components are three separate frame nodes in retail (stored Z, Y, X into [34h], [1Ch], [38h]); a named
  `C3DVector offset = forward * d` (and `offset; offset = ...`, the 3-argument ctor) stays one 12-byte node; writing `forward * d` inline in
  both loops is not hoisted by c2 (the multiplications stay in the loops).

## Skipped
None.

## Names
All function names proven from two sources (LABELS.tsv). No descriptive function or class names were introduced. Local variable names are
descriptive (manager, menu, pastQuestsText, ..., carrying, goods, action_points, forward, offset, reached, exclusive_use, crate_index, solid).
The PlaceGoods parameter names `distance` (unused in the body) and `route_thing` are descriptive (no PDB parameter names on this machine).
Wrong landed / bound spellings found: NAME_FIXES.tsv (SetUser 0x00916640, the PutGoodsDown ctor 0x00915810, AIAction_MoveToPreCalcRoute,
FetchGoods, GetFreeActionPoint).

## Header debt
See hdr/README.md: one nested opt-in in CTCInventoryMap.h (matched row); for the parked row new CTCExclusiveUse.h / CTCCrateStack.h /
CTCUserActionPointBase.h and opt-ins in CAIStateGroupBase.h (nested CAIFilterBase + MoveToPreCalcRoute), CAIStateGroup_MoveGoodsBase.h (retail
class), CCreatureAction_PutGoodsDown.h (retail class) and C3DVector.h (GetMagnitude2D alone).

## Tools (tools/, copies)
md.py / rel.py / pc.py / inl.sh (from chunk 2025, repointed), fpairs.py (frame-offset pairs of a q.py listing), pk/ (frame-packer trace from
agent2009). c2prio: tools/decomp_pipeline/linux_lane/lane_tools/c2probe/c2prio.py (read-only, run in place).
