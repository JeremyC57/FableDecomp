# Hand chunk 238 (CV_BeardyBaldyScript .. CWorldMap): 25 of 40 byte-exact

All grades under the lane default RTM cl 13.10.3077; no QFE row. Flags: `/GS` on 0049eaf0, 00629b10, 0062c1c0 (`// Flags: /GS`).
Jump tables inside the row: 00eda9b0 (7 entries), 00ec3130 (4 entries). No EH, no leaf-symbol surprises beyond the
`// Leaf:` lines given.

## What matched (25)
- **CV_ quest scripts** (game interface at +0x40; CGSI slot tables generated from `ghidra_out/cgsi_slots_aligned.tsv`):
  - 00e53f60 CV_BeardyBaldyScript::SetWanderPointAndDistance -- four interface calls with by-value CScriptThing copies built in their argument slots (inline copy ctor: CBaseClass vtable, CScriptThing vtable, PImp add-ref); the C3DVector argument needs the engine's user copy ctor (dword copy through ecx).
  - 00eda9b0 CV_TalentlessBardScript::ProcessSongsMorality -- the landed ProcessSongsRenown switch: implicit literal temporaries per case, own epilogues.
  - 00edaba0 CV_TalentlessBardScript::ProcessSongsMarriage -- push_back inlined at the first (bachelor) site only: a `__forceinline` copy of the STLport body there, the out-of-line push_back 0x0044BFF0 elsewhere.
  - 00e97ea0 CV_HeroDollsScript::RemoveHeroDolls -- seven TakeObjectFromHero calls on implicit temporaries.
  - 00ec3130 CV_RandomPopulationSimScript::ChooseRandomVillagerFromDefs -- landed Guard sibling: default-constructed string, `rand() % 10` switch with block-scoped name tables sharing frame words, tail-merged operator=.
  - 00ee4a70 CV_TourGuideScript::WatchForGuideKilled -- BeardyBaldy wait pattern with two loops (IsAlive slot 75, MsgIsKilledBy slot 18); inline handle dtor at the first exit only.
  - 00ec5010 CV_RockTrollFirstEncounterScript::WatchForRockTrollKilled -- `SetPredicate(CIsThingAlive(troll))` temporary (novtable predicate base, ool ~CIsThingAlive), the by-value parameter's inline dtor at one exit.
  - 00d27c00 "CV_ChapelOfEvilScript::EndMission" -- two wait loops, `Game->SetThingAsUsable(Game->GetThingWithScriptName("MainGuildMap"), false)` (outer vtable in edi, bool pushed first). Class label doubtful (see corrections).
  - 00e84a20 CV_FisticuffsClubScript::IsKnotholeGladeNotAPunchClub -- named bool over an `||` chain of four implicit temporaries: construction-flag mask in ebx (1/3/7/F), the entry zero store, result in a frame byte.
- **Entity scripts**: 00ddc6c0 CWaypointSpeedCheck::Main -- `bool reset = Me.GetDataString() == "0";` with the inline CCharString operator== (strcmp intrinsic / `Compare ? false : true`, same-TU Compare stand-in), rotated `while (!IsActiveThreadTerminating())`.
- **CWorld** (local models; engine/CWorld.h flattens the counted pointers):
  - 0049daa0 GetCurrentSoundCamera -- render-camera overloads at slots 5/6 (reverse declaration order); the hero physics pointer is a named local so its loads precede the pushes; the returned camera a named local.
  - 0049e620 InitMeshBank -- counted-pointer operator= copying the source words first; bank manager through an inline reference getter.
  - 0049eaf0 HandleMoveHeroToRegionGameEvent (/GS) -- CMemoryBuffer payload idiom, char flags normalised with `!= 0`.
  - 00629b10 EAChargeQuickAccessItem (/GS) -- named first player; inline by-value ReadChar puts the flag byte and its bool into the dead `event` slot.
  - 00629f50 EASkipCutScene -- CSoundInit default ctor (landed PlaySoundAtPos), sound player slot 4 PlaySoundEvent, subtitle renderer flags +0xDE..+0xE0.
  - 0062c1c0 EAOpenHeroCentreDoor (/GS) -- the landed EACloseHeroCentreDoor PeekTC idiom; the hero lookup goes through the const LowerBound 0x004365B0 (const CThing*).
  - 004a0bb0 AutoSaveCheckPoint -- landed AutoSave siblings; three HasTC + GetTC inventory calls inside the alive test.
- **CWorldMap**:
  - 00506c40 GetLevelStaticMapChecksum -- `CCountedPointer<CAFile> file(new CDiskFileWin32(...))` (inline block allocation), the landed GetLevelDimensions(CAFile&) body inlined.
  - 00506f30 AddLevelFromFile -- null-initialised slot pointer set in the search loop, CArray resize fallback, box translated to the position.
  - 0050af10 ActivateNavMap -- CNavigationAreaInit local (inline zeroing ctor, inline derived dtor freeing two POD vectors, ool ~CNavigationAreaUpdate).
