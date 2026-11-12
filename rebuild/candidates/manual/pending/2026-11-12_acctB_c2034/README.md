# acctB hand-back, chunk 2034 (1 reserved row worked: parked after two agent sessions; the other row not started)

Authored by two successive Opus 5.5 agents; the acctB local coordinator regraded the parked source and ran its test.
Nothing is landed and there is no row to land here.

| address | result | notes |
|---|---|---|
| 0x0097BDA0 CAIStateGroup_ScorpionKing::UpdateCombat | parked, DIFFER 2025v2009 | same call sequence (85 of 85 calls) and control flow as retail; 532 of 624 instructions equal unmasked, 561 with frame offsets masked. 9 grades over two sessions plus about 100 free probes. Moves to the acctB near-miss queue |
| 0x00BB8750 CEnginePrimitiveManagerParticleDecalGroup::RenderLandscapeDecalGroup | not started | re-queued as its own acctB chunk |

Residue of 0x0097BDA0 (retail addresses), as measured by the second agent:

1. **One allocator decision accounts for the size and the frame.** The near-arm piece of `battle_def` (the
   DamageToCauseTailStrike read plus the tail-plunge Terminate release; priority 13, ebx only) takes ebx before the
   near-arm piece of `boss` (priority -15). Retail has `mov ebx,[esp+18h]` (0x0097BFD9) / `mov ecx,ebx` (0x0097C1C9) and
   a near Terminate path of `mov ecx,[esp+10h] ; jmp 0x0097BFA3` (0x0097C1B4); ours keeps a full release-and-return copy
   (+16 bytes) and packs the frame differently. Forcing that one priority in a c2 what-if (diagnostic only) gives 2009
   bytes, retail's frame exactly, and 611 of 624 instructions.
2. Left even then: `fstp st(0)` one slot early (0x0097BE7E); operand order of the near facing dot (0x0097BFFF,
   0x0097C009) and the side dot (0x0097C092); load order at the two tail-strike compares (0x0097C15A, 0x0097C234).
3. The gap in (1) is 28 priority points and spelling tweaks move it by about 10 at most: the agent concludes retail's
   near arm differs structurally (how the tail-strike damage is read, or where the battle_def release sits). About 25
   natural variants are listed as measured and rejected in `NOTES.md`, with ranked next steps.

Coordinator checks (cl 13.10.3077 under Wine, `FABLE_HDR_OVERLAY=hdr/`): the parked source regrades DIFFER(2025v2009);
`rows/parked/0097bda0.test.cpp` prints `0097bda0_TEST PASS` under g++ (VC7.1 pass reported by the first agent; 11
scenarios). Purity grep clean.

Useful now, independent of the row:

- NAME_FIXES.tsv (5): **0x0097C580 is `?UpdatePhase2@CAIStateGroup_ScorpionKing@@IAEXXZ`, landed as SpawnBabies** (Ego_r's
  UpdatePhase2 is 0x60 bytes = the retail extent; its SpawnBabies is a 1-byte `ret`); 0x007AED50
  `CTCBoss::ResetStrikesSinceHitHero`; 0x0097D980 `CAIStateGroupBase::AIAction_PumpCombatActions` (bound to a derived
  class); 0x0097C900 `~CCreatureAction_ScorpionChargeInto`; 0x008C4E20 the ChargeInto constructor takes references and a
  float.
- `hdr/`: new CScorpionKingBattleDef.h and NEntityEvents_CEventCreatureCharging.h, eight new opt-ins (list in
  `hdr/README.md`), on copies of c2026 / c2029 / c2017 headers.
- `LESSONS.md`: a forced-priority what-if proves when one allocator decision is the whole residue; frame-packer weights
  are counted after allocation; frame-masked instruction counts hide x87 operand-order residues.

No retail bytes are in this folder.
