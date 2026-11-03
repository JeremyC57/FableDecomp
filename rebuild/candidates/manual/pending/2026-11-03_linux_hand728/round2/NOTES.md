# Chunk 728 round 2 — CTCWife (0x006D0830 GlobalUpdate, 0x006D1E30 FrameUpdate, 0x006D2BD0 OnSerialise)

## Results
| row | grade | attempts | file |
|---|---|---|---|
| 0x006D2BD0 CTCWife::OnSerialise | RELOCATION_MATCH, vland PASS (test PASS) | 21 (round 2) | src/006d2bd0.cpp, test/006d2bd0.cpp, payload.json |
| 0x006D0830 CTCWife::GlobalUpdate | DIFFER 2581v2584, dl=24 | 16 | differ/006d0830.cpp |
| 0x006D1E30 CTCWife::FrameUpdate | DIFFER 3464v3484, dl=369 | 10 compiled | differ/006d1e30.cpp |

Every attempt is in LOG.tsv (work/<addr>_<tag>.cpp/.txt/.d hold each source, grade and masked diff).

## CTCWife layout decision (task 1)
One retail layout: **sizeof 0xEC**, CTCBase-derived, members at the merged offsets in hdr/engine/CTCWife.h (FABLE_RETAIL_CTCWIFE).
- Alloc 0x004D32A7: `push 0ECh ; call 0x00BFEA1A` then the ctor 0x006CF040, which stores vtable 0x0125ED5C.
- The ctor writes every member at the branch offsets: the map header at +0x78 (0x18-byte node, count +0x7C), the set at +0x98
  (0x14-byte node), FrameNoticedChangeInAppearance at +0xA4..+0xE0, LastFatnessChangePoint at +0xE4.
- Ego_r.pdb gives 0xF4 because its Dinkumware map / set are 0x10 each. STLport 4.6 makes them 0xC, so each later member is
  4 bytes lower.
- The 729 branch added two out-of-line const queries, now merged into the one branch: IsMarriedToThePlayer 0x006CEC80 and
  IsAttractedToHero 0x006CFE80 (Ego_r / FableWin `?..@CTCWife@@QBE_NXZ`).
- Round 2 also added the non-virtual members GlobalUpdate / FrameUpdate call, with Ego_r spellings (FableWin agrees). Public:
  SendPauseEvent, InitWifeAtEndOfMarriageFresco{Start,End}FadeIn, IsCourtableAndNotBlockingIt, IsCourtingHero,
  IsInLoveWithHero, HaveSex, OnHeroViolence, Divorce and InitWifeNonpersistentState. Private (AAE / ABE in the PDB):
  GetNumHoursMarriedFor, FindAppropriateGift, SetAttractionIcon and CheckAppearanceChanges.
- The landed 0x0121E210 is the only landed row that defines FABLE_RETAIL_CTCWIFE. It stays RELOCATION_MATCH on the merged
  overlay. The other 19 landed CTCWife rows (0x006CEC80 .. 0x006D1DE0) use the default path or local models, so the change
  does not touch them.
- PDB spellings were confirmed before authoring. ?GlobalUpdate@CTCWife@@UAEX_N@Z is at Ego_r 0x00705510 and FableWin 0x02483170.
  ?FrameUpdate@CTCWife@@UAEXXZ is at Ego_r 0x007072E0 and FableWin 0x02485720.

## Levers found
- **OnSerialise:** the residue was one store. The fix puts the three-argument `CPersistContext::Transfer<T>` template body in
  the TU, as CTCWife.cpp includes it in retail. VC7.1 then knows the callee only reads its `const T&` default, and stores the
  BooleanHusbandAppearances loop default once before the loop (round 1 stored it every iteration, dl=8).
  - 18 named variants failed before this, including named locals, `__forceinline`, `/Oa`, `/Ow`, do-while and an unsigned
    counter.
  - Open family: **CQuestManager::TransferBoasts 0x004B05C0** is parked on the same symptom and should be retried this way.
- **GlobalUpdate:**
  - Inverted `if (!WaitingForDivorce) {...} else {divorce}` and `if (!WaitingForSex) {...} else {...}` structure.
  - Implicit `const char*` to `CCharString` temporaries.
  - CWorld accessors for the object expressions.
  - The GetSubDef<T> body in the TU.
  - `!IsDead()`.
  - A local `CGameTimeManager&` for FastForwardTimeTo.
  - `GetParentThing().GetPos()` for AddMessage.
- **FrameUpdate:**
  - IsMarriedToThePlayer's body in the TU (retail expands it twice).
  - The CDefinitionManager::GetDef<T>(name) body in the TU.
  - A local `CThingAICreature* wife` in the house-dressing block.
  - A plain `while` for the gift-opinion loop.

## Residue
- **GlobalUpdate, dl=24:** only the wedding block differs. Retail pushes the eight zero arguments of the movie call as
  immediates and loads ebx=0 just before FastForwardTimeTo. This build keeps 0 in ebx from the movie call on: 3 bytes shorter,
  which then adds a `lea esp,[esp]` pad at the village loop head. Tried without effect: default arguments on the movie
  declaration, `/24.0f`, and moving the vector.
