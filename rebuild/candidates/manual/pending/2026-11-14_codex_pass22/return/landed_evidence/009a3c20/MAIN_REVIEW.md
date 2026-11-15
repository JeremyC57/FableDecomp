# Main verification — 009A3C20

The independently compiled selected constructor matches all 1,298 retail bytes after relocation masking (414 instructions). All 32 target and 12 vtable operands were re-audited against native targets and the recovered original identities. The complete production body was checked verbatim in the behavioral fixture, then rebuilt and run with VC7.1 RTM and GCC: 2,304 cases each passed. Native layout compilation confirms 12 sizes, 34 offsets and two physical bitfield initializers.

Merged overlay36 recompiles all78 accepted rows, preserving every inventoried emitted C++ code section and relocation name, and every selected target remains exact. See `review/main_009a3c20/MAIN_AUDIT.json` and `review/MERGED_AUDIT_36.json`. Compiler weak scalar fallbacks for external native vector-deleting vtable symbols are explicitly inventoried; their unselected bodies are not presented as additional matches.
