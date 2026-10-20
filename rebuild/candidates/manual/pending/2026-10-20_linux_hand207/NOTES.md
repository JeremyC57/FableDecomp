# Hand chunk 207 (CTCDivineWrath .. CTCHeroAttachableAppearanceModifiers): 25 of 40 byte-exact

All 25 are RELOCATION_MATCH under q.py (re-graded together with gall.sh, `grades.txt`); every one has a
self-contained behaviour test printing `<addr>_TEST PASS` under g++ (tall.sh) and VC7.1/Wine (tall.py, `tall_vc.txt`).

## What matched (25)
- 00793d20 CTCUnholyPower::SetPTarget -- named block local for the out-of-line intelligent-pointer ctor
- 00793e00 CTCUnholyPower::ResetCreatureHitState -- out-param GetInterface template
- 007949f0 CTCUnholyPower::OnCreate -- vtable 0x0126BFDC slot 4; named physics/driver locals
- 007c30e0 CTCDrainLife::CTCDrainLife (leaf `??0CTCDrainLife`) -- CCharString temporary in the dead arg slot
- 007a54a0 CTCForcePushPower::CTCForcePushPower (leaf `??0CTCForcePushPower`) -- DrainLife twin, HasTC+GetTC
- 007e2600 CTCElectrocuteOnTouch::OnHit -- member-wise direction + owner local
- 0071ac70 CTCEmoteIcon::SetEmoteText -- EmoteType store before EmoteScale
- 0076cc00 CTCEnemy::RemoveEnemyThing -- two nested named bools sharing one frame byte
- 007c8fd0 CTCFireballSpell::CancelMove -- parent local in edi
- 007e76b0 CTCFireheartMinigame::FrameUpdate (slot 9, leaf `FrameUpdate`) -- unset-result GetScriptedControl accessor
- 007e78e0 CTCFireheartMinigame::PlayGame -- nested loops + physics local
- 007b0880 CTCFishing::GetPFishingRodDef -- forceinline def copy with AddRef/Release pointee members
- 006d50c0 CTCFollowed::AddFollower
- 006d5180 CTCFollowed::RemoveFollower -- single-fetch forceinline alive test
- 00798570 CTCForcePushed::CTCForcePushed (leaf `??0CTCForcePushed`)
- 007ffeb0 CTCGatherPointBase::AddAsUser -- push in the if-arm, return false in the else
- 007c5710 CTCGhostSword::CancelMove -- `*(begin()+i)` operator[] loop
- 006ca4e0 CTCHero::OnInitialActivate -- plain vptr word (int64 member); block-scoped out declared after the base-def read
- 006ca400 CTCHero::SetTitle -- world/thing locals, type byte != 2
- 007a69f0 NGraphicAppearance::CTCGraphicAppearanceReplaceableMesh::ReplaceMeshByIndex (leaf `ReplaceMeshByIndex`)
- 004bc410 NGraphicAppearance::CTCGraphicAppearance::GetFirstPrimitiveWithDummyObject (leaf) -- mesh info named local before the hidden-return counted temporary
- 004bc750 NGraphicAppearance::CTCGraphicAppearance::GetRenderMeshObject (leaf) -- bank named local
- 004bd200 NGraphicAppearance::CTCGraphicAppearance::WasRenderedLastFrame (leaf) -- conditional def temporaries, split returns
- 004bd2e0 NGraphicAppearance::CTCGraphicAppearance::IsToReduceLODForDummiesAndHelpers (leaf) -- inline distance helper, `!f()` tail
- 004c11f0 NGraphicAppearance::CTCGraphicAppearance::GetStaticFocalPosIfLoaded (leaf) -- named-result operators, sum operand order

