# Hand chunk 295 (CQ_BreakSiegeScript .. CQ_MinionClifftopChaseScript): 35 of 40 byte-exact

All 35 graded RELOCATION_MATCH under the default RTM cl 13.10.3077 flags (no pragma/flag sweeps needed; three rows
carry a `// Leaf:` line). Every test passes under g++ and under VC7.1 in Wine. Quest scripts have no
rebuild/include/engine header. They use local models with `// Layout:` evidence: the retail offset is the
Ego_r.pdb offset - 0x14, less 4 per earlier PDB vector or map, because CScriptBase is 0x40 bytes in retail.

## What matched (35)
- **CQ_BreakSiegeScript::WatchForWinLoseConditions 00d20790**: watches the attackers, adds the siege leader once he is attacked, then frees the imprisoned leader. A pre-loop terminate test and the loop-bottom test share one exit block, which is the top-tested `for (;;) { if (IsActiveThreadTerminating()) return; ... }` form. `#pragma inline_depth(0)/()` toggled inside the body picks which return sites get the inline ~vector.
- **CQ_CinemaTestScript::ArenaIntroduction 00f02210**: scripts the hero and plays ARENA_HOH_INTRO. Uses the landed PlayCutsceneScript cast-map / handle / movie model, all inline.
- **CQ_FireHeartScript::Init 00d347b0**: resets the state and sizes the tables. The count is loaded before the temporary's ctor, which is the one-argument inline `resize(n) { resize(n, T()); }` wrapper.
- **CQ_FireHeartScript::RunThemeOverriding 00d35ed0**: haunted theme while in LookoutPoint. The loop test is written `IsLevelLoaded(...) != false`, giving test/setne.
- **CQ_GuildTrainingScript::KeepBookcaseExitRemoved 00d3c8f0, RunArrivalCutscene 00d44cb0, RunSaveXPCutscene 00d496f0, RunSaveXPCutscene2 00d49a20**: CScriptThing model of CV_ChapelOfEvilScript plus the cast-map/handle/movie model. CBaseObject needs its vptr: the handle ctor's `ecx = handle+4` gives this away.
- **CTheKraken::Main 00df4be0** (catalog: GuildTraining RunSaveXPCutscene2): waits for zero health, then the outro. `fcomp; test ah,0x41; jnp` is `while (!(health <= 0.0f))`.
- **CV_ChickenKickingScript::LookAfterOrganiser 00e631f0** (catalog: GuildTraining KeepTabsOnWhisper): re-creates the chicken master after each return to Oak Bay. The `!= false` wait form again.
- **CUnknownSystem1D8::Init 00c3fd20** (catalog: GuildTrainingSkillScript InitializeRenderer). Not a quest script: it is the 0x1D8-byte owner built at 0xC4D3B5.
  - Shape: copies or defaults the params, then QPF/QPC ticks per 44.1 kHz sample, then the device.
  - Each disabled or failed sub-system is wiped with intrinsic `memset`s of its 0x18-byte slots. Each memset expansion gets its own `xor ecx/edx` zero register, apart from ebx. Statement order places the ebx int stores.
  - The device's 0x80000000 field is stored before the vfptr because it lives in a non-polymorphic base.
- **CQ_GuildTrainingWoodsDepartureScript::DoMission 00d63dc0**: spawn model plus the mission-over wait.
- **CQ_GuildTrainingWoodsMeleeScript::Main 00d66620, WatchForTermination 00d66880** (catalog: HobbeCave): CinemaTest Main binding/spawn model. In WatchForTermination the five frame waits are written out, and literals go straight to `const CCharString&` parameters.
- **CQ_HangingTreeEvilScript**:
  - Init 00d68ff0, CleanUp 00d6b720, SpawnParty 00d6c4a0: spawn model with a flag word guarding the name temporaries.
  - DoMission 00d6bfa0: Introduction 0xD6D080 and Conclusion 0xD6D670 are named from the PDB method order.
  - MonitorRegionPresence 00d6cdc0 (catalog: HangingTreeGood): STLport `back()` as `*(end() - 1)` gives `add eax,-4`.
- **CQ_HangingTreeGoodScript**:
  - Init 00d6f020: twin of HTE Init.
  - RemoveParty 00d732f0: CRGBColour by value needs a `__forceinline` ctor and copy ctor. CCountedPointer::operator= reads Info and then Data first.
  - SpawnEnemies 00d736b0: the local CTimer needs the inline ~CTimer body, and the code uses `front()` rather than `[0]`.
  - WatchForHeroNearExit 00d76490: `__forceinline` ~vector.
