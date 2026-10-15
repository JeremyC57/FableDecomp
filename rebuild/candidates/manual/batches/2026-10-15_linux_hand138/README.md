# Hand chunk 138 (CEngineWeatherRenderer .. CFactionDatabase): 37 of 40 byte-exact

All 37 src/ rows grade MATCH or RELOCATION_MATCH (grades.txt) and every test prints `<addr>_TEST PASS` under g++ (tall.sh)
and the lane's VC7.1 cl.exe (tall.py). One row needs a pragma: 0x00453178 (`#pragma optimize("s", on)`). Leaf lines are set
wherever the source names the function differently from the catalog.

## What matched (37)

- **CEngineWeatherRenderer** -- 00b52090 IsPointInPrecipitation: true when the point is inside any of the three weather-mask
  spheres grown by the box radius. Shape: the sphere centre comes through a BY-VALUE getter (`GetCentre()`), which gives the
  retail half-FPU/half-frame copy (x/y on the stack, z copied as a dword) and the 0x1c frame; reading the member directly
  folds the subtraction into memory operands.
- **CEngineWorldMap (+ mislabelled relatives)** --
  00b3ff70 CreateNewBankMap (the real one): AddEntry(name, 0, name, CCountedPointer<CBankStateBlock>(new CStaticMapBankStateBlock)) then FindEntry(name);
  the counted pointer is a named local passed by value (copy built in the argument area, local released after the second call).
  00b40000 CloseStaticMap (label "FrameStart" wrong: no argument): close every map after the first, the water renderer, the bank file; clear the two state words.
  00b42c70 DeleteAllMaps: DeleteMap(i) per index, Dinkumware `resize(0)` with the `T()` default in the `push ecx` word and the
  early-out copy-down helper, then `if (Map) { delete Map; Map = 0; }`.
  00512c70 (free /Gr fastcall factory) and 009d58d0 (CBankFileAsync::CreateEntry): `CCountedPointer<T> p(new T(...)); return p;`
  -- no NRVO, so the copy-out + destroy fold into the retail `inc ecx ; mov eax,ecx ; dec eax` with both stores kept.
  0088b910 FindLastRecordBelow (12-byte keyed records, signed key, count re-derived each pass).
  00d77340 CQ_HangingTreeGoodScript::StoreGuardsHealth (GetHealth slot 264 into GuardsHealthVector, terminating test per guard).
- **CEntity** -- 00c647b0 / 00c64940: validate a (level -10000..0, ratio 0..1) pair, error -2/-3, store on change and set a dirty
  bit. Shape: the level is a local read once (no reload after the stores), the ratio re-read through the pointer; the
  deferred copy is a chained `current = deferred = x` (float through `fst ; fstp`), a single float store is an integer move.
- **CEnvironment** -- 006b2b10 OverrideWindDirection (CEnvironmentGameTickState fields modelled `pack(1)`: OldWindDirection
  is unaligned at +0xc61); 006b3600 SetThemeWeight, 006b3680 SetThemeWeightAllChannels, 006b3800 TransitionToTheme(4 args),
  006b38d0 TransitionToThemeAllChannels -- the landed 0x006B37A0 model (theme set at retail +0x1954, 12-byte CArrays, 48-byte
  entries) with the channel/priority range tests in source order.
- **CEnvironmentLookupTexture** -- 008860d0 LookupFloatColour: BGRA texel to (c + 0.5) / 255 floats; CRGBFloatColour with a user
  constructor returns through the hidden pointer and its four arguments evaluate right to left (A, B, G, R).
- **CEnvironmentSpellEffectInterface** -- 00885070 SetRadialBlurParams, 00885420 SetScreenFilterParams (parameter block built in a
  frame local, POD `rep movsd` into current and, when not interpolating, previous-frame params); 00885110
  SetRadialBlurCenterWorldPos (blur then area centre, track 1); 008851a0 / 00885210 screen-position centres (named
  `C3DVector(x, y, 0)` local, track 2). 00c6ca20 is NOT this class: CVtable_012bed7c constructor (see label corrections) --
  aligned pointer `(this+0x68+15) & ~15`, two memset records, pointers to them, and a two-array clear loop VC unrolls.
