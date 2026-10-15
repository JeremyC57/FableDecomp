# Hand chunk 156 (CRenderManagerCore .. CScriptGameResourceObjectScriptedThing): 30 of 40 byte-exact

## What matched (30)
- **CRenderManagerCore** (11): the four texture-array Draw overloads (0xA0AB50, 0xA0ABE0, 0xA0ACA0, 0xA0AEA0) are the landed
  four-argument tails behind an inlined `AttachTextureToStage` loop (landed 0x009A0CF0 body as an inline member); ctor
  0xA0AFE0 (out-of-line CRenderStateManager(ulong) base, DisplayManager from the argument stores before the vtable store,
  CPixelFormat/CSystemFont members, element loop over CurrentlySelectedTextures); the five transform setters
  0xA0B0A0/0xA0B140/0xA0B1E0/0xA0B280/0xA0B3E0 all go through an inline private `SetD3DMatrix(D3DMATRIX&, 16 floats)` (the PDB
  member) then SetTransform (slot 44); BeginRender 0xA0B480 needs the open-frame body as the fall-through arm and
  `Rendering = true` written BEFORE the last-frame copies (retail stores last-frame Z first, the other two after the flag).
- **CRenderStateManager** (2): ForceResendRenderStatesToHardware 0xA044E0 (switch on StateType, header members);
  ctor 0xA05970 (inline CStateInfo zeroing ctor for BookmarkDummyState, list sizes/InitFlags/StateType 4/mask 1 as body stores).
- **0x004FA860** (catalog CRigidBodyConfig::operator=): a pack(1) 0x10822-byte two-layer map's user `operator=` (members
  from +5 one by one, two implicit CLayer copies as `rep movsd`, trailing word zeroed).
- **CRuntimeScriptedEntityPersist::SaveToGameStateScript** 0xCB7A00: `out += "..." + CCharString(id) + "\r\n";`.
- **CGameCameraManager** (2, catalog CSACCombat::SetUpPhysics) 0x69E460 / 0x69E5D0: `SetCameraMode(CCountedPointer<CBase>(new
  CTrack/CSnapToPoint(this, def, AvailableCamPoints.front().GetPItem())))`; the counted-pointer raw ctor must be the if/else
  form re-reading Data (gives the three tail-duplicated exits) and the list front an STLport-style `*begin()` iterator (`lea`).
- **CSTS_BriarRose::Init** 0xDF1760: ten GSI calls (slot names from rtti-vtables + manifest), `*Game->GetHero()` nested.
- **Static handler registrar** 0x8162F0 (catalog CScreenEffectPriorityStack::Process): four `map.insert(value_type(id, &CThing::Hn))`.
- **CScreenEffectPriorityStack** (3): Reset 0x880A40 (chained `Alpha = Contrast = Brightness = 0`, track positions through an
  inline two-argument setter); Process 0x880E50 (three named ICustomProcessInterface callback locals); InsertDisplacementEntry
  0x881780 (inline push_back capturing the head BEFORE _Buynode; the returned key kept in esi while the set insert takes it from
  the dead `time` slot = by-value parameter of a `__forceinline` helper).
