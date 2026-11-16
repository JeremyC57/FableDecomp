# Pass23 — Full collision mesh query

Read all four earlier drafts, Q14 hand-back notes/lessons/status, six neighbors and both original class/type records before authoring. Original FableWin031994f0 confirms the private helper call and full-loop shape. This pass does not repeat the old flattened local-model experiments.

|Build|Strategy|Measured result|
|---|---|---|
|baseline_package_headers|Supplied source unchanged, package headers|818bytes,12strict hunks, wrong public/struct Leaf and substitute names.|
|original_shared_structure_01|Restore original helper graph, shared types, original scalar count and update order; reuse sibling arithmetic opt-ins|818bytes,256instructions, one delayed normal.X integer load (2hunks).|
|grouped_subtraction_02|Existing XY subtraction grouping across original vector operator|818bytes,5hunks; worsens entry and body schedule. Reverted.|
|cross_ungrouped_03|Remove the sibling first-component grouping|818bytes,same2hunks. Reverted.|
|cross_all_grouped_04|Group the three original cross-product components independently via a new header opt-in|818bytes,256instructions,zero masked differences. No reassociation or extra operations.|
|final_source_05|Accurate inline-budget/source comments; identical implementation|Exact match retained.9relocations all audited.|

Three related scheduling experiments plus the substantial original/shared-header rebase; do not count this as ten distinct strategies. This row matched, so no parked-attempt threshold is claimed. The existing HasCollidedWithTriangleAt opt-in is shared with00a177a0, where ordinary inline, per-row budget flags and scoped depth all left calls. Native00a17ff0..00a1820b independently proves expansion here. No new inline device was added to this row's headers.

Native VC7.1 RTM /Od /W3 WITHOUT /EHsc and GCC64 C++03 each pass6912cases using the real shared classes. Exercise0..3faces, three axes, winding/direction rejection, both face orders, three scales, four segment ranges, four transverse positions and forced external failures. Independent geometric oracle verifies nearest output and untouched failure output. Segmentation helper must not be called. Every production statement is pasted in the standalone test.
