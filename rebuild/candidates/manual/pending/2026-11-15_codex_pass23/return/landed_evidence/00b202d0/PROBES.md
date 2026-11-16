# Pass23 probes

The supplied best source and sole prior source are byte-identical and were read in full. Earlier18probe conclusions are retained in their comments and handback_STATUS.tsv. Package baseline failed exact Leaf review because it declared a public method while claiming a private symbol; its850-byte diagnostic is not accepted as an identity-correct match.

|Build|Strategy and result|
|---|---|
|original_call_graph_01|Reconstruct original donor call graph and shared class types;850bytes,9hunks. Dead-argument scalar slots now correct.|
|native_float_turns_02|Use native float reciprocal turns instead of debug double division;850bytes,7hunks.|
|magnitude_product_groups_03|Group all three squared components;850bytes,10hunks. Moves initialization boundary too far.|
|magnitude_xy_product_groups_04|Two squared groups XY;850bytes,5hunks. Initialization exact, magnitude sum order wrong.|
|magnitude_xz_product_groups_05|Two squared groups XZ;850bytes,4hunks. Same boundary, first two sums still reordered.|
|magnitude_yz_product_groups_06|Two squared groups YZ;850bytes,242instructions exact.8relocations independently verified.|

These six builds represent original-source restoration, angle-precision correction, and a related scheduling-group probe family. They are not claimed as six unrelated strategies. The row matched; no parking minimum is invoked. Native and GCC behavioral tests pass on their first fixture attempt.
