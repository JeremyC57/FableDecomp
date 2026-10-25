# Hand chunk 481 (NCreatureMode .. NPhysicsHelpers): 28 of 40 byte-exact

All 28 src/ rows grade RELOCATION_MATCH (final `gall.sh` run in grades.txt). Their tests print PASS under g++ (`tall.sh`) and VC7.1 (`tall.py`). `calls.py` pairs every relocation of every src/ leaf with retail: there are no import mismatches, and all 36 float-pool references bind to retail constants of the same value. 12 rows are parked in differ/, and nothing is skipped.

## What matched (28)

### NCreatureMode
- 0085ff10 GetUpFromSitting
  - If the creature's current mode is the sitting mode, it ends that mode with a get-up action. Otherwise it offers a plain get-up action: through the brain (output type 8, a clone) or by setting it directly.
  - Shape: the mode-manager pointer is assigned under HasTC(0x31) into an otherwise unset local. The two actions are block-scoped so they share one temp slot. The 2-arg action ctor is forceinline, and Clone goes through a base-reference inline helper.
- 00861e60 GetRandomAnimationFromCreature
  - Picks a random animation entry filed under the name's CRC, among those that pass the filter, else among the no-group ones.
  - Shape: an `/Oa` TU (the string data pointer is loaded once for both CRC calls). Same-TU bodies stand in for `_M_insert_overflow` and GetSubDef, and the def is released through a DecRefToZero dtor.
- 008df510 CModeScorpionKingTailPlunge::Recover (catalog: CTCLightningOrb_OnUpdate)
  - Kills the tail emitter, spawns TAIL_DUST_EFFECT, kills the dependent things and starts the withdraw action.
  - Shape: the landed Spike 0x008DA0F0 model, with same-TU out-only GetSubDef/GetEffectFromString stand-ins.
- 008ebb50 / 008ebc90 `std::__find_if<CHoverState, CIsValidHoverStateFor{MovementType,Velocity}>`
  - Shape: STLport 4.6 4x-unrolled `__find_if` verbatim. 008ebb50 needs a same-TU IsPosInArea so that `ecx` stays live.
- 008ec070 `vector<CModeHoverBase::CHoverState>::_M_insert_overflow` (catalog: CExplosionRing): STLport regrowth verbatim.

### NCreatureAI
- 00864250 IsAwareOfThingImplementation
  - True when the creature perceives the thing through the given perception member pointer.
  - Shape: forceinline HasTC; the `thing == GetPerceivedThing()` operand order.
- 00864700 IsAwareOfThingIndirectly (catalog label propagated from 00864250)
  - Shape: the same body without the FOV test.
- 008643a0 Talk
  - Plays the creature's entity sound at the listener.
  - Shape: a function-scope entity-sound def plus a same-TU GetSubDef stand-in (no reload at release).
- 008648a0 IsWillingToAttackTarget
  - Shape: the IsOverridingFactionEnemyOnTarget skeleton inverted; `if (state != 3) {..} else return true;` for block order.
- 00864b30 IsOverridingFactionEnemyOnTarget
  - Shape: a same-TU GetSubDef keeps the def in ebx; the scripted-control flag avoids a duplicated epilogue.
- 00864d00 IsToAttackTarget
  - Shape: out-pointer GetTC/PeekTC forceinline, with everything under `if (IsBestTargetForAttack)` and one shared `return false`.

### Containers and find helpers
- 00870530 `std::__find_if<CThingComponentSet::CEntry, CPred_EntryIsDriver>` (catalog: CWorldDriver_FindClosestMeshVolume)
  - Shape: STLport `__find_if` over the packed 9-byte entries; the early `return false` keeps setne/test.
- 00888ee0 `vector<SElement12>::operator=` and 00889f00 `vector<CBlendedEnvironmentTheme>::operator=`
  - Shape: STLport assignment verbatim. 00889f00 uses same-TU noinline helper stand-ins, which home the tags on the x slot.
- 0088b4f0 `vector<C3DSkeletalBoneMorph>::_M_insert_overflow` and 008f0450 `vector<Coin>::_M_insert_overflow`
  - Shape: STLport 4.6 regrowth: `(max)` returns a reference, the new block is parked in fill_len's slot, and the fill is a placement copy.

