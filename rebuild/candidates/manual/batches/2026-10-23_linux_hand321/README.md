# Hand chunk 321 (CParticleEmitter .. CQ_HeroSoulsMotherScript): 22 of 40 byte-exact

## What matched (22)
- **CPlayerGui::DrawDebugSafeAreas 0x0043B050**: resizes the 8-entry handle vector. With the safe-area debug flag set, it builds eight one-pixel line sprites (outer 640x480 frame with alpha 0, inner frame opaque) and adds them to handles 0..7; otherwise it clears every handle. Matched on the first grade. The sprite ctor and renderer slot 23 come from the landed CTCFireheartMinigame::DrawPreparePrimitivesForRendering 0x007E7C90; for_each with mem_fun_ref(&Clear) comes from NPlayerGui::CStatBar::ClearGraphics 0x00645F8D. Two details mattered: the sprite needs an `unsigned __int64` member, so its 0x58 size and 8-byte alignment give retail's `and esp,-8` frame; and the boxes and uv vectors are temporaries in each ctor call.
- **CQR_EscortTrader_ManagerScript::Main 0x00E24E10**: a phase machine (activate escort, wait for escort and gating quest, repeat on quest days). The match needed one change: the day and time tests go through `__forceinline` helpers that take the `CScriptDef*` (`DayModInterval(Game->GetDayOfWeek(), g_pScriptDef)`). With them, g_pScriptDef is loaded into callee-saved edi before each interface call. edi then becomes a saved register, and VC also uses it as the zero register (xor edi,edi for the flag word, the first-run stores and the phase-0 compare). That one change removed the entire 2-byte residue.
- **CQ_HeroSoulsMotherScript::Main 0x00D835D0**: needs the forceinline CSpawnedFunc ctor and SpawnFunction. The re-open-conversation arm is written `if (!IsActiveThreadTerminating()) conversation = ...; else return;`, which puts the shared ~CScriptThing exit after the spawn.
- **CQ_HeroSoulsBriarScript::Main 0x00D80E40**: matched on the first grade. The health waits are `!(GetHealth(briar) < 50.0f)`, and the morality call is `GiveHeroMorality(g_pScriptDef->HS_GoodSoulTaken)` with the field at +0xc9c (PDB - 0xb0).
- **CQ_OrchardFarmRaidScript::Main 0x00DCC770** (catalog label CQ_GuildTrainingScript::Main): the crate-drop copies are CScriptThing::operator= over the counted pointer. They need `__forceinline` on CScriptThing::operator=, CCountedPointer::operator= and Release.
- **CQ_NewOakValeIntroScript::Main 0x00DABAC0** (catalog label CPersonalScript_GlobalThingsScript::Main): 16 NOVI_ bindings with flags 0, built from the CQ_GuildTrainingWoodsDepartureScript::Main binding/spawn model.
- **CQ_WaspBossScript::Main 0x00E0EA40** (catalog label CQ_GuildTrainingScript::Main): the DoMission thread is spawned by a direct `AddSpawnedFunction(new ..., "PicnicArea")`, so the region temporary is built before the `new`.
- **CQ_HangingTreeGoodScript::CleanUp 0x00D72B70**: modelled on the landed sibling CQ_HangingTreeEvilScript::CleanUp 0x00D6B720. Both vectors are declared before the first GetThingWithScriptName, because retail's zero stores come first.
- **CQ_BanditCampScript::Main 0x00D00AF0, CQ_GuildTrainingWoodsDepartureScript::Main 0x00D61D20**: binding model of CQ_GuildTrainingWoodsMeleeScript::Main, with one flag word guarding the name temporaries.
- **CQ_BreakSiegeScript::WatchForTermination 0x00D1D7D0**: region-loaded loop with the inline CCharString == of the landed CQ_BountyHuntScript 0x00D151D0.
- **CQ_GuildTrainingScript::KeepTabsOnWhisper 0x00D3CBD0 / RunCeremonyCutscene 0x00D49D50**:
  - KeepTabsOnWhisper: the "still active" wait is written `!= false` (test/setne), and the "not loaded" waits are `!f()`.
  - RunCeremonyCutscene: cast/movie model of CQ_HeroSoulsBriarScript::PlayCutscene.
- **CQ_GuildTrainingMeleeScript::Main 0x00D55E90**: handle destructors are inline only at the first wait's return. Every later exit wraps its `return;` in an in-body `#pragma inline_depth(0)` (the CMeleeThunder::Main model).
- **CPrisonGuard::Main 0x00DD44C0, CPrisonEscapeGuard::Main 0x00DD6440, CPrisonRacer::Main 0x00DDC980**: the CTimer destructor (DeregisterTimer) is inline at some exits and out of line (0x00CD4470) at others, using in-body inline_depth(0). CPrisonRacer also uses the `for (;;) { if (terminating) return; ... }` main loop.
- **CQ_AmbushTradersScript::WatchForRegionClear 0x00F09CD0 / WatchForQuestSuccess 0x00F0A470 / DoQuest 0x00F0B130 / Intro 0x00F0BB10 / WatchForSpySpeechMarkers 0x00F0EF60**:
  - `__forceinline` ~vector and CCharString ==.
  - `for (;;)` waits (VC duplicates the head test).
  - Per-return inline_depth(0) for the out-of-line ~CScriptThing exits.

