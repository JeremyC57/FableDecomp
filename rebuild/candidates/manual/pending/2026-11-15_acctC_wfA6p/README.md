# acctC hand-back wfA6p (parked rows, Opus pass of 2026-11-15)

Rows: 0x00ba6240 (2937 bytes).

Coordinator-verified RELOCATION_MATCH / test PASS (`coordverify.log`). RULE AUDIT DONE: independent audit found no hard-rule violation. Known warning RELOC_CONFLICT D3DAlphaFuncModeTable (this row binds the table base 0x01396DD8; the landed 0x00BBC460 binding 0x01396DE8 has its index addend folded in: fix that landed binding, see LANDING GUIDE). Review: same-TU callee bodies are the wfB3 translation-unit text (SetupFirstTextures uses the PDB names where landed 0x00B9D670 uses older ones; SetupStaticBlocks 0x00BA50A0 is staged, not landed); two new headers CEngineInternalPrimitiveMorphInfo.h / CEngineInternalPrimitiveMorphedStaticMesh.h. Behaviour tests are standalone models unless stated.
Fitted items and devices are marked in each source header comment; residue history in NOTES.md (grep the address).
