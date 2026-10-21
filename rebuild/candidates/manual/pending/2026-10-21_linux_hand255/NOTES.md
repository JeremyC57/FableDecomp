# Hand chunk 255 (CThingBuildingDef .. CTraderToEscort): 27 of 40 byte-exact

All 27 sources grade RELOCATION_MATCH under RTM cl 13.10.3077 (gall.sh). All 27 tests pass under g++ (tall.sh) and VC7.1 (tall.py).

## What matched (27)
- **CThingBuildingDef (4)**
  - 0x00453911 CHeroMarriageDef::operator= and 0x0045473F CVillagePeopleDef::operator=. Both are `#pragma optimize("s")` def TUs. The class is modelled with a polymorphic CDefClassBase base and real CCharString members, because the engine headers are flattened pack(1) PDB views.
  - 0x007ADF00 CAIScratchpadDef::operator=. Speed-optimised. The C3DVector member is copied as a 12-byte struct.
  - 0x00454B8E CThingBuildingDef::Transfer. The four flags share ONE bool local, homed in the dead context slot.
- **CThingComponentSet**
  - 0x008707A0 TransferBinaryIn. The inverse of the landed TransferBinaryOut. It inlines the CMemoryDataInputStream Read three times.
  - The fast-path test must be written `size <= m_remaining` (`cmp size,[+14h] ; jg`). `m_remaining >= size` flips it to `cmp [..],size ; jl`.
- **CThingCreatureBase (6)**
  - 0x00661B40 GetHeadPos: an inline graphic accessor and a declared C3DVector copy ctor.
  - 0x00662D80 GetRandomAnimFromName: the sub-def is fetched before the filter, and a has-animation result is discarded.
  - 0x006644F0 SetCurrentAction: counted action copies and an out-param GetInterface.
  - 0x00664680 CreateDefaultWeapons: an inline GetDef() and a const CThing* lookup.
  - 0x00664A50 UnsheatheDefaultWeapon: a local counted copy and an `!(a == b)` compare.
  - 0x006666A0 WorldUpdate: a named world local and `__forceinline` CIntelligentPointer ops.
- **CThingCreatureDef**
  - 0x00670F70 ctor. Every zero goes in the mem-init list. The vptr store sinks only as far as the vector member ctor.
- **CThingDeadCreature**
  - 0x00835E30 SetupDeadAnimation: the counted pointer's inline operator* and a three-float zero frame time.
- **CThingFilter_IsPotentialCustomer**
  - 0x00799E30 operator(). One nested if with a single trailing `return false` gives retail's shared tail.
- **CThingManager (2)**
  - 0x0051E2F0 ActivateThings. Each progress line is an inline helper with a NAMED CCharString local; retail builds the string before the float/bool pushes.
  - 0x00522720 RuntimeLoadEntities: CArray walks, and `free()` for 0x00BFEA14.
- **CThingPhysical**
  - 0x006A4BA0 Save. One `out += prefix + value + ";\r\n"` expression per line.
- **CThingPhysicalSwitch**
  - 0x0083DDC0 TriggerMe. The first PeekDefinition is inlined, with a same-TU noinline GetDef stand-in.
- **CThingPlayerCreature**
  - 0x006A9A00 InitialiseMovementSpeeds. The implicit conversion temporaries share one frame slot.
- **CThingSwitch (4)**
  - 0x0083C690: a C2DBoxF filled member by member, and a C2DVector built Y first.
  - 0x0083C800: `inArea` lives in bl and is written back on both exits.
  - 0x0083CA90: an early `if (Triggered) return`.
  - 0x0083CC10 OnThingWithScriptNameApplied (the catalog says vfunc_71). PeekDefinition returns by value and is inlined; GetDef has a same-TU stand-in.
- **CThingTrackNode**
  - 0x00837CE0 SerialiseGameState. The AddTC names are conversion temporaries in the dead context slot.
- **CThreadedFileManager**
  - 0x0098F3A0 ctor. The STLport tree is modelled as alloc-proxy plus `_M_empty_initialize`, followed by an inline 14-queue array loop.
- **CTortureDoorCourtyard**
  - 0x00DD8BA0 Main (the CQ_PrisonEscapeRescueMotherScript one). It is the landed sibling CTortureDoorPassage::Main 0x00DD8990 with the lock order reversed plus an inverse flag loop, and matched on the first grade.
