# Hand chunk 403 (CCreatureInteraction .. NCameraModes): 37 of 40 byte-exact

All 40 rows are module `_global` in the catalog. Every row was renamed from caller, vtable and PDB-name-set evidence;
see Label corrections. gall.sh: 37/37 MATCH or RELOCATION_MATCH (3 MATCH, 34 RELOCATION_MATCH). tall.sh (g++) and
tall.py (VC7.1): 37/37 PASS. No flags or optimize pragmas were needed. Nothing was skipped.

## What matched (37)

- **CCreatureInteraction** (00863200, constructor). The list is the inline STLport list constructor: zero the proxy,
  malloc the 0x0C sentinel, self-link it through a local, then store it. After that come the Type initialiser and an
  inlined push_back. The push_back is `_M_create_node` with a placement-new null test, then the four link stores.
- **STLport list<T> default constructor** (00864f60, 0x1C-byte element). The sentinel goes in a local node pointer that
  is self-linked BEFORE the member store; assigning the member first stores `[esi]` too early.
- **NCreatureAI** (00864170 IsThingRunningAwayFromThing, 008644e0 IsSpeaking, 00864580 StopSpeaking,
  00864620 IsAwareOfPosition, 00864840 IsBestTargetForAttack, 00864cb0 IsThingRunningAwayFromCombat).
  - 00864170: an inline `C3DVector::Get2D()` returning C2DVector(X, Y) builds the 2D facing vector. Two bool locals are
    combined by `if (a && b) return true; return false;`.
  - The GetInterface out-pointer template must be `__forceinline` (00864580, 00864cb0, 008644e0). It keeps the component
    in eax/esi. The HasTC + FindEntry form instead loads the component straight into ecx for the call (00864840 uses the
    landed 0x0091EB60 FindEntry).
  - 00864580: the empty string is a NAMED local built before the argument pushes.
  - 008644e0: `return talking || emoting;` is int-valued (`mov eax,1`).
  - 00864620 reads GetInterface's out-pointer without checking it. The not-present path reads the uninitialised slot.
- **Anonymous-namespace combat helpers** (00865ac0 IsKnockdownToStun, 00865e40 IsHitToDaze). These are `__fastcall` file
  statics with a `Call...` wrapper under `auto_inline(off)`.
  - The health is a named float local read BEFORE the threshold call (00865ac0).
  - `Health < GetDazedHealthThreshold()` puts the call first (00865e40).
  - CThingCreatureBase is modelled locally (see Review at landing in LESSONS.md).
- **CHitParameters / armour helpers** (00868690 GetDamageToThing; 008686e0, 008687a0, 00868a80 anonymous-namespace armour
  helpers).
  - STLport map/multimap walks use `_Rb_global<bool>::_M_increment` inlined.
  - `find` is the STLport lower-bound-then-check loop, with the def pointer's map spelled `it->first->Map` on both sides
    of the `!= end()` compare. A reference local moves the map address into edx.
  - VC7.1 hoists a loop-invariant divisor into a reciprocal multiply (`it->second / totalWeight * multiplier`).
- **CRandomAppearanceMorph and its STLport instantiations** (0086a9d0, 0086ac40, 0086aea0, 0086af60, 0086b2a0, 0086b900,
  0086bb60, 0086bc10, 0086bc90, 0086c5c0, 0086cc50, 0086d740, 0086dc50).
  - CVectorMap is vector + empty Compare + `bool Dirty`. Insert is `push_back(pair(key, value)); Dirty = true;` and
    Update is `if (Dirty) { sort; Dirty = false; }`.
  - The push_back callers match only with a same-TU `_M_insert_overflow` stand-in under `auto_inline(off)` +
    `__declspec(noinline)`. A bare declaration spills the pair temporary or emits `add [esi+4],N`.
  - 0086bc10 also needs `__forceinline` on Insert.
  - 0086b900:
    - a block-scoped named CCharString gives `lea` of the temporary;
    - `__false_type` needs a user constructor, or VC zero-stores the tag.
  - Count (0086a9d0) is `UpperBound - LowerBound`, with UpperBound as the left operand.
  - STLport pair has a user copy constructor (`first(o.first), second(o.second)`).
  - The `__unguarded_insertion_sort_aux`, `__linear_insert` and `__copy` instantiations are the STLport 4 bodies with
    same-TU noinline callees.
- **CVectorMap<long, CLongMapValue8>** (0086f3e0, 0086f600 Add, 0086fa00). This is the same family with a descriptive
  8-byte value. Add is `Insert(value.Key, value); Sort();`.
