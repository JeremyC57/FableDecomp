# 2026-11-11 landing ce -- chunk Q3 (fast-authoring lane, Opus 5.5): 4 never-authored rows

Source: `pending/2026-11-11_linux_handQ3/`. vland --land: 4 wins (4 RELOCATION_MATCH), tests PASS.
- 0x00CDD6B0 NScript::CGlobal_TeleportToHeroGuildScript::Main: the shared recall tail is a `goto done;`.
- 0x0076EB00 CTCWeaponTrail::InitialiseTrails: `// Flags: /d2inlT65`.
- 0x008E20F0 NCreatureMode::CModeGuildmasterPowerUpShield::FrameUpdate.
- 0x008E24F0 NCreatureMode::CModeGuildmasterPowerUpSpeed::FrameUpdate: the Shield body plus a boss GetTC right after `--Timer`.
  The manifest placeholder `vfunc_3` was renamed to FrameUpdate, the Ego_r.pdb spelling.

Headers:
- New: engine/NCreatureMode_CModeGuildmasterPowerUpShield.h, engine/CCombatAction_Guildmaster_FollowersRelease.h, engine/NParticleEngine_CParticleUpdateParameters.h, engine/CHeightMap.h.
- New opt-ins: FABLE_RETAIL_CTCGRAPHICAPPEARANCE_HELPERNAMES and FABLE_RETAIL_CTCBOSS_LIVINGHELPERS. Two more (FABLE_RETAIL_CPARTICLEEMITTER_INTERPOLATE and FABLE_RETAIL_CTCDPARTICLEEMITTER_GETPEMITTER_INLINE) are used only by the 0x007CCF50 draft.
- hdrcheck2 reports default=same for every changed header.

Names:
- Labels: ghidra_out/labels_landing_2026-11-11ce.tsv (12 entries, PDB spellings).
- Name fix queued as land1011ce: 0x007AEEC0 is ?CountLivingHelpers@CTCBoss@@QAEJXZ in both Ego_r.pdb and Ego_d.pdb. The landed row and three label files say CountLivingHelperCreatures.
- vland warned RELOC_NAME at 0x0129BA3C (`DELTA_NOTIONAL_ZERO` against the old catalog label `EPSILON`). Naming wave 18 and landing 08h already settled it as DELTA_NOTIONAL_ZERO.
