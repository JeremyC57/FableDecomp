# CPSCUpdateNormal::Update — pass21 work in progress

Identity: both original PDBs and vtable0129FC1C slot9. Selected source uses shared original types; inherited local replacement classes are retained only in immutable history.

## Strategies and diagnostics

- Baseline supplied source:5601/5602 after excluding14 padding bytes. Incorrect local list signatures, incomplete types, two invented perpendicular helpers, frame2D0.
- a01: rebase onto shared CPSCBase, CParticleSystem, emitter, math and pointer headers; restore complete original CParticle/CRandomAcceleration types, scoped_ptr ownership, private/protected access and proper STLport signatures. Initial compile exposed required existing dependency opt-ins; a01b repairs these,4752/5602. No new packing; existing approved C3DVector packed branch supplies native offsets.
- Inline diagnostics: S15 neutral4752; T200 expands original local-transform/GFRandom helpers5393 but leaves quaternion ToMatrix called; T4005425 also incorrectly expands list push_back. Scoped depth255 around ToMatrix neutral. These are diagnostics, not independent reconstruction strategies.
- a02: portable shared-header opt-in for quaternion ToMatrix after the ladder above:5815/5602. Ordinary native helper call set restored.
- a03/a03b: original const timestep, particle reference, local_acceleration/system_accelecration/acceleration separation, original -= accessor, donor TransformPoint after the zero-translation quaternion matrix, and actual cross_vec blocks. 5601/5602; frame2D0,50 loose hunks. Compilation dependency repair is not a separate strategy.
- a04: original CMatrix3x4::Set(CRightHandedSet,translation) body from0261C580 replaces expanded call-site spelling; neutral.
- a05: original for-owned iterator scopes and collision-local declaration positions; neutral.
- a06: donor02F3FD34..4D proves velocity += acceleration * fraction. The inherited draft modified acceleration in place. Restoring the actual returned vector temporary fixes frame2D0 ->2DC, adds the exact12-byte frame object:81 nodes/59 slots versus80/58. 5601/5602; strict83.437%,46 loose hunks. This is the best measured improvement.
- a07: original C3DVector copy constructor scalar assignments in its body instead of initializer list; neutral. Removed the unused experimental header branch from the selected overlay.
- a08/a08b: earlier expansion of local math helpers under default flags calls Set(CRightHandedSet) unexpectedly,5425/5602. Restoring T200 gives exact a06 bytes. Removed the redundant additional force-inline branches; retain simpler a06 helper policy.
- a09: original GFPower(float,float), proved by FableWin name and01C954E0 body, used at the two donor call sites; neutral but restores original idiom.
- a10: original positive orientation-arm ordering, then UpdateOrientation else arm; neutral.

## Compiler evidence

The existing local c2 diagnostic proxy now works without GDB/TCP. Every traced object is checked byte-for-byte against the ordinary unmodified RTM object; FRAME_VALIDATION.json records this. Native bytes are never substituted. Frame trace a06 measures0x2DC exactly. The missing object was a real scaled-vector temporary, not padding or a dummy local.

First residue: the two environment-return temporaries use stack1C4 instead of1CC. Remaining differences include velocity-copy/arithmetic scheduling, math operand/temporary slots, orientation stores and loop padding. This is IN_PROGRESS, not a claimed match and not a final park. A full-body behavior fixture remains pending. Do not equate numbered probes or flag builds with ten independent strategies.

- a11: existing shared C3DVector SCALE_X_PAREN opt-in tests measured scheduler tuple-boundary mechanism, previously successful on007bc6fa. Current first arithmetic mismatch interleaves velocity-copy stores and x87 acceleration products; no changed math or local model.

a11 result5601/5602,101strict/51loosehunks versus selected98/46: worse,restorea10. Grouping changed scheduling but not in the required direction.

Full unchanged Update behavior fixture PASS under VC7.1 /GR- and64bitg++03 (main_run001),29 scenarios: timestep/fraction/drag,gravity/2D/local transforms,parameter fallback/wind,lifetimes/alpha,erase iterator,collision/decal allocation/emitter failure,noise allocation/reuse,orientation and emitter-list cleanup. Complex math dependencies are deterministic doubles;no helper correctness or byte-match claim. Inactive-particle loop is excluded because native-selected control flow does not advance on that path.

- a12: restore original dead old_rhs calculation (PDB local -1A0 and donor02F4040F..2E) with genuine visible ToRightHandedSet body02F30360, original Init01D0D780 and GetOrientation02F41070. No guessed stand-in. Tests whether native elimination of this pure calculation still affects later code shaping. All added names have original PDB spelling and donor evidence.

