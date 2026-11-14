# wfB2 NOTES (acctC lane, w22) -- rows 0x00603000 CreateMiniMapMarkers, 0x005FF190 CreateMapMarkers. Neither matched.
Continues agentC21 / C26 / C31 (prev/NOTES_*.md). ~45 grades + 10 traces. No header change (hdr/ = the copied overlay, untouched).

## What is new in this session (all MEASURED; tools in tools/, probes in probe/fq/)
1. c2 static-frequency rules, measured on small probe TUs with the inline tracer (tools/fq.sh <probe.cpp> <tag>; probe/fq/p1..p6.cpp;
   one `mark()` site per position, f printed in order). Entry f = 1.
   - `if (c) X` : X 0.5, join 1.   `if (a && b) X` : X 0.25 (each && operand halves).   `if (a || b) X` : X 0.75.
     A condition with temporaries (materialised bool) or `bool b = ...; if (b)` : 0.5 whatever its inner shape.
   - `if (c) return;` : rest 0.5.   `if (a && b) return;` and `if (a) { if (b) return; }` : rest 0.75.   `if (a || b) return;` : rest 0.25.
     `if (!c) NoReturnCall();` (live call of a __declspec(noreturn) function) : rest 0.5.
   - `for (init; cond; inc) B` (ANY spelling: ++i / i++, index or iterator, i declared outside, != or <, inline size()) :
     B = 5 x pre, and the code AFTER the loop stays at 5 x pre (the leak); `return` inside B does not reduce the leak, `continue`
     halves the rest of B only, `break` ADDS its edge (after = 7.5 x pre for one 50% break; `cond1 && cond2` in the header too).
   - `for` WITHOUT increment clause, `while` : B = 9 x pre, after = 1 x pre (no leak). With a `return` inside: after = 0.5 x pre and
     the return arm itself is scored 0.5 x PRE (not x body) -- row 1's IsMapLoaded loop as `while` makes its return-path
     destructor f 0.125 -> CALLED (retail expands it), so that loop is a real `for`.
   - `do B while (c)` : B 10, after 1.  `do { } while (0)` : B 10 (!), after 1.  `if (n) do { if (x) return; } while (..)` : after 0.75.
   - `for (;;) { B; if (x) break; }` : B 1 (not seen as a loop), after 0 (!).   switch: 1/3 per arm for 3 arms, 0.1 per case for a
     4-case jump table, join 1 (minus returning arms).
   - `if (c) { for (...) B }` : join = 0.5 + 0.5 x 5 = 3 x pre (a guarded `for` leaks 3 instead of 5), B = 2.5.
   - An inline helper never changes the caller's frequencies (early returns, loops, noreturn checks inside it: no effect after the call).
2. Argument savings of the inliner (probe/fq/p7.cpp): 3 for a `this` passed in a register, +3 when `this` is the address of a local
   (also member-of-local / array element / through an inline wrapper or operator=), +2 per stack argument, +3 for a constant
   argument. `this` through a named reference or pointer to the local (`T& r = local; r.f()`) loses the +3: Reset would price 98
   instead of 95 (called iff f <= 3.77). No natural spelling of these rows does that (retail addresses the local directly).
3. Caller-size penalty: pen = sz x (a+g) / 18000 once a+g > 9000 (row 1: g 1971 at the first decision, 3193 at the creatures site,
   9009 only at the last four decisions) -- not a lever for the Reset split.
