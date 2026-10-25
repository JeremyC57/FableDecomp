# Hand chunk 478 (NShape .. NThingShot::CTCShotDefinitionThrow): 23 of 40 byte-exact

All 23 src/ files grade RELOCATION_MATCH through gall.sh; all 23 tests PASS under tall.sh (g++) and tall.py (VC7.1).
calls.py: every call target matches retail (operator delete 0x00BFE9BC and malloc/free 0x00BFEA0E/0x00BFEA14 where
retail calls them).

## What matched (23)

- **NShape** (1)
  - 00760760 `NShape::GetFirstPointThatBreaksConvexity(std::vector<C3DVector>&)` (__fastcall): walks the point triples
    from the second point and returns the middle point of the first triple that breaks convexity, else end().
    Shape lessons: name the six vectors in source order (`p0, p1, p2, toFirst, inEdge, outEdge`) so the x87 temps
    come out in retail's order, because VC evaluates call arguments right to left. Reuse one iterator
    (`it = begin(); first = *it; for (++it; ...)`); a separate begin local swaps two registers.
- **std (STLport 4.x algorithms)** (1)
  - 007649d0 `std::__find_if<const CCountedPointer<NShape::CShape>*, NShape::CIsShapeTrack>` (random-access form):
    finds the first shape whose GetType() is 1. Copied verbatim from STLport: a trip count of `>> 2`, four tests per
    trip, then a falling-through switch. The by-value CCountedPointer predicate argument has an inline add-ref and
    release.
- **std::vector instantiations** (15). They follow the landed 0x00569E30 `_M_insert_overflow` recipe:
  - The tag-taking copy/fill helpers are same-TU `__declspec(noinline)` bodies that never read their tags. This keeps
    each `__false_type()` homed on the dead `atend` slot.
  - Element copy ctors are `__forceinline` where retail inlines them.
  - `len = size + (max)(size, n)` goes through references.
  - `_M_clear()` is the inline destroy loop plus `free`.
  - The rows:
    - `_M_insert_overflow`: 00765b70 (vector<C3DVector>), 0076b850 (ParticleAndEndPoints), 00770810
      (list<CWeaponTrailEntry>), 00770bb0 (vector<pair<C3DVector,C3DVector> >), 00777fe0 (CTCShop::GoodsPlace),
      0077ef30 (pair<CCharString,C3DVector>), 00781a80 (NFire::CFirePoint), 00787380 (CTCSkeletalMorph::CSkeletalMorph),
      007b6ef0 (CIntelligentPointer<CThingMarker>, virtual dtor loop).
    - `_M_fill_insert`: 0076b950 (CDefPointer<const CThingBaseDef>) and 00770990 (list<CWeaponTrailEntry>). Both are
      vector.c verbatim, with x_copy living in the dead `x` slot. VC's inline budget reproduces retail's mix: inline
      `__copy_backward_ptrs`, out-of-line `uninitialized_fill_n`.
    - `operator=`: 0076b1e0 (ParticleAndEndPoints), 0077b860 (CEngineTextureDecal), 00778c00 (const CThing*).
    - `insert(pos, x)`: 007658b0 (CCountedPointer<NShape::CShape>). This is STLport's in-place arm with `T x_copy = x`
      and `__copy_backward_ptrs`.
- **CTCObjectAugmentations** (4)
  - 00766550 `ClearAllSounds()`: stops every played looping sound through the owner's sound player, then clears both
    sound maps. The map iterator `++` is the static `_Rb_global<bool>::_M_increment` (inlined, walk in eax), and
    `_Rb_tree::clear()` is inline.
  - 00766c00 `SetupAugmentationSounds()` (private): rebuilds the init and looping sound tables from the def unless the
    owner is in limbo. Shape lesson: a named CCharString local gives retail's temp slot order.
  - 007676a0 `~CTCObjectAugmentations()`: `ClearAllParticleEffects()` plus the compiler's member teardown through the
    STLport destructors. CDefPointer releases through the pointee's inline ReleaseReference.
  - 00768210 `CopyAugmentationDataFromThing(const CThing&)`: clear() = erase(begin, end), then reserve, then
    `copy(.., back_inserter(..))`. The callee SetupAugmentationSounds is declared `private:` (Ego_r.pdb `AAE`).
