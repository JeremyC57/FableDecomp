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
