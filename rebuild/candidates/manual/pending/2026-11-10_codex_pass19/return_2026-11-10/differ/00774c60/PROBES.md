# 00774c60 probes

Prior four drafts and all handback notes/neighbours read; all input attempt history remains in input/prior. No prior attempts reset.

## a01_original_locals_loop
Original donor three locals and indexed std::min loop: vectors remain potentially escaped through hidden return; sizes recomputed in loop.
Result: DIFFER 173/188 bytes; instruction agreement 0.3111111111111111. Immutable source and header snapshot retained.

## a02_const_original_vectors
Const vectors: same code; const does not establish no-escape.
Result: DIFFER 173/188 bytes; instruction agreement 0.3111111111111111. Immutable source and header snapshot retained.

## a03_explicit_invariant_counts
Explicit invariant counts and pointer cursor: matches frame, extent and control-flow, leaves allocation/read scheduling.
Result: DIFFER 188/188 bytes; instruction agreement 0.7142857142857143. Immutable source and header snapshot retained.

## a04_indexed_invariant_loop
Indexed loop with invariant counts: strength-reduced byte offset rather than spilled pointer.
Result: DIFFER 185/188 bytes; instruction agreement 0.6762589928057554. Immutable source and header snapshot retained.

## a05_reference_bound_size_temporaries
Const-reference-bound size temporaries: canonicalized to a03.
Result: DIFFER 188/188 bytes; instruction agreement 0.7142857142857143. Immutable source and header snapshot retained.

## a06_alias_receiver_after_return_values
Receiver alias after factory calls: canonicalized to a03.
Result: DIFFER 188/188 bytes; instruction agreement 0.7142857142857143. Immutable source and header snapshot retained.

## a07_original_fastcall_template_default
Original std::min fastcall default /Gr: same target code.
Result: DIFFER 188/188 bytes; instruction agreement 0.7142857142857143. Immutable source and header snapshot retained.

## a08_bind_return_vectors_by_const_reference
Const-reference-bind whole returned vectors: same escape behavior as a01.
Result: DIFFER 173/188 bytes; instruction agreement 0.3111111111111111. Immutable source and header snapshot retained.

## a09_guarded_do_loop_cursor_lifetime
Guarded do-loop with narrowed cursor lifetime: extra entry branch.
Result: DIFFER 202/188 bytes; instruction agreement 0.6527777777777778. Immutable source and header snapshot retained.

## a10_staged_argument_reads
Stage AddSaleItem arguments and establish index before counts: fixes pre-loop scheduling, best 188/188.
Result: DIFFER 188/188 bytes; instruction agreement 0.7571428571428571. Immutable source and header snapshot retained.

## a11_direct_minimum_condition
Direct minimum conditional rather than reference-returning std::min: loses required address-taken count slots.
Result: DIFFER 171/188 bytes; instruction agreement 0.7352941176470589. Immutable source and header snapshot retained.

## a12_named_minimum_reference_exit
Named minimum reference and explicit infinite-loop exit: same best code as a10.
Result: DIFFER 188/188 bytes; instruction agreement 0.7571428571428571. Immutable source and header snapshot retained.

Selected a10; no byte-match claim. Next substantive avenue: full genuine same-TU GetFreeSaleItemPlaces/GetSaleItemsToPlace bodies to remove hidden-result escape effects; their large dependencies were inspected but have not been authored. No invented reduced helper body used. Named invariant counts/cursor are inferred release spelling, not recovered original debug locals.

## a13_portable_original_iterator_typedef
Use original std::vector<GoodsPlace>::iterator typedef instead of assuming all host libraries use raw pointers. Identical188/188code to a10; selected. This portability correction is not an additional independent strategy. VC71 and g++03 fixtures now both PASS867cases; initial VC min and host iterator fixture portability errors corrected, all build logs retained.
