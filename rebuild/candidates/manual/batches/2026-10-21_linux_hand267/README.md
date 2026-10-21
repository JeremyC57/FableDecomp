# Hand chunk 267 (CPlayerGui .. CPersistContext): 15 of 40 byte-exact

Each of the 15 rows grades RELOCATION_MATCH from the file in src/. Each has a behaviour test in test/ that prints `<addr>_TEST PASS` under both g++ (`tall.sh`) and VC7.1 RTM (`tall.py`).

Totals: 20 parked in differ/, 5 skipped (rows that hold several functions).

## What matched (15)

**CPlayerGui**
- 00437ae0 `CPlayerGui::UpdateStat`:
  - What it does: feeds a value into a stat bar. Depending on the threshold it jumps or animates OldValue by `step`, tracks the recent maximum, and times the settle and fully-open phases. When they expire the bar goes to mode 3. A change while the bar is open sends it to mode 2. `this` is unused.
  - Shape: each `if (a < b) a = b` clamp is an inline helper taking references (`ClampMin`/`ClampMax`). That gives retail's `fstp`/`fld` reload of OldValue and the integer copy of RecentMaxValue.
  - Shape: the frame-timeout test is a `__forceinline` helper. It reads the frames-per-second global into a local before the frame call (homed in the dead `value` slot) and takes the start frame by `const&`, so it is re-read after the call.
- 004387f0 `CPlayerGui::RenderUpdateGuiTimers`:
  - What it does: runs the GUI timer at 0.5/0.5, then the seven stat bars at the def's open/close times, then the quest-info illustrator. The illustrator's secondary fade runs with a local mode forced to 2.
  - Shape: matched first try with the landed `GetPlayerGuiDef` `__forceinline` helper (0x00438990). The two def times are float locals.

**CPlayer**
- 0048a210 `CPlayer::PlayerInit(const CPlayerInit*)`:
  - Uses a retail-view CPlayer (PlayerModes at +0x210 is a 4-byte STLport list).
  - push_front of the null mode and InitJoystickDeviceNumber are inline.
  - `ControlledCreature = CIntelligentPointer<CThingPlayerCreature>(0)`.

**CProgressDisplay**
- 00499880 `CProgressDisplay::RenderProgress`:
  - The render-state capture block is a scoped object, `CRenderStateCaptureBlock`. Its restore must run after the CRenderTarget local's inline destructor; a plain trailing call, or an explicit `{}` scope, reorders it.
  - C2DBoxI needs a member-wise user copy constructor, so the viewport is copied field by field from the display manager.
  - The display manager's `Rendering` flag is homed in the dead `force` slot.

**CPersistContext**
- 009bba40 `CPersistContext::TransferObjectLoadText`:
  - Behaviour: works on a copy of the context's parser and loops while the context is not finished and the parser finds the name. It then calls the item's TransferIn, expects `;`, and blanks the consumed text (newlines kept) or reports an error.
  - Shape: the loop test converts the name implicitly (`CCharString(const char*, long = -1)`). An explicit `CCharString(name, -1)` pushes eax and swaps the frame slots.
  - Shape: the error message is a block-local named string, and the cleaning loop's end is a local read before the PStringToClean test.

**CPhysicsFacesSegmentBranch**
- 00a86b30 ctor: the CPlane user copy ctor makes the member-wise copy go straight into `[esi+4]`. For each side it builds a Branch or a Leaf (`depth + 1`), then clears both child arrays.

**Quest scripts**
- 00d129c0 `CQ_BanditCampHoldingScriptScript::Main`.
- 00d255e0 `CQ_DragonBossFightScript::Main`: SpawnFunction is a `__forceinline` wrapper and the CSpawnedFunc1 ctor is `__forceinline`.
- 00d26cc0 `SpawnMinions`.
- 00d26e60 `SpawnSummoners`.
- 00d34470: really the `CQ_FireHeartScript` constructor.
- 00d66ca0 `CQ_GuildTrainingWoodsMeleeScript::DoMission`.
- 00d88260 `CQ_HeroSoulsNostroScript::WatchForNostroHit`.
- 00ec5140: really `CV_RockTrollFirstEncounterScript::WatchForRockTrollHit`. CScriptThing's copy needs `CBaseClass(other)`, so no out-of-line CBaseClass() call is made.
- 00e2a900 `CQS_ScytheInfoScript::PlayCutscene(CCharString, bool)`:
  - The cast-map key converts implicitly (`cs_chars["..."] = hero`).
  - `CCountedPointer::operator=` copies the source Data/Info into locals before it releases (two frame words).
  - The scripted-thing handle's destructor is inline; the movie handle's PImp destructor is out of line.
  - The movie-sequence name is a block-local named string, and pause/unpause go through one named copy of Game.

