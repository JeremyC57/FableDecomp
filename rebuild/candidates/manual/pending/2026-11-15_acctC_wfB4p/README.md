# acctC hand-back wfB4p (parked rows, Opus pass of 2026-11-15)

Rows: 0x00b8b780 (3921 bytes).

Coordinator-verified RELOCATION_MATCH / test PASS (`coordverify.log`). RULE AUDIT DONE: independent audit found no hard-rule violation. Known warning RELOC_CONFLICT D3DAlphaFuncModeTable (this row binds the table base 0x01396DD8; the landed 0x00BBC460 binding 0x01396DE8 has its index addend folded in: fix that landed binding, see LANDING GUIDE). OWNER REVIEW: four dead FABLE_RETAIL_ASSERT arms are a fitted count (the debug build suggests one); /GS flag tied to the retail frame; Retail-asm fistp through the shared GFFloatToLongFloor.h helper; local CEngineBillboard has no engine header; label AttachZBuffer 0x00A10E90 looks stale. Behaviour tests are standalone models unless stated.
Fitted items and devices are marked in each source header comment; residue history in NOTES.md (grep the address).
