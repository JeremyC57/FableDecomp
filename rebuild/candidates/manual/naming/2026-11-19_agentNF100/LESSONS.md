# NF100 lessons (one line each)
- Count binders of a data symbol from the exe, not from reloc_bindings: one absolute-reference scan (absrefs.py) over the code sections found 77 wrong binders of 0x0137DAD8 where the queue said ~35 (reloc_bindings keeps one binder per symbol).
- A shared header that would clash with a row's pre-existing local model can still serve one declaration: a whole-header `#ifndef FOO_ONLY` (on line 1, as a prefix to its existing comment, so no assert moves) plus an `#ifdef FOO_ONLY` block at the end with just that declaration; this moved 56 rows in two payloads.
- Before porting a slot call onto a CAIStateGroup header, check the slot offset in the first DIFFER: AIAction_MoveToPosAndFacePos lands at +0x38 unless FABLE_RETAIL_CAISTATEGROUP_ENEMYCREATUREBASE_VSLOTS supplies slots 14..21.
- A same-TU strong callee copy (nf96a) fixes "retail reuses ecx after the call" rows in one grade (0x005C150B); once its own leaf changes, port its predicate-instantiation binders in the same pass.
- Ego_d.pdb local names are readable straight from the PDB bytes (`dummy_bool`, `category_it`, `new_pos`): grep the S_GPROC32 name and read the next strings.
- An uninitialised out-flag `bool` homed in the high byte of a live parameter slot (`lea edx,[ebp+0Bh]`) does not come from a plain named local under VC7.1 (3 forms): record it rather than burn the budget.
