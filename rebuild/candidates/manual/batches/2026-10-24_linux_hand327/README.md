# Hand chunk 327 (CTCTrophy .. CThingCreatureBase): 14 of 40 byte-exact

All 14 sources in src/ grade RELOCATION_MATCH under the RTM cl 13.10.3077 lane (gall.sh -> grades.txt). Each test/ file
calls the candidate and prints `<addr>_TEST PASS` under both g++ (tall.sh) and VC7.1 + Wine (tall.py).

## What matched (14)
- **CTavernGameDef / CTavernGameShoveHaPennyDef** -- 004ed472 `CTavernGameDef::Transfer`, 007d0a40
  `CTavernGameShoveHaPennyDef::Transfer`: def persistence through the inline `CPersistContext::Transfer<T>` model of the
  landed CShoveHaPennyRegionDef::Transfer. The leaf needs `class CPersistContext` (a `struct` changes the mangled name).
- **CThingBaseDef** -- 00676be0 `Transfer`: twenty named flag bits. Each flag is a block-scoped
  `{ bool flag = X; context.Transfer("X", flag, def); X = flag; }`. A single flag local at function scope moves the traits
  temporaries onto the default's frame byte. The default-constructed CThingComponentSet temporary leaves a `delete[] 0`
  behind, so global `operator delete[]` has to be declared.
- **Entity-script Mains (CTC_*, CTH_*, CQ_*)** -- 00df9180 Villager, 00dfb320 BanditGuard, 00df8970 BanditFighter, 00df80b0
  BanditFollower, 00edf3a0 ThunderBoy, 00edfa00 ThunderGirl, 00ee1e40 Maze, 00ee1500 MagicBarrier, 00d50c60
  TheRealGuildmaster. These use the landed GameCalls / CScriptThing slot models. The forms that mattered:
  - CTimer SetTime/GetTime are `__forceinline`.
  - Write `if (x < 1) ++x; else x = 0;` in that order.
  - A by-value CScriptThing argument goes through the InlineCopy tag ctor, with the counted pointer as a member class that
    has an inline copy ctor.
  - GetMasterData() is an inline accessor, which puts the load in eax.
  - To keep IsAlive virtual, bind a non-const reference to the CreateEffect temporary (VC extension). A value local gets
    devirtualised and inlined instead.
  - Spawned functions go through an inline `SpawnParentFunction` wrapper, which re-reads ParentClass after `new`.
  - A CRGBColour built in place as a by-value argument needs a user copy ctor.
- **CThingCreatureBase** -- 00663f50 `OnDie`, in four parts:
  - The EXPLODE_ON_KILLING_BLOW effect, through GetSubDef/GetEffectFromString declared only. A same-TU stand-in lets the
    def slot share a frame word with the CCharString temporary.
  - The dead-creature body. Copy the five components and the lighting channel through a const
    `GetConstInterface(&out, type)` out-parameter form. A plain `if (HasTC) ...GetConstTC` sinks the component load and
    rotates its register. Read the mode, player number and def index into locals before `CThingDeadCreature::Create`. Route
    the body's physics TC through an inline accessor (keeps the object in edi).
  - The death message, with the physics TC and world as locals.
  - Killer and died events. Fetch each entity-events TC before its `new`.
- **NUISystem::CTextSlider** -- 00549b20 `Initialize` (def-driven), on the landed CScrollableList::Initialise model:
  - The action parameter goes through the real STLport `list::push_back -> insert(end()) -> _M_create_node` chain.
  - The default action uses the inline STLport `vector::push_back` with a same-TU noinline `_M_insert_overflow` body that
    only reads the value. The count block then stays in edi across the call, and the empty `__true_type` tag (with a user
    ctor) is not stored.
  - CAction() and CCountedPointer(CAction*) are plain in-class inline. VC7.1 keeps the first four uses out of line
    (0x0042BE50 / 0x0042AA29) and expands the last one, as retail does.

