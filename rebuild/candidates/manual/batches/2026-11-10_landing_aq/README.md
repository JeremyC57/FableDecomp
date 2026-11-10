# 2026-11-10 landing aq -- agentSND2: sound-manager naming follow-up (94 landed rows re-authored)

Source: `pending/2026-11-10_linux_handSND2/`. All 94 rows `vland --recheck-landed`: 12 MATCH + 82 RELOCATION_MATCH, every test PASS.
- 15 CSoundEmitterLH self rows (vtable 0x0129E6EC slots) re-authored and renamed as `CSoundEmitterLH : CASoundEmitter` overrides on
  the shared headers (FABLE_RETAIL_CASOUNDMANAGER_SIZE40 + the real STLport list); 0x0088F610 / 0x0088F620 renamed to the PDB members
  CGameScriptInterface::IsSoundPlaying / EnableSounds; 0x0088F660 is ?StopSound@CGameScriptInterface@@UBEXK@Z. New self-contained tests.
- 79 rows moved off local sound-manager models onto engine/CASoundManager.h (PSoundManager + slot names). Two left (0x009A6610 needs
  the Reset + LH ctor together; 0x00A0B9A0 has no manager model).
- Header opt-ins (each default path unchanged, hdrcheck2 default=same): CASoundManager.h SIZE40 / BANKLOOKUP / ATMOSMAP, CSoundInit.h,
  CThingSoundEmitter.h. Bank lookups named from the Fable Anniversary 360 PDB publics (same TU order) with the TLC bodies; 0x00991690
  GetPSoundBank(const CLipSyncDataBank&) is descriptive (README in hdr/).
- reloc_bindings.tsv fixed by hand (recheck does not write bindings): 19 stale spellings removed, 20 added (logs/bind_*.txt), no
  conflicting target.
- Parked (differ/): 0x005A94BD CTCInventoryTrade::OpenInventory DIFFER(1488v1482), 0x005ACEDA ConstructActualInventoryPC
  DIFFER(1113v1067), 0x005F6793 CycleAssignedExpressions DIFFER(447v442), 0x005F64DD CycleAssignedAbilities DIFFER(508v508).
