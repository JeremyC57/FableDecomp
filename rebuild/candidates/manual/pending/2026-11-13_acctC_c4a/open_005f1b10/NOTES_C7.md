# agentC7 NOTES (acctC lane; continues agentC5 -> prev_C5/, agentC4 -> prev_C5/prev_C4/)
Tools: tools/ = C5's tools with paths moved to agentC7 (export W before g.sh / vs.py). hdrx/ = DIAG-only overlay (Reset / operator->
hooks FABLE_DIAG_RA_PRE/MID/POST, FABLE_DIAG_ARROW): never hand back.

## Row 1 0x005FF190 CTCInventoryMap::CreateMapMarkers -- DIFFER(4096v4176), best still differ/005ff190.cpp (= prev_C5 best)
### Assert lead (C6) -- measured, does NOT move this row
- Dead FABLE_RETAIL_ASSERT arms inside the default Reset text (pre / post, `data != Data`, `Info != 0`): byte-identical to no
  assert, and the c2 inline trace gives Reset c1 = 95 with or without the arm (tmp/inl_d0.txt v tmp/inl_d1.txt). c1 is costed
  after the dead arm is gone => an ASSERT in a helper cannot raise its inline cost; C5's "why 108-109" is not an assert.
- Row-body asserts (probe/a/A.cpp template, sites A1..A10: parent, manager, heroMarker, card x3, found != end, !miscDef.IsNull(),
  from && to, i < size) one at a time: all inert; all ten: -8 retail-only bytes only. Ruler of 3..40 asserts at the top: same
  (-8 bytes from 3 on). Not the residue class of this row (allocation, not record-order ties).
### TU context -- measured, inert
- Unrelated functions placed before CreateMapMarkers in the TU (a pure int function; a function with the same manager / Reset /
  AddChild loop, inline or auto_inline(off)): byte-identical. Preceding TU content does not reach this function's allocation.
### Inline threshold
- Default Reset text (no RESET_ELSE_NULL) + /d2inlT65 reproduces retail's whole call list (133 v 133, the 9-site Reset split with
  the DEFAULT text whose sites read Data after new, as retail 0x005FF8C7 / 0x005FFD4D do): score 95*(1/f+2)/3 = 64.35 at f 31.25
  (expanded) v 65.4 at f 15.6 (called). Grade DIFFER(4048v4176), masked residue worse (1078 v 973 retail-only bytes). T64 3952,
  T66 4160. FITTED either way (sibling 0x00605570 is landed with /d2inlT107 in the same TU: no single T serves both), recorded as an
  alternative to C4's fitted RESET_ELSE_NULL text, not adopted.
### c2 colouring facts (tmp/prio_base_tr.txt, c2prio --trace --blocks on the best source)
- manager piece #218 (head) prio 27 takes ebx (cost ebx 500); const-0 head piece #4 prio 18 also ebx; heroMarker 24 -> ebx.
  Retail needs ebx for const 0 at the head then parent through the hero block: i.e. a parent piece (or const-0 piece) above 27,
  or the manager piece below 18. Whole-function block shares: parent is referenced only in block 5 (AddChild, K 1, +4) and live
  through blocks 1-4 (-2 -2 -8 -2), so no hero-region parent piece can win with this IL.
- Retail ebx use is sparse: const 0 (head), parent (hero block, loop-1 AddChild, loop 3 0x005FFBA3, loop 4 0x005FFCD9..path loop
  0x006000A0), the Position.X temp in the loop-1 arms, `bl` at 0x005FF941, 0x005FFA5E.. loop 3; NOTHING in loop 2 or the list
  destructor loops (retail keeps the dtor node in a frame word [esp+0x4c]; ours ebx).

## Row 2 0x005F1B10 CTCInventoryAbilities::ConstructPCMagicList -- DIFFER(8383v8383) (same length), best differ/005f1b10.cpp (= probe/b/E4.cpp)
Code identical to retail except the frame-word assignment (packer) -- 167 differing instructions, all `[ebp+x]` permutations or
relocation fill (pairs 54/58, 10/14, 2c/34, 24/2c, 34/28, 28/30, 30/14, 30/24, 1b/13, -24/-20).
What moved it (on C5's 8367v8383 best), each graded:
1. Same-TU bodies of `long GetAbilityLevel(EHeroAbility) const { return Abilities[ability].first; }` (retail 0x005EDE93, loads
   Abilities._M_start before the index, exactly the order at 0x005F2E7C / 0x005F2F3B) and GetAbilityGraphicIndex (0x005EDE7F,
   `Abilities[a].second->IconGraphicIndex`), placed after the auto_inline(off) block so c2 auto-inlines them (/O2 = /Ob2), and
   `GFIntToWideString(GetAbilityLevel(*it)+1)` x2 + `LevDescription[GetAbilityLevel(*it)]`: the `lea edx,[eax+1]` comes from the
   inline's return temp. 23 -> 21 masked hunks. (GraphicIndex sites code-identical either way; kept as written in C5's text.)