## Parked for Fable 5.1 (20)
- 004888c0 CPlayer::SwitchPlayerCharactersWith DIFFER(443v443): register swap only, retail keeps `this` in ebx and the vtable constant / reloaded `other` in ebp (ours the reverse) | tried: statement orders, named pointer locals, IS_THING_ALIVE as macro | next: lane gap: register allocation
- 00488b20 CPlayer::LocateCharacterStartHolySite DIFFER(494v494): retail pushes the find_if iterator-tag word as `lea ecx,[esp+38h]; push ecx`, ours `mov ecx,[esp+38h]; push ecx` | tried: tag by value POD/derived/user ctor/user copy ctor, by const&, iterator_category() helper, named tag local, owner pointer declared before the vector | next: model STLport's `__find_if` with a tag parameter of a different (non-empty) type
- 00499aa0 CProgressDisplay::StartProgress DIFFER(410v418): retail keeps the GetFont temporary's dead `Data = 0; Info = 0` stores after its inline release | tried: temporary vs named local (named keeps them but passes its own address to operator= and swaps eax/ecx), pack(4) layout | next: a counted-pointer destructor that zeroes through a non-inlined path / different operator= signature
- 00815ce0 NHitLocations::CPhysicalPrimitiveCylinder::IsPosInside DIFFER(420v409): x87 schedule, ours keeps the normalised axis on the FPU stack with extra fxch/stores; retail spills the length once and folds the epsilon widen into one fadd | tried: one form (inline GetCylinderRadius, C3DVector operators) | next: statement-by-statement float locals for the axis and the dot product
- 00816150 NHitLocations::CPhysicalPrimitiveCylinder::Init DIFFER(400v406): retail loads the float traits' `this` and the value pointer with two separate `lea`s of the same slot (ours `mov ecx,edx`), and puts the second ',' marker in the dead context slot | tried: per-parse float locals, inline ParseFloat returning a local, traits.DefaultVal value (best), named locals, a shared function-scope traits, the natural SkipPastNextItemIfIs(CCharString(...)) | next: CPersistTraits<float>::TransferIn through a by-reference helper returning the traits' DefaultVal
- 008faa80 NCameraModes::CPhysicsForceModelBase::DoCreatureEvasion DIFFER(411v417): retail spills the sample's y/z to frame temporaries and adds `offset*0.1f` then the camera position from memory (fadd [mem]); ours keeps them on the FPU stack (fadd st(2)) | tried: 4 forms (b/c best) | next: named float temporaries per component
- 008fadb0 NCameraModes::CPhysicsForceModelBase::DoGroundEvasion DIFFER(504v446): the shared evasion loop's x87 schedule plus the result/flip/clamp tail (retail builds the result in the frame and keeps z on the FPU stack through both clamps) | tried: the 008faa80 loop forms | next: solve 008faa80 first, then the tail
- 008faf70 NCameraModes::CPhysicsForceModelBase::DoThingEvasion DIFFER(482v510): first-pass sketch, the evasion loop plus the thing/mesh list build and teardown order differ | tried: sketch | next: solve 008faa80, then model the 0x008FCA40 list helper
- 00a11d10 CPhysicsMeshObject::GetNavigationLevelAt DIFFER(491v422): our inline inverse keeps a whole returned CMatrix3x4 temporary (frame 0x70 v 0x40), and the fld/fmul operand order differs per product | tried: plain inline (not inlined, 74 bytes), __forceinline GetInverse + operator*, swapped operand order (no effect) | next: write the inverse statement by statement into named locals in the function body
- 00a11f20 CPhysicsMeshObject::IsSpherePartContainedInBoundingBox DIFFER(491v428): same inverse family; the 0x00A17690 call's argument shape is guessed | tried: sketch | next: after 00a11d10
- 00a128e0 CPhysicsMeshObject::IsInContainmentVolume DIFFER(538v464): same inverse family (plus a CIVCountedPointer copy of the mesh around the 0x00A6CA20 query) | tried: sketch | next: after 00a11d10
- 00a12ab0 CPhysicsMeshObject::PeekRoughProportionOfSphereContainedInVolume DIFFER(539v468): same as 00a128e0 with the float query 0x00A6CA80 | tried: sketch | next: after 00a11d10
- 00d91740 CQ_HobbeCaveScript::RescuedBandit DIFFER(372v481): retail expands the local vector<CScriptThing> destructor inline at the three early returns and calls 0x008AC970 at the found path and function end; ours calls it out of line everywhere | tried: plain inline, out-of-line | next: lane gap: inline-once destructor
- 00d97170 CQ_HobbeCaveScript::CheckJamesKilled DIFFER(469v491): retail calls the out-of-line ~CScriptThing (0x004AA840) at the hit-block terminate return and expands it inline at the others; __forceinline expands all | tried: plain inline, __forceinline | next: lane gap: inline-once destructor
- 00e1fcf0 CQR_EscortTraderScript::WatchForExpressionFollow DIFFER(388v435): inline-once ~CScriptThing (three inline, one out of line), and the inlined `CCharString ==` bool frame byte [esp+13h] is not reproduced | tried: the advisor e4 shape | next: lane gap: inline-once destructor
- 00e1fef0 CQR_EscortTraderScript::WatchForExpressionWait DIFFER(388v435): same as 00e1fcf0 (EXPRESSION_WAIT) | tried: the advisor e4 shape | next: lane gap: inline-once destructor
- 00e6b750 CV_ChapelOfEvilScript::WatchForRegionEntry DIFFER(390v424): inline-once ~CScriptThing (out of line at the region-hit terminate return), and the `==` bool frame temporary is missing | tried: the advisor e4 shape | next: lane gap: inline-once destructor
- 00f081e0 CQ_AmbushTradersScript::BanditHireSetup DIFFER(440v440): frame layout only, retail gives the "" region string its own frame slot and reuses only the dead banditObject slot for the thread name (frame 0x10 v 0x14) | tried: 5 forms (c best): temporary, named local, implicit conversion | next: a block-local region string declared before the CScriptThing temporaries
- 00f09260 CQ_AmbushTradersScript::WatchForHeroAttackOnSentry DIFFER(424v445): one site left, the hit-block terminate return calls the out-of-line ~CScriptThing | tried: plain inline / __forceinline / advisor e4 | next: lane gap: inline-once destructor (VC7.1 inlines function-wide)
- 00f09950 CQ_AmbushTradersScript::WatchForHeroAttackOnContact DIFFER(424v437): twin of 00f09260 | tried: advisor e4 | next: lane gap: inline-once destructor

