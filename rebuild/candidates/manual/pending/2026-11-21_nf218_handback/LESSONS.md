# NF218 lessons (one line each)
- A queue line's binder list can be short: callers.py found a fourth wrong binder of 0x0041D165 (0x00621C80) the line did not name; census every entry.
- lnk.py needs env.sh sourced in the same Bash call (FABLE_VC71 / FABLE_WINE); without it the link fails on the Windows default path.
- A leaf-owner's one-definition fix (inline + nf88 address helper) leaves the leaf's bytes unchanged; check it with comdiff and the selection with comdat.py, not only the grade.
- A family entry can be mostly done already: compile-probe the whole census before planning a batch (q12377: 25 caller rows, 23 already right; the reloc_bindings witnesses were stale).
- A free-function binder of a protected member is often a compiler-emitted function (here a static atexit destructor): find the TU that emits it (the $E initializer row) before trying a respell.
