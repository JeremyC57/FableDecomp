# NF90 lessons

- **Read the PDB for container calls before choosing a spelling.** FableWin.pdb names the exact CVectorMap members a TU uses: AddTC
  has `Insert(key, value)` + `Sort()` and no `Update`. Retail's lack of a Dirty re-test confirms `Sort`. The `Insert(pair)` + `Update`
  form costs a grade and still differs (513v504).
- **Use Ego_d.pdb parameter and local names for the body's skeleton.** In 0x004C9D60, the bool `inserted_in_place` and the nested
  `component_it` / `remove_tc_it` scopes match the retail control flow directly.
- **Fix a missing inline expansion with a per-row `/d2inlS<N>` flag first** (preference 2), then sweep N. AddTC needed 60 (20, 30
  and 45 all differ); 0x004F9934 needed 22.
- **Emitting an implicit special member (dtor / operator=) from a shared header.**
  - A plain anonymous-namespace helper (qualified destructor call, or `a = b`) under `#pragma optimize("s")` emits the COMDAT
    out of line.
  - Do not use `#pragma inline_depth(0)` around such a helper. The implicit member is generated under the same pragma, so nothing
    inlines into it: the map and vector<long> destructors were called instead of expanded (111v135).
- **One-time capstone disassembly of an object COMDAT** (`dobj.py`) beats objdump. objdump stops at local labels, so it cannot show a
  single COMDAT function cleanly.
- **Shell paths.** g.sh / vl.sh cd into the repo, so row paths must be absolute. A relative path gives CPP_FAIL_SKIPPED, which is not
  a grade but wastes a call.
- **Never `ls` or `diff` a file that may be missing as the last command of a Bash call.** It makes the call exit non-zero; end with
  `; true` or `echo`.
