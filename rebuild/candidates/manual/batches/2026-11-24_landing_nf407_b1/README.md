# Landing nf407 b1: StatGetClassTypeName defined in class, so it links once beside CTCBase::RegisterTCs

RegisterTCs 0x004EE23F emits every registered TC class's `StatGetClassTypeName` as a pick-any COMDAT. A row that defines the same static out of line therefore gives an LNK2005 duplicate.
- b1 fixes the regression nf402_b1 introduced. Before nf402, these five rows clashed only with each other: 2 duplicates through a shared stand-in, 0 against the binder. At HEAD they had 5 duplicates against the binder.
- b2 fixes five older rows from NF406's unmeasured list.

The form is the nf406_b2 / 0x004D31C0 one (nf86 / nf88, ruling nf387): the static is defined in the class body, and an anonymous-namespace helper marked scaffolding takes its address. Names, literals, includes, leaves and tests are unchanged.

NF407 gates:
- vland: RELOCATION_MATCH PASS on both sides, with the same WARN set.
- objsec: only the leaf's COMDAT selection (1 -> 2) and the one 6-byte helper differ.
- pk: 0 NEQ, 0 reach flips.
- /FORCE link with the binder: 5 -> 0 per batch. Linking everything (binder, the 10 rows, 0x004D31C0 and the nf406_b1 callers) gives 10 -> 0, with nothing newly unresolved.

Debt (T3 nf407):
- 0x004D4A86 keeps a cast literal, so its copy is not byte-identical to the binder's. Both copies match retail; this is R10 debt.
- The tests of 0x004D4249, 0x004D429B, 0x004D430F and 0x004D5FDB are self-contained.
- At least 250 more family rows clash the same way at HEAD. NF408 is re-forming them.

Coordinator: 5/5 landed, 0 LITERAL_DIFF, no table or self change, baselines OK.
