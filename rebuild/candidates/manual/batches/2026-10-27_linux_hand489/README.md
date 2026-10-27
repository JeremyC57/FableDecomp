# Chunk 489 (agent489, 2026-10-27): 39 of 40 single functions byte-exact

Fifth queued 40-row chunk of single functions under 128 bytes (address order). One Opus 5.5 agent at high effort.

- **Landed:** 39 rows with `vland.py --land` (verify.log: 11 MATCH, 28 RELOCATION_MATCH, every test PASS, no
  RELOC_IDENTITY). Nothing parked.
- **Skipped (1):** 0x007891F2 packs two functions (the CTCTrapBase::BulletTimeFrameUpdate thunk `jmp [eax+24h]` and,
  at 0x007891F7, the `ret` that is CTCTrapBase::InitTrap's base body, slot 28); the catalog row needs splitting.
- **Identities:** 25 proven (ghidra_out/labels_chunk489_2026-10-27.tsv), 11 probable, 3 descriptive (NOTES.md).

Label corrections (NOTES.md): 0x007899D0 / 0x00789A60 / 0x00789A80 are CTCHeroRangedAttacks::AttemptToLoadRangedWeapon /
GetShotBuildupTime / GetShotAccuracyFraction; 0x00787BF1 / 0x00787BFE are CTCTrapBase::Reset / ManualDeactivate
(landed 0x00787C0C calls 0x00787BF1 as an extern "C" `Cleanup`); 0x0079A4D0 / 0x007D1AA0 (labelled Catch@0047e8cc)
are ~CTCShopKeeper / ~CTCTavernGameCoinBase; 0x005EDA50 is ~CTCTavernGame; 0x0080E480 is
CThingFilter_IsAffectedByExplosions; landed 0x007BED90 is SetDieInSeconds and landed 0x007A0D80 OnRemove; the
GetTCDriver<CTCDExperienceOrb> labels on 0x00617D30 / 0x006AE5C0 / 0x007512C0 are shape-copied (different bits).

Review at landing: 0x0049C770 is called PeekAllThings here (as in landed 0x0052B270) but labelled PeekClassList in the
catalog; engine/CTCTrapBase.h models the vptr as data (header debt list in NOTES.md).
