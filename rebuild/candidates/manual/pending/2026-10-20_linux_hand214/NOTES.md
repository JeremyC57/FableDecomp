# Hand chunk 214 (CThingBuildingDef .. CThingPlayerCreature): 25 of 40 byte-exact

All 25 rows grade RELOCATION_MATCH (re-graded at the end under the RTM 13.10.3077 lane, no pragmas or flags needed
anywhere); every one has a behaviour test that PASSes under g++ and the lane's VC7.1 (tall.sh / tall.py).

## What matched (25)
- Def assignments (catalog "CThingBuildingDef::operator="): 0x004F914A is CHeroExperienceDef::operator= (slot-19 thunk
  0x004F9145, vtable 0x0124390C), 0x0067CE50 COpinionReactionManagerDef::operator= (thunk 0x0067CE40, vtable 0x0125B7DC).
  Memberwise copies over 12-byte retail containers; the second inlines the byte-packed CParentDefClassBase part around the
  out-of-line CDefClassBase::operator= 0x00431F10; the 0x278-byte POD traits block copies as `rep movsd`.
- 0x006F1280 CThingCompareMap::Insert: STLport `_M_insert` for `pair<const u64, CThing*>` (landed CCombatantMap 0x006F4D00
  shape, extern "C" Rebalance_0042971d).
- CThingCompare_BestCreatureToAttack::GetCompareValueForThing (0x007F6BF0 / 0x007F6CB0, byte twins): 1/(sqrt(2D dist)+1)
  with four weights; the last-attacker pointer is read through an inline creature accessor (`lea ecx,[eax+0F4h]`).
- 0x007A2F90 CThingCompare_Nearest::operator()<CThingAICreature>: the plain `X*X+Y*Y+Z*Z` (VC ranks the terms differently
  for the spilled and the stack delta; the statement chain differs).