- **CTraderToEscort**
  - 0x00E23F30 GetRandomSpeech. It follows the landed CAC_Owner::GetRandomSpeech shape with two quest stages.
  - Each stage's pick `break`s out to ONE shared `speech = ...; if (IsDone()) return none; return speech;` tail. Retail cross-jumps the operator= push.
  - Returning from inside each stage costs 57 bytes.

## Parked for Fable 5.1 (13)
- 0051e630 CThingManager::DebugVerifyUniqueIDs DIFFER(369v369): scratch registers rotate (retail: def-name index in ecx, hidden pointer in edx, loop tail reloads this into ecx; ours: eax/ecx/edx) | tried: inline GetDefName (forceinline), named/temporary names, loop forms | next: lane gap: register rotation
- 006381a0 CThingMapManager::AddGameMapWithArea DIFFER(340v348): retail homes push_back's empty __true_type tag (and later the set key copy) in the LIVE `area` argument slot; we use a frame byte (0x24 v 0x20 frame) and pass &map directly | tried: STLport push_back/_M_insert_overflow models, named key copies | next: lane gap: empty tag over a live argument slot (GOTCHAS 24l/24n/24p)
- 00638340 CThingMapManager::RemoveGameMap DIFFER(332v334): find's empty __true_type tag in the LIVE `map` argument slot; game-map record address materialised (`lea ecx,[eax+ecx*8] ; mov eax,[ecx+4]`); refcount decrement split | tried: equal_range/erase forms, record pointer locals | next: lane gap: empty tag over a live argument slot
- 00666d40 CThingCreatureBase::GetHeightZBelowAt DIFFER(346v348): finder placed below the top vector (retail: top 0x18, finder 0x24, bottom 0x30); 1.0f loaded before adding to the CSE'd pos.Z+radius (retail `fld st0 ; fxch ; fadd [1.0]`) | tried: declaration orders, named top/bottom vectors, radius locals | next: frame-slot order of the filter temporary
- 006ac200 CThingPlayerCreature::CreateAndAddObjectToInventory DIFFER(404v398): retail lays the add-directly arm out first with the quick-access block as its fall-through and moves the hero-receive-items (0xcb) arm to the end with a jump back (sharing the 0x1a key slot) | tried: every if/else, else-if, goto and flag form, bitset::test with puts/abort | next: block-order lane gap or an inline helper per arm
- 006ad580 CThingPlayerCreature::OnCollision DIFFER(378v380): the orb out-slot is homed at [esp+0x20]; retail reuses the dead `type` argument slot [esp+0x14]. eax/ecx order around the hero-experience call | tried: plain body (384v380), inline CollectExperienceOrb helper (378v380), orb declared at the top | next: lane gap: out-pointer local homed in a dead argument slot
- 007032f0 CThingObject::OnCreate DIFFER(383v380): we keep the literal's address (the CCharString ctor's eax) in edi for the SameText compare and push ebp; retail re-reads the data word from the frame | tried: forceinline operator==, same-TU LowerBound stand-in, named/temporary literal | next: compare through a CCharString temporary with a member operator==
- 00760c40 NShape::CTrack::GetTrackPosAtLengthFraction DIFFER(343v342): frame 0x38 v 0x30 (retail overlaps the found arm's delta*remaining temps with the dead first-point Z slot); the first-point copy goes through esi instead of eax; the reciprocal is kept in st instead of being stored to the dead `fraction` slot | tried: C3DVector first copy, float x/y/z locals (fixed the return copy), post-increment iterator (fixed the loop), `<` with else-return (fixed `test ah,5 ; jp`), reciprocal written into the operator/ argument | next: lane gap: x87 slot packing
- 0083d2e0 CThingMarker::DrawPreparePrimitivesForRendering DIFFER(356v372): before the two interpolated-transform calls retail pre-evaluates the size fld, both PhysicsTC loads and the 64-bit UniqueID (ebx plus a spilled high word, a 4th callee-saved register), and it sinks the theme-desc zero stores next to the slot-71 call | tried: temporary vs named mesh, size/id/physics locals | next: a CEngineMesh-info constructor that takes the id
- 00876ca0 NCameraModes::CTools::PotentialFieldBufferObjects DIFFER(362v368): retail FPU-enregisters the copied object position's X/Y (`fld x ; fld y`, Z int-copied) and the step*offset temp's X/Y, and spills dx instead (frame 0x44 v 0x38); our copy is three int moves | tried: direct `sample - GetPosition()`, named position copy, by-value operator- argument | next: lane gap: struct-member FPU enregistration (scalar replacement)
- 0090c900 CThingFilter_IsValidGossipee::operator() DIFFER(345v345): only the esi/edi assignment of the gossipee and the map/home-building temporaries is swapped | tried: local orders, inline accessors | next: lane gap: register swap
- 00986ed0 CThingFilter_IsAICreatureMatchingTypeAndFactionNearby ctor DIFFER(353v353): x87 operand order of three of the six cross-product multiplies (VC7.1 canonicalises commutative fmul operands) | tried: free CrossProduct, member Cross, swapped factors, reference locals for Up/Forward | next: lane gap: commutative fmul operand order
- 00987040 CThingFilter_IsAICreatureMatchingTypeAndFactionNearby::operator() DIFFER(370v374): x87 schedule of the scaled-component dot (retail scales x,y,z first and pops r last) and the order of the true/false return blocks | tried: in-place Normalise plus Dot (both orders), GetNormalised, by-value Dot, explicit locals, if/||/bool-local returns | next: a fast-normalise helper returning the scale factor

## Skipped
- None. No row contains a second function body.

## Label corrections
- 0x0083D2E0: the catalog says "CThingMarker::vfunc_36". It is CThingMarker::DrawPreparePrimitivesForRendering.
- 0x006AD580: the catalog says "CThingPlayerCreature::vfunc_41". It is CThingPlayerCreature::OnCollision.
- 0x0083CC10: the catalog says "CThingSwitch::vfunc_71". It is CThingSwitch::OnThingWithScriptNameApplied (Ego_r.pdb mangled name).
- 0x00453911 / 0x0045473F / 0x007ADF00: the catalog says "CThingBuildingDef::operator=" (propagated). They are CHeroMarriageDef / CVillagePeopleDef / CAIScratchpadDef::operator= (each is RTTI vtable slot 19 Copy).
- 0x00431F10: the catalog says "CThingBuildingDef::operator=". It is CDefClassBase::operator= (the root def copy).
- 0x00454886: the manifest says "CActiveFile::operator=". It is vector<long>::operator=.
- 0x004C7BC0: the manifest says "_Cons_val<...>". It is a CThing forwarder to the thing manager's +0x9a flag.
- 0x0051E2E0: the manifest says "GetCanItemBeQuickAssigned". It is a CThingManager flag getter (+0x9a, PDB SerialisingGameState).
- 0x0083D190: the manifest says "CThingVillage::...". It is CThingMarker::GetCurrentGraphic.
- 0x00449700: the manifest says "CWorld::DrawGetEnvironment". It is the CPlayerManager main-player-number getter.
- 0x00993B10: the manifest says "~CChunkedFileChunk". It is ~CMemoryDataInputStream.
- 0x0049D150: the manifest says "GetFacingVector". It is the physics interpolated-position getter.
- 0x004C7A00: GetPlayerManager. 0x013B8A1C is the CThingManager global.
- 0x004C73D0: the manifest says "CVertexBufferWin32::DoSizeof". It is CThing::GetPosition() const.
- 0x00F35B30: the manifest says "CCreatureAction_TrollWhackGroundBase::Initialise". It is the entity script's IsActiveThreadTerminating / IsDone, as the landed scripts already say.
- 0x00870370: the manifest says "CSmallVector<CANavQuadTreeNode const*,8>::resize". It is the CThingComponentSet::CEntry instantiation, as landed 0x00870480 already says.
- 0x0083CFA0: the manifest says "GetDef<CThingPhysicalSwitchDef>". It is GetDef<CThingSwitchDef>.
- 0x00876E10: the manifest says "Vector_ComputePotentialGradient_00876e10". It is NCameraModes::CTools::PotentialFieldBufferPoints. It walks the same 0x01385D20 step and 0x01385D24 offset table.
- 0x0090C900: the parameter is `const CThingAICreature&`.
- 0x00DD8BA0: Ego_r.pdb has two CTortureDoorCourtyard classes. This one is CQ_PrisonEscapeRescueMotherScript's, because the parent flag is at +0x67. CQ_SecretPassageScript's is the landed 0x00DEEFC0.
