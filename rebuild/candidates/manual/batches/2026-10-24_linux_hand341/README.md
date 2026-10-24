# Hand chunk 341 (NCreatureMode::CModeStunned .. NParticleEngine::CPSCUpdateNormal): 39 of 40 byte-exact

## What matched (39)
- **NCreatureMode** (6)
  - 008d9060 CModeStunned::OnDeactivate: an inline bitset-test GetInterface for type 9 calls ReturnToDefaultTargetingFlags, then CModeOutOfActionBase::OnDeactivate.
  - 008de650 CModeStunned::Recover (slot 15): builds a CCombatAction_RecoverFromStunned temporary over the base ctor 0x867AF0 (owner, false), as in the landed CModeCreatureIsProjectile::Recover.
  - 008dfef0 CModeSummonerDie::FrameUpdate: `if (--frames <= 0)`, then the SummonerDead action, `CCombatActionBase(owner, 250, CSoundPair())`.
  - 008d9410 CModeUnderground::FrameUpdate: the const out-param `GetConstInterface(&link, 0x111)` over the const LowerBound 0x4365B0 gives retail's `lea esi,[eax+0Ch]`.
  - 008d75d0 CModeWading::FrameUpdate: SetGroup with an implicit CCharString temporary in each arm.
  - 008ea9b0 CModeHoverBase::OnDeactivate: an inline base `Active = false`, then `IS_THING_ALIVE` and Kill(true). This is a label correction.
- **NDisplayView** (2)
  - 0069edc0 CView3DBase::SetVWindow: a 16-byte box copy.
  - 0069e8f0 CView3DThirdPerson::GetCamera: casts `GetDisplayEngine()`'s void* to CCameraSource and calls slot 5, passing the hidden return slot straight through.
- **NGameText::CDataBank** (4)
  - 009c79e0 GetEnumerationType: a two-test if-chain of `return CCharString(lit, -1)`. VC hoists the shared `push -1` above the first branch.
  - 009c82c0 NotifyBankSize: the landed CMeshDataBank::NotifyBankSize shape. Everything sits under `if (ReadOnly)`, `resize(n, 0)` with the zero in the frame, and a strength-reduced {type, count} walk. It matched on the first try.
  - 009c85a0 the ctor: CBankFileAsync + CResourceBank bases. The arrays are zeroed by inline CArray ctors but the narrator map word by a body statement. That split is what puts `xor edi,edi` between the two vtable stores.
  - 009c8660 CloseTextAndGroups: the two entry buffers use an inline Dinkumware `clear() { erase(First, Last); }` (`_Move(last, Last, first, tag, false)`, `_Destroy`). The pointer table uses an inlined scalar move with its member operands read in place, which keeps `cmp ecx,ecx ; je`. This is a label correction.
- **NGraphicAppearance** (4)
  - 004c01e0 CTCGraphicAppearance::HandleMessage: dispatch on two virtual GetType calls. Type 1 inlines `SetAsChanged(8)` with the constant folded; type 4 calls the landed CatchMessage 0x4BFD10.
  - 007e1570 / 007a6740 CTCGraphicAppearanceMultipleStaticMeshes / ReplaceableMesh::OnCreate: `if (ParentThing->PDef) PDef->GetSubDef(PDef)`.
  - 0077c740 CTCGraphicAppearanceSprite::FrameUpdate: source statement order fixes the register pairing.
- **"NHeroInformationScreens::CBase"** (2, both are label corrections)
  - 00a373b0 CClassFactory::CClassFactory(unsigned char): the STLport `_Vector_base` + `_List_base` models; NoAllocatedClasses is cleared in the body before `resize(MaxClassTypeID)`. It matched on the first try.
  - 00b94a80 CEngineScene::CEngineScene(float, ulong, float, float, const CCharString&): two STLport rb-trees (the CShaderRenderManager model) and body scalar stores. The source order is load-bearing: MaxAveragePrimitivesPerCell goes after the occlusion defaults.
- **"NHitLocations"** (1, a label correction)
  - 00cac400 CAudioThreadOwner_00cab870::AttachAndWait: CreateEventW, Attach (0xCAB870), WaitForSingleObject(INFINITE), then a `switch` with a duplicated CloseHandle + `hr >= 0 ? 0 : hr` tail. Bounds are correct.
- **NHitLocations::CPhysicalPrimitive*** (7)
  - Mesh / Sphere / Cylinder Alloc: `return new T;` (a CDefString(-1) member for sphere and cylinder).
  - 008156a0 Cylinder::TransferBinaryIn: the landed sphere model, with one shared float local serving both reads.
  - 00814bc0 Mesh::GetClosestPointToPos2D: `return pos;` with a memberwise C3DVector copy ctor.
  - 00815200 Mesh::GetBoundingSphere: `thing.PhysicsTC->GetBoundingInfo(&c, &r)`.
  - 00814d90 Sphere::GetFirstIntersectionWithTriangle: the landed FiniteLine twin 0x814D10, used verbatim.