4. Anniversary (Xbox 360) PDB locals of both rows (weaker source, later revision; stack locals only) -- tmp/anniv_mm.txt / anniv_cm.txt:
   - CreateMiniMapMarkers(pmini_map, region, updateCurrentRegionInfo): function level ptc_followed, regionNum (const long, NOT in TLC:
     retail recomputes GetRegionNumberFromName per pass), phero, region_maps, followers, demon_door_list, hud_markers; first block
     prison_it_2, prison_it, hill_it; hero block relative_pos; creatures: creature_list, ptc_followed, followers, relative_pos (no named
     filter object; in TLC the named `hasInformation` IS byte-evidenced: as a temporary the row is 9511/9530 bytes); teleporters:
     marker_list, ptc_teleporter, relative_pos, marker (the CUIMiniMapMarker); houses: building_list, pbuyable_house, marker,
     relative_pos; wives: wives, marker, ptc_wife, relative_pos; enemies: enemies, relative_pos, marker; demon doors: relative_pos,
     marker; traders: trader_list, ptrader, ptc_shopkeeper, relative_pos, marker; hud: pos, relative_pos; henchmen: relative_pos, marker.
   - CreateMapMarkers(pmap, regions): quests, phero, region_quests; loop 1: quests (the per-region list copy), ptc_card,
     newRegionNumber; loop 2: ptc_card, region_number (long), found; loop 3: ptc_card, region_new_number, offset (C2DVector),
     position (C2DVector), found, newRegionNumber. (TLC's loop 3 writes through GetStates()[0] per case: the 360 locals are a later text.)
5. Structural measurement (tools/sx.py <q.txt> r : frame- and register-masked diff): the no-guard default source of row 1
   (probe/r1/a4.cpp, DIFFER 9698v9571, 303/304 calls) differs from retail ONLY by the one Reset expansion at 0x00603833, loop
   alignment pads and register / reload placement: 3 rets, every exit kind, arm order and destructor copy already equal retail.
   So no exit kind is wrong in row 1; the statement-shape method has nothing left to change there.

## Row 0x00603000 -- state
- Regraded starting states on this overlay: prev d0 (DIAG guards, ParentThing->) DIFFER(9571v9571) 303/303, 6 real hunks; a1 (DIAG
  guards + GetParentThing()) 9555v9571, 92 real; a4 (no guard) 9698v9571, 303/304 calls (trace tmp/inl_a4.txt: Reset site 160
  f 15.625 sc 65 expanded, retail calls it at 0x00603838; LowerBound site 143 f 15.625 sc 60).
- Needed (price 95, T 74): creatures Reset f <= 2.97 and the creatures LowerBound (GetTC<CTCFollowed>) still expanded, i.e. the
  creatures loop 5.48..8.58 times less frequent; the two early-return destructors and the IsMapLoaded in-loop return (dec 76) must
  keep f >= ~0.3. With rule set 1 NO natural statement between `if (maps.empty()) return;` and the creatures loop gives a factor
  in that window: the only reducers are returns (x0.5, x0.75) and non-leaking loops (x0.2 relative to `for`), and the retail bytes
  have neither a further return nor room for a `while` (b1/b2: IsMapLoaded as while = 9459 bytes, dec 76 and LowerBound flip).
- NEW device-free-of-flags state (FITTED, same family as the landed / pending `if (it != end)` loop guards of 0x00607330 /
  0x006096E0): three loop-entry guards that c2 folds into the loop's own entry test (no bytes) --
  `if (!maps.empty())` before the IsMapLoaded `for` (after-loop 3 x instead of 5 x), `if (!maps.empty())` before the creatures
  outer `for` (x0.5), and `it` declared before `if (it != creatures.end()) for (; it != creatures.end(); ++it)` (x0.5) = 6.67.
  Result differ/00603000.cpp: DIFFER(9571v9571), 303/303 calls, DEFAULT Reset text, no flag, no GFDiag guard, 6 real hunks at
  the three KNOWN sites only (0x006031D7, 0x00603C22, 0x00604B40) = exactly C26's DIAG state without its dead guards.
  Two guards only (e2 / e3) leave Reset expanded (f 3.9). `if (!creatures.empty())` in front of the for (test before the `it`
  store) is NOT byte-neutral (0x006035F3 je before the store).
- The three hunks (unchanged since C21):
  * 0x006031D7: closes with EVIDENCED `GetRegionNumberMapIsIn(GetParentThing().GetCurrentMapNumber())` (landed 0x00605570), but then
    ours keeps `this` in ebx through the teleporter loop (92 real hunks, differ/00603000_getparentthing.cpp). Measured coupling: the
    `this` allocation is right exactly when EITHER `ParentThing->` is spelled at the top OR the creatures guard tests BEFORE the
    `it` store (e1 / k2: 6 real hunks = 0x006035F3 + the two below); with the store first AND GetParentThing() it flips (e4, k1,
    k3, k4, m3..m5, c1..c4, f1..f4: all the same 92). So one unit of weight on `this` / the creatures `it` decides it.
    No effect: one shared loop index declared once (c1..c3; VC7.1 splits the webs), GetParentThing().GetTC(&followed) at either or
    both sites, guards on the teleporter loops, postfix `it++`, const_iterator.
  * 0x00603C22: at the end of the teleporter if-body retail restores `this` (edi, [esp+38h]) BEFORE `it` (esi, [esp+34h]); ours the
    other way round. Both are restores for the join at 0x00603C2A (the false arm 0x006039C2 jumps past them).
  * 0x00604B40: retail reloads `it` from [esp+24h] at the traders loop top; ours carries it in eax over the back edge (bottom blocks
    are identical, 0x00604DE5). A register-web decision for `it`, not a statement.
- Not tried / next: the remaining freedom is which no-byte spelling adds one direct use of `this` (or removes one of the
  creatures `it`) with GetParentThing() in place; and whether retail's frequencies come from a different Reset price after all
  (readings a / c of C31) -- the fitted guards prove only that f in the C26 window reproduces every inline decision.

## Row 0x005FF190 -- state
- Regraded: prev p0 (RESET_ELSE_NULL) DIFFER(4096v4176) 133/133, 154 real; d0 (DIAG guards) 4128v4176 133/133, 116 real; a0 (no
  guard, default Reset) 4636v4176, 133/140 calls.
- Frequencies needed (C26): loops 1-2 k in (2.19, 6.56), loop 3 k in (5.26, 10.5). The top of the function has no early return
  (retail 0x005FF190..0x005FF2C0 read: GetInstance, marker ctor, hero, `if (hero && !IsDead)` only), so again nothing natural.
- Predicted from rule set 1 and CONFIRMED by grade: `if (!regions.empty())` before loop 1 + `if (!cards.empty())` before each of the
  three cards loops gives loop-1 sites k 4 and loop-3 sites k 5.56 -> differ/005ff190.cpp DIFFER(4160v4176), 133/133 calls with the
  DEFAULT Reset text, no flag, no GFDiag guard (three guards: 133/134; two: 133/138). 148 real hunks remain = allocation from the
  head (retail: `this` stored at entry [esp+14h], manager only in [esp+10h], 0 then parent in ebx, heroMarker in [esp+34h]); the
  cards guards are the test-before-store form and must become the `it`-declared-first form to be byte-neutral (not done: needs
  each loop in its own block). Row 2 was only measured, not worked hunk by hunk.

## Names (unchanged, proven by predecessors): both rows Ego_r + FableWin PDB names; callee 0x00429C6F = CCountedPointer<CComponent>::Reset
(reloc_bindings). No new label, no name fix.

# SUCCESSOR SESSION (second agent of wfB2) -- measured additions; neither row matched yet
Tools added: tools/mv.py (variant sweep from a python dict of text replacements, one grade line each), tools/hk.py (row 1: which of
the known sites still differ), tools/pp.py + tools/pagg.py (c2prio JSON: per priority pass, the blocks that feed one candidate /
piece, aggregated by source region). Probes in probe/s1/. Priority traces: tmp/pt_s1d.* (differ/00603000.cpp), tmp/pt_s1gp.*
(the GetParentThing variant), tmp/pt_t7.* (no `length` local in the teleporter loop).

## Row 0x00603000 -- what the three hunks ARE (c2prio traces, ~40 grades + 3 traces this session)
- Sibling STL layer set (LIST_BASE_CONVERT_ALLOCATOR + MALLOC_ALLOC_ALLOCATE instead of ALLOC_PROXY_REBIND, with or without
  FABLE_RETAIL_CCOUNTEDPOINTER) on the unguarded default source: NO change (9698v9571, 303/304 calls): the layers only move a+g, which
  is under 9000 at the creatures site. FABLE_RETAIL_CTHING_PEEKTC_ORIGINAL_CHAIN (FableWin-evidenced, used by 0x0061E6D0): no byte change.
- Hunk 0x006031D7 / the `this` flip (EXPLAINED): `this` (candidate #10, whole-function priority negative) is split; one piece (#155)
  covers head + cached-return + maps + erase + hero + the rest of creatures + teleporters + houses + wives (247 blocks). It competes
  for ebx with the teleporters list head `n` (prio 32, key 740). With `ParentThing->` at the top the piece has prio 27 -> `n` is
  coloured first (ebx), the piece is split again (edi in the teleporter loop = retail). With the EVIDENCED GetParentThing() the
  inline's result temp is one more candidate in the depth-0 block of the region compare (K 5 -> 6), `this` costs 6 there, so the
  piece gains exactly +6 = 33 > 32 and takes ebx through the teleporter loop (the 92 hunks). So retail needs piece <= 32 (ties go to
  `n` by key) or `n` >= 34 with GetParentThing() in place: ONE unit of K anywhere in those 247 blocks or in the teleporter loop.
  Measured closers of this contest (each breaks other bytes, so none is the source fact): the creatures guard testing before the
  `it` store (predecessor), `miniMapMarker.GraphicIndex = 0;` moved before the UIDefName assignment or before `length` in the
  teleporter loop (L8 / L11: 0x006031D7 closed, the store of 0 lands at 0x00603B7F / 0x00603B9D instead of 0x00603BB9), no `length`
  local in the teleporter loop (t7: `n` 44, but the length is stored in both arms, 0x00603B9D).
- Hunk 0x00603C22 (reload order `this` then `it` at the teleporter if-body end): NOT the colouring order of the two pieces (t7 has
  the same colouring order as d -- it-piece prio 160 before this-piece 152 -- and emits retail's order). Fits "ascending piece id"
  in the three cases read (d teleporters it #5 < this #262/#333 -> it first; t7 this #335/#338 < it #377 -> this first; d houses
  this #75 < it #109 -> this first); piece ids come from a free list, so the order depends on the whole split history.
- Hunk 0x00604B40 (traders loop top reloads `it`): in ours the `it` piece #141 = {a depth-1 block of the `for` line, the top block,
  the bottom block} (prio 48) -> eax is carried over the back edge; the creatures loop (which matches) has {preheader} and {bottom}
  only and loads `it` at the top as a temp. Retail's traders loop has the creatures shape. No effect: the fitted guard shape on the
  traders loop, `it` declared outside, shopKeeper declared first / `= 0`, the condition split into nested ifs.
- Teleporter-loop statement variants graded on the GetParentThing base (all worse or equal): `teleporter = 0` (9571 bytes but 105
  real hunks), Reset before PeekTC (97), `thing = *it` for the first use (same), position declared before Reset / after it (same),
  miniMapMarker declared at the top of the if-body (43, 9570), teleporter declared before the if (same), PeekTC in the condition (104),
  DefNameLength without the local: plain / `(long)(unsigned long)` cast / IsEmpty ?: (19 / 19 / 11 real, store in both arms),
  through an inline by-value setter (= the local, 92), if-form length helper (114), `long length = 0; if (!IsEmpty) ...` (95).
- The `const long length` local (all six marker loops) is byte-evidenced only as "the length lives in eax across the join and is
  stored after the push_back prologue" (0x00603B97..0x00603BB2); an inline by-value parameter gives the same IR. Whether the
  original is a local or such a parameter cannot be told from this row.

## Row 0x005FF190 -- successor session: 148 -> 46 real hunks by statement shape (differ/005ff190.cpp, DIFFER(4192v4176), 133/133 calls)
- THE closer (statement shape, no device): `parent->AddChild(marker);` is written inside EACH arm, not once after the if-chain.
  Loop 1: `if (IsCore) { Reset; Position; AddChild } else if (IsOptional) { ... AddChild } else if (IsVignette) { ... AddChild }`
  (the old `else continue;` is gone). Loop 3: `if (first == 1 && second == 0) { AddChild } else { switch ...; AddChild }`.
  VC7.1 cross-jumps the identical tails, so the bytes show one AddChild per loop, but in c2's allocator each arm block then
  references `parent` with a large K: `parent` outranks `manager`, takes ebx from the head (retail `xor ebx,ebx .. cmp edi,ebx ;
  mov ebx,[ebp+8]` 0x005FF1B0..0x005FF1CD), manager stays in its frame word [esp+10h] and heroMarker in [esp+34h] -- the whole
  head / hero block / loop 1 now equal retail (first structural hunk is 0x005FF715). Loop 3's shape also closes the branch
  polarity at 0x005FFAE0, the four switch-case schedules 0x005FFB06..0x005FFB3E and the loop-4 pad 0x005FFBF4.
- Other evidenced fixes: `CTCQuestCard* card;` uninitialised in loops 2 / 3 (retail takes the stale frame word when the HasTC bit
  is clear, 0x005FF704 / 0x005FF858); loop 3 has its own iterator (retail home [esp+1Ch], loop 2's is [esp+20h]); loop 4 has its
  own index (retail home [esp+20h], loop 1's is [esp+34h]); NO FABLE_RETAIL_C2DVECTOR_NAMED_RESULT_SUB (retail stores X before it
  computes Y at 0x005FFCAF; the paths loop is unchanged by it).
- Guards (FITTED, still needed for the Reset split: without any 133/140 calls, f 6.25 / 3.1 / 1.56 in loop 1 and 31 / 15.6 / 7.8 / 7.8
  in loop 3, retail calls all three of loop 1 and the last three of loop 3): the byte-neutral forms are
  `i = 0; if (i < regions.size()) for (; i < regions.size(); ++i)` (NOT `!regions.empty()` / `size() > 0`: cmp + je at 0x005FF2C8) and the
  iterator declared first + `if (it != cards.end()) for (; ...)`. A guard on loop 4 changes nothing and is dropped.
- Remaining (all allocation, measured with tmp/pt_u0 / pt_u1 traces):
  * loops 2 / 3 (0x005FF6AB..0x005FF803): retail keeps the function-level cards head in esi (hole + restore from [esp+24h] around
    the inline LowerBound 0x005FF745) and the source-list address in edi; ours the reverse (`n` whole prio -479 is split late, the
    source address temp prio 56 takes esi first).
  * loop 4: retail holds the name sprite in esi across SetGraphicIndex and GetStates (one load 0x005FFC63) = a NAMED sprite local
    used for both calls (differ/005ff190_namesprite.cpp gives retail's instruction shape there), but then the loop-4 offset temp
    (prio 212) is coloured before nameSprite (208) and takes esi, parent loses ebx to `this` and loop 1 changes (175 real hunks).
    Retail's order needs nameSprite > offset temp (4 units: nameSprite then gets esi, the offset edi, the loop-top begin temp esi).
  * 0x005FFD3F `push 0` (ours `xor reg,reg ; push reg`): a const-0 piece.

## Row 0x005FF190 -- MATCHED (RELOCATION_MATCH, test PASS under vt.py, addmatch done, read-only vland RELOCATION_MATCH PASS)
src/005ff190.cpp, test/005ff190.cpp. No header change (hdr/ = the copied overlay, untouched). ~75 grades, 5 priority traces, 1 inline trace.
What closed it, in the order found (each step is in the source's header comment with retail addresses):
1. `parent->AddChild(marker);` inside EACH arm (loop 1's three arms, loop 3's two arms) -- 148 -> 46 real hunks (see the section above).
2. `CTCQuestCard* card;` uninitialised in loops 2 / 3, loop 3's own iterator, loop 4's own index, no NAMED_RESULT_SUB.
3. Region-name loop: named `nameSprite` used for SetGraphicIndex AND GetStates (retail loads it once into esi, 0x005FFC63), and a
   named `C2DVector position` in its teleport-marker arm. The sprite alone made the row WORSE (175 real hunks: the loop's offset
   temp, prio 212, was coloured before nameSprite, 208); the named `position` is the companion fact that restores retail's order.
   The pair went 46 -> 6 real hunks in one grade.
4. Paths loop: named `pathSprite` used for both calls (retail `mov edi,ecx` 0x005FFFFB keeps it across SetGraphicIndex).
5. Hero block: `GetParentThing().GetCurrentMapNumber()` (0x005FF21A: eax / ecx roles and the push order; same fact as row 1's 0x006031D7).
FITTED (coordinator decision): the four loop-entry tests c2 folds away (first regions loop + the three cards loops). Without them
DIFFER(4612v4176), 133/138 calls. EVIDENCED: everything else.
Alternative to the guards, measured (probe/s2/za.cpp): plain `for` loops + no-byte `if (!(c)) __assume(0);` statements (two at the top
on manager / parent, one after each GetTC on card) reach the same Reset split: DIFFER(4176v4176), 2 real hunks with a sloppy
placement (one duplicate in loop 1). Retail's release build does use `__assume(0)` macro bodies (landed CPersistContext rows), and
both rows carry locals that only a compiled-out check would read (`teleporterRegion`, `worldMap`). Not handed back; a coordinator
decision if an assert-shaped macro is preferred over the loop tests.
vland warnings (names, not bytes): 0x007021F0 is catalogued 'CTCQuestCard::GetMarkerRegionName' -- both PDBs only have
?GetTeleporterRegionName@CTCQuestCard@@QBE?AVCCharString@@XZ (LABELS.tsv); 0x007022E0 is catalogued
'CTCShotDefinitionBase::CanBeHitByMeleeStrike' but reloc_bindings binds ?IsVignette@CTCQuestCard@@QBE_NXZ there (folded bodies).

## Row 0x00603000 -- what to try next with row 2's closers (NOT yet tried there)
- Cross-jumped duplicates hide statement shape: any statement retail shows once after an if-chain may be written in each arm
  (houses loop: Reset + UIDefName arms then one `if (owned || forSale)` block; wives loop the same) -- K of the arm blocks changes
  the priorities of `this` / `parent` / the list heads without changing a byte of the arms.
- Named locals come in pairs: a pointer used for two calls (sprite) and a named C2DVector next to it flipped a 4-unit priority
  contest in row 2; row 1's open contests are 1 unit (`this` piece 33 v teleporters head 32) and piece-id order (0x00603C22).
- Row 1, last sweep after row 2 matched (gp base = GetParentThing): in the cached-marker loop, `sprite` declared before the Position
  assignment (with or without `sprite->GetStates()`) flips the `this` contest right again but moves the sprite load (0x00603299..
  0x006032E2 wrong) -- so the loop's original order is right and this is one more K-only closer, not the fact. Pure declaration moves
  (creature / followers order, position before the if in creatures / houses / wives, house / wife declared before the if,
  GetParentThing().GetTC at the creatures site) leave the IR and the 92 hunks unchanged. State of the row = differ/00603000.cpp
  (6 real hunks, ParentThing-> FITTED) and differ/00603000_getparentthing.cpp (the evidenced spelling, 92 real hunks, one K unit off).
