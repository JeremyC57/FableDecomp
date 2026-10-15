# Hand chunk 90 (CTCHeroExperience .. CTCHeroStats): 35 of 40 byte-exact

Every src/ file grades MATCH or RELOCATION_MATCH (grades.txt). Every test/ file prints PASS under both g++ (tall.sh) and the lane's VC7.1 cl.exe (tall.py).

Most rows model `this` locally at the retail offsets. The engine headers for CTCHeroExperience and CTCHeroStats (and the PDB layouts of the other classes) assume 16-byte vectors and maps and 12-byte lists. In retail these are 12 bytes and 4 bytes (a single head pointer). So every member after a container sits lower in retail:
- CTCHeroStats: 8 bytes lower from the money block, 0x14 lower from +0x158, 0x18 lower at Marriages +0x1D8.
- CHeroStatsDef: 8 bytes lower at +0x88, 0x14 lower at +0xD8, 0x2C lower at +0x114.

Only 006d8ad0 reads no member, so it uses `engine/CTCHeroExperience.h` through a derived method struct.

## What matched (35)

### CTCHeroExperience (3)
- **006d8ad0 UpdateAbilitiesForAllStats(bool)**: runs the ten-stat UpdateAbilitiesForStat loop, then OnStatLevelChange(false). Plain loop; the flag is forwarded.
- **006d8b00 OnInitialActivate**: AttackHistory.resize(10, {-1, 0}), then the stat loop with `true`. The pair is a value-constructed temporary bound to a const reference, so it is filled after the pushes.
- **006d8ca0 Copy(const CTCHeroExperience*)** (slot 12, manifest `vfunc_12`): copies the totals, both stat vectors (out-of-line `operator=`) and AbleToGainExperience, then runs the stat loop. Needs `// Leaf: Copy`.

### Mislabelled "CTCHeroExperience::AddSuccessfulBlockParry" rows (2 matched)
- **007497c0 CTCNoise::ResetLifeTimer**: `LifeTimer = (long)(fps * Definition.LifeTime + 5.0f)` after a bare `GetWorld()` call. This is the out-of-line copy of the inline body that CTCNoise vtable slot 4 contains.
- **007bed90 CTCShotDefinitionBase::SetTimeToDie(float)**: `DieOnFrame = (long)(fps * seconds + GetWorldFrame())`. The frame rate must be read into a named local before the call, which gives retail's `mov eax,[g] ; mov [esp+8],eax`.

### CTCHeroMorph (2)
- **0071c790 OnInitialActivate**: sets Changed (+0x3D), calls InitialiseCustomTattoos, then UploadTextureMorphsToEngine(app->display->engine, 0, false, 0.0f). The float literal 0 is passed as `push 0`.
- **0071e130 ApplyChanges** (slot 28): if Changed, sends CMessageOnMorphChanged to the parent, rebuilds the texture morphs, calls SetAsChanged(8) on the graphic appearance and clears Changed. The message must live in its own inner block so its base destructor runs right after the send call.

### CTCHeroOpinionDeedLog (3)
- **006c58f0 GetDeedFromHandle**: STLport map `find` (the const twin, 0x006C7830). The iterator is returned through a hidden pointer (it has a user constructor); `end()` is the header word re-read after the call.
- **006c5a50 SetCrimeDeedReportedToVillage**: same shape with the non-const `find` (0x006C77E0). Sets `IsCrimeReportedToVillage`.
- **006c6e20 PlayerTakenOutWeapon**: the handle is re-read from the member before each of the three removals; then PostDeed(1, pos, 0, 0, 2, true, true, 0, false, 0).

### Mislabelled "CTCHeroPodium::CTCHeroPodium" constructors (2) and CTCHeroPodium (1)
- **0041cadc NUISystem::CYesNo::CYesNo(long)**: the size-TU sibling of the landed COk. Two init-list zeros come before the three vtable stores.
- **00555180 NUISystem::CScrollableList::CScrollableList(long)**: a speed TU. Three dwords and two flags are zeroed and 16 bytes of layout are left untouched.
- **00771db0 CTCHeroPodium::CTCHeroPodium(CThing*)**: the occupant count is zeroed before the vtable store (init list); the occupation vector is a member object with an inline constructor, zeroed after it.

### Mislabelled "CTCHeroRangedAttacks::CTCHeroRangedAttacks" (1)
- **004e30e7 CTCDExplosion::CTCDExplosion(CThing*)**: size TU. The radii (`fldz`) and the -1 life timer are plain initialisers placed above the vtable store. The def pointer, flags, vectors, evolution timer and ring follow in declaration order.