2. `NUISystem::CText* descriptionText = static_cast<NUISystem::CText*>(description.Get()); descriptionText->SetValue(...)`: retail
   loads description.Data into edi BEFORE the L"" temp (0x005F3420). 21 -> 17.
3. Loop-1 bound: a named `unsigned long count` now loses esi (with step 1's extra candidates c2 colours it into memory [ebp+6Ch]),
   and the if/else form `if(size <= total) count = size; else count = total;` gives retail's per-arm stores (0x005F1BED /
   0x005F1BF2; a ternary stores once at the join). 17 -> 14 masked hunks, length 8383 = retail.
Lesson: the predecessors' "count takes esi" (C5 c2prio: count prio 41) was decided by the K of blocks; adding the inlined accessor
IL changed it -- fix load-order hunks first, re-test earlier dead ends after every IL change.
### Row 2 residue (frame packer), measured on probe/b/E4.cpp (tmp/pk_E4.json, tmp/pkslots_E4.txt; ebp offset = pkslots off - 8)
- Ours sorted (density = w*1000/size): slot38 {_entry 13, _swap 11, _action 11} w35 @0x24, slot51 {faded/deletion/inside/children}
  w31 @0x2C, frame w15 @0x34. Retail: slot51 @0x24, frame @0x2C, slot38 @0x30. sim.layout what-if: slot38 w35 -> 29 (exactly -6)
  gives retail's three offsets (w-5: only 51/38 swap; w-7: 38 drops behind slot35/36). So retail's entry/swap/action slot weighs
  29: most likely a different JOIN (retail 0x30 also hosts temporaries that are in our slot15 @0x14: 0x005F225D, 0x005F232F,
  0x005F240F, 0x005F244B, 0x005F25D3.. use [ebp+30h] where ours use [ebp+14h]) -- i.e. one of _swap/_action (w11) is in another
  slot in retail and a w5 item (candidates: _slot, _def, the #temps) joined slot38.
- Other pairs: retail 0x10 holds items of our slot2 (0x08) and slot15 (0x14) (0x005F2436.. 0x005F28C7..); retail 0x54/0x58 swapped
  with ours 0x58/0x54 (0x005F1EDD.., 0x005F2011.., 0x005F2E1B, 0x005F3096..); retail 0x28 = ours 0x30 (0x005F2ECB, 0x005F3701..).
- Tried (inert or worse): hoisting the common swap / action tails out of the expression block's if/else (P1 8278, P2 +4 hunks:
  retail has them duplicated), GetAbilityGraphicIndex at any subset of its 4 sites (code + frame identical), isSpell / def
  declaration order (Q1 worse, Q2 identical).
- Next: decode the join order (pkg.py 'place'/'join2' events in tmp/pk_E4.json: item order and why _action / _swap join slot38),
  then find the source fact that makes retail join differently (scopes of the spell-block `action` / `swap` and of the main-loop
  `entry`; temporaries of the expression block). No behaviour test written yet (model: prev_C5/prev_C4/test/0060eeb0.cpp +
  tools/mktest1.py).

## Session summary (agentC7)
Grades: 81 lines in ATTEMPTS.log (31 row 1, 50 row 2) + 2 inline traces + 3 c2prio traces + 1 packer trace + fwsize.
No row matched (no src/ / test/ / payload.json). Header changes: none (hdr/ = C5's). Names: GetAbilityLevel / GetAbilityGraphicIndex
are PDB names (LABELS.tsv); descriptive local names: count, descriptionText. Asserts (C6 lead): none placed in either row (no
effect measured, so none is evidenced).
- Packer join order (tmp/pk_E4.json, 8-byte items by weight): _action/14 -> slot37, _entry/13 -> new slot38, _swap/11 (expression
  block, if/else) -> 38, _action/11 (spell block, Reset reuse) -> 38, ... Retail needs slot38 = 29: e.g. entry 13 + swap 11 + a w5
  item, or the same three with 6 fewer weight units (identical code => IL that folds away, e.g. copies / temporaries).
- Dead FABLE_RETAIL_ASSERT(frame) x1/x3/x6 after the FindChild: byte-identical (dead arms do not add packer weight either).
