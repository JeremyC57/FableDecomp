# Hand chunk 61 (CNavigatorQueueManager .. COpinionReactionMaskDef): 35 of 40 byte-exact

Every src/ file grades MATCH or RELOCATION_MATCH (grades.txt) and its test prints PASS under both g++ (tall.sh) and the
lane's VC7.1 cl.exe (tall.py). Pragmas are in the sources; no `// Flags:` were needed.

## What matched (35)

- **CRetailList_00b836e0::Clear** (0x00B836E0, label CNavigatorQueueManager::FrameUpdate is propagated): the VC7.1
  `list::clear` template with a CIVCountedPointer value; the release lives on the pointee (`if (--RefCount == 0)
  Destroy();`) so the object stays in ecx for `dec [ecx+4] ; jne ; call [eax+4]`.
- **CNearestBuildingWithProperty** ctor (0x0065846C) and copy ctor (0x0065D750): the (pos, property) ctor is a size TU
  (`and [eax+10h],0`); under /Os a POD C3DVector copy becomes `movsd` x3, retail's word-by-word edx copy is C3DVector's
  memberwise user copy constructor over raw words. The copy ctor is byte-identical to the landed 0x0049CF80.
- **CNavigatorOwner_00711520::ResetBuffers** (label wrong): two STLport `vector::clear()` = `erase(begin(), end())` --
  `__copy_trivial(finish, finish, start)` written as `(last == first) ? result : memmove(...) + n` gives the retail
  `cmp eax,eax ; jne`, the 8-byte record vector uses the SGI random-access `__copy` loop (`n = (last-first)>>3 ; n > 0`);
  the counters reuse the loop's zero register. Matched first try.
- **SwapKeyedRecord** (0x0086A690, 0x0086AF10, label wrong): /Gr `std::swap` of a {long key, 12-byte POD} pair with a
  memberwise pair copy constructor -- the value block is copied through the re-based pointer `lea esi,[ecx+4] ; mov
  ebx,esi`, ebx spilled into the 16-byte frame. A POD record copies key and value as one block.
- **SThreeWordRecord::Swap** (0x00F0CA30, label wrong): three inlined `std::swap` calls on member words.
- **CNewFrontendGameComponent::Init** (0x0042F75E): `optimize("s")` -- `push dword ptr [esi+10h]` straight from memory;
  XMVCodeLoaded is at retail +0xB1 (PDB +0xB9); `CConsole::Get()->Initialise(...)` pushes before the getter call.
- **CPointerHolder_00c48d90** ctor (label CNoise wrong) and the real **CNoise::CNoise** (0x00F36BA0: Seed +0x1800 then
  InitPerlinNoise).
- **SPrimitiveHandleHolder_004c4160::~** (label CObject wrong): destroys a CEnginePrimitiveHandle member at +4. Needs
  `__forceinline` on the handle destructor, the counted pointer's destructor and its `operator=(0)` (else `add ecx,4 ; jmp`);
  the last-handle test must be an inline `IsUnique()` and the call go through `operator->` to keep the object in eax then
  `mov ecx,eax` (a direct member test loads ecx).
- **CObjectAugmentationParticleSet** copy ctors (0x004E4D3B, 0x004EC46D): `optimize("s")` = every scalar through eax.
- **CObjectAugmentationSoundSet** ctors (0x00765CC0, 0x00768FA0, and 0x007688E0 with `ret 4` = an ignored argument,
  modelled as a non-copying copy constructor): plain init list Played(false), Sound("", -1), Handle(-1).
- **CLocalDetailCacheMap::CObjectCacheGroupCollection** UnloadContents / ~ and **CObjectTypeCollection::~** (0x00BDD9A0,
  0x00BDDE80, 0x00BDDDA0): PDB offsets are exact here; the inline ClearLoadRequest on a `bool:1` bit-field plus
  `while (p) { next = p->Next; delete p; p = next; }`; the CMovableResource base has an inline empty virtual destructor.
  All three matched first try.
- **CBaseObject ??_G** (0x0099A4D0, label CObjectTypeCollection wrong; vtable 0x0129A7D8): the intelligent-pointer
  detach (`object = 0; if (refcount == 0) delete record;`) only inlines into the `??_G` with `__forceinline virtual
  ~CBaseObject()` plus a `new CBaseObject` factory.
