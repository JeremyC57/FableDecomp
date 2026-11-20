# 00a1e5c0 (p25B, Account B, 2026-11-20): CNavigatorAStar::CalculateAndMoveToPosProximity -- READY

| Row | Function | Bytes | Grade | Test | Mutants |
|---|---|---|---|---|---|
| 0x00A1E5C0 | CNavigatorAStar::CalculateAndMoveToPosProximity(..) | 5437 | RELOCATION_MATCH (no flags) | PASS (12 targeted + 20,000 random runs) | 107 live, all killed; device mutants listed separately |

**Strict vland** (LANDVERIFY_STRICT_HEADERS=1, no header overlay): `RELOCATION_MATCH PASS`, with three RELOC_NAME warnings. None is a wrong binding; NAME_FIXES.tsv covers all three.
- `?DELTA_NOTIONAL_ZERO@@3MB -> 0129ba3c 'g_NormaliseEpsilon'`: the known record.
- `?IsConnectedToRegion@CNavRegion@@QBE_NABV1@@Z -> 00a77780 'NavigationGraph_BreadthFirstSearch'`: a forge autoname. FableWin and Ego_r both give the PDB name.
- `list<CNavWayPoint>::size() const -> 006cd240 'CGuiControlTreePane::SortTreeRecursively'`: the landed row there is a made-up name on an
  ICF-folded STLport list size body.

NAME_FIXES.tsv also records two more fixes:
- the catalog label `CNavigatorAStar::MoveTowardGoal`, a forge autoname, for this row;
- the landed 0x00A1A1A0 spelling `long .. const`. Both PDBs spell it ?IsCurrentNavigabilityBetter@CSteeringAgent@@QAE_NXZ.

Headers: none changed. The draft's FABLE_RETAIL_NAVIGATORASTAR_PASS21 hit the C2DVector.h double declaration, which main has since fixed.
The row drops that opt-in, because it renames parameters only and changes no byte. Purity: plain C++, no asm, no pragma, no flag.

## What the parked diagnosis missed
The draft was DIFFER(5437v5437) with 16 diff lines, all in the second (last) expansion of IsRouteClear: the x87 order of
`normal *= GetSmallestNodeSize() * 0.45f` (0x00A1F65A) and of `direction * GetSmallestNodeSize() * 0.125f` (0x00A1F9D1).
- Retail keeps the scalar on the stack: `fld Y ; fmul st(2)`, then a late `fstp st(0)`.
- The draft consumes it: `fxch ; fmul [Y]`.
- The first expansion (0x00A1E932) consumes it in both retail and ours.

The cause is c2's symbol-number sort key, not the text of the expansion. The draft's own block locals differed from Ego_d's.

### Mechanism (paste into FINDINGS_LOG)
2026-11-20 (p25B, 0x00A1E5C0) CNavigatorAStar::CalculateAndMoveToPosProximity: x87 multiply operand order decided by a mod-2048 wrap of
c2 symbol numbers.
- c2's tree canonicaliser sorts the operands of a commutative product by a key whose low word is `symbol_number << 5`, truncated to 16 bits.
  That is the number mod 2048 (c2 symbol-order kit, 2026-11-20). The higher key is loaded first.
- In IsRouteClear's `normal *= scale` the operands are the inlined `operator*=` parameter `scalar` and normal's Y field symbol. The field
  symbol is made later, in the tree optimiser, so it carries a higher raw number.
- In the draft the second expansion's `scalar` is #2025 and the field is #2281, which is 233 mod 2048. Scalar sorts first, so the stack
  value is consumed: `fxch ; fmul [Y]`.
- With the Ego_d locals, the same symbols are #2053 (5 mod 2048) and #2277 (229). The scalar's number has wrapped past 2048 and the field's
  has not, so the field sorts first and the scalar stays on the stack (`fld Y ; fmul st(2)`). The 0.125 product (`2026*1628` -> `1625*6`)
  flips the same way.