## Parked for Fable 5.1 (5)
- 00ee2490 CTH_Thunder::Main DIFFER(1334v1334): one coalescing in the seh_Rival release, retail `mov eax,[esi+14h] ; lea ebp,[eax+48h]` v ours `mov ebp,[esi+14h] ; add ebp,48h` | tried: direct/temporary spawn forms, a GetParentClass() accessor for the handle address | next: lane gap: register coalescing (scheduling-only)
- 00d43db0 CQ_GuildTrainingScript::CPreMeleeMaze::Main DIFFER(1665v1799): the loop's terminate exit calls the handle/marker destructors out of line (retail expands both inline there) and the first movie ctor zeroes through ebp instead of `xor eax,eax` | tried: natural inlining (2016v1799), inline_depth(0) regions, inline_depth(255) restores, for(;;) with a top terminate return | next: per-call-site inline budget; try __forceinline on only the exit-path dtors with the movie blocks' ctors kept plain
- 00832260 CThingAICreature::InitialActivate DIFFER(1052v1076): register allocation around the family component. Retail keeps the thing manager in ebx, spills the family pointer to the lookup-key word [esp+14h] (frame 0x10) and reloads the low UID dword; ours keeps the family pointer in ebx and the manager in edi (frame 8) | tried: plain inline helpers (685v1076), __forceinline helpers/ctors, the UID by const& | next: the const GetConstInterface(&out) out-parameter form that fixed 00663f50 for the family lookup
- 00830fe0 CThingAICreature::SerialiseGameState DIFFER(1505v1504): the empty CPersistTraits<float> temporary of the three InitialPos transfers sits at [esp+1bh] (retail [esp+20h], hasRelationships' byte in the context arg slot); the bit-7 EnableCreatureAutoPlacing write-back loads the flag before the member byte | tried: named traits local in the template, early `return false`, two-arg float Transfer, inline/__forceinline C3DVector::Transfer | next: lane gap: empty-temporary slot choice; try a C3DVector Transfer<C3DVector> traits specialisation
- 008329b0 CThingAICreature::OnHit DIFFER(1208v1187): frame 0x3c v 0x38. Retail shares one word [esp+18h] for the lookup keys, opinion and tolerance bool. The imposter is in edi (retail ebp). The common PostDeed tail pushes are not hoisted above `cmp eax,-1`. The ability-def release is a register read-modify-write (retail `dec [ecx+4] ; jne`) | tried: CDefPointeeBase/ReleaseDef dtor, explicit release through a local, the CDefBase layout | next: the GetConstInterface out-parameter form for the 0x49/0x4f/0x51 lookups, a C3DVector with a user copy ctor for the imposter position

## Skipped
None of the skipped rows holds two functions; I checked every row for int3 padding followed by code. These 21 were not
attempted in this session: the time went to the cheaper rows first. All are large (1070-1833 bytes) and mostly STL- or
x87-heavy:
- 006de280 / 006de8c0 / 006deee0 (CTCTrophy)
- 007c6e50 (CTCTurncoatSpell::FrameUpdate)
- 00659800 / 0065b854 (CTCVillage)
- 00785620 (CTCVolumeContainmentTracker::Update)
- 007b5e50 (CTCWaterWader::FrameUpdate, decoded only)
- 0076eb00 / 0076efa0 (CTCWeaponTrail)
- 006d1250 / 006d18a0 (CTCWife; Marry decoded only)
- 007a0d90 (CTCWillLightning::GetTargets)
- 00550dc0 (NUISystem::CTable::Draw)
- 00dcd350 (CTeamSpawn::Main)
- 004c84b0 / 004c9120 / 004ca390 / 004caa00 (CThing)
- 008637d0 (CThingBodyReorienter)
- 00665860 (CThingCreatureBase::UpdateAnimation)

Decoded facts for the next pass on 00dcd350 CTeamSpawn::Main. In the parent CQ_OrchardFarmRaidScript:
- Retail offsets: DoneIntroduction +0x50, WhisperSpawned +0x52, MissionSucceeded +0x60, HeroTeam +0x78, BanditWavesSpawned
  +0x7c.
- Teams are 64-byte records at +0x8c, with MemberCount +0x18 and a CTimer at +0x1c.
- MakeTeamMemberComment is at 0xDCDA80.

## Label corrections
- 00edfa00 is CTH_ThunderGirl::Main (catalog says CTH_ThunderBoy).
- 00d43db0 is NScript::CQ_GuildTrainingScript::CPreMeleeMaze::Main (catalog CTH_Maze, propagated from the twin 0x00EE1E40).
- 00d50c60 is NScript::CQ_GuildTrainingDepartureScript::CTheRealGuildmaster::Main.
- 0x00EE2DB0 is CV_TravellingHeroesScript::PlayCutscene (catalog CV_ExposeMayorScript). 0x00EE29D0 is
  CV_TravellingHeroesScript::ThunderChatter.
- 0x0088B6E0 clears CAIBrain byte +0x8f; it is not a generic setter.
- 0x008389E0 is a base InitialActivate shared by thirteen thing vtables; the catalog label "AddCombinationBlend" is
  propagated.
- 0x004ED953 is TransferVectorOfSubComponents<CPrizeScoreDef>.
- 0x0049CF80 is the CRightHandedSet copy ctor (catalog "CNearestBuildingWithProperty", a byte twin).
- 0x00669230 stores CThingDeadCreature::PKiller (+0xd8); the catalog "AssignCreatureToTask" is propagated.
- 0x008E7E10 is the Briar Rose concentrate mode's imposter-count test, landed as CIconText::HasEnoughIcons (a twin).
- 0x006A4CF0 is CThingPhysical::OnDie (engine_api "CParticleSystem::Kill").
- 0x004C73D0 is CThing::GetPosition (engine_api "CVertexBufferWin32::DoSizeof").
- 0x00549b20 "CTextSlider::Initialize" is the def-driven NUISystem::CTextSlider initialisation, distinct from the landed
  Initialise 0x00549230.
