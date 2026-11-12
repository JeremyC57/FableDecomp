# Hand chunk 3011 (CAIStateGroup_MoveGoodsBase .. CAIStateGroup_MoveGoodsBase): 1 of 1 byte-exact

## What matched (1)
- CAIStateGroup_MoveGoodsBase::PlaceGoods 0x00915B60 RELOCATION_MATCH (src/00915b60.cpp, test/00915b60.cpp). Shape lessons: the
  exit structure (which failures share the final `return false` block) decides where c2 hoists the by-value vector's `_M_start`
  load and how its live range splits; an extra (cross-jumped) return copy is one more IL reference of every destroyed local and
  breaks frame-slot density ties. Details in the continuation section below.

## Parked for Fable 5.1 (0)

## Skipped
none

## Label corrections
- 0x007FEF40: manifest label 'CTCUserActionPointBase::FreeActionPoint' (vland WARN RELOC_NAME) is wrong; Ego_r.pdb + FableWin.pdb
  name ?SetActionPointAsFree@CTCUserActionPointBase@@QAEXJ@Z (already bound in rebuild/oracles/reloc_bindings.tsv).

# Working notes (predecessor + continuation)

## State (superseded: see the continuation section at the end -- the row MATCHES, src/00915b60.cpp)
- (old) Best source: differ/00915b60.cpp (= probe/v1.cpp = prior p12 + separate goods-null return; DIFFER(1923v1931), graded g06/g07). Behaviour test: differ/00915b60.test.cpp (PASS under g++ and VC7.1).
- Overlay: hdr/ = prior chunk 2029 overlay unchanged (README.md there).
- Grades: grades.log (g01..g07; g05 = diagnostic what-if object, counted). Every grade so far is in retry pass 1 (7 of 10).

## Tools built here (all free, none diff against retail except md.sh on a graded object)
- gr.sh <src> <tag>: q.py grade (COUNTS), appends grades.log, output tmp/<tag>.q.
- prio.sh <src> <tag>: c2prio_b trace (PlaceGoods) -> tmp/<tag>.prio/.json + tmp/<tag>.obj (object is byte-identical to q.py's).
- nb.sh <src> <tag>: gdb hook on c2.dll 0x10784b48 = the chooser's neighbour penalty `COSTS[reg] += 100 * spill(nb)` (nb = an
  uncoloured neighbour left with a single register; esi = reg index 1 eax 2 ecx 3 edx 4 ebx 6 ebp 7 esi 8 edi), + every choice
  (0x107834a4). tools/nb/nbhook.py. Output tmp/<tag>.nb.json events ["pen", head_id, reg, nb_id, nb_spill, nb_prio].
- pk.sh <src> <tag>: frame-packer trace (agent3013 pk_host) -> tmp/<tag>.slots.
- fr.sh <probe.cpp>: plain compile of a fragment probe -> tmp/<name>.fdis. odis.py <obj>: own disassembly (capstone).
- md.sh <obj> [--frame --regs -n 0]: md.py masked diff vs retail (use only on GRADED objects).