- **CEnvironmentSpellEffectInterpolator** -- 006b84e0 RadialBlurFadeTo (default CRadialBlurPositionParams: the track words are
  plain initialisers stored before the pushes, the C3DVector members have inline constructors stored after), 006b85d0
  ScreenFilterFadeTo.
- **CEnvironmentThemePriorityStack / Entry** -- 008889c0 CalculateTheme (`1.0f - BaseWeight > 1/1024` gate, loop from entry 1);
  00886160 GetThemeWeight (found arm returns through the begin pointer captured at loop entry).
- **CEventShownItem** -- 008116d0 destructor: IS_THING_ALIVE + forceinlined out-parameter GetInterface(type 0x1a), result used
  unchecked; members destroyed in reverse order; the inline NEntityEvents::CEventBase destructor stores its vtable last; a
  non-virtual destructor in the polymorphic class keeps `??_G` out.
- **CExplosiveTrailDef** -- 00453178 Transfer (engine header): nine typed transfers then `if (f < 0) f = 0; else if (f > 1) f = 1;`
  -- `#pragma optimize("s", on)` gives the eax-only `lea`s and the fldz/fld1 + shared `fstp` clamp.
- **CExpressionSet (one template, four element sizes)** -- 00757500 (44), 0081ace0 (24), 0081ad50 (40), 0086f590 (12):
  vector TransferBinaryOut = WriteLong(size), Write(`size ? first : this`, size * sizeof(T)), WriteLong(end marker).
- **CFH_* entity scripts** -- 00d37330 BriarRose::Main (named handle pointer local for IsScripting/Release, rotated
  `while (!StartScriptingEntity(...)) { NewScriptFrame(); if (IsDone()) return; }`, tail-jump IsDone); 00d36860
  GuildMaster::Init (the by-value CScriptThing copy for slot 844 goes through the PImp counted pointer's own copy constructor so the
  member is addressed from `this`); 00d3af50 MemorialCutsceneMarker::Main (`SetHeroAge(GetHeroAge() + 1)`, by-value
  CCharString cutscene name built in the argument slot). GSI vtables generated with probe/gen_gsi.py from cgsi_slots_aligned.tsv.
- **CFactionDatabase** -- 009ad2e0 FindFactionBySymbolName: CRC of the name (length read first), inline map Find whose search key
  is a COPY of the register-held CRC (homed in the dead `name` slot), caller re-reads `end`.
- **_global** -- 00a70ff0 SGI `__linear_insert` over pair<CCharString, 52-byte POD> (/Gr), the landed 0x0081BDA0 template with a
  class key.

## Parked for Fable 5.1 (3)

- 00b3edf0 CEngineWorldMap::LinkMapToWorld DIFFER(131v131): retail `mov ebx,[ecx+18h]` before the map words, unshifted start X kept in edi and shifted after the Y bounds, row index in ebx; ours `mov edi,[ecx+18h]`, start X shifted at once, x/y registers swapped in the loops | tried: natural expressions (short and long index), offset/position locals, unshifted bounds shifted later | next: lane gap: register allocation (same instructions, registers swapped)
- 00b3ee80 CEngineWorldMap::UnlinkMapFromWorld DIFFER(119v119): retail `mov ebx,edx ; ... ; sub ebx,edi` (start X subtracted after loading the Y words, shifted after start Y); ours `sub edx,edi ; mov edi,[ecx+1Ch] ; sar edx,5` before the Y loads | tried: natural expressions, four-argument SCellBox constructor (141 bytes, worse) | next: lane gap: scheduling (same instructions, other order)
- 00530ec0 CDefOwner00530ec0::GetDef DIFFER(99v98): retail `mov ecx,[esp+4] ; ... ; mov edx,[ecx+4] ; inc edx ; mov eax,edx ; dec eax ; mov [ecx+4],edx ; mov [ecx+4],eax` vs ours `mov eax,[esp+4] ; mov ecx,eax ; mov edx,[eax+4] ; add eax,4 ; inc edx ; mov [eax],edx ; dec edx ; mov [eax],edx` | tried: CDefPointer local returned by value (reloads the slot after the inc), ternary manager (Get() hoisted above the pushes), if/else on a manager local, frontEnd local + hand release through an inline guard (manager part now byte-exact) | next: build the returned pointer straight from the out slot (`CDefPointer(const SDefSlot&)`) or let the guard read the slot itself so the object is loaded into ecx

