# Hand chunk 271 (CTCInventoryItem .. CTCPhysicsControlled): 25 of 40 byte-exact

## What matched (25)
- CTCInventoryItem (catalog) / CTCTargetingPlayer: 008026a0 OnInitialActivate -- takes the largest value of a def map<long,float>; the tree walk is a static `_M_increment(node)` that returns the next node; the vectors are assigned from constructed temporaries.
- CTCInventoryMap:
  - 005faf50 DrawPreparePrimitivesForRendering (/GS) -- the quests sibling's body; the inline CGameEvent ctor plus AddUBYTE homes the byte in the dead `info` argument slot.
  - 005fb630 GetRelativePosOnMap -- the element is read from raw `_M_start[i]`; GetDef has a same-TU noinline stand-in; size() is a signed /72.
  - 005fb8f0 SetPanMode -- the by-value text argument is hoisted above the branch; find has a noinline stand-in; inline C2DVector operators.
- CTCInventoryTrade: 0059e4d7 ConstructSubtitlesPC -- thirteen FindChild calls, each building its by-value CCharString in the stack slot.
- CTCInventoryWeapons:
  - 005c7913 / 005c7f07 GetItemDescription / GetItemDescriptionDamage, 005c7b10 GetItemDescriptionNoDamageNorWeight, 005c7d0d GetItemDescriptionWeight -- the def pointers are released by hand; GetSubDef / GetDef have noinline stand-ins; the final lookups sit in their own block.
  - 005cb780 CheckForSoldWieldedWeapons -- an escaping STLport vector, two size() loops each with a `break`.
- CTCJackPillar: 007317d0 Enable -- the out-parameter GetInterface plus a GetTC named local whose miss path reads the dead key slot.
- CTCKraken:
  - 007faf60 DoHeadTracking -- the IsDead() member form keeps `& 1` an immediate; Clamp returns a const reference; the limit is written twice in place.
  - 007fb6a0 StartBeamEffect / 007fb870 KillBeamEffect -- the declaration order sets the frame stores; CIntelligentPointer operator= names the item in a local.
  - 007fbc90 GetCentreMarker -- a block-scoped `found` out pointer shares the name temporary's word.
