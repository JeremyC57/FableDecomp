# agentC21 (acctC lane) -- NOTES (continues agentC18: prev_C18/, agentC15: prev_C15/)

## c2 inline cost model, completed (measured; c2.dll 0x1078D1AA, tools/c2dis.py; mini probe probe/mini + tools/mq.sh, tools/osz.py)
- `sz` (callee +0x81) = the BYTE SIZE OF THE CALLEE'S OWN OPTIMISED COMDAT as compiled in this TU (it is compiled first, bottom-up).
  Proof: mini probe sz 53 / 103 / 116 / 89 = objdump sizes of ReleaseData / Reset default / Reset ELSE_NULL / `{ReleaseData(); Data=data;}`
  (tools/osz.py); retail __copy<CUIMiniMapMarker> 0x00612F70 is 133 bytes = our sz 133. So dead IL (assert arms, do-while(0), Validate {})
  can never change a cost; only bytes of the standalone body count. Non-monotonic in text (a shorter text can be more bytes).
- cost = sz - sav[0] - sav[1] (+ caller-size penalty (sz/2)*(a+g)/9000 once callee a + caller growth g > 9000 tuples)
  - 10 if the callee is a LEAF (callee-info +0x50 bit 3; knob 0x1088195C = 10) - weighted savings, + sz*w/255 where
  w = 1 + 254*x/size is the callee's LOOP weight (x = tuples of its biggest innermost loop; +0x4E) -- LowerBound, _M_find, clear.
  score = (cost*1e8/f + (k-1)cost)/k, k 7 (f<=1) / 3, inline iff < 74. Nested calls of an expanded callee are re-decided at the site.
- Implicit template instantiations are compiled with the `#pragma optimize` state at the END of the TU (mini probe: a trailing
  pragma changes Reset/ReleaseData sizes). Retail's TU end state is plain /O2: 0x00612F70 = our 133 bytes.
- Reset default text: 103 bytes at /O2 = retail's /O2 COMDAT of another TU (0x008AB2B0, 103 bytes, same code). No flag / pragma
  state gives 116-117 with ReleaseData <= 57 (/Oy- 107, /Gh 108+58, /G7 104, /Oa 95, auto_inline(off) 66, optimize off 98 ...).
- FableWin flavour = /Od /Os /EHsc (fwsize FW_EXTRA="/EHsc /Os" gives SAME for ctor(), dtor, reset, DeleteData). Assert unit M = 13i/35b
  (Get / IsNull / copy ctor / operator= = text + M exactly). Reset: default text + 32i/86b (= two 2-term asserts 16i/43b, or M + 17i/44b + `Info = 0`
  2i/7b): ambiguous, no proof of an else arm. ReleaseData +4M +4i/13b, ctor(T*) +18i/49b: texts not pinned by sizes alone.
- Retail: ALL 198 /O2 Reset<T> COMDATs are 103 bytes (tools/rscan.py; 735 dtors 53 bytes) = default text. TU template state is plain /O2
  (retail __copy<CUIMiniMapMarker> 0x00612F70 133 = ours, __find 0x00612DB0 112 = ours; tools/csz.py).
- Row 2 loop-1 site 0x005FF454 is called at f >= 6.25 under ANY loop spelling (c2 multipliers measured, probe/fr: for x5 + leak to all
  later code, while x9 no leak, do-while x10, guard /2; loops inside an inlined helper do not leak into the caller; nested sites get
  site f x internal f, cold sites clamp to 1) and the path has exactly two 50/50 branches (operator== result byte, IsCore call) -> Reset
  cost >= 103 there; default cost 95 (sav 8 for &local this; 5 through a pointer / reference / member). So the +13 is real, not frequency.
- Third row of the TU, 0x00605570 ConstructRegionDescription (pending 2026-11-11_acctB_c2025, RELOCATION_MATCH only with /d2inlT107):
  retail expands GetDef<CUIMiscThingsDef> (c 106) at F but calls Reset at 8 sites at F/2 -> needs Reset cost >= 102 for any F.
- Site flags (+0x18 of the site record): 4 = in a loop, 2 = inside __try (size regime: nothing above cost 5 is inlined), 1 = __finally.
- Contexts that do NOT change sz 103: earlier unoptimised caller, explicit instantiation before / after, address taken, derived-class
  caller, inline helper caller, end-of-TU pragmas other than optimize (auto_inline(off) -> 66, inline_depth(0) -> 65).

## Reset<CComponent> price in the CTCInventoryMap TU: what is proven and what is not (agentC21)
- PROVEN: the price is 108 or 109 = a 116 / 117-byte own COMDAT (sav 8). Three rows of the TU need it (0x00603000, 0x005FF190,
  0x00605570); no frequency spelling can replace it (0x005FF454, see above); every other COMDAT of the TU prices as ours.
