# Hand chunk 198 (CPSCLight .. CPatchTesselationEdgeStrip): 25 of 40 byte-exact

## What matched (25)
- **Def-class Copy (operator=, 6 rows: 0045c2e8 CCombatTypeDef, 0045cb1c CHeroAbilityDef, 0045d63c CExpressionDef,
  0045f635 CSpecialAbilitiesBattleChargeDef, 0045fd53 CSpecialAbilitiesForcePushDef, 004629fb CEngineThemeDef)** -- identified by
  slot 19 of each vtable (the `jmp` thunk right before the body). Memberwise assignment in PDB order at retail offsets
  (PDB - 4, 12-byte containers), containers through their out-of-line operator= (vector<long> 0x454886, vector<float>
  0x45EF2D, vector<CCharString> 0x432EE9, the attack-pattern map 0x45C3CF), `#pragma optimize("s", on)`. A `float[3]`
  member copies as an element loop (`push 3 ; pop edx`), a `bool : 1` member as the xor/and/xor merge.
- **CParentDefClassBase::InternalSaveBinary (009b0200)** -- the CDefClassBase part (three bit flags through WriteCBYTE) is an
  inline virtual called qualified; the packed base keeps the CVectorMap at +0x29 (vector, 1-byte comparator, needs-sort flag);
  an inline Sort() (SGI sort helper 0x9B84C0, comparator by value) runs before the count (WriteUShort) and the entries.
- **CPSCSplineBase::WriteBinary (00b21630)** -- the points are written from a by-value copy of the CArray (copy ctor 0x6F0810,
  inline free-dtor); the walking pointer must be declared inside `if (count > 0)` so its load sits after the guard.
- **CParticleEmitterInfo::GetEmitterTemplate (00af1480)** -- the intrusive pointer's release must go through an inline
  `CResource::Release()` (`dec [ecx+4] ; jne`), not an open-coded `--Object->RefCount`.
- **CParticleEmitterDatabase GetEmitterTemplate twin (00ae8a80)** -- the flag parameter is `long` (no movzx before the push);
  callees named exactly as in the landed twin 0x00AE89E0 so each address keeps one symbol.
- **CParticleEmitterManager Update / StartFrame (00aeb4a0, 00aeb550)** -- inline STLport list erase (unlink, counted-pointer dtor
  that releases then nulls both words, free); Dead/Initialised read through `bool IsDead() const` accessors (one `mov ax`
  load, `mov dl,al ; shr dl,5`); the byte flags at +0x44 need `Emitter2DLayer : 7` declared so the uchar group starts there;
  StartFrame's if/else of two calls tail-merges (the ternary argument does not).
- **CEngineTexturePalette ctor (00bf90a0)** -- body assignments in flag/default/bank order after the vector's inline ctor;
  the default entry is a local with `Index = 0` after its CCharString ctor, pushed through the inline STLport push_back.
- **CParticleProgram (00ae1260, 00ae1600, 00ae1150)** -- retail layout = PDB less 16 bytes of narrower containers
  (NonSimCount +0xbc, NonSimPositions +0xc0, SimGroups +0xfc, NonSimGroups +0x108); the non-sim copy is an SGI random-access
  copy that must be `__forceinline`; SetParticles derives from the landed CParticleArrayOwner so AllocateParticles binds to
  its symbol; the C4DVector(C3DVector, 0.0f) temporary gives the hoisted W store.
