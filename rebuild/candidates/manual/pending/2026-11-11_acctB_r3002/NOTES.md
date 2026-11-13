# Hand chunk 3002 (CTCLightningOrb .. NUISystem::CTable): 1 of 2 byte-exact

Retry pass on two rows parked by chunks 2022 / 2006 with every instruction right and only the frame layout wrong. Row 0x007F26B0
grades RELOCATION_MATCH under VC7.1 RTM from src/007f26b0.cpp (gall.sh, `FABLE_HDR_OVERLAY=hdr`), its test prints
`007f26b0_TEST PASS` under g++ (tall.sh) and VC7.1 (tall.py), and the read-only vland reports RELOCATION_MATCH PASS (warnings
RELOC_NAME only: the CTCLightningNode setters and ProcessPoints already filed in NAME_FIXES.tsv, DELTA_NOTIONAL_ZERO labelled
`EPSILON`). Row 0x00550DC0 is parked (frame slots only). Both extents were confirmed by the prior chunks and again here
(0x007F26B0 `sub esp,8Ch` .. `ret 8` 0x007F2DB7; 0x00550DC0 .. `ret 14h` 0x00551334); no SPLITS.tsv.

## What matched (1)
- **0x007F26B0 CTCLightningOrb::Initialise(CThing&, CThing&)** (1802 bytes, RELOCATION_MATCH, pass 1 grade 2). Method: mapped every
  frame word of the prior build onto retail's (tools/my/fmap.py, instruction-aligned, call stack-arg counts from the callees' `ret N`),
  named every packer node from the c2 frame-packer trace (phook; temporaries identified by their c1 offset order and code sites), then
  replayed c2's final packer (tools/my/sim.py: nodes by size asc / weight desc, each joins the LATEST slot with size <= 2*slot and no
  conflict) and found the conflict changes that give retail's exact membership and 0x8C frame. Three source facts give exactly those:
  1. `effect` must not conflict with the GetTC key: the same-TU noinline GetEffectFromString body (existing repo opt-in
     FABLE_RETAIL_CSPECIALEFFECTSDEF_GETEFFECT_BODY). The prior agent rejected it because it turned on dead-parameter-home reuse; measured
     here: c2 packs once WITH the two parameter homes as the first slots and, when the packed frame is >= 0x70 bytes, repacks without them
     (c2.dll 0x10790D90 -> 0x10734A51 `cmp edi,70h`). With the other two facts the frame is 0x8C, so the homes stay unused, as retail.
  2. The two Fireheart control points are named block-scoped vectors (`C3DVector fireheart_approach = target_pos - flat_direction * 7.0f;
     PSpline->AddControlPoint(fireheart_approach); C3DVector fireheart_rise = ...;`): their addresses go to AddControlPoint, so each is live
     to the end of the block; they then conflict with the second point's temporaries, open retail's temporary slots [esp+60h] / [esp+3Ch]
     and push the second `flat_direction * 7.0f` product into its own slot [esp+90h] (temporaries passed directly die at the call and let
     three 12-byte temporaries fall into the 8-byte group).
  3. `C3DVector difference = target_pos - start; float distance = difference.GetMagnitude(); difference /= distance;
     C3DVector direction = difference;` -- normalised in place, then copied: the copy is forwarded, one node holds difference / direction /
     velocity / two later temporaries ([esp+18h]; difference Z at [esp+20h]), and the x87 code keeps retail's X, Z, Y squares and stored
     reciprocal. Measured alternatives: dividing a copy (`direction = difference; direction /= distance`, prior best) makes difference and
     direction conflict (difference falls into the 8-byte group, frame 0x90); a single in-place vector (`C3DVector direction = target_pos -
     start; ... direction *= reciprocal`) gives the exact 0x8C frame but squares X, Y, Z (4 x87 instructions); `difference * (1/d)`, the
     3-argument constructor, `/` and default-construct + assign all keep the conflict or change the code.
  Grades: g1 baseline (DIFFER 1793v1802, as recorded), g2 the probe (RELOCATION_MATCH), g3 / g4 src/ via gall.sh (RELOCATION_MATCH).

