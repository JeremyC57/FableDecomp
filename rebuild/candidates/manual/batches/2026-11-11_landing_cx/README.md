# 2026-11-11 landing cx -- naming agent NF22: Codex pass 21's name-fix queue, 278 landed rows respelled, 27 renamed

Source: `naming/2026-11-11_agentNF22/` (STATUS, APPLIED, REJECTED, RENAMES, NOTES, LESSONS, QUEUE, hdr README).
- The queue had 115 entries (land1011cr): 45 done, 70 partial, 0 rejected.
- Most partial entries stop at binders that go through stand-in classes or free functions; finishing them needs new local models.
- 0.0f (6 rows) and ConstantFPS (21 rows) keep their aliases where the literal or inline accessor changes the schedule.

**Verification**
- vland --recheck-landed ran in two parts: all 278 grade MATCH or RELOCATION_MATCH, with no HARD finding.
- Running every test exe found its catalog PassPattern in 278 of 278.
  - 0x0099BF30's catalog test embedded the old source. It was re-embedded with the respelled row, and the test now defines and
    reads `CWideString::NoStrings`. It was rechecked on its own (logs/vland_recheck_0099bf30.log).
  - The recheck helper first picked up an orphan test, `CWideString_AddWideStringAndLiteral_0099bf30_test.cpp`, which is not in
    the catalog. Seven addresses have such duplicate test files (hygiene debt).
- Bindings were re-derived. bindings_multi_check is at its NullCoord baseline.
- NF22 copied its rows from fae6c54be. None of the 278 changed since, and its six headers are supersets of HEAD (default=same).

**Changes**
- 27 self-row renames or signature fixes (RENAMES_applied.tsv). Among them: CThingNoise::SetCreator, CalculateNavigableWidthOfLine,
  CTCFlammable::SetOnFire, PeekEmitterTransform, GivenTeddy, CThing::GetTCDriver<CTCDParticleEmitter> / <CTCDExplosion>,
  CPlayer::GetCurrentMode / IsFirstPersonTargeting, CCombatManager::DeregisterCombatant, and ST_HEAD_TURN_H / _V (the queue's
  `@@A` corrected to anniv360.pdb's const `@@3VCCharString@@B`).
- 97 caller respellings. Among them: DELTA_NOTIONAL_ZERO, GPInputManager (31 rows), the float literals, GPDisplayManager,
  GPLightingManager, NoStrings, GFastCosTable, AddTC and pDSTGame.

**Headers (opt-ins only)**
- fable_string.h: CWIDESTRING_NOSTRINGS and CCHARSTRINGDATA_FRIEND.
- CTCFlammable.h: FIREPOINTS.
- CParticleSystem.h and CParticleEmitter.h: PEEKEMITTERTRANSFORM.
- CPlayer.h: PLAYER_MODES_LIST.
- CCombatManager.h: PERFORMSEQUENCERESET.

**Follow-ups**
- Queued as land1011cx: 0x00749880 is CTCNoise::SetCreator. Its self row is still the `_unattributed::AssignTarget_00749880`
  stand-in.
- 0x006ED9C0 is still spelled DeregisterCombatant(CThingCreatureBase*). Both PDBs have only the reference overload at 0x006EA430,
  so 0x006ED9C0 is something else (unproven).
- Also in NOTES.md:
  - 0x00445BA0 and 0x00445BC0 are probably IsToggleSneak and IsRequireAttackButtonHeldForFlourish (unverified).
  - 0x0099BDB0 has two names: the label says SetSubstring, the header says GetSubString.
  - Header debt: CTCFlammable.h is not standalone, CQ_SunnyvaleMasterData.h statics, and a stale CEngineLightingManager.h comment.

Labels: ghidra_out/labels_landing_2026-11-11cx.tsv.