- CTCLook: 00752930 SetEyesLooking -- the GetAnglesFromBodyToPos family: inline operator!=, WrapAngle, the IS_THING_ALIVE template.
- CTCMeshAutomaticEntityCreator: 0072ddb0 CreateObject / 0072df50 CreateBuilding -- the empty script name is a NAMED CCharString in its own block around the factory call (an unnamed temporary pushes the ctor's eax).
- CTCObjectAugmentations (operator[] written `*(begin() + n)` through an inline begin()):
  - 00766a60 FireCombatAugEffect -- forced-inline STLport erase/clear (advisor).
  - 00767200 OnSerialise -- saving transfers the bool parameter itself.
  - 007674a0 ClearAllParticleEffects -- inline clear() = erase(begin, end); the FalseType tag needs a user ctor (advisor).
- CTCOracleMinigame: 007e5d20 PlayGame -- each track name is a named CCharString in its own block.
- CTCPhysicsBase: 006b0c10 SetPosition (advisor):
  - a same-TU auto_inline(off) LowerBound stand-in lets the key share the zeroed mapwho word;
  - the final call goes through an inline GetParentThing().
- CTCPhysicsControlled:
  - 00725060 ApplyControlledAcceleration -- the C3DVector user copy ctor scalar-replaces the named copy; Velocity goes through a named reference; the dot product is `X*X' + (Y*Y' + Z*Z')`.
  - 00725290 UpdateOnSurface -- VC folds `x + 0.0f` (the named `down(0,0,-r)`); the radius comes from an inline by-value getter.

## Parked for Fable 5.1 (13)
- 005a3581 CTCInventoryTrade::BuyHighlightedItem DIFFER(435v435): the hero-stats GetInterface out pointer gets its own zeroed slot and the zero store is hoisted (retail shares the key slot) | tried: /Os pragma + probe variants (probe/005a3581*) | next: same-TU LowerBound stand-in (the 006b0c10 fix)
- 005cd4c4 CTCInventoryStats::BuildHeroClass DIFFER(483v499): the `first` bool and best/bestIndex do not share retail's slots (retail has 16 bytes more stack traffic) | tried: /Os pragma, begin() indexing (probe/best_005cd4c4.cpp) | next: an inline max-stat helper on CTCHeroStats
- 00641020 CTCLight::Persist DIFFER(461v488): this/context register swap; the Flicker transfer tail is not merged | tried: probe/pl1.cpp variants | next: lane gap: tail merge
- 006412b0 CTCLight::SetActive DIFFER(463v463): the GetTC(0x5c) key sits at [esp+8], where retail uses the dead argument slot [esp+10h] | tried: sa1..sa5 lookup forms | next: same-TU LowerBound stand-in (the 006b0c10 fix)
- 00641c30 CTCLight::OnAppearanceDraw DIFFER(528v490): the fallback-position copy is not tail-merged into one DrawLight call | tried: probe/best_00641c30.cpp variants | next: lane gap: tail merge
- 007f24e0 CTCLightningOrb::FrameUpdate DIFFER(453v453): scheduling only (hitLocations load order) | tried: one form | next: lane gap: scheduling
- 00753930 CTCLook::SetBodyMovingViaAnimation DIFFER(401v401): register allocation only (this in edi and the component in ebx; retail ebx / edi), so the second anim-index movsx temporaries swap | tried: CRightHandedSet copy, two named vectors, GetTC, early return | next: same-TU LowerBound stand-in (the 006b0c10 fix)
- 00753e90 CTCLook::LookAtPos DIFFER(447v447): lengthSq is not homed in the dead pos-argument slot (frame 0x10 vs 0xc); pos is in edi, retail ebp | tried: user operator=, default assignment, user copy ctor, length declared first | next: lane gap: float homed in a dead argument slot
- 005fd410 CTCInventoryMap::GetRegionsToDisplay DIFFER(-v451): not attempted (STL list thing search + inline lower_bound + set inserts) | tried: - | next: author
- 0061a6a0 CTCInventoryQuests::OnCreate DIFFER(-v508): not attempted (the switch's jump table ends exactly at the row end, so it is one function) | tried: - | next: author
- 005d64dd CTCInventoryStats::AddHeroAbility DIFFER(-v477): not attempted (/Os TU: push/pop constants, `leave`) | tried: - | next: author with #pragma optimize("s", on)
- 00788617 CTCMeleeTrap::ProcessPossibleStrike DIFFER(-v423): not attempted (/Os TU, heavy STL) | tried: - | next: author with #pragma optimize("s", on)
- 0078891d CTCMeleeTrap::PerformPossibleTrapDamage DIFFER(-v466): not attempted (/Os TU, heavy STL) | tried: - | next: author with #pragma optimize("s", on)

## Skipped
- 005aceda CTCInventoryTrade::OpenInventory -- the row is truncated mid-function.
- 007fc820 CTCKraken::SetPhase -- a jump table followed by another function body inside the row.

## Label corrections
- 008026a0 is CTCTargetingPlayer::OnInitialActivate (catalog CTCInventoryItem::OnCreate).
- 005c7b10 is GetItemDescriptionNoDamageNorWeight; 005c7d0d is GetItemDescriptionWeight.
- 00641020 is CTCLight::Persist (catalog vfunc_1).
- 0x005D97F0 "IsAugmentation" is a weapon test.
- 0x00787580 "InitialActivate" takes a bool (switchable navigation enable).
- 0x00641260 is CTCLight attach-particles; 0x007F8860 is the particle detach.
- 0x005ED440 is CTCCreatedEntity::SetPCreator (not AssignCreatureToTask).
- 0x0072DD60 is AddOwnedEntityTC (not ~COpinionDeedEffectsDef).
- 0x007E6EC0 / 0x0056D050 are GetPFirstThing<HasTrackName> (not HasDefGlobalIndex).
- 0x006AF620 is CTCPhysicsBase::GetWorldMap (not CThingTrackNode::DrawIsDrawable).
- 0x0076BD70 / 0x0076BDD0 are STLport __destroy_aux (not _Uninit_copy); 0x0076B1E0 / 0x0076B320 are vector::operator=.
- 0x004C7590 is CThing::UpdateGraphicAppearance (not DetachTarget); 0x004167C8 is the frame getter.
- 0x0070B460 is CTCAnimationComplex::GetAnimation (returns CAnimateThing&; manifest DrawGetEnvironment).
- 0x0070C0D0 is CAnimateThing::GetMeshBank; 0x00A242C0 is CMeshDataBank::PeekAnimation (manifest GetAbilityLevel).
- 0x005DC000 is the turning-anim entry's turn query (from, to, float*) (manifest _Cons_val<...CAnimationEntry...>).
- 0x00A14440 is C3DVector::Normalise (manifest GetScaled; agrees with chunks 217 / 239).
- The CTCBase accessors 0x686D60 / 0x686D90 / 0x6869D0 / 0x686D40 / 0x686A70 / 0x686D20 / 0x686AE0 carry propagated labels.
