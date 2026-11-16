# Pass23 source probes

All preceding pass history remains in the supplied prior/ drafts. These are new local builds; grouping variants are related probes, not claimed as independent strategy families. The row matched, so no parking minimum is invoked.

|Build|Result and reason|
|---|---|
|original_corner_identity_01|Original protected static corner-table identity; source already uses shared classes. Same8hunks.|
|donor_local_construction_order_02|Match debug constructor order: corners, plane, vector_normal, plane_normal, int_cell_pos. No change.|
|grouped_direction_copy_03|Parenthesized direction dereference at assignment. No change; discarded.|
|original_free_dot_helper_04|Restore original free GFDotProduct and actual local/parameter names, not the member Dot substitute. Same8hunks.|
|original_square_helper_formal_05|Original GFSqr(float n) call graph/formal inside Normalise. Same8hunks.|
|donor_named_cell_parity_06|Restore original base_x/base_y scalar conversions before parity. Same8hunks.|
|square_arithmetic_groups_07|Group each original GFSqr product. Prologue becomes exact; only two swapped pointer-load pairs remain,4hunks.|
|end_reference_after_corner_loop_08|Const end-vector reference introduced after corner sampling. No change; discarded.|
|dot_xy_group_09|Group X/Y pair inside real GFDotProduct.19hunks; discarded.|
|donor_double_zero_comparison_10|Use donor double-zero comparison. Changes four constant operand widths; discarded.|
|grouped_facing_predicate_11|Group the comparison instead of only the float result. No improvement; discarded.|
|dot_product_groups_12|Group the three dot products independently.6hunks; discarded.|
|real_same_tu_segment_predicate_13|Expose genuine PointIsWithinLineSegment implementation from the same original TU. No improvement; discarded.|
|named_facing_predicates_14|Named boolean facing per branch. First edit had a missing declaration and failed compilation; no byte result.|
|named_facing_predicates_14b|Correct the declaration. Exact1017-byte/318-instruction match; all31relocations independently proven.|

Native standalone test history is retained, including two fixture compilation/link fixes and a failed normalization assertion which exposed the missing seed-table patch. Final tests pass207872cases. No production semantics changed to satisfy a test.
