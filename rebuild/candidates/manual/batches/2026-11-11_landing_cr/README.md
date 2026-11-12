# 2026-11-11 landing cr -- Codex pass 21 return: 26 of 40 large rows (mostly NScript entity Main bodies)

Source: `pending/2026-11-11_codex_pass21/return/` (START_HERE_CLAUDE, STATUS, ROW_DETAILS, LESSONS, LABELS, NAME_FIXES, NEW_BINDINGS,
BOUNDS_FIX, OWNER_CLARIFICATIONS, hdr README, and the 14 parked rows with their sources, tests, PROBES and NAMING_EVIDENCE). The
archive's SHA256SUMS were verified (939 files).

**Landed: 26 rows**
- vland --land: 26 RELOCATION_MATCH, every test PASS.
- Lengths come from BOUNDS_FIX, cut from our own Fable.exe:
  - 0x00D3E2E0 is 9440 bytes: four jump tables, ending before the CSpeedFriend allocator.
  - 0x00AFCC30 is 5274 bytes. The others were trimmed by alignment only.
- The rows: CTCQuestCompletionUI::GetQuestDescription, NThingShot::CTCShotDefinitionBase::OnCollisionProper,
  CTCTargetingPlayer::UpdateTargetList, NParticleEngine CPSCRenderSprite::PreparePrimitives and CPSCEmitterGeneric::DatabaseSerialise,
  and 21 NScript entity Main bodies (BountyHunt, EndGame, GuildTraining x3, HangingTree, NewOakValeIntro x3, PrisonWarden,
  RansomVictim x2, AmbushScam, Fisherman, GuildMaster, Oracle, SickChild x4, SwordInTheStone).
- 0x00AFCC30's first grade was NOT_GENUINE. Its `// Retail-asm:` line cited `AFCCAC/AFD424/...` without the 0x00 prefix and
  named no instruction.
  - The coordinator rewrote that comment line to cite 0x00AFCCAC / 0x00AFD424 / 0x00AFD846 / 0x00AFDCE5 `fistp dword ptr`, which is
    what retail has at each (db 5D / db 1A).
  - It then graded retail_asm (shared GFFloatToLongNear / Floor helpers) and RELOCATION_MATCH.

**Headers**

The 154-file overlay was 3-way merged onto the working tree, with the Pass 21 input include/ as the base (logs/merge.log):
74 clean, 49 new, 25 auto-merged, and 5 conflicts resolved by hand. Each conflict was two additions at the same spot, and both
were kept:
- C3DVector.h: Codex's GFAngles include, plus Q9's MAGNITUDE2D condition.
- CQ_GuildTrainingScript.h: Q10's CKillBird and Codex's CApprenticeSpeedTest.
- CTCDExplosion.h: FAM03's VSLOTS and Codex's Create / SetThingAsImmune.
- NEntityEvents_CEventHitBase.h: FAM05's empty EEventType is kept only without Codex's opt-in, which brings
  NEntityEvents_EEventType.h and its own CEventBase.
- stlport/fable_containers.h: NF19's DESTROY_AUX_FASTCALL_DECL wraps Codex's CTCTARGETINGPLAYER_UPDATE_LIST variants.

hdrcheck2 (logs/hdrcheck.txt) reports default=same, except two files whose only change is cosmetic:
- CParticleSystem.h: one line split in two, and __LINE__-numbered static-assert names.
- GFAngles.h: its preprocessed default output is token-identical.

**Names**
- Labels for all 40 rows: ghidra_out/labels_landing_2026-11-11cr.tsv.
- 106 NAME_FIXES and 5 NEW_BINDINGS are queued as land1011cr. They explain vland's 22 RELOC_NAME warnings, for example
  0x0129BA3C `?DELTA_NOTIONAL_ZERO@@3MB` (landed as g_NormaliseEpsilon) and 0x00D1CAC0 WatchForLeaderChargeGruntsKilled.
- Codex's naming qualifications are in its START_HERE: the anonymous-namespace hash of 0x00804020, the local-static ordinals of
  0x00DB5330 / 0x00DBB310, and one local renamed in the parked 0x00B0DC20.

**Parked (14)**

return/parked/<addr>/ holds each row's best source, test, PROBES and NAMING_EVIDENCE. The narrowest are 0x00B24850, 0x00A1E5C0 and
0x00975BB0.
