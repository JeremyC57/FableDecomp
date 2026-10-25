# Hand chunk 339 (Mass .. NCreatureAI): 37 of 40 byte-exact

All 37 matched rows grade MATCH or RELOCATION_MATCH (`grades.txt`). Their behaviour tests pass under both g++ (`tall.sh`) and VC7.1 under Wine (`tall.py`). Every `vfunc_N` catalog row was named from its vtable slot (`rtti-vtables.tsv`) and the FableWin.pdb mangled names. Retail camera-mode offsets are the PDB offsets less 0x24, because retail NCameraModes::CBase is 0xa4 bytes.

## What matched (37)
- **NCameraModes::CCameraPointModeBase** 0056e2b0 `GetLookDirection() const`. Returns C3DVector(1,0,0) through the hidden result pointer, using a value constructor.
- **NCameraModes::CSnapToPoint**
  - 008506e0 `UpdateDesiredPos(C3DVector&)` is a plain `desired = Pos`.
  - 00850700 `OnInitLeaf`:
    - The camera point's CTCCameraPointDefinitionBase is fetched with an inlined const `PeekTC`, which uses the const LowerBound 0x004365B0 and leaves the out pointer uninitialised.
    - It is then checked with an inlined `GetPointType<CTCCameraPointFixedPoint>` (slot 28).
    - It copies TrackThing, then LookVector when the point does not track a thing.
    - The point class and the mode both need `#pragma pack(1)`: TrackThing is at +0x40 and LookVector at +0x41; in the mode, TrackThing is at +0xc0 and LookDirection at +0xc1.
- **NCameraModes::CStringAndCageBase** 0069ddc0 `SetSACPosition` is `GoTo.Position = pos` (GoTo is the CPhysicsState at +0x54).
- **NCameraModes::CStringAndCage / CSACCombat**
  - 00871e40 and 00871e60 `SetSACPosition` call `AddAttractorGoTo(pos, 1.0f)` (0x0084E1A0).
  - 00873250 and 008732d0 `SetUpPhysics` are `AddSubComponent(CCountedPointer<CSubComponentBase>(new CRigidPhysicsLeaf(*this, *CameraManager)), false)`. Shape: the CCountedPointer `T*` constructor is inline, `if (Data) Info = new CCPPointerInfo(DeleteData, Data); else Info = 0;`.
- **Init::Alloc (slot 0)** for CStringAndCageInit 00873160, CSACCombatInit 008731b0, CZTargetNewInit 00873200, CFirstPersonInit 008738f0 and CPCMouseLookInit 00874770.
  - Each is `return new T(manager, def[, def->StringLengthXY, def->CageRadius, def->HeightOffsetZ, false, 0.0f, def->FOV * (1/360)])`.
  - CCameraModeDef retail offsets are the PDB offsets less 4.
  - CZTargetNew and CFirstPerson have inline constructors over the out-of-line CBase constructor 0x0084E420. CZTargetNew's inline CIntelligentPointer member constructor stores its own vtable.
- **NCameraModes::CScriptModuleBase**
  - 00851dd0 `GetModuleFOV(float&)`: `if (FOVOverride > 0) { fov = FOVOverride; return true; } return false;`.
  - 00853fb0 `ResetCameraOnEnd`:
    - If CutOutOverride is not -1, it returns `CutOutOverride == 1`.
    - Otherwise, when the camera point is alive and its point definition has CutOutOf set, it returns true.
    - Shape: the shared header already declares the virtual, so do not redeclare it.
- **NCameraModes::CScriptModuleTransitionBetweenPoints**
  - 00852040 `OnInitLeaf` calls the inline owner setters SetPosition, SetObservedPos and SetFOV.
  - 00854340 `GetModuleFOV` is `fov = FOV; return true;`.
