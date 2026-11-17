# NF75 lessons
- STLport helpers that a TU instantiates (pair copy ctor, _Rb_tree operator=, _M_insert_overflow) land best as explicit instantiations, using the repo opt-ins in fable_map.h / fable_containers.h. Grade the row under the mangled library name.
- If an implicit copy ctor will not inline under /Os, try `/d2inlS22` with an `// Inline-budget:` line. Sweep S values and record each grade.
- Ego_d carved_functions gives each function's object file. Use it to pick between byte-identical bodies (retail has no ICF): CScrollableList vs CList counted pointers.
- Access-only fixes (IAE/AAE/MAE) need only `protected:` / `private:` on the existing local declaration. Recheck callers that the change makes unable to compile.
- With a multi-header class chain, include the most-derived opt-in header (CCombatActionBase.h) last, or the negative-subscript asserts fire.
- A header opt-in that is gated on another macro (CWorldMap STRONGESTTHEME needs CWORLDMAP + CCHARSTRING + SCOPED_PTR) fails with SRC_FAIL, not with a clear error. Read the gate first.
- Never put backticks in a double-quoted shell argument (st.py notes). Write notes through a quoted heredoc.
- Check that a path exists before running `ls` on it, so every Bash call exits 0.
- Before using an access edit inside an existing opt-in block (CMap THEMEPALETTE), find and regrade every user of that block.