- **CTCGuard** (2)
  - 007b2e20 `ReportAllPendingCrimes(CTCVillage*)`: reports each pending crime to the village and informs the village's
    opinion of the hero of each deed it does not know yet. When the hero's deed log rates any crime severe (2 or 3),
    it re-runs the village security sweep. Then it clears the deque.
    - Shape: STLport deque iterator with a member `_M_increment`, and the by-value crime copy.
    - The `CDefPointer<COpinionDeedEffectsDef>` temp releases with `ReleaseTemporary()`.
  - 007b3760 `OnSerialise(CPersistContext&, bool)`: the pending-crime deque goes through a temporary vector (copied
    out on save, copied back on load), then six member transfers. Shape lessons:
    - `_S_buffer_size()` is STLport `_MAX_BYTES` 128, so a node is 0x80 bytes (16 crimes).
    - The two vector teardowns cross-jump into one free.

## Parked for Fable 5.1 (17)

- 00760190 NShape::IsTriangleWithPointsInside DIFFER(324v340): integer register allocation only. Retail keeps the copied vertex in edx/esi/edi, the points iterator in ebp, and the counter and triangle.begin() in frame slots. Ours keeps the counter in edx and begin() in edi, and spills the vertex. The x87 part already matches | tried: literal vs extern-const epsilon (extern gives the hoisted `fld [g]; fchs`), `normal = normal * -1` vs `normal *= -1` | next: named locals for the three triangle vertices, and a `for` over a const_iterator local declared before the counter
- 0076e4d0 CTCWeaponTrail::GetLastWeaponTrailPoints DIFFER(253v253): scheduling only. push_back's in-place copy of the 24-byte pair temp: retail pipelines it (two loads ahead, rotating ecx/edx), ours copies member by member | tried: STLport `min` vs a ternary, a true-path-first return, a C3DVector user copy ctor and computing n before the iterator, a same-TU noinline `_M_insert_overflow` stand-in | next: a pair copy ctor without the C3DVector user copy ctor (implicit memberwise), else lane gap: copy scheduling
- 00774610 CTCShop::GetStockList DIFFER(348v282): retail rounds each quantity through Lionhead's inline-asm ceil (`fld; fadd 0.5; fistp; fild; fsub 1.0; fstp`, then a bit compare and `dec`) with an ebp frame. differ/ holds the C++ reference body | tried: one C++ reference grade | next: lane gap: retail_asm (a minimal inline-asm ceil helper under the retail_asm exception)
- 00793370 std::vector::_M_insert_overflow (CTCDivineWrath::PerEffectInfo) DIFFER(300v300): esi/edi swap only. Retail keeps `this` in edi and new_finish in esi | tried: implicit vs __forceinline record copy, the STLport __destroy/__destroy_aux chain, declared-only helpers, a base-less pointer copy-ctor init list, QFE-4035 | next: lane gap: register swap (compiler build)
- 007981b0 std::vector::_M_insert_overflow (CTCUnholyPower::PerEffectInfo) DIFFER(300v300): esi/edi swap only (byte twin of 00793370) | tried: the 00793370 variants, QFE-4035, a virtual ~CBaseIntelligentPointer | next: lane gap: register swap (land with 00793370)
- 0079aa90 CTCSummonSpell::GetValidTeleportPos DIFFER(557v557): x87 slot allocation only. Retail stores dir.Z with `fst` and squares it in place (ours `fstp` + `fld`), and keeps the growing ring radius in the dead `target` slot with the fixed distance in a frame slot (ours swapped), which moves the loop-alignment `jmp` | tried: GetLength via `Dot(*this)` (fixed the z,y,x order), `*=` then Normalise() (fixed the temp copy), radius/distance declaration order | next: write the normalise by hand (`len = GetLength(); dir *= 1/len`) with `distance` declared before `radius`
- 007aa880 CTCAssassinRush::IsValidTeleportPos DIFFER(350v346): register allocation only. Retail keeps `to` in ebx for the whole body, shares ebp between `this` and the list iterator, and re-reads the map vptr. Ours caches the map vptr in ebx, spills `this` and hoists `physics` | tried: same-TU noinline `GetAllThingsInRoughArea` and `~list` stand-ins (removed the dead header zero store and the reload) | next: call the map virtuals through a `const CWorldMap*` local, else lane gap: register allocation
- 007afe20 std::vector::_M_fill_insert (CIntelligentPointer<CThingAICreature>) DIFFER(302v302): ebx/ebp swap only. Retail keeps `pos` in ebp and elems_after (then n*8) in ebx | tried: STLport iterator-tag chain vs a plain tag with a __forceinline `__copy_backward_ptrs`, QFE-4035, a same-TU noinline `_M_insert_overflow` | next: lane gap: register swap
- 007b6b10 CTCCrateStack::FillStacks DIFFER(343v333): retail keeps the physics component pointer in ebx, seeded from the stale frame word that also homes `stack`. Ours spills physics to its own frame word (extra slot, a reload on the not-found path) and splits the seed pointer and the random bit over edi/esi | tried: both component pointers declared uninitialised before the loop, physics outside / stack inside the loop | next: one GetInterface template call site shared by both pointers (an out-param helper), else lane gap: register allocation
- 007b9b40 std::vector::_M_fill_insert (CIntelligentPointer<CThingObject>) DIFFER(302v302): ebx/ebp swap only (byte twin of 007afe20) | tried: the 007afe20 variants, no element destructor, /Gr, /Ob1, /Os | next: lane gap: register swap (land with 007afe20)
- 007bbc9f NThingShot::CTCShotDefinitionThrow::InitMovement DIFFER(299v299): x87 stack/frame allocation. Ours keeps the second sin(angle) copy on the stack but also stores it (frame 0x38 v 0x34, every local shifted by 8) and keeps cos before sin. Retail's velocity products pick the other operand order for Y/Z | tried: `#pragma optimize("s", on)` with `__forceinline` Normalise, a physics local (esi now matches), `Square((float)s)` vs `s * s` vs a float sin local | next: `float s = (float)sin(A)` computed after cos, with the velocity built as `C3DVector(dir.X * s, dir.Y * s, c) * speed`
- 00761280 NShape::DrawPrimitiveLineOverLandscape DIFFER(-v733): not attempted (large x87 body; budget spent on the tractable rows) | tried: none | next: author from dis_all.txt with the C3DVector inline operators of 00760760/0079aa90; expect x87 slot gaps
- 00767da0 CTCObjectAugmentations::UpdateParticleAttachment DIFFER(-v622): not attempted | tried: none | next: reuse the CTCObjectAugmentations models of 00766c00/00768210; the by-value CIntelligentPointer<CThing> argument needs a user copy ctor
- 007748e0 CTCShop::AddSaleItemPlacesFromThing (likely) DIFFER(-v596): not attempted (recursive, "GoodsPlace"/"GOODS_PLACE_" string matching) | tried: none | next: confirm the name against Ego_r.pdb CTCShop; reuse 00777fe0's GoodsPlace vector model
- 00779ef0 CTCDExperienceOrb::OnInitialActivate DIFFER(-v567): not attempted. It is a retail_asm candidate: Lionhead's inline-asm ceil (bare `fistp dword [ebp-4]`) with an ebp frame. It calls the CTCBase base 0x00686870 | tried: none | next: one C++ reference grade, then lane gap: retail_asm
- 0077a890 CTCDExperienceOrb::Create DIFFER(-v707): not attempted. Static __fastcall(CThing&): orb count = `(long)(experience * multiplier)` via `__ftol`, then for each orb a `vector<CThing*>` push_back (inline, overflow 0x00436AC0) and a RotateXY launch | tried: none | next: author it; models for the intelligent pointer, the GetInterface one-ternary and RotateXY 0x00A14040 exist in this chunk
- 007808b0 CTCFlammable::InitFirePoints (likely) DIFFER(-v923): not attempted. It is the tail jump from Initialise 0x00780C50, with an `and esp,-8` aligned frame (double locals) | tried: none | next: confirm the name and look for a retail_asm/aligned-frame lane gap before authoring

