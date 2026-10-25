# Hand chunk 456 (pair<CCharString,CSymbolMap .. pair<unsigned_long): 40 of 40 byte-exact

All 40 rows grade MATCH (4) or RELOCATION_MATCH (36) and every test prints `<addr>_TEST PASS` under g++ (tall.sh) and VC7.1 (tall.py).
Most catalog "pair<...>" labels in this chunk were propagated across byte-identical bodies; 29 of the 40 rows are a
different function or a different element type than the catalog says (see Label corrections). Identities come from the PDB name sets, Ego_r.pdb layouts, RTTI of the stored vtables and callers.

## What matched (40)

- **STLport pair, copy / value / converting constructors** (0076dd40, 004b7490, 0057ed6c, 0057f30a, 00748570, 0080de90,
  00a20240, 00a20670, 00cb90f0, 00af67b0): STLport 4.6 `pair` is a `struct` with a memberwise user copy constructor, the
  member-template converting constructor and `pair() : first(_T1()), second(_T2())`; emit exactly one with an explicit
  instantiation of that member (`template pair<A,B>::pair(const pair<C,D>&);`). 0057ed6c/0057f30a need
  `#pragma optimize("s", on)` (odd addresses, `push 11h ; pop ecx ; rep movsd` for the 0x44-byte status). 00a20240 is the
  converting ctor `pair<const long, CAStarState>(const pair<long, CAStarState>&)` (map::insert(make_pair(..))), 00a20670
  its byte twin, the copy ctor; CAStarState's user copy ctor counts states into 0x013D27B8/BC/C0.
- **Compiler-generated assignments** (009e6380, 004b7650, 0059308a, 005f853e, 00a85700, 006ba0d0 CFaction, 00f27190
  CHeroSoulsCreatureDef): STLport pair declares no `operator=`; taking its address in a global member pointer
  (`T& (T::*g_p)(const T&) = &T::operator=;`) makes VC7.1 emit the implicit `??4` out of line. 0059308a/005f853e (/Os) need
  `__forceinline` on CDefPointer's assignment and on a `Release()` that lives on the def (one load of the pointer, then
  `dec [ecx+4] ; jne`). 006ba0d0's set assignment inlines over a declared `_Rb_tree::operator=` 0x006B9E00.
- **Record copy constructors** (00f27060 CHeroSoulsCreatureDef, 009cb8f0 CBankCreatorException, 00a85010 CNavigationLayer,
  006e4900 CActiveDialogueInfo, 0082c370 CMarkerInfo, 008126b0 CGroupMemberData, 007229d0 CGenerationPos, 0078e600 /
  00793b60 PerEffectInfo, 006e6b60 CScriptConversation): memberwise user copy ctor written out of line; the intelligent
  pointer's copy is `CBaseIntelligentPointer()` 0x00A01B10, own vtable store, `SetPItem(other.GetPItem())`; C3DVector with
  a memberwise copy ctor gives the three interleaved dword moves; CArray is an empty class over `std::vector`.
- **STLport heap/algorithm helpers** (005e1b60, 006bedc0 `__pop_heap`; 00769cb0 `swap`): STLport 4.6 bodies as `__fastcall`
  templates in `std`, explicitly instantiated. 005e1b60's CIVCountedPointer assignment must read both pointers into locals
  before comparing (`cmp ecx,eax`); 006bedc0 inlines the entry's implicit assignment (CWideString operator= x2, then the
  floats and the flag) and builds the by-value copy in the argument area; 00769cb0's temporary stores the
  CParticleUpdateParamDesc vtable (the vptr is never copied) and dies through ~CBaseClass 0x0099A2E0.
- **Constructors / destructor** (0041a1a0 GetDebugSaveDir is a `__fastcall` free function returning through the RVO pointer;
  00ac6ce0 CQuadric::Clear: each `d = 0.0` reloads `fld [__real@0]`; 00a0e0a0 CJoystickPositionTracker: deque map as
  `_STLP_alloc_proxy` fixes the `push 0` placement, body `resize(30, C2DVector(0,0))`; 00b8fbf0 CDefaultEngineMemoryInit:
  stores in retail order, HiresTextureMemory last; 00c6b9a0 CStateVariableFilter (libpk, descriptive): zero the targets then
  `m_current = m_target` (the inlined struct copy gives the copied-this/copied-zero registers); 00bca0d0 / 00bca4d0 the
  dynamic resource manager (explicit ctor instantiation) and CEngineVertexBufferPool; 00739e20 ~CModeBase: empty virtual user
  dtor, CAnimFilterOfGroup's implicit dtor does not re-store its vtable, CAnimFilterBase's inline user dtor does; 009ea150
  CGenericVar(): string member first, `Type = 0` in the body; 009f9990 CProfileTreeEntry(): `CProfile() : Manager(0)`
  plus the STLport `_Vector_base` default ctor).

