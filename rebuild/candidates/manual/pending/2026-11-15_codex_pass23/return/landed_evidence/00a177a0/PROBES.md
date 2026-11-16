# Pass23 — CPhysicsMesh::GetPointIntersectionFromToUsingSegmentation

Read every supplied prior source (hand293, hand620, retry2_sec5, Q12, Q13), all hand-back notes and all six neighbors. Sec5 reports8grades; hand620 reports6; Q12/Q13 are baseline regrades. Lost individual variants are not invented here. Prior comments identify one integer-load scheduling residue, but the supplied best also has wrong class keys, access and callee names.

| Grade/tag | Change and measured result |
|---|---|
|baseline_package_headers|Provided draft compiled840bytes. Short Leaf needed diagnostic resolution; no naming acceptance. One load-order residue.|
|shared_header_rebase_01|Real CPhysicsMesh/CArray/C3DTriangle2/CCountedPointer/CPlane/C3DVector; proven private target, const methods and global names.840bytes,8hunks.|
|plane_distance_expression_02|Flat scalar distance expression instead of grouped Dot.838bytes,7hunks; scheduling and operand roles worse.|
|stepwise_direction_products_03|Real Dot for plane distance; two direction products written as incremental locals.838bytes,9hunks.|
|original_helper_structure_04|Original debug call chain reveals private HasCollidedWithTriangleAt, GFDotProduct vs C3DVector::Dot, original GetA/B/C, outer hit/normal locals. Restore it, including GFSqr-based Normalise.305bytes because VC7.1 calls the large helper.|
|original_helper_budget_05|Owner ladder T200: helper still called,305bytes.|
|original_helper_size_budget_06|Owner ladder S500: helper still called,305bytes.|
|original_helper_depth_07|Owner ladder scoped depth255: helper still called,305bytes.|
|original_helper_expansion_08|FABLE_FORCEINLINE only on the proven original helper after ladder failures.840bytes,264instructions,3hunks/two delayed integer loads. All temporary budget flags/pragmas removed.|
|grouped_newton_schedule_09|Existing natural Newton-product grouping:840bytes; only normal.X integer reload remains delayed.|
|grouped_normalise_schedule_10|Existing squared-sum grouping adds one scheduler tuple; residue unchanged.|
|grouped_cross_schedule_11|Existing first cross-component grouping adds another tuple;840bytes,264instructions,zero masked differences.|
|portable_host_header_12|New LP64-only safe float-bit copies; original VC7.1 text unchanged.840bytes/264instructions/zero differences retained.|

These are12 source compilations after the baseline, not12 independent strategies. The inline-control ladder and scheduling-grouping substeps are related experiments. No parked minimum-attempt claim is needed: this row matched.

## Fresh verification

Native VC7.1 RTM13.10.3077 compiled the self-contained test /Od /W3 WITHOUT /EHsc with include/ only. All6,912axis-plane scenarios passed. GCC64 C++03 passes the same test. Cover three axes, both windings, three scales, two face orders, four transverse positions, four segment ranges,0..3candidate faces, and normal/forced-plane-rejection/forced-triangle-rejection cases. Check nearest point, normal, distance, untouched failure output, exact external arguments, call counts and unchanged inputs. The actual shared classes are used; only external methods are stubbed. Table seeds are generated mathematically rather than copied from game data. Borrowed arrays are detached before real container destruction.

Fresh audit:11object relocations all resolve to the correct five callees, two globals and three literal values. Whole function extent ends at00a17ae8 with ret12. Exact private leaf confirmed in objdump symbol table. Final aggregate-header regression is also required before packaging.

## Why this is better than the inherited draft

No substitute classes, fake DotDir/FastNormalise methods, free scratch-list getter, wrong vector/plane argument class keys, or ad hoc triangle type. Restore the original call graph seen in FableWin03198f80 and03199990. The three natural arithmetic groupings change the compiler scheduler region boundary without changing arithmetic, adding instructions, or forcing any reload. Native no-op host-portability branch avoids LP64 overreads of a float as unsigned long.