### CTCHeroRememberCrimes (1)
- **007556b0 OnSerialise(CPersistContext*, long)**: TransferMap with a `pair<long,long>(0, 0)` temporary, then Transfer<signed char> with a literal 0 default. That default is homed in the dead `context` argument slot.

### CTCHeroSpecialMovement (2)
- **007e0670 SetSprinting(bool)**: nested `if`s around a `__fastcall IsAvailableForHeroSprint(ParentThing)`.
- **007e0700 IsAvailableForSprint**: a named float `fps * def->recharge` is computed before the global-flag test. Then `Sprinting && frames > 0.0f`, then `frames > minimum`.

### CTCHeroStats (20 rows, 18 matched)
- **0057a774 SetMoney(long, bool)**: books earned/spent, then `MaxMoney = Max(MaxMoney, Money)` (Dinkumware const-ref max) and `ClampMin(Money, 0)` with a `long&` parameter. This is the landed SetMaxStamina helper pair under `optimize("s")`.
- **0057a82a Drink(float)**:
  - `BeerReservoir += DrinkAffectMultiplier * amount` (fld amount, fmul member).
  - `float sobering = 1 - def->DAMM` goes into a named local before `++NumberOfDrinks`; that order places the `inc` where retail has it.
  - The def is reached through the raw def-pointer word (an `operator->` on the pointer object adds `add eax,0DCh`).
- **0057a8af GetHeroStatExperience / 0057a8da AddHeroStatExperience**: an inline switch into a zero-initialised index local. `case 0` reuses the zero register (`xor edx,edx ; sub eax,edx`); per-case `return` breaks it.
- **0057a939 GetStartingMaxStamina / 0057a90b GetMaxExtraMaxHealth**: the counted CDefPointer is copied through an inline holder accessor that returns it by value, and AddRef/Release are inline members of the pointee. 0057a90b is a frame-pointer TU (`y` off + `s`) and needs the accessor `__forceinline`.
- **0057aa5b GetFavouriteAbility / 0057aaa8 GetNumberOfAbilityCasts**: the loop test is `i != size()`. That gives `test edx,0FFFFFFFCh` on entry and `jne` on exit; `<` gives `sar ; je` and `jb`.
- **0057aad9 GetPercentageThroughRenownLevel**: a signed then unsigned range check, then `(float)points / cost` (`fild ; fidiv`), else `return 0.0f`.
- **0057ab6a GetMaxBoastingCrowdForRenownLevel / 0057ab14 GetMaxFollowersForRenownLevel**: the guard is `if (size() > 0) { ...; return t[level < size() ? level : size() - 1]; } return 0;`. The `== 0` early-out uses a mask test with the zero arm first. The stack argument is unused (`ret 4`).
- **0057abd6 GetPMaritalHome(const CThing*)**: a list walk with a 64-bit UID compare, then `GetWorld()->GetThingFromUID(home)` (arguments are pushed before the object). `optimize("s")` alone produced retail's `push dword ptr [mem]` and register colouring.
- **0057ac4b HasCurrentMarriage**: four `||` calls with a `long` result (`xor eax,eax ; inc eax`).
- **0057ac89 PeekMarriageInfo(u64, CHeroMarriageInfo*)**: a list walk; the 24-byte POD copy is `push 6 ; pop ecx ; rep movsd`.
- **0057acc4 GetHeroSexuality**: two byte flags and an `||` chain on states 1, 3, 2, 4. `if (!gay) return straight ? 1 : 0; return straight ? 2 : 3;` (writing `if (gay)` first swaps the blocks).
- **0057adf7 RemoveHousePosition(const CRegion*, const C3DVector&)**: the squared 2D distance is written `d = dx*dx; d += dy*dy;`. The one-expression form squares dy first.
- **0057cb4b AddHousePosition**: a frame-pointer size TU. The pair temporary's C3DVector copy goes through eax/edx only with a memberwise user copy constructor (the implicit copy is `movsd`).
- **0057b3e2 GetNumberOfAvailableAbilities**: the inlined out-parameter `GetInterface(&abilities, 0x6F)` template (the 0x007BEA53 family body) must be `__forceinline` under `y` off + `s`. The out pointer is null-initialised, and the loop runs `1 .. != 20`.

