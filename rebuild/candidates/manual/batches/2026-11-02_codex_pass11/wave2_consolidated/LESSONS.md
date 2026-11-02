# cons11 lessons (one line each)

- Do not union-resolve Codex feature macros mechanically. Rows also define STL members themselves, so hand-write one STLport model (stlport/fable_containers.h) and delete the row-local copies it makes redundant.
- Under Wine, VC7.1 `cl.exe` can hang for more than 5 minutes on some fatal errors (a missing `#error` macro, a type redefinition cascade). Run `gcc -E` with `-D_MSC_VER=1310 -D_WIN32` first (tools/pp.sh) to catch preprocessor failures in under a second.
- Per-class gating of a Codex cluster block: `fileres.py <codex file> <row defines> FABLE_RETAIL_<CLASS>`, then check that the `#else` branch equals the repo text. Repo files changed since the Codex base need a manual three-way merge (CSimpleMemoryPool).
- A Codex "second model" header (engine/CWideString.h, engine/CDefStringTable.h, stlport46/*.h) always has a home in an existing header (fable_string.h, fable_definition_table.h, fable_containers.h). Add a gated branch there instead.
- Patch adopt.py so it never re-adds a second-model header; otherwise it re-copies them silently.
- When a retail TU expands a member that the repo declares out of line (CScriptThing::operator=), use the header's own contract: an inline definition in the row after the include. It is not a header variant.
- If retail expands a base dtor with no vptr store, the dtor is compiler-generated. An inline `{}` adds the store and a declaration adds a call. Gate the declaration with a per-class macro, as for CLevelScriptBase.
- If retail folds a constant key after an out-of-line call (`cmp [eax],8`), the callee body was visible in the TU. Use a same-TU `__declspec(noinline)` body (EXTRA_RULES chunk 422), not a bare declaration.
- A spill/reload of a just-stored member (`if (Data)` vs `if (data)`) is a real source difference. Prove it with try.sh before calling it a register-allocation gap.
- tools/fdiff.py (full disassembly diff with call/immediate/relocation-zero operands masked) and tools/calls.py (retail call targets vs object relocation symbols, in order) find the first structural difference in seconds. q.py `-v` truncates at 80 lines.
- The `--recheck-landed` vland takes about 1 s per row. Split a large includer set (3,344 NSCRIPT rows) into 4 payload/oracle shards with separate LANDVERIFY_WORK directories and run them in parallel.
