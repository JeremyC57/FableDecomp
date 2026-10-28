# Hand chunk 526 (CCreatureAction_RockTrollShakeAndExplode .. CBasicString<char>): 13 of 20 byte-exact

All 13 are RELOCATION_MATCH under VC7.1 RTM 13.10.3077 (default flags), tests PASS under g++ and VC7.1/Wine;
vland dry run: 13 WINS, no RELOC_DUPLICATE / RELOC_IDENTITY (RELOC_NAME warnings for 0x00792240, 0x007FEF40, 0x00947780, 0x00772110 and one RELOC_CONFLICT on 0x00436AC0 vs landed 0x004AD780 -- settled for this chunk by evidence, see LESSONS.md Review at landing).
No retail_asm rows.

## What matched (13)
- CCreatureAction_RockTrollShakeAndExplode::FinishAction 0x008C6CE0 -- death explosion from CTrollBattleDef (immune
  creature), base FinishAction, Kill(true). Shape: the landed 0x008C78F0 CDefPointer model plus a same-TU
  `template<> GetSubDef<CTrollBattleDef>` body under auto_inline(off) (keeps `this` in a register, shares the bl=1
  constant), HasTC as `if (bit) return true; return false;` and a named PTCDriver local. The file also matches 0x008C78F0.
- CCreatureAction_HobbeSpellcasterAim::Initialise 0x008C9BA0 -- base Initialise, kill a live old effect, attach
  "CHARGE_SPELL" to the staff through the anonymous-namespace AttachEffectToStaff (__fastcall).
- CCreatureAction_NymphGetHit::FrameUpdate 0x008C9E60 / ::Initialise 0x008CA220 -- the Ego_r inline accessors
  GetTCGraphicAppearance() / GetPhysicsTC() (object before the arguments, `mov ecx,[eax+60h]` before `push 0`);
  `if (!GetDummyObject(...))` gives neg/sbb/inc.
- CCreatureAction_NymphTeleportOutOfLimbo::Initialise 0x008CA170 -- DoEffect (protected), ResetPosition /
  SetForcedToMovePos(NewPos +0x114), SetCombatAnimation("TELEPORT_OUTOF_LIMBO"), base, StopFlyingAround(TotalTime).
- CCreatureAction_FireMissileWeapon ctors 0x008D5020 (nine arguments; the second float is the accuracy) and 0x008D5110
  (two arguments; accuracy from GetShotAccuracyPercentage, `fild`) -- by-value CDefPointer with inline copy ctor/dtor.
- CCreatureAction_GetInBed ctor 0x0092E300 -- CDefString -> CCharString conversion through g_DefStringTable.GetString.
- CAIStateGroup_HandyMan::AcquireWorkSpot 0x0091B010 -- same-TU noinline bodies for GetAllThingsInRoughArea, list::remove
  and _List_base::~_List_base keep the list header in esi; the box is a named local built before the call; `empty()`
  then `size()` (VC folds the first walk into the empty test); the random index is taken before `begin()`; a
  user-declared C3DVector copy ctor keeps the position copy in memory so ebp is free for the shared zero (the last fix).
- CAIStateGroup_UsesSeat::GetUpFromSeat 0x00938A10 -- inline CCreatureAction_GetUpFromSitting ctor (CCombatActionBase
  ctor, vptr 0x01276C2C, SittingMode/RemoveOnPerformAction, SetCombatAnimation) passed to AIAction_PerformActionAndWait;
  the one-ternary GetInterface puts the selected component straight into ecx (the if-form went through eax).
- CAIStateGroup_WanderAroundShops::FindASimplyDelightfulShop 0x00946F90 -- same-TU __declspec(noinline) stand-ins
  for GetAllLeafThings and vector::_M_insert_overflow (auto_inline(off) alone does not stop a template member
  instantiated at the end of the TU from inlining) keep the found vector's start/size hoisted as in retail.
