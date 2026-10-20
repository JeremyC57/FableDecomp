# Hand chunk 206 (CTCCarriedActionUseLamp .. CTCDivineWrath): 30 of 40 byte-exact

All 30 sources in src/ re-graded RELOCATION_MATCH with gall.sh (catalog leaf / `// Leaf:` line) and with tools/rcheck.py (the
lander's leaf rule plus `reloc_identity.check`: no HARD findings; the WARNs are propagated Ghidra labels and the landed rows
00435000 / 0051fba0 that bind the const and non-const LowerBound / GetMainPlayer overloads the wrong way round, see below).
All 30 tests print `<addr>_TEST PASS` under g++ (tall.sh) and VC7.1/Wine (tall.py). No pragmas, no flags. `pack(1)` only on the
CIntelligentPointer classes where the PDB puts the pointer at an odd offset (+0xad, +0x11, +0x2a, +0x6d) and `pack(2)` on
CEngineGraphic (Ego_r.pdb 14 bytes). Leaf lines: `??1` for the three destructors, `??0CTCCombat`, the mangled InformOfDeed
overload, `FrameUpdate` / `vfunc_6` / `vfunc_28` / `FindMostRecentDeed`.

## What matched (30)
- **CTCCarriedActionUse\*:** 007ecee0 Lamp::vfunc_28 (= OnActionUse; light pointer zero-initialised before the HasTC test, the
  light def out pointer in the dead `user` slot, null-tested ReleaseTemporary dtor); 007f03d0 Spade::vfunc_28 (activate-quest
  action temporary, novtable action; `pack(1)` on the intelligent-pointer classes only so the action stays 4-aligned in the
  frame); 007edee0 Trophy::OnActionUseCommon (unchecked out-parameter GetInterface for the trophy; a one-ternary GetTC so the
  targeting component lands in ecx for its virtual call; the CThing vptr must live in the root model).
- **CTCCarryable / CTCCarrying:** 00705b60 UpdateCarriedPosition (`(float)fabs(a-b) > eps ||` chain, reference local for
  GetPosition); 006a1370 SetAsDrawable (inline intelligent-pointer copy ctor per slot, `*(begin()+i)`, VC aligns the frame).
- **CTCChest:** 00750780 FrameUpdate (inline IsIdle(); `if / else if` whose UpdateAnimation tail VC7.1 duplicates; frame time
  through the 0x686D60 accessor).
- **CTCCombat:** 00733690 GetBlockingAnimationEntry (local STLport vector freed with `if (first) free`, WorldRandomRange);
  007337e0 SetBlocking; 00733e10 SetMeleeFlourishValid (bool preset declared first -> bl; block-scoped name string shares
  the dead `valid` slot with the lookup key); 00735540 ctor (`_Vector_base` + alloc-proxy vectors, `_Rb_tree` header malloc,
  init list in declaration order). Local models: retail containers are 12 bytes (engine/CTCCombat.h is PDB).
- **CTCContainer:** 008ed090 AddItemToInventory (STLport exception-free `bitset::test` = `puts("bitset"); abort();` with
  abort `__declspec(noreturn)` so the hero stays in eax; unset result local).
- **CTCCreatureFlyInCircles:** 007ddcc0 ChangeMode (switch cases written in retail layout order 2, 1, 0; long duration local
  for the `fidiv`).
- **CTCCreatureGeneratorCreator:** 007c9920 OnInitialActivate (`uid > 0` on ulong64; intelligent-pointer operator= takes
  `GetPItem()` into a local first).
- **CTCCreatureGroupBoastingCrowd:** 00784bd0 dtor (manager named local before the pushes; retail PHero +0x24).
- **CTCCreatureModeManager:** 0073a8e0 AllocateMode (upper bound in a named local so it is computed first; thing local before
  the registry lookup; raw-pointer CCountedPointer ctor in the return slot); 0073aba0 RemoveMode (idle delay
  `(long)(fps*30)` as a named local -- written inline VC7.1 calls GetWorldFrame first and folds the add into `sub` of -30).
- **CTCCreatureNavigation:** 006cb780 GetNearbyDirectedPosition (named-local delta + `s = Z*Z; s += X*X; s += Y*Y`; the same
  formula should recover the parked siblings 0x6CB620/6E0/840).
- **CTCCreatureOpinionOfHero:** 00682310 ClearPersonalityOverride; 00683590 InformOfDeed(long, bool) (CDefPointer temporary's
  `.Object` read through the returned pointer); 0093a3c0 FindMostRecentDeed<IsStillHappening> (list sentinels cached in
  locals; two found returns merged by VC; explicit template instantiation).
- **CTCCreditsUI:** 007e4a90 FrameUpdate (catalog "vfunc_9" = slot 9 FrameUpdate; delete per index; inline erase with the
  early-out trivial copy); 007e4b60 Activate (component kept in esi across virtual calls only with a same-TU
  `_M_insert_overflow` body under `auto_inline(off)`).