- **NParticleEngine** (13)
  - 00415cf0 CAEnvironmentInteraction::GetVectorParam: `return C3DVector(0,0,0)`.
  - CPSCLight:
    - 00b187d0 GetComponentCost: a max-radius ternary into a named float local, scaled through a const_cast system call.
    - 00b19c20 Kill: `PrimHandle.Clear()` with the landed handle model.
  - 00b163a0 CPSCOrbit::DatabaseCopy.
  - CPSCSpline:
    - 00b1af50 InterpolateTo.
    - 00b1afc0 DatabaseCopy.
    - 00b1b0c0 StartUpdate: SplinePosParamData must be the counted pointer with an inline `operator->`.
    - 00b1b2e0 DatabaseSerialise.
  - CPSCUpdateNormal:
    - 00b0a6d0 GetUpdateParam: `X = Y = Z = 0` stores Z first.
    - 00b0a710 Initialise.
    - 00b0c830 / 00b0c8a0 GetParticleCount / GetDecalCount: inline bool bit-field getters give `shr al,5 ; test al,1`.
    - 00b0c910 PreparePrimitives: the list loop with an inline STLport `erase(iterator)`.

## Parked for Fable 5.1 (1)
- 004bfc50 NGraphicAppearance::CTCGraphicAppearance::OnCreate DIFFER(144v144): `mov ecx,[eax+4] ; mov [ebp+24h],ecx` (the GetInterface component load) where retail uses eax; everything else matches | tried: GetInterface into the member out pointer, ternary vs if-assign GetInterface, a local out pointer then assign (fixes eax but rotates the later edx/al/ecx), HasTC + GetTC, a same-TU noinline GetSubDef stand-in | next: an inline GetInterface returning the component (not through an out pointer) assigned straight to PTCAnimation, or lane gap: register choice for a value stored to a member.

## Skipped
- none. No row holds two functions; every row's bounds match its body plus int3 padding (00cac400 was checked specifically: 149 bytes, three duplicated return tails, then int3).

## Label corrections
- 008ea9b0 `NCreatureMode::CModeWalkInAllDirections::vfunc_6` -> `NCreatureMode::CModeHoverBase::OnDeactivate`. FableWin.pdb has it on CModeHoverBase; CModeWalkInAllDirections inherits slot 6.
- 009c8660 `NGameText::CDataBank::CommitChanges` (labels_forge_autoname2) -> `NGameText::CDataBank::CloseTextAndGroups`. PDB `?CloseTextAndGroups@CDataBank@NGameText@@IAEXXZ`. CDataBank::Close (slot 5, 0x9C8BF0) is `CloseTextAndGroups(); CBankFile::Close();`; the other caller is 0x9C87DA.
- 009c79e0 `vfunc_19` -> `CDataBank::GetEnumerationType(unsigned long) const`. 009c82c0 `vfunc_10` -> `CDataBank::NotifyBankSize(unsigned long, const CArray<CBankFileDataTypeCount>&)`.
- 00a373b0 `NHeroInformationScreens::CBase::CBase` -> `CClassFactory::CClassFactory(unsigned char)`. It stores vtable 0x129CF70 = RTTI CClassFactory, and the PDB `??0CClassFactory@@IAE@E@Z` takes the movzx'd byte. The label was propagated from the CBaseClassNonCopyable ctor 0x99A2F0, which the manifest names "CBase".
- 00b94a80 `NHeroInformationScreens::CBase::Init` -> `CEngineScene::CEngineScene(float, unsigned long, float, float, const CCharString&)`. It stores vtable 0x12A3E68 = RTTI CEngineScene; PDB `??0CEngineScene@@QAE@MKMMABVCCharString@@@Z`; `ret 14h`.
- 00cac400 `NHitLocations::CPhysicalPrimitiveCylinder_TransferBinaryOutAsync` (labels_forge_autoname_t5) -> `CAudioThreadOwner_00cab870::AttachAndWait` (descriptive). It calls the landed CAudioThreadOwner_00cab870::Attach 0xCAB870 on its own `this`; the callee's own manifest label "NHitLocations::CPhysicalPrimitiveCylinder::TransferBinaryOutAsync" (labels_forge_autoname3) is wrong too.
- Callees:
  - 0x9FC520 (manifest `CInputProcessGameBase::AddToInterface`) is also `CResourceList::SetMaximumMemory(unsigned long)` (a byte-identical fold).
  - 0xA38150 (`CTCInventoryMap_InitMiniMapMarkers`) is `vector<CClassTypeInfo>::resize(unsigned long)` on CClassFactory+4.
  - 0x9CA660 / 0x9CA6C0 / 0x9CABC0 / 0x9CAC00 are the CDataBank entry buffers' `_Move` / `_Destroy`. The manifest's CMeleeCombatAbilityDef / GroupEntry element names are folds.
  - 0x00B187D0 is CPSCLight::GetComponentCost (slot 20).
