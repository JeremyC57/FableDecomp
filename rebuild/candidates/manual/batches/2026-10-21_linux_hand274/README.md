# Hand chunk 274 (CThingPlayerCreature .. CWaterPatchMesh): 31 of 40 byte-exact

## What matched (31)
- CUserProfileManager (3):
  - 00407e10 GetSaveDisplayNameFromFileName maps "AutoSave" and manual-save names to their display names. Shape: explicit `CCharString("")` temporaries placed where retail builds them, and one trailing return per exit.
  - 00408120 GetPathNameFromFileName puts a save-extension name under the profile directory, and anything else under the save game's path. Shape: `/GS` (the frame is 0x21c, with a MAX_PATH buffer).
  - 00408d90 GetAssignedInputForMovementAction does a passive/aggressive binding search over 28-byte entries. Shape: two written-out loops, `rep movsd` for the struct copy.
- CThingShotDef 00458d6f and CVillagePeopleDef 0046a80e Transfer: straight-line CPersistContext::Transfer chains. Shape: the right Transfer overload for each member type.
- CVertexGroupBlockOptimiser (3), CVertexGroupBlockList layout given in a `// Layout:` comment (12-byte STLport vectors):
  - 00ad9b20 CalcCurrentBlockSegments. Shape: tag/allocator types with user constructors, so that empty value-initialised temporaries emit no byte stores.
  - 00ad9d70 CalcCurrentBestTheoreticalResult. Shape: `fullBlocks = (groupCount-1)/(MaxN-1); remainder = groupCount - fullBlocks*(MaxN-1);` written inline, with no named per-block local.
  - 00ada240 Process. Shapes:
    - the STLport list insert with a named `prev` local;
    - ~CVertexGroupBlock declared out of line (0xA96DA0);
    - `unsigned long maxN = MaxN;` before the OptimalSize division.
- CWaterPatchMesh 00beaca0 Render(long) const. Shape: a named camera local with a per-field Position copy, and a named vertexBuffer local.
- 00c9cd20 libpk stream-decoder init (the catalog says CVsyncCallbackTimerInfo::Initialize). Shape: a plain C body with calloc buffers and a fallback one-entry buffer.
- CTriangleStripifier2 00f3f3a0 GenerateTriangleListAndStrip(CArray<C3DTriangle2>...) flattens triangles to indices, calls the index-list overload 0xF3EA50 and rebuilds the list. Shape: hoisted base-pointer locals (`unsigned short* index = &indices[0];`) for the ushort write loops.
- Quest scripts:
  - 00d8ee50 CQ_HobbeCaveScript::WatchForHavingKey.
  - 00e0de90 CUndeadRisingZombieAttacker::OnPredicateFail.
  - 00e36590 CV_ArcheryCompetitionScript::DoTimesUp.
  - 00e3a150 CV_BordelloScript::WatchForDeedStatus.
  - 00e44cc0 CV_BordelloScript::IsHeroWearingTash.
  - 00e4df20 CTollBandit::Should_I_Flee.
  - 00e538f0 CV_BeardyBaldyScript::IsHeroWearingAnyTash.
  - 00e53ad0 CV_BeardyBaldyScript::IsHeroWearingAnyOddHairdo.
  - 00e5d300 CV_BodyGuardScript::OnPersist.
  - 00e6a340 CV_ChapelOfEvilScript::WatchForWeaponUnsheathed.
  - 00e76ca0 CV_ExposeMayorScript::CheckLampSlot.
  - 00e7e370 CV_FishingCompetitionScript::Main.
  - 00e86e60 CV_FisticuffsClubScript::FisticuffsChallenge.
  - 00e953e0 CV_HauntedHouseScript::CheckRegionLeaving.
  - 00e9fcd0 CV_KnotholeGladeGatesScript::Main.
  - 00ea1ec0 CV_KnotholeGladeGatesScript::DoPanicBanter.
  - 00ea7460 CV_LostTraderScript::WatchForQuestFailureState.
  - 00edc010 CV_TempleOfLightScript::WatchForWeaponUnsheathed.
  - 00ee6850 CV_TourGuideScript::MoveToNextWaypoint.
  - 00ee71a0 CV_TrophyDealerScript::HilightGuildTeleporter.

  Shapes:
  - Bit-flag temporary dtors for the `||` chains of CCharString temporaries (the OddHairdo/Tash rows).
  - `__forceinline` virtual getters.
  - A named local for an early global read.
  - Write every exit out in full.
  - For 00e6a340/00edc010: `if (!IATT()) { do { ... } while (!IATT()); }` gives the single inline-dtor expansion at the loop exit. This came from the advisor (v2).