- The first expansion (#1263 / #1292) is nowhere near a wrap, so it is unchanged.
- So any change in the number of local-pool symbols allocated before the last IsRouteClear expansion can flip these two products. The fix
  was Ego_d's block locals, not a fitted count.

Measurements (probe/c2symbols/products_*.txt, `fm` dump of every fmul's operand keys; columns are key>>5 = number mod 2048):

| text | exp1 `scalar*X`, `scalar*Y` | exp2 `normal` products | exp2 `direction*scalar` products |
|---|---|---|---|
| draft | 1263*1262, 1263*119 | 2025*2024, 2025*233 | 2026*1628, 2026*1630 |
| Ego_d-exact (no `last`) | 1292*1291, 1292*122 | 2028*2027, 2028*236 | 2029*1663, 2029*1729 |
| final (A+B with `last`) | 1292*1291, 1292*115 | 5*4, 229*5 | 1625*6, 1627*6 |

## Source shape: Ego_d's locals (egod_lines/egod_locals.txt)
Function scope: prox_and_rad_sq, end_node, reset_navigation, start_node, initial_distance_apart_sq.

Blocks:
- {final_waypoint}
- {near_node}
- {new_dest_pos, {old_dest_pos}}
- {init_end_node, init_start_node, init_dest_pos, {near_node}}
- {one_past_begin, next_section}
- {nav_layer, it}

The draft differed in two blocks, now written as Ego_d has them:
- **A, the short-cut block.** Was {it, next}. Now `iterator it = Route.begin(); unsigned char nav_layer = it->NavLayer; ++it;` with
  `IsRouteClear(CNavWayPoint(.., nav_layer, true), *it, true)`. A alone: DIFFER(5437v5437), 45 lines.
- **B, the route loop.** Was {next_section, here, arrival}. Now `iterator one_past_begin = Route.begin(); ++one_past_begin;` with
  `C2DLineF next_section(Route.front().Position, one_past_begin->Position)` and
  `if (next_section.GetSquaredDist(C2DVector(current_pos)) > GFSqr(SteeringAgent->GetProximityForArrival())) break;`.
  B alone: DIFFER(5437v5437), 45 lines.
- **A+B: RELOCATION_MATCH.** A and B are Ego_d's locals and need no label.

IsRouteClear keeps the draft's text, which has Ego_d's IsRouteClear locals (egod_locals.txt). Its `c1 == 0` arm is unreachable: the loop
starts at 1. The arm is compiled, as in the landed CullWayPoints 0x00A1C010.

## `last` (`// Inferred:`, coordinator ruling 2026-11-20)
`CNavWayPoint& last = Route.back();` in the {new_dest_pos} block is the draft's. Ego_d does not list it.

**/Od calibration** (egod_lines/l.cod, b.cod; the block's lines):

| Ego_d line sizes | ours with `last` (VC7.1 /Od) | ours with three Route.back() calls |
|---|---|---|
| 583: 19, 585: 12, 586: 39, 588: 26, 589: 28, 590: 24, 591: 22, 593: 55 | 23, 12, **18** (the `last` line), 19, 12, 14, 17, 20, 42 | 23, 12, **36**, **24**, **26**, 17, 20, 42 |

Ego_d's 39 / 26 / 28 fit three Route.back() calls (VC10 runs 2-3 bytes larger on each Route.back() line), not `last`. So Ego_d's revision had no
`last`, and the label "retail-only, not in Ego_d" stands.

**What the bytes need:**
- Ego_d-exact text (three Route.back() calls): DIFFER(5437v5437), 22 lines. The x87 pair comes back, plus a register swap at 0x00A1E81B
  (`mov ecx,eax` -> `mov ecx,edx`, `cmp al,bl`). The same result as mutant dead/last_to_back.
- A dead `long` in its place (in the block, or at function top): fixes 0x00A1E81B only; DIFFER, 16 lines (probe/egod_shape yD1 / yD2).
- No-local `(*--Route.end()).Position`: DIFFER, 16 lines.
- `CNavWayPoint* last = &Route.back()`, `C2DVector& last_pos = Route.back().Position`, and `iterator last = Route.end(); --last;`:
  RELOCATION_MATCH, byte-equal (probe/last_forms). The form and name are unconstrained.
- So the bytes prove one evaluation of Route.back() held in a named local, but not its name or form. The retail sites are 0x00A1F65A,
  0x00A1F9D1 and 0x00A1E81B.

## Devices: ladder and ablation (ablation/, grades.txt)

| device | without it |
|---|---|
| A (Ego_d nav_layer block) | DIFFER(5437v5437), 45 lines |
| B (Ego_d one_past_begin block) | DIFFER(5437v5437), 45 lines |
| `last` (`// Inferred:`) | DIFFER(5437v5437), 22 lines |
| same-TU strong definition of 0x00A1A1A0 (`// Fidelity:` line; landed body expression verbatim) | declaration only: DIFFER(5445v5437) |
| the draft's `#pragma auto_inline(off)` around it | none: RELOCATION_MATCH, so it is dropped |
| FABLE_RETAIL_C2DVECTOR_ASSIGN3D | DIFFER(5465v5437) |
| FABLE_RETAIL_C2DVECTOR_ISNONZERO_FORCEINLINE | DIFFER(5381v5437) |
| FABLE_RETAIL_CANAVQUADTREENODE_GETSIZE_OUTOFLINE | DIFFER(5463v5437) |
| FABLE_RETAIL_C3DVECTOR | DIFFER(5437v5437), 16 lines |
| row-local GFGetSign as FABLE_FASTCALL (plain inline) | DIFFER(5437v5437), 18 lines |
| row-local CNavigatorAStar::GetNodeAt body | DIFFER(5314v5437) |
| row-local IsEqual body | DIFFER(5425v5437) |
| row-local IsRouteClear body (declaration only) | DIFFER(2529v5437) |
| FABLE_RETAIL_C2DVECTOR_SQUAREDMAGNITUDE_YX, _C3DVECTOR_SQUAREDMAGNITUDE_XYZ, _NAVIGATORASTAR_PASS21 | no byte change: dropped |
| the other FABLE_RETAIL_* defines | each SRC_FAIL (header layout / body selectors) |

Variants that are also RELOCATION_MATCH (text unconstrained):
- `inline` on IsRouteClear or IsEqual;
- GFGet2DDistanceSquared as `return dx*dx+dy*dy`. The draft's incremental `dy *= dy; dy += dx*dx` (the FableWin donor body) is kept.

No flag replaces the pragma, because the pragma itself changes nothing. The 0x00A1A1A0 body stays:
- Ego_r.pdb puts IsCurrentNavigabilityBetter in lib_navigator_a_star.obj, this row's object.
- With the body visible, VC7.1 keeps the front route node in edx across the call (0x00A1F4A8 .. 0x00A1F4BB).

## Test (test/00a1e5c0.cpp)
Links the real row object. These run as compiled:
- the row body and both expanded IsRouteClear walks;
- GetNodeAt / GetMyNodeAt and IsEqual (+ operator==);
- the STLport list bodies the row compiles (malloc / free of 0x14-byte nodes);
- the inline CSteeringAgent accessors;
- the TU-mate IsCurrentNavigabilityBetter.

Setup:
- Every external is bound by /alternatename to a __fastcall stub that logs `this` and every argument. Every unused vtable slot is bound to
  a trap.
- The world is a 6 x 6 x 2 grid of 10-unit quad-tree nodes (0x10-byte objects with the header layout). It has random blocked / type /
  preferability / region / hole / layer tables; GetNodeAt, IsNeighbourOf, GetRegion and IsConnectedToRegion are computed from it.

Model(): written from the source statements, including IsRouteClear's. It runs on a copy of the navigator with a deep-copied route. Each
run compares:
- the return value;
- every navigator byte, with node, iterator and own-object pointers translated;
- the route contents, the steering agent and resulting_vector;
- the full call log: kind, `this`, every argument, floats to 1e-5.

The targeted biases are:
1. close destinations;
2. axis-aligned / close destinations with stop-within-line-of-sight, for IsRouteClear's vertical / horizontal flags;
3. exact `<=` ties, (radius + proximity)^2 = 25 against distance 25;
4. a kept one-way-point route at the current position (the final reached test);
5. a destination 4e-5 across a cell edge, for IsRouteClear's near-zero-line return.

Paths reached in 20,000 runs: reached 4,601 / cannot 6,917 / moving 8,494; Initialise 6,604; CalculateRoute 4,879; IsRouteClear 1,175
(multi-step walks 232); UpdateGoal 1,447; blocked-node copies 5,769; way-point erase 566; FindNearestNavigableNode 1,455; CullWayPoints 1,792;
MoveTowardNextWayPoint 7,115.

## Mutants (mutants/)
**Live: 107 one-decision mutants (ma1e.tsv), all killed.**
- mut2 killed 95 with the first test.
- 7 survivors (rerun.tsv / rerun2.tsv) were killed after the test gained biases 2..5 and a CalculateRoute stub that sometimes leaves
  PartWayThroughAStar alone. They were L101, L105, L195, L262, L329 and L367 x2.
- One CC_FAIL (`min` -> `max`, no macro) was replaced by `best_cost = from cost`, which is killed.
- 4 SAME_CODE mutants (constants 0.45 / 0.5 / 0.125, `<= DELTA` -> `<= 1.0f`) were compiled and run against the test (sc/), and all FAIL.

**Unreachable arm** (ma1e_deadarm.tsv): IsRouteClear's `c1 == 0` arm. Its 3 mutants survive the test; the grade kills them (dead/:
DIFFER(5428v5437), DIFFER(5424v5437), DIFFER(5437v5437) 4 lines).

**`last` -> Route.back()** (dead/last_to_back): behaves the same (test PASS); grade DIFFER(5437v5437), 22 lines. The pointer / `C2DVector&` /
iterator forms are byte-equal and listed as unconstrained.