## Parked for Fable 5.1 (5)
- 00c2d800 CAudioFormat::BytesToSamples DIFFER(36v30): retail keeps `this` in ecx across `call GetBlockAlign` and the quotient in edx across `call GetSamplesPerBlock` (`mov edx,eax ; call ; imul eax,edx`); the lane saves both in esi/edi | tried: natural member form | next: lane gap: LTCG callee register knowledge
- 00c2d820 CAudioFormat::SamplesToBytes DIFFER(36v30): same as 00c2d800 with the callees swapped | tried: natural member form | next: lane gap: LTCG callee register knowledge
- 00771d50 CTCHeroPodium::GetCrowdPos DIFFER(85v85): only the frame differs, retail `sub esp,10h` / `add esp,10h` (4 dead bytes above the 12-byte zero C3DVector temporary), lane `0Ch` | tried: temporary in the assignment, named local, an empty CDebugBreak-style temporary, the vector element through a begin() iterator object | next: lane gap: dead frame bytes (RTM vs QFE frame layout), or an unknown dead 4-byte local in the original
- 0057a962 CTCHeroStats::GetMaxExtraMaxStamina DIFFER(40v39): the best graded form emits `add eax,0DCh ; mov ecx,[eax]` for retail `mov ecx,[eax+0DCh]` | tried: reference copy + destructor release, raw-pointer ctor + pointee AddRef/Release, raw-pointer ctor + destructor release, reference copy + pointee members | next: the by-value holder accessor that matched the byte-twin 0057a939; differ/0057a962.cpp already holds it UNGRADED (budget spent), re-grade first
- 0057adb8 CTCHeroStats::ResetTrainingStatus DIFFER(70v63): retail loads the def object once into ecx and releases with `dec dword ptr [ecx+4] ; jne`; the lane loads it into eax and emits `mov ecx,eax ; add eax,4 ; dec [eax] ; cmp [eax],0` (or re-reads the frame word after the dec) | tried: pointee Release member + `if (Object) Object->Release()`, `if (Object && --Object->RefCount == 0)` in the destructor, local copy + direct dec, local copy + pointee Release | next: try the by-value accessor or out-parameter idiom that GetSubDef's other callers use; otherwise lane gap: def-pointer register allocation (cf. 24k "CDefPointer local re-read from its slot")

## Skipped
- None. No row covers two functions.

## Label corrections
- 007497c0: CTCHeroExperience::AddSuccessfulBlockParry -> CTCNoise::ResetLifeTimer. The members are CTCNoise's Definition.LifeTime (+0x38) and LifeTimer (+0x40), and CTCNoise slot 4 (0x007498E0) inlines the body.
- 007bed90: CTCHeroExperience::AddSuccessfulBlockParry -> CTCShotDefinitionBase::SetTimeToDie(float). It sets DieOnFrame +0x60, the field IsToDie (0x007BED10) tests; its only caller is TearInGround slot 41.
- 00c2d800 / 00c2d820: CTCHeroExperience::AddSuccessfulBlockParry -> an audio-format record's BytesToSamples / SamplesToBytes (word +0x0C channels, +0x0E codec; 36-byte / 64-sample ADPCM blocks).
- 0041cadc: CTCHeroPodium::CTCHeroPodium -> NUISystem::CYesNo::CYesNo(long) (vtables 0x0122F884/85C/854).
- 00555180: CTCHeroPodium::CTCHeroPodium -> NUISystem::CScrollableList::CScrollableList(long) (vtables 0x0124A72C/708/700).
- 004e30e7: CTCHeroRangedAttacks::CTCHeroRangedAttacks -> CTCDExplosion::CTCDExplosion(CThing*) (vtable 0x01242A54).
- 0057ab14: CTCHeroStats::GetMaxBoastingCrowdForRenownLevel -> GetMaxFollowersForRenownLevel. It reads CHeroStatsDef::RenownLevelMaxFollowers (retail +0xB8, PDB +0xC4); 0057ab6a reads RenownLevelMaxBoastingCrowd (+0xC4).
- 006d8ca0: CTCHeroExperience::vfunc_12 -> Copy (slot 12, as for CTCInventoryBase::Copy). 0071e130: CTCHeroMorph::vfunc_28 -> ApplyChanges (descriptive; the base slot is an empty `ret`).
- Callee labels:
  - 0x00686D60 "CViewBase::GetDisplayEngine" is a CTCBase world accessor.
  - 0x0049D870 "GFGetPreMainMemoryUsed" is GetWorldFrame.
  - 0x006869D0 is CTCBase::GetDefinitions (ParentThing -> global 0x013B8A1C +0x20; +0xDC is the CHeroStatsDef pointer).
  - 0x00686AF0 "GFGetPreMainMemoryUsed" is CTCBase::GetWorld.
  - 0x0051ED60 "CStopWatch::GetTicks" is CWorld::GetThingFromUID(u64).
  - 0x004C73D0 "CVertexBufferWin32::DoSizeof" is CThing::GetPosition.
  - 0x00580034 "GetSubDef<CAbilityDef>" is GetSubDef<CHeroDef> (it looks up the "CHeroDef" name).
  - 0x006DB9D0 "LinkedList_Trim" is list<pair<EDamageAttribute,long>>::resize.
  - 0x00454886 "CActiveFile::operator=" is vector<long>::operator=.
