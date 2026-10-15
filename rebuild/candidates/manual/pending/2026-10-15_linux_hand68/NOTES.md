# Hand chunk 68 (CPlayer .. CQ_HangingTreeGoodScript): 36 of 40 byte-exact

All 36 sources in `src/` grade MATCH/RELOCATION_MATCH with the plain lane flags (no pragma, no /EHsc), their
leaves were checked against the object's symbols, and all 36 tests print `<addr>_TEST PASS` under g++ and under
the lane's VC7.1 cl.exe (`tall.sh`, `tall.py`). Most catalog labels in this chunk were BSim-propagated; the real
identities used in line 1 are listed under "Label corrections".

## What matched (36)

- **Quest-script watch loops** (CScriptBase::IsActiveThreadTerminating 0x00CB7940 + CGameScriptInterface slot 7
  NewScriptFrame through the quest's interface at +0x40; retail quest members sit 0x14 below Ego_r.pdb):
  - 00e1f3a0 CQR_EscortTraderScript::EscortTrader, 00d68cd0 CQ_GuildTrainingWoodsWillScript::EndMission,
    00d360e0 CQ_FireHeartScript::CheckGameEnding -- `while (!cond) { NewScriptFrame(); if (terminating) return; }`
    then the end action; VC rotates the loop (entry + bottom test, `lea ebx,[ebx]` pad). A trailing
    `IsActiveThreadTerminating();` with the result dropped becomes the tail `jmp` (EscortTrader).
  - 00f03270 CQ_AmbushTradersScript::WatchForQuestFailure, 00d15090 CQ_BountyHuntScript::WatchForQuestFailure,
    00d150f0 CQ_BountyHuntScript::WatchForQuestSuccess, 00d74ce0 CQ_HangingTreeGoodScript::WatchForMissionEnd --
    `while (!terminating) { if (cond) { if (terminating) return; Flag = true; } NewScriptFrame(); }`; the BountyHunt
    failure watcher leaves after the store (`IsActiveThreadTerminating(); return;` = tail jmp, branch placed after
    the loop `ret`); HangingTree sets ReasonForFailing through CCharString::operator=(const char*) 0x0099EFE0.
  - 00e69f90 CV_ChapelOfEvilScript / 00edbd10 CV_TempleOfLightScript ::WatchForQuestInfoDisplay -- the condition
    `DisplayQuestInfo ? (AttackedByHero || !Inside) : (Inside && !AttackedByHero)` (a ternary: both arms share one
    `test al,al ; jne`); the branch body is only the termination re-check (no store in retail).
  - 00d25c20 CQ_DragonBossFightScript::SpawnEnemies -- `MinionSpawningEnabled = true; if (++since >= between) { if
    (!terminating) { Summoner = true; since = 0; } }` (pre-increment gives `inc edx ; mov eax,edx ; cmp`).
- **Script entities / interface calls** (slot names from ghidra_out/cgsi_slots_aligned.tsv):
  - 00e23eb0 CQR_EscortTraderScript::ForceEnableWait, 00dd59f0 CPrisoner::Init -- a vtable fetched into ebx before
    an inner GetHero() call is the nested `GSI->F(GSI->GetHero(), thing, false)` / `F(thing, GetHero())`.
  - 00d2d7b0 CPermanentBarrier::CPermanentBarrier -- the landed Create_CVtable_* three-level chain (root interface
    pointer before the dead root vtable, CScriptThing copy after the base vtable); ParentClass in the init list,
    the two vectors' inline zeroing constructors, PMasterData a BODY assignment (stored last).
- **CPlayerGui / CPlayerManager / CGameCameraManager**:
  - 00437770 CPlayerGui::SetCrimeAsDisplaying -- `PCrime->FrameFullyOpenedOn = 0; if (PCrime->GuiMode != 0)
    PCrime->GuiMode = 2;` through an inline counted-pointer `operator->` (gives the `mov eax,edx` re-address).
  - 00449c30 CPlayerManager::PrepareForLoad -- `for (unsigned i = 0; i < size(); ++i) Players[i]->PrepareForLoad();`.
  - 006976b0 CGameCameraManager::ResetRightStickLookAroundVector -- the chained `a = b = C2DVector(0, 0)`: one
    8-byte frame temporary written with immediate zeros and read back into eax/edx.
- **Containers walked by index** (retail vectors 12 bytes; size re-derived each iteration):
  - 0098a7a0 CShaderRenderManager::Uninitialise (8-byte counted pointers, then `D3DDevice = 0`),
    00a773e0 CNavSwitchableLeafNode::IsEveryUniqueIDOpen (16-byte entries, flag at +8),
    00a78dd0 CNavQuadTree::UnblockAllSwitchableNodes (`Node->SetAsBlocked(ID, false)`),
    00a69ed0 CPreallocTexturePool::GetTexturesInUse (`Allocated && GetUnusedFrames() <= limit`).
- **Copies**: 00987f70 CPreTransposedBoneMatrix::Set (12 transposed float assignments -> integer moves),
  00f37970 CGPUScratchBufferTextureManager::CResourceEntry copy ctor, 009f9450 CProfile copy ctor,
  006b62b0 NEnvironmentInternal::CEnvironmentThemePriorityStackEntry copy ctor -- explicit memberwise init lists;
  floats copy through integer registers, doubles through the FPU, member objects through their copy constructors.
- **Constructors**: 005191e0 NThingMap::CThingMapManager (two CArrays + SGI `_Rb_tree` set: `header = 0; header =
  malloc(0x14)`, count 0, red header self-linked, header re-read per store), 009bdb00 CXMVPlayer(bool, unsigned
  long) (counted pointer + STLport list sentinel, args unused), 00a3aea0 CACompressor(long) (vector inline ctor,
  `resize(n, 0)` with the byte temporary in the `push ecx` slot), 00642390 NGraphicAppearance::CPrimitive,
  00879620 NCameraModes::CProjectileWeapon, 00bdefc0 CLocalDetailCacheMap (Map is a body assignment: stored after
  every member constructor), 00564b00 / 0071f3d0 `{0, member()}` records (_unattributed).
- **Projectile-weapon helpers**: 00879560 GetHeightParamForTargetedThingWithPhi, 008798c0 GetPhiForTargetedThing --
  the C3DVector from the `__fastcall` NTargeting::GetTargetingPositionOnThing (hidden result in ecx, world in edx)
  must be a NAMED local: the temporary form pushes eax and evaluates phi before the call; the local is
  re-addressed with `lea` afterwards. C3DVector needs a constructor for the `__fastcall` hidden return.
- **Other**: 00c3f510 CVtable_012b5c60::GetDataSize (null arm last = `if (p) { ...; return size; } return 0;`,
  locals taken before the first call), 00c3e260 `__cdecl` default-descriptor filler (_unattributed).

## Parked for Fable 5.1 (4)

- 004497e0 CPlayerManager::GetPlayerDefIndexFromNumber DIFFER(101v91): retail cross-jumps the NEUTRAL and SPIRIT `lea ecx ; call CCharString ; lea eax,[esp+8] ; push eax` blocks (HERO uses edx), the lane re-addresses with eax/ecx/edx and merges none | tried: explicit CCharString temporaries (73, all paths merged, ctor eax pushed), implicit conversions in three returns (101), if/else into a long local (101), `number != 4 && number == MainPlayer` + literal ternary (94) | next: lane gap: register allocation / tail-merge (RTM vs QFE)
- 00e24210 CQR_EscortTraderScript::InFinalRegion DIFFER(66v68): retail `mov eax,[ecx+eax*4] ; mov ecx,[ecx+114h] ; sub eax,4 ; mov eax,[eax]` vs lane `mov edx,[ecx+eax*4] ; mov eax,[edx-4] ; mov ecx,[ecx+114h]` | tried: operator== reading the left block first (70), right block first (66), back() via iterator `--it` (66) and `operator-(1)` (66) | next: lane gap: RTM vector-slot address materialisation (GOTCHAS 24l)
- 00ab37b0 CVertexStreamFormat::CalculateVertexSize DIFFER(44v58): retail keeps the index (`inc esi ; cmp esi,edi ; jb`) against a size re-derived in the preheader (`mov edi,[ecx+8] ; mov eax,edx ; sub edi,eax ; sar edi,3`), the lane reverses the counter (`mov edi,eax ... dec edi ; jne`) | tried: unsigned index with GetDataTypeSize defined in the TU (noinline, needed for edx live across the call), signed index, callee defined after the caller | next: lane gap: IPA-known callee + loop-counter reversal (QFE optimiser)
- 00c70a60 CopyAligned128 (catalog CPreTransposedBoneMatrix::Set) DIFFER(94v91): retail `push esi ; push edi ; mov esi,[esp+10h] ; mov edi,[esp+0Ch] ; movdqa xmm0..7` (inline-asm register discipline) vs intrinsics `push ebp ; mov ebp,esp ; ... and esp,-16` with rotated xmm order | tried: _mm_load_si128/_mm_store_si128 x8 | next: lane gap: inline-asm original (asm_bake only)

## Skipped

None (no row spans two functions).

## Label corrections

Propagated manifest labels found wrong (line 1 of each source carries the identity used):

- 00c3f510 CPlayer::GlobalEventCreatureDied -> slot 13 of the no-RTTI vtables 0x012B5C60/0x012B5CD0/0x012B5D70/0x012B8AE0 (named CVtable_012b5c60::GetDataSize).
- 0098a7a0 CPlayerManager::PrepareForLoad -> CShaderRenderManager::Uninitialise (callers: the shader render manager dtor and CDisplayManager dtor; Ego_r.pdb name).
- 00a773e0 CPlayerManager::PrepareForLoad -> CNavSwitchableLeafNode::IsEveryUniqueIDOpen (callers Unlock / SetAsBlocked).
- 00a78dd0 CPlayerManager::PrepareForLoad -> CNavQuadTree::UnblockAllSwitchableNodes.
- 00ab37b0 CPlayerManager::PrepareForLoad -> CVertexStreamFormat::CalculateVertexSize (calls GetDataTypeSize 0x00AB3740).
- 00c70a60 CPreTransposedBoneMatrix::Set -> a free `__cdecl` 128-byte SSE copy (inline asm).
- 00f37970 CPreTransposedBoneMatrix::Set -> CGPUScratchBufferTextureManager::CResourceEntry copy constructor.
- 005191e0 CPreallocTexturePool::CPreallocTexturePool -> NThingMap::CThingMapManager::CThingMapManager (vtable 0x01244678 RTTI).
- 009bdb00 CPreallocTexturePool::CPreallocTexturePool -> CXMVPlayer::CXMVPlayer(bool, unsigned long) (vtable 0x0129B3BC).
- 00a3aea0 CPreallocTexturePool::CPreallocTexturePool -> CACompressor::CACompressor(long) (vtable 0x0129B424).
- 00642390 CPrimitive::CPrimitive -> NGraphicAppearance::CPrimitive (not CTriangleStripifier::CPrimitive).
- 00564b00 / 0071f3d0 CProfile::CProfile -> small `{long, CWideString}` / `{long, CDateAndTime}` record constructors (likely std::pair default constructors; landed as _unattributed).
- 006b62b0 CProfile::CProfile -> NEnvironmentInternal::CEnvironmentThemePriorityStackEntry copy constructor (48-byte vector element).
- 00879620 CProjectileWeapon::CProjectileWeapon -> namespaced NCameraModes::CProjectileWeapon (label right, namespace added).
- 00bdefc0 CProjectileWeapon::CProjectileWeapon -> CLocalDetailCacheMap::CLocalDetailCacheMap(CEngineMap*).
- 00d2d7b0 CProjectileWeapon::CProjectileWeapon -> CPermanentBarrier::CPermanentBarrier (vtable 0x012CC48C, 0x34-byte entity script).
- 00e69f90 / 00edbd10 CQ_BountyHuntScript::WatchForQuestSuccess -> CV_ChapelOfEvilScript / CV_TempleOfLightScript (spawned by their Main; method name WatchForQuestInfoDisplay is descriptive).
- 006976b0 CQ_GuildTrainingSkillScript::Init -> CGameCameraManager::ResetRightStickLookAroundVector.
- 00c3e260 CQ_GuildTrainingSkillScript::Init -> free `__cdecl` default-descriptor filler of the 0x00C3xxxx module (_unattributed).
- Callee labels: 0x00487550 `_Dest_val<...>` is CPlayer::PrepareForLoad; 0x0084D050 CPixelShader::CountReferences is NCameraModes::CBase::GetWorld; 0x0084E420 CActionParam_Constructor is NCameraModes::CBase::CBase; 0x007EA2F0 Creature::GetTargetFirePoint is NTargeting::GetTargetingPositionOnThing; 0x00F024C0 CQ_HangingTreeGoodScript::Main is most likely the ambush-traders Main (its watcher's offsets only fit CQ_AmbushTradersScript); landed 0x00697710 GetNoControlMovementTime reads retail +0xc0, which by the -0xc shift seen in 006976b0 is most likely ControlNoControlMovementStartFrame (PDB +0xcc), not InsideHeroCount.

Header policy: 7 sources still define a struct whose engine header exists (CPlayerGui, CPlayerManager,
CThingMapManager, CGameCameraManager, CACompressor, CNavQuadTree, CLocalDetailCacheMap) because those generated
headers carry the Ego_r.pdb offsets/widths that disagree with the retail bytes (or flatten the vptr); each source
says so. CScriptThing, CPlayer, CPixelFormat, CManagedTextureData and CNavSwitchableLeafNode come from the headers.