- CThingCreatureBase: UpdateBodyReorientation 0x006622D0 (scoped_ptr operator->, inline IsPerformingAnyAction),
  UpdateAction 0x006643C0 (block-scoped counted-pointer copy, list promotion), SheatheAllWeapons 0x006647F0 (GetTC must be
  `__forceinline`), CanSeeTarget 0x006650E0 (the angle call nested in AngleDifference's argument, not a named local),
  IsThingInFOV 0x00665190 (`lenSq <= r*r` tail-calls the angle test).
- 0x006B1960 is the CAtmosProcess constructor (catalog label "GetCombatAnimationSet" is wrong): CMyAtmosMap member at +4,
  pack(1) intelligent pointer at +0xa, MAIN_SOUND_SETUP def handed to the sound system.
- Thing filters: IsInSameVolumeContextAsThing 0x006A8C60 (`(!a && !b) || (a && b)` term order), IsMySleepPoint 0x006A8930
  and IsOpinionSourceWithFreshDeed 0x00960FC0 (nested ifs with the single false exit last; inline by-value flag helpers),
  IsPotentialCustomer ctor 0x00799D80 (implicit CCharString conversion re-addressed with lea), NoiseCanBeHeardFromPosition
  0x006A8240 (Dinkumware reference Min, HasTC wrapping the body).
- 0x0051E210 CThingManager::RemapUIDForSerialisation: `(uid >> 40) & 0xffffff` keeps the mask; each arm calls the lookup.
- 0x00637E70 CThingMapManager::GetThingLayerCellPositions: uses engine/CThingMapManager.h through the `_PDB` rename;
  C2DCoordI temporaries, bottom-left corner declared before top-right.
- CThingObject: OnDie 0x007038D0 (component-wise spin, implicit "" name, qualified CThingPhysical::OnDie), slot 19
  0x00704150 = WorldUpdate (catalog "vfunc_19"; out-parameter const GetInterface for TC 0x47, `!(Health <= 0)`).
- CThingPlayerCreature: ResolveMovementAcceleration 0x006AB770 (by-value Magnitude helper, inline `*=`), vfunc_68
  0x00663E90 (ternary GetTC lands the component in ecx; CAnimInit temporary), vfunc_73 0x00662CC0 (def copied into a
  holder released at every exit, restricted false last), vfunc_34 0x006A9D10 (by-value CDefPointer copy; if/else-if clamp).

## Parked for Fable 5.1 (15)
- 008098b0 CThingCompare_BestForTargeting2::operator() DIFFER(183v183): `mov ecx,esi` scheduled before the first score's `fstp` | tried: named scores, folded second score, single expression, by-value position getter | next: lane gap: scheduling only
- 00662ef0 CThingCreatureBase::GetRandomAnimFromNameWithFilter DIFFER(175v169): def spilled into the dead name slot, name/filter cached in registers (retail keeps def in esi and re-reads name) | tried: raw def local, CHeldDef holder, CDefPointer dtor, implicit/explicit DEFAULT | next: the vfunc_73 holder form with GetSubDef in an inline helper
- 00663510 CThingCreatureBase::InitialiseMovementSpeed DIFFER(179v183): retail `fstp ; fld` of the scaled distance vs our `fst` | tried: cast, compound `*=`, inline getter | next: lane gap: store-reload of a float local
- 006648d0 CThingCreatureBase::UnsheatheWeapon DIFFER(239v235): weapon/def in esi/edi swapped, string and def out pointer frame homes swapped, long release form | tried: inline def helper, declaration order, named script-name local | next: CHeldDef holder for the def (vfunc_73 shape)
- 00664ba0 CThingCreatureBase::GetCurrentMeshRadius DIFFER(207v184): failure-path null-tested release long form, key/out separate frame words | tried: CDefPointer dtor, Release() member, local pointer release | next: CHeldDef holder (vfunc_73 shape) or lane gap: inline release
- 006c8de0 CThingFilter_IsEligibleWitness ctor DIFFER(170v172): `xor eax,eax ; test dl,dl ; sete al` (full register) vs `test al,al ; sete dl` for UsePerceiving | tried: `== false`, `!`, inline bool accessor, long local | next: a non-inline-looking int helper (`return x == 0;` returning int)
- 006a8370 CThingFilter_CanCollideOrInteractWithThing::operator() DIFFER(226v224): second IsSolidToThing component kept in eax + `mov ecx,eax` (retail loads it straight into ecx before the argument) | tried: chained, named local, parent local, accessor | next: ternary GetTC (the 0x00663E90 fix)
- 00809030 CThingFilter_IsInTargetingRangeAndFOV ctor DIFFER(166v166): `lea ecx,[esi+0Ch]` scheduled mid facing copy | tried: init-list memberwise copy, implicit copy, body assignments | next: lane gap: scheduling only
- 008090e0 CThingFilter_IsInTargetingRangeAndFOV::operator() DIFFER(192v192): IsInFOV ctor argument loads interleaved with the pushes, lock target in esi | tried: four exit structures | next: lane gap: scheduling/register only
- 007f6df0 CThingFilter_IsValidAttackTargetForCreature::operator() DIFFER(289v239): every creature-def release in the long `add eax,4 ; dec ; test ; mov [eax],..` form | tried: CDefPointer dtor, Release(), direct decrement | next: CHeldDef holder declared after GetSubDef (vfunc_73 shape)
- 00525cd0 CThingManager::OnLevelsUnload DIFFER(192v179): `this` kept in ebx instead of spilled, vector words reloaded per walk, separate binder slot | tried: inline for_each over mem_fun / bind2nd | next: lane gap: register allocation
- 006626e0 CThingCreatureBase::GetAnimMovementVector DIFFER(160v178): retail keeps a partial second copy of the movement vector (X in the dead index slot, Y in a fourth frame word) | tried: member transform, forceinline, chained temporary matrix | next: two by-value C3DVector hops (local plus by-value parameter of a non-member inline)
- 00667b30 CThingCreatureBase::GetAppropriateCombatAnimation DIFFER(218v206): long-form def release and the push_back rvalue in its own slot | tried: one form | next: lane gap: value/tag slot sharing (GOTCHA 389) and inline release
- 006a7fb0 CThingFilter_BoundingBoxOverlapsMap::operator() DIFFER(220v220): frame slot of the top-left coordinate copy (+0x0c vs +0x20) and the two map Y floats swapped | tried: by-value C2DCoordI conversions, C2DBoxI ctor, named coordinate copies | next: lane gap: frame-slot assignment
- 0085cde0 CThingMap::Clear DIFFER(229v214): loop register allocation (retail: this in ebp, layer in ebx with a frame copy) | tried: one speculative form | next: re-derive the loop nest (the walk body may be a compiled-out release)

## Skipped
None.

## Label corrections
- 0x004F914A = CHeroExperienceDef::operator=, 0x0067CE50 = COpinionReactionManagerDef::operator= (both "CThingBuildingDef::operator=").
- 0x006B1960 = CAtmosProcess::CAtmosProcess() (catalog "CThingCreatureBase::GetCombatAnimationSet").
- 0x00667B30 is not GetCombatAnimationSet (returns a CAnimationEntry* picked by GetAppropriateCombatAnimationFromList); descriptive name GetAppropriateCombatAnimation.
- 0x00704150 = CThingObject::WorldUpdate (slot 19; catalog "vfunc_19").
- 0x007F6BF0 / 0x007F6CB0: both catalog rows say GetCompareValueForThing<CThingAICreature>; the callers are the operator()<T> pair at 0x007F6F20 / 0x007F6FD0, so one is the <CThingCreatureBase> instantiation (0x007F6CB0 modelled so; unprovable from bytes).
- Propagated callee labels found wrong: 0x00662430 is CThingCreatureBase::IsCombatant (not C3DAnimationBlendState::AddCombinationBlend); 0x004C7AE0 is the game random-seed getter (not CViewBase::GetDisplayEngine); 0x004C79F0 the player-manager getter; 0x00863260 a CThingBodyReorienter byte setter (not CTCPhysicsNavigator::EnableNavigator); 0x006A7550/0x006A7570/0x006A7580 are thing-filter operator()s (not _Cons_val); 0x007445D0 a CTCPerceiveThing FOV getter (not CTCPhysicsBase::GetAirResistance); 0x005271B0 a CThing member (not CThingFilter_IsValid::operator()); 0x00838BA0 CThingGameObject::WorldUpdate (not EditWorldUpdate).

## Name choices (relocation identity)
- 0x00A01B50 declared PDB-correct as `CBaseObject* CBaseIntelligentPointer::GetPItem() const` (the landed tree also has 432 `CThing*` spellings).
- 0x0044C6B0 as `static CDefinitionManager& __fastcall CDefinitionManager::Get()` and 0x004196B2 as `CDefinitionManager::GetDef<T>` (PDB); the landed CMusicManager ctor spells them CGameDefinitionManager::Get / CDefinitionManager_Calls::GetDef.
- 0x00703210 declared as the landed callers' free `__fastcall CreateObject(...)` (PDB: static CThingObject::Create).
- 0x00666B50 declared on CThingCreatureBase (PDB); the landed CCombatActionBase::Initialise declares it on CThing.
- 0x0045DD2C uses its own landed class name CRetailTree_0045dd9f (callers spell it CReactionEnabledMap / CReactionFlagMap); the other tree operator=s likewise use their landed CRetailTree_<copy fn> names.
- 0x005E1420 declared exactly as the landed CRetailVector_dword::InsertGrow (00667b30, parked).
- 0x0087C420 declared as PDB CTCSinglePersonOccupiable::IsOfType(EOccupyType) (landed: IsOfType(unsigned long) const / HasProperty).
