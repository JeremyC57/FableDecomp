# Hand chunk 288 (CGameScriptInterface .. CHeroStatsDef): 24 of 40 byte-exact

All 24 are RELOCATION_MATCH under RTM cl 13.10.3077. Each has a behaviour test that passes under g++ and VC7.1 (tall.sh / tall.py).
There are no /GS, /EHsc or jump-table flags on any matched row.

## What matched (24)
- **CGameScriptInterface**
  - 0x008A9100 CreateCreature. The init's two CCharString default arguments are `""` literals, which become lea-passed temporaries; writing `CCharString("")` passes eax instead. `result` is declared before the def pointer. The appear action is a 0x114-byte object. GetDef 0x00513160 is a same-TU stand-in.
  - 0x008A1360 GetStealDuration.
    - `itemDef && itemDef->GetSubDef(stealDef)` uses same-TU stand-ins for 0x005130A0 and 0x0059350C.
    - The thing's def is fetched twice through an inline by-value `GetDef()` inside one `&&`, so the two conditionally destroyed temporaries produce retail's flag word.
    - The three floats are named locals in retail's load order (Min, Max, then the skill cap).
    - Matched in 2 grades.
  - 0x0089EF90 GetNearestEnabledDiggingSpot. Reuses the landed sibling GetFurthestWithDefName 0x0089AA70 model verbatim, with a `long` best distance starting at -1 and a `found` flag. Matched on the first grade.
- **CGraphicInfo / CGraphicInfoFrame**
  - 0x0041C260 CGraphicInfoFrame::GetFrame (the catalog says CGraphicInfo::GetFrame).
    - Intrusive releases are member DecRef functions on a cached `this`, which gives `dec [esi]; jne` with no re-read.
    - `~T(); operator delete(this)` keeps the ??_G call out.
    - The low-mip test is `<` with the early return in the else arm.
    - A single BeginLoading statement after the if becomes retail's shared tail.
  - 0x009FF8D0 CGraphicInfo::GetFrame(out, frameIndex, mip, flags). It is `Frames[frameIndex].GetFrame(...)` with the same model and the inner method `__forceinline`.
- **CGameflowScript / CGameflowAssistanceScript**
  - 0x00CEF550 CheckBarrowFieldsGuards: construction-flag temporaries in `!A && !B`.
  - 0x00CEFCC0 Tutorial_CombatMultiplier: an unrotated `for (;;) { if (flag) break; ... }` loop.
  - 0x00CF02A0 Tutorial_RenownLevelIncrease: a `CCharString[8]` array whose three terminate exits sit in `inline_depth(0)` regions.
  - 0x00E76040 DeclareGossipCategories: implicit `const char*` conversions, so all temporaries are built before the first push.
- **Quest scripts**
  - 0x00D6B240 CHTE_Audience::Main.
  - 0x00D72200 CHTG_Audience::Main.
  - 0x00D72590 CHTG_Boss::Main: shrink-wrapped ebx.
  - 0x00D73C70 CHTG_AmbushPointBackup::Main: inline devirtualised GetPos, and a swap through a temporary declared before the loop.
  - 0x00D7F880 CHSA_ScreamingVillager::Main.
  - 0x00DD84B0 CGuardMotherEscape::Main.
  - 0x00E5F2B0 CHenchmanComment::Main: the counter sits in its own block, and one index serves both loops.
  - 0x00E5F8F0 CHenchmanRegionExitComment::Main: forceinline ~CScriptThing on the empty argument temporary.
  - 0x00EEC410 CGlobal_GiveHeroItemsFromRewardChestScript::Main.
  - 0x00EF45B0 CGhostDude::Main.
  - 0x00EFD740 CGift::Main: a frame bool, with edi and ebx shrink-wrapped.
- **Def transfers**
  - 0x004665B6 CHeroCombatDef::Transfer: HeavyWeapons is moved ahead of the FOV values.
  - 0x00472506 CGlobalDef::Transfer: declaration order, with no base transfer.
  - 0x00479CBA CHeroStatsDef::Transfer: Vandalism comes before Picklock.
- **CGlyph**
  - 0x00AB96B0 CGlyph::Read: seven inlined stream reads into block-scoped locals homed in the dead `in` slot.

