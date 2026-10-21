# Hand chunk 254 (CTCTavernGameSpotTheAddition .. CThingBodyReorienter): 31 of 40 byte-exact

All 31 grade RELOCATION_MATCH under RTM cl 13.10.3077, and every behaviour test passes under g++ (tall.sh) and VC7.1
(tall.py). Special flags: 006cf920 needs `/GS`, 00ca6e10 needs `/EHsc`.

## What matched (31)
- **CTCTavernGameSpotTheAddition** 007d8c20 UpdatePointerScale: grows the pointer toward 1 while a selection is being made and shrinks it to 0 otherwise, at the def's PointerPhaseSpeed, clamped at 0.1. Each def lookup is a CDefPointer local through GetDef 0x5EA000. The appearance comes through the out-parameter GetInterface.
- **CTCTrapBase** 00788229 Trigger:
  - What it does: makes the explosion with the live hit and immune things excluded, makes the physical obstruction (the explosion's physics radius, 100 s) that ignores the immune things, calls the slot-30 TriggerTrap, then sets Triggered.
  - Same-TU noinline stand-ins for both IS_THING_ALIVE overloads and both GetTCDriver instantiations let VC7.1 pack the things and drivers into shared frame words.
  - A `const CTrapDef* def` local keeps the def in edi.
  - The immune list is indexed as `*(begin() + i)` (`lea ecx,[eax+ebx*8]`).
  - The TU sets `#pragma optimize("y", off)` and `#pragma optimize("s", on)`.
- **CTCTrespasser** 007baf00 LeavingRegion:
  - What it does: ends the open trespass deeds and removes those other than index 2, then the inside-building deed. It un-flags the trespassable with the leave frame, clears the thing and detaches the opinion source.
  - The fix was an inline `ClearOpinionSource(CThing*)` helper plus an inline `CWorld::GetOpinionReactionManager` getter. With them the parent loads into edi early and the whole register assignment matches retail.
- **CTCUnholyPower** 00794c10 FinishMove: the landed CTCDivineWrath FinishMove 0x0078F6E0 body, plus `DarkFacesEffect.GetPItem()->Kill(true)` and an inline `StopSound(unsigned long)` through GetInterface(&emitter, 0x10).
- **CTCVillage**
  - 0065935e OnHeroSlept: three walks that re-read end() through the spilled `this`.
  - 0065a28f GetNormalisedTimeLeftOnFines: a named region local, a CDefPointer raw-object ctor and inline static getters reached through `GetX()->`.
  - 0065b598 ~CTCVillage: the member pointer is uninitialised when the interface is absent (retail calls SetPVillage on the stale slot), and the CDefPointer release is inline.
  - 0065b6dd RemoveThingAndUpdateCameraPoints: same-TU noinline find bodies sink the key stores.
  - 0065c45a EnableGuards: the EnableVillagerDefTypes model with the guard filter.
- **CTCWallMountEffects** 007df4b0 OnWallMountUseAction:
  - The private IsAllowedToUseEffect is inlined, with the def and the frame rate in named locals.
  - The question is a CCharString assigned in a switch.
  - The No/Yes/"" substitutions are block-scoped named locals.
- **CTCWife**
  - 006ced50 GetAppearanceChangedNoticed: RTM unrolls the 16-pass loop by 4.
  - 006cf920 HaveSex (`/GS`): inline display-engine and def getters, plus a same-TU LowerBound that packs both keys and the scoreboard pointer into one slot.
  - 006cfaa0 FindMaritalHome: the filter is a temporary passed by const reference, which frees its slot.
  - 006d0410 GetGreetDialogueGroupName: an inline helper taking the parent thing, and the engine's inline LCG.
- **CTCWillLightning**
  - 007a07f0 ~CTCWillLightning: `emitter = 0` before GetPItem gives the `xor edi,edi` preload.
  - 007a16f0 InitialiseBeams: a same-TU GetTargets stand-in.
- **CTCWound** 00757b30 AddWound: a same-TU GetSubDef stand-in, and an intrinsic memcmp `CEntry::operator==` (`repe cmpsd`).
- **Scripts**
  - 00e035e0 CTE_BanditLeader::Main
  - 00e48eb0 CTauntingBandit::Init: by-value CScriptThing arguments built in their slots.
  - 00dcd1d0 CTeamSpawn::Init: CCountedPointer operator= loads both source words first.
  - 00ef3840 CTestPos::Main
  - 00eeea90 CTestTheInventoryScript::Main: an implicit-conversion temporary in the loop test.
  - The wait loops are written as `while (!test) { NewScriptFrame(); if (IsDone()) return; }`.
- **CTargetingDef** 0045d923 Transfer: one `lea/push/call` per field; the last one folds into `add esi,8Ch`.
- **CText** 0054ed90 GetDataFromDef (NUISystem::CText): an inline CDefString accessor returning by value, plus the inline CCountedPointer operator=.
- **CTextureRenderer** 00ca6e10 vfunc_39 (`/EHsc`): this is strmbase `CBaseRenderer::Receive`, with CAutoLock EH states.
- **CThing**
  - 004c8050 WorldUpdate: same-TU `list::remove` and LowerBound stand-ins, and the erase branches written out in full.
  - 004c8e50 SetAsDrawable: one out-parameter GetInterface per component, so the component comes back in eax.
- **CThingAICreature**
  - 008330d0 AutoSetupSexuality: one bool verdict, then constant bit stores in an if/else.
  - 00833390 GetSleepTimes: every case returns.
  - 00834760 WorldUpdate (catalogued as vfunc_19): the pack(1) CThingCreatureBase model, with the plain GetTC pick for the weapon trail.
- **CThingBaseDef** 00463950 operator=: the compiler-generated assignment (bitfield read-modify-writes).

## Parked for Fable 5.1 (8)
- 006594bb CTCVillage::UpdateCrowds DIFFER(363v362): frame-slot order (retail: name temp -8, crowd -0xc, player/key -0x10), the const GetTC result in edi vs ecx, sound-def release form `mov ecx,[ebp-4] ; dec [ecx+4] ; jne` | tried: same-TU impure bitset test + pure lower bounds, crowd/name/key local orders, release spellings | next: model the sound def as a CDefPointer member read through an inline getter
- 0065a075 CTCVillage::GetWarningStringForCurrentCrimes DIFFER(386v392): retail keeps the crime iterator in edi (homed [ebp-0xc]) and the deed effects def in ebx; ours enregisters the iterator in ebx and end in edi, reloads the def into eax and the loop entry jumps to the bottom test | tried: iterator/def local orders, for vs while loops, const_iterator, named end | next: a do/while with the empty-list test hoisted, or the effects def through an inline getter
- 0065a8ee CTCVillage::IsTimeToGoToSchool DIFFER(378v385): child in esi (ours edi), retail reuses the start/run argument slots for the school-time local and the animation string, pushes the ST_RUN/ST_WALK literal inside the branches after push -1 | tried: ternary vs if/else name, named school-time local, child local placements | next: two CCharString constructions in the if/else branches (one per literal)
- 0065b142 CTCVillage::EnableVillagerDefTypes DIFFER(386v386): retail loads this->ParentThing (`mov eax,[ebp-4]; mov ecx,[eax+4]`) right after `fld 50.0` before the pushes; ours after `push eax` | tried: named parent locals (branch/top/after nav), nav local, centre ref local, radius local, enable as arg, default args, pointer centre param, same-TU GetPosition (pure/impure), GetNearbyWanderPosition and bitset::test bodies | next: lane gap: scheduling
- 0065be6c CTCVillage::UpdateHornTasks DIFFER(336v336): pure ebx/edi swap (retail this in ebx; def temp / find_if result / new'd task in edi) | tried: found as temp/named/assigned/top-of-function, const_iterator, inline IsCurfewTime/AddHornTasks helpers, same-TU find_if body, dtor and def-constness variants | next: lane gap: register allocation
- 007df130 CTCWallMount::UnattachObject DIFFER(370v370): scratch registers only (retail eax for the interface-9 component and ecx for the hero lookup key; ours ecx/edx) | tried: GetTC if/ternary/typed forms, bool HasTC, named targeted/trophies locals, setter, reused locals, alive-test forms, decrement spellings | next: out-parameter GetInterface for interface 9 (the eax pick) with the plain pick elsewhere
- 007fede0 CTCUserActionPointBase::GetFreeActionPoint DIFFER(348v348): the find value copy lands in the dead `index` slot `[esp+2Ch]`; retail uses the dead `thing` slot `[esp+20h]` (shared with the iterator tag) | tried: Contains helper, rvalue casts, placements, named tag, same-TU Find (edx-preserved compare instead of edi), external Find 0x50E540 with `long value = index` copy, OnCreate-style operator=, erase/do-while/GFRandom forms | next: lane gap: frame-slot assignment
- 00863ca0 CThingBodyReorienter::FillBodyReorientationPossibleIntersecting DIFFER(361v358): retail aligned frame `sub esp,30h` keeps the C2DBoxI temporary in its own words (O+0x30) and sinks the vector's zero stores to `xor esi,esi` just before the search; ours `sub esp,20h` packs the box over the size float and zeroes the vector at the top | tried: vector/filter placement (top, block, before call), filter temporary, named box, placement-new null check, C2DCoordI centre | next: an inline STLport `_Vector_base(const allocator&)` ctor (wave-25 24i model) for the local vector, or the box built by a same-TU helper

## Skipped
- 004cd1e0 CThing::vfunc_30: the catalog row holds int3 padding followed by another function's bytes, so it is not one function (SKIP rule).

## Label corrections
- 0x00834760 is catalogued `CThingAICreature::vfunc_19`. It is `CThingAICreature::WorldUpdate` (Ego_r.pdb `?WorldUpdate@CThingAICreature@@UAE_NXZ`).
- 0x00CA6E10 is catalogued `CTextureRenderer::vfunc_39`. It is the inherited strmbase `CBaseRenderer::Receive(IMediaSample*)`.
- 0x00834530 has the manifest label "AnimationBlend_UpdateCombatants". It is `(anonymous)::CombatStressTestDebug` (__fastcall).
- 0x004C7A60 is `CThing::PeekThingSearchTools`.
- 0x0080C460 is `CTCDExplosion::Create`.
- 0x007FD4B0 has the manifest label "GetFOVToBeInToBeTargetable". It is `CTCDPhysicalObstruction::Create`.
- 0x0080CBF0 has the manifest label "CExplosionDriver::AddExcludedThing". It is `CTCDExplosion::SetThingAsImmune`.
- 0x007FD3D0 has the manifest label "CTCBarrierDriver::SetLifetime" and was landed as CAIStateGroupBuyFromSeller_BoughtSomething. It is `CTCDPhysicalObstruction::SetLifeTimeSeconds`.
- 0x007898A3 was landed as CTCTrapBase::SetThingAsImmune. It is `CTCDPhysicalObstruction::SetThingToIgnoreObstruction`.
- 0x007512C0 is a folded `GetTCDriver` (tests +0x28 & 2). It was landed as the CTCDExperienceOrb instantiation.
- 0x006C5920 has the manifest label "Clone...". It is the deed log's EndDeed (descriptive name).
- 0x006C59E0 has the manifest label "AIState_DestroyWithPointerCleanup". It is `CTCHeroOpinionDeedLog::RemoveDeed`.
- 0x007008C0 is `COpinionReactionManager::ClearOpinionSource(CThing&, bool)`.
- 0x00863E70 is `GetAllThingsInRoughArea<const CThing, vector<const CThing*>, CThingFilter_HasPhysicsMesh>`.
- 0x00661FF0 has the manifest label "CPixelShader::CountReferences". It is `CThingCreatureBase::GetSex`.
- 0x00686A70, 0x00686A80 and 0x00686D40 have the propagated label "GetDisplayEngine". They are `CTCBase::GetWorld`, `CTCBase::PeekWorld` and `CTCBase::GetPlayerManager`.
- 0x007EE540 has the label "IS_THING_ALIVE<...>". It is `CTCVillageMember::GetPVillage`.
- 0x00686D90, 0x0049D9D0 and 0x0099EAE0 have the label "_Cons_val<...>". They are `CTCBase::GetThingSearchTools`, `CWorld::GetCurrentPlayer` and `~CCharString`.
- 0x00686830 has the label "~CChunkedFileChunk". It is `CTCBase::~CTCBase`.
- 0x007A3910 has the label "remove_if<...>". It is `list<CCountedPointer<CLightningBeam>>::clear`.
- 0x0065F4E2 .. 0x0065F5AA were landed as "CVector range constructors". They are STLport `find` (__fastcall, input-iterator tag).
- 0x006BB390 has the manifest label "Game_IsTimeOfDayInRange". It is the game-time manager's time-of-day range test (thiscall).
- 0x00811380 has the label "GetPActiveStrikeEvent". It is the crowd interface's event gate.
- CTavernGameDef PointerPhaseSpeed is at retail +0x1A0.
