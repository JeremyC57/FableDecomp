# Hand chunk 151 (CParentDefClassBase .. CPatchTesselationEdgeStrip): 30 of 40 byte-exact

All 30 sources in src/ grade MATCH/RELOCATION_MATCH (grades.txt); all 30 tests PASS under g++ (tall.sh) and the lane's
VC7.1 cl.exe (tall.py). 9 rows parked in differ/, 1 skipped. Most catalog labels in this chunk were propagated; the real
identities are listed under "Label corrections".

## What matched (30)
- **CSkyDef / CJackOfBladesBattleDef** (0x0046F4F0, 0x004E46CD): copy-assignment of def classes (vtable slot 19 `jmp`
  stub before each body). Shape: `optimize("s")` def TU, packed 0x39-byte CParentDefClassBase with out-of-line operator=,
  PDB members 4 bytes lower; CSkyDef's vector member goes through its out-of-line operator= last.
- **CParentDefClassBase::GetPSubDefInfo** const / non-const (0x009AD4A0, 0x009AD510; vtable slots 14/15): CRC the name and
  look it up in the sub-def CVectorMap. Shape: the map's inline Find takes the key BY VALUE and passes `&copy`
  (`unsigned long k = key; LowerBound(&k); ... key < it->first`), so the key stays in edi while the copy is homed in the
  dead name slot; `if (it != end()) return &it->second; return 0;`.
- **CParticleCreationParameters** ctors (0x006E25C0, 0x006E2640): everything in the init list; the RHSet copies word by word
  through ecx/edx only when C3DVector has a memberwise user copy ctor; the update parameters' inline ctor (CBase out of
  line, vtable, vector zeros + Dirty byte).
- **NParticleEngine::CParticleDataBank** ctor (0x00AF1670): CBankFile + CResourceBank bases (secondary vtable first), the
  CArray as STLport _Vector_base with an `_STLP_alloc_proxy` end member (puts `lea ecx` before the third zero store), body
  `ResourceList.m_usedSize = 0; ResourceList.SetMaxSize(0x10000)`.
- **NUISystem::CParticleEffect / CViewport::GetDataFromDef** (0x00543FA0, 0x0054E5D0): virtual GetDef by value, cache the
  first record's +0x3c word. Shape: STLport `size()` over begin/end and `operator[]` = `*(begin()+n)` (re-reads begin);
  the CDefPointer temporary releases through the two-accessor ReleaseTemporary.
- **_global::__pop_heap<pair<ulong,CAnimationEntry>>** (0x005E21F0): out-of-line pair operator=, implicit pair copy into
  the __adjust_heap argument area (counted pointer inc), callee destroys its by-value `value`.
- **CLevelLoader::CObject::~CObject** (0x005195B0): empty dtor; STLport ~vector (_Destroy with by-value bounds) and ~set
  (inline clear in STLport order, header freed).
- **CParticleEmitter::ShouldSpawn** (0x00AECF90): camera position fetched before the flag test, named delta vector,
  `!(lengthSq <= max*max)` gives `test ah,1 ; je` (`>` gives `test ah,5 ; jp`).
- **CParticleEmitter::SetPosition** (0x00AEDDC0): flag through an inline `bool IsEmitter2D()` (whole-word read); the
  interpolator copied by its IMPLICIT operator=, then an inline three-float matrix-translation setter.
- **CParticleEmitter::ClearAllPrimitives** (0x00AEE940): CEnginePrimitiveHandle::Clear inlined (null-safe GetRefCount()==1,
  release, second zero store); engine slot 31 removes the group.
- **CRecordSet_00cbd510** dtor (0x00CBD510) and **CScriptRecordSet_00e299f0** ctor (0x00E299F0): the same CBase-derived
  class (vtable 0x012C1648, ctor 0x00CB8110). Dtor: `__declspec(novtable)` over CBase's out-of-line virtual dtor, members in
  reverse (inline _Destroy loop, _Destroy_range with an `allocator<T>()` temporary, list clear + header free). Ctor of the
  derived class (vtable 0x012E22C8): the two argument words stored before the vtable store come from an inline
  `novtable` intermediate base; two STLport _Rb_tree members (header malloc'd in the base part, count zeroed, then
  _M_empty_initialize).
