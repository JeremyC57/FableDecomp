# agentC26 NOTES (acctC lane, w22) -- continues agentC23 (prev_C23/) and agentC21 (prev_C21/)

## Hypothesis "the map rows' Reset +13 is the caller-size term" -- REFUTED (3 traces + c2 disassembly)
- c2.dll 0x1078D1D4..0x1078D1F9 / 0x10742BA5: pen = (sz/2)*(a+g)/9000, applied ONLY when a+g > 9000 (0x2328) and sz > knob [0x108819A8];
  otherwise 0. It is a STEP: for Reset (sz 103) the first non-zero value is 51, never 13.
- Row 0x005FF190, default Reset text, no flag (probe/r2/d0.cpp, tmp/inl_r2d0): a+g at the nine Reset sites = 966, 1644, 1738, 1832, 3354,
  3527, 3611, 3695, 4136; function end 5508. All far below 9000; pen 0 everywhere; retail needs cost 108/109 at f 15.6 and f 31.25.
- Two dead-arm asserts inside Reset (hdrx, mkx.py `reset`): sz 103, sav 8, c1 95 unchanged; only the caller's a+g grows (+41 per expanded
  site). So asserts cannot price Reset; C21's conclusion stands (the price is the own COMDAT's bytes).
- Release-assert forms that could change the own /O2 body (vt3.py, tmp/a1.txt, 27 texts: `__assume(c)` before / after / both,
  `if (!(c)) __assume(0)`, `(void)(c)`, `if (!(c)) {}`): all 103 bytes, /O1 body and expansions unchanged. No effect.
- Alternative measured (tools/fsim.py): row 0x005FF190's retail Reset split is ALSO what the default price 95 gives if every site
  frequency were 6.25..10 times lower (exactly the five sites 74/81/233/236/239 flip, nothing else of 401 decisions); /5 leaves site 233
  (f 3.125, sc 73 v T 74) expanded. Row 0x00603000 is NOT explained by a uniform divisor (return-path destructors 63/75 flip first): there
  only site 159 (0x00603838, f 15.6) must drop below ~3. No source fact found for either; map rows stay parked on C21's device.

## FableWin assert texts of CCountedPointer<CComponent> (tools/fw.sh + tools/mk2.py + probe/fw/t.cpp, /Od /Os /EHsc, live DIAG assert)
- FableWin's assert unit is `if (!(c)) { if (F(4 args)) int 3; }` WITHOUT a do-while(0) (a do-while adds 2i/4b at /Od and misses);
  one unit with a one-member condition = 13i/35b.
