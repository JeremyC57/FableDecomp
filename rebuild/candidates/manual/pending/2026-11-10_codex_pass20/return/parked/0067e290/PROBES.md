# New attempts

a01: complete original functor bases and genuine temporary comparator, real shared bool traits signature, all shared classes.

a02: original generic traits writer named signed-long conversion local, as unoptimized donor shows.

a03: branch-local original traits objects, restricting empty object lifetime to the selected transfer branch.

a04: visible original bool traits reader body (donor01874a40) calling genuine shared ReadEBOOL chain; retain out-of-line choice.

a05: visible original WriteSLONG and WriteEBOOL full bodies, independently read in donor03082880/03082720; noinline through auto_inline(off).

a06: restore both original traits constructors/destructors, plus complete visible enum ReadSLONG helper. Full original constructor/read graph removes legitimate default stores through callee analysis.

a07: early-return transfer guards instead of switch lowering, preserving every mode behavior and full genuine helper graph.

a08: PAREN scheduling tuples around enum conversion, stream receiver and converted argument, using measured playbook expression-shape lever. Restore switch.

a09: split outgoing value and stream receiver into real named value/reference locals in save arm, preserving value-first evaluation.

a10: const-reference outgoing alias after stream selection, so the live value is read at use instead of copied early.

## Measured results

- a01_original_functor_bases: 273/273, 10 strict hunks, first byte difference 92.
- a02_original_named_enum_conversion: 273/273, 10 strict hunks, first byte difference 92.
- a03_branch_local_traits: 273/273, 10 strict hunks, first byte difference 92.
- a04_visible_bool_reader: 273/273, 10 strict hunks, first byte difference 92.
- a05_visible_original_stream_writers: 273/273, 10 strict hunks, first byte difference 92.
- a06_complete_traits_lifetime_graph: 273/273, 10 strict hunks, first byte difference 92.
- a07_transfer_guard_returns: 274/273, 13 strict hunks, first byte difference 29.
- a08_grouped_conversion_operands: 273/273, 10 strict hunks, first byte difference 92.
- a09_named_save_value_and_stream: 273/273, 10 strict hunks, first byte difference 92.
- a10_reference_save_value: 273/273, 10 strict hunks, first byte difference 92.

Selected a01 full genuine functor bases, regraded selected_documented; 273/273,102instructions,10hunks. All17operands correct,18432testsPASSbothcompilers. PARKED after10distinctstrategies. Header/body-only helper attempts preserved in immutable history.