- **CParticleEmitterDatabase::CreateSymbolMap** (0x00AE85F0): per entry a NAMED name copy, a named empty prefix, then
  `AddSymbolDirty(prefix, name.ToUpper(), handle)`. **GetEmitterTemplate** (0x00AE89E0): the info pointer as a named
  local BEFORE the counted-pointer local (call precedes the zero stores); map path is the Find ternary.
- **CParticleEmitterManager::CreateInterpolatedEmitter** x2 (0x00AEB850, 0x00AEB8C0): signatures from the manifest (PDB);
  `>= 1.0f` / `<= 0.0f`; the database through a static counted pointer's inline operator->.
- **CParticleEnvironmentInteraction::GetWindVelocity** (0x0049BE10): `direction * GetSpeed()` through an inline
  `C3DVector::operator*(float)` building a named result (param homed with `fst`), both virtual calls through an inline
  `CWorld::GetWind()` accessor (the direct member chain picks edx for the first vtable).
- **CParticleMorphs::MorphEntry_FromPointer / FromValue** (0x00AB0B70, 0x00AB0BF0): `p ? CCharString(p,-1) :
  CCharString("",-1)` conditional-temporary bookkeeping; the FromValue twin needs the entry getter `__forceinline`.
- **CVector_ulong_00c29c30::push_back** (0x00C29C30): Dinkumware size()/capacity(), inline _Ufill passing an empty
  `_Scalar_ptr_iterator_tag` BY VALUE (VC homes it in the dead argument slot and pushes that dword).
- **CRegionMinimap::CreateRegionMinimap** (0x0066DC50): resize cells, inlined InitialiseSurface, box, populate, TGA, then
  an inline Free() (first read, three zeros, free).
- **CStringPairRecordVector_00cb7210::push_back** (0x00CB7210): STLport push_back of a 24-byte record; the record's
  memberwise copy ctor must be `__forceinline` (implicit and plain inline ones are emitted out of line).
- **NParticleEngine::CParticleSystem::ScaleParticleDimensions** (C2DVector&) / (long&) (0x00AF8F00, 0x00B05B80): nested
  `MAX(MAX(x,y),z)` macro (the inner MAX re-evaluated on the taken arm), C2DVector `operator*=` / `*= (long)`.
- **CSpecialAbilitiesBulletTimeDef / PhysicalShieldDef** dtors (0x004575FC, 0x0045784B): `optimize("s")`, `__forceinline`
  vector<float> dtors in reverse order, base dtor 0x00430300 as tail jump (base modelled without vptr like 0x00459ABE).

## Parked for Fable 5.1 (9)
- 009b00d0 CParentDefClassBase::AddSubDef DIFFER(163v155): placement copy of the pair reads `second` back from the frame (`mov edx,[esp+14h] ; mov [ecx+4],edx` x2) where retail stores edi/ebx | tried: field-wise pair, pair ctor from CSubDefInfo temp, Add(key, def, parent) with CSubDefInfo ctor, memberwise CSubDefInfo copy ctor | next: pair built with a by-value ctor of three scalars, or `make_pair(key, CSubDefInfo(d, p))` passed by value
- 009b0170 CParentDefClassBase::RemoveSubDef DIFFER(140v138): key and random_access_iterator_tag both homed in the dead `name` slot by retail; lane opens a `push ecx` word for the key | tried: key local, by-value inline Remove, const& Remove | next: lane gap: several short-lived locals sharing one dead argument slot
- 009b9700 CParsedItem::InitAsInteger DIFFER(91v103): retail threads the zero-length path (own `mov [ecx],3 ; ret` exit, `xor bl,bl` with edx as shared zero/index) where the lane keeps one tail with `cmp bl,1`; `[edx+edi+1]` operand order | tried: bool / unsigned char flag, preloaded char with `text[i+1]`, declaration order | next: an explicit `if (length == 0) { Type = 3; return; }` guard or a do/while form
- 0054e980 NUISystem::CScrollingViewport::RemoveChild (slot 89) DIFFER(131v126): retail homes find()'s random_access_iterator_tag in the `child` argument slot; lane opens a `push ecx` byte, flipping esi/edi | tried: plain erase, __copy_trivial helper with Clear inside the found branch | next: lane gap: empty tag temporary at an existing argument slot
- 00aee8c0 CParticleEmitter::Kill DIFFER(126v117): retail `or byte ptr [esi+40h],20h` before the zero stores and `and byte ptr [esi+41h],0FEh` after; lane loads both bytes into cl/al and stores at the end (ushort group: one merged word RMW) | tried: ushort bit-fields, inline setters, unsigned char units, optimize("s") | next: bits through a byte-typed flags view (`m_flagBytes[0] |= 0x20`)
- 00ae2cb0 CParticleProgram::SimulationStep DIFFER(107v102): retail `push ecx ; fstp [esp]` straight into the argument AND re-reads the vtable for the second virtual call; nested call caches the vtable in ebp (102v102), named float local spills via [esp+10h] | tried: nested, named local, inline forwarding wrapper, __forceinline inner accessor | next: lane gap candidate (virtual-call designator evaluated before the arguments)
- 00ae2d20 CParticleProgram::SimulationStep (mesh overload) DIFFER(102v97): same as 0x00AE2CB0 | tried: nested, named local, static forwarding helper | next: as 0x00AE2CB0
- 00af5010 NParticleEngine::CParticleSystem::AllocateParticle DIFFER(127v128): retail pushes esi at entry; lane delays `push esi` into the append block, moving the null-result exits | tried: return 0 vs guard, STLport link order vs prev local | next: lane gap: callee-saved push delayed into a branch
- 00af56e0 NParticleEngine::CParticleSystem::DeleteAllParticles DIFFER(138v138): loop-bottom store of the frame iterator `mov [esp+0Ch],edi` (retail) vs `mov [esp+0Ch],ebx` (lane); all other bytes match | tried: iterator by ref / const ref, by-value wrapper, separate `current` frame iterator | next: register-choice artefact; try `current = Particles.erase(it); it = current;` ordering

