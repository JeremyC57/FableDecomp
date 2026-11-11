# 2026-11-11 landing cb -- chunk Q2 (Fable 5.2 fast-authoring lane): 5 never-authored rows

Source: `pending/2026-11-11_linux_handQ2/`. vland --land: 5 WINS (1 MATCH, 4 RELOCATION_MATCH), tests PASS.
- 0x004C84B0 CThing::SaveBinary (catalog "CThing::Serialise" was a propagated label; retail writes the game-state flag twice and drops
  IsLevelPersistent()'s result), 0x004C9120 CThing::Save, 0x0052F1B0 NUISystem::CComponent::IsZoomIndependent (true extent 8 bytes),
  0x007E6750 CTCOracleMinigame::FrameUpdate (832 bytes with jump table), 0x008E0A10 CModeScorpionKingVulnerable::FrameUpdate.
- New header engine/NCreatureMode_CModeScorpionKingVulnerable.h; 13 opt-ins on existing headers (hdr README), default paths unchanged.
- Extents into BOUNDS_FIX.tsv; name fixes queued as land1011cc (OracleSymbolSize / PopFront / PopBack, CDataOutputStream::Write overload,
  CPersistTraits<signed char>, CThingManager::IsSerialisingGameState).
