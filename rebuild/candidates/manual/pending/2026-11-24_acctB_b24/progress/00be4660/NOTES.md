# 0x00BE4660 CWaterPatchTesselatorBase<CTVertexWaterBackground>::TesselateEdge -- in progress (B24C2), best DIFFER(2349v2349) 4 hunks (differ/ = z2.cpp; see 'Exact state at hand-back' at the end)
## Lessons that apply
- Row 1 (sea twin 0x00BE4FE0) MATCHED with: FableWin-exact FindWaterVert (early return 0 + while loops), continue-shaped bridged
  span with one shared skip `previous = -1`, `start` read through strip->GetWaterVertex(i), not-found `previous = -1` before the
  index store; AddFanBaseVertex with a named `CVertex& vert` is FableWin-exact (60i/182b) and still matches. ONE template text.
- GOTCHAS 335 / 466: same instructions with swapped frame slots = block-frequency weights / symbol-number ties (c2 kit
  pending/2026-11-20_c2_symbol_order). GOTCHAS 382 (static-assert typedef names): probed, NOT the cause here.
## Earlier state (superseded by z2; was differ/ = r2f.cpp: row-1 template text + water AddDuplicateEdgeVertex / AddFanBaseVertex with memcpy shore copy)
Every helper expansion and the loop body are retail's. Residue:
- frame slots transposed in three pairs (ours v retail, F = esp after `sub esp,11Ch`): FindWaterVert pos F+0x10 / Iterate `vertical`
  byte F+0x17 (retail byte F+0x13, pos F+0x14); fan+x-copy F+0x18 / previous F+0x1c (retail previous F+0x18); i F+0x30 / &water
  F+0x34 (retail &water F+0x30, i F+0x34). /FAs map: fas.py (fas/r2f.asm).