### NCameraModes
- 00876e10 CTools::PotentialFieldBufferPoints
  - Forward-difference gradient of the buffer points' potential field.
  - Shape: a per-component `/ step` becomes VC's reciprocal multiply; four zeroed potentials.
- 00878db0 CProjectileWeapon::GetLookDirectionFromParams (catalog: CalculatePitch)
  - Pitches the horizontal look direction so the observed thing (+1.6 Z) is centred.
  - Shape:
    - the fpatan's `fxch st(1); fxch st(1)` pair needs atan2's x to be a float local (`float xy = (float)sqrt(..)`);
    - the rotation axis is the side vector `(look.Y, -look.X, 0)` negated memberwise (double fchs, +0.0f Z);
    - the pitch is wrapped into [0,1) with fmod.

### NScreenEffect / CEnvironmentSpellEffectInterface
- 008814c0 CScreenEffectPriorityStack dtor
  - Shape: a user-declared empty virtual dtor (vtable store), then the members' inline STLport teardown.
- 00882fe0 CScreenFilterParams::LerpTo, 008854a0 InterpolateRadialBlurParams, 00885680 InterpolateRadialBlurPositionParams
  - Shape:
    - the clamp is `if (t < 0) t = 0; else if (t > 1) t = 1;` on t's own argument slot;
    - a user default ctor puts the hidden result pointer in ecx (008854a0);
    - the blend is `from * (1 - t) + to * t`.

### NEnvironmentInternal
- 008894a0 CBlendedEnvironmentTheme implicit copy ctor
  - Shape: memberwise copy, `rep movsd` blocks for the POD arrays, then the 3 nested arrays.

### Actions and tavern game
- 008b7cf0 CCreatureAction_UnsheatheWeapons::ShouldPlayUnsheatheWeaponAction
  - Shape: named CCharString blocks; a combined `< 0 || !HasTC` test places the epilogue. Case 5 is written before case 4 (the reloc-masked grader cannot see jump-table order).
- 008f4990 GetActionAnimSetList
  - Shape: `if (HasTC(0x5A)) complex = GetTC(0x5A);` into an unset pointer, and an anonymous-namespace `_Use` caller.
- 008f06f0 CCard::FrameUpdate (catalog: CTCTavernGame_UpdateCardTilt)
  - Lerps the card's move and turn, then lifts and tilts it, then calls PositionMesh.
  - Shape:
    - dt stays on the x87 stack;
    - the double pool 0x0124F2B8 is `turn * (double)TWO_PI` with `static const float TWO_PI = 6.2831855f`;
    - tilt/lift are float locals, and the C3DVector operators are by value.
- 008f2280 CTCTavernGameCardBase::CardsMoving (catalog: OnFrameUpdate; vtable slot 52)
  - Shape:
    - `Moving == true` gives `cmp byte,dl` with dl=1;
    - an explicit `return` per state branch duplicates the release and epilogue;
    - a same-TU GetCardDef stand-in drops the null test;
    - the `(53 - i)` counter is strength-reduced into a decrementing memory counter.

