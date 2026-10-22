# Hand chunk 293 (CPSCSingleSprite .. CPhysicsMesh): 15 of 40 byte-exact

All 15 are RELOCATION_MATCH under q.py (gall.sh). Each has a behaviour test that passes under g++ (tall.sh) and VC7.1 (tall.py).

## What matched (15)
- CParentDefClassBase rows (really the operator= of the four def classes):
  - 0045be2b CEnvironmentDef::operator=
  - 0045f854 CSpecialAbilitiesBerserkDef::operator=
  - 00461f7c CHeroCombatDef::operator=
  - 00465eb3 CGlobalDef::operator=
- CPatchTesselationEdgeStrip row: 00459e46 is really CPlayerGuiDef::~CPlayerGuiDef (compiler-generated member teardown).
- CPSCSplineBase: 00b22310 InterpolateTo.
- CPSCUpdateNormal: 00b0b370 DatabaseCopy.
- CParticleEmitter:
  - 00aeefe0 DatabaseCopy
  - 00af0380 the constructor
- CParticleEmitterManager: 00aeac90 DumpInfo.
- CParticleProgram:
  - 00ae2230 WriteToFile
  - 00adfdc0 OptimiseConstraints
- CParticleSystem: 00af4660 ReadBinary.
- CPersonalScript_EmptyScript: 00ef5200 CheckForHeroActions.
- CPersonalScript_AardvarkScript: 00efc500 Main.

## Parked for Fable 5.1 (23)
- 00b08250 CPSCSingleSprite::SetTrailPositionsFromSpline DIFFER(745v724): retail does `fld [E31]; fmul [z]` (matrix element first) in the inline system-space transform; the clamped count spills to [esp+10h] where retail keeps it in ebx; the else loop keeps `system` in ebp, not ebx | tried: helpers forceinline/out of line, operand-order swaps, count as local/member | next: lane gap: x87 commutative operand canonicalisation (see LESSONS)
- 00b08530 CPSCSingleSprite::Update DIFFER(664v654): the key is reloaded through &PositionParam after LowerBound 0xAF2C50, where retail keeps the tested value in ebp; the transform evaluates E21*y first with operand order reversed | tried: 5 variants (forceinline find, by-value key, helper splits) | next: model the UpdateParams map find as an inline STLport lower_bound on a const key copy
- 00b22b40 CPSCSplineBase::ReadBinary DIFFER(592v594): prologue only. Retail copies `this` to edi and spills edi; ours spills ecx | tried: pointer loop, indexed loop (fixes the prologue but reloads the base) | next: scheduling-only, leave
- 00aed2f0 CParticleEmitter::IsVisible DIFFER(778v778): Emitter2D is held in dl across CalcFadeAlpha (the same-TU stand-in preserves edx) where retail uses bl with ebx pushed | tried: stand-in on/off, optimize pragmas, forceinline loop helpers, is2D local | next: compile the real CalcFadeAlpha so it clobbers edx (e.g. a body that needs edx), or give up the stand-in and find another way to stop the flag-word reload
- 00aede60 CParticleEmitter::SetOrientation DIFFER(796v796): every fmul pair in the inlined SetInverseNoTranslate (and the Right.Z cross term) loads the other operand first | tried: forceinline Set/SetInverseNoTranslate, all operands swapped (identical output), union/array matrix (identical), /G6, /Op | next: lane gap: x87 commutative operand canonicalisation
- 00aee240 CParticleEmitter::Initialise DIFFER(797v791): inline SetInverse only (cofactor operand order, a13*E31 summed first, 1/det kept in st0 rather than spilled) | tried: early return vs shared return (fixed the prologue), determinant term orders (identical output) | next: same lane gap as 00aede60
- 00aee560 CParticleEmitter::PreparePrimitives DIFFER(854v854): inline SetInverse only, plus the int stores interleaved with it | tried: IsSleeping/IsEmitter2D accessors (fixed the prologue), C2DVector user copy ctor (fixed the frame), PDB anonymous-union matrix (no change) | next: same lane gap as 00aede60
- 00aeeba0 CParticleEmitter::StartFrame DIFFER(892v890): inline SetInverseNoTranslate x87 stack use and operand order; the fsqrt tail pops with `fstp st(0)` x3 where retail does `fstp st(3)` | tried: forceinline interpolators, named camera local, separate zero stores | next: same lane gap; the length could be the out-of-line-style `sqrt` of a named lengthSq
- 0049be90 CParticleEnvironmentInteraction::IsParticleCollidingAt DIFFER(806v808): register allocation only. Retail keeps hitPos in edi, the iterator in ebx and the hit flag in the filter temp's byte | tried: 5 variants (filter temp, CThingFilter_All() {}, temp in the condition) | next: scheduling/regalloc, leave
- 00ae1320 CParticleProgram::ClingToMesh DIFFER(376v681): SSE branch is inline asm | tried: behavioural C++ of the x87 branch | next: lane gap: inline __asm SSE
- 00ae29d0 CParticleProgram::SetupSimulation DIFFER(422v726): SSE branch is inline asm | tried: behavioural C++ | next: lane gap: inline __asm SSE
- 00adf960 CParticleSimulation::SatisfyDistanceConstraints DIFFER(466v766): SSE branch is inline asm (push esi/pop esi, aligned ebx frame) | tried: behavioural C++ | next: lane gap: inline __asm SSE
- 00af5090 CParticleSystem::ClearParticle DIFFER(619v657): retail has an 8-aligned frame, unrotated loops, a setne-materialised handle test and the group index in frame slots | tried: optimize("g", off) (1484 bytes), inline/out-of-line erase, handle copy forms | next: look for a /Od-like pragma region or a volatile-free shape that keeps the loops unrotated
- 00af5430 CParticleSystem::ClearDecal DIFFER(572v607): twin of 00af5090 | tried: the ClearParticle model | next: as 00af5090
- 00f361d0 CPerlinNoise::Get3DNoise DIFFER(740v873): floor is inline asm (fld/fsub 0.5/fistp/fild/fadd 1.0 on ebp-frame slots) | tried: behavioural C++ | next: lane gap: inline __asm floor
- 00463334, 00475026, 0047550f CPersistContext::TransferMap<CCharString,long> DIFFER(838v838 / 838v838 / 850v850): frame slots only. The key traits sit at [ebp-24h] vs retail [ebp-8] | tried: advisor slot-weight rule, scope moves, temporaries | next: frame-slot ordering, lane gap: slot weights
- 005d8978 CPersistContext::TransferMap<enum,enum> DIFFER(754v754): frame slots only (insert result temp -3Ch vs -30h) | tried: advisor RST2 best.cpp | next: as above
- 009bae70 CPersistContext::ReadSection DIFFER(589v593): only the "" filename temporary (retail builds it in the dead name-parameter slot) | tried: temp forms | next: build the CCharString temp from a by-value parameter
- 00a177a0 CPhysicsMesh::GetPointIntersectionFromToUsingSegmentation DIFFER(838v840): the normal's Y scale is issued one slot late, and the dot products with the direction sum Z first where retail sums Y first | tried: extern const epsilon (fixed the -eps), swapped dot operands (no change), CPlane(normal, d) (fixed the plane stores) | next: x87 scheduling, leave
- 00a17f80 CPhysicsMesh::GetPointIntersectionFromToUsingFullCollision DIFFER(822v818): same as the 00a177a0 twin, plus `fld st(0); fstp` for the distance store where retail does `fst [eax+18h]` | tried: twin model, chained store | next: x87 scheduling, leave
- 00a6c640 CPhysicsMesh::CalculateBoundingInfo DIFFER(514v517): frame 0x38 vs 0x44 (retail keeps the operator+/- results of the sphere section as frame temporaries) | tried: operator temporaries | next: named C3DVector temporaries with user copy ctors in the sphere section