- PROVEN: the 116/117-byte body is NOT observable in retail (the linker kept the NUISystem /O1 copy 0x00429C6F), and no compile state
  gives it from the default text (flags, #pragma optimize / auto_inline / inline_depth at the TU end, instantiation context: all measured).
- NOT FOUND: a natural text. Hard constraints on any text: (1) /O1 body = 0x00429C6F (59 bytes), (2) row expansions = the default
  shape, (3) own /O2 body 116/117 bytes, (4) ReleaseData 53 / 0x004291DE unchanged. Harness tools/vt3.py checks 1-4 in 7 s per text
  (tmp/var*.txt = ~120 texts). Natural spellings (if (Data) / if (!data) return / Get() / temp info / dtor call / ReleaseData
  spellings) are all 103. The ONLY class that moves (3) while keeping (1)(2)(4) is a store of a member's own value on the
  data == 0 path: `else Info = Info;` (u5) = 114 bytes (the same 11 bytes as `else Info = 0;`, q1 114, which fails (1) and (2));
  `if (Data == 0) { Info = Info; return; }` (p5) 110; `if (info) Info = info; else Info = 0;` after new (p8) 105. Mechanism: c2 folds a
  reload-and-store of a member through an opaque `this` only when the release was not inlined (/O1) or `this` is &local (rows).
  ELSE_NULL's 116 = 114 + 2 for keeping `data` in edi across new, which changes the expansion (fails (2)).
- Search record (tools/vt3.py, tmp/var1..12.txt/.out, ~560 texts): texts passing (1)(2)(4) reach only 103 / 104 / 105 / 110 / 112 / 114 bytes.
  114 = `if (Data) Info = new ...; else Info = Info;` (u5; null path gets its own `mov [esi+4],0; pop esi; ret 4`, 11 bytes); 116/117 were
  reached only by texts that fail (1) (/O1 body differs) or (2). Byte accounting of the own body: default = base + new-failed `xor eax,eax;
  mov [esi+4],eax` (5) + one shared exit (4); a null path with its own tail adds 7 (register store) or 11 (immediate store); two identical
  immediate-store tails are tail-merged (z1 105).
- Price combinations that give 108/109: 116/117 - 8 (this = &local), or 113/114 - 5 (this through a pointer / reference). Not natural at
  the rows' sites (marker is a plain local in retail: `lea ecx,[esp+60h]` 0x005FF204).
- Per-row /d2inlT: row 2 T65 (C7), row 3 T107 (as handed back by chunk 2025), row 1 none (T65 flips 17 other callees). No single
  knob raises only Reset: per-argument savings knobs also move the destructor (c 47, must stay <= 51 for the 0x00603xxx return at f 0.25).

## Devices measured on the rows (none handed back as a match)
| device | row 1 0x00603000 | row 2 0x005FF190 | note |
|---|---|---|---|
| FABLE_RETAIL_CCOUNTEDPOINTER_RESET_ELSE_NULL (C4, 116 bytes, price 108) | DIFFER(9538v9571) D113, 303/303 calls, frame slots = retail (711 of 713 esp refs) | DIFFER(4096v4176) D708, 171/171 calls | expansions use `data` (`mov [eax+8],esi`), retail reloads Data; = differ/ |
| NEW FABLE_RETAIL_CCOUNTEDPOINTER_RESET_ELSE_KEEPS_INFO (114 bytes, price 106) + `/d2inlT72` | DIFFER(9555v9571) D484, calls = retail, every Reset expansion = retail bytes, but 333 esp refs on rotated slots (probe/r1/k0.cpp) | DIFFER(4112v4176) D706, 171/171 calls (probe/r2/k0.cpp) | two stacked devices; T72 is the only threshold that works with price 106 |
| default text + `/d2inlT65` | 8 other callees flip (dtor sc 67, push_back x7 sc 68/69) | DIFFER(4048v4176), 170 calls (one decision wrong) | C7's claim for row 2 is not exact |
| default text, interesting site forced through a noinline helper (DIAG, d0G) | DIFFER(9570v9571) D495, slots rotated | - | call target wrong by construction |
| default + `(void*)data`, same forcing (DIAG, g_V1) | DIFFER(9507v9571) D100, slots = retail | - | best D of all, but the member reload is missing at every expanded site |

## Row 1: what the frame says (new)
- With the retail-shaped expansions (member reload, k0 / d0G / V2 / V3) one 4-byte frame word sits FIRST in the 0x2C..0x58 block where
  retail has it LAST: retail [esp+58h] = the creatures list node / iterator / later loop counters (0x006035A7, 0x006035F3, 0x0060391A),
  retail [esp+2Ch] = the traders-loop `trader` home (0x00604E93 / 0x00604EA0) that our code never stores (C15 residue (d)); a second
  rotation in 0x88..0xA4. Texts that keep `data` in esi across `new` (ELSE_NULL, V1) put every slot where retail has it. So the "D113"
  of the ELSE_NULL differ is partly a coincidence of register pressure: the honest state of row 1 with retail's expansion bytes is D~490
  with ONE slot-order cause, and the traders loop (`trader` in a frame word, `it` in esi, parent read from [ebp+8] at 0x00604A94 /
  0x00604E35) is where that slot order is decided. Work this before anything else on row 1; alignment pads follow.
- Return cleanups: retail keeps a full private copy of [free maps; marker release; delete; epilogue] at the `maps.empty()` return
  (0x0060339B..0x006033DC, last reload through eax) and cross-jumps the IsMapLoaded return into the function end (0x0060347A
  `test edi,edi; je 0x605534; push edi; jmp 0x60552C`, reload through edx at 0x0060554D). Ours (k0) does the opposite (maps.empty()
  merged, IsMapLoaded private). Loop forms L1 (flag + break) = same bytes as d0; X4 (`&& loaded` in the for) changes the loop code.
- Other real hunks in k0 (tools/fdiff.py tmp/q_k0.txt): 0x006031D7 movsx ecx v edx + push order; 0x00604852 the mem_fun_ref(IsDemonDoor)
  functor word [esp+30h] v ours +34h; traders loop 0x00604A80..0x00604B9E; 0x00604E35..0x00605301 (esi/edi swapped for the marker
  pointer, `test dword [ecx+34h],10000000h` v our byte test at 0x00605075, [esp+24h] counter in edi v memory); 0x00605201 byte flag store order.

## Row 2
- Not worked beyond the device grades above (D ~706 either way; 83 of 200 esp references on other slots; same 80-byte deficit as
  C18 described). The Reset price question is the same; the frame-home residue has to be worked on a base with the retail-shaped
  expansions (probe/r2/k0.cpp), not on the ELSE_NULL base.

## Descriptive names: none new. No new retail names, labels or name fixes this session.
- k0m/k1 (probe/r1/k1.cpp) = k0 + named local `const CMiniMapDisplay::CMarkerInfo* markerInfo = *info;` in the non-auto-markers loop (retail homes it at [esp+2Ch], 0x00604E93): DIFFER(9571v9571) D34, all 737 esp refs on retail's slots, cleanup cross-jumps = retail. Remaining real hunks: 0x006031D7, 0x00603C22, 0x00604B40, 0x00605201 (+ pads that follow).
- k2 = k1 + followers condition as nested ifs (`if (IsThingAsPosBeingDrawn(..)) if (a || b || c || d) {`: retail's not-drawn exit 0x00605114 jumps past the body, the or-chain alone is materialised at 0x00605201): DIFFER(9571v9571) D29. Left: 0x006031D7, 0x00603C22, 0x00604B40.
- differ/00603000.cpp = k2 (DIFFER(9571v9571), D29, needs FABLE_RETAIL_CCOUNTEDPOINTER_RESET_ELSE_KEEPS_INFO + /d2inlT72). The previous
  ELSE_NULL best (D113, no flag) stays at prev_C18/differ/00603000.cpp. The same two source facts on the ELSE_NULL base give
  DIFFER(9557v9571) D89 (probe/r1/b0m.cpp, only the markerInfo local applied).
- The three hunks left in k2 do NOT depend on the Reset device: the default text with the interesting site forced out of line (k2G, DIAG)
  has exactly the same three. Tried on them (all leave D29 or break the row): traders loop as while / `trader` declared outside / iterator
  declared outside / postfix `it++` (traders, teleporters); 0x006031D7: operands of the != swapped, `currentMap` local, `worldMap`
  reference, `heroRegion` local -- each of these moves ~270 frame references and 12-32 bytes (D ~980): the row's allocation is balanced
  on that statement, do not respell it. Not tried: the creatures / teleporters loop bodies for 0x00603C22 (retail reloads edi =
  [esp+38h] before esi = [esp+34h] after the push_back at 0x00603C14).
- Row 1 order of work for a successor: (1) decide the Reset device (below); (2) 0x00603C22; (3) 0x00604B40 (retail `mov eax,[esp+24h]`
  at the traders loop top although eax holds it on both edges); (4) 0x006031D7; then test + vland.