- **NCameraModes::CScriptModuleFOVZoom** 008520c0 `OnInitLeaf`: `FOVIncrement = (FOVFraction - 1) * POwner->GetFOV() / WorldFrames; POwner->SetPosition(StartPos);`.
- **NCameraModes::CScriptModuleFullTransition** 008521e0 `OnInitLeaf` calls SetPosition(StartPos), then SetObservedPos(LookAtPos).
- **NCameraModes::CScriptModuleSpline** 008540f0 `OnInitLeaf`:
  - An inline non-const `GetTC` (LowerBound 0x0040F020) finds the spline.
  - It calls InitialiseSpline, then sets `Index = (float)start / (NumKeyCameras() - 1)`.
  - It saves Index around the virtual Update() call.
- **NCameraModes::CSimplePointBase** 00873530 `Update`, 008734f0 `OnInit` and 00873570 `OnDie`.
  - Shape: `GetCameraManager().SetSingleCut()`, an inline manager helper that sets the byte at +0xb0 and then `OldCamera = Camera`, keeps the manager pointer in a register across the byte store.
  - Writing the store through `CameraManager->` re-reads the pointer.
- **NCameraModes::CFixedDolly** 0087b770 `GetPhysicsMode` returns the CCountedPointer by hidden pointer, with the same inline constructor.
- **NCameraModes::CScriptedControl** 0087ba30 `GetDefaultFOV` and 0087ba60 `GetDefaultRollAngle` call module slot 5 or 6 through `CCountedPointer::operator->`. They fall back to InitCamera.GetHorizontalFOV() and 0.0f respectively.
- **NCameraModes::CPhysicsForceModelRigid** 008fce10 `DoAttractToPoint` is `return LinkedState->Attractors.front().first;`.
  - Shape: a user memberwise C3DVector copy constructor gives the hidden-result copy.
  - The implicit copy goes through an esi temp.
- **CTCHeroStats::MirroredCriminalStatus** 0057a5dd, the constructor.
  - `#pragma optimize("s", on)`.
  - The store order follows the body assignment order: loop, X, Y, Z, LastTimeSeenByGuard, ExitedUsingTeleporter, LastTimeExited, EntranceAngle.
- **SNamedZeroRecord** 006f2bd0, the constructor. A CCharString member at +0x10 and six zeroed words are followed by `memset(this, 0, 0x2c)` (intrinsic).
- **std::vector\<C3DVector\>** 0075f4e0 `erase(iterator)`:
  - `if (pos + 1 != end()) __copy(pos + 1, finish, pos, random_access_iterator_tag(), (int*)0); --_M_finish;`.
  - __copy is the `__fastcall` 0x0074CA20; its tag is passed by const ref.
- **CSoundBankRegistry** 00991840 `GetBank(long)` is a vector-map find. The found arm comes first: `if (it != end()) return it->second; return 0;`.
- **NBankFileManager::CRetailBank** 009ac4d0, the constructor. Shape: the STLport `_Rb_tree_base` constructor allocates the header (malloc 0x24) before the derived node-count initialisation and `_M_empty_initialize`.
- **std::map\<CCharString, CContainedBank\>** 009ac530 `operator[]`.
  - Shape: `insert(it, v)` must be an inline forwarder to the out-of-line `insert_unique` (0x009AB870). Then the result is re-read from its return slot, as retail does.
- **MicroThread::MicroThreadDetails** 009d8890 `MicroThreadStackSize`: `if (g_pActiveMicroThread == this) return 0; return Stack.size();`.
- **NParticleEngine::CDecal** 00af8ab0 `GetInterpolatedLifetimerInSeconds(float)` is `GetParticleUpdatePeriod() * (Lifetimer + fraction)`. It uses engine/CDecal.h through a derived struct.
- **SetAlphaFalloffSharpness** 00c1bd80, `__stdcall`: `g_alphaFalloffExponent = (float)pow(2.0, -sharpness)` with `#pragma intrinsic(pow)`. It is a free function, so line 1 has no module prefix.
- **CConstraintRepeat_00c38470** 00c38570, the constructor (`/EHsc`). It reuses the class model of the landed destructor 0x00C38470.
  - Members: base with vtable 0x012B53A0 and an out-of-line destructor 0x00C383A0; program pointer +4; target pointer +8; a CInterpolatedFloat at +0xc, whose inline constructor clears the +0x30 angle flag; the +0x40 slot (constructor 0x00C2C6B0, destructor 0x00C147D0).
  - The body is `m_value.Reset(v); program->m_updates.Add(this) (slot 9); program->m_pOwner->m_constraints.Add(this) (slot 9)`.
  - The unwind map (state 0 = base destructor, state 1 = slot destructor) confirms the member order. It matched on the first try once `/EHsc` was set.