## Skipped

None. No row holds two functions.

## Label corrections

Catalog row labels (propagated or descriptive) against the identities found:
- 007649d0 "RBTree_Find_Comparator" -> `std::__find_if<const CCountedPointer<NShape::CShape>*, NShape::CIsShapeTrack>`
- 007658b0 "Vector_Insert_CCountedPointer_MoveAndRealloc" -> `vector<CCountedPointer<NShape::CShape> >::insert(pos, x)`
- 00765b70 "vector_insert_range_CFeatDef" -> `vector<vector<C3DVector> >::_M_insert_overflow` (the element is a 12-byte vector, not CFeatDef)
- 00766550 "CTCObjectAugmentations_UpdateTreeStructure" -> `CTCObjectAugmentations::ClearAllSounds`
- 00766c00 "CTCObjectAugmentations_UpdateParticleMorphs" -> `CTCObjectAugmentations::SetupAugmentationSounds` (private)
- 00767da0 "CTCLinkDisplay_SetupParticleNodes" -> `CTCObjectAugmentations::UpdateParticleAttachment(CIntelligentPointer<CThing>, const CCharString&, const CCharString&)`
- 00768210 "CTCObjectAugmentations_InitAbilitySlot" -> `CTCObjectAugmentations::CopyAugmentationDataFromThing`
- 0076b1e0 "Vector_Assign_ReserveCapacity" -> `vector<ParticleAndEndPoints>::operator=`
- 0076b850 "Vector_Insert_CBrainUpdateZone_MoveAndRealloc" -> `vector<ParticleAndEndPoints>::_M_insert_overflow`
- 0076b950 "Vector_Resize_IVCountedPointer" -> `vector<CDefPointer<const CThingBaseDef> >::_M_fill_insert`
- 0076e4d0 "WeaponTrail_CollectPoints" -> `CTCWeaponTrail::GetLastWeaponTrailPoints` (the private overload). The global 0x013828A0 is a descriptive name (g_WeaponTrailInterpolationSteps)
- 00770810 "Vector_Insert_LinkedList_MoveAndRealloc" -> `vector<list<CWeaponTrailEntry> >::_M_insert_overflow`
- 00770990 "Vector_Insert_LinkedList_Conditional" -> `vector<list<CWeaponTrailEntry> >::_M_fill_insert`
- 00770bb0 "Vector_Assign_Uninitialized_uint6Array" -> `vector<vector<pair<C3DVector,C3DVector> > >::_M_insert_overflow`
- 00774610 "CTCShop_RebuildCacheMap" -> `CTCShop::GetStockList() const`
- 007748e0 "CTCGraphicAppearance_InitGoodsPlaces" -> likely `CTCShop::AddSaleItemPlacesFromThing(const CThing*)`
- 00777fe0 "Vector_InsertRange_Fill_CKeyCamera" -> `vector<CTCShop::GoodsPlace>::_M_insert_overflow`
- 00778c00 "Vector_Assign" -> `vector<const CThing*>::operator=`
- 00779ef0 "CTCObjectView_InitializeWillPowerDisplay" -> `CTCDExperienceOrb::OnInitialActivate` (vtable 0x0126ABFC slot 6)
- 0077a890 "Create" -> `CTCDExperienceOrb::Create(CThing&)` (static __fastcall)
- 0077b860 "Vector_Assign_CBodyPartMesh" -> `vector<CEngineTextureDecal>::operator=`
- 0077ef30 "Vector_Assign_TargetPoint" -> `vector<pair<CCharString,C3DVector> >::_M_insert_overflow`
- 007808b0 "CTCAvoidRegion_Initialize" -> likely `CTCFlammable::InitFirePoints`
- 00781a80 "Vector_InsertRange_Copy" -> `vector<NFire::CFirePoint>::_M_insert_overflow`
- 00787380 "StdVector_ReallocateWithCopy" -> `vector<CTCSkeletalMorph::CSkeletalMorph>::_M_insert_overflow`
- 00793370 "Vector_InsertRange_CBaseIntelligentPointer" -> `vector<CTCDivineWrath::PerEffectInfo>::_M_insert_overflow`
- 007981b0 "StdVector_ExpandAndCopy_CBaseIntelligentPointer" -> `vector<CTCUnholyPower::PerEffectInfo>::_M_insert_overflow`
- 0079aa90 "CTCSummonSpell_GetValidSummonPos" -> `CTCSummonSpell::GetValidTeleportPos`
- 007aa880 "CTCVillage_FindValidTeleportPos" -> `CTCAssassinRush::IsValidTeleportPos`
- 007afe20 "Vector_InsertRange_CBaseIntelligentPointer" -> `vector<CIntelligentPointer<CThingAICreature> >::_M_fill_insert`
- 007b2e20 "CTCVillage_ProcessDeedReports" -> `CTCGuard::ReportAllPendingCrimes`
- 007b3760 "Save_TransferCriminalRecord" -> `CTCGuard::OnSerialise`
- 007b6ef0 "StdVector_AllocateAndCopy" -> `vector<CIntelligentPointer<CThingMarker> >::_M_insert_overflow`
- 007b9b40 "vector_Insert_Pair_SmartPtr" -> `vector<CIntelligentPointer<CThingObject> >::_M_fill_insert`
- 007bbc9f "CTCPhysicsRigidBody_ApplyForce" -> `NThingShot::CTCShotDefinitionThrow::InitMovement` (vtable 0x0123CDC4 slot 33)
- Right as labelled: 00760190, 00760760, 00761280 (NShape), 007676a0 (~CTCObjectAugmentations), 007b6b10 (CTCCrateStack::FillStacks).

