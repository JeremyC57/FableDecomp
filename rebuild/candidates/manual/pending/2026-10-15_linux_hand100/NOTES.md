# Hand chunk 100 (CTCRotate .. CTCSkeletalMorph): 36 of 40 byte-exact

Linux VC7.1 lane (Wine + DDK cl.exe). All 36 sources in `src/` grade RELOCATION_MATCH (`grades.txt`, via `gall.sh`, honouring
each file's `// Leaf:` line); all 36 behaviour tests print PASS under g++ (`tall.sh`) and under the lane's cl.exe (`tall.py`).
Four rows are parked in `differ/` with their best source and a `// Grade:` line. No row was skipped. Pragmas used:
`optimize("s", on)` (004d2b2a, 007bbc65, 004dc991, 004dd728) and `optimize("y", off)` + `("s", on)` (004de6b6, 007bd7ca,
007bbdca); no `/EHsc` or other flags.

## What matched (36)
- CTCRotate: the constructor 0x004D2B2A (CTCBase(parent), size TU: `push dword ptr [esp+8]` + `and [..],0` = `optimize("s")`;
  the scalar initialisers around a CCharString member are scheduled around the vtable store by themselves) and
  SetRotationInfo 0x0072A530 (12-byte POD axis assigned whole through `lea eax,[esi+0Ch]`; `seconds * g_GameFramesPerSecond`
  loads the extern first).
- SRegionLookupEntry00c46570 (0x00C46570, labelled "CTCRotate::SetRotationInfo"): `{Region(0), Position(v), Length(v.GetLength())}`,
  the 16-byte twin of the landed 0x00C46550 key.
- CTCScriptedControl: IsActive 0x004C70F0 (`if (!m_locked && TasksEmpty()) return 0; return 1;` -- two separate ifs fold to
  `neg al ; sbb ; inc`); SetLocked 0x00712C20 / ClearLocked 0x00712C80 (temporary CIntelligentPointer copied into the +0xc
  controller; the temporary must live in an INNER scope or VC schedules the flag store before its destructor call).
- CTCTargetingPlayer::ClearLockedTarget 0x00802650 (labelled CTCScriptedControl::ClearLocked): same inner-scope temporary shape
  on +0xe4 / +0xf4 (PDB +0xec / +0xfc: the retail class is 8 bytes narrower there).
- IsCreatureButNotHero 0x0085EE10 (labelled CTCScriptedControl::IsActive): a `__fastcall` CThing helper like its landed
  neighbours, `type == 2 && GetDefName() != "CREATURE_HERO"` with the CDefString temporary in the `push ecx` slot.
- CVectorMap<K,V>::Add family (vector at +0, empty Compare +0xc, Dirty +0xd; all labelled CTCSkeletalMorph::AddSkeletalMorph
  except 0x006A3DA0): `{ pair<K,V> p(key, value); m_vector.PushBack(p); } Dirty = true;` -- 0x006A3DA0 (ulong, CIntelligentPointer:
  the pointer's copy constructor = base ctor + SetPItem(other.GetPItem())), 0x006FCF40 (ulong, CSmallVector<CEvent,32>: copy
  constructor = two zeros + Copy(other), destructor `delete[]`), 0x0077F270 (CCharString, 12-byte record), 0x00A72B90
  (CCharString, 16-byte record), 0x00A41A70 ({CCharString, CCharString}, long), 0x00AF2910 / 0x00AF3A40 (CCharString,
  {long, long, CCharString}; the same instantiation in two TUs). The register-per-dword value copies need a memberwise USER
  copy constructor over dword members; CCharString is modelled locally (retail calls its copy constructor 0x0099EC30).
- CTCSearchableContainer / CTCContainerRewardHero: OnSerialise 0x00812370 (slot 1: base OnSerialise, then Transfer<long>
  "NumberOfTimesToSearch" of NumberOfSearchesRemaining at +0x1c with the zero default homed in the dead bool slot);
  CTCContainerRewardHero::OnDie 0x0074F080 (slot 13: the landed inline `GetInterface(0xA0, &out)` template, owner in a named
  local, then `Enabled && type != 2 && type != 13` -> CreateItemPickupContainer).
- CTCShapeManager::DrawPreparePrimitivesForRendering 0x00761B10: the WORLD is the named local and the camera member is read
  twice (`if (world->Camera) pos = world->Camera->GetPosition()`), which loads the camera straight into ecx.
- CTCShop::AddShopper 0x00773DC0 (RemoveShopper, then a named CIntelligentPointer pushed onto +0x18).
- CTCShopKeeper: AddNewCustomer 0x00798D90, HasBeenCustomer 0x00798DD0, EmptyCustomerList 0x00798E10 -- the three recent
  customers are a retail std::vector at +0x1c (ring index +0x28): the CRetailVector iterator-object `operator[]`, the copy
  written `slot.SetPItem(other.GetPItem())` (the inline operator= computes the slot address before the call), and in
  HasBeenCustomer the element bound to a named reference local (`mov eax,[first] ; lea ecx,[off+eax]`); RequiresNewCustomer
  0x007990C0 (nested `!=` ifs with ONE `return true` at the end; the 0x18-byte filter object with an out-of-line constructor
  and no destructor; `return !filter(customer)`); OnDie 0x00799320; GetAreOtherPeopleInShop 0x00799A60 (CWorld::GetWorldFrame
  static, signed `> 15`).
- CTCSkeletalMorph::GetPSkeletalMorph 0x00786870: counted pointer {object, count*} returned through the hidden pointer by an
  inline copy constructor.
- NThingShot shot definitions: MultiArrow constructor 0x007BBC65 (`optimize("s")`; C3DVector(x,y,z) member, StraightArrow
  initialiser, the base's MultiArrowShotType (+0x98) a body assignment); Projectile constructor 0x004DC991 (`optimize("s")`;
  the CEnginePrimitiveHandle member's default constructor must be `__forceinline` under "s"); GetInitialVelocity 0x007BEFB0
  (named float `speed` local, `((0.5f - GFFloatRandom(1.0f, seed)) * 0.4f + 1.0f) * speed`); ~DrainLife 0x007BD80A
  (`if (IS_THING_ALIVE(PSoulEffect)) PSoulEffect->Kill(true);`, implicit member destructor, base tail jump); ~FireballSpell
  0x007BD7CA (`optimize("y", off)` + `("s")`: frame + `leave`, no tail jump); TearInGround::UpdateMovement 0x007BBDCA
  (`y` off + `s`; the position copy needs a memberwise user copy constructor or "s" turns it into `movsd` x3).
- Constructors with propagated "shot definition" labels: CTCDiggingSpot 0x007E2500, CEnginePrimitiveManagerParticleGroup
  0x00BBBF40 (bytes stored before the vtable = initialisers, the byte after it = a body assignment), CTCCreatureItemLevitationBase
  0x004DE6B6 (`y` off + `s`; the STLport list constructor's allocator temporary in the dead argument slot `[ebp+0Bh]`).
- SPrimitiveHandleHolder_004dd728 0x004DD728 (labelled ~CTCShotDefinitionProjectile): the size-optimised twin of the landed
  0x004C4160 handle-holder destructor (`GetRefCount() == 1` with an `Object ? count : 0` getter, `__forceinline` release /
  operator=(0) / member destructor).

## Parked for Fable 5.1 (4)
- 00772790 CTCShop::GetDaysTillRestock DIFFER(78v80): retail keeps this=esi and the node in ebx read back from the iterator frame slot (`push ebx ; ... mov ebx,[esp+0Ch]`), ours colours this=ebx, node=esi via `mov esi,[eax]` | tried: plain expression, `day` local, node local from the find call (best), named iterator + node local | next: an inline find helper returning the node pointer through a reference out-parameter, or a CRetailMap iterator object with operator-> (lane gap: register colouring likely)
- 00779ca0 CTCDExperienceOrb::CTCDExperienceOrb DIFFER(46v46): `xor ecx,ecx ; or eax,-1` (retail) vs `or eax,-1 ; xor ecx,ecx` (ours) | tried: init list, pointer-typed +0xc, the -1 pair and the flag pair as body assignments | next: lane gap: scheduling-only
- 00812340 CTCSearchableContainer::Search DIFFER(36v35): retail decrements via edx and re-reads the member after the clamp store (`mov eax,[ecx+1Ch] ; test eax,eax` into one shared `jne`), ours knows the clamped 0 and calls from the clamp arm | tried: plain --/clamp (`dec [mem] ; jns`), `long& ClampToZero(long&)` (best), local + assignment expression, ternary lvalue, reference local, Dinkumware `Max(m, 0L)` / `Max(m - 1, 0L)`, by-value clamp | next: a `volatile`-free out-of-line-looking helper such as a const-ref `std::max` with a 0 held in a named local, or a clamp on an `unsigned`/`long` mix
- 0087c420 CTCSinglePersonOccupiable::IsOfType DIFFER(91v76): ours reads the def pointer into eax so each CDefPointer release becomes load/dec/store + `mov ecx,eax` (retail `mov ecx,[esp]` then `dec dword ptr [ecx+4] ; jne`), and the prologue order differs (zero store before the push) | tried: member-test destructor (best), local copy in the destructor, Release() on the pointee, optimize("s") | next: the landed CDefPointer model from the GetSubDef callers (0x0087C490 family) with a `__forceinline` destructor and an `operator->` accessor for the flag test

## Skipped
- none (no row holds two functions).

## Label corrections
- 0x00C46570 "CTCRotate::SetRotationInfo" -> SRegionLookupEntry00c46570 constructor (CTCRotate::SetRotationInfo is 0x0072A530).
- 0x006A3DA0 "CTCScriptedControl::ClearLocked" -> CVectorMap<unsigned long, CIntelligentPointer>::Add.
- 0x00802650 "CTCScriptedControl::ClearLocked" -> CTCTargetingPlayer lock clear (ClearLockedTarget; caller CPlayer::UninitModeZTarget).
- 0x0085EE10 "CTCScriptedControl::IsActive" -> `__fastcall` IsCreatureButNotHero(CThing*) (callers CTCTargetingPlayer).
- 0x0074F080 "CTCSearchableContainer::vfunc_13" -> CTCContainerRewardHero::OnDie (TC vtable slot 13 is OnDie).
- 0x00812370 "CTCSearchableContainer::vfunc_1" -> CTCSearchableContainer::OnSerialise.
- 0x00779CA0 "CTCShotDefinitionMultiArrow::CTCShotDefinitionMultiArrow" -> CTCDExperienceOrb constructor (vtable 0x0126ABFC).
- 0x007E2500 "CTCShotDefinitionNymphFun::CTCShotDefinitionNymphFun" -> CTCDiggingSpot constructor (vtable 0x0127017C).
- 0x00BBBF40 "CTCShotDefinitionNymphFun::CTCShotDefinitionNymphFun" -> CEnginePrimitiveManagerParticleGroup constructor (vtable 0x012A5BB8).
- 0x004DE6B6 "CTCShotDefinitionProjectile::CTCShotDefinitionProjectile" -> CTCCreatureItemLevitationBase constructor (vtable 0x0124156C).
- 0x004DD728 "~CTCShotDefinitionProjectile" -> destructor of a non-polymorphic holder of a CEnginePrimitiveHandle at +4.
- 0x006FCF40, 0x0077F270, 0x00A41A70, 0x00A72B90, 0x00AF2910, 0x00AF3A40 "CTCSkeletalMorph::AddSkeletalMorph" -> CVectorMap<K,V>::Add
  instantiations (ghidra_out/egor_signatures.tsv even names 0x00AF3A40 "CSystemManager::ClearAllInputEvents").
- Callee labels: 0x00686A70/0x00686A80/0x00686D30 "CViewBase::GetDisplayEngine" = CTCBase::GetWorld / GetWorld const / GetWorldMap;
  0x0049D870 "GFGetPreMainMemoryUsed" = CWorld::GetWorldFrame; 0x0049D810 "CVertexBufferWin32::DoSizeof" = CWorld::GetPRandomSeed;
  0x00686800 "CEngineSubPrimitive2DClockSprite" = CTCBase::CTCBase(CThing*); 0x0099EAE0 "_Cons_val<...>" = ~CCharString;
  0x00771200 = CTCDriverBase::CTCDriverBase(CThing*); 0x0087C490 = CDefClassBase::GetSubDef<T> (instantiation name folded).