## Parked for Fable 5.1 (8)
- 0x006d8f10 CHeroExperienceDef::Transfer DIFFER(693v692): retail pushes ebp late (shrink-wrapped around the StatIncreasePerHitType transfers); ours saves it in the prologue | tried: the landed CCreatureGeneratorDef::Transfer inline-transfer template, transfer ordering | next: lane gap: ebp shrink-wrap
- 0x006dd1c0 CHeroExperienceDef::ReadFromStream DIFFER(520v520): every instruction matches; only frame offsets differ. Retail packs the count local, the key-traits temporary and the 8-byte insert_unique result into one 8-byte slot (sub esp,34h); ours gives the result its own slot (38h) | tried: a 12-byte STLport map (fixed `and esp,-8`), an inline ReadCount helper (fixed the dead store), a block-scoped traits temporary | next: lane gap: frame packing (try the wave-25 real `_Vector_base`/`_STLP_alloc_proxy` model for the map traits)
- 0x00704860 CGameScriptThingManager::CreateScriptThing DIFFER(538v531): the not-found path keeps an extra unused 8-byte slot between `result` and the counted pointer (sub esp,20h) and uses esi as its zero register | tried: by-value uid PushBack, a not-found `{}` block, local ordering | next: real STLport list-node allocator model (wave-25 24i lesson)
- 0x0089ce70 CGameScriptInterface::IsThingAwareOfOtherThingInAnyWay DIFFER(610v616): x87 residue. Retail squares the offset Z, X, Y and re-sums cached squares for the inline fast Normalise, while VC7.1 canonicalises the sum and keeps the whole length. The std::min lea order differs, and VC merges the two `return true` epilogues | tried: Min arg/if forms, `<=`/`!(<)`/`>=` range tests, Y*Y+X*X+Z*Z, facing as const-ref, a double cos local, cos-left compare (9 grades) | next: sequenced square statements; lane gap: x87 sum canonicalisation
- 0x0089feb0 CGameScriptInterface::AddCrimeCommitted DIFFER(567v559): VC7.1 keeps `victim` in ebx and spills criminalThing; retail keeps criminalThing in ebx, re-reads victim from its slot and keeps the deed index in the dead criminal slot | tried: ternary init, late deed declaration, the loop as a forceinline TC member, a named group local | next: lane gap: locals in dead argument slots with no frame
- 0x008a7a70 CGameScriptInterface::TeleportAllFollowersToHeroPosition DIFFER(721v738): the hero is in ebx with an edi zero register (vector init by register); retail keeps the hero in ebp, spills it for the final loop, never uses ebx and zeroes with immediates | tried: forceinline copy ctor/push_back, a virtual-dtor pointer model, an aliased hero local | next: wave-25 named-pointer-local lesson; lane gap: regalloc
- 0x00d6aea0 CHTE_AssaultGuard::Main DIFFER(721v721): retail hoists one `lea ecx,[esp+0x10]` above the inner wait loop's terminate `jne` | tried: inline helper, const-ref, pointer, if+do-while, for(;;)+named bool; an advisor found no fix | next: lane gap: scheduling
- 0x00d701c0 CHTG_FreedomFighter::Main DIFFER(850v850): the frame slots of the exit point and its copied position are swapped | tried: declaration order, nested blocks, function-scope pos, a user copy ctor, a named bool; an advisor found no fix | next: lane gap: frame slot order

## Skipped
- 0x0089e190 CGameScriptInterface::CameraUseCameraPoint (803 bytes): a large camera-point x87/right-handed-set body, not reached within the budget.
- 0x008a93a0 CGameScriptInterface::EntityWillTeleportToArea (620 bytes): a random wander-point pick (inline LCG, a 16-byte by-value struct, vector erase, a creature action, SetCurrentAction), not reached within the budget.
- 0x00875690 CGeneralCircle::CGeneralCircle (762 bytes): x87 circumcircle geometry (cross products, two Normalise calls, fsqrt); x87 scheduling residue expected.
- 0x009f83d0 CGraphicDataBank::CreateAndInitialize (693 bytes): list search with a CCharString compare, a 0x30C new, a counted pointer, 44-byte struct copies (rep movsd) and a list insert; not reached within the budget.
- 0x00a02df0 CHandleBasedMemoryPool::PartialDefragment: a known lane gap (whole-program register caching). The call to 0x00A01F50 is followed by a thiscall to 0x00A02020 without reloading ecx.
- 0x00c28870 CHandleBasedMemoryPool::PartialDefragment: the EH-framed (/EHsc) twin of 0x00A02DF0, with the same caching.
- 0x008f61a0 CHeightMap::Resize: an LTCG-style register-argument callee (0x008F6740 takes ecx/edx plus the stack), an __int64 overflow guard and std::min/max reference selects; not reached within the budget.
- 0x00cdd6b0 CGlobal_TeleportToHeroGuildScript::Main (1012 bytes): a large script with x87 distance tests; not reached within the budget.

## Label corrections
- 0x00CEFCC0 is CGameflowAssistanceScript::Tutorial_CombatMultiplier, not Tutorial_LowHealth.
- 0x0041C260 is CGraphicInfoFrame::GetFrame(out, mip, flags), not CGraphicInfo::GetFrame. 0x009FF8D0 is the CGraphicInfo wrapper.
- 0x0074DBB0, labelled "CCombatAction_Guildmaster_HealFollowersInto", is propagated. CreateCreature uses it as the 0x114-byte appear-action ctor.
- 0x006CB6E0 has a stale label `?DeactivateThemeDensityDisplay@CEditControlCentre@@` in one labels file. It is CTCCreatureNavigation::GetNearbyWanderPosition.
