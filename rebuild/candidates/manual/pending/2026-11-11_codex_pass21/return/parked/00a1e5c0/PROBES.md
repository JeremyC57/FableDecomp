# Pass21 navigation probes

Every build is retained in history. Baseline/regrades and name-only repairs are not counted as distinct source strategies. No final parking decision has been made.

| Build | Size / retail | Strict hunks | Status |
|---|---:|---:|---|
| main_b00_supplied_headers | - / 5437 | - | COMPILE_FAIL |
| main_a01_genuine_header_rebase | 5423 / 5437 | 134 | DIFFER |
| main_a02_implicit_conversion_arguments | 5439 / 5437 | 94 | DIFFER |
| main_a03_cached_steering_receiver | 5439 / 5437 | 94 | DIFFER |
| main_a04_original_square_helper | 5439 / 5437 | 94 | DIFFER |
| main_a05_original_radius_and_locals | 5439 / 5437 | 98 | DIFFER |
| main_a06_original_front_references | 5439 / 5437 | 92 | DIFFER |
| main_a07_original_intersection_initialise | 5423 / 5437 | 88 | DIFFER |
| main_a08_original_distance_helper | 5423 / 5437 | 89 | DIFFER |
| main_a09_original_double_normal_scale | 5423 / 5437 | 91 | DIFFER |
| main_a10_original_main_distances | 5435 / 5437 | 19 | DIFFER |
| main_a11_radius_before_square | 5435 / 5437 | 17 | DIFFER |
| main_a12_original_reset_assignment | 5435 / 5437 | 19 | DIFFER |
| main_a13_original_node_accessors | 5437 / 5437 | 18 | DIFFER |
| main_a14_correct_speed_proximity | 5437 / 5437 | 16 | DIFFER |
| main_a15_original_route_locals | 5437 / 5437 | 16 | DIFFER |
| main_a16_radius_original_context | 5437 / 5437 | 11 | DIFFER |
| main_a17_protected_radius_call | 5437 / 5437 | 11 | DIFFER |
| main_a18_grouped_radius_argument | 5437 / 5437 | 11 | DIFFER |
| main_a19_distance_square_helper | 5437 / 5437 | 11 | DIFFER |
| main_a20_original_arrival_expression | 5437 / 5437 | 14 | DIFFER |
| main_a21_original_iterator_no_back_alias | 5437 / 5437 | 14 | DIFFER |
| main_a22_visible_smallest_node_size | 5408 / 5437 | 207 | DIFFER |
| main_a23_grouped_distance_squares | 5437 / 5437 | 11 | DIFFER |
| main_a24_original_separate_node_guards | 5437 / 5437 | 11 | DIFFER |
| main_a25_grouped_radius_operand | 5437 / 5437 | 9 | DIFFER |
| main_a26_grouped_normal_scale_operand | 5437 / 5437 | 15 | DIFFER |
| main_a27_original_inequality_operator | 5423 / 5437 | 87 | DIFFER |
| main_a28_distance_y_first | 5437 / 5437 | 9 | DIFFER |
| main_a29_original_manager_reference | 5437 / 5437 | 9 | DIFFER |
| main_a30_vector_x_product_group | 5437 / 5437 | 9 | DIFFER |
| main_checkpoint_original_headers | 5437 / 5437 | 9 | DIFFER |

## Findings and strategy history

Read best, both prior sources, all six landed neighbours and every supplied handback. Rebased local class models onto original private shared headers and real STLport list/map/vector/priority queue types. CNavWayPoint has natural size10 through the existing C2DVector alignment1; inherited redundant packing removed. Original ENavigatorReturn and node-type enum names are proved by both PDBs.

