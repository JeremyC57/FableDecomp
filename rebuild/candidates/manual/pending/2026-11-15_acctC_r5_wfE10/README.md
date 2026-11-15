# acctC hand-back 2026-11-15_acctC_r5_wfE10

Rows: 0x00507c30, 0x007a1b20.

Coordinator-verified RELOCATION_MATCH / test PASS (`coordverify.log`). RULE AUDIT DONE: independent audit found no hard-rule violation. Review: 00507c30 has 3 dead FABLE_RETAIL_ASSERT arms (Inferred; the count is fragile to TU changes), a local CGenericException (no engine header; same model landed at 0x00A0D140), row-local CBasicString Compare/operator== text (retail calls 0x004115A0; same text as the landed 0x00837E40 preamble, not word-for-word the landed 0x00411570 body) and a noinline opt-in on the CArray(unsigned) ctor (retail calls 0x00410F30); 007a1b20 has 1 dead FABLE_RETAIL_ASSERT (Inferred) and the new shared header CSpecialAbilitiesLightningSpellDef.h; label fix AllChildrenInactive 0x0079F730. Behaviour tests are standalone models unless stated.
Fitted items and devices are marked in each source header comment; residue history in NOTES.md (grep the address).
