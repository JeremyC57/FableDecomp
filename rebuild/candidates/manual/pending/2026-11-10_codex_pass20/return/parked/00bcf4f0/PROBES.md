# New source attempts
Fifteen authored source variants across twelve families; flag budget probes are diagnostic and not counted as source attempts. Closest selected source is a09/budget144,final documented regrade.
- a01_original_reciprocal_graph: DIFFER,271v374,12hunks,flags.
- a02_named_camera_product: DIFFER,374v374,3hunks,flags/d2inlT200.
- a03_sequential_camera_scales: DIFFER,380v374,4hunks,flags/d2inlT200.
- a04_original_vector_copy: DIFFER,295v374,12hunks,flags/d2inlT100.
- a05_named_interpolator: DIFFER,275v374,7hunks,flags/d2inlT100.
- a05b_named_interpolator_expanded: DIFFER,379v374,17hunks,flags/d2inlT200.
- a06_explicit_assignment_call: DIFFER,374v374,3hunks,flags/d2inlT200.
- a07_original_vector_alignment: DIFFER,374v374,3hunks,flags/d2inlT200.
- a08_named_source_reference: DIFFER,374v374,3hunks,flags/d2inlT200.
- a09_named_destination_reference: DIFFER,374v374,2hunks,flags/d2inlT200.
- a10_representation_copy: DIFFER,374v374,3hunks,flags/d2inlT200.
- a11_both_position_references: DIFFER,374v374,2hunks,flags/d2inlT200.
- a12_exit_expression_scope: DIFFER,374v374,2hunks,flags/d2inlT200.
- a13_named_camera_receiver: DIFFER,373v374,17hunks,flags/d2inlT200.
- a14_component_position_copy: DIFFER,383v374,17hunks,flags/d2inlT144.
- a15_copy_scope_end: DIFFER,374v374,2hunks,flags/d2inlT144.
- budget_boundary_T125: DIFFER,231v374,9hunks,flags/d2inlT125.
- budget_boundary_T137: DIFFER,231v374,9hunks,flags/d2inlT137.
- budget_boundary_T143: DIFFER,231v374,9hunks,flags/d2inlT143.
- budget_boundary_T144: DIFFER,374v374,2hunks,flags/d2inlT144.
- budget_boundary_T146: DIFFER,374v374,2hunks,flags/d2inlT146.
- budget_boundary_T150: DIFFER,374v374,2hunks,flags/d2inlT150.
- diagnostic_T200: DIFFER,374v374,4hunks,flags/d2inlT200.
- selected_documented: DIFFER,374v374,2hunks,flags/d2inlT144.

Families: (1) full original reciprocal/helper/layout graph; (2) named camera product, fixes x87; (3) sequential compound camera scaling, extra stores; (4) explicit vector value copy, extra staging/call; (5) named interpolator receiver, rotates whole body; (6) explicit assignment member call,flat; (7) existing original vector alignment opt-in,flat and not retained; (8) source/destination/both reference forms, destination fixes Initialised store; (9) standard representation copy,flat; (10) exit-expression/narrow copy scope,flat; (11) named camera receiver,whole-body rotation; (12) component-wise copy,extra x87 movement. Every earlier version preserved in immutable history; selected no raw copy or alignment change.

20736behaviorcasesPASSactualVC71/g++03; all7operands audited.

## Final close-row review
Seven additional probes a16..a22 all remain374/374 with exactly the same adjacent load/store residue. Explicit assignment on the already named destination, grouped copy source/receiver, grouped transform argument and three genuine TransformPoint last-row arithmetic groupings do not move the tail pair. These are grouped as two additional review families (combined copy-call form and arithmetic grouping), not seven independent strategies. Selected source and headers unchanged.