## Measurements (pass 1 of this retry)
- Head merge at the top: retail 0x00915BFC `lea ecx,[esp+14h]` hoisted above `je 0x009162B7` (jumps past the final block's own lea)
  = a separate `if (!goods.Get()) return false;` before the GetTC test (probe/v1.cpp reproduces it; p12's `goods.Get() && GetTC`
  does not).
- Loop-head padding at 0x00916047 (`jmp` + `lea esp,[esp]`, MoveToPos loop head 0x00916050) is NOT a source shape: c2 pads a loop
  head when <= 9 bytes are needed; ours sits 6 bytes earlier because exclusive_use's init is `xor ebp,ebp` (2 bytes) instead of
  retail's `mov dword [esp+30h],0` (8). Fixes itself with the register cascade.
- Offset vector: ours ALREADY stores Z/Y/X into scalar temps (Z [esp+40h], Y [esp+14h], X into forward.X); the `_offset` 12-byte
  node only holds X. Frame residue is slot ORDER, not node shape.
- Neighbour penalties for crate_index (p12/g01): ebx 1000 = action_points piece (spill 10, ebx-only); ebp 1400 = exclusive_use
  piece #29 (spill 8, ebp-only: it spans init `= 0` .. success-path SetUser(0)) + vector-start piece #94 (spill 6, ebp-only).
- Vector start (goods_names._M_start, a "param" candidate): c2 hoists its load to the IDOM of the block where a partially redundant
  read sits (the bottom `free; return false` reached from the PerformAction failure AND another path). In p12 that is the pre-route
  `if (destination.Get() && goods.Get())` (line 119): the web spans the exclusive wait loop where the User-address temp holds ebx ->
  piece #94 ebp-only -> crate_index ebx. Fragment probes probe/f/pre1..pre3 (K1: hoist to the if-head; K2 explicit fail return:
  no hoist; early returns before the if stop the hoist at the if-head; reads in other return blocks are NOT in the web).
- Variants (traces, free): t1/t2 (PerformAction failure explicit): web {index, fail copy, success} whole ebx (fail copy then not
  cross-jumped, +19 instr), crate_index ebp, exclusive esi/ecx, carrying ebx; BUT action_points piece ebx / filter ebp (swapped).
  t3 (pad C explicit, pad A as the if/else): web def at A's check (line 151), crate ebp, filter/action_points swapped.
  t4: crate tie 800/800 -> ebx. t5 (LookAt+Perform failures share the bottom): web def at the LookAt check, overlaps filter's piece.

## Key what-if result (diagnostic, counted as grade g05)
- wi.sh / priow.sh (tools/t13/c2p/c2prio_w.py = c2prio_b + env C2_ZERO='[[head_id, reg_idx]]' zeroes that head's neighbour
  penalties on that register at c2.dll 0x10784b4b). `C2_ZERO='[[42,6]]' ./priow.sh probe/v1.cpp w1t` = v1 with crate_index's ebp
  penalty removed: crate_index ebp, exclusive_use esi+ecx, filter ebx, action_points ebp, carrying ebx piece -- ALL retail; md.py
  --frame: every instruction up to 0x009160F7 identical to retail (registers too). Remaining in that diagnostic: the vector-start
  web piece #27 (lines idx 179/180 .. 190) covers idx, success AND the bottom `free; return false` (J) -> J reads ebx (retail J
  0x009162C8 reads memory into eax); because J differs, the LookAt-failure copy is not cross-jumped into it (+10 instructions),
  and the pad registers shift.
- So two facts must hold in retail: (1) crate_index's ebp cost < ebx cost; (2) the bottom J's vector read is NOT in the ebx
  piece (only idx -> success is). In every structure tried, J's read is in the piece reached forward from idx (P-fail -> J).

