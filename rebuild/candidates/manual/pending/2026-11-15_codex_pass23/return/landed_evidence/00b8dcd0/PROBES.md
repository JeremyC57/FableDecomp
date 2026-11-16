# Pass23 attempts and test port

1. Baseline real RTM compile on untouched package headers:639/639 masked bytes,198/198 instructions; exact target Leaf. No source-shape retry needed.
2. Replace rejected private fixture classes with actual shared class declarations and the exact target body pasted into a self-contained test. Preserve all962 scenarios,57 overflow checks,20,014 rounding values and ownership/copy checks. Initial GCC compilation exposed forward enums and host ABI assertions.
3. Recover EIndexBufferFormat, EIndexBufferPool and CSimpleMemoryPool::EAreaStatus from both original PDBs. New additive opt-ins permit real shared models on LP64; native ABI assertions remain enabled. GCC C++03 build/run passes.
4. VC7.1 /Od /W3 without /EHsc compiled successfully. Link initially crashed: system trace proves a mapped linker temporary on the full workspace filesystem raised SIGBUS. Windows cmd explicitly setting TEMP/TMP to Z:\tmp restores linking; native run passes. This is infrastructure, not a source-shape attempt.
5. Fresh final source compile on the delivered shared-header additions:639/639 masked bytes. Reaudit all12 emitted matched bodies, including11 existing helper bodies:42 relocation bindings pass; every literal and symbol addend checked. Helpers are not new scope credit.
6. Native preprocessing of the source against package vs updated headers is byte-for-byte identical, including static-assert names. Every moved CSimpleMemoryPool assert is pinned. No fable_string.h change.

No ten-attempt parking rule is invoked: this is a successful test_port row, not an unmatched row parked after one grade. Previous40-spelling rounding-helper ladders and earlier source history remain in the supplied codex_pass22 notes and prior handoff evidence.
