# Chunk 864 -- twenty parked near misses, retried with the research rules and the c2 allocator probe

Result: **2 of 20 matched** (both RELOCATION_MATCH under q.py and the read-only vland check, tests PASS, bcheck 0 masked
differences); 18 parked again, each with the probe's deciding quantity on line 2 of `differ/<addr>.cpp`. 60 grades and
~30 probe runs this chunk (ATTEMPTS.log; STATUS.tsv is rebuilt from it by bin/st.py).

## Matched

| row | device | grades here | what closed it |
|---|---|---|---|
| 0x0050A650 CWorldMap::GetMapNavigationAreaInit | plain C++ + header opt-in `FABLE_RETAIL_CTHINGSEARCHTOOLS_TCSINROUGHAREA_BODY` (hdr/engine/CThingSearchTools.h) | 14 | c2prio --trace: candidate #37 (`region_exits._M_start`, lines 235/239, prio 30, refs 4) took ebx only through a `-1` copy preference to `exit_it` (#38, ebx). Diagnostics Dg/Dh showed that only a call receiving an *exposed* local's address kills an exposed vector's web (the loop body's GetPos / GetNavigationLayerAt / _M_insert_overflow calls do not), so no in-loop spelling could split the web. Research 2.4 the other way round: with same-TU noinline bodies for GetAllTCsInRoughArea / GetAllTCDsInRoughArea (templates retail defines in its header) the vectors are not address-exposed, their `_M_start` webs are promoted locals (like the GetAllThings-filled region_entrances #34, prio -58) that the allocator splits, and the free's reload takes eax as retail. |
| 0x00D33730 CQ_EndGameBossBattleScript::CTheRealGuildmaster::Main | `#pragma inline_depth(0)` ... `#pragma inline_depth()` round the talk branch's first thread-check return (owner 2026-11-08 inline control, preference 3) | 2 | The only residue was one reloc-masked byte: VC7.1 expanded the implicit ~CTimer of that one return and cross-jumped it into the loop-exit copy, retail jumps to the pad that calls ~CTimer. Chunk 856's 103 plain-C++ grades and its /d2inlS0-S20, /d2inlT50-T100 diagnostics never reproduce the single-site call, so the source-level device is the least invasive one that does. Line 2 carries the `// Inline-budget:` marker. |

Both final sources were graded with q.py (`$S/wine/q/<addr>`), checked with bcheck.py (0 masked byte differences) and the
relocation symbol lists; vland on payload.json: see tmp/vland.log (both RELOCATION_MATCH).

## Parked, with the probe's deciding quantity (details on line 2 of each differ/ source)

Register / allocator class (a quantity the probe shows, no byte-preserving spelling found):
- **0x00C52BB0** (md 4): `this` 18 v the K5 Model temp 16; `Model < 4` reaches a 20/20 tie (the later-defined temp wins -> eax) by
  linking its compare to the const-4 record the switch's `sub eax,4` created, but emits `cmp 4 ; jae`. Compare constants alone never
  create a record (const 3/6/7/8 refs 0). Diagnostic D1 (`== 5` in the family-6 arm) proves the const-4 pairing is the whole gap.
- **0x0085C7D0** (md 8): scratch rotation pointer at the GetFacingVector vptr load: ours edx (the CameraManager argument load is one
  rotation step), retail eax. The first 42 instructions are byte-identical, so only the assignment path of that load differs.
  Named-local (hoists, flips R6), shared CCountedPointer/CThing models (flip the x87 product order, md 22) and reference ctor args
  (same md 8) do not move it. Callee 0x0084E810 respelled AddSubComponent (NAME_FIXES.tsv).
- **0x00D3A4A0** (md 13): one rotation temporary on the element-address line (ours) v three (retail): the `MyCageNumber << 4` chain
  is targeted into seh_me's register (seh_me #12 has 7 references). Nine spellings (expression twice, vector reference, iterator,
  casts, inline helper, 2-def pointer, const reference) keep the in-place chain.
- **0x005253F0** (md 13, was 21): FABLE_RETAIL_STLP_MAP gives retail's uid-in-ebx / this-reload shape. Left: at in-loop Replace 2 the
  GetAsCharArray join #59 (32) colours after the `line.PStringData` load #20 (40: block K=8 with the two uid halves) and takes eax.
  The preset accessor (GETASCHARARRAY_SELECT, M3) has retail's shape at every site but colours text/search_text edi/ebp in the wrong
  order at three of four sites (text 56/112/124 v search preset 64/128/128; site 1 136 v 128 is right); one more loop reference for
  `text` (M4) overshoots and flips the function-level this/context pair.
- **0x00B92300** (md 4): `this` (prio 5) is split (piece #32 ebp); every register matches retail; the residue is where the piece's
  copy and spill are emitted relative to the layer load and the handle lea. Index-first spelling: no change; mask-first: 348 B.
- **0x00A8B990** (md 317): `bones._M_finish` (#15) `split, memory` against bone.Info (#38 -> esi); the overflow body is visible, so
  it is a weighting, not exposure. Not pursued beyond the probe.
- **0x009D9C80** (md 17): the zero constant has two pieces (edx stores, eax later) where retail has one -- research 2.3 threshold.
- **0x009D8CF0** (md 12): per-site OR operand order of the bookmark temporaries (uniform in ours) -- R6-class, park with 0x009FA780.

Emission-order class (no allocator quantity; every integer candidate already as retail -- research 2.5 "IR tuple order"):
0x0047ED76 (push/fsubp), 0x00A22E10 (`mov esi,17Ch` slot), 0x006AFE60 (velocity load, alias-driven), 0x005E78F0 (x87/int
interleave), 0x0065B142 (ParentThing load slot), 0x00F36A30 (split-piece reload slot), 0x00DDAE40 (sunk fstp), 0x00CFE030
(SIB operand order of a CSE'd element address), 0x00E76E90 (block layout; per-row /d2inlS2/S6/T80 and inline_depth(2..4) are
byte-identical to the default, S12/T20/T40/inline_depth(1) break the expansion mix -- not an inline residue), 0x00B5A3F0
(RMW count/position effect). Each has 100+ earlier grades of behaviour-identical spellings; the probe adds the reading that
their registers are already retail's, so the lever is not in the allocator. These are documented structural residues until a
tuple-order readout exists (C2_ALLOCATOR_PROBE.md section 7 follow-ups).

## Probe findings worth keeping (also in LESSONS.md)
1. Exposed-local kill rule (0x0050A650 Dg/Dh/Y1): c2 kills the memory webs of address-exposed locals only at a call that receives an
   exposed local's address; opaque calls with no local address and calls receiving a temporary's or named-but-unexposed local's
   address do not. A vector filled by a declared-only callee therefore keeps one `_M_start` web across its own loop and the dtor's
   reload coalesces with the iterator; the fix is mod/ref (a visible body), never an in-loop spelling.
2. Compare constants never create a const candidate record; a switch lowering's `sub` does, and a compare on the same value then
   links to it (0x00C52BB0).
3. The probe's `text`-named candidates can be an inlined accessor's local of the same name: rename the accessor's local in a
   diagnostic overlay before reading a trace (0x005253F0 M3 tb2).
4. odis.py's from/to are absolute addresses, not offsets.

## Lane / housekeeping
No background jobs left; probe run dirs (tmp/c2runs) and lv/ deleted; q864 work dirs are ~2 MB and kept for the diffs.
Header overlay: hdr/README.md (two new additive opt-ins, one used by a matched row).