- **SStringPairRecord_0055de90** ctor (label CObservable wrong): {CCharString, long -1, CCharString}.
- **CObstacleNavigatorExternalInterface** PeekNavigatorOrientation / PeekNavigatorPosition2D / 3D (0x004DF3F7, 0x004DF434,
  0x004DF44E): the module is `/Os`. LESSON: in a size TU a memberwise float copy constructor (and a C3DVector->C2DVector
  conversion) copies the FIRST float through the x87 (`fld ; fstp`) and the rest through integer registers; the speed
  build moves every word with `mov`. Orientation also needs `optimize("y",off)` (frame pointer + `leave`). CThing's
  physics component is at retail +0x60, its position at +0xC.
- **CTCMeshAutomaticEntityCreator::AddOwnedEntityTC** (0x0072DD60, label ~COpinionDeedEffectsDef wrong; callers are
  CreateObject / CreateBuilding): `created->AddTC("CTCOwnedEntity", false, 0)` as an implicit CCharString conversion
  temporary, the component in a named local across the string destructor, then SetPOwner(m_pThing).
- **COpinionDeedFilter_IsCrimeToRunAndFindAGuard::operator()** (0x00681750): a `switch` with cases 2..9 sharing one arm
  and case 53; `return x != -1 ? true : false;` gives the bare `setne al`. (finish.py names it `operator` because the
  first `(` ends the name; the Leaf line `??RCOpinionDeedFilter_IsCrimeToRunAndFindAGuard` selects the symbol.)
- **COpinionDeedReactionDef** operator= / copy ctor (0x00471E17, 0x00471F60): retail def base 0x3C bytes, Attitudes at
  +0x3C, Animation +0x48, DialogueTag +0x4C (PDB 4 higher); explicit memberwise forms in a size TU.
- **CVtable_012fb358** copy ctor (0x00F25DD0, label COpinionDeedReactionDef wrong; no-RTTI vtable): CDefClassBase copy +
  two {CCharString, word} pairs, the vtable store between the first member's `lea ecx` and its call.
- **COpinionPersonalityDef** ctor (0x00456F71): `new COpinionPersonalityTraits[5]` (out-of-line ctor, no dtor, no cookie)
  goes through the non-EH `vector constructor iterator`; the map member must be word-aligned (an `unsigned char` body put
  it at +0x41); body assignments after the member constructor; size TU.
- **CParticleSimulationUser_00adf5b0::SavePositions** (label COpinionPersonalityDef::operator= wrong): `memcpy(Saved,
  Positions, Size * 16)` on the argument, `this` unused (`ret 4`); callers are the cloth simulations' RunSimulation.
- **COpinionReactionManager** GetAttitudeType / LogDeedReactionPlayingSpeech / GetToleranceToBeingHit: retail
  AttitudeMask +0x554 and DeedReactionSpeechPlaying +0x5A4 (PDB 8 higher); `playing ? 1 : -1` = `setne ; lea
  [edx+edx-1]`; the tolerance map is at def +0x6C (STLport, value at node +0x14), the attitude local homed in the dead
  argument slot, `it != end` found-arm first, `return 0.0f` loads the pool constant 0x0122DEDC.
- **COpinionReactionMaskDef::Transfer** (0x0045DE67): two context Transfer calls, then an STLport map erase loop --
  end() cached, `next` advanced first through the __cdecl `_Rb_global::_M_increment` (`pop ecx`), `erase(iterator)` by
  value with a user copy constructor (`push ecx ; mov eax,esp ; mov [eax],esi`). Matched first try.

## Parked for Fable 5.1 (5)

