# Landing nf408 b4: 50 StatGetClassTypeName rows defined in class, so they link once beside CTCBase::RegisterTCs

RegisterTCs 0x004EE23F emits every registered TC class's `StatGetClassTypeName` as a pick-any COMDAT. These rows defined the same static out of line, which is an LNK2005 duplicate.

NF408 linked the binder against every landed row that mentions the static (463 objects): 272 duplicates, 270 of them against the binder. It re-formed all 270 in the nf86 / nf88 form (ruling nf387; NF407's generator, extended to the namespaced shape), with names, literals, includes, leaves and tests unchanged.

Gates (NF408, checked by script):
- vland HEAD and new: RELOCATION_MATCH PASS on both sides, with the same WARN set.
- objsec: only the leaf's COMDAT selection and one 6-byte helper differ.
- pk: 0 NEQ, 0 reach flips.
- /FORCE link of binder + batch: duplicates go to 0.
- Full census link: 272 -> 2 duplicates (the 2 left are a separate stand-in family, T3 nf408); unresolved 651 -> 651.
- A probe TU proves every static is still emitted.
- Literal mutants fail the linked tests.

Debt: each row still pushes HEAD's cast address instead of the literal. That is grade `baked_string` (owner 2026-11-25), and it is also why the row's copy is not byte-identical to the binder's (R10). For all 270 rows the address holds the class name in retail. Spelling the literals is the follow-up.

Coordinator: 50/50 landed, 0 LITERAL_DIFF, no table or self change. T3 nf291 0x004D5853 was set aside for the landing and restored (it is about binder 0x004DEC09). Baselines OK.
