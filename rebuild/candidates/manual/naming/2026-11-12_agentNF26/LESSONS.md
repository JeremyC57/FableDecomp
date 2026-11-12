# agentNF26 lessons (one line each)

- Diff the relocation pairs of a baseline batch against the final batch: it yields RENAMES section 2 and catches stray rebinds automatically.
- A scratch Python tool named dis.py shadows the stdlib `dis` and breaks capstone; never name a helper after a stdlib module.
- Keep `\n`-escaped replacement pairs in quoted heredocs; printf in double quotes expands them into real newlines.
- Changing an object's type (unsigned long -> const long) through a `const unsigned long&` helper makes a temporary and changes codegen; retype the helper parameter too.
- A mechanical `*(T*)&g_x` -> `NAME` respell must drop the leading `*`; grep the output for `= *NAME` before grading.
- A protected static's address in a test is reached through a small derived peek struct, which keeps the row's @@1 spelling linkable.
- Turning a forward declaration into a local class is a NEW local model; use the shared header's flattened view with an opt-in instead.