a01 genuine header rebase exposes correct original CSteeringAgent methods and original CalculateNavigableWidthOfLine callee, replacing invented names. a02 implicit C3DVector conversion inside CNavWayPoint arguments restores the native temporary-copy shape, resolving the old line-of-sight blocker. a03 cached SteeringAgent receiver is neutral, discarded. Diagnostic inherited-draft build is not a new strategy. a04 real GFSqr helper is neutral. a05 restored protected PeekNavigatorRadius and original Main parameter/local names; expression grouping changes entry scheduling. a06 removes the front-waypoint alias and restores native front-node reload order. a07 original lambda/y plus C2DVector::Initialise restores intersection code shape. a08 real GFGet2DDistanceSquared in IsRouteClear. a09 donor double0.45 is wrong for this native build, discarded for native float0.45f. a10 original GFGet2DDistanceSquared calls in Main resolve most frame-slot differences (94 to19 strict hunks). a11 two-stage radius then square fixes entry but changes a later min-call ordering; retained for comparison. a12 original whole reset_navigation boolean assignment is byte-neutral. a13 original IsNavigable/GetNavLayer/GetPosition and GetMyNodeAt accessors plus protected field access and real enum values yield exact total5437 bytes. a14 fixes a genuine inherited semantic error: MoveTowardNextWayPoint takes speed BEFORE proximity, proved by original PDB argument records, donor031ABB81/88 and nativeA1FAAD/A1FAB1 pushes. a15 restores IsRouteClear original names, int best_cost and function-scope start_pos; byte-neutral. a16 retries original two-stage radius in the corrected source context. Further attempts are listed in the build ledger above.

Names: Original row mangled name proved by both PDBs and donor/native method behavior. CalculateNavigableWidthOfLine(const C3DVector&) is original public method (both PDBs plus donor031AB288/031AB8CF), not inherited CalcWayPointWidths. GFGet2DDistanceSquared is proved by FableWin PUBLIC32 and donor01B4F090 body/typed locals, not by a nonexistent Ego_r public entry. Original node protected members/accessors and ENavQuadTreeNodeType are retained in NODE_TYPES_SYMBOLS.json. Full relocation audit and behavior fixtures remain pending until final candidate; do not treat current row as matched.

Selection: main_checkpoint_original_headers; sourceSHA256 34c81cbd1099df6a637707206601d6eb43b2b6aa176a642f44e052e9892a0c36. No attempt or prior history was overwritten.

## Additional x87 probes

- main_a31_named_distance_products: explicitly square original dy then dx before adding;5437/5437, unchanged9 strict hunks. No improvement.
- main_a32_original_scale_grouped_y: original PDB scale parameter spelling plus grouped Y in both C2DVector scalar operators;5437/5437, unchanged9 strict hunks. Grouping is a neutral spelling probe, not an additional distinct strategy. Donor02089C60/02742FA0 confirms the existing multiply/operator bodies. Restored checkpoint header/source.

Correction to earlier pending-fixture note: full unchanged Main and IsRouteClear behavior fixture already PASS under VC71 /Od /GR- and g++03,33 scenarios (main_run_002). Still IN_PROGRESS; no exact byte match claimed.

- main_a33_distance_poststore_read: measured single x87 pair technique, (void)dy after original dy initialization. No change:5437/5437,9 strict hunks. Removed and checkpoint restored.

## Continued operand-shape probes
A34: const-reference scale operand inside both scalar vector operators; unchanged5437bytes/9hunks. A35: const-reference dy inside the original distance helper; unchanged5437/9. A36: split size-return and scaling into a named normal_scale;5437/15hunks,worse. These are expression/lifetime refinements, not three independently proven reconstruction families; all rejected.
A37 restores the original PDB parameter name scale in the two scalar C2DVector operators, without the rejected reference/grouping changes. This is a naming correction, not a source-shape attempt.

A38 accumulates the distance squared in original dy (dy*=dy;dy+=dx*dx;return dy), resolving the first x87 squared-pair mismatch A1EC9E..A4.5437/5437,99.5178%strict,8hunks remain only two later inlined scaling regions. A39 reverses only scalar Y multiply operands;neutral. A40 groups X product in in-place scalar multiply (distinct from prior non-mutatingoperator* probe);neutral. A41 documented post-store float read of scale;neutral. These three refinements are not counted as three independent strategies;original header restored. A42 tries constant-first normal-scale expression.
A42 constant-first factor is neutral; restored A38 selected source and original scalar operators. Current improved source5437/5437,8hunks. No final park.