- **CQ_HeroSoulsMotherScript::CreateScreamers 00d84630**: CScriptThing model; the CreateCreature result is discarded.
- **CQ_HobbeToothContestScript**:
  - Init 00d99e80.
  - DisplayTimer 00d9b420: CTimer inline model, with `inline_depth(0)` at the final return.
  - HeroKilledHobbe 00d9b8f0.
  - WhisperKilledHobbe 00d9bb50: an early `return` instead of a guarded block fixed both scheduling and register colouring.
- **CQ_MinionCampScript**:
  - Init 00d9e2e0 and Main 00d9e5e0: binding model.
  - DoMission 00da0230: a constant-false `if (false) SpawnFunction(...)` reproduces retail's dead flag store and the loop padding.
  - PhaseOne 00da05d0: both spawn pieces are `__forceinline`.
  - RunFirstWaveControl 00da0820: `inline_depth(0)` around the in-branch returns.
- **CQ_MinionClifftopChaseScript::Init 00da3100**: Init twin model.
- **CQ_RansomVictimScript::WatchForTermination 00de14a0** (catalog: HobbeCave): literals go to `const CCharString&` / `const CWideString&` parameters.

## Parked for Fable 5.1 (5)
- 00d38d20 CQ_FireHeartScript::RunVillagerComments DIFFER(529v526): retail lays out the loop-exit tail (inline ~vector free + DeregisterTimer) before the terminating-return copies; ours puts it after. It also uses `lea ebp,[ebx+eax]` where ours has `mov ebp,[esi+0x110] ; add ebp,ebx` for &Villagers[i] | tried: inline/out-of-line vector dtor, loop restructurings, inline_depth pragmas | next: top-tested `for (;;) { if (MissionSucceeded) break; ... }` and `Villagers.begin() + i`
- 00d6c830 CQ_HangingTreeEvilScript::CheckBanditFollowStatus DIFFER(607v607): the instructions are the same, but retail shares one terminating-return block between the inner and outer waits (only branch targets differ) | tried: original form, the advisor's top-tested form (worse) | next: lane gap: tail-merge
- 00d773e0 CQ_HangingTreeGoodScript::MonitorRegionPresence DIFFER(686v686): firstTime bool at [esp+0x12] where retail has [esp+0x13], because retail shares the slot with the inline push_back's __false_type temporary | tried: declaration placement before/after the vector, block scoping | next: lane gap: frame-slot sharing
- 00d8a240 CQ_HerosOldHouseScript::WatchForBooty DIFFER(610v610): same instructions; retail shares the terminating-return block between the Oak Bay waits | tried: original form, the advisor's b2 top-tested form (kept) | next: lane gap: tail-merge
- 00f0f910 CQ_AmbushTradersScript::WatchForArcherSpawnConditions DIFFER(599v599): the archer CScriptThing and faceMe C3DVector frame slots are swapped | tried: direct-init, a user copy ctor, an extra block, moving faceMe | next: lane gap: frame-slot order

## Skipped
None. No row holds two functions.

## Label corrections
- 00c3fd20 "CQ_GuildTrainingSkillScript::InitializeRenderer": wrong. It is an unnamed system Init(void* owner, const params*), `ret 8`, called from 0xC4D3B5 on a 0x1D8-byte object (ctor 0xC3E520).
- 00d496f0 "RunSaveXPCutscene2": is CQ_GuildTrainingScript::RunSaveXPCutscene (plays CS_GUILD_SAVEXP).
- 00df4be0 "CQ_GuildTrainingScript::RunSaveXPCutscene2": is CTheKraken::Main.
- 00e631f0 "CQ_GuildTrainingScript::KeepTabsOnWhisper": is CV_ChickenKickingScript::LookAfterOrganiser.
- 00f0f910 "CQ_GuildTrainingScript::WatchForSparrowKilled": is CQ_AmbushTradersScript::WatchForArcherSpawnConditions (spawned by ProcessInitialSpyExtraction 0xF0F680).
- 00d6cdc0 "CQ_HangingTreeGoodScript::MonitorRegionPresence": is CQ_HangingTreeEvilScript::MonitorRegionPresence. The HTG one is 00d773e0.
- 00d66880 "CQ_HobbeCaveScript::WatchForTermination": is CQ_GuildTrainingWoodsMeleeScript::WatchForTermination.
- 00de14a0 "CQ_HobbeCaveScript::WatchForTermination": is CQ_RansomVictimScript::WatchForTermination.