- CDataInputStream::Read3DAngles 0x00995210 -- three inline Reads into a C3DAngles local returned by value (GPR copies).
- _HyperlinkProc 0x0099D660 -- Neal Stublen's hyperlink window procedure with Fable's hover flag (g_HyperlinkHovered)
  instead of the underline-font swap.

## Parked for Fable 5.1 (6)
- 008c9a40 AttachEffectToStaff DIFFER(342v342): the "SHOT_CREATION_DUMMY" CCharString temporary sits at [esp+18h] (the emitter/key slot) where retail reuses [esp+14h] (the dead effect index) | tried: B2 shape with same-TU GetSubDef<CSpecialEffectsDef>, forceinline GetTC pick, named/unnamed helper string, scope variants (2 grades + 3 probes) | next: give the effect index a narrower scope or pass it through a by-value temporary so its slot dies before the string is built
- 00924520 `anonymous namespace'::CCreatureAction_KrakenLowAttackLoop::FinishAction DIFFER(336v334): frame 0x124 v 0x120 (the inline GetInterface key gets its own slot where retail shares the kraken out slot [esp+10h]) and the dead test keeps eax/al=1 where retail keeps eax and uses cl=1 also stored to Interrupt | tried: K1-K4, L1-L3, M1-M2 (key written into the out var fixes the frame but not the registers) | next: an out-parameter GetInterface whose key is the out variable itself (retail's slot sharing) combined with `Interrupt = true` hoisted into the dead test's constant
- 00929fc0 CAIStateGroup_ApprenticeHero::GetObjectWithScriptName DIFFER(265v242): VC7.1 inlines the found path's list destructor (retail calls 0x006FE660 out of line, result in esi, ebx shrink-wrapped round the pick) and folds GFRandom to `xor edx,edx / div` where retail branches into ecx | tried: early `size()==0` return, inner `return 0`, if-form GFRandom, up-counting index, while-advance (1 grade + 3 multi-variant probes; VC inlines ~list at both or neither of two sites, or out-of-lines the empty path, never the found path alone) | next: `std::advance` + STLport's real `_List_base` dtor nesting (deeper inline chain) or an iterator-returning helper so the found return is a separate inline site
- 00947110 CAIStateGroup_WanderAroundShops::FindPrettyThingInShop DIFFER(354v354): frame 0x18 v 0x14 -- the pushed item temporary gets [esp+10h] where retail homes it in the dead `shop` argument slot with the lookup key and the overflow's __true_type tag; every other instruction matches | tried: out-param GetInterface, named GetTC owner, named item local, two overflow stand-ins | next: push a by-value ThingCast<T>(item) temporary (the landed 0x0062E840 form) or a CIntelligentPointer conversion temp
- 00947eb0 CAIStateGroup_GoToPub::GetObjectWithScriptName DIFFER(265v242): twin of 0x00929FC0 (retail ~list 0x00792240 out of line on the found path) | tried: the 0x00929FC0 model (1 grade) | next: whatever lands 0x00929FC0
- 009a01c0 CBasicString<char>::ExtendData DIFFER(312v316): register allocation only -- VC keeps the new length in ebp and spills the new buffer to the `length` argument slot; retail keeps the buffer in ebp and spills the length (so it reads the length into eax before the shrink-wrapped pushes) | tried: length reassigned in place, a separate local, a pre-declared buffer local (+ __forceinline HighestSetBit, a local copy of StringLength) | next: lane gap: register allocation (try a ClearStringEnd/Copy helper taking the buffer by reference, or the PDB's SetString-style helper order)

## Skipped
- 0099c7e0: the row (334 bytes) covers two functions -- STLport `basic_string<wchar_t>::append<const wchar_t*>(first,
  last, const forward_iterator_tag&)` is 306 bytes (through the int3 after the noreturn `_M_throw_length_error` call at
  0x0099C90C), then 14 int3 and the next function 0x0099C920. differ/0099c7e0.cpp RELOCATION_MATCHes the true 306-byte
  extent (the landed char twin 0x00567D60's source with the character type changed); land it once the oracle row is
  trimmed to 306 bytes (as 0x00567D60's was to 281).

## Names
### Proven (two sources each)
- 0x008C6CE0 ?FinishAction@CCreatureAction_RockTrollShakeAndExplode@@UAEXXZ -- RTTI 0x0128154C vtable slot 4 + Ego_r name set
- 0x008C9A40 ?AttachEffectToStaff@?A0x13e2aa0e@@YIPAVCThing@@AAVCThingCreatureBase@@ABVCCharString@@@Z -- Ego_r name + donor order (between Aim::FinishAction and Initialise) + "SHOT_CREATION_DUMMY"/"CSpecialEffectsDef" strings
- 0x008C9BA0 ?Initialise@CCreatureAction_HobbeSpellcasterAim@@UAEXXZ -- slot 12 of vtable 0x01263CF4 + Ego_r
- 0x008C9E60 ?FrameUpdate@CCreatureAction_NymphGetHit@@UAEXXZ -- slot 56 of vtable 0x01263F84 + Ego_r
- 0x008CA170 ?Initialise@CCreatureAction_NymphTeleportOutOfLimbo@@UAEXXZ -- slot 12 of vtable 0x01282A04 + Ego_r
- 0x008CA220 ?Initialise@CCreatureAction_NymphGetHit@@UAEXXZ -- slot 12 of vtable 0x01263F84 + Ego_r
- 0x008D5020 ??0CCreatureAction_FireMissileWeapon@@QAE@AAVCThingCreatureBase@@PBVCThing@@MMM_NV?$CDefPointer@$$CBVCMeleeCombatAbilityDef@@@@JJ@Z -- vtable 0x01285424 store + `ret 24h` arity + Ego_r donor 0x0090BD10
- 0x008D5110 ??0CCreatureAction_FireMissileWeapon@@QAE@AAVCThingCreatureBase@@PBVCThing@@@Z -- vtable 0x01285424 + `ret 8` + Ego_r donor 0x0090BE10
- 0x0091B010 ?AcquireWorkSpot@CAIStateGroup_HandyMan@@AAE_NXZ -- Ego_r donor order (its GetAllThingsInRoughArea instantiation right before, IsRunnable/Update after) + bool return + WorkSpot +0x14 (PDB layout)
- 0x00924520 ?FinishAction@CCreatureAction_KrakenLowAttackLoop@?A0x29922355@@UAEXXZ -- RTTI 0x01290C64 slot 4 + Ego_r anonymous namespace
- 0x00929FC0 ?GetObjectWithScriptName@CAIStateGroup_ApprenticeHero@@IAEPAVCThingGameObject@@ABVCCharString@@@Z -- Ego_r name set + body (GetAllThings<CThingGameObject,...,CThingFilter_HasScriptName>) + caller GetDummy 0x0092A0C0
- 0x0092E300 ??0CCreatureAction_GetInBed@@QAE@AAVCThingCreatureBase@@AAVCThing@@ABVCAnimationEntry@@ABVCCharString@@JJ@Z -- vtable 0x01291BFC + Ego_r ctor
- 0x00938A10 ?GetUpFromSeat@CAIStateGroup_UsesSeat@@IAE_NXZ -- Ego_r name set + bool return + inline CCreatureAction_GetUpFromSitting (RTTI of vtable 0x01276C2C)
- 0x00946F90 ?FindASimplyDelightfulShop@CAIStateGroup_WanderAroundShops@@AAEPAVCThing@@XZ -- Ego_r name set + no-argument CThing* signature + shop body (TC 0x53 CTCShop)
- 0x00947110 ?FindPrettyThingInShop@CAIStateGroup_WanderAroundShops@@AAEPAVCThing@@PAV2@@Z -- Ego_r name set + CThing* argument (`ret 4`) + shop owner list body
- 0x00947EB0 ?GetObjectWithScriptName@CAIStateGroup_GoToPub@@IAEPAVCThingObject@@ABVCCharString@@@Z -- Ego_r name set (donor 0x0096E1B0) + byte twin of 0x00929FC0
- 0x00995210 ?Read3DAngles@CDataInputStream@@QAE?AVC3DAngles@@XZ -- Ego_r reader order (Read2DVector/Read3DVector/Read3DAngles/ReadMatrix3x4 = retail 0x00995030/0x009950F0/0x00995210/0x00995340) + 12-byte C3DAngles by hidden pointer
- 0x0099D660 ?_HyperlinkProc@@YGJPAUHWND__@@IIJ@Z -- FableWin.pdb + the only reference is the landed ConvertStaticToHyperlink 0x0099D7D0
- 0x009A01C0 ?ExtendData@?$CBasicString@D@@IAEXK@Z -- Ego_r.pdb + landed callers Append/Insert/ctor
- 0x007FEF40 ?SetActionPointAsFree@CTCUserActionPointBase@@QAEXJ@Z (callee) -- the only (long)->void method in the Ego_r set + body = push_back onto FreeActionPointIndexes (+0xC, PDB layout)
- 0x00772110 ?GetNumShoppers@CTCShop@@QBEJXZ (callee) -- Ego_r name + body `(Shoppers.end - begin) >> 3` over the PDB Shoppers vector at +0x18 of 8-byte pointers (ICF-folded with CShaderPreParser::GetConstantCount)
- 0x00947780 ??$GetAllLeafThings@$$CBVCThingBuilding@@V?$vector@PBVCThingBuilding@@...VCThingFilter_HasTC@@@CThingSearchTools@@QBEX... (callee) -- Ego_r donor order (the only such instantiation, right after WanderAroundShops::Update at Ego_r 0x0096B940; retail 0x00947780 follows the WanderAroundShops methods) + body PeekTypeList(3) / CThingFilter_HasTC 0x006A80A0
### Probable
- 0x00436AC0 ?_M_insert_overflow@?$vector@PBVCThingBuilding@@...@std@@IAEXPAPBVCThingBuilding@@ABQBV3@ABU__true_type@2@I_N@Z -- the overflow the proven 0x00947780 calls for its vector<const CThingBuilding*> (0x009477F5); ICF-folded with vector<CThing*>::_M_insert_overflow
- 0x0099C7E0 ??$append@PBG@?$basic_string@GV?$char_traits@G@std@@V?$allocator@G@2@@std@@QAEAAV01@PBG0ABUforward_iterator_tag@1@@Z -- byte shape of the landed char twin 0x00567D60 with 2-byte chars + callers append [s, s+len); PBG vs PAG iterator not distinguishable (the char version has both, byte-identical)
- 0x0099C550 ?_M_throw_length_error@?$_String_base@GV?$allocator@G@std@@@std@@QBEXXZ -- noreturn callee of every wide grow path; char twin 0x00565A30
- 0x0091ADB0 ??$GetAllThingsInRoughArea@VCThingObject@@...CFilter_Bind2<CThingFilter_ObjectHasProperty,CThingFilter_In2DRange>... -- Ego_r instantiation adjacent to AcquireWorkSpot; 3 arguments
- 0x0091AD70 ?remove@?$list@PAVCThingObject@@V?$allocator@PAVCThingObject@@@std@@@std@@QAEXABQAVCThingObject@@@Z -- STLport list::remove called with the old work spot (ICF-folded; labelled list<CTCBase*>::remove)
- 0x00792240 ??1?$_List_base@PAVCThingObject@@V?$allocator@PAVCThingObject@@@std@@@std@@QAE@XZ -- STLport list has no destructor of its own; ICF-folded across element types
- 0x0094B490 ??$GetAllThings@VCThingObject@@V?$list@PAVCThingObject@@...VCThingFilter_HasScriptName@@... -- Ego_r instantiation + mask 0x807; caller 0x00947EB0
- 0x00995030 ?Read2DVector@CDataInputStream@@QAE?AVC2DVector@@XZ -- reader order
### Descriptive
- 0x013CA828 g_DefStringTable (no PDB data name; existing binding ?g_DefStringTable@@3VCDefStringTable@@A reused)
- 0x013BB6E0 g_GetUpName ("ST_STAND_UP_FROM_SEAT") / 0x013BB6E8 g_GetUpNameAlternate ("ST_STAND_UP_FROM_BENCH") -- the names of the landed OnReplace 0x009387F0
- 0x0138CE30 g_MaxShoppersPerShop (unsigned, initialised 3, read only by 0x00946F90)
- 0x013BD6F8 g_HyperlinkHovered (BOOL; also read by _HyperlinkParentProc 0x0099D5B0)
- AttachEffectToStaff_Use / KrakenLowAttackLoop_FinishAction_Use -- caller devices that keep anonymous-namespace bodies emitted (GOTCHAS chunk 390); not retail functions
- FindThingInRoughArea (0091b010 stand-in body only) -- a declared-only helper inside the same-TU stand-in of 0x0091ADB0; not a retail function

## Label corrections
- 0x00929FC0 "CAIStateGroupBase_SelectRandomElement" -> CAIStateGroup_ApprenticeHero::GetObjectWithScriptName
- 0x00947EB0 "CAIStateGroup_GoToPub_FindMarkerByName" -> CAIStateGroup_GoToPub::GetObjectWithScriptName
- 0x00938A10 "CAIStateGroup_MinionSitAtSchool_Update" -> CAIStateGroup_UsesSeat::GetUpFromSeat
- 0x00946F90 "CAIStateGroup_WanderAroundShops_SelectRandomEnemy" -> FindASimplyDelightfulShop
- 0x00947110 "CAIStateGroup_WanderAroundShops_FindRandomNPC" -> FindPrettyThingInShop
- 0x0091B010 "CAIStateGroup_HandyMan_FindClosestThing" -> CAIStateGroup_HandyMan::AcquireWorkSpot
- 0x00924520 "vfunc_4" -> FinishAction
- 0x00995210 "CPersistTraits<CBrainUpdateZone>::TransferBinaryIn" -> CDataInputStream::Read3DAngles (propagated label; the PDB TransferBinaryIn is `ret 8`)
- 0x0099C7E0 "CCharString_Insert" -> STLport basic_string<wchar_t>::append<const wchar_t*>
- 0x00947780 "GetAllLeafThings<CThingAICreature,...EnemyCreature...>" -> GetAllLeafThings<const CThingBuilding, vector<const CThingBuilding*>, CThingFilter_HasTC>
- 0x0091ADB0 "GetPBestThingInRoughArea<...>" -> GetAllThingsInRoughArea<CThingObject, list<CThingObject*>, CFilter_Bind2<ObjectHasProperty, In2DRange>>
- 0x0099C550 "CTCElectrocutedEffect::OnInitialActivate" -> _String_base<wchar_t>::_M_throw_length_error
- 0x006FE660 "StdList_Clear" -> the (ICF-folded) STLport _List_base<T*>::~_List_base
- 0x007FEF40 "FreeActionPoint" -> CTCUserActionPointBase::SetActionPointAsFree
(29 rows of landed spellings with evidence in NAME_FIXES.tsv.)

## Header debt
- CAIStateGroupBase / CAIBrain / CThingSearchTools / CDataInputStream / CCreatureActionBase / CThingCreatureBase /
  CTCPhysicsBase: the generated engine headers carry no methods (or the PDB layout, not retail's: CCreatureActionBase
  0xA8 retail v 0xB4 PDB; CCombatActionBase 0x114 v 0x12C), so callers that must mangle a base member
  (AIAction_PerformActionAndWait, PeekThingSearchTools, GetPWorldSeed (IBE), Read3DAngles on CDataInputStream itself)
  use local models -- vland's 22 SHARED_HEADER_AVAILABLE warnings. A header with method declarations (or a
  `methods` sidecar) would let these include the shared header.
- CAIStateGroup_UsesSeat / _WanderAroundShops / _HandyMan / _GoToPub have no engine headers (PDB offsets equal retail
  for these: CAIStateGroupBase is 0x14 in both).