a12:5601/5602;strict84.2146% versus83.4370%,loose44hunks versus46. Original unused calculation is fully eliminated from the target and influences subsequent scheduling. Selected a12 because it restores proven original source and improves instruction agreement; previousa10 remainsavailable. No new force-inline annotation needed. Objdump confirms exact three added symbols.

## Continued original-source reconstruction
A13 restores original PDB local spellings (trans/rhset/rhs/type/pemitter/rot/rot_mat/orient/it_emitter), byte-neutral. A14 restores original named system_alpha (donor02F3F5C5..D2), byte-neutral. These naming/neutral refinements are not separate distinct-strategy claims. A15 restores donor noise factor=1.0f with a grouped product; minor schedule change. A16 uses donor factor*scale*noise order,5601/5602,84.3701%strict,44loosehunks; keep this original idiom.
A17 changes the explicit intelligent-pointer constructor argument to the original implicit conversion in DecalEmitterList.push_back(pemitter). NativeB0E932 constructs the pointer thenB0E937 separately takes its address; the inherited explicit temporary reused constructor EAX. The implicit conversion restores that LEA and raises the temporary reference weight, moving its8-byte slot from270 to1C4 and the later matrices/return buffers into native positions. 5607/5602,strict91.9907%,40loosehunks. This is an improvement, not a byte match. All normal unmodified RTM grades retained.
A18: grouped addition X in genuine C3DVector;5607/5602,strictunchanged,loose40->43hunks;rejected. A19 restores original donor024543E0 TransformPoint scalar captures and flat xx*E1k+yy*E2k+zz*E3k+E4k body behind an opt-in, instead of another row's parenthesized matrix-first spelling.
A19 original matrix TransformPoint body improves53strict/37loosehunks,5607/5602. A20 original UpdateAngle->GetAngleChange chain, native quantization coefficient, is byte-neutral; retained for original structure. A21 original CreationTime/255.0f is neutral; retained. A22 implicit byte flicker arguments is neutral. A23 restores complete original GFRandom018CF370: multiply/add separate statements and zero-range guard. This removes the extra movcl,al at the second flicker setter,5605/5602,1286/1285instructions,36loosehunks;selected. A24/A25 genuine existing full SetFromRotationMatrix body visible inline and ordinary sameTU definition:both neutral;discarded to keep the simpler declaration. A26 restores original CMatrix3x4 Data anonymous union via existing shared-header opt-in, testing real alias information.
A26 original matrix Data union is neutral;retained original type. A27 restores genuine ClearAcceleration->C3DVector::Clear chain proved02F41010/01D8BC00;5605/5602,50strict/37loosehunks,92.2598%strict;selected. A28 existing matrix-array destination opt-in is byte-neutral and removed. Current best keeps original named locals, noise factor, implicit pointer conversion, flat original TransformPoint, GetAngleChange chain, CreationTime division, complete GFRandom, real matrix union and Clear chain.

## A29–A38 continued math and copy reconstruction
A29 gates the added Quaternion include and Decal forward declaration under the existing row opt-ins; entire normalRTM leaf remains identical. Seven existing-header default preprocessing checks pass.
A30 uses the existing original-type GetRight component spelling:5605/5602,92.96%strict,36loosehunks. A31 explores bulk copy of the proven four-float quaternion;5605,52strict/36loosehunks,earlier changes; discarded in favor of original assignment.
A32 uses the existing cross-product XY grouping option:5605,93.82%strict,22loosehunks,substantially improving the final matrix schedule. A33 default-constructs velocity then uses implicit vector assignment (instead of the user copy constructor):5607,worse; discarded.
A34 uses existing direct XYZ IsNonZero sum rather than the inherited running X/Z/Y form:5605,95.37%strict,31strict/22loosehunks. This fixes both native nonzero-test sequences and subsequent cross-product operand choices.
A35 groups both X and Y sums in the genuine vector addition:5605,95.60%strict,29strict/25loosehunks; the collision-position TransformPoint arithmetic becomes exact. A36 places the independent local-acceleration construction before the velocity copy:5604,95.68%strict,28strict/25loosehunks; first load/add schedule improves and native position-base addressing is restored. The later donor has velocity first, so this source order is a reconstruction hypothesis, not an asserted original source spelling.
A37 also moves the timestep multiplication before velocity construction:entire output unchanged fromA36; rejected as unnecessary. A38 tests the corresponding implicit-assignment shape only on test_pos.
Grouping variants and neutral source spellings are refinement probes, not independently counted strategy families. No byte match is claimed.