- **CTCCreatureHitNotification** (008650e0 UpdateHitNotification). This is the erase-or-advance loop over the inline
  STLport `vector::erase`:
  - `if (pos + 1 != end()) __copy(...)`, where the element assignment is list operator= plus the frame word;
  - then `--finish; _Destroy(finish)`;
  - the list destructor is clear() followed by a null-tested free of the sentinel.

  It matched on the first grade.
- **CThingComponentSet** (00870930 Remove, 008709c0 GetDriverInfo).
  - Remove is an erase-or-advance loop with `end` recomputed from `GetAsCArray() + GetNoComponents()`.
  - GetDriverInfo is `*find_if(begin, begin + n, CPred_EntryIsDriver())`. The 3-argument find_if is inline; the
    4-argument tag dispatch 0x00870530 is only declared.
- **CTCDInternalMarker::Create** (00870bb0). It is the landed CTCDExperienceOrb::Create shape without the Kill branch.
- **GetDataFromEventString** (00870c90). The loop is `while (--index != -1)`. The start is `FindCharPos(' ') + 1`,
  tested against 0. Each GetSubString temporary sits in the dead argument slot.
- **NCameraModes** (00874e00 CPCMouseLook constructor, 00874ee0 IsThemeToBlockCameraBetween).
  - 00874e00 is the base constructor, the vtable store and the inline STLport list constructor.
  - In 00874ee0 the two bool locals feed an if/else whose two arms are identical. VC keeps the dead `test bl,bl`.

## Parked for Fable 5.1 (3)

- 00864ff0 CTCCreatureHitNotification::DrawHitNotification DIFFER(227v227): retail keeps this in edi and the flash iterator in ebx (ours ebx/edi), loads `info` into edx (ours eax) and splits the first end() compare `mov eax,[edi+10h] ; cmp ebx,eax` (ours `cmp edi,[ebx+10h]`); everything else, including the temporary's inline list destructor, matches | tried: CDefPointer member, loop-body engine local, engine + 64-bit UniqueID locals (best), iterator declared at function scope | next: model Flashes with the real STLport `_Vector_base` and const begin()/end() accessors, or iterate through a `const vector<CFlash>&` local; otherwise lane gap: register allocation
- 00867fe0 CAnimationEntry::GetComponent<CAnimComponentFollowingOnAnim> DIFFER(120v125): the key is re-read from its slot for the `key < it->first` compares (retail compares against the immediate 8: `cmp [eax],8 ; jle/jg`), and its store is not hoisted above the null test (retail does one `mov [esp+4],8` before `je` and reuses it for the default-map lookup) | tried: named/const local, by-value find with an inner key copy, static-const and rvalue-function keys (5 grades) | next: a same-TU LowerBound stand-in body (auto_inline(off) + noinline) so VC sees the key unchanged
- 00871d50 NCameraModes::CStringAndCageBase::UpdateCageRadius DIFFER(227v227): retail squares the spilled offset.X first (`fld [esp+4] ; fmul [esp+4] ; fld st(1) ; fmul st(2)`); ours squares the register-held offset.Y first (`fld st(0) ; fmul st(1) ; fld [esp+4] ; fmul [esp+4]`); nothing else differs | tried: float dx/dy locals, the sum written Y-first (same code), a C3DVector offset local (best: fixes the frame and slot homes), an inline C3DVector::GetMagnitude2D() (217 bytes) | next: a C2DVector offset or an inline GetSquaredMagnitude2D() returning `X * X + Y * Y` from a separate named local; otherwise lane gap: x87 operand scheduling

## Skipped

None. No row holds padding followed by another function, and no row ends in int3 after a noreturn call.

## Label corrections

The propagated manifest labels are wrong for every row below. Ego_r names were checked against the PDB name sets;
"descriptive" means there is no PDB name.

- 00863200 "CreatureInteraction_InitializeList" = `CCreatureInteraction::CCreatureInteraction(ECreatureInteractionType, CThingCreatureBase&)`.
- 00864f60 "LinkedList_Init" = the STLport `list<T>` default constructor (0x1C-byte element; descriptive).
- 00864ff0 "CThingPhysical_RenderWeaponTrails" = `CTCCreatureHitNotification::DrawHitNotification(const CEnginePrimitivePrepareInfo&) const`.
  The component is interface type 0xCD (vtable 0x012436CC slot 21).
- 008650e0 "NDisplayView_CleanupLipSyncPhonemesOnMemoryThreshold" = `CTCCreatureHitNotification::UpdateHitNotification()`.
- 00867fe0 "CTCBase_GetInterfaceAbility" = `CAnimationEntry::GetComponent<CAnimComponentFollowingOnAnim>`. Its byte twin
  0x00867F60 is the CAnimComponentFollowingOnAnimFilter instantiation.
