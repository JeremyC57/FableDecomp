# Landing nf298 (Account A, 2026-11-23): T1 "other" class / namespace, NF295's continuation (8 rows, 28 table lines)

Hand-back: `pending/2026-11-23_nf298_handback/`; helpers (proofx.py explicit rename-map objsec, block-bounded flags.sh / srcof.sh) under
`naming_batch/agent_tools/nf298/`. 70 entries reached (1-60 plus 10 pulled ahead by a stale-witness scan, recorded per entry): done 9,
done-already 14, done-conditional 1, done-partial 4, skipped-port 21, skipped-held 21; 465 go to NF301. No header or vtable change.

- **NScript nested allocator classes onto their PDB names (6 quest Main rows):** 0x00D80E40 CHSBMagicBarrier -> CHSB_MagicBarrier;
  0x00D835D0 CHSScreamer -> CHS_Screamer; 0x00D9E5E0 CMCMinion / CMCMinionGuard / CMCSpawnPoint -> CMC_Minion / CMC_MinionGuard /
  CMC_SpawnPoint; 0x00DCC770 (through its ALLOC macro) CWhisperMarker -> CMK_OFI_GWLL_WHIS2, CWhisperFarmRaidIntro -> CM_WhisperFarmRaidIntro;
  0x00E5D4F0 CBodyGuardRespawn -> CM_BodyGuardRespawn; 0x00DBF2D0 CRestartMarker -> CM_RestartMarker. Two sources each: both PDB name sets,
  and the binder's own binding string literal (the PDB class name without its C); each Alloc has one imm32 reference, inside its own Main.
- **0x00AEAC90** `GetNoParticles() const` (PDB QBE; its test took the D1 `cl /Zs`); **0x008F63C0** `private:` before Clear (PDB AAE).
- Proof: objsec ALL_EQ for all 8 (`.drectve`, vtables) under an explicit rename map checked to cover each object's symbol diff exactly.
  Link: the 9 stand-in Allocs and the QAE Clear HEAD left unresolved now resolve, 0 duplicates; GetNoParticles QBE stays unresolved as
  QAE was (owner 0x00AEDCA0 defines a global stand-in, NAME_FIXES).
- **Table:** stale witnesses deleted (0x00EA8830's global CV_MurderTwistScript lines; 0x00AEE8C0, 0x00D6ED10, 0x00B218B0, 0x00E8B9C0;
  entry 426 from objects). The two conditional deletes were settled by the coordinator compiling their uncompiled callers
  (`logs/witnesses_conditional_head.log`): 0x00AEB6D0 / 0x00AEB790 bind the `J_N` GetEmitterTemplate overloads, never the `JJ` stand-in;
  0x00D73970 / 0x00D73C70 bind the NScript-qualified SpawnEnemies, never the global one; both deletes applied. 28 deleted, 9 added.
- Queued T3: 17 landed family rows that define the owners' Alloc PDB symbols as strong same-TU copies (LNK2005 in a full link; pick-any
  scaffolding): 0x00D81C00, 0x00D81C30, 0x00D83F00, 0x00D84470, 0x00D9EB20, 0x00D9EDF0, 0x00D9EE00, 0x00D9F150, 0x00D9F250, 0x00D9FFA0,
  0x00DC0150, 0x00DCF5E0, 0x00DCF5F0, 0x00DCF6B0, 0x00DCF6C0, 0x00E5E090, 0x00E5E0A0. Debt: engine/CQ_MinionCampScript.h's default-branch
  forward declarations without the PDB underscore; `struct` on the nested allocator classes (no emitted symbol either way).

## Recheck
`logs/recheck.log` (landed tree): 8 RELOCATION_MATCH PASS, 0 DIFFER, 0 RELOC_CONFLICT.