- **CTCCupboard:** 00749b40 UpdateAnimation (unset-local accessor over a one-ternary GetTC -> component straight in ecx).
- **CTCDExplosion:** 0080cc40 SetSpecificThingsToHit (intelligent-pointer temporaries into the inline push_back).
- **CTCDParticleEmitter:** 006e0af0 ClearAttachments (AttachmentFlags assigned before the merged bit-field RMW).
- **CTCDPhysicalObstruction:** 007fd580 UpdateAdditionalImpulse (CFilter_Bind3<IsNotList, IsWithinArea,
  IsAffectedByObstructions> temporaries in the search call; implicit functor copy; same-TU for_each body and a STAND-IN
  search body -- see LESSONS "Review at landing").
- **CTCDRegionExit:** 007795a0 dtor (user dtor, packed pointer at +0x2a, unchecked out-param region display).
- **CTCDayOrNightOnlySupport:** 00702b10 vfunc_6 (= OnInitialActivate; inline CCharString `==` with the landed Compare body
  under `auto_inline(off)`).
- **CTCDivineWrath:** 0078e900 ResetCreatureHitState (by-value pointer destroyed by the callee); 0078f4d0 OnCreate (inline
  GetPhysics() accessor fixes the radius register; driver named local before the push_back temporary).

## Parked for Fable 5.1 (10)
- 007e1eb0 CTCCarriedActionUseRead::vfunc_28 DIFFER(187v187): this/def colouring ebx/edi vs retail edi/ebx (retail pushes ebx inside the !AlreadyRead block) | tried: holder dtor, out-param GetInterface, def local first, stats declared first | next: lane gap: register colouring (try a same-TU ApplyBonusItemEffects stand-in? it is in another TU, so probably not)
- 00705190 CTCCarryable::GetInterpolatedPickupOffsetAndOrientationAt DIFFER(168v168): flags/position esi-edi swap and the dummy local above the difference temporary | tried: inline appearance accessor, dummy declared first, inline ternary flags, reference local | next: lane gap: frame-slot order of a named local vs an inline operator- temporary
- 006a1490 CTCCarrying::IsCarriedThingInPrimarySlot DIFFER(181v173): def release `mov esi,[eax+4] ; add eax,4 ; dec ; test` vs retail `mov ecx,[slot] ; mov eax,[ecx+4] ; mov bl,[ecx+30h] ; dec eax ; mov [ecx+4],eax ; jne` | tried: forceinline FindValue, key temporary + bool outside the def block, nested success path, forceinline Release | next: the __forceinline def-getter helper of 008ed1f0 plus a direct `--m_refCount` holder (retry 8)
- 00750050 CTCChest::GetNumberOfKeysToUnlock DIFFER(199v186): retail builds the GetString temporary in the dead out-pointer word (8-byte frame), ours opens a third word; OpenerObject reload | tried: def local first, block-scoped out pointer, block-scoped name, forceinline def getter | next: lane gap: QFE frame-slot sharing (GOTCHAS 23d)
- 00736590 CTCCombat::UpdateUsableMeleeCombatAbilities DIFFER(176v176): register choice only (push through ecx vs eax, edx/ecx leas, vtable edx vs eax) | tried: const copy, same-TU _M_insert_overflow body (fixed the def caching), Move/DestroyRange bodies | next: lane gap: register choice
- 008ed1f0 CTCContainer::InsertItem DIFFER(194v184): holder form loads the def via eax then esi (186v184, probe v1); the forceinline GetItemDef helper loads esi directly but its holder release becomes the load/dec/store form | tried: holder, raw local + hand release, inline flag accessor, forceinline helper | next: helper + a holder whose dtor is a direct `dec [p+4]` (retry 8 "holder type whose dtor is a direct --count")
- 00813010 CTCCreatureGroup::AddNewMember DIFFER(250v238): frame 0x64 vs 0x5c (find key and make_pair key not sharing a slot) and the null/dead `return false` tail at the end | tried: named data local, forceinline data ctor, one full-expression insert, by-value forceinline find/insert helpers | next: lane gap: QFE frame-slot sharing
- 00779d20 CTCDExperienceOrb::Construct DIFFER(239v239): params temporary's vtable + Rhs stores sunk below the physics vtable load and push; out pointer in a new frame word | tried: temporary, named block local + forceinline def helper (fixes the slot) | next: lane gap: vptr-store scheduling of a stack temporary (same as 006dfe70)
- 006dfe70 CTCDParticleEmitter::Construct DIFFER(178v178): only the params temporary's vptr store, sunk below `mov edx,[edi] ; push eax` | tried: two-argument inline ctor | next: lane gap: vptr-store scheduling
- 0078e820 CTCDivineWrath::SetPTarget DIFFER(218v220): one instruction: the explicit CIntelligentPointerConstThing(target) temporary reuses eax as this where retail re-addresses it with `lea ecx,[esp+8]` | tried: base operator=, copy+converting operator=, GetPItem local, forceinline operator= | next: `PTarget = target;` (implicit conversion temporary, GOTCHAS wave 11) -- very likely MATCH