## Parked for Fable 5.1 (12)
- 00868e50 GetArmourWeightsFromThing DIFFER(456v456): ours `fld [edi+60h] ; fadd [eax]` v retail `fld [eax] ; fadd [edi+60h]` in `total_weight += weighting` | tried: `+=` on the reference, same-TU GetDef/GetSubDef/_M_insert stand-ins, STLport list ctor | next: `total_weight = total_weight + weighting` or a float local total stored once, else "lane gap: x87 operand commutation"
- 008722d0 NCameraModes::CStringAndCageBase::UpdateStringLength DIFFER(417v419): the 2D length is kept with `fst` where retail does `fstp`/`fld`; retail keeps 1/dist on the stack (`fst ; fmul st(2) ; fstp st(1)`) | tried: 20 diagnostic variants (length after dist, named/unnamed inverse, operator/ vs *inv, QFE toolset, /Oa) | next: lane gap: x87 scheduling
- 00873690 NCameraModes::CFirstPerson::Update DIFFER(603v603): frame 0x40 v 0x48 (retail parks the offset height at the top of the frame), plus the x87 order of the yawed-direction sum and the radius scale | tried: phi local stored before the member, `C3DVector(Base.X, Base.Y, Base.Z + height) + dir * r` temporary | next: declare the height local first / in an outer block to move its slot, then revisit the x87 order
- 00879680 NCameraModes::CProjectileWeapon::GetPhiForPosition DIFFER(535v573): frame 0x54 v 0x48 (retail reuses the position copy's slot for the roots vector<float> and parks x in a third slot), so frame offsets and the store schedule differ; the logic and call sequence match | tried: one form over GetRootsNumerically<CTanMethod>/<CCotMethod> (0x0087AA10/0x0087AFF0, hidden vector<float>) | next: block-scope the position copy so its slot dies before the roots vector; x as its own named local
- 008818a0 NScreenEffect::CScreenEffectPriorityStack::DeleteScreenFilterEntry DIFFER(366v410): default-params zero stores Contrast, Brightness, Alpha v retail Brightness, Contrast, Alpha; the found-entry copy is `rep movsd` v retail memberwise mov pairs (4 scalars, then Tint via `add eax,20h`) | tried: 4 grades (store orders, assignment forms) | next: a CScreenFilterParams default ctor initialising Brightness first plus a user copy assignment copying members in order
- 00881c90 NScreenEffect::CScreenEffectPriorityStack::InsertScreenFilterEntry DIFFER(874v874): register allocation (&things[i] in ebp for the first two GetPItem calls, the operator[] result in ebx), the default-params zero-store order, and the tail back() re-fetch registers | tried: per-use `ScreenFilterParamStack[priority].back()` re-fetch, cached reference, rb-tree base-ctor ordering, forced inlining (9 diagnostic variants, 1 grade) | next: share the CScreenFilterParams default-ctor fix of 008818a0, else "lane gap: register allocation"
- 00888a60 NEnvironmentInternal::CEnvironmentThemeResourcePallette::Build DIFFER(273v289): in the def loop retail builds the CCharString before pushing (index, &day), keeps the manager in ebp, the count in a frame slot and i*12 in the manager's dead argument slot; ours puts day in that slot (frame 0x14 v 0x18) | tried: 4 grades (loop forms, index types, temporary placement) | next: a named count local before the loop and the name built in a nested block
- 0088ac90 GFSaveSkeletalMorphTargetToFile DIFFER(409v417): retail homes `file` in edi (spilled for the loop) and keeps the vector start in ebx into the first BoneName access (frame 0x28); ours keeps `file` in ebp and reloads [esi+4] (frame 0x20) | tried: PDB accessors GetBoneMorphs/GetName/GetScaling, _Vector_base/begin()-size/operator[], int index, hoisted i, while loop, separate end marker | next: lane gap: register allocation
- 0088ae40 GFSerialisePointer DIFFER(433v431): register allocation throughout (save arm: retail keeps the thing in eax and loads `level` after the UID pushes, ours hoists `level` into edx; the Transfer `lea` temporaries and the World->DefinitionManager chain rotate registers) | tried: 1 grade (scheduling-only) | next: lane gap: register allocation
- 0088b0c0 GFLoadSkeletalMorphTargetFromFile DIFFER(970v971): exit layout only. Retail inlines the implicit ~CStringParser at the start-marker return alone and calls 0x004047A0 elsewhere, so the four loop-failure returns (~bone_name) fall into the shared ~CArray/~CStringParser calls; ours also inlines it at the loop-failure returns, which get their own block after the `ret` | tried: explicit CCharString temps, named-local helpers, negated helpers, an inline/forceinline bone-block reader, out-of-line ~CArray (8 diagnostic variants, 1 grade) | next: give the parser dtor a single call site (loop failures `break` into a `do { } while (0)` / goto to the cleanup), or an out-of-line ~CStringParser with an inline twin for the early return
- 008e8e50 NCreatureMode::GetStrafingAnimationName DIFFER(409v405): retail `fstp st(3) ; fstp st(0) ; fstp st(0) ; fst [esp+4] ; fcomp eps` v ours `fstp [esp+4] ; fstp st(0) x3 ; fld [esp+4] ; fcomp eps` | tried: ~50 diagnostic variants (statement orders, magnitude forms, assignment-in-condition, /Oa /Ow /G5-/G7 /Ox, QFE toolset) | next: lane gap: x87 scheduling
- 008f5970 NPhysicsHelpers::GetClosestPointOnFiniteLineToPoint DIFFER(465v465): retail `fst [esp+30h] ; fmul st(1) ; fstp st(2) ; fstp st(0)` v ours `fstp [esp+30h] ; fmul [esp+30h] ; fstp st(1)` | tried: ~30 diagnostic variants (named vs ctor-return operators, in-place vs separate direction, t named/unnamed, friend t*v, /= forms) | next: lane gap: x87 scheduling (a separate `direction` local gives the fst form but moves the slots)

## Skipped
None. No row covers two functions.

## Label corrections

### Rows in this chunk

| Address | Catalog label | Correct identity |
|---|---|---|
| 0x00861E60 | Animation_SelectRandomFromAbilityAnimations | NCreatureMode::GetRandomAnimationFromCreature |
| 0x00864700 | IsAwareOfThingImplementation (propagated) | NCreatureAI::IsAwareOfThingIndirectly |
| 0x00870530 | CWorldDriver_FindClosestMeshVolume | `std::__find_if<CThingComponentSet::CEntry, CPred_EntryIsDriver>` |
| 0x008722D0 | ParticleMorph_UpdatePositionWithVelocity | NCameraModes::CStringAndCageBase::UpdateStringLength |
| 0x00873690 | CVertexBufferWin32_UpdateTransform | NCameraModes::CFirstPerson::Update (vtable 0x012776AC slot 1) |
| 0x00876E10 | Vector_ComputePotentialGradient | NCameraModes::CTools::PotentialFieldBufferPoints |
| 0x00878DB0 | CProjectileWeapon_CalculatePitch | NCameraModes::CProjectileWeapon::GetLookDirectionFromParams |
| 0x00879680 | ProjectileWeapon_CalcPhiAngle | NCameraModes::CProjectileWeapon::GetPhiForPosition |
| 0x008814C0 | CEnvironment_Destructor | NScreenEffect::CScreenEffectPriorityStack::~CScreenEffectPriorityStack |
| 0x008818A0 | CEnvironmentSpellEffectInterface_UpdateRadialBlurParams | NScreenEffect::CScreenEffectPriorityStack::DeleteScreenFilterEntry |
| 0x00881C90 | CEnvironmentSpellEffectInterface_AddScreenFilter | NScreenEffect::CScreenEffectPriorityStack::InsertScreenFilterEntry |
| 0x00882FE0 | InterpolateRadialBlurParams | CEnvironmentSpellEffectInterface::CScreenFilterParams::LerpTo |
| 0x00888A60 | CEnvironmentThemeSetDay_BuildDay | NEnvironmentInternal::CEnvironmentThemeResourcePallette::Build |
| 0x008894A0 | LargeStruct_CopyWithNestedArrays | NEnvironmentInternal::CBlendedEnvironmentTheme copy ctor |
| 0x00888EE0 / 0x00889F00 | Vector_Reallocate | `vector<SElement12>::operator=` / `vector<CBlendedEnvironmentTheme>::operator=` |
| 0x0088B4F0 / 0x008F0450 | Vector_Reallocate | `vector<C3DSkeletalBoneMorph>::_M_insert_overflow` / `vector<Coin>::_M_insert_overflow` |
| 0x008DF510 | CTCLightningOrb_OnUpdate | NCreatureMode::CModeScorpionKingTailPlunge::Recover |
| 0x008E8E50 | GetStrafeAnimationName | NCreatureMode::GetStrafingAnimationName |
| 0x008EBB50 / 0x008EBC90 | Search_GeometryQuad* | `std::__find_if<CHoverState, CIsValidHoverStateForMovementType / ForVelocity>` |
| 0x008EC070 | `std_vector_assign<CExplosionRing>` | `vector<CModeHoverBase::CHoverState>::_M_insert_overflow` |
| 0x008F06F0 | CTCTavernGame_UpdateCardTilt | CCard::FrameUpdate |
| 0x008F2280 | CTCTavernGameCardBase_OnFrameUpdate | CTCTavernGameCardBase::CardsMoving (slot 52) |
| 0x008F5970 | Geometry_ClosestPointOnLineSegment | NPhysicsHelpers::GetClosestPointOnFiniteLineToPoint |

### Callees

| Address | Catalog label | Correct identity |
|---|---|---|
| 0x00681850 | IsWillingToAttackHeroOnOpinion | IsFactionEnemyToHeroOverriddenOnOpinion |
| 0x00811340 | CPlayer::HasMode | CTCEntityEvents::HasEventOfType |
| 0x0073A6E0 | CountReferences | CTCCreatureModeManager::GetCurrentMode |
| 0x0062E900 | ~CTCLightningOrb | ~CCombatActionBase |
| 0x008574D0 | SetPDef | CCombatActionBase::SetCombatAnimation |
| 0x0088B8F0 | RemoveAvailableQuestCardType | CAIBrain::SetOutput |
| 0x006623F0 / 0x00662400 | CPixelShader::CountReferences | GetMeleeWeaponDef / GetRangedWeaponDef |
| 0x00A14440 | GetScaled | C3DVector::AccurateNormalise |
| 0x008F28A0 | std_deque_rotate_insert | `vector<CCard*>::_M_range_insert<CCard**>` |
| 0x0084F1B0 | _Cons_val | NCameraModes::CBase::GetObservedThing |
| 0x004C73D0 | CVertexBufferWin32::DoSizeof | CThing::GetPosition |
| 0x0084D080 | CVertexBufferWin32::DoSizeof | NCameraModes::CBase::GetCamTools |
| 0x0087AA10 / 0x0087AFF0 | ProjectileWeapon_CalcPhiAngleTrajectory(_Alt) | `NMaths::GetRootsNumerically<CTanMethod>` / `<CCotMethod>` |
| 0x0045EF2D | CActiveFile::operator= | `vector<float>::operator=` |
| 0x0099EAE0 | _Cons_val | CCharString::~CCharString |
| 0x009B9BA0 | CThingCreatureBase::SerialiseGameState | CStringParser::NextItemIs (FindKeyword in two landed sources) |

### Unlabelled callees identified
- 0x00662010 CThingAICreature::GetCurrentStateGroupType
- 0x0065FBA4 CSoundMap::GetPSoundPair
- 0x0066B280 CTCSoundPlayer::PlayAttachedSample
- 0x00669580 `GetSubDef<CSpecialEffectsDef>`
- 0x00669D30 CSpecialEffectsDef::GetEffectFromString
- 0x007AEF60 the 0xAD component's KillDependentThings
- 0x008E45B0 CCreatureAction_ScorpionTailWithdraw ctor
- 0x0042B0A2 `GetSubDef<CAppearanceDef>`
- 0x004C7990 CThing::GetDefinitionManager
- 0x0057FF92 `GetSubDef<CAppearanceModifierDef>`
- 0x0042B040 / 0x00817B40 `GetDef<CThingDef>` / `GetDef<CArmourDef>`
- 0x00869CA0 multimap `_Rb_tree::_M_insert`
- 0x00706800 CTCHeroAttachableAppearanceModifiers::GetAppearanceModifierList
- 0x005E1420 `vector<const CAnimationEntry*>::_M_insert_overflow`
- 0x005E9B20 CTCTavernGame::SetState
- 0x008F0B20 CTCTavernGameCardBase::GetCardDef
- 0x00878B60 CProjectileWeapon::GetLookDirectionXYFromParams
- 0x00BFEA70 `__ftol2`
- 0x00A44DF0 C3DSkeletalBoneMorph(const CCharString&, const C3DVector&)
- 0x00A45150 C3DSkeletalMorphTarget::Initialise
- 0x006C39A0 ~CArray<C3DSkeletalBoneMorph>
- 0x004047A0 ~CStringParser

### Other notes
- 0x0088B0C0 matches its PDB name (the catalog agrees).
- Slot 31 of CStringAndCageBase (0x0069DDE0, `xor al,al`) is named IsDriftToMaxStringLength descriptively.