## Candidates tried in this retry (all on probe/, traces free unless graded)
| probe | change (base) | result |
|---|---|---|
| v1 | p12 + `if (!goods.Get()) return false;` before `if (goods->GetTC(&action_points)) {` | g06 DIFFER(1923v1931), same alloc as p12, fixes the head merge -> NEW BEST (differ/) |
| v2 | v1, MoveToPos loop as `while (MoveToPos(...) == 4 && filter()) ;` | trace: no change, no loop padding (padding is a size effect) |
| t1 | v1, PerformAction failure explicit `if (result != 2) { free; return false; }` + success last | g02 1996v1931/604i: no PRE, web {idx, fail copy, success} whole ebx (fail copy not merged); crate ebp; action_points ebx / filter ebp (wrong) |
| t2 | t1 + pre-route check explicit (all failures explicit, like prior p1) | g03 1996v1931 (pads A ecx / B edx / C eax in retail order!; rest as t1) |
| t3 | v1, pad C explicit, pad A as the if/else into the bottom | trace: web def at A check, ties filter piece at prio -71 and wins on key; layout of pads wrong |
| t4 | t3 + LookAt failure nests the crate section | trace: crate costs 800/800 -> ebx |
| t5 | A explicit, `if (result == 2) { crate..; if (Perform == 2) {success} }` then bottom (LookAt+Perform failures share it) | g04 1975v1931/595i: success path exactly retail's shape (excl. [esp+2ch] into ecx, index edx, vector ebx); web def in the LookAt loop test, prio -26, takes ebx whole before filter's piece -> filter ebp, action_points ebx, carrying memory; the shared bottom J sits inline after LookAt |
| v3 | v1, LookAt failure also into the bottom | trace: PRE still at 119, same as p12 |
| v4 | v1, `if (AIAction_PerformActionAndWait(...) == 2)` without `result =` | trace: worse (filter esi/ebp pieces) |
| w1 (g05) | v1 + what-if C2_ZERO='[[42,6]]' (crate_index's ebp penalty zeroed) | 1991/595i: retail-exact (registers too) up to 0x009160F7; vector piece #27 = {idx, success, J} ebx -> J reads ebx, LookAt copy unmerged |

## Split-history facts (web.py <tag> <root id>)
- p12 (t01): web #26 -> #94 {idx, succ x2, J x2} (ebp-only at crate_index's choice) -> split into #12 {succ} (eax 2-ref) and #8 {def, J}
  -> #127 {J} (eax 2-ref); the idx read stays memory. First sub-piece formed = {succ}.
- forced (w1t): #97 -> #27 {idx, succ, J} chosen whole at prio 26 (only ebx allowed).
- Retail needs pieces {idx, succ} (ebx) and {J} (eax/memory): J must be split off BEFORE idx's piece forms, or J must not be in idx's
  forward reach. Hypothesis (unverified): the split forms sub-pieces greedily by piece priority (p12 chose {succ} first, then {J});
  retail would then need J's piece to outrank {succ} and {idx, succ} to outrank {succ}.

## Ranked untried ideas
1. Instrument the split pass (c2.dll 0x1078B2AA; piece constructors 0x1078B494 parent esi / 0x1078BF46 parent ebp) to log each piece's
   refs and seed order; derive the rule that groups {idx, succ} apart from {J}; then choose the tail spelling. Start from v1 + the
   forced crate (C2_ZERO) so only the tail decides.
2. Tail spellings that keep p12's pad layout but move the vector-start PRE def after the exclusive wait loop (so crate_index's ebp side
   loses the 600 from the vector piece): the bottom J must have no predecessor before the exclusive section; e.g. the post-exclusive
   `!destination || !goods` check as the if/else arm into J while the pre-route check stays an explicit `{ if (goods) free; return false; }`
   copy -- t3 did this but its pads came out in the wrong order; try t3 with the exclusive section/claim reordered or with A's copy
   written as in p12's B.
3. In t5 (success shape already retail), make filter's MoveToPos/LookAt piece outrank the vector web (web prio -26 v filter -72): a web
   def earlier than the LookAt loop test lowers the web's priority (more live-through blocks).
4. Frame (after the allocation is right): measure with pk.sh; retail slots: [13h] reached+HasTC bytes, [14h] goods, [1Ch] keys+offset.Y,
   [20h] carrying+key 0x46, [24h] index, [28h] action_points, [2Fh] HasTC bytes, [30h] exclusive_use+key 0x2F, [34h] offset.Z,
   [38h] forward (offset.X), [44h] position, [50h] rh_set, [68h] action. The offset is ALREADY scalar-replaced in ours (Z/Y temps,
   X in forward.X) -- only the slot order differs (density sort; it moves with the allocation).

## Names
Unchanged from chunk 2029 (LABELS.tsv / NAME_FIXES.tsv copied, all two-source PDB names). Descriptive (made-up) names: the PlaceGoods
parameter names `distance` and `route_thing`, and the locals (carrying, goods, action_points, forward, offset, offset_distance,
reached, exclusive_use, crate_index, crate_stack, solid, timeout, end_frame, position, rh_set, index, result) -- no PDB parameter or
local names on this machine.

