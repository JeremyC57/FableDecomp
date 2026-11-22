# NF239 lessons (one line each)
- Triage from compiled bindings (grade every referencing row at HEAD, keep its relocation list): the table's one witness per symbol and the exe call-site scan both missed live binders (0x00899DB0).
- An STLport base-class member is proven by its size-optimised wrapper: list's `~list() {}` is a 5-byte `jmp ~_List_base` and list(const allocator&) a `call _List_base ctor`; search retail for E9/E8 to the address.
- A PDB that shows `_Tidy` / `_Orphan_ptr` / `_List_val` is Dinkumware: it cannot name STLport layering (no `_List_base` exists there), so a header comment citing it is not evidence.
- A row that includes engine/CPlayerGui.h with MEMBERS (or any retail model guarded by #error) spins the lane cl.exe when a guard macro is missing; run a clang -target i686-pc-windows-msvc -fsyntax-only precheck with the VC7.1 include dir first (tools/pre.sh).
- After a q.py timeout the wine cl.exe child keeps spinning: kill only the processes whose argv[0] is cl.exe and whose arguments name your folder (tools/killmine.sh); another agent's cl.exe may be running beside yours.
- A test that stubs a callee by its old spelling LINK_FAILs after a binder respell: rewrite the stub on the shared header (explicit specialization of the declared-only member).
- census.py's build_candidates regex missed every catalog entry written `Address = '..'; Module = '..'` on one line (65 rows): match `[;\s]*` between the keys or call sites vanish.
- A compiled census of a data address shows which LITERAL each row binds there: a row can grade RELOCATION_MATCH while loading the wrong constant (0x0041735A: 0.5f at the -1.0f address), because relocation fields are masked.