- 00768a70 SwapKeyedRecord (label CNearestBuildingWithProperty) DIFFER(73v81): retail builds the swap temporary's last three words in the 16-byte frame through esi (`mov esi,[ecx+4] ; mov [esp+8],esi` x3) and copies left=right through esi/edi only, lane keeps them in ebx/ebp/esi/edi | tried: POD, 4-word memberwise ctor (+operator=), pair<long,POD12> with pair ctor (matches 0086a690), member ctor, float members, array members, memcpy member, all again under optimize("s") | next: lane gap: register allocation (temporary not scalar-replaced in retail)
- 00769340 SwapKeyedRecord (label CNearestBuildingWithProperty) DIFFER(73v81): byte-identical to 00768a70, same frame-temporary difference | tried: same set as 00768a70 | next: lane gap: register allocation
- 008068b0 SwapKeyedRecord (label CNearestBuildingWithProperty) DIFFER(73v81): retail spills the temporary's first three words to the frame through eax and keeps the fourth in eax | tried: same set as 00768a70 (12-byte member first and last) | next: lane gap: register allocation
- 00700120 COpinionReactionManager::GetBlurredAttitude DIFFER(79v95): the lane drops the `range ? seed % range : 0` guard after the random step (`test esi,esi ; je` + duplicated `blurred[0]` arm in retail) because the entry test already proved count > 0 | tried: named unsigned count, size() re-evaluated for the range (keeps the guard, recomputes the size after the call: 105), early return with a long count (81) | next: lane gap: value-range propagation (QFE drops a redundant zero test RTM kept)
- 004e9550 CPersistContext::TransferHelper (label CObjectAugmentationParticleSet::TransferHelper) DIFFER(80v77): retail forms the traits `this` (`lea ecx,[ebp-2Ch]`) once above the mode switch's first `je` and the default arm falls into the destructor call without re-forming it; the lane re-forms it per arm | tried: switch, if/else chain (83), __forceinline traits TransferBinary(context, value) holding the switch (80) | next: lane gap: rematerialisation/hoisting of a frame address

## Skipped

None: no row spans two functions (0x008068B0 / 0x0086A690 / 0x0086AF10 / 0x00F0CA30 carry only int3 padding).

## Label corrections

- 0x00B836E0 "CNavigatorQueueManager::FrameUpdate" -> CRetailList_00b836e0::Clear (list of CIVCountedPointer<N3DPrimitive::CTriangleList>).
- 0x00711520 "CNearestBuildingWithProperty::CNearestBuildingWithProperty" -> a two-vector reset called by the navigator-queue owner @ 0x0070EC30.
- 0x00768A70, 0x00769340, 0x008068B0, 0x0086A690, 0x0086AF10 "CNearestBuildingWithProperty" -> /Gr std::swap of 16-byte records.
- 0x00F0CA30 "CNearestBuildingWithProperty" -> three-word member swap (CQ_AmbushTradersScript code).
- 0x00C48D90 "CNoise::CNoise" -> a 0x14-byte holder (three zeroes + CBaseIntelligentPointer); the real CNoise ctor is 0x00F36BA0.
- 0x004C4160 "CObject::~CObject" -> destructor of an owner holding a CEnginePrimitiveHandle at +4.
- 0x0099A4D0 "CObjectTypeCollection::`vector_deleting_destructor'" -> CBaseObject::`scalar deleting destructor' (vtable 0x0129A7D8 slot 0).
- 0x0055DE90 "CObservable::CObservable" -> {CCharString, long -1, CCharString} record ctor.
- 0x0072DD60 "COpinionDeedEffectsDef::~COpinionDeedEffectsDef" -> CTCMeshAutomaticEntityCreator helper adding CTCOwnedEntity.
- 0x00F25DD0 "COpinionDeedReactionDef::COpinionDeedReactionDef" -> copy ctor of the no-RTTI def with vtable 0x012FB358.
- 0x00ADF5B0 "COpinionPersonalityDef::operator=" -> CParticleSimulation SavedPositions memcpy (this unused).
- 0x004E9550 "CObjectAugmentationParticleSet::TransferHelper" -> a CPersistContext member (`this` is the context).
- Callee labels seen wrong: 0x0049D810 is CWorld::GetPRandomSeed (not CVertexBufferWin32::DoSizeof), 0x004292C0 the
  STLport tree increment (not GetNoAllocatedAreas), 0x004534A6 a map constructor (not CopyBackBufferToTexture),
  0x004E9627 / 0x004E25F1 the CPersistTraits<CObjectAugmentationParticleSet> ctor/dtor.