- 00868690 "Health_ComputeWithEffects" = `CHitParameters::GetDamageToThing(const CThingGameObject&) const`.
- 008686e0, 008687a0, 00868a80 "CTCBase_*FromArmourTree / *FromTree" = the anonymous-namespace
  `GetArmourThresholdValueFromArmourWeights`, `GetArmourResponseDamageAttributeDamageMultiplier` and
  `GetArmourResponseAugmentationDamageMultiplier`.
- 0086a9d0 "CVectorMap_FindEqualRange" = `CVectorMap<long, CSkeletalMorph>::Count(const long&) const`.
- 0086ac40 "fill<C3DClothQuad*,C3DClothQuad>" = `__uninitialized_fill_n<pair<long, CSkeletalMorph>*>`. This is an
  ICF-shared 16-byte body.
- 0086aea0, 0086af60, 0086f3e0 "_Copy_impl<CNavigationPosition*>" = `__unguarded_insertion_sort_aux` over
  pair<ulong, CTextureMorph>, pair<long, CSkeletalMorph> and pair<long, CLongMapValue8>.
- 0086b2a0 "Vector_CopyInit" = `vector<CMorphSet::CMorphSetTextureMorph>::vector(const vector&)`.
- 0086b900 "CreateRandomMorphEntry" = `CRandomAppearanceMorph::AddSkeletalMorphToMorphSet`.
- 0086bb60 "CRegion_AddNavigationExit" = `CRandomAppearanceMorph::AddSkeletalMorph`.
- 0086bc10 "TextureMorph_AddToList" = `CRandomAppearanceMorph::AddTextureToTextureMorph`.
- 0086bc90 "CRandomAppearanceMorph_AddTextureMorph" = `CRandomAppearanceMorph::AddTextureMorphToAll`.
- 0086c5c0, 0086fa00 "CPersistInfo_InsertionSortStep" = `__linear_insert` (pair<ulong, CTextureMorph> and pair<long, CLongMapValue8>).
- 0086cc50 "Vector_Reallocate" = `vector<pair<long, CSkeletalMorph>>::_M_insert_overflow`.
- 0086d740 "Vector_PushBack_4dword" = `CVectorMap<long, CSkeletalMorph>::Insert`.
- 0086dc50 "_Move<CEngineTextureMorph*>" = `__copy<CRandomAppearanceMorph::CBodyPartMesh*>`.
- 0086f600 "PriorityQueue_PushSift" = `CVectorMap<long, CLongMapValue8>::Add` (descriptive).
- 00870930 "CSmallVector_EraseByName" = `CThingComponentSet::Remove(const CCharString&)`.
- 008709c0 "CWorldDriver_FindClosestMeshVolume_DefaultCall" = `CThingComponentSet::GetDriverInfo() const`.
- 00870bb0 "Create" = `CTCDInternalMarker::Create(long, const C3DVector&)`.
- 00871d50 "ParticleMorph_UpdatePosition" = `NCameraModes::CStringAndCageBase::UpdateCageRadius(C3DVector&) const`.
  Its sibling 0x008722D0 is `UpdateStringLength`.
- 00874e00 "CActionParam_Constructor" = `NCameraModes::CPCMouseLook::CPCMouseLook(CGameCameraManager&, const CDefPointer<const CCameraModeDef>&)`.
  0x0084E420 is the NCameraModes::CBase constructor.
- 00874ee0 "CWorldMap_AreBothPosCameraBlocked" = `NCameraModes::IsThemeToBlockCameraBetween`. 0x00874F10 is
  IsThemeToBlockPassabilityBetween.
- Callees:
  - 0x0076FF90 "std::vector<CLipSyncPhoneme>::operator=" = STLport `list<CEnginePrimitiveWeaponTrail::CWeaponTrailEntry>::operator=`.
  - 0x004C73D0 = CThing::GetPosition.
  - 0x00665260 = CThingCreatureBase::IsPosInFOV.
  - 0x006CB610 = CTCCreatureNavigation::IsPosNavLineOfSightToPos.
  - 0x00662030 = GetStunnedHealthThreshold (not CTCPhysicsBase::GetAirResistance).
  - 0x00870B30 = CTCDInternalMarker::Construct.
  - 0x0099EAE0 = ~CCharString.
  - 0x009D5060 = CDefString(const CCharString&).
  - 0x00870530 = the 4-argument find_if.
  - 0x006741E0 = vector operator=.
  - 0x0086DD20 and 0x0086DD80 = sort.
  - 0x0086CB30 and 0x0086FC00 = `_M_insert_overflow`.