A43 evaluates scalar products as arguments to the genuine Initialise helper:neutral5437/5437,8hunks. A44 computes through the genuine value-returning scalar operator then assigns the vector:5437/5437 but101strict/22loosehunks andfive fewer instructions. Both rejected; A38 preserved. These test the real helper/temporary chain, not artificial extra uses.

A45 stores Y before X in the scalar in-place multiply:5411/5437,121strict/21loosehunks;worse. A46 computes both scalar products before storing:raw-neutral5437/5437,8hunks. A47 routes the last product through the by-value scale parameter:also raw-neutral. All rejected; selectedA38 source/header restored. These product/storage refinements are not three independent source families.

A48 puts scale first in both scalar products:raw-neutral; reverted. A49 tests the documented chunk805 memberwise-vector construction mechanism inside the genuine in-place operator, preserving its signature and defaults. UnlikeA44 it calls the real scalar constructor directly rather than the value-returning multiplication helper.

A49 produces5437bytes but101strict/22loose hunks, identical regression class toA44; discarded. The chunk805 temporary mechanism does not transfer directly to this two-component in-place operator. OriginalA38 restored.

A50 writes the normal scaling as scoped named factor and direct X/Y products.5437bytes: late A1F65A region becomes exact, but early A1E94A gains the opposite retained-factor residue. Last vector step still differs. Original helper spelling restored; this is useful per-context diagnostic evidence, not a match.

A51 binds the original scalar-multiply receiver by const reference, testing base-symbol identity rather than a reference to the factor.Entire leaf raw-neutral5437/5437; original header restored. Same reference-shape family, not another independent success.

## Completion audit of distinct source strategies

At least 16 distinct source reconstructions were compiled successfully in this pass. The conservative list excludes flag-only builds, failed compiles, name/model-only corrections and repeat grades; it is not the total number of compiler invocations.

1. `main_a01_genuine_header_rebase`: Genuine shared navigator/vector/header models. 5423/5437 bytes; 134 strict hunks.
2. `main_a02_implicit_conversion_arguments`: Implicit constructor argument conversions. 5439/5437 bytes; 94 strict hunks.
3. `main_a03_cached_steering_receiver`: Named steering receiver lifetime. 5439/5437 bytes; 94 strict hunks.
4. `main_a07_original_intersection_initialise`: Original intersection initialization helper. 5423/5437 bytes; 88 strict hunks.
5. `main_a10_original_main_distances`: Original distance expressions. 5435/5437 bytes; 19 strict hunks.
6. `main_a12_original_reset_assignment`: Original assignment/reset idiom. 5435/5437 bytes; 19 strict hunks.
7. `main_a13_original_node_accessors`: Original node accessor chain. 5437/5437 bytes; 18 strict hunks.
8. `main_a21_original_iterator_no_back_alias`: Iterator expression without back-element alias. 5437/5437 bytes; 14 strict hunks.
9. `main_a22_visible_smallest_node_size`: Visible genuine called node-size body. 5408/5437 bytes; 207 strict hunks.
10. `main_a24_original_separate_node_guards`: Separate node guards. 5437/5437 bytes; 11 strict hunks.
11. `main_a25_grouped_radius_operand`: Grouped radius operand scheduling. 5437/5437 bytes; 9 strict hunks.
12. `main_a31_named_distance_products`: Named live distance products. 5437/5437 bytes; 9 strict hunks.
13. `main_a38_incremental_distance_sum`: Incremental distance accumulation. 5437/5437 bytes; 8 strict hunks.
14. `main_a44_value_product_assignment`: Value scalar-product result assigned to vector. 5437/5437 bytes; 101 strict hunks.
15. `main_a45_y_first_inplace_scale`: Y-first in-place coordinate updates. 5411/5437 bytes; 121 strict hunks.
16. `main_a50_scalar_normal_fields`: Scoped factor and direct coordinate normal scaling. 5437/5437 bytes; 8 strict hunks.