- **CVillageTask*** (local models; the engine headers flatten the intelligent pointers):
  - 00860650 CVillageTaskTorch / 00860840 CVillageTaskHorn ::IsCreatureMoreAppropriateForTask -- the landed SecuritySweep form with the goal taken from the torch/horn thing.
  - 00861530 CVillageTaskCarryCrate::IsCreatureMoreAppropriateForTask -- Pickup re-read after the validity call keeps both compare arms (2.25 factor).
  - 00860d00 CVillageTaskPickCrops::IsBetterThan -- landed SecuritySweep IsBetterThan form.
- **STLport**: 0067cb20 vector<CBlurredAttitudesDef>::_M_insert_overflow (label CVectorBase::resize_realloc) -- landed 0x00CBC850 model with same-TU stand-in bodies for the two fastcall helpers (precedent row), virtual element destructor in the inline _Destroy loop.

## Parked for Fable 5.1 (15)
- 00ed9bb0 CV_TalentlessBardScript::ProcessSongsClothes DIFFER(298v306): retail zeroes its one frame word (`mov [esp+4],0`) at entry before IsHeroNaked; everything else matches | tried: plain else-if chain, unused zeroing struct local | next: the entry zero store is the flagged-temporary pattern of 00e84a20/00506c40 -- try a form where the literal temporaries are conditional within one full-expression (e.g. `?:` or `&&` around push_back).
- 00f058d0 CQ_AmbushTradersScript::WatchForRegionChange (label CV_ChapelOfEvilScript::WatchForLeaving) DIFFER(280v308): retail materialises each block compare (neg/sbb/inc) into a frame byte [esp+0xf] in an 8-byte frame; ours branches on `test eax,eax` | tried: inline operator== || chain, named bool, split null/block tests, `? false : true` block compare | next: the 00ddc6c0 form (named frame-byte bool) with the bool declared at function scope before the loop.
- 00629c40 CWorld::EAControlledCreatureUseAbility DIFFER(275v275): edi/ebp register swap only (event vs creature) | tried: named first player, named player number, by-value ReadUShort/ReadFloat | next: lane gap: register swap.
- 0049f2d0 CWorld::GetEnvironmentFishWeightMult DIFFER(282v283): register permutation and the fild temporaries reuse the dead GetDef out slot in retail | tried: named world map, def holder copied from the out slot, block-scoped out slot | next: lane gap: frame-slot sharing of a dead out slot.
- 00506e30 CWorldMap::GetLevelDimensions(const CCharString&) DIFFER(252v247): retail loads PWADBank (LevelWADBank +0xA0) into esi before ToWideString inside the new-expression and keeps the count block in esi for the release | tried: pointer and reference bank arguments, dtor with/without a local | next: the 00506c40 counted-pointer-from-new ctor form instead of default + Reset.
- 004feb70 CWorldMap::HeightAreaChanged DIFFER(260v261): retail keeps a zero in ebx (area test, vector zeroing, re-zeroed after loop 1 for the free compare) and walks loop 2 in ebp | tried: World offset fix, `area == 0`, unsigned map | next: lane gap: zero-register hoisting.
- 004ff920 CWorldMap::Update DIFFER(319v319): the callback-pointer local is stored above the loop guard in ours, in the loop preheader in retail | tried: callback in the loop body, explicit `if/do-while` guard, const-ref callback temporary | next: lane gap: store sinking below a loop guard.
- 00860ad0 CVillageTaskPatrol::IsCreatureMoreAppropriateForTask DIFFER(302v302): scheduling only (first copies load all three dwords before storing; fnstsw after the two fstp) | tried: SecuritySweep form | next: lane gap: scheduling.
- 00e11450 CWaspAttacker::Main DIFFER(291v291): retail hoists the loop's `lea ecx,[victim]` above the terminate `jne` (return enters the dtor past its lea) | tried: by-value victim (devirtualised), const-ref victim, inline IsAlive helper | next: lane gap: cross-jump/tail placement.
- 00ad8dd0 CVertexGroupBlockOptimiser::CanInsertMoreBlocks DIFFER(240v243): outer-loop register allocation (retail count in ebp, i/block spilled around the inner find loop) | tried: named current block, `n > 0` inner bound (fixed jbe), walking block pointer, direct members | next: lane gap: register allocation.
- 00ad89b0 CVertexGroupBlockOptimiser::CalcMinimumBlocks DIFFER(306v314): register allocation (retail sizes in edx reloaded after each call, begin in edi, the available-slot pointer spilled into the dead `start` slot) | tried: natural recursion, in-place `remaining -= size` | next: a named `bool& slot` kept across the call, sizes begin as a named local.
- 00ad9770 CVertexGroupBlockList::RemoveAllSubsets DIFFER(292v280): loop register allocation (ebp walks the blocks and holds the kept finish pointer) | tried: goto-continue with indexed and walking inner loops | next: same-TU stand-ins for the vector operator= 0x00ADCA30 / _M_insert_overflow 0x00ADCE60 (value read through its const ref).
- 00c3a7d0 "CWorld::AnimationCurve_ApplyTiming" DIFFER(304v273): retail keeps the envelope value on the x87 stack through the angular wrap (fcom/fsub/fadd merge, then fst into the dead buffer slot) | tried: clamped float local | next: an inline `WrapAngle(float)` returning by value; class unidentified (audio library, 0x00C3xxxx).
- 0049ffd0 CWorld::SetKickPos DIFFER(259v258): scheduling only (first position copy loads before stores, filter-pointer store order, found thing in esi) | tried: plain locals, block-scoped search locals (fixed the frame), named found pointer, filter ctor body order | next: lane gap: scheduling.
- 004fce20 CWorldMap::AllocateMapUID DIFFER(343v318): count/range register swap and the retry-path layout (retail reloads the range spill only on retry and returns from the search loop) | tried: 32-/64-bit uid (64-bit fixes the two-part compare), goto retry | next: `for(;;)` with the uniqueness test in an inline bool helper.