## Skipped (2)
- 00f334e0 and 005e9840 CPersistContext::Transfer<signed char>: each row holds two functions.

## Label corrections
- 00459e46 (manifest ~CPatchTesselationEdgeStrip) is CPlayerGuiDef::~CPlayerGuiDef.
- The four CParentDefClassBase::operator= rows are CEnvironmentDef / CSpecialAbilitiesBerserkDef / CHeroCombatDef / CGlobalDef::operator=.
- 00efc500 is NScript::CPersonalScript_AardvarkScript::Main.
- Stream callees:
  - 0x994B40 is CMemoryDataOutputStream::GetSize.
  - 0x9950F0 is CDataInputStream::Read3DVector.
  - 0x993EA0 "WriteCBYTE" is used as WriteULong.
  - 0x993B10 "~CChunkedFileChunk" is ~CMemoryDataInputStream.
  - 0x996B30 "~CCompressedMemoryDataInputStream" is ~CMemoryDataOutputStream.
  - 0x996B60 is CMemoryDataOutputStream::Reset.
  - 0x9964E0 is CDataInputStream::ReadToStream.
- 0x99F8B0 is GFUIntToCharString.
- Particle emitter:
  - 0xAEEF40 is CParticleEmitterManager::AddParticleSystem.
  - 0xAEAA20–0xAEAA60 are the emitter default-distance getters.
  - 0xAECD70 "IsVisible_CalcFadeAlpha" is CParticleEmitter::CalcFadeAlpha.
  - 0x6E1B10 is ICF-shared with CParticleEmitter::GetPosition.
  - 0xAEDCA0 "GetNoDecals" is CParticleEmitter::GetNoParticles (0xAEDCD0 is GetNoDecals).
  - 0xAF0880 "C3DMeshFileBoneChunk::SetLocalizationMatrix" is the CEnginePrimitiveParticleGroup2D constructor.
- CPSCSingleSprite:
  - 0xB081A0 "EventQueue_PushEvent" is CPSCSingleSprite::AddTrailPoint.
  - 0xB09D50 "SortTreeRecursively" is the trail list clear.
- 0xAF7910 "Vector_CopyAndDeleteOld" is vector<CArray<long>>::erase.
- Physics:
  - 0x49C490 is the thing-search all-things-in-rough-area query.
  - 0xA6CBB0 "CEngineInternalPrimitiveBase::AddChildPrimitive" is the scratch face-list getter (clears and returns the static vector<long> 0x13D2D1C).
  - 0xA14480 "C3DVector::GetScaled" is C3DVector::Normalise() returning the length.
- 0xAE04E0 "GetGroupVectorBuffer" returns the scratch CMatrix4x4 buffer (0xAE0540 is the C4DVector one).
- 0xAE2510 "ParticleProgram_ExecuteLooped" is the program pass called by SetupSimulation (thiscall, (simulation, steps)).
- PersistContext:
  - 005d8978 is an enum-to-enum TransferMap.
  - The 00463334 family are <CCharString,long>-style maps.
  - 0x5D8730 is map erase(key).
  - 0x46367A / 0x463732 are the write and read paths.