## Parked for Fable 5.1 (9)
- 006ad3c0 CThingPlayerCreature::ApplyRelativeMovementAcceleration DIFFER(394v447): retail has `sub esp,0x68` with unpacked C3DVector temporaries, `this` in edi, and `push ebp` only around the camera block; ours packs the frame to 0x1c and holds acceleration in ebp | tried: GetInterface lookup scoped inline vs in a block, a const-ref acceleration local, named direction/scaled temporaries (5 variants) | next: lane gap: frame packing/register allocation.
- 00ad9980 CVertexGroupBlockList::FilterBySubset DIFFER(415v416): retail keeps the filtered finish pointer in ebx across both loops and homes the inner index in the dead argument slot; ours keeps finish in memory and puts the index in ebp | tried: tag/allocator user ctors, local order (filtered before available), loop-index declaration scope (5 variants) | next: lane gap: locals in dead argument slots.
- 00e36390 CV_ArcheryCompetitionScript::WatchForAttack DIFFER(480v502): retail calls the out-of-line ~CScriptThing at the attacked return and expands it inline at the other exits | tried: the do-while wrapper and the 00e53d90 family probes (advisor) | next: lane gap: per-site inline decision (VC7.1 decides function-wide).
- 00e53d90 CV_BeardyBaldyScript::WatchForAttack DIFFER(442v464): the same dtor site band as 00e36390 (an out-of-line call at the in-loop attacked return, inline elsewhere) | tried: 15+ probes with the advisor, including the do-while wrapper | next: lane gap: per-site inline decision.
- 00e6ad20 CV_ChapelOfEvilScript::WatchForAcolyteDeath DIFFER(513v505): retail has `push edi; xor edi,edi`, a zero register for the inline ~CScriptThing compares; ours compares against an immediate 0 | tried: `PImp.Data != 0 && ...MsgIsKilledBy`, a bool local, the do-while wrapper (497) | next: lane gap: zero-register caching.
- 00edc980 CV_TempleOfLightScript::WatchForAcolyteDeath DIFFER(513v505): a byte-identical twin of 00e6ad20 (same missing edi zero register) | tried: same as 00e6ad20 | next: lane gap: zero-register caching.
- 00ebf1e0 CV_ParanoidWhispersScript::CheckForNecklaceAndQuit DIFFER(434v432): retail hoists both the global and its vtable load (`mov edx,[ecx]`) above the in-loop term `jne`; ours hoists only the global | tried: timer.SetTime vs a direct SetTimer call, a named bool condition, the plain && condition | next: lane gap: scheduling.
- 00ed9790 CV_TalentlessBardScript::WatchForAttack DIFFER(413v453): retail expands the inline ~CScriptThing at the two non-loop exits with an edi zero register, and calls it out of line at the in-loop returns | tried: a named bool `hit` vs the inline ||, the do-while wrapper, the advisor's family findings | next: lane gap: per-site inline decision.
- 00ee6a40 CV_TourGuideScript::WatchForNoFollowers DIFFER(303v441): retail expands the vector<CScriptThing> dtor inline at four exits (out of line 0x8AC970 at one) and splits the term re-check tail in two; ours inlines it once and merges the break paths | tried: a static Destroy helper, for(;;) vs while loop, a named last local, return vs break exits | next: lane gap: per-site inline decision.

## Skipped
None.

## Label corrections

| Address | Catalog / manifest label | Correction |
|---|---|---|
| 00e538f0 | CV_BordelloScript::IsHeroWearingTash | CV_BeardyBaldyScript::IsHeroWearingAnyTash |
| 00d8ee50 | CV_TrophyDealerScript::HilightGuildTeleporter | CQ_HobbeCaveScript::WatchForHavingKey |
| 00e953e0 | CV_ExposeMayorScript::DoMission | CV_HauntedHouseScript::CheckRegionLeaving |
| 00e6a340 | CV_ChapelOfEvilScript, catalog OK | twin is 00edc010 |
| 00edc010 | CV_ChapelOfEvilScript::WatchForWeaponUnsheathed | CV_TempleOfLightScript::WatchForWeaponUnsheathed |
| 00e6ad20 | CV_TempleOfLightScript::WatchForAcolyteDeath | CV_ChapelOfEvilScript::WatchForAcolyteDeath |
| 00c9cd20 | CVsyncCallbackTimerInfo::Initialize | libpk stream-decoder init (InitStreamDecoder) |
| 00ad9980 | CVertexGroupBlock::FilterBySubset | CVertexGroupBlockList::FilterBySubset |
| 00ad9b20 | Optimise | CVertexGroupBlockOptimiser::CalcCurrentBlockSegments |
| 00ad9d70 | CountOptimisedBlocks | CVertexGroupBlockOptimiser::CalcCurrentBestTheoreticalResult |
| 00ada240 | AddBlocksFromOptimisation | CVertexGroupBlockOptimiser::Process(CVertexGroupBlockList&, bool) |
| 0xF3EA50 | CTriangleStripifier2::GenerateStrips | GenerateTriangleListAndStrip(CArray<ushort>&, CArray<ushort>&, CArray<ushort>&), the index-list overload |
| 0xA96DA0 | FinishWithDynamicTextures | CVertexGroupBlock::~CVertexGroupBlock |
| 0xE9FEC0 | CMeleeApprentice::Init | CV_KnotholeGladeGatesScript reset of the mission state |
| 0x99AF20 | _Getcont | CWideString::Contains |
| 0x99B2C0 | ConvertToCharString | CWideString::ToCharString |
| 0x8AC970 | (CConversation) | vector<CScriptThing> dtor |