## Skipped
- none.

## Label corrections
- 0x00860650 is CVillageTaskTorch::IsCreatureMoreAppropriateForTask (labelled CVillageTaskHorn; byte twin of the real Horn row 0x00860840).
- 0x00861530 is CVillageTaskCarryCrate::IsCreatureMoreAppropriateForTask (labelled CVillageTaskHorn; reads Pickup +0x18, calls 0x008614B0).
- 0x00F058D0 is CQ_AmbushTradersScript::WatchForRegionChange (labelled CV_ChapelOfEvilScript::WatchForLeaving; calls RefreshSpyAllegiences 0x00F05A10 for Gibbet Woods / Deserted Farm / Bowerstone Jail).
- 0x00D27C00 "CV_ChapelOfEvilScript::EndMission" is not the chapel script (no bool at retail +0x51 in that class; guild region / guild map / hero guide body); owner unidentified.
- 0x0067CB20 is vector<CBlurredAttitudesDef>::_M_insert_overflow (labelled CVectorBase::resize_realloc).
- 0x00C3A7D0 "CWorld::AnimationCurve_ApplyTiming" is an audio-library gain envelope applied to a 16-bit sample buffer, not the game CWorld.
- Callees: 0x005BCF51 CTCInventoryBase::IncrementQuestPersistentRefCountForNonPersistentItems (labelled `_Dest_val<...CFlash>`; false twin 0x005BCF59); 0x004BBFD0 NGraphicAppearance::CTCGraphicAppearance::SetMeshBank (labelled SetCurrentThingManager); 0x0049D9B0 CWorld::GetSampleBank (labelled CPixelShader::CountReferences); 0x00437A10 CPlayerGui::ToggleQuickAccessCharge, 0x004379B0 CPlayerGui::OnEventAttack (labelled `_Cons_val`); 0x00640560 CEngineManager::AddChangedArea (labelled std_vector_push_back); 0x00A14E40 CNavigatorManager::UpdateNavigationMapArea(long, const C2DBoxF&) (labelled `$E2`); 0x00436FB0 CTBaseSingleton<CEngineManager>::Get (labelled CGameJoystickManager Get); 0x0051D530 ~CNavigationAreaUpdate (landed as CVectorQuad_0051d530); 0x00CB7920 CScriptBase::SetPredicate (labelled `_Cons_val`); 0x00ED9CF0 / 0x00EDA0C0 / 0x00EDA490 / 0x00EDA5E0 CV_TalentlessBardScript::IsHeroDressedInArmour / InWillRobe / AsApprentice / AsAWoman (labelled DressedAsEvilMage / ProcessSongsClothes_ApplyDressCode); CWorld vtable 0x012390F0 slots 5/6 are the two GetCurrentRenderCamera overloads.
- Retail offsets noted: CWorldMap LevelWADBank +0xA0, UseLevelWADBank +0xA8, CurrentMapUIDCount +0xAC, PLevelLoader +0xBC, PassabilityBeingShown +0xD5, HeightChangedAreas +0x84; CSubtitleRenderer InCutscene/CutsceneSkipped/CutsceneSkippable +0xDE/+0xDF/+0xE0; CNavigationAreaInit 0x5C bytes; CEnvironmentThemeDaySetDef FishWeightMult +0x68.
