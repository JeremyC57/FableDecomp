# NF219 lessons (one line each)
- A free-function stand-in binding a member through a "thunk" can be the ONE out-of-line copy of an inline destructor: check who reaches the thunk (callers.py found an EH unwind funclet) before naming it a forwarder (0x00C3CF30 = ~CircularQueue).
- The parent-vs-funclet test settles a destructor: a parent that calls Release directly while its unwind funclet calls a 5-byte `jmp Release` means an inline `~T() { Release(); }`; one compile with the member modelled that way proves both addresses.
- Swapping a local struct for a sibling model, compare sizes: the sibling CircularQueue carries 0xC stats bytes the old model kept as a separate pad (BufferState[0xc]); drop the pad or every later member moves.
- A static's atexit-destructor row can declare the static's companion data extern (the live-string count) and still grade: only the static itself must be defined twice (the rule's known LNK2005).
- Ego_d.pdb PUB32 (egod_carved_names.tsv) gives static data access digits the initializer entries do not (?NullString@CCharString@@2V1@A public; ?NoStrings@CCharString@@1JA protected).
- A header-inline member's owner row ports with the nf88 helper in one form when a landed sibling TU names the right opt-in mix (0x0099B4D0 = 0x0099BA70's CWIDESTRING + SPRINTF_ORIGINAL).
- Run callers.py on a family's addresses before planning: q12226-q12229 joins the 0x004291DE CComponent conflict (576 call sites), far beyond one slice.