- **FrameUpdate, dl=369:** the control flow and every call match. Retail keeps 0 in ebx for the whole function and the hero in
  ebp through the married block, with frame 0x2C; this build spills the hero, with frame 0x30. Three specific differences:
  - **Brain name:** the retail ternary pushes each literal in its own arm.
  - **Gift opinion:** retail passes each OpinionValues entry through a stack temporary, which means an rvalue accessor. This is
    probably the inline `?GetRelativeOpinion@CTCCreatureOpinionOfHero@@QBEMW4EOpinion@@_N@Z`, but its body is unproven, so it is
    not modelled.
  - **SetMarriageState:** the state is passed through eax.

## Overlay changes (hdr/, full merged overlay; hdrcheck2: every changed header default=same)
- **Re-copied from the repo** (it changed under me at 12:58, landing round b / chunk 726), with my edits reapplied:
  stlport/fable_containers.h and stlport/fable_map.h.
- **CTCWife.h:** merged branch (see above).
- **CTCBase.h:** AddToGlobalTCs 0x00686ED0 and GetPWorldSeed 0x00686DA0.
- **CWorld.h:** inline GetMessageEventManager (both), GetGameScriptInterface, and Get/PeekOpinionReactionManager (FableWin).
- **fable_game.h (CMainGameComponent):** PDebugFont / PCutSceneMainFont (+0x16144 / +0x1614C), GetWorld, GetDisplayEngine,
  PeekCutSceneMainFont, and GetCurrentServerFrame 0x00416225.
- **CDisplayEngine.h:**
  - ScreenFadeOutInfo is typed as CFadeInFadeOutBase.
  - Methods: EndScreenFadeOut 0x00434C90, IsScreenFadeOut, and IsScreenFullyFadedOut (the inline body matches the COMDAT
    0x004A9210).
- **CSubtitleRenderer.h:** SetAsInCutscene 0x0049319C, the two SetText overloads 0x00492E4B / 0x00492ED1, and the inline
  skippable setters.
- **CPlayerGui.h:** GetPHeroInfoScreensManager 0x00437CD0.
- **CTCSoundPlayer.h:** PlayAttachedDialogue, IsAttachedSoundPlaying, StopAttachedSound and SetSpecificSampleVolume (0x0066AFF0);
  PlayAttachedCriteria was already declared.
- **Small additions:**
  - CTCVillage.h: OnHeroSlept 0x0065935E and StatGetInterfaceType.
  - CTCPhysicsBase.h: StatGetInterfaceType.
  - CThingCreatureBase.h: AutoPlaceNear and GetSex.
  - CPlayer.h: RemoveMode.
  - CGameCameraManager.h: ResetCamera.
  - CTCBuyableHouse.h: GetCurrentDressLevel (FableWin inline).
- **CThingAICreature.h** (retail branch):
  - HomeBuilding (+0x168) is typed; GetHomeBuilding is inline, and its body is defined after the class or in CThingBuilding.h,
    whichever comes second.
  - Added SetNewBrain 0x00833010, IsBrainUpdateZoneActiveThisFrame 0x00830130, and IsCourtable (CanBeCourted) as an inline.
- **CTCCreatureOpinionOfHero.h:** RelativeOpinion is now typed as COpinionSet (new header), plus StatGetInterfaceType and
  SetPersonalityOverride 0x00681E60.
- **CDefinitionManager.h:** GetDef<T>(const CCharString&, CDefPointer<const T>*) const and GetPDefFromInstantiationName 0x009ADA40.
- **New FABLE_RETAIL branches:**
  - CHeroMarriageDef (sizeof 0xC8, typed).
  - CHeroMarriageInfo (0x18, zeroing ctor).
  - CTCHeroStats (NumLoversAcquired +0x140, PeekMarriageInfo, private SetMarriageState with friend CTCWife).
  - CTCPerceiveThing (GetPerceivedThing, IsAwareOfThing, CanSeeOrSmellPerceivedThing, GetExtendedSightRadius).
- **New headers:** CWifeDef, CGameTimeManager, CMessageEventManager, CTCHeroOpinionDeedLog,
  NHeroInformationScreens_CManager, COpinionReactionManager, COpinionSet, CTCHeroOnlineScoreboard,
  CTCVolumeContainmentTracker and CPersistContext_TransferableComponent.
- **Kept from round 1:** CPersistContext.h, CIEngine.h, CTCCreatureGenerator.h, CTCShapeManager.h, CThingSearchTools.h,
  CThingPlayerCreature.h, and the CEnginePrimitive* headers.

Sibling regrades on the final mirror: 14 rows, all RELOCATION_MATCH (SIBLINGS.tsv). 0x004FF750 needed the GetHomeBuilding
move.

## Header debt / open questions
- 0x00661600 keeps the landed spelling CanSeeOrSmellPerceivedThing (body: `LineOfSight || Smelt`). The probable PDB name is
  ?IsThingPerceived@CTCPerceiveThing@@QBE_NXZ (Ego_r 0x0053C6B0), but that is unproven.
- The CThing::PBaseDef access in GlobalUpdate goes through `&ParentThing->PeekBaseDef()` with a null test; retail tests [+70h].
- The TransferVector<pair<CCharString,long>> spelling in LABELS.tsv comes from the overlay template and needs PDB verification.
- hdrcheck2 conflict: the coordinator script writes coordinator/stubinc in the repo, and EXTRA_RULES forbid the `git clean`.
  I ran a scratch copy instead (coord/hdrcheck2.py) and deleted its stubs.
