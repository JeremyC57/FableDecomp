# Hand chunk 63 (CParentDefClassBase .. CParticleEmitter): 40 of 40 byte-exact

All 40 rows grade MATCH / RELOCATION_MATCH from the file in `src/` (gall.sh, `grades.txt`) and every
behaviour test prints `<addr>_TEST PASS` under g++ (tall.sh) and the lane's VC7.1 cl.exe (tall.py,
`tall_vc.txt`). Flags needed: `/EHsc` for 0x00C3F3D0, `/Oa` for 0x00AEE180 / 0x00AEE1E0 (both in the
lander's sweep, recorded as `// Flags:` lines); the def-module rows carry `#pragma optimize("s", on)`.

## What matched (40)

- **Def classes behind the propagated "CParentDefClassBase" labels (10)** -- the five
  `operator=` rows are the targets of the slot-19 `jmp` thunks of CArmourDef (0x00461344),
  CCombatSequenceDef (0x00461D50), CBrainDef (0x00462B15), CPlayerInventoryDef (0x00462E2D) and
  CEnvironmentThemeDaySetDef (0x00472E83): memberwise assignment in PDB member order (retail base
  0x3c, retail maps/vectors/CArray 12 bytes, so every member sits 4 below the PDB). Shape lesson:
  the def module is `/Os` (`#pragma optimize("s", on)`: every copy pair through eax; floats copied
  as integers). 0x0046EE98 is the real CParentDefClassBase copy constructor (pack(1): InstantiationName
  +0x25, SubDefInfoMap +0x29; the CVectorMap = 12-byte vector + empty comparator +0xc + Dirty +0xd)
  -- its inline CVectorMap copy constructor needs `__forceinline` under `/Os`. 0x0067C6D0 is
  CBlurredAttitudesDef's copy constructor with that base copy inlined (speed TU, no pragma).
  0x00F25C60 / 0x00F25F60 are the copy constructor and `operator=` of CArenaWaveDef (no-RTTI vtable
  0x012FB2F0; its Transfer slot names NumWaveCreatures +0x28, Creatures +0x2c, ShortWave +0x38;
  base CDefClassBase directly, default packing). 0x00C3D2D0 ("GetInstantiationName") is a two
  out-parameter getter of the no-RTTI static object 0x0143BB58 (`CVtable_012B5870::GetPair80`).
- **NParticleEngine::CParticle (2)** -- the constructor 0x00AF7A90: every zero store follows the
  vtable store (inline member constructors: scoped_ptr + two 12-byte CArrays) and the body's
  `RandomAcceleration.reset()` re-reads the pointer it just zeroed. GetInterpolatedLifetimerInSeconds
  0x00AF85E0: `GetParticleUpdatePeriod() * (Lifetimer - CreationTime * (1.0f / 255.0f) + fraction)`
  (pool 0x01231724 = 1/255), the getter called first and kept on the x87.
- **CParticleDataBank (1)** -- NotifyBankSize 0x00AF16E0: `Resize(count, CParticleEmitterInfo())`
  then a counted loop; the element must be a reference local (`info = ParticleEmitters[i]`), two
  `operator[]` uses re-read the data pointer twice.
- **Destructors behind the propagated "CParticleEffect" labels (12)** -- `??_G` of
  NUISystem::CDynamicList 0x00541870, NUISystem::CParticleEffect 0x005440B0,
  NUISystem::CSwappingStateComponent 0x00547AB0 (three vtables + inline member/body work, the
  CChangingStateComponent destructor 0x0052CCF0 out of line), CHeightMap 0x008F6DE0 and
  CShaderDataBank 0x00A62860: `__forceinline virtual ~T()` + a `new T` factory, vector members with
  inline `if (m_first) free(m_first)` destructors. CCreatureAction_DragonBurninateInto 0x00953A40:
  NO vtable store because the class declares no destructor (VC7.1 drops the vfptr reset in an
  implicit destructor). Three unreferenced `??_G` bodies (0x009B6C50, 0x009F9A10, 0x00A5E7B0) are
  non-polymorphic records deleted through a helper; the two that form `lea edi,[esi+4]` before the
  null test hold that member as an object with its own user destructor (nested member), a flat
  layout gives a late `lea ecx`. 0x00562E70 / 0x00563410 are CTCHeroOnlineScoreboard members
  (ResetScoreboard: inline scoped_ptr reset + ClearScoreboard + zero stores; AbortWithMessage: the
  CWideString text homed in the dead argument slot, boost safe-bool `if (PHttp)` = `neg ; sbb ;
  test edx,offset get`). 0x00C3F3D0 is the no-RTTI CVtable_012B5C10 destructor (`/EHsc`): the
  `shr eax,1Fh ; not al ; test al,1` test is a NEGATING inline accessor
  (`bool IsRunning() const { return !m_stopped; }`), the virtual Stop() devirtualised.
- **CParticleEmitter (15)** -- the retail layout pinned here: ParticleSystems (12-byte vector of
  8-byte counted pointers) +0x08, UpdateParameters (0x24) +0x14, PrimitiveGroupHandle +0x38, the
  PDB bit-field groups at +0x40 (Priority..Emitter2D, Sleeping = bit 10), +0x42, +0x44, +0x45
  (ClearedParticles bit 3, ClearedDecals bit 4), Transform +0xb0, sizeof 0x16c; CParticleSystem:
  Particles/Decals lists +0x1c/+0x20, NoActiveParticles/NoActiveDecals +0x48/+0x4c, bool
  bit-fields +0x50 (Enabled bit 1, Dead bit 2, Visible bit 5), Name +0x6c, sizeof 0x70.
  GetEmitterCost / EnterTimelineEnd / GetNoParticles / GetNoDecals / ClearAllParticles /
  ClearAllDecals are plain `for (it = first; it != last; ++it)` walks (bit-fields test/set as
  `test/or byte ptr`). GetParticleStats 0x00AED290 needs `bool IsSleeping() const { return
  Sleeping; }` to keep `xor edx,edx ; mov dx,[edi+40h] ; shr edx,0Ah ; test dl,1`.
  GetCurrentOrientation 0x006E1B40 = `CRightHandedSet(Transform.GetRow(2), Transform.GetRow(1))`
  with by-value row accessors and a memberwise C3DVector copy constructor. 0x006E2570 is
  CParticleUpdateParameters::operator= (returns this), 0x00AF0610 the emitter's
  SetUpdateParameters with it inlined (inline CVectorMap assignments). GetPParticleSystemFromName
  0x00AEE180 / const twin 0x00AEE1E0 need `/Oa` (name block hoisted, `last` cached) and a FREE inline
  `operator==(const CCharString&, const CCharString&)` re-reading `left.m_pData` at every use to
  keep `add eax,6Ch ; mov eax,[eax]`. DatabaseClone 0x00AF0650 (emitter) / 0x00AF5960 (system):
  `new T; clone->DatabaseCopy(*this);` (both arms duplicated by the compiler).
  0x00815820 is CTCHitLocations::GetPHitLocationFromName: the def's virtual GetInstantiationName
  (slot 10, CDefString by hidden pointer) must go into a NAMED local so the call precedes the name
  push and `==` re-addresses the dead argument slot.

## Parked for Fable 5.1 (0)

None.

## Skipped

None (no row spans two functions).

## Label corrections

- 0x00461344 / 0x00461D50 / 0x00462B15 / 0x00462E2D / 0x00472E83 "CParentDefClassBase::operator=" ->
  CArmourDef / CCombatSequenceDef / CBrainDef / CPlayerInventoryDef / CEnvironmentThemeDaySetDef::operator=.
- 0x0067C6D0 "CParentDefClassBase::CParentDefClassBase" -> CBlurredAttitudesDef copy constructor.
- 0x00F25C60 / 0x00F25F60 "CParentDefClassBase" -> CArenaWaveDef copy constructor / operator=.
- 0x00C3D2D0 "CParentDefClassBase::GetInstantiationName" -> getter of the no-RTTI class 0x012B5870
  (the static object 0x0143BB58).
- 0x00431F10 "CThingBuildingDef::operator=" -> CDefClassBase::operator= (callee).
- 0x00541870 / 0x00547AB0 / 0x008F6DE0 / 0x00953A40 / 0x00A62860 "CParticleEffect `scalar deleting
  destructor'" -> NUISystem::CDynamicList / NUISystem::CSwappingStateComponent / CHeightMap /
  CCreatureAction_DragonBurninateInto / CShaderDataBank; 0x009B6C50 / 0x009F9A10 / 0x00A5E7B0 are
  unreferenced `??_G` of unknown records.
- 0x00562E70 / 0x00563410 "CParticleEffect::~CParticleEffect" -> CTCHeroOnlineScoreboard members
  (ResetScoreboard / AbortWithMessage, descriptive names); 0x00C3F3D0 -> destructor of the no-RTTI
  class 0x012B5C10.
- 0x006E2570 "CParticleEmitter::SetUpdateParameters" -> CParticleUpdateParameters<ulong,ulong>::operator=.
- 0x00815820 "CParticleEmitter::HasAllocatedDecals" -> CTCHitLocations name lookup.
- 0x00AED140 "ClearAllDecals" -> CParticleEmitter::ClearAllParticles; 0x00AEDCA0 "GetNoDecals" ->
  CParticleEmitter::GetNoParticles; callees 0x00AF58A0 -> CParticleSystem::ClearAllParticles,
  0x00AF3F30 -> CParticleSystem::GetNoActiveParticles.
- 0x00AF5960 "CParticleEmitter::DatabaseClone" -> NParticleEngine::CParticleSystem::DatabaseClone
  (callee 0x00AF4E10 "CHeroMorphDef" -> CParticleSystem constructor).
- 0x00AEAA00 "GetMaxDrawDistance" is the particle update-period getter used by CParticle.
- engine/CParticleEmitter.h (PDB offsets) disagrees with retail from +0x14 on (UpdateParameters
  +0x14 not +0x18; Transform +0xb0 not +0xbc) -- the 12-byte vector and 16-byte CVectorMaps.
