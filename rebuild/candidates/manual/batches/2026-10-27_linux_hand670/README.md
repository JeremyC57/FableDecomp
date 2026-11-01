# 2026-10-27_linux_hand670 — chunk 670 (NA015, never-attempted pool)

Landed 2026-11-01 via `land_chunk.sh 670`: 10/10 RELOCATION_MATCH, bindings multi-check 0. Ten destructors; the two duplicated catalog labels
(~C3DMeshLODInfo, ~CFontDataBankEntry) resolved by the vtable each row stores + RTTI COL chain: 6 of 10 labels were wrong.
NAME_FIXES.tsv (29 rows) queued as c670. Descriptive (address-suffixed) names kept where neither PDB names the member: CSoundBankLH::Unload_00a0b6c0,
CASoundManager::ClearBanks_009918f0, PFrameMemoryPool_0143bac4 (type proven CSimpleMemoryPool*). CSoundBankLH / CSoundManagerLH are in neither PDB; names from RTTI.
Open (NOTES.md): CSoundBankLH ctor 0x00A0BAC0 parameter is a sound-manager pointer (CASoundManager* vs CSoundManagerLH* unresolved); header debt for ten flattened engine headers.
Decision (coordinator under owner delegation): the empty-bodied PDB ctor ??0CEngineBillboardGenerator exists only to emit the implicit dtor (chunks 664/665/667 practice).