## Continuation agent (retry pass 1 cont., grades g08+)
- w2 = v1 with the bottom `action_points->SetActionPointAsFree(index);` falling into the GetTC block's end (the `return false;`
  after it removed, so J shares the function's final `return false` with the GetTC failure). Trace (`bash prio.sh probe/w2.cpp w2`):
  crate_index ebp (costs ebp 800 v ebx 1200), exclusive_use esi+ecx, filter ebx, action_points ebp, carrying ebx piece -- ALL retail.
  g08 = DIFFER(1938v1931) 586i: every instruction to 0x009161E9 retail-exact (frame aside). Remaining (md.sh tmp/g08.obj):
  vector start {idx, success} ebx in retail v ours idx temp ecx + success reload eax (pieces #9 {succ} eax, #1 {def@GetTC-if, J} eax,
  idx dropped); success `push edx` (rotation) v ours eax; frame: action_points [esp+28h] v ours [2Ch], HasTC byte [2Fh] v ours [2Bh]
  (0x00915C4C, 0x00915D1E, 0x00915F25, 0x00916235, 0x009162AA).
- Seed experiments (traces, free): w3a (Perform result unnamed) crate ebx -- worse; w3b (named `const CCharString* name`) = w2;
  w6 (w2 + LookAt failure nested `if (result == 2) {...}`) = w2. Block-address data (blk.py): in w2 the succ/J blocks are in a
  later-allocated block chunk than the idx block, the split seeded {succ} before idx (idx dropped); in w1t all three in one chunk.
- w8 = w2 + the P-fail free moved INSIDE the pre-route if (`if (result == 2) {...return true;} action_points->SetActionPointAsFree(index);`)
  and the pre-route else as `else if (goods.Get()) action_points->SetActionPointAsFree(index);` (both fall to the final return).
  Trace: vector web split eax #9 {J} + ebx #1 {idx, succ} = retail. g09 = DIFFER(1931v1931) 585i: md.sh --frame shows NO
  instruction difference; the residue is frame SLOT ORDER only (goods [esp+1Ch] v retail [14h], key/temp [18h] v [20h], etc.).
  -> NEW BEST, differ/00915b60.cpp = probe/w8.cpp. (w9 = w8 with the LookAt failure nested: same allocation.)
- Frame (free): pk.sh probe/w8.cpp w8 + tools/fs/qs.py (replays c2's slot sort 0x107AE0C4 = K&R qsort, pivot middle swapped left,
  comparator 0x107AE9C2 density w*1000/size, ties -> 0; reproduces our order exactly). w8: goods (8 bytes, w22), keys+offset.Y slot
  (w11) and carrying slot (w11) all density 2750 -> tie order s6 s7 goods; goods w23 (one more IL reference, counted before
  cross-jumping) gives retail's goods, s6, s7.
- x1 = w8 with the pre-route else as `else if (!goods.Get()) { return false; } else { action_points->SetActionPointAsFree(index); }`
  (the explicit return copy adds one goods destructor reference -> goods w23; the copy cross-jumps away). Trace: same allocation
  as w8; pk: every slot at retail's offset. g10 = RELOCATION_MATCH.
- Final: src/00915b60.cpp = probe/x1.cpp with the header comment updated (g11 via gall.sh: RELOCATION_MATCH); test/00915b60.cpp =
  the old differ test with the tail restructured the same way (PASS g++ + VC7.1). vland (read-only): RELOCATION_MATCH, behaviour PASS,
  one WARN RELOC_NAME (manifest label of 0x007FEF40, see Label corrections). lv/ deleted. differ/ emptied (old best kept as
  probe/differ_best_w8.cpp, old test as probe/differ_old.test.cpp).
- Tools added: vt.sh (trace -> allocation lines), ch.py (choices with block lines), blk.py (pieces touching a source line's block),
  tools/fs/qs.py (replay of c2's frame-slot sort from a pk.json presort event; `--w slot=weight` what-ifs).
