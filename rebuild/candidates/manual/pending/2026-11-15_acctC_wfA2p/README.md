# acctC hand-back wfA2p (parked rows, Opus pass of 2026-11-15)

Rows: 0x00dce230 (2589 bytes), 0x0048e78e (2662 bytes).

Coordinator-verified RELOCATION_MATCH / test PASS (`coordverify.log`). RULE AUDIT DONE: independent audit found no hard-rule violation. Review: 00dce230 has two Inline-budget lines (forceinline on row-local IsBeingCarriedBy/GetPos; inline_depth(0) round the CIsThingAlive ctor) and a local enum EScriptAIPriority; 0048e78e wraps three same-TU callee bodies in auto_inline(off) (landed idiom) and uses optimize("s") with stated evidence; its test links and calls the row. NAME_FIXES filed for SetState 0x00DCEC50 and GetTextWidthFromText 0x0048E062. Behaviour tests are standalone models unless stated.
Fitted items and devices are marked in each source header comment; residue history in NOTES.md (grep the address).