## Skipped

None: no row of this chunk spans two functions.

## Label corrections

- 00512c70 "CEngineWorldMap::CreateNewBankMap" (thiscall, 1 arg) is a no-argument /Gr fastcall factory returning a counted pointer to a 5-byte flag record.
- 009d58d0 "CEngineWorldMap::CreateNewBankMap" is CBankFileAsync::CreateEntry(unsigned long) (vtable 0x0129B880 = CBankFileAsyncEntry; descriptive name).
- 0088b910 "CEngineWorldMap::BuildAndSaveSea" is a search (FindLastRecordBelow) on an unknown owner with records at +0x94.
- 00d77340 "CEngineWorldMap::BuildAndSaveSea" is CQ_HangingTreeGoodScript::StoreGuardsHealth (descriptive; getter half of SetGuardsHealth 0x00D766C0).
- 00b40000 "CEngineWorldMap::FrameStart(CEngineComponentParams*)" takes no argument: CloseStaticMap (descriptive).
- 00b42c70 "CEngineWorldMap::DeleteMap" (no-arg) renamed DeleteAllMaps; DeleteMap(long) is 0x00B42B10.
- 006b3600 "CEnvironment::TransitionToTheme" is CEnvironment::SetThemeWeight(channel, priority, theme, weight) (calls 0x00886D70).
- 006b38d0 "CEnvironment::SetThemeWeightAllChannels" (duplicate) is TransitionToThemeAllChannels (calls 0x008861D0).
- 008851a0 "SetRadialBlurCenterAreaScreenPos" (shared with 00885210) writes the blur centre: SetRadialBlurCenterScreenPos.
- 00c6ca20 "CEnvironmentSpellEffectInterface::CEnvironmentSpellEffectInterface" is the CVtable_012bed7c constructor (base 0x00C6E6A0; that class is 0x104 bytes with no vtable).
- 00a70ff0 "CEnvironmentSpellEffectInterface::SetRadialBlurParams" is SGI __linear_insert<pair<CCharString, 52-byte POD>>.
- 00530ec0 "CEventKilledCreature::CEventKilledCreature" is a hidden-return def getter (def index at +0x130) on an unknown class.
- Callee labels: 0x00993EB0 "WriteCBYTE" writes a long; 0x00431061 / 0x00431102 "UpdateShadowScene" are persist transfer helpers;
  0x00F35B30 "CCreatureAction_TrollWhackGroundBase::Initialise" is the entity script's IsDone; 0x00CD23B9 / 0x00CD2770
  ("HasPhysicsMesh" / "Clear") are the scripted-thing handle's IsScripting / Release; 0x00D39330 "Game_InitializeRound" is a
  CQ_FireHeartScript cutscene player; 0x005D8E50 "CChunkCollectionIndicator::OnPredicateFail" is a type-0x1a component method;
  0x0050D230 "operator_new" is a plain delete deleter; 0x009CCDC0 "CQuestManager::OnQuestStartScreenDisplayed" and
  0x009CD620 "Map_InsertOrReplacePair" are bank-file FindEntry / AddEntry.
