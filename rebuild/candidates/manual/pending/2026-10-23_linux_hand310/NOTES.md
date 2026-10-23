# Hand chunk 310 (CV_ChapelOfEvilScript .. CVictimsBrother): 38 of 40 byte-exact

All 38 matched rows grade RELOCATION_MATCH (gall.sh). Their tests pass under g++ (tall.sh) and VC7.1 (tall.py).

## What matched

- Quest-script mains and watchers, written in translator style (a terminate check before every statement; WAIT loops):
  00e62a00, 00e82220, 00e8b9c0, 00ea8830, 00eb6d70, 00ed10a0, 00ed4d20, 00d2f7f0, 00e1a990, 00ea1090, 00ea1360,
  00eb70f0, 00eb75f0, 00eb94a0, 00e98fb0, 00e99470, 00e9a270, 00da3f40, 00e7a740, 00e7b050, 00ea8c20, 00e9dc80,
  00ed7320, 00edbd80, 00edcb80, 00e9cb20.
- Predicates and text helpers: 00e97fd0, 00e98200, 00eb03a0, 00e82f80, 00e6de10.
- Cut-scene and handle rows, using the cast map, movie and RunCutsceneMacro models:
  - 00e9aeb0, 00ee2db0, 00deb000 and 00e6da80.
  - 00e6da80 TortureVictim builds both the cast map and the `$SOUND` argument map inline (malloc'd header, inline clear + free).
- Rows that needed callees defined in the TU under `auto_inline(off)`: 00ee2a80 DoConversation and 00ec2240 ProcessAlbionPopulationSim.
- 00e8cb40 CreateBandit:
  - The outer wait is `while (!IsActiveThreadTerminating())`, with the necklace test first and the post-loop statements inside its branch.
  - CSpawnedFunc1<T, CScriptThing> is a 0x48-byte object with a by-value arg copied to +0x3c. Its vtable is 0x012EBC50 and its body is CheckBanditDead 0x00E8CF40.

## Parked for Fable 5.1

- 00ed94d0 CV_TalentlessBardScript::WatchForResetWanderCentre DIFFER(689v689): one instruction differs. After the inlined `bard.IsAlive()` VC7.1 emits `cmp al, bl` (ebx = 0) where retail has `test al, al` | tried: about 14 IsAlive / condition forms (&&, if/return-true, ternary, bool local, `== true`, `!!`, impl-class model, for(;;), centre pointer local); every form that restores the test reschedules the Game load into the by-value C3DVector copy and swaps the cleanup copies | next: try an out-of-line-declared IsAlive with a separate inline wrapper, or a predicate object, to change when the zero register gets assigned.
- 00ad9f10 CVertexGroupBlockOptimiser::AddBlocks DIFFER(826v806): the control flow, calls, frame (0x40) and stack slots all match. Register allocation is the difference:
  - Retail gives the hoisted zero to ebp, pushed late after the early returns, and &OptimalBlockList to ebx.
  - Ours gives the zero to ebx, so ebp is pushed in the prologue and the inner group loop is not strength-reduced to a pointer plus count-down.
  - tried (9 variants):
    - index vs inline HasMoreBlocks with a cached count;
    - inline CountNewGroups / HasGroup (forceinline) and a `current` block local;
    - reusing the `start` parameter slot for the counters (this fixed the frame size);
    - first/last locals for the candidate vector;
    - a reference local for OptimalBlockList (worse).
  - next: shape the j-loop so that ebp is reused as the group pointer (an explicit `const unsigned long* p` / count-down loop), and try a `CVertexGroupBlock::CountNewGroups` member taking the block's group array.

## Skipped

None. Every row was attempted.

## Label corrections

- Row labels that are wrong in the catalog (each source has a `// Leaf:` line):
  - 00d2f7f0 is CQ_EndGameBossBattleScript::Main, not GhostGranny::Main. Its body is 0x00D30030.
  - 00da3f40 is CQ_MinionClifftopChaseScript::ProcessMission, not HeroDuel::WatchForHeroEnteringCave.
  - 00e1a990 is CQ_WizardBattleScript::Main, not MurderTwist::Main. Its Init 0x00E1AD30 is mislabelled ChickenKicking.
  - 00edbd80 is CV_TempleOfLightScript::WatchForInsideTemple. The catalog name CV_ChapelOfEvilScript::WatchForInsideChapel belongs to 0x00E6A0B0.
  - 00edcb80 is CV_TempleOfLightScript::WatchForAcolyteHit(CScriptThing), not ChapelOfEvil.
  - 00ee2db0 is CV_TravellingHeroesScript::PlayCutscene(CCharString), not ExposeMayor (that one takes CCharString, bool, bool).
  - 00deb000 is CQ_RansomVictimScript::CVictimsBrother::DealWithHeroHittingMe (nested class).
- Callee labels:
  - 0x00EB7370 is WatchForTermination.
  - The CTriangleList Alloc labels on the entity-script allocators are wrong.
  - 0x00EC2520 is SetCurrentPopulationRegionFromRegionName (manifest: CScriptBase_ProcessAlbionPopulationSim_Filter).
  - 0x00CD3EF0 is GFGetPDefinitionManager (manifest: GFGetPreMainMemoryUsed).
  - AddBlocks callees:
    - 0x00ADB570 is list<CVertexGroupBlock>::size (manifest: CGuiControlTreePane::SortTreeRecursively).
    - 0x00A96DA0 is ~CVertexGroupBlock (manifest: CEngineScreenEffectOutlineGlow::FinishWithDynamicTextures).
    - 0x00AD9030 is CVertexGroupBlock::CVertexGroupBlock.
    - 0x00ADC790 is list<CVertexGroupBlock>::pop_back.
    - 0x00ADD020 is list operator=.
    - 0x006699C0 is map<unsigned long,long>::find.
