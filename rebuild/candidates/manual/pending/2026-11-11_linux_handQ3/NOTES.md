# Chunk Q3 notes

**Grade every source here with `FABLE_HDR_OVERLAY=$A/hdr`.** All header changes are behind new `FABLE_RETAIL_*` opt-ins or new files (hdr/README.md, Q3 entries at the end).

## Matched (4, all RELOCATION_MATCH, call targets checked: 0 mismatches against rebuild/oracles/reloc_bindings.tsv)
- 0x00CDD6B0 NScript::CGlobal_TeleportToHeroGuildScript::Main -- the recall tail is shared through `goto done;` (lesson 1).
- 0x0076EB00 CTCWeaponTrail::InitialiseTrails -- `// Flags: /d2inlT65`; new opt-in FABLE_RETAIL_CTCGRAPHICAPPEARANCE_HELPERNAMES.
- 0x008E20F0 NCreatureMode::CModeGuildmasterPowerUpShield::FrameUpdate and 0x008E24F0 NCreatureMode::CModeGuildmasterPowerUpSpeed::FrameUpdate -- new headers
  NCreatureMode_CModeGuildmasterPowerUpShield.h (both modes), CCombatAction_Guildmaster_FollowersRelease.h (both release actions, inline PDB ctors),
  NParticleEngine_CParticleUpdateParameters.h; opt-in FABLE_RETAIL_CTCBOSS_LIVINGHELPERS; plus FABLE_RETAIL_CVECTORMAP_CTOR/INSERT, CTHINGAICREATURE,
  CINTELLIGENTPOINTER_COMPARE_NULL_INT, STLP_SORT_FASTCALL.

## Identities proven (LABELS.tsv; every spelling found verbatim in the PDB symbol dumps)
Row names above plus callees IsResourcesLoaded 0x004BE3E0, HasHelperPoint 0x004BE170, GetAllHelperPointNames 0x004BDA30, GetNoHelperPoints 0x004BDE90,
CountLivingHelpers 0x007AEEC0, CParticleEmitter::GetApproximateTotalSplineLength 0x00AED050, CParticleEmitter::InterpolateTo 0x00AED600.
- 0x008DFB00 is CModeSummonerLightningOrb::FrameUpdate by body (catalog says vfunc_3); 0x00745840 CTCPerceiveThing::vfunc_11 is the debug-draw virtual (name not proven).
- 0x004199C0 is NOT a function start (mid-instruction inside 0x00419880..0x00419C9A); skip it.
- No descriptive (made-up) names introduced.

## Near misses / next steps
- 0x007CCF50 CTCMultiStrike::ApplyEffectOnWeapon DIFFER(1068v1088), 4 grades, source in differ/. Jump table matches now; residue is register choice
  (retail weapon in esi, zero in edi), local slot order of effectsDef/shortEffect/longEffect, a duplicated failure epilogue after the first
  GetEffectFromString (0x007CD031), and a loop kept with a stack counter + edi = i*8. Next: reorder locals, counted do-while, /d2inlT bisect.
  Uses draft-only opt-ins FABLE_RETAIL_CPARTICLEEMITTER_INTERPOLATE and FABLE_RETAIL_CTCDPARTICLEEMITTER_GETPEMITTER_INLINE.
- 0x008F61A0 CHeightMap::Resize DIFFER(518v530), 4 grades (Q2 + Q3); named-locals source in differ/ is the best.
- The other rows were read only or not reached; per-row notes are on line 2 of each differ/ file.

## Header debt / suspicions (not acted on)
- rebuild/oracles/reloc_bindings.tsv names 0x00440F00 as `vector<long>::_M_insert_overflow` in some binding, but the retail call site passes 3 args
  (looks like `_M_fill_insert`). Not verified with a second source, so not in NAME_FIXES.tsv.
- NAME_FIXES.tsv: none.