- **CParticleSimulation::WriteToFile (00ae2050)** -- plain field writes; TimestepChanged goes out as a ULONG.
- **Eight destructors labelled "~CPatchTesselationEdgeStrip"** (identified through the deleting dtor in each vtable's slot 0):
  004c5da0 CTCGraphicAppearance (novtable, inline map dtors), 004e2170 CVillageDef (pragma s, novtable over the landed
  CDefPointeeBase/CBase_009fc550 chain), 00589c55 CTCInventory and 005f3cfe CTCInventoryAbilities (pragma s, the landed
  CTCInventoryClothing two-base model), 00706e00 CTCHeroAttachableAppearanceModifiers and 007223d0 CTCHeroMorph (the landed
  CTCLight model), 009a7b70 CBankFileManager, 009f5c80 CInputManager. Members die in reverse order; STLport map dtors inline
  as erase/reset/free; an element-destroying vector loop needs an inline `DestroyRange(first, last)` helper so `last` is cached.

## Parked for Fable 5.1 (15)
- 00b1a640 NParticleEngine::CPSCLight::CPSCLight DIFFER(190v192): `mov ecx,ebx ; mov eax,ebx` vs retail reloads `[esi+0x64]` / `[esi+0x60]` for the user-point array clear() | tried: ctor clear(), reference-store Assign helper, body clear() | next: the CArray ctor or clear() may be a separate out-of-line-then-inlined template; try a non-inline `CArray()` member defined later in the TU with `#pragma auto_inline(off)` off/on
- 009d0630 CPackedBlockAllocator::RebuildFreeList DIFFER(203v192): retail keeps the address-taken block index in esi and only re-homes it at its definitions; ours reloads after `*last = i` and InsertGrow and keeps the 4 for `++last` in esi | tried: long / unsigned long index, temp-copy push_back, STLport _Construct, unsigned-long vector | next: lane gap: alias analysis of an address-taken loop variable
- 00a62c90 CPackedUIntArray::PackArray DIFFER(238v238): `mov edi,[bits]` / bias computed in ecx (retail) vs eax / in-place edx (ours) | tried: declaration orders (frame slots now match), compound vs single-expression bias | next: lane gap: register allocation
- 009b9770 CParsedItem::InitAsFloat DIFFER(177v181): divisor kept in memory only with `volatile` (probe artefact), loop not rotated (text[0] preload), bl/cl swap, integer phase fmul+fiadd vs fild/fxch/fmul/faddp | tried: float/double accumulators, digit+whole order, pragma s, volatile divisor | next: the divisor's memory home suggests /Op or an address-taken divisor in the original; try a `float* scale = &divisor` style helper
- 00543d30 NUISystem::CParticleEffect::Initialise DIFFER(183v183): the default CRightHandedSet temporary is stored before the third push and the vtable load comes first | tried: inline helper, ctor temp, front-end local | next: scheduling only (compiler build)
- 00543df0 NUISystem::CParticleEffect::ChangeState DIFFER(217v217): same scheduling as Initialise | tried: as Initialise | next: scheduling only
- 00543ee0 NUISystem::CParticleEffect::Draw DIFFER(194v190): retail loads Position.X straight from the member and copies only Y into the local; ours copies both halves | tried: plain copy, inline helpers, ternaries, user copy ctor | next: try `C2DVector position(Position.X, Position.Y)` with a (x, y) ctor and the rescale as member functions of C2DVector
- 00aeabe0 CParticleEmitterManager::CreateDecalEmitter DIFFER(158v166): retail keeps the null result in ebx (pushed, both failure returns `mov eax,ebx`, frame offsets +4) | tried: nested ifs, early returns, a named null local | next: lane gap: constant-in-register heuristic
- 00aeb600 CParticleEmitterManager::CreateInterpolatedEmitter DIFFER(202v202): `mov esi,[clone]` + arg in ecx (retail) vs `mov ecx,[clone] ; mov esi,ecx` + arg in edx | tried: result / created locals | next: lane gap: register allocation
- 00aeb6d0 CParticleEmitterManager::CreateEmitter DIFFER(201v192): retail loads the clone pointer after the C3DPositionInterpolator ctor and caches it; ours loads before (local) or reloads after each call (operator->), template/result regs swapped | tried: operator->, created local, const-ref Insert, result placement | next: lane gap: load scheduling across an escaped local
- 00aeb790 CParticleEmitterManager::Create2DEmitter DIFFER(186v171): retail this=esi/result=edi, ours the reverse | tried: result before/after the template fetch | next: lane gap: register allocation
- 0081b160 CParticleMorphs::GetAllMorphsOfType DIFFER(189v181): retail zero-fills the blank CEntry from one `xor edx,edx` before the pushes; `= { 0 }` spreads it over eax/edx/imm and interleaves | tried: CEntry() argument, `= CEntry()`, `= { 0 }`, nested success path (block order now right) | next: try an explicit full aggregate list or a CEntry ctor writing all nine fields
- 00af4240 NParticleEngine::CParticleSystem::WriteBinary DIFFER(219v219): strength-reduced walk pointer biased to entry+0x14 (retail) vs +8 (ours) | tried: iterator loop, landed state-pointer walk | next: walk a pointer to InstanceName (the field read first after the call) or index by `i`
- 00af5380 NParticleEngine::CParticleSystem::AllocateDecal DIFFER(170v167): retail pushes edi in the prologue and does `inc dword ptr [ebp+4Ch]`; ours shrink-wraps edi and does load/inc/store | tried: placement new after raw allocation, class operator new + retry, merged null tests (block order now right) | next: scheduling only
- 00b201d0 NParticleEngine::CParticleSystem::GetInverseSystemTransformWithTranslate DIFFER(225v225): x87 term order of the three translation dot products and the fld/fmul operand choice of some scale products | tried: three association forms | next: try C3DVector-level helpers (Dot of a column, component-wise scale) so the operand order follows the inline structure

## Skipped
None (no row contains two bodies).

## Label corrections
- 0x0045C2E8 / 0x0045CB1C / 0x0045D63C / 0x0045F635 / 0x0045FD53 / 0x004629FB "CParentDefClassBase::operator=" are the Copy (slot 19) bodies of CCombatTypeDef / CHeroAbilityDef / CExpressionDef / CSpecialAbilitiesBattleChargeDef / CSpecialAbilitiesForcePushDef / CEngineThemeDef.
- 0x00BF90A0 "CParticleMorphs::Initialize" is CEngineTexturePalette::CEngineTexturePalette(CGraphicDataBank*, bool, unsigned long) (only caller CEngineLandscapeMap ctor 0x00BDD2F0).
- 0x00AE1150 "CParticleSimulation::SetParticles" is a CParticleProgram method (NonSimCount/NonSimPositions).
- 0x00543D30/0x00543DF0/0x00543EE0 "CParticleEffect" is NUISystem::CParticleEffect (vtable 0x0124735C), not the particle engine.
- "~CPatchTesselationEdgeStrip": 0x004C5DA0 ~CTCGraphicAppearance, 0x004E2170 ~CVillageDef, 0x00589C55 ~CTCInventory, 0x005F3CFE ~CTCInventoryAbilities, 0x00706E00 ~CTCHeroAttachableAppearanceModifiers, 0x007223D0 ~CTCHeroMorph, 0x009A7B70 ~CBankFileManager, 0x009F5C80 ~CInputManager.
- Callees: 0x00AEAAF0 "GFGetPreMainMemoryUsed" is CParticleEmitterManager::Get (returns 0x013D2E1C); 0x00AF1280 "CTCInventoryBase::GetCategoryWithIndex" is CParticleDataBank::GetEmitterInfo; 0x00B20B10 "C3DMeshFileUniqueVerticesChunk::WriteToFile" is CSplinePosParamData::WriteBinary; 0x00AE18C0 "WriteAnimationData" writes a C4DVector array; 0x00AED010 "CTCPhysicsStandard::CanCollideWithLandscape" is CParticleEmitter::IsVisible; 0x00AF3D80/0x00AF3DA0 "CVertexBufferWin32::DoSizeof" are CParticleSystem::GetSystemTransform / GetInverseSystemTransform; 0x009FC550 "~CMemoryBlock" is the CResource-style base dtor (landed CBase_009fc550); 0x009B84C0 "make_heap_partial" is SGI sort; 0x00AF8B00 "operator_new" is the particle memory allocator (CDecal::operator new).
- 0x00AE8A80 and 0x00AE89E0 both carry "CParticleEmitterDatabase::GetEmitterTemplate(long,bool)": 0x00AE8A80 has no callers and no pointer refs (named GetEmitterTemplate_00ae8a80 here).
