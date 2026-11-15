# acctC hand-back wfB3p (parked rows, Opus pass of 2026-11-15)

Rows: 0x00ba2350 (9632 bytes) 0x00ba5260 (4056 bytes).

Coordinator-verified RELOCATION_MATCH / test PASS (`coordverify.log`). RULE AUDIT DONE: independent audit found no hard-rule violation. OWNER REVIEW: 18 identical dead FABLE_RETAIL_ASSERT(material) lines in 00ba2350 are a fitted count (only 18 matches in a 0..26 sweep; the debug build suggests about 3); about 20 same-TU helper definitions carry FABLE_FORCEINLINE and the Inline-budget marker names only 5 of them; the two rows are one translation unit and should land together; the Retail-asm line cites the shared GFFloatToLongFloor.h helper. Same-TU callee bodies each grade RELOCATION_MATCH against their own retail rows. Behaviour tests are standalone models unless stated.
Fitted items and devices are marked in each source header comment; residue history in NOTES.md (grep the address).