## Parked for Fable 5.1 (15)
- 00730a00 CTCDoor::PlaySoundW DIFFER(212v210): base-def load after the component load; sub-def copied ecx->edi where retail forms `lea ecx,[edi+28h]` | tried: const vs non-const Destroy/RefCount, inline ~CDefPointer, named holder local (4 grades) | next: hand the holder copy a forceinline accessor that returns the def pointer by value
- 007310c0 CTCDoor::UpdateOpenStatusTriggerTypeScript DIFFER(163v163): only the out def pointer's zero store (retail right before GetSubDef, ours before Open(true)) | tried: out local before/after Open, block scope (4 grades) | next: declare the out pointer inside the GetSubDef call's condition (`if (CDefPointer<..> p = ..)`-style init)
- 00750be0 CTCExplodingObject::CTCExplodingObject DIFFER(242v234): sub-def out slots swapped and zeroed by immediates where retail shares the ebp zero | tried: declaration order swaps, explosion pointer init 0 before/after (4 grades) | next: init the explosion pointer from a shared zero local declared first
- 007986a0 CTCForcePushed::SetUpForcePush DIFFER(202v202): x87 -- retail pops the three components (`fstp st(3); fstp st(0); fstp st(0)`) before storing the length | tried: member GetLength, free GetLength(C3DVector), named length local (4 grades) | next: lane gap: x87 stack-cleanup ordering
- 007c6460 CTCGhostSword::FrameUpdate DIFFER(256v240): register allocation in both loops (timer slot lea form, ebx/edi swap, extra loop-alignment lea) | tried: index vs pointer loops, operator[] forms (4 grades) | next: lane gap: register allocation
- 00706450 CTCHeroAttachableAppearanceModifiers::IsWearingAppearanceModifierCoveringBodyArea DIFFER(216v187): loop-invariant zero hoisted into ebx (entries vector spills to the frame) | tried: forceinline DoesEntryCoverBodyArea helper, reference-to-vector local (4 grades) | next: per-iteration block-scoped out pointers constructed inside the loop body
- 007cbdc0 CTCGold::GetGold DIFFER(204v166): block-scoped out slots get separate frame words; releases take the long `mov/lea/dec/test/mov` form instead of `dec [reg+4]; jne` | tried: forceinline GetThingDef/GetGoldDef helpers, const vs non-const Release/Destroy (4 grades) | next: `--Object->RefCount` through a non-mutable member on a non-const pointer for the short form, then the shared slot
- 00706370 CTCHeroAttachableAppearanceModifiers::RemoveAppearanceModifier DIFFER(190v187): ebx/ebp/esi allocation and the thing def read from its slot at each use | tried: CDefOutPointer variant, found-flag sequencing, named appearance pointer (4 grades) | next: lane gap: register allocation
- 00706880 CTCHeroAttachableAppearanceModifiers::AddAppearanceModifier DIFFER(211v202): the entry temporary takes its own frame word (retail reuses the dead thing-def out slot), registers rotate | tried: entry built in place vs named, block-scoped outs (3 grades) | next: build the entry inside the scope that ends the thing-def out's lifetime
- 0077c160 NGraphicAppearance::CTCGraphicAppearanceStaticMeshFadeOutOnProximity::GetMeshesUsed DIFFER(171v169): def pointer read via eax then copied to esi; index/size loads swap | tried: def local, out.Object direct, CDefHolder (4 grades) | next: lane gap: register allocation
- 007e1990 NGraphicAppearance::CTCGraphicAppearanceMultipleStaticMeshes::GetMeshesUsed DIFFER(214v214): load scheduling only (RenderSizeX before the mesh index, flag word before the first element store) | tried: member-wise vs struct copy (2 grades) | next: copy the CEngineGraphic as a whole struct into a named local, then push_back
- 004bc800 NGraphicAppearance::CTCGraphicAppearance::GetPositionMatrix DIFFER(217v217): the mesh object's vtable store emitted one push earlier | tried: temporary vs named mesh object (2 grades) | next: lane gap: store scheduling
- 004be3e0 NGraphicAppearance::CTCGraphicAppearance::IsResourcesLoaded DIFFER(175v173): vector start loaded via eax and copied to edi | tried: for-init vs pre-loop pointer, STLport _Vector_base/alloc_proxy model (4 grades) | next: lane gap: register allocation
- 004bc340 NGraphicAppearance::CTCGraphicAppearance::RefreshPrimitivePos DIFFER(209v202): x87 spill placement in `Start + (End - Start) * t`; orientation words copied after the position math | tried: ctor-form vs named-result C3DVector operators, component-wise lerp (4 grades) | next: lane gap: x87 spill ordering
- 007e7c90 CTCFireheartMinigame::DrawPreparePrimitivesForRendering DIFFER(230v230): only the C2DBoxF TopLeft zero stores are scheduled after the uv C2DVector zero stores (retail before); frame, registers, pushes match | tried: two-vector box ctor (271v230), four-float box ctor, inline `clear()` (fixed esi/edi), body-assignment box ctor (4 grades) | next: give the uv vectors a default zeroing ctor or a static zero C2DVector, so their stores lose priority

## Skipped
None.

## Label corrections
- 00793d20, 00793e00, 007949f0: chunk label CTCDivineWrath -> CTCUnholyPower (SetPTarget, ResetCreatureHitState, OnCreate; vtable 0x0126BFDC)
- 00706450: chunk label CTCGold::GetGold -> CTCHeroAttachableAppearanceModifiers::IsWearingAppearanceModifierCoveringBodyArea
- 00706370: chunk label CTCHeroAttachableAppearanceModifiers::AddAppearanceModifier -> RemoveAppearanceModifier
- 007e76b0: CTCFireheartMinigame::vfunc_9 -> FrameUpdate
- 007e7c90: CTCFireheartMinigame::vfunc_11 -> DrawPreparePrimitivesForRendering
- CTCGraphicAppearance* rows: the class lives in namespace NGraphicAppearance (PDB)
