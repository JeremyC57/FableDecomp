# acctC hand-back 2026-11-15_acctC_r5_wfE3

Rows: 0x00e4b5b0.

Coordinator-verified RELOCATION_MATCH / test PASS (`coordverify.log`). RULE AUDIT DONE: independent audit found no hard-rule violation. Two independent audits, no flags or devices. Landing fix: the row-local enum EScriptEntityMoveType uses MOVE_TYPE_WALKING / MOVE_TYPE_RUNNING where rebuild/include/engine/EScriptEntityMoveType.h has the PDB spellings ENTITY_MOVE_WALK / ENTITY_MOVE_RUN (bytes unaffected; 4 landed rows carry the same local form): respell at landing. NAME_FIXES for SetUpBanditToll 0x00E4E0E0. Behaviour tests are standalone models unless stated.
Fitted items and devices are marked in each source header comment; residue history in NOTES.md (grep the address).
