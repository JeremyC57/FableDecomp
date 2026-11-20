# NF176 lessons (one line each)
- Run the REFS probe first for the whole slice (369 compiles took about 15 min): it settled 13 done-already / rejected entries without one edit.
- ENTRIES columns drift by tag: from q4178 on, "correct_symbol" can hold the landed spelling and the claim sits in the evidence column; read both before judging.
- A static's mangled name embeds its function's own leaf: renaming a function-local static means renaming the function, so every referencer of the function joins the family.
- A binder whose callee stand-in is a whole free function at another address (0x00810F60 Cons_val) is usually a mis-identified row: check the PDB name set by the retail layout run before respelling.
- `pgrep -f <pattern>` inside `bash -c` matches its own command line; wait on a PID instead.
- Old rows with a malformed `// Leaf:` line (comment text after the symbol) still grade (largest function); leave the line and say so.
- For a struct key that changes when a global class is nested (U vs V), copy the exact spelling from the PDB name set (CSymbolInfo@1@) and confirm with the PAIR line.
