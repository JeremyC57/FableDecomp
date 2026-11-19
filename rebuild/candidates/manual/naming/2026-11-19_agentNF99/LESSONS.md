# NF99 lessons (one line each)
- For a GameCODA row, disassemble the CODAApiDbg body first: its /Od call and store order is the source statement order (ClearHistory's MATCH came from it in one grade), and its out-of-line calls name the inline helpers the release expands (SetQ, IsHistoryClear, MatchToken, UpdateLevel).
- A Dbg .lib member keeps every function in one .text section: locate a function by its symbol value and the next symbol, not by the section (the COMDAT reader from hand2c/tools prints the whole section).
- Relocation offsets in an SDK caller plus `E8` sites in retail give the second source for every callee name at once (call site + 1 - function start = REL32 offset).
- Never name a scratch script after a stdlib module (dis.py broke capstone's import); run helpers with `python3 -I`.
- Take RENAMES section-2 old symbols from compiling the current landed source (objdump -t on the q.py object), not from reloc_bindings.tsv, which lags behind landed respells.
- A local stand-in whose call must move to a shared-header class can keep its own leaf by holding the header class as a member and casting only the argument it cannot yet type; record the self rename (here CFloatReverbEngine::Commit) in NAME_FIXES with the binder that blocks it.
