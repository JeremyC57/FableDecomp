# Hand chunk 257 (C3D* mesh/anim + CAIStateGroup_* rows, 401..512 bytes): 22 of 40 byte-exact

All 22 grade RELOCATION_MATCH under RTM cl 13.10.3077 with no flags. Each has a behaviour test that passes under g++ (tall.sh) and VC7.1 (tall.py). No row needed QFE.

## What matched
- 00917bb0 CAIStateGroup_Clean::OnReplace
- 0092a220 CAIStateGroup_ApprenticeHero::MoveToAppropriateDummy -- the GetDummy 0x0092A0C0 model, adjusted as follows:
  - C3DVector has a user copy constructor, so the position stays in the frame.
  - The CCharString is named and scoped.
  - do/while(result == 4) keeps the loop unrotated.
  - A named `action` copy is read before the dead 0xAA lookup.
- 0093a5c0 CAIStateGroup_BanditSitRoundCampfire::Eat -- the Drink 0x0093A8C0 model.
- 0094ce60 CAIStateGroup_GameMaster::OfferPlayerAGame
- 00970f30 CAIStateGroup_CreatureUseCoverBase::IsRunnableLeaf -- the RefreshCoverList 0x009710D0 model:
  - The retail CoverList is 12 bytes, so the later members sit 4 bytes below the header; they are reached through forceinline accessors.
  - The offset is a C3DVector built with operator-, with Z dead.
  - The success block is a positive `!empty()` test.
- 0097df60 CAIStateGroupBase::AIAction_PerformActionTargetFace
- 0097fa00 CAIStateGroupBase::AIAction_MinionIdle
- 00a25530 C3DMeshInfo::GetMeshStats -- the resource Touch is `__forceinline`.
- 00a2eaa0 C3DMeshStats::SaveBinary
- 00a2ed10 C3DMeshStats::UnpackNames
- 00a4beb0 C3DAnimationSequenceData::WriteCompressedToFile
- 00a4df10 C3DAnimation2::ReadCompressedFileData
- 00a89450 C3DMesh2::SaveBinary
- 00aa0dc0 C3DAnimationManager::CombineBlendedBaseAnimationsBoneList
- 00aa96b0 C3DMeshFileHelperPointChunk::GetChunkInfo
- 00aa9ba0 C3DMeshFileHelperPointChunk::ReadFromFile
- 00aaab10 C3DMeshFileConvexVolumeChunk::ReadFromFile
- 00ab9fc0 C3DMeshFileSubMeshChunk::ReadFromFile
- 00aba170 C3DMeshFileBoneChunk::ReadFromFile
- 00abc1b0 C3DMeshFileSubMeshChunk::CountTriangles
- 00ac3e00 C3DClothPrimitive::C3DClothPrimitive
- 00addd80 C3DBoundingSphere::AddToBounds

## Parked for Fable 5.1
- 00929020 CAIStateGroup_BanditPlayDice::IsRunnable DIFFER(506v507): the only remaining difference is the village list. Retail keeps `&village->VillageCages` in ebp. | tried: village local, ir1 list reference, forceinline distance helper (fixed GetPItem order and the best slot) | next: ir1 `CThingList& markers = ...` together with ir4's forceinline helper.
- 00929340 CAIStateGroup_BanditPlayDice::Update DIFFER(506v507): three differences remain:
  - Retail calls GetOrientation before GetPosition in the LookAt sum.
  - The frame is 0x110 against retail's 0x120 (the sheathe actions are larger or 8-aligned).
  - Retail does ++NumberOfPlayers as load/inc/store.

  | tried: dispatch-struct virtuals, early returns, copy constructor, 1-argument action constructors | next: `GetPosition() + Forward`, then 0x120 action size.
- 0095e0b0 CAIStateGroup_CreatureRangedCombatFireAtSpecificTargets::Update DIFFER(426v428): three differences remain:
  - esi and edi swap roles.
  - The frame is 0xbc against retail's 0xcc, because the Fire action is larger than modelled.
  - Retail fetches the Fire target and the creature before copying the IP argument.

  | tried: an inline IS_THING_ALIVE whose inline budget runs out, so the later calls go out of line as in retail (0x0069CE20 / 0x00654D58); statement-level results with no conditional temps | next: a 0xc8 Fire action and a named `target` local.
- 0094f480 CAIStateGroup_BalverineJumpAtFleeingEnemy::IsRunnable DIFFER(449v437): register colouring only (this=ebp and creature=ebx in retail). The named `ahead`/`approach` locals fixed the dot-product order. | tried: 3 orderings | next: scheduling-class; leave parked.
- 0093ab70 CAIStateGroup_BanditSitRoundCampfire::GetPNearCampfire DIFFER(510v504): retail spills the HasTC(0x23) bool to the byte later reused as the uninitialised __false_type tag. Retail also keeps the list head in edi and best in ebp. | tried: value-initialised tag (stores 0), named tag, iterator objects (worse) | next: a model that keeps the bool live.
- 00928e20 CAIStateGroup_BanditPlayDice::OnReplace DIFFER(503v505): retail loads the creature before GetPItem in the current-player test. | tried: 4 | next: a named creature local recolours the action block (505v505).
- 0095fed0 CAIStateGroup_DeedReactionRoot::IsStillValid DIFFER(407v403): two differences remain:
  - The inline counted-pointer release captures the object in ecx.
  - The hero is reached via `lea ecx,[eax+714h]`.

  | tried: 4 | next: a destructor with a local object pointer and an inline player accessor.
