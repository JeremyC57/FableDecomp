- (agentC6, 0x00BB3E80) A dead `if (0)` arm's record weight scales with its IL: calls / string args count, a plain-pointer condition
  counts ~0, a condition with an inline accessor counts AND moves early-record orders (L). Class-typed temporaries in a dead arm change
  live codegen. Dead arms inside a FORCEINLINE helper expanded many times can change frame / inlining.
- (agentC6) The shared release-assert macro (hdr/fable_assert.h, FABLE_RETAIL_ASSERT_DEAD_ARM) closes record-count ties that move only
  the "O" class of orderings; scan the count with tools/nat2.py site labels (2 s per grade) and keep conditions accessor-free.
