# acctC hand-back 2026-11-15_acctC_r5_wfA4b

Rows: 0x00b5a3f0, 0x00bb2540.

Coordinator-verified RELOCATION_MATCH / test PASS (`coordverify.log`). RULE AUDIT DONE: independent audit found no hard-rule violation. No flags or devices. 00b5a3f0 grades retail_asm through the shared GFFloatToLongNear.h helper (Retail-asm line cites 0x00B5A70F fld / 0x00B5A713 fistp). Review: 00bb2540 defines in-row FABLE_FORCEINLINE bodies that retail inlines (SelectVertexBuffer, both DrawBuffersIndexed overloads, GetDynamicVB*/IB* accessors), duplicating shared-header opt-in text (hygiene: prefer the header opt-ins at landing); the 9-arg body uses the PDB name AttachTextureToStage where landed 0x00BA7DB0 uses SetTexture; new overlay header CEngineParticleSimulationRenderData.h. Behaviour tests are standalone models unless stated.
Fitted items and devices are marked in each source header comment; residue history in NOTES.md (grep the address).