- **CScreenMessage** (2): ctor 0x438020 (stale pointer from a converted temporary, the temporary sits in the dead `text` slot);
  Update 0x438770 (agent153's lazy GUI-def helper, `if (n < 0) n = 0; else if (n > len) n = len;`).
- **STLport vector<CCountedPointer<T> >::erase** (2, catalog CScriptBase::RemovePredicate) 0x7F3E90 / 0x84F570: `__copy` with a
  `random_access_iterator_tag()` temporary -- here VC put the empty tag in the dead `position` argument slot by itself.
- **CScriptBase** (3): LoadEntityScript 0xCB7EE0 (catalog LoadOrCreatePersistInfo; inline find with `it != end() && key >=
  it->first` gives retail's jb-before-ja order; copy as a named local in its own block); SaveEntityScript 0xCB83D0 (landed
  0x4B5030 persist-context form); SaveGameState 0xCB8820.
- **CScriptGameResourceObjectScriptedThing::Converse** 0x903760: `Speak(other.GetScriptThing(), text, 0, true, true, false)` with
  the CScriptThing temporary's inline dtor (vptr, counted release, base CBase dtor).

## Parked for Fable 5.1 (10)
- 00a07900 CRenderStateManager::Init DIFFER(117v115): extra `mov ecx,esi` before the ForceResendRenderStatesToHardware call | tried: plain body | next: lane gap: callee clobber knowledge -- needs the real 8 KB call-free SetDefaultRenderStates (0xA059E0) defined above in the TU
- 009fc5f0 CResourceBank::CResourceBank DIFFER(155v156): first CResourceList copy -- retail hoists the CResource vtable store next to the two outer vtable stores and zeroes the head via esi before `lea eax,[esi+8]` | tried: count as member init, in CResource body, in the list body, as an inline base | next: separate inline CResourceList ctor shapes per member, or a CResource base ctor + body mix
- 009fc170 CResourceList::UnloadNextResource DIFFER(155v149): after ListRemove retail uses ecx/edx (`mov ecx,eax ; mov [ecx+10h],esi`), `inc dword ptr [edi+38h]` and a separate return-true epilogue | tried: ListInsert forms, optimize("s"), if/else return | next: define the real ListRemove 0x9FBFB0 body above under auto_inline(off) (callee clobber knowledge)
- 007767a0 CMatrix3x4::Set(const CRightHandedSet&, const C3DVector&) DIFFER(127v127): `fstp [ecx] ; mov [ecx+8],edx ; fstp [ecx+4]` vs ours storing the spilled z first | tried: implicit copy, user operator=, user copy ctor, operator^ cross product (products now match) | next: Set(x,y,z)-style inline assignment of the right vector, or explicit float members
- 00c404f0 COrientedEngineObject::SetOrientationVectors DIFFER(155v147): `or dword ptr [ecx+48h],20000h` vs `mov eax,[ecx+48h] ; or eax ; mov [ecx+48h],eax` (x2) | tried: plain word `|=`, 1-bit field, optimize("s") (+__forceinline compares) | next: lane gap? (memory-operand `or` in a speed TU; check other landed `or dword ptr [m],imm` speed rows)
- 00a87260 CSB::ShouldIgnoreAgent DIFFER(133v137): retail `mov cl,[esi+61h] ; test cl,cl ; jne ; test al,al ; je` re-tests the loaded +0x60 flag; ours folds the second test | tried: inline helper (__forceinline), one && expression, bool local | next: a non-folded helper returning bool by value (e.g. member function with `bool` result tested by the caller) or a volatile-free second read through another accessor
- 00d89160 CQ_HeroSoulsNostroScript::CSUMMONED_CREATURE::OnPredicateFail DIFFER(107v107): retail loads the def pointer 0x143E90C into edi before each GetTimer call and forms the timer as `add eax,54h ; mov eax,[eax]`; ours reads the def after the call | tried: operands swapped, `CScriptDef* const`, const-long& timer helpers | next: a CTimer helper taking the def by reference (`IsBelow(const CScriptDef&)`), or the comparison inside an inline on the parent script
- 004381b0 CScreenMessage::IsHardCoded DIFFER(118v117): frame slots of the "(HC)" marker and the converted text swapped, text re-read from the frame instead of the returned pointer | tried: temporary in the call, named local, const& binding, inline StartsWith (not inlined) | next: `__forceinline bool StartsWith(const CCharString&)` on the conversion temporary
- 00cb7c40 CScriptBase::RunSpawnedFunctions DIFFER(107v107): register allocation only (this ebx vs ebp, zero ebp vs ebx; count-block reload eax vs ecx) | tried: operator delete vs delete | next: lane gap: register allocation (scheduling-only)
- 00cb88b0 CScriptBase::OnScriptedEntityDeactivated DIFFER(131v128): retail keeps the re-read key in edx/ebx and tests the bool with `cmp byte ptr [esp+1Ch],0`; ours ecx/edx + `mov esi,ecx` and `mov bl ; test bl` | tried: find(const&), __forceinline by-value find, open-coded LowerBound + const copy, const copy after find | next: register allocation; maybe the key compare inside a CKeyPairCompareLess functor

## Skipped
- none (no row holds two functions or runs into a catch funclet).

## Label corrections
- 0x00A0B140 is `CRenderManagerCore::SetWorldTransform(const CMatrix3x4&)` (D3DTS_WORLD 0x100), not SetViewTransform.
- 0x007767A0 is not CRightHandedSet::GetRight (that is out of line at 0x004A67D0 in Ego_r); it is a 48-byte matrix setter built from a CRightHandedSet + position (`ret 8`, no direct callers): right = Forward ^ Up, then Forward, Up, position.
- 0x00C404F0 is not a CRightHandedSet member: `this` embeds Up/Forward at +0x24/+0x30 with a dirty word at +0x48 (callers 0xC3395F, 0xC4073E).
- 0x004FA860 is not CRigidBodyConfig::operator= (0xB8 bytes with a vector): it copies a pack(1) 0x10822-byte object holding two CLayer (0x8400) members, cloned by the factory at 0x0051C2F0.
- 0x0069E460 / 0x0069E5D0 are CGameCameraManager members (call SetCameraMode 0x697E20 on `this`), not CSACCombat::SetUpPhysics; they construct NCameraModes::CTrack (ctor 0x69DF80, vtable 0x125C50C, 0xCC bytes) and NCameraModes::CSnapToPoint (ctor 0x56E330, vtable 0x124DE5C, 0xD0 bytes); 0x69DF80 is labelled CSnapToPoint in the manifest but is CTrack's ctor.
- 0x008162F0 is a static registrar called from Init_RegisterGameComponents (0x4EE286), not CScreenEffectPriorityStack::Process; 0x818000 is the map's `_Tree::insert(const value_type&)`; 0x8F5CF0 is `CThing::Kill(true)` wrapper.
- 0x007F3E90 / 0x0084F570 are STLport `vector<CCountedPointer<T> >::erase(iterator)`, not CScriptBase::RemovePredicate; 0x7F3CE0 / 0x84F330 are STLport `__copy` (random-access, `__fastcall`), manifest "_Move".
- 0x00CB7EE0 is `CScriptBase::LoadEntityScript(unsigned __int64, CActiveEntityScriptBase&)` (catalog LoadOrCreatePersistInfo).
- 0x00D89160 is `CQ_HeroSoulsNostroScript::CSUMMONED_CREATURE::OnPredicateFail` (UndeadAlive/NostroVulnerableTimer at parent +0x50/+0x54), not the arena script's.
- 0x0099A2F0 is CBaseClassNonCopyable's ctor (stores 0x129A7C4), not CBase::CBase; 0x00A079B0 is CDeviceState's ctor (labelled CPSCOrbit); 0x00F359F0 (labelled CTCPhysicsNavigator::EnableNavigator) is used here as the active entity script's +0x28 bool setter (byte twin); 0x0099B220 is called on a CWideString (labelled CCharString::GetLength).