## Parked for Fable 5.1 (8)
- 00d39960 CQ_FireHeartScript::StartRound DIFFER(1774v1774): the post-loop `if (Term) return;` lays its timer-cleanup copy (DeregisterTimer x2 + epilogue) inline after `je` where retail puts it at the function end after `jne` | tried: if/return, if(!T) X else return, if(!T) X, if(T) return else X | next: lane gap: block placement of a shared return cleanup (same bytes otherwise)
- 00ae1bc0 CParticleSimulation::ReadFromFile DIFFER(1158v1158): `mov eax,[ebx+4]; lea ecx,[eax+4]` (retail) vs `mov ecx,[ebx+4]; add ecx,4` at the first read after the alpha-array read | tried: named temps -> inline ReadValue<T> (fixed the frame), size test in Read -> separate ReadArray with `size <= remaining` (fixed the compare), nested-if ReadArray, template count ReadArray | next: lane gap: register choice at a join
- 00cddcb0 CPersonalScriptMainScript::Main DIFFER(1605v1475): ours calls ~CDefPointer and ~list out of line at the LAST return ('already active') and keeps m_p=0 on the normal path, so the region exits do not cross-jump | tried: allocator(){}, null-checked free, clearing CDefPointer dtor, merged arm, inline_depth(0) at the first return (fixed that one), __forceinline ~CDefPointer (worse) | next: find what spends the inline budget before the last return
- 00d20a40 CQ_BreakSiegeScript::WatchForOpenGates DIFFER(1433v1478): two returns need the CTimer dtor inline but the thing and vector dtors out of line | tried: one pragma state per return | next: __forceinline ~CTimer under inline_depth(0)
- 00f05a10 CQ_AmbushTradersScript::RefreshSpyAllegiences DIFFER(1312v1312): the vector-base, Game and offset loads around the two EntitySetThingAsEnemyOfThing(GetHero()) calls are scheduled differently, which shifts one alignment pad | tried: (earlier session) argument and temporary forms | next: lane gap: scheduling
- 00497b60 CProgressDisplay::CalculateNextTextTag DIFFER(1060v1052): this-pointer spill slot order and register choice around the EndsWith chain | tried: (earlier session) | next: lane gap: spill-slot order
- 00756d00 CPersistContext::TransferMap<CObjectAugmentationParticleSet,EObjectAugmentationType> DIFFER(1120v1116): retail loads both pair words in each branch and shares the two stores | tried: if/else-if copy, node local, empty traits + value, pointer select, guarded ternary | next: lane gap: found-branch store sharing
- 007ae3d0 CPersistContext::TransferMap<...> DIFFER(1104v1104, frame 0x74v0x70): retail aliases the empty CPersistTraits<bool> `this` with &start | tried: block local, temporary, six declaration positions | next: lane gap: empty-object slot aliasing

Decoded only and not compiled, per the brief's ">1 KB x87 / fistp / low-yield" rule (10). Each is DIFFER `-v<len>` in STATUS.tsv:
- 00aed600 CParticleEmitter::InterpolateTo: bitfield interpolation that rounds with inline-asm `fld/fistp` (no __ftol2). Asm-only, so it is a lane gap.
- 00a17af0 / 00a182c0 CPhysicsMesh::GetSphereIntersectionFromToUsing{Segmentation,FullCollision}: about 170 x87 ops each.
- 00a12ee0 CPhysicsMeshObject::GetNavigationLevelVolumes: x87 plus CArray/vector templates and quickselect.
- 00487570 CPlayer::DrawFreeCamDebug: x87 debug text (9 __ftol2, CWideString temporaries).
- 00497fd0 CProgressDisplay::DisplayCenteredText: static x87 drop-shadow text. Its sibling 0x00497B60 is already parked.
- 009bbcc0 CPersistContext::TransferSubComponentLoadText: CStringParser loader with flag-guarded temporaries and error strings.
- 00af4900 CParticleSystem::SerializeComponents: inline CRC32-table hash, inline CCharString compares, CMemoryDataInputStream.
- 00a6cc10 CPhysicsMesh::GetMeshDataFromFileData: /GS, propagated-label helpers, vector templates.
- 00a6a670 CPreallocTexturePool::ResizePool: inline vector erase/insert_n, inline CPreallocTexture ctor, counted-pointer swaps. Fully decoded in probe/a6a670.c.

## Skipped
- None. No row holds two functions.

## Label corrections
- 0x00DCC770 is CQ_OrchardFarmRaidScript::Main, not CQ_GuildTrainingScript::Main.
- 0x00DABAC0 is CQ_NewOakValeIntroScript::Main, not CPersonalScript_GlobalThingsScript::Main.
- 0x00E0EA40 is CQ_WaspBossScript::Main, not CQ_GuildTrainingScript::Main.
- 0x00D848D0 is CQ_HeroSoulsMotherScript::PlayCutscene, not Main_InitializeFourierAnalysis_1.
- 0x00DB4F70 is CQ_NewOakValeIntroScript::StartBarrelTimer, not Script_Guard_Duty.
- 0x00E12330 is CQ_WaspBossScript::WatchForCutscene. 0x00E0F1D0, landed as EndMission, is spawned under the name "WatchForTermination".
- 0x00D39330 is CQ_FireHeartScript::PlayCutscene(CCharString), not Game_InitializeRound. 0x00D38400 is TriggerProphetComment, not CTCGameModeController_UpdateSunCount (both already noted in landed siblings).
- 0x00443F90 is vector<CEnginePrimitiveHandle>::resize, not CEnginePrimitiveRenderer::AddPrimitive.
- 0x0075F580 / 0x0075F6C0 are list<CCharString>::clear / ~list, not CNavigatorQueueManager::FrameUpdate / CActiveFile::OnReadFinished.
- 0x0099F690 / 0x0099F600 / 0x0099F570 are the three CCharString operator+ overloads (__fastcall, hidden result in ecx).