- Instruction counts reproduced exactly (bytes within 0..4, the line-number push imm8/imm32 is not modelled) by tmp/full.py:
  Get = text + 1 unit `ASSERT(Data)`; get() = `return Get();` (8i/17b SAME); IsNull = text + 1 one-term unit; Validate() = ONE unit
  (FableWin 19i/44b; it is a real function in FableWin); operator-> = text + 1 unit whose condition is `Get() != 0 && <constant>`
  (25i/58b; `!IsNull()` 23i, `Data && Info` 24i, `Get()` 22i do not fit); copy ctor = text + 1 one-term unit; operator= = text + 1 unit;
  ctor(T*) = text + ONE compound unit (58i: `(Data && Info) || !Data` 177b v 176b, `(Info || !Data) && k` 178b; not pinned);
  Reset = default text + one one-term unit + one compound unit (the ctor's): 69i; ReleaseData = text + 4 units (93i/252b v 249b);
  ShareData = text + 78i/213b (six units, text not built).
- The dead-arm weight of this FULL set overshoots every big row under every non-empty release form tried (A = fable_assert.h dead arm,
  B = without the debugger test, C = `if (0) { (void)(c); }`): 0x0061E6D0 12247..11876 v 12356, 0x00601310 6371..5948 v 6931, 0x0061B610
  11951..11711 v 12341, 0x0060C900 / 0x0060EEB0 break. Form D `(void)(0 && (c))` is folded by c1 and adds nothing. So "every FableWin
  assert leaves dead IL in retail" is FALSE; any working set is a SUBSET (operator-> alone adds +1052 to 0x00601310 and overshoots).

## Caller-size windows (tools/slack.py on inline traces; D = extra caller tuples needed before the critical site, v the assert-free source)
| row | base source | window for D | critical site |
|---|---|---|---|
| 0x0061E6D0 | prev_C23 clean, REBIND, no inline_depth devices (probe/m/e6v3a.cpp) | [222, 905) | idx 549 `CCountedPointer<CAction>(T*)` at a+g 8779 must reach 9000 (the ONLY wrong decision of 798) |
| 0x0061B610 landed (REBIND) | repo source | (-353, +118) | idx 546 ctor at 8883 must stay below 9000 |
| 0x00601310 (no REBIND) | prev_C23 diag minus row arms | [706, 762) at the last end dtor, and <= 769 at the pastList/curList ctors (idx 579/589) | 7 end dtors 56 apart |
| 0x00601310 + REBIND | same + FABLE_RETAIL_STLP_ALLOC_PROXY_REBIND | overshoots by ~180 (+~940: both (T*) ctors called at 9128 / 9158) | REBIND is not neutral: +~940 tuples here |
- Get assert adds +270 before idx 549 of 0x0061E6D0 (inside its window) but breaks landed 0x0061B610 (12287 v 12341) unless REBIND is dropped.
- No single helper-level set satisfies the three windows (same result as C23): get fits 61E6D0+REBIND, breaks 61B610+REBIND; get+ctor+isnull
  fits 61E6D0 without REBIND and (with ~8 row arms; FableWin row +151i ~ 11 units) 0x00601310, but overshoots 61B610 without REBIND.

## NEW LEAD for the map rows: static FREQUENCY, not price (measured; DIAG only, nothing handed back as a match)
- Retail's Reset<CComponent> split in BOTH map rows is exactly what the DEFAULT Reset text (price 95) and NO compiler flag give when
  the static frequency of the code after a certain top-level point is 5.5..6.5 times lower than our sources give:
  - tools/krange.py (effective cost solved from each decision's own score): row 0x005FF190 uniform divisor k in (5.48, 6.56)
    [lower bound: Reset site 233 f 15.6 must be called; upper: LowerBound<ETCInterfaceType> at dec 65 / 165 must stay expanded];
    piecewise also works: loops 1-2 k in (2.19, 6.56), loop 3 k in (5.48, 11), teleport loop k < 28.6.
  - row 0x00603000: decisions <= 76 (the early-return destructors) need k < 1.27, decisions >= 92 need k in (5.48, 8.58)
    [Reset site 159 = 0x00603838 called; LowerBound dec 142 expanded]. So the divisor starts between the IsMapLoaded loop and the
    creatures loop, i.e. around the `if (hero && !hero->IsDead())` block -- the same statement that precedes the loops in row 0x005FF190.
- c2 fact (4 traces): a top-level dead statement whose arm does not return, `if (0) { NoReturnFn(); }` (callee declared
  `__declspec(noreturn)`), emits no byte, changes no register, and HALVES the static frequency of everything after it (c2 scores the
  constant `if` 50/50 and the arm's mass is lost); `if (0) { if (!c) NoReturnFn(); }` multiplies by 0.75; `if (!c) __assume(0);` halves
  (but is a live optimisation hint); the same guard inside an `if` arm of probability p only gives 1 - p/2 (0.875 in the hero block).
  NO effect: `__assume(c)`, the fable_assert.h dead arm (its `__debugbreak` arm rejoins), any assert inside an inline helper.
- DIAG proof on the rows (probe/r1/h1..h6.cpp, probe/r2/h1.cpp; default Reset text, no flag, hdr overlay):
  - 0x00603000: three halving guards anywhere between the IsMapLoaded loop and the creatures loop (or 6..7 of the 0.75 kind) =
    DIFFER(9571v9571), 303/303 calls, and ONLY the three known hunks 0x006031D7 / 0x00603C22 / 0x00604B40 (same as C21's k2 with
    RESET_ELSE_KEEPS_INFO + /d2inlT72). Placement / strength of the guards does not change those hunks (5 variants, same 6 real hunks).
  - 0x005FF190: two halving guards at the top + one before the third cards loop = DIFFER(4112v4176), 133/133 calls (= C21's k0 state).
    Three at the top is wrong (k 8: LowerBound called at dec 65 / 165).
- Not found: the ORIGINAL construct. It must be row-level (nothing inside an inlined helper changes the caller's frequencies), at
  function scope, and lose ~82..85% of the mass with no byte. Candidates not excluded: dead no-return arms of some row-level macro,
  or a loop / early-exit spelling with the same bytes but another frequency (C21: L1 flag+break = same bytes). Recurring coincidence:
  dropping ONE x5 `for` leak puts the critical site of each row at f 3.125 = score 73 against T 74 (still expanded by 1); a Reset
  price of 96 instead of 95 would then close both rows with k = 5 (C21: /G7 makes the own body 104 bytes = price 96, but the rows are /G6).
- Consequence: FABLE_RETAIL_CCOUNTEDPOINTER_RESET_ELSE_* and `/d2inlT72` are NOT needed to explain the map rows; the retail price of
  Reset can be the default 95. 0x00605570's `/d2inlT107` is a different fact (GetDef<CUIMiscThingsDef> c 106 expanded at F ~ 1, score
  106 v 74: GetDef is too dear by >= 33, not Reset too cheap); not explained by a frequency divisor (it needs a HIGHER f there).

## Row 0x0061E6D0 CTCInventoryQuests::ConstructQuestListPC -- budget CLOSED by a source fact (no device)
- `CTCInventoryMap* inventory_map = 0;` at the mini-map site (source line `GetParentThing().GetTC(TCI_INVENTORY_MAP, &inventory_map);`
  before `inventory_map->CreateMiniMapMarkers(...)`). Evidence: retail zeroes a register before the inlined TC lookup (`xor ecx,ecx`
  at ~0x0061F0A8, between `shr edx,1Bh` and `and dl,1`) and loads the found TC straight into ecx (`mov ecx,[esi+4]` 0x0061F0F2); the
  uninitialised local was homed in memory in ours (`mov [esp+290h],eax` / reload) -- found with the frame map: retail has no slot for it.
- Effect: DIFFER(12356v12356), 464/464 calls, frame 0x41C = retail (was 0x424), with the PLAIN overlay: no dead-arm assert, none of
  the six `#pragma inline_depth(0)` devices, no flag (differ/0061e6d0.cpp; REBIND kept as in the landed sibling 0x0061B610).
  So this row needs NO CCountedPointer assert: C23's "Get assert" result was a second way to pay the same caller-size gap.
- Residue (not closed): frame-slot order. tools/fa.sh (/FAs listing + efm.py votes) + tools/fmis.py: 55 of 101 frame symbols off
  the modal shift. Leads read from the map: retail gives `region_number` (block local, line `long region_number = ...`) its own slot
  near -608 where ours shares the inventory_map/type slot; retail's `state_index` shares the function-level inventory_map / inline
  `type` slot (ours: own slot); `$T` temporaries around boasts / renown_needed rotate with them. Function-level
  `inventory_map = 0` changes nothing. Next: weights of those two nodes (IL reference counts decide the packer order).

## Row 0x005FF190 / 0x00603000: null-initialised TC out-locals (same idiom as 0x0061E6D0), measured on the DIAG frequency base
- 0x005FF190: `CTCQuestCard* card = 0;` in the second and third cards loops (not the first): DIFFER(4128v4176), 116 real hunks (base
  4112, 151) -- differ/005ff190_diag_freq.cpp (probe/r2/z011.cpp). All three: 4128 / 119. First loop only: no gain. Not checked
  against retail bytes loop by loop (do that first: look for the zeroing `xor` before each inlined GetTC as at 0x0061F0A8).
- 0x00603000: none of the six TC out-locals (followed x2, teleporter, house, wife, shopKeeper) wants `= 0` (three are neutral, three
  break the row). Its three hunks 0x006031D7 / 0x00603C22 / 0x00604B40 are unchanged by guard placement, guard strength and these.
- 0x00601310: not worked this session beyond the window measurement (differ/00601310.cpp = C23's; it has no TC out-local).

## Grades / traces used (agentC26): 16 inline traces, ~45 row grades (ATTEMPTS.log), ~60 /Od fwsize probes, 27 vt3 texts.
## Names: no new retail names, labels or name fixes. Descriptive DIAG-only names (never handed back): GFDiagNoReturn, FDiagAssert.