## Skipped
- 00f27ea0, 00f28610, 00f33b90, 00f349f0, 00f351f0 `CPersistContext::Transfer<signed_char>`: each 512-byte catalog row holds several function bodies separated by int3 padding.

## Label corrections
- 00ec5140: manifest "CQ_AmbushTradersScript::WatchForHeroAttackOnBandit" is propagated; the body is CV_RockTrollFirstEncounterScript::WatchForRockTrollHit(CScriptThing).
- 00d34470: manifest "CQ_HangingTreeGoodScript::CQ_HangingTreeGoodScript" is propagated; the body is the CQ_FireHeartScript constructor.
- 00e1fef0: the catalog repeats "WatchForExpressionFollow"; the body is CQR_EscortTraderScript::WatchForExpressionWait.
- 00e6b750: catalog "CQ_BountyHuntScript::WatchForEnableFisherCreekHutDoor"; the body is CV_ChapelOfEvilScript::WatchForRegionEntry.
- 0x0099A350 is CInitBaseClass::Init (manifest "CXMVPlaybackInfo" propagated).
- 0x0044BA90 is a shared CGameDefinitionManager::GetDef template body (labelled GetDef<CThingPhysicalSwitchDef>).
- 0x0084F1B0 (the owner's thing getter) carries the propagated label "_Cons_val<...>".
- 0x0048CA60 (list<C3DVector> destructor) carries a propagated label.
- 0x009B9C00 carries the labels "SerialiseGameState" / "CLocalLightElementLink::operator delete". It is a CStringParser method taking a `const CCharString&` (seek to the named item); named SkipToItem here (descriptive).
- 0x013CB508 is g_pRenderStateManager (as the landed 0x00B25C00). 0x00A05840 is CRenderStateManager::RestoreCaptureBlock.
