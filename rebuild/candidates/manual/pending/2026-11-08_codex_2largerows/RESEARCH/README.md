# Diagnostic research

Only ordinary, unmodified RTM compiler builds supplied grades. The optional allocator proxy source hooks the child compiler process in memory, never the compiler file on disk. It contains instrumentation trampoline bytes, not game bytes. No proxy binary or compiler is included. Four DIAGNOSTIC_VALIDATION receipts in HISTORY confirm selected-section equality with ordinary builds. The original c2.dll SHA256 was bcd28f39b1798b07989c30a1df471cf87fc20e3802d130901a08f83800e6b81e.

Local Python tools are preserved as research and contain environment-specific absolute paths/imports. grade.py imports the existing verifier's environment/path helpers and coff_reader; it is not self-contained. Use your established grader or adapt dependencies. trace.py additionally expects a locally built proxy and task-specific Wine runner; none of those runtimes is in this package. frame_audit.py produces diagnostic alignment, not identity proof. Its /FAs build is accepted only after its selected section matches the ordinary object.

Native binaries, object files, raw byte dumps, instruction listings and raw instruction-diff files are excluded. Included frame maps, allocator traces, source diffs, compiler receipts and symbol/relocation metadata retain the useful findings without distributing game data.