- prologue: retail loads y straight into ebp (the FindFirst y copy) and pushes ebp; ours loads edx, pushes, `mov ebp,edx`.
- loop head: retail enters with eax = i (xor) and ecx = strip from both edges; ours reloads both (strip in edx at the tail).
## Tried (no effect unless noted)
loop-scope found/fan; previous declared first; i declared before loop; both instantiations in one TU (either order); FindFirst texts
(pos local, count local, call in the while condition: all identical code); Iterate texts (index+1 < count: worse 64 hunks; water
local: same); for-loop shore copy (no rep movsd, 2577); helper local-count perturbations (vert inline: worse; water_fan inline:
53 hunks; extra pointer local: same); static-assert typedefs removed (probe: same); start via water ref (r2e/r2i: 2349 bytes and
retail's loop entry, but the LandscapeVertexIndex CSE comes back, 47 hunks).
## FableWin sizes (fwsize): water AddDuplicateEdgeVertex / AddFanBaseVertex are both +113i/+408b over ours (the sea twins are exact) --
a later revision of the shore copy; FindFirstWaterAdjacentVertex +4i/+10b and IterateThroughAdjacentVerts +2i/+3b still open (both rows).
## (superseded) first next idea
Run the c2 symbol-order kit (pending/2026-11-20_c2_symbol_order, tools_il lpool.py / sym.sh) on r2f and on the matched sea source to see
which ranking key orders the frame slots, then find the helper-text fact (symbol count before the pairs) that flips all three pairs.
## B24C2 later grades: the frame packer is the instrument (MATCHING_PLAYBOOK "Frame packer")
- Tool: rebuild/candidates/manual/pending/2026-11-08_linux_hand885/tools/pk/pk.py <src> <out.json> --func TesselateEdge (c2 probe, ~1 min;
  C2PRIO_RUNS=$W/pk/runs) + $W/pkdec.py (slots, weights, pre/post sort) + sim.py (density qsort model; reproduces our order exactly).
  Model from r2f weights: retail order needs i -1 / &water +1 AND previous +1 (or fan/x-copy slot -1).
- z2 = r2f + else-chain skip stores, NO continue (`} else previous = -1; } else previous = -1;`): DIFFER(2349v2349) 4 hunks -- frame
  order = retail, loop entry = retail; left: prologue y copy (ours `mov edx,[y]; push edx; mov ebp,edx`, retail `mov ebp,[y]; push ebp`)
  and the first loop-head alignment that follows from it.
- The SAME text on the sea twin: s_z2 22 hunks (strip enregistered in ebx at the loop head, no ebp = -1 hoist). Sea matches with the
  continue-shape (m1a), with else-chain + continue after the span (s_z4) and with else-chain + continue in the found arm (s_z6);
  water with those: z4 16 hunks (i/&water slots + loop entry), z6 104, z7 16. Conflict: water wants no `continue`, sea wants one.
- No effect on water: release ASSERT dead arms in the water helpers (z4 base), start via water ref on z4 (z5 17).
- Packer probe (pkq.sh, always source the lane env first): z2 and z4 give the same POST order but z2 creates the &water temp (w3)
  before i (w3) while z4 creates i (w4) first: with a `continue` inside the bridged branch i gets one more IL reference (cause unverified; zc has two
  continues in the skip arms and still gives 4 hunks, so it is a continue INSIDE the bridged branch that water rejects), and creation
  order (size asc, weight desc) then decides. Named `next` reference (z8): no change (i still w4).
- Sea twin matches with a `continue` anywhere in the bridged branch (m1a, s_z4 after the span, s_z6 found arm, s_z7 not-found arm) and
  fails without one (s_z2, s_z9 loop-scope found/fan: 22 hunks, strip kept in ebx, no ebp = -1 hoist). Water: z2 / z9 (no continue)
  4 hunks; z4 / z7 16 hunks (i v &water slots + loop entry with i in eax); z6 104.
- (superseded) next idea: keep a sea-compatible text (continue in the not-found arm, z7, is the most natural: `previous = -1; indices = ...;
  continue;`) and find what lowers i's IL weight by one in water only -- the water helper text is the only instantiation-specific
  input (FableWin's water AddDuplicateEdgeVertex / AddFanBaseVertex are +113i/+408b over ours, sea's are exact). Probe each idea with
  pkq.sh (POST order, slot 11/12 creation order) before grading; then the prologue y-copy register (priority tools: c2probe/lk README,
  hand885 pk/wi.py). Row 1 must be regraded with whatever shared text is final (its payload uses the m1a continue-shape).
- Also tried: early-continue skip chain (zc: `if (EdgeIndexBufferIndex == 0xffff) { previous = -1; continue; }` + combined skip test with
  continue; found / not-found as if/else): water 4 hunks (= z2), sea 45. Sea z2 with the not-found index store first (s_z2b): 22.
  Water helper without the `water` local in AddDuplicateEdgeVertex (zw1, probe only): weights scrambled, not retail.
## Exact state at hand-back (B24C2, context spent)
best water source = differ/00be4660.cpp (= z2.cpp, 4 hunks: prologue `mov ebp,[esp+138h] ; push ebp` v ours `mov edx,.. ; push edx ;
mov ebp,edx`, then the FindFirst loop-head padding that follows). Open problem = one TesselateEdge text for both instantiations:
of the texts tried, water is closest (4 hunks, still DIFFER) only without a `continue` inside the bridged-span branch (z2, z9, zc);
sea matches only with one (m1a, s_z4, s_z6, s_z7). The mechanisms differ: sea under the else-chain (g_sz2.txt in ../00be4fe0) has retail's
frame slots and only register differences (strip held in ebx, start in ebp, no ebp = -1 hoist) -> a register-PRIORITY question (tools:
c2probe/lk README, hand885 pk/wi.py; sea was never probed); water under a bridged-branch continue (z4/z7) has the i / &water slot swap and
the loop-entry difference -> a frame-packer question (pkq.sh).
## Next ideas (in order)
1. Probe sea s_z2 with the priority tooling: why strip outranks the -1 constant without the continue; find the sea-side fact.
2. Row 3 evidence: hand873 calls the `<= 1` else-first layout a big win on the landscape sibling 0x00BE3970; compare its skip tests in
   ../00be3970/dis_all.txt with the water rows' and try that loop-tail skeleton on BOTH instantiations.
3. Then the prologue y-copy register (ours `mov edx,[y]; push edx; mov ebp,edx`, retail `mov ebp,[y]; push ebp`), regrading sea after
   any FindFirst change.
Tools in this folder: fas.py / fmap.sh (frame map from /FAs), pkq.sh + pkdec.py (c2 frame-packer probe, POST order and weights),
hvar.py (header-overlay variants), var.py-style text variants. No payload for this row.
## B24C3 (2026-11-24) -- tools in c3/: pair.sh (one TesselateEdge body -> water + sea sources, grades BOTH; DEFS / OV env for row
## defines / overlay), prall.sh / prq.sh (c2prio colouring trace, ~20 s), fwv.sh + mkov.py (FableWin /Od sizes of FindFirst / Iterate
## under an overlay with a replaced helper body), hv/ (water helper variants), ff/ (FindFirst variants), dbg/ (debug-ASSERT size probe).
- Baseline: water z2 4 hunks, sea z2 22; water m1a 43, sea m1a RELOCATION_MATCH (pair.sh reproduces B24C2's numbers).
- c2prio on water z2: the prologue residue is a colouring ORDER: the FindFirst y/x ARGUMENT pieces (prio 52, chosen first, "all costs 0"
  -> edx / esi) are coloured before the inline y/x COPY pieces (prio 48; the y copy piece is allowed only ebp). Retail needs the copy
  pieces first so the argument pieces take their register by copy preference (y -> ebp; x -> esi). 4-point gap, not a tie.
- Loop form: the bridged-branch `continue` moves BOTH rows the same way (continue -> reload-at-head loop: water i piece goes to memory,
  sea strip piece ecx + const -1 piece ebp = retail sea; no continue -> water i piece eax = retail water, sea strip piece ebx, -1 immediate).
  IL blocks of sea m1a v z2 differ only in the skip blocks (one shared `previous = -1` block v two else-arm blocks). So no loop-tail
  spelling alone separates the rows; the separating input must be instantiation-specific or shift both rows' balance.
- No effect / worse (graded): prologue declaration orders p1..p10 (z2 water: 4..38 hunks, none better); FindFirst texts f1..f7 under
  z2 water (f1 4, f4 6, others worse); water helper texts h1..h6 under z7 (16..65, none better); CEdgeStipVertexArray::operator[]
  release-ASSERT dead arm (FableWin operator[] is 73i/245b = ours + one assert; dead arm inert on both rows: 4 / 22 / 43 / RELOC_MATCH);
  not-found falling into the shared skip store with `continue` in the found arm (e/f: water 104, sea 31).
- FableWin /Od sizes: FindFirst f1 (pos + count locals) 89i/252b, f4 (PeekSize() x3, no size local) 88i/250b v FableWin 89i/250b.
  FableWin water AddDuplicateEdgeVertex / AddFanBaseVertex / AddCompositeVertex all = sea + 118i/422b = ours(memcpy) + 113i/408b, which
  is two debug ASSERTs (probe: 2 asserts = +115i/+412b); TesselateEdge +131i/+516b = two asserts + ~16i.
- (B24C3) 0x00BE3970 else-first skeleton: k1 (BOTH tests else-first: `if (water.EdgeIndexBufferIndex == 0xffff) previous = -1; else {
  ... if (end - start <= 1 || next != +1) previous = -1; else {...} }`, NO continue anywhere): SEA RELOCATION_MATCH, water 16 hunks
  (= z7: i / &water slots + loop entry + prologue). k2 (span test only else-first): water 4, sea 22 (= z2). So the toggle is the
  first test's arm order / skip-block position, not `continue` itself; it still moves both rows together.
- (B24C3) screened 42 prologue-order x FindFirst-text combos with c2 traces (c3/sw, scr.sh): the y argument piece always edx.
- (B24C3) Loop-skeleton sweep (c3/gen.py, gx/; pair grades, sequential -- two pair.sh in parallel corrupt each other's fg runs):
  first test S1 x span test S2 x not-found order F (18 combos): S1 decides everything; S2 and F are inert (F2 = index store first
  costs sea 4 hunks). Every form graded so far is either W (water 4 hunks = prologue only, sea 22/45) or S (sea RELOCATION_MATCH or 4,
  water 16/43/104): W = z2/gaa*/gab*/gac* (S1 then-first + else store), gc* / zc / h1 (S1 early `{ previous = -1; continue; }`),
  i1 (else arm with continue), k1-new / da / db (continue at the END of the 0xffff-then arm, shared or else store);
  S = gb* (S1 else-first `if (== 0xffff) previous = -1; else {...}`, no continue), m1a, z4 / z6 / z7 (continue INSIDE the bridged
  span branch). No natural loop-tail form splits the rows (about 30 forms).
- (B24C3) Mechanism readouts (c2prio raw JSON + c3/rounds.py): water prologue race = y argument piece 4*K(prologue)=52 v y copy piece
  2*K + loop terms (-2 +4 +20) = 48; retail needs the copy first (then the argument piece takes ebp by copy preference). Water loop
  form: under S forms the main-loop i piece (#38, spill -28/-30) is not split into the latch->head eax piece that z2 gets.
- (B24C3) More closed leads: plain `inline` helpers + /d2inlT200 / 1000 (heuristic inliner does not expand them: 1415 / 1829 bytes);
  two release-ASSERT dead arms per water helper (FableWin +2 asserts) on gba1 / gaa1: inert (16 / 4); sea AddFanBaseVertex local
  orders under z2: 22 / 36 (no flip); both instantiations in one TU (water first) with gba1 / gaa1: unchanged (16 / 4, sea gba1 MATCH);
  PeekWaterVertexCount IS in FableWin (fablewin_pdb_names: 0x02CC1BC0, 22 bytes = ours; the signatures file just lacks it).
- (B24C3) Prologue y register, mechanism confirmed: moving `map` AND `adjacent_index` above the `length` ternary drops K(prologue) to 11
  and the y argument piece then takes ebp by copy preference (c2 screen sw/, 8 orders) -- but those orders move the map /
  adjacent_index work in front of the ternary (7 hunks, not retail's schedule). So retail's prologue block has ~2 fewer referenced
  register candidates than ours with the SAME instructions; the statement giving that is not found (orders with `length` first,
  FindFirst texts, p-variants all leave y in edx).
## Exact state at hand-back (B24C3)
- Water best = differ/00be4660.cpp (= z2, W-form, 4 hunks, prologue y register only). Sea matches under ANY S-form: m1a (payload),
  s_z4 / s_z6 / s_z7, and now gba1 / k1 (first test else-first `if (water.EdgeIndexBufferIndex == 0xffff) previous = -1; else {...}`,
  no continue anywhere: c3/gx/b_gba1.cpp). Water under every S-form: 16 hunks (gba1 / z7 / k1) or worse.
- Open: the instantiation-specific input that puts sea on S and water on W under one text. Not the loop tail (30 forms), not the
  water helper text (h0..h6, ha), not dead arms, not TU composition, not prologue order with length first.
## Next ideas (B24C3)
1. Instrument the split pass (0x1078b2aa) in c2prio for the main-loop i piece (#38): find why W-forms cut a latch->head piece and
   S-forms do not (which block / ref rule), then look for the smallest statement-level change that makes S-form water cut it.
2. The prologue: find which of the 13 prologue-block candidates (rounds.py on raw JSON; ids 116 const 0, 21, 31, 36, 26, 28, 40, 54,
   32, 38, 109, 121, 154) retail lacks; candidates: the `side` argument piece, the map piece, the LandscapeMap temp (#40).
3. If (1)/(2) fail: ESCALATE that the two retail instantiations look like different loop-tail forms (sea = S, water = W), and ask
   whether the template text may differ by a FableWin-unsupported specialisation (FableWin says one text: equal strict signatures).
- (B24C3, corrections) Sweep cells that errored in the parallel run, regraded sequentially: w_gaa2 4, w_gca1 4, w_gcc1 4, s_gaa2 22,
  s_gcb2 45 -- the pattern holds. Name clash: c3/w_k1.cpp / s_k1.cpp were overwritten by the later hybrid `k1` (continue at the end of
  the 0xffff-then arm, W-form); the prio_ / raw_ / blk_ "k1" files and NOTES "k1 (BOTH tests else-first)" are the else-first S-form
  (c3/b_k1.cpp = gbb1).
- (B24C3, corrections) Prologue candidate labels from the trace (ids are recycled; the raw-JSON id lookup is unreliable): 26 = the `+2`
  temp piece (length value, edi), 32 = `+6` local (edi), 109 = `+2-3` temp (eax), 40 = `+6` two-ref local (ecx). Two readings fit the
  4-point gap: retail's prologue block has ~2 fewer referenced candidates (K 13 -> 11), OR the copy pieces gain ~5 in the FindFirst
  loop (bottom GetEdgeStrip block K 5 -> 7). Test both in next idea 2.
## B24C4 (2026-11-24): is it ONE template text? Evidence (b24c4/: w.dis / s.dis = retail listings, diff in this note)
- PDB files are NOT on this machine (no Ego_r / Ego_d / FableWin .pdb anywhere; debug_build/ absent), so egod_lines.py / egod_proc.py
  (C13 line blocks) cannot run here: same-line-range proof needs the owner's Windows box (D:\..\debug_build\Ego_d.pdb). What the
  derived tables say:
  - Ego_r.pdb module streams (ghidra_out/egor_obj_functions.tsv): both in Release\Engine_landscape_tesselator.obj, consecutive procs
    55 (Water, 0x009C4710, 2452 bytes) and 56 (Sea, 0x009C50B0, 2327 bytes): same compiland, template instantiations side by side.
  - Ego_d.pdb carved procs (egod_carved_functions.tsv): both in FableEngine\Debug\Engine_landscape_tesselator.obj, EQUAL size 1307.
  - FableWin.pdb (fablewin_pdb_names.tsv span to next public): TesselateEdge Water 0x02CC14F0 and Sea 0x02CC3440 both 1648; the
    per-T helpers differ (AddFanBaseVertex Water span 768, Sea 240 -- vertex copy vs int->float conversion; spans may include unnamed code).
  - Retail TLC listings: identical callee multiset (GetEdgeStrip 0x00BDC980 x4, _M_insert_overflow 0x005135D0 x10, the per-T
    CArray<T>::push_back x4: 0x00BE4F90 water / 0x00BE5950 sea), identical non-type immediates. Every difference is type-dependent:
    vertex stride (imul 0x38 / divide magic 0x92492493 sar 5 for 56-byte water vertices v lea*3*4 / 0x2AAAAAAB sar 1 for 12-byte sea
    vertices), vertex fill (water: u16 X / u16 Y / dword Z + `rep movsd` 12 dwords ShoreLookUpArray; sea: fild/fstp ushort->float X/Y
    (+Z)), frame (0x11C water: four 0x38-byte push_back temporaries at esp+0x4C/0x84/0xBC/0xF4; 0x68 sea: four 12-byte ones), and
    register allocation / loop-head alignment (sea pads `lea ecx,[ecx]` before the bridged-span loop at 0x00BE51E0, water does not).
    The branch structure of the stretch 0x00BE47EE..0x00BE48D6 v 0x00BE517E..0x00BE5266 is the same block for block (side-by-side);
    the loop head and the not-found tail differ in LAYOUT (that is the W / S difference), not checked block for block.
  - No explicit specialisation of TesselateEdge is indicated (no extra / missing statement, no different constant, same callees).
- Conclusion: ONE TesselateEdge template text (premise holds). The separating input is type-dependent: the inlined per-T vertex
  helpers (AddDuplicateEdgeVertex / AddFanBaseVertex: struct copy + rep movsd v float conversion) and the vertex size, which change
  register pressure / frame and, through them, our compiler's loop-tail shape. Since one text cannot be W for water and S for sea in
  our compiles, the remaining candidates are (i) the helper TEXT (water helper forms h0..h6 / ha tried by B24C3 under S; the sea
  helper under W was tried for local order only), (ii) the vertex class (copy assignment / constructor of CTVertexWaterBackground:
  a user-declared copy or a member-wise copy changes the push_back temporaries and the fill), (iii) B24C3's split-pass idea.
- (B24C4) Probe: Ego_r.pdb vertex hierarchy on both types (CTVertexWaterBackground : CGenericVertex<CTVertexWaterBackgroundBase>,
  probe overlay b24c4/ovh + the repo's FABLE_RETAIL_CTVERTEXSEABACKGROUND_HIERARCHY): INERT. z2 (W) water 4 / sea 22; gba1 (S) water
  16 / sea RELOCATION_MATCH -- identical to the flat classes.
## Exact state at hand-back (B24C4)
- Unchanged best: water differ/00be4660.cpp (z2 W-form, 4 hunks, prologue y register); sea matched under S-forms (held: needs the same
  text as water). Premise "one template text" now evidenced (see the B24C4 evidence block above); the line-range proof needs the
  PDBs, which are not on this machine.
- Next: B24C3's split-pass instrumentation (idea 1) and prologue candidates (idea 2); the per-T helper texts are the only
  type-dependent source left -- try sea helper variants under the W-form (only local orders were tried) before more loop-tail forms.
- (B24C4, advisor lead, not yet checked) Sea keeps -1 in ebp (`or ebp,0xffffffff`, `mov [esp+1Ch],ebp` 0x00BE51D3) and seems to have
  TWO `previous = -1` stores at the tail (an immediate before the index store and `mov [esp+14h],ebp` near the skip target 0x00BE5925);
  water stores immediates (0x00BE4843) and seems to have ONE, possibly shared with its skip target 0x00BE4F5C. Disassemble from
  0x00BE4F49 and 0x00BE5904 to confirm. If so, the S order is the source order and water's W shape is VC7.1 tail-merging two identical
  immediate stores: z2 would be a hand-merged text, and the question becomes whether our S-form water (w_gba1) keeps -1 in a register
  where retail does not (register pressure in the water instantiation tying the 16 hunks and the prologue y register to one cause).
- (B24C4) Strongest one-text evidence without the PDBs: ghidra_out/naming_stragglers/FableWin.exe.signatures.tsv gives both FableWin
  instantiations (0x02CC14F0 water, 0x02CC3440 sea) 1307 bytes, 387 instructions and EQUAL strict (075ae730d3131c87) and loose
  (00df6ec15f10d4b5) signatures.
## B24C5 (2026-11-24) -- hand read of the tails: the S-form IS the template text; water's single store is a COMPILER merge
- Retail sea tail (s.dis): not-found arm 0x00BE5904..0x00BE5923 = `mov [esp+14h],-1` (immediate, scheduled BEFORE the index store)
  + index store + `jmp`; the skip target 0x00BE5925 = `mov [esp+14h],ebp` (ebp = hoisted -1). TWO previous = -1 stores (they differ
  as instructions: immediate v register, so they cannot be cross-jumped).
- Retail water tail (w.dis): not-found arm 0x00BE4F45/0x00BE4F49 = index store then FALLS INTO the shared `mov [esp+28h],-1` at
  0x00BE4F5C, which is also the skip target of the span tests (jle/jne 0x00BE4F5C). ONE store; water never hoists -1 into a register
  (fan = -1 at 0x00BE4843 is an immediate too), so both stores are the same instruction and VC7.1 tail-merges them.
- Our S-form water compile (c3/w_gba1.cpp: source has `previous = -1;` BEFORE the index store) emits exactly retail's merged tail
  (c5/w_gba1.dis 0x905..0x924). So z2 (W-form) is a hand-merged text; the S-form (which matches sea) is the template candidate.
- Remaining water residue under S (c5/dd.py b24c4/w.dis c5/w_gba1.dis -> c5/d_gba1.txt, 2381 v 2349): (a) prologue y argument in
  edx (= z2's only residue); (b) back edge: retail carries i in eax and strip in ecx into the loop head (preheader `xor eax,eax;
  mov [esp+44h],eax`), ours reloads both at the head; i/&water slot swap and alignment pads follow. No type-dependent expansion differs.
- (B24C5) Tools: c5/pair5.sh <name> <body> (pair grade + both objects disassembled + c5/dd.py instruction diffs <name>_w.dd /
  <name>_s.dd; SEDW / SEDS sed the prefixes, OV overlay); c5/pre_w.cpp / pre_s.cpp = the current prefixes (z2 1..67, fanvert 1..65).
- (B24C5) Every S-form gives the SAME water residue (gba1 / gbb1 / gbc1: 16 dd blocks = prologue y edx + the back-edge facts);
  W v S differ in source ONLY in the first test's arm order (`!= 0xffff) {body} else previous = -1` v `== 0xffff) previous = -1;
  else {body}`), i.e. IL block order / static branch weights, not statements.
- (B24C5) Graded, no effect or worse: push_back(const T&) body visible (explicit specialisation decl dropped): w 148 / s 173 hunks,
  frame 0x44 (temporaries merged) -- retail's decl-only form stands; probe noinline+visible pair: 1968 / 1862 bytes (wrong);
  S-form + `long i;` before the loop (v1: 16 / MATCH), previous declared first (v2: 17 / 18), `long i = 0;` + `for (;` (v3: 17 / 3).
- (B24C5) Back-edge reading: retail water latch `mov eax,[i]; mov ecx,[strip]; mov edx,[ecx+1Ch]; inc; dec; cmp; mov [i],eax; jl head`
  and the preheader `xor eax,eax; mov [i],eax` feed ONE head piece (eax = i, ecx = strip). Ours (S): latch strip in edx, count in ecx,
  preheader `mov [i],0`, head reloads both. Sea retail: back-edge block reloads ecx = strip and ebp = -1, head reloads i from memory.
  So water retail gives the head piece of i a register that sea retail spends on the hoisted -1.
- (B24C5) c2prio traces of S-form water / sea (c3/prio_w_gba1.txt, prio_s_gba1.txt): the clearest type-dependent difference is the
  water helpers' `v` candidates at the top of the colouring order (prio 1152 / 1080 / 1008, 9 refs each; sea v 480 / 448 / 416, 4 refs).
- (B24C5) Water helpers with no named `v` (BridgeVertexBuffer[new_index].X = ... each use, c5/pre_w_hb.cpp): S 130 / W 124 hunks
  (worse; the named `CTVertexWaterBackground& v` stands, as in retail's schedule: v computed right after push_back).
## Exact state at hand-back (B24C5)
- Water best unchanged: differ/00be4660.cpp (z2 W-form, 4 hunks). But z2 is now known to be a hand-merged text: the template is an
  S-form (sea RELOCATION_MATCH; S-form water = 16 hunks: prologue y register + back-edge head piece of i/strip, tail = retail's).
  When the water row is solved it must be under an S-form, and the sea payload regenerated with that same text.
- Next: (1) find why our water allocator gives the head piece of i no register under the S block order (c2prio: identify the main-loop
  i / strip pieces in c3/prio_w_gba1.txt v prio_s_gba1.txt, which candidate takes the register retail gives i); (2) the prologue y
  register (B24C3 idea 2); (3) the c2 symbol-order kit (pending/2026-11-20_c2_symbol_order) for TU-level id effects: retail compiles
  CLandscapePatchTesselator::TesselateEdge (0x00BE3970) and ~50 vector / CArray instantiations before both TesselateEdge bodies.
- (B24C5, late) CEdgeStipVertexArray::operator[] dead-arm assert (c3/ov_ea, the device that moved 0x00BE3970 from 35 to 25 blocks)
  on the S-form pair gba1: water dd identical (16), sea RELOCATION_MATCH -- inert here, compatible as a shared opt-in for all three rows.
## B24C6 (2026-11-24) -- compile-order test (coordinator / owner request): NO EFFECT
- One TU holding the 0x00BE3970 body (00be3970/c6/s1.cpp, real opt-ins), the water and sea specialisations and the S-form template body
  (c3/gx/b_gba1.cpp), compiled in three orders: landscape, water, sea (retail address order; o_lws) / landscape, sea, water (o_lsw) /
  water, sea, landscape (o_wsl). Object symbol order confirms the bodies were emitted in the requested order (o_wsl: water, sea,
  landscape). Files: 00be3970/c6/ord/o_*.cpp, grader 00be3970/c6/ord/ord.sh (fg per address + dd), outputs o_*_<addr>.dis/.dd.
- Result: in all three orders, water = DIFFER(2381v2349) 16 dd blocks, sea = RELOCATION_MATCH, 0x00BE3970 = DIFFER(2392v2390) 25 blocks;
  the water and 0x00BE3970 disassemblies are BYTE-IDENTICAL to the one-row compiles (u2_w.dis, 00be3970/c6/s1.dis). So the water
  residue (prologue y edx; back-edge i/strip head piece) is NOT a compile-order effect of these bodies within one TU. (Not tested:
  the rest of retail's object -- TesselateEdgeGeneric, constructors, DummyFuncInstantiateTemplates -- which a grouped build would add.)
- Unified overlay: 00be3970/hdr (repo-current headers + ADDPOLY + FINDWATERVERT_EARLYOUT + GETEDGESTRIP + water helpers header +
  FINDVERT (FableWin while loop, FABLE_FORCEINLINE) + CEDGESTIPVERTEXARRAY_INDEX_ASSERT (Inferred)) is inert on both twins:
  pair u1 (without the arm) and u2 (with FABLE_RETAIL_CEDGESTIPVERTEXARRAY_INDEX_ASSERT) = water 16 / sea RELOCATION_MATCH.

## B24C7 (2026-11-24): not reached (context spent on 0x0070D940). State unchanged from B24C6.
## B24C8 (2026-11-24): not reached (context spent on 0x0070D940 and 0x00BE3970). State unchanged.

## B24C9 (2026-11-24): not reached (context spent on 0x0070D940 and a short 0x00BE3970 pass). State unchanged.
## B24C10 (2026-11-24) -- short pass (context spent on 0x0070D940's escalation and 0x00BE3970)
- Baseline regrade of the S-form pair (c3/pair.sh c10base gx/b_gba1.cpp): water DIFFER(2381v2349) 16 hunks, sea RELOCATION_MATCH
  (unchanged). Reading of c5/gba1_w.dd: the four inserted `jmp X` before loop heads are alignment consequences (ours jumps over the
  pad where retail executes a 7-byte `lea esp,[esp]`, our code being shifted by the prologue's extra `mov ebp,edx`); the real residue
  stays (a) the y argument piece in edx + `mov ebp,edx` (retail loads y straight into ebp, 0x00BE4689) and (b) the main-loop i start
  (retail `xor eax,eax` .. `mov [esp+44h],eax`; ours an immediate store to [esp+40h] + entry jump) with the i / other slot pair
  [esp+40h]/[esp+44h] swapped.
- fwsize (c10fw, /EHsc): GetWaterVertex / GetWaterFanBaseVert / GetMainAxisVertexWorldPos / AddPoly SAME; IterateThroughAdjacentVerts
  107i/304b v FableWin 109i/307b; FindFirstWaterAdjacentVertex 85i/240b v 89i/250b (both still open, as B24C3 recorded).
- Callees: retail calls CArray<CTVertexWaterBackground>::push_back(const T&) 0x00BE4F90 x4 (the COMDAT right after the row, rep movsd
  of 14 dwords), vector<unsigned short>::_M_insert_overflow 0x005135D0 x10 (our copy EQ, pickany_vs_retail), GetEdgeStrip x4.
- No new grade beyond the baseline. Next (unchanged from B24C5): the y-piece colouring order (c2prio on S-form water v sea).