A38 result5607/5602,89.74%strict,32loosehunks:rejected. A39 whole-object assignment inside the genuine vector copy constructor yields5633,78.60%strict:rejected. A40 restores the original unused ToRightHandedSet double2.0/divide conditional seen atdonor02F303C7..DD:target leaf unchanged; retained as original-source correction. A41 grouped scalar reads in copy-constructor body:5604,95.06%strict,worse; rejected. A42 supplies the full original GetSystemTransform body before Update under auto_inline(off):target leaf unchanged; removed unneeded visible-body opt-in. Its original private PeekEmitterTransform callee was identified and the wrong older binding recorded in NAME_FIXES.tsv. SelectedA40 retains all improvements; seven existing-header default checks and fresh whole-body tests are recorded separately.

A43 rechecks default construction plus implicit velocity assignment after A40 math refinements:5606/5602,44 strict/29 loose hunks; worse than selectedA40 (5604,28/25). Rejected and A40 source restored. Same copy-strategy family asA33, not an additional distinct strategy.

A44 routes the original vector copy constructor through the genuine Initialise helper. Entire leaf raw-neutral toA40; no improvement. Restored original scalar-copy body.

A45 changes the genuine three-scalar vector constructor from initializer list to same-order body stores under a narrow opt-in. Entire leaf raw-neutral5604/5602; discarded and originalA40 restored. Constructor form is documented separately from copy-constructor probes; no extra match.

## Completion audit of distinct source strategies

At least 18 distinct source reconstructions were compiled successfully in this pass. The conservative list excludes flag-only builds, failed compiles, name/model-only corrections and repeat grades; it is not the total number of compiler invocations.

1. `main_a01b_header_dependencies`: Shared original particle/system/quaternion models. 4752/5602 bytes; 215 strict hunks.
2. `main_a03b_subtract_accessor`: Original acceleration/subtraction accessor chain. 5601/5602 bytes; 194 strict hunks.
3. `main_a05_original_for_scopes`: Original for-loop scopes. 5601/5602 bytes; 194 strict hunks.
4. `main_a06_original_scaled_temporary`: Original scaled vector temporary. 5601/5602 bytes; 98 strict hunks.
5. `main_a07_vector_copy_body`: Genuine vector copy constructor visibility. 5601/5602 bytes; 98 strict hunks.
6. `main_a09_original_power_helper`: Original power helper chain. 5601/5602 bytes; 98 strict hunks.
7. `main_a10_original_orientation_arm`: Original orientation branch shape. 5601/5602 bytes; 98 strict hunks.
8. `main_a17_implicit_pointer_argument`: Implicit pointer argument conversion. 5607/5602 bytes; 55 strict hunks.
9. `main_a19_original_transform_point`: Original matrix point-transform association. 5607/5602 bytes; 53 strict hunks.
10. `main_a23_original_random_body`: Genuine random helper body. 5605/5602 bytes; 52 strict hunks.
11. `main_a24_visible_original_matrix_quaternion`: Visible genuine matrix/quaternion helper. 5605/5602 bytes; 52 strict hunks.
12. `main_a27_original_acceleration_clear_chain`: Clear acceleration through actual vector helper. 5605/5602 bytes; 50 strict hunks.
13. `main_a28_existing_matrix_array_destination`: Real matrix-array destination. 5605/5602 bytes; 50 strict hunks.
14. `main_a31_orientation_bulk_copy`: Implicit orientation value copy. 5605/5602 bytes; 52 strict hunks.
15. `main_a32_cross_product_groups`: Cross-product grouping. 5605/5602 bytes; 42 strict hunks.
16. `main_a34_nonzero_direct_sum`: Direct squared-component sum. 5605/5602 bytes; 31 strict hunks.
17. `main_a36_acceleration_before_velocity`: Acceleration computation before velocity copy. 5604/5602 bytes; 28 strict hunks.
18. `main_a43_velocity_assignment_corrected_math`: Default construction then velocity assignment. 5606/5602 bytes; 44 strict hunks.

Final lexical-rule correction: the original PDB local iterator spelling contains a substring explicitly forbidden by EXTRA_RULES. Only that non-ABI local is descriptively respelled emitterIterator in source and fixture; all class/member/function/callee identities remain original. Regrade and fresh dual tests are required. This is not a reconstruction attempt or match improvement.