## Skipped
None (every row is a single function; no int3-separated bodies or jump tables).

## Label corrections
- 0x005D8E50 (manifest "CChunkCollectionIndicator::OnPredicateFail") is the interface-0x1a component's no-argument hook; two
  landed spellings (CTCShowableItem::OnShowEnded in 0x008116D0, CTCInterface1A::Notify in 0x007EDDF0); used OnShowEnded. It
  sits among CTCInventoryItem's members (0x5D8D50 AddToInventory, 0x5D9030 the type getter), so the class is probably
  CTCInventoryItem.
- 0x00686D60 returns `[0x013B8A1C]+0x1c`; the landed CTCNoise::ResetLifeTimer 0x007497C0 calls it GetWorld, but GetWorld is
  0x00686A70 / 0x00686A80 (`+0x24`). Used GetFrameSource (two landed callers). 0x00686D40 / 0x00686D50 (`+0x30`) are
  CTCBase::GetPlayerManager (non-const / const), not CViewBase::GetDisplayEngine.
- 0x0073A6E0 "CPixelShader::CountReferences" = CTCCreatureModeManager::GetCurrentMode; 0x00663340 "CEventKilledCreature" =
  CThing::GetHeroCombatDef; 0x006925B0 "CWorld::DrawGetEnvironment" = CCreatureActionBase::GetTotalTime (`mov eax,[ecx+10h]`);
  0x008F4CB0 "CCreatureAction_CastTimeSpell" = CCreatureAction_DragonFlight ctor (vtable 0x01288544, as landed);
  0x006C5630 "CThingBuilding::GetFloorMaterial" = the deed log's effects-def getter (CDefPointer by hidden pointer).
- 0x007FDA80 "_For_each<..., CApplyAdditionalImpulseIfInCombatAndTooClose>" = for_each<CThingCreatureBase**,
  CKeepOutOffSmearedOutLine> (12-byte functor {StartPos*, EndPos*, Radius}); 0x007FDB10 "GetAllThingsInRoughArea<...,
  CThingFilter_None>" = the CFilter_Bind3<CThingFilter_IsNotList, CThingFilter_IsWithinArea, CThingFilter_IsAffectedByObstructions>
  instantiation (Ego_r.pdb names all three filters and the binder).
- STL out-of-line helpers: 0x00485DBC "Std_Vector_Insert_Range_Fill" = vector<NUISystem::CComponent*>::_M_insert_overflow;
  0x004AC940 "Std_Vector_Insert_Range_SmartPtr" = vector<CIntelligentPointer<CThing>>::_M_insert_overflow; 0x0074FCE0
  "std::vector_InsertRangeWithCopy" = vector<pair<long, CCharString>>::_M_insert_overflow; 0x00739BC0
  "std::vector_insert_CCountedPointer" = vector<CDefPointer<const CMeleeCombatAbilityDef>>::_M_insert_overflow; 0x00738C20
  "_Destroy vector<CDataBankGroupEntry>" = the CDefPointer range destroy; 0x0073D210 / 0x0073D0E0 "LowerBound" (CDefString value
  type) = CVectorMap<unsigned long, CTCCreatureModeManager::CRegisteredModeInfo>::LowerBound const / non-const on the static
  registry 0x013BAD44.
- 0x004E5C71 "_Dest_val<...CMusicManager...>" = ~CTCCreatureGroup (destroys the member map, then ~CTCBase 0x00686830, whose
  label "~CChunkedFileChunk" is propagated); 0x00681E60 "SetPersonalityOverride_BuildOpinionTable" = the
  SetPersonalityOverride(long, bool) overload; 0x006A3060 "GetDef<CThingPhysicalSwitchDef>" here instantiates CCarrySlotDef
  (PrimarySlot +0x30); 0x0042B0A2 "GetSubDef<CAbilityDef>" is CAppearanceDef (as landed); 0x007E1D30 is GetSubDef<CBonusItemDef>.
- 0x004349D0 is landed as "CTCCreditsUI::Deactivate" but is called on `g_pMainGameComponent->+0x28` (Ego_r.pdb PDisplayEngine);
  another landed row binds it as CDisplayEngine::EndLetterBox -- used that.
- Vtable slots: CTCBase slot 6 OnInitialActivate, 9 FrameUpdate, 28 OnActionUse; CThingCreatureBase slot 77 IsPlayer()
  (0x0052ABE0 returns 1, 0x00835940 returns 0); NUISystem::CComponent slot 48 ChangeState(long).
- Name conflicts in the landed tree: the frame-rate global 0x01375550 is `g_framesPerSecond` in 44 declarations and
  `g_FramesPerSecond` in 34 (used the former); 0x0049D870 is GFGetCurrentFrame (most rows) vs CWorld::GetWorldFrame (GOTCHAS,
  used here as a static); landed 00435000 binds the non-const LowerBound name to 0x004365B0 and the non-const GetMainPlayer to
  0x00449960 (both the const overloads) and 0051fba0 the const GetMainPlayer to 0x00449970.