Callee labels found wrong:
- 0x0066ADD0 "CGameScriptThing::SetToKillOnLevelUnload" -> `CTCSoundPlayer::StopSample(unsigned long)`
- 0x00769650 "LTextTreeWalkThrough::BuildTreeArray" -> `_Rb_tree::_M_erase`
- 0x006C5630 "GetFloorMaterial" -> `CTCHeroOpinionDeedLog::GetDeedEffectsDef`
- 0x007B4B00 "CharacterBuffer_Reset" -> `deque<CCrimeWaitingForBribe>::clear`
- 0x004AB490 "SortTreeRecursively" -> `list<CThing*>::~list`
- 0x00A14040 "Math_CartesianToSpherical" -> `C3DVector::RotateXY(float)`
- 0x0066F9B0 "SetFacingAnglesFromRHSet" -> a CTCPhysicsRigidBody velocity setter (descriptive name)
- 0x007BEDD0 "CTCPhysicsRigidBody::GetFacingVector" -> a CTCShotDefinitionBase target-position getter (descriptive name)
- 0x00686D90 -> `CTCBase::GetThingSearchTools`; 0x0079BF30 -> GetAllThingsInRoughArea into a `list<CThing*>` (descriptive name)
- 0x00686830 "~CChunkedFileChunk" -> `CTCBase::~CTCBase`

Descriptive names with no PDB name: g_WeaponTrailInterpolationSteps (0x013828A0), the velocity setter 0x0066F9B0, the target getter 0x007BEDD0, CWorld vtable slot 13 GetWorldMap, CTCPhysicsRigidBody slot 38 Activate, and the "likely" row names 007748e0 / 007808b0 until checked.