## Parked for Fable 5.1 (0)

## Skipped

None.

## Label corrections

Propagated catalog labels found wrong (new name from the PDB name sets / Ego_r.pdb layout / RTTI / callers):
- 00f27060 / 00f27190 "pair<CCharString,CSymbolMap::CSymbolInfo>" -> CHeroSoulsCreatureDef copy ctor / operator= (vtable 0x012FB500).
- 0041a1a0 "pair<CCharString,CTCBase::CRegisteredTCInfo>" -> NResourceDirectoryNames::GetDebugSaveDir.
- 0076dd40 "pair<CCharString,CTCBase::..>" -> pair<CCharString, CCharString> value ctor.
- 009cb8f0 "pair<CCharString,CTCBase::..>" -> CBankCreatorException::CBankCreatorException(const CCharString&, const CCharString&).
- 006ba0d0 "pair<CCharString,..CPersistInfo>" -> CFaction::operator=.
- 00a85010 / 00a85700 "pair<CCharString,..CPersistInfo>" -> CNavQuadTree::CNavigationLayer copy ctor / operator=.
- 006e4900 "pair<CIntelligentPointer<CThing>,long>" -> NSpeechGainManager::CActiveDialogueInfo copy ctor.
- 00748570 -> pair<CIntelligentPointer<const CThing>, NStealth::CDangerStatus> copy ctor.
- 008126b0 -> CGroupMemberData copy ctor; 0082c370 -> CMiniMapDisplay::CMarkerInfo copy ctor.
- 00ac6ce0 "pair<EBodyLocation,CWoundMorphs::CEntry>" -> QuadricOptimiserInternals::CQuadric::Clear.
- 00a0e0a0 "pair<EHeroMorphType,..>" -> CJoystickPositionTracker ctor; 00b8fbf0 -> CDefaultEngineMemoryInit ctor;
  00c6b9a0 -> libpk CStateVariableFilter ctor (descriptive name: libpk has no PDB/RTTI).
- 00bca0d0 / 00bca4d0 "pair<EPSCType,CComponentInstance>" -> CEngineDynamic3DResourceManager<CEngineDynamic3DResource<CVertexBufferWin32,3> > ctor
  / CEngineVertexBufferPool(long) ctor.
- 005e1b60 "pair<long,..>::operator=" -> std::__pop_heap over pair<EAnimComponent, CIVCountedPointer<CAnimComponentBase> >.
- 005f853e -> pair<long, CDefPointer<const CHeroAbilityDef> >::operator= (twin of 0059308a).
- 007229d0 / 0078e600 / 00793b60 "pair<long,CIntelligentPointer<CThing> >" -> CTCSmokeGenerator::CGenerationPos /
  CTCDivineWrath::PerEffectInfo / CTCUnholyPower::PerEffectInfo copy ctors.
- 006bedc0 "pair<long,CMusicManager::CRegisteredMusicEntry>" -> std::__pop_heap over that element.
- 006e6b60 "pair<long,CMusicManager::..>" -> CScriptConversation copy ctor.
- 00769cb0 "pair<pair<long,long>,CEngineTextureDecal>" -> std::swap of pair<unsigned long, NParticleEngine::CParticleUpdateParamDesc<ul,ul> >.
- 00cb90f0 -> pair<unsigned __int64, NScript::CRuntimeScriptedEntityPersist::CPersistInfo> copy ctor.
- 00739e20 "pair<unsigned_long,C3DMeshVolume>" -> CTCCreatureModeManager::CModeBase::~CModeBase; 009ea150 -> CGenericVar::CGenericVar();
  009f9990 -> CProfileManager::CProfileTreeEntry::CProfileTreeEntry().
- Callee labels: 0x00406780 is CUserProfileManager::GetDebugSaveDir (not ConvertPathToFullPath); 0x00431F10 is
  CDefClassBase::operator=; 0x0046644C is CDefClassBase's copy ctor; 0x0042D9E0 (labelled CPSCBase in the landed
  0x00F27650 source) is CDefClassBase's ctor.
- Already landed: 0x00F27650 "CFireheartMinigameDef" ctor is CHeroSoulsCreatureDef's default constructor (same vtable 0x012FB500).
