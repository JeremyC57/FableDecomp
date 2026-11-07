# Chunk 849 lessons (GOTCHAS one-liners)

- 2026-11-07 chunk 849: held Codex pass 12/13 rebases land on first grade once the class models are supplied as opt-ins (0x0058E99F, 0x009CF3A0); the rest needed one to three same-TU visibility levers each.
- 2026-11-07 chunk 849: an out-of-line filter `operator()` (declared only, retail calls it) stops VC7.1 hoisting the filter temporary's store above an enclosing loop (0x00501450: 235 -> 92 lines; FABLE_RETAIL_CTHINGFILTER_HASTC_OUTOFLINE).
- 2026-11-07 chunk 849: a visible STLport `_tree.c` insert_unique body (FABLE_RETAIL_STLP_MAP) changes frame-slot sharing for the insert pair temporary even when retail calls insert_unique (0x00501450: 92 -> 8 lines).
- 2026-11-07 chunk 849: a two-local spill-slot swap that declaration order and const do not move went away with a named reference local for the intermediate object in a getter chain (`CPlayer& player = ...GetMainPlayer(); player.GetPControlledCreature()`); the PDB locals list need not show it.
- 2026-11-07 chunk 849: STLport `list::size()` must be `distance(begin(), end())` (const-ref distance walking a copy) for retail's head-in-eax / walk-in-ecx; the for-loop count swaps the registers (FABLE_RETAIL_STLP_LIST_SIZE_DISTANCE, 0x0057E2F1).
- 2026-11-07 chunk 849: a CArray push_back that retail sends straight to `_M_insert_overflow` wants the inline fast-path push_back (FABLE_RETAIL_CARRAY_PUSHBACK_FASTPATH); a forwarding or plain push_back inlines the overflow path (0x00AE2E20: 1389 v 1293 instructions).
- 2026-11-07 chunk 849: a same-TU `__declspec(noinline)` body for a called CArray(n) ctor / GetDef<T> template fixes caller register residue that a declared-only one leaves (0x00AE2E20, 0x0041D21B).
- 2026-11-07 chunk 849: a UI factory's operator new sizes come from full `*_MEMBERS` opt-ins (Ego_r members 0x30 lower, +4 per vector); grade after sizes before chasing codegen.
- 2026-11-07 chunk 849: run ferr.py with FABLE_HDR_OVERLAY exported, or a header that only exists in hdr/ is missing and cl.exe hangs instead of erroring.
- 2026-11-07 chunk 849: byte-identical COMDAT twins (0x0051AC70 / 0x00AE76E0 `__destroy_aux<set<unsigned long>*>`) give vland RELOC_CONFLICT for a correct spelling; report it in NAME_FIXES, do not invent a second name.
