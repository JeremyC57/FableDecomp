# Pass23 — original DirectShow parent and generated cleanup

The package maps this row to the earlier destructor00caa790. The original EH chain instead identifies parent00caa8d0. Re-running tools/eh_parent.py finds its push at parent+6; the preceding six-byte FS load is part of the same prologue. This is a real new parent, not the listed landed destructor.

|Build|Strategy|Result|
|---|---|---|
|sdk_original_parent_01|Original Microsoft amfilter.cpp method body; complete real CBaseMediaFilter added over existing package CUnknown/IMediaFilter/CAutoLock types; /EHsc|Whole parent and generated18-byte section match immediately.|
|portable_sdk_headers_02|New opt-in for host size_t memcmp and signed32-bit E_POINTER meaning on LP64; native header tokens unchanged|Whole parent,18-byte generated section,10-byte existing CAutoLock destructor and36-byte EH data all match.|

This is one reconstruction strategy, not ten independent attempts; it matched without parking. No source-shape tuning was needed. The extra source/header dependency is handed back; no copied executable bytes or handwritten cleanup instructions are used.

## Tests

Each self-contained test pastes the three real SDK parent bodies and includes only the shared include tree.76normal scenarios cover clock replacement/null/self-alias cases, AddRef-before-Release order, pointer-error handling, state transitions, Pause failure and64-bit time values. API and interface stubs record exact lock/call order. Native /Od/W3 WITHOUT /EHsc passes. A separate /EHsc run and GCC64 C++03 also pass four thrown-callee cases, verifying release of the actual CAutoLock at exceptional exits. The first EH harness attempt for01216e00 failed to insert the flag; its result is explicitly marked as an ordinary test, and the correctly flagged02run supersedes it.

The compiler emits the assigned cleanup and handler when compiling the included parent. The entire reachable graph is rechecked, including all relocation targets. The graph's10-byte CAutoLock body is existing support code, not another reserved-function completion.
