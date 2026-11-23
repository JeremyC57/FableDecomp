# NF316 lessons (one line each)
- Held entries do not free up when the agent that held them lands: its continuation agent (NF314 for NF309) inherits the same spellings-column witnesses, so re-screen with the NEW neighbours' ENTRIES before taking a "freed" entry (12 of 29 here).
- A table line's witness can be stale with the self still wrong: 0x0051E210's QAE `long` line was witnessed by a row (0x005FAE90) that already binds the PDB form, while the self and 9 untabled binders still carried the old form; compile the whole exe census before treating an entry as a binder respell.
- The scratch grader (nf.py grade) can report DIFFER on a landed row that vland matches (0x00846100, 687v686 for HEAD and new): judge an in-place respell by ALL_EQ and the vland recheck, not the scratch grade.
- A full exe census of a target can show a third, untabled stand-in binder (0x006A4DC0 `Sub::Run_006b0210`): the table lists one witness per spelling, so only compiling every census row finds it.
- A `sed` edit of a helper with `#` in the replacement text broke the expression and left the old cwd; edit helpers with a Python replace and grep the result before launching.
