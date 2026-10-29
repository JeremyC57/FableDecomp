# Chunk 573 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- Quest-script `Main` rows (bindings + spawned threads + one wait loop) match on the first probe from the landed
  CV_RockTrollFirstEncounterScript::Main 0x00EC3D40 model, conditional-destruction flag bits included (0x00E9C4B0, 0x00EE6E90).
  Name them from the quest vtable (slot 2 = Main; slots 1/3 are usually landed RegisterMain/Init) plus the Ego_r name.
- The verifier skips string-literal relocations (`$SG` / `??_C` in reloc_identity.SKIP_PREFIXES): pair each literal with the
  retail address at the same offset and compare the bytes (a 60-line COFF reader does it), especially for SetQuestCardObjective-
  style calls with several string arguments (chunk 573 `rel.py`).
- A compile-only probe sweep (verify_and_land.parity_of in a private work dir, normalised capstone diff with
  `SequenceMatcher(autojunk=False)`) tries dozens of statement forms without spending grades (0x00E29B20, ~30 forms).
- Inline CTimer dtor exits: VC7.1 emits two `DeregisterTimer + ret` copies distinguished only by the register that carries the
  vtable (eax vs edx); which copy stays at its source position is the open residue of 0x00E29B20 -- no loop/if/scope form,
  accessor form or the QFE compiler moved it.

Review at landing:
- 0x00E29B20 parked (differ/00e29b20.cpp, 856v846); more than 4 grade-equivalents spent across the two agents (mostly
  compile-only probes).
- Landed 0x00EE71A0 CV_TrophyDealerScript::HilightGuildTeleporter is declared outside NScript (global `struct`), and it binds
  0x00CB7940 as `?IsActiveThreadTerminating@CV_TrophyDealerScript@@QAE_NXZ` (NAME_FIXES.tsv).
- 0x0143E8F8 carries several bound spellings (g_pThingManager_0143e8f8 as CThingManager_Calls / CTimerService, g_timerMgr_cd4470,
  g_pScriptTimerManager, g_pThingManager, g_pGameScriptInterface as GameCalls, g_pScriptTimers, g_Game); retail uses it as the
  CGameScriptInterface* (timer slots 87-90). No PDB data symbol exists, so the name stays descriptive; the
  `?g_pGameScriptInterface@@3PAVCGameScriptInterface@@A` spelling is used here.
- Catalog: vtable 0x012DF424 slot 4 0x00E05330 is labelled `PersistTransfer_BrainState`; Ego_r has no CDarkwoodTrader::OnPersist,
  so check the slot's real owner before naming it.