## Skipped
- 009b98b0 CParsedItem::ConvertToString: the row holds two functions -- the jump-table body (byte-identical through
  0x9B992F, source kept in differ/009b98b0.cpp) plus a separate 16-aligned `ret 4` stub at 0x9B9930.

## Label corrections
- 0x0046F4F0 = CSkyDef::operator= (slot 19 of 0x0123353C); 0x004E46CD = CJackOfBladesBattleDef::operator= (0x01242E3C).
- 0x0054E5D0 = NUISystem::CViewport::GetDataFromDef twin (callers 0x0054EA54/0x0054EAB4 in CViewport ctor 0x0054EA00), not CParticleEffect.
- 0x005E21F0 = SGI __pop_heap<pair<unsigned long, CAnimationEntry>>; 0x005E0E20 is that pair's operator=.
- 0x005195B0 = CLevelLoader::CObject::~CObject (0x0050F980 ~CLevelFiles; deleting dtor 0x0051A3D0).
- 0x0054E980 = NUISystem::CScrollingViewport vtable 0x012468D4 slot 89 (remove child), not CParticleEmitter.
- 0x00CBD510 = dtor and 0x00CB8110 = ctor of a CBase-derived class with vtable 0x012C1648 (no RTTI); 0x00CBD4F0 its ??_G.
- 0x00E299F0 = ctor of the class with vtable 0x012E22C8 (derives from the above; factory 0x00E2AB00), not CHeroMorphDef.
- 0x00C29C30 = Dinkumware vector<unsigned long>::push_back (not CParticleProgram::AddTextureCoord).
- 0x0066DC50 = a CRegionMinimap member (create/build), not CParticleSimulation::Clear; caller 0x00829FBB (CMiniMapDisplay).
- 0x00CB7210 = STLport vector<24-byte record>::push_back.
- 0x004575FC = ~CSpecialAbilitiesBulletTimeDef, 0x0045784B = ~CSpecialAbilitiesPhysicalShieldDef (not ~CPatchTesselationEdgeStrip).
- Callees: 0x009E5250 CSymbolMap::Clear, 0x009E5C30 its finalise (labelled ProcessButtonBReleased), 0x00AF1280 the data bank's
  emitter-info getter (labelled GetCategoryWithIndex), 0x00AEAB10 / 0x00AEAAF0 particle-engine global getters (labelled
  GFGetPreMainMemoryUsed), 0x00CBB090 a list clear (labelled remove_if), 0x00C294B0 _Uninitialized_fill_n (labelled _Fill_n),
  0x009E5200 CSymbolMap ctor (labelled CParticleUpdateParameters).