- 00913f30 CAIStateGroup_BirdFlyToFood::IsRunnable DIFFER(500v500): scheduling only (when the out-pointer zero and the compare copy are stored). | tried: 3 | next: none (scheduling).
- 00920dc0 CAIStateGroup_CreatureSeekEnemyUnderground::OnReplace DIFFER(455v455): esi/edi roles and the x87 Newton-step schedule. | tried: Normalise variants, 2D/3D, named angle | next: none (scheduling).
- 0097cb10 CAIStateGroup_CreatureFindCover::IsStillRunnable DIFFER(443v443): the x87 Newton-step store/reload. | tried: 4 | next: a Normalise helper that stores r.
- 0097d6f0 CAIStateGroupBase::AIAction_LookAtThing DIFFER(422v444): VC tail-merges the two 12-byte position copies. | tried: 4 | next: make the branches' copies differ.
- 009486d0 CAIStateGroup_GoToPub::FreeSeat DIFFER(436v432): VC hoists -1 into ebp, where retail uses immediates. | tried: 4 | next: a model whose index stores differ.
- 0096cfc0 CAIStateGroup_EatAtTable::OnReplace DIFFER(463v463): the frame is 0x10 against retail's 0xc (packing of the lookup-key slot). | tried: 4 | next: share the key slot with the string.
- 00a90260 C3DPrimitive2::CreateDegenerateTriangleIndexBuffer DIFFER(496v496): register allocation only. | tried: 4 | next: none (colouring).
- 00a898d0 C3DMesh2::GetHelperPointPositionAnimated DIFFER(393v402): the x87 copy schedule of the helper position. | tried: copy constructor, member operator* | next: none known.
- 00addbe0 C3DBoundingSphere::BuildFromSubSpheres DIFFER(1124v414): a lane gap. RTM and QFE both unroll the min/max loop by 4; retail keeps it rolled. | tried: both compilers | next: needs a toolchain that does not unroll.
- 00a4d770 C3DAnimation2::CalcLocalTransformationsRelative DIFFER(89v420): asm_bake family. Retail's inline-asm fistp keeps a dead x87 block alive, and compiler-emitted C++ deletes it. | tried: 4 | next: not byte-pure reconstructible.

## Skipped
- 0092b110 CAIStateGroup_ApprenticeHero::Update: the 440-byte row holds more than one function. Update ends at 0x0092B20F, followed by its 5-entry jump table, then padding, then the separate thunks at 0x0092B230 and 0x0092B240.

## Label corrections
- 0x00993E50 is WriteUChar; 0x00993E90 is WriteShort; 0x00996610 is the compressed WriteBuffer.
- 0x00AAB500 and 0x00AAB570 are vector<CPlane> erase and insert; 0x00A96100 is vector<unsigned long>(n).
- 0x00ABB300, 0x00ABB390, 0x00ABA980 and 0x00ABAA10 are the triangle and primitive chunk finders.
- 0x009FC440 is CResource::AddToResourceList, not CTCActivationTrigger::OnKill.
- 0x0099EAE0 is ~CCharString, not _Cons_val.
- 0x00AE3330 is the cloth group reserve, not CSpellContainerList::AddChild.
- 0x00AC6490 and 0x00AC6520 are the cloth chunk group finders, not FindFirstBone/FindNextBone.
- 0x008FCFA0 is CAIStateGroupBase::GetPThingWorld; 0x008FCFC0 is CAIStateGroupBase::GetPWorld, not CPlayer::GetCurrentMode.
- 0x0049D870 is GFGetCurrentFrame, not GFGetPreMainMemoryUsed.
- 0x004C73D0 is CThing::GetPosition, not CVertexBufferWin32::DoSizeof.
- 0x008FCF40 is CAIStateGroupBase::Terminate, not the CXMVPlaybackInfo constructor.
- 0x00692620 is GetCurrentActionID; 0x0088B8F0 is SetBrainOutput; 0x00693EF0 is ~CCreatureActionBase.
- 0x007B8570 is IsSeatTaken; 0x007B85F0 is SetOccupant; 0x006BB390 is IsTimeOfDayInRange.
- 0x00929550 is CTCDiceGame::SetCurrentPlayer, not CVillageTaskCarryCrate::AssignCreatureToTask.
- 0x00916640 is CTCExclusiveUse::SetUser, not CVillageTaskCarryCrate::AssignCreatureToTask.
- 0x006C5630 is a deed-info getter returning CIVCountedPointer, not CThingBuilding::GetFloorMaterial.
- 0x00914330 is GetPBestThing over a def-name filter, not <CIsFreeBonfire,...>.
- 0x006CB6A0 is the mode getter, not CIsHitEventBlocked.
- 0x00718710 is the CCreatureAction_ReloadMissileWeapon constructor, not AimMissileWeapon.
- 0x00929E60 is ~CCreatureAction_ReloadMissileWeapon (folded), not _Dest_val<...CMusicManager...>.
- 0x0062E900 is the Sheathe/UnsheatheWeapons action destructor (folded), not ~CTCLightningOrb.
- 0x008645E0 is CanCreatureSeeCreature, not CMainGameComponent::BeginInputLoading.
- 0x007EE540 is GetVillage (CTCVillageMember), not IS_THING_ALIVE.
- 0x0093B640 is GetPBestThing<CIsNearCampfireWithSpace,...>, not <CIsFreeBonfire,...>.
- 0x004B8050 is STLport list<CIntelligentPointer<..>>::clear, not CGuiControlTreePane::SortTreeRecursively.
- 0x00618100 is the STLport list range `_M_insert_dispatch`, not Campfire_InsertRegionNode.
- Vtable 0x01244344 is CIntelligentPointer<CThingBuilding>. The landed GameMaster comment calls it something else (see LESSONS, Review at landing).