## Parked for Fable 5.1 (3)
- 00854030 NCameraModes::CScriptModuleBase::IsStretchAroundHero DIFFER(180v178): before the slot-30 IsHeroTheSubject call, ours reaches the component with `mov eax,[eax+4] ; mov edx,[eax] ; mov ecx,eax`, retail with `mov ecx,[eax+4] ; mov edx,[ecx]` | tried: the original && form, a local copy of the component pointer, inline HasTC/GetTC with a camPoint local, a single && chain with a function-scope point | next: model the component fetch as a `T* const&` accessor or a named `CTCCameraPointDefinitionBase* def = point->...` member pointer local; otherwise lane gap: register choice.
- 0087c070 NCameraModes::COrbitGame::OnInit DIFFER(214v214): only the last ObservedPositionZOffset copy, where ours has `mov edx,[ecx+98h] ; mov eax,[edx+8Ch]` and retail `mov eax,[ecx+98h] ; mov edx,[eax+8Ch]` | tried: inline get/set accessors, reordering before DefaultFOV, CurrentMode as a CCountedPointer with operator-> | next: a named `CBase* old = manager->CurrentMode;` local; otherwise lane gap: eax/edx register swap.
- 008eebe0 Mass::BackplaneFriction DIFFER(129v131): in the angular `1 - angular/spin` arm, ours drops the dead fabs value with `fstp st(1)`, retail with `fxch st(1) ; fstp st(0)`. The linear-velocity part and the rest match | tried: an if/else scale local, a ternary, an inline FrictionScale helper (friction by value or const ref, `>` test first, early return, default-0 local), re-reading fabs, a separate angular scale variable, explicit tail-duplicated arms | next: an inline `float& value` friction helper shared by both halves, or a C2DVector GetLength returning double; otherwise lane gap: x87 dead-value pop form.

## Skipped
None. All 40 catalog bounds are right. The dis_all `len=` values include int3 padding; the oracle lengths are the true ones.

## Label corrections
- 0075f4e0 "MedianCut::GetNewTreeNode" is `std::vector<C3DVector>::erase(iterator)`. Its caller is NShape::CShape::UpdateDeletePoint 0x00762010.
- 00991840 "MessageMap::FindMessageByID" is CSoundBankRegistry::GetBank(long). It is a vector map at +0x30, with LowerBound 0x00992290.
- 006f2bd0 "MirroredCriminalStatus::MirroredCriminalStatus" is wrong. It is the constructor of an unreferenced 0x2c-byte record (CCharString at +0x10, then memset); the real MirroredCriminalStatus constructor is 0057a5dd.
- 00af8ab0 "NCreatureAI dynamic_initializer..." is NParticleEngine::CDecal::GetInterpolatedLifetimerInSeconds(float).
- 00c1bd80 "NCreatureAI dynamic_initializer..." is a `__stdcall` setter, SetAlphaFalloffSharpness(float), which writes 0x0139C974.
- 00c38570 "Matrix_Rotation::Constructor" is the CConstraintRepeat constructor, the twin of the landed destructor 0x00C38470. Its sole caller is 0x00C2C895, which allocates 0x4c bytes.
- Callee labels that are also wrong:
  - 0x00C0B510 "Matrix_SetIdentityRotation" is CInterpolatedFloat::Reset (already noted in the landed 0x00C3A970).
  - 0x00C2C6B0 "CQ_GuildTrainingPreMeleeScript::Init" is the +0x40 slot constructor (vtable 0x012B4630).
  - 0x0084E1A0 "LinkedListInsertNode" is NCameraModes::CBase::AddAttractorGoTo.
  - 0x0084E810 "ScriptEventList::InsertEvent" is CBase::AddSubComponent.
  - 0x0084D050 is CBase::GetWorld.
- All `vfunc_N` rows were renamed through their vtable slots; line 1 of each source gives the name.