## Parked for Fable 5.1 (1)
- 00550dc0 NUISystem::CTable::Draw DIFFER(1399v1399): retail 0x00550EFE `fstp [esp+74h]` (parent R) / 0x00550F53 `fstp [esp+3Ch]` (table
  R), ours 3Ch / 74h (reads 0x00550F77 / 0x00550F7B follow); retail 0x00550F59 `fstp [esp+38h]` (alpha product) / 0x00550F63 `fstp [esp+34h]`
  (blue product), ours 34h / 38h (reads 0x00550F83 / 0x00550F92, 0x00551080/84 follow) | tried: ~35 free probes with frame-packer traces
  (named / temporary parent and table colours, `const CRGBColour&` parent, implicit conversion, explicit cast, member-call order, operands
  swapped inside operator* (overlay probe), parentheses, `const`, direct-init, declaration-first + Initialise, assign, in-place `*=`,
  RenderColour intermediate) | measured: the 16-byte parent (P) and table (T) temporaries each have packer weight 2 (one `fstp`, one x87
  operand); new nodes are appended to their size group and on each increment move only past STRICTLY lower weights (list insert c2.dll
  0x10788642, increment path 0x10789A99), so equal weights stay in the order they REACHED that weight; the weight walk (0x10785C60: blocks,
  then tuples, records at tuple+0x28) sees the references in final scheduled order: P def 0x00550EFE, T def 0x00550F53, T use `fld [T.R]`
  0x00550F77, P use `fmul [P.R]` 0x00550F7B -> list T, P -> T joins colour's slot (created just before), P conflicts with T and joins
  child_colour's. Retail needs P first (P one extra reference, or P's use before T's) with byte-identical code, so retail's IL must carry a
  reference that emits no code. The 4-byte swap is the same kind of tie: the alpha / blue product temporaries (weight 3 each, one slot each,
  density tie 750) come out of the final qsort in the other order. | next: find the no-code reference (a named object's scope/kill
  reference, a forwarded copy) that raises P to weight 3 or moves its use first; read c2prio's candidate ids on the colour line (8
  conversion candidates ids 31-38, colour fields 42-45 in the baseline) to see which side the canonical operand order favours; passes 2 and 3
  were not used (no code-identical spelling moved the decision; context budget).

## Skipped (0)
None.

## Grades
- 0x007F26B0: pass 1, 4 grades (baseline re-grade, y4 probe, src x2). On top ~45 free probes: frame-packer traces (tools/my/pk/run.py),
  packer replays (tools/my/sim.py), c2 flag reads (tools/my/fl), c2prio runs.
- 0x00550DC0: pass 1, 1 grade (baseline re-grade). On top ~35 free frame-packer / weight-walk traces. Passes 2-3 not used.

## Names
All names of the matched row and its callees are PDB-proven by chunk 2022 (two sources each; LABELS.tsv carries those rows); the Draw
row's name by chunk 2006. Descriptive (made up, no original found): the local names `fireheart_approach` / `fireheart_rise` (the two
Fireheart control points), and the prior descriptive locals (velocity, difference, direction, middle, up, flat_direction, approach,
approach_distance / approach_height, fireheart, helper_name, parent / target parameter names).

## Header debt
No header was changed by this chunk. hdr/ is the union of the chunk 2022 and chunk 2006 overlays (hdr/README.md lists both, verbatim);
those still have to land first (they build on the c2007 / c2002 overlays).

## Label corrections
NAME_FIXES.tsv repeats the six chunk-2022 corrections the matched row depends on (CTCLightningNode::SetPParent / SetPSourceOfFaction /
SetPSourceOfHit landed as unattributed AssignTarget_*, GetDef<CLightningDef>, CCountedPointer<CLinearSpline>::Reset, ProcessPoints).
