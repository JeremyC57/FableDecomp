# Hand chunk 115 (CV_MurderTwistFinalScript .. CViewport): 36 of 40 byte-exact

All 36 src/ rows grade MATCH/RELOCATION_MATCH under gall.sh (grades.txt) and every test prints PASS under g++ (tall.sh)
and the lane's VC7.1 cl.exe (tall.py). 4 rows parked in differ/ after 3-4 grading attempts each; 0 skipped.

## What matched (36)
- **CVersion** (009ea080): `CVersion(uchar minor, uchar major, long build, long rev)` -- the init list in declaration order
  stores +0 = arg2, +1 = arg1; the two bytes are loaded before the stores.
- **CVertexShaderData family** (00a62fc0 CVertexShaderData, 00aab810 CActiveSoundInfo, 00b0f310 CParticleUpdateParamDesc,
  00bdaf80 CShaderConstantRange, 00b29720 CEngineLayerRenderer, 00b98bc0 CEngineSubPrimitiveRepeatedStaticMesh): constructor
  shells over CBase (0x0099A2D0/0x0099A2F0). SHAPE: the word stored at +4 BEFORE the vtable store is an argument-carrying
  `__declspec(novtable)` intermediate base; words stored after it are the derived init list. Three zero words after the
  vtable (00b29720) must be a 12-byte vector member with an inline zeroing constructor (plain init-list zeros hoist the
  `xor` above the vtable store); when EVERY store precedes the vtable (00b98bc0) the whole init list belongs to the
  novtable base and the derived class only adds its vtable. `ret 14h` with three reads = five parameters, two unused.
- **CPixelShader / CVertexShader `??_G`** (0098c3d0, 009c1ae0): `~T() { D* d = Data; if (d) { if (--d->m_refCount == 0)
  delete d; } }` over `CShaderBase { virtual ~CShaderBase() {} }` (vtable 0x0129A030), `__forceinline virtual` destructor
  and a `new T` factory. SHAPE: the local copy gives `dec [ecx+8] ; jne` (the member form re-reads Data), plain `delete`
  adds no second null test, the base's inline destructor stores its vtable before the free. Count at +8 (pixel) / +0x10
  (vertex) data objects. Both rows are the SCALAR deleting destructor (`test byte ptr [esp+8],1 ; ret 4`).
- **CVertexShaderData `??_G`** (00a630d0): `if (m_pDeclaration) m_pDeclaration->Release(); if (m_pShader) ...` through
  IUnknown slot 2 (`virtual __stdcall`, `this` pushed), out-of-line `~CBase` (0x0099A300), `__forceinline virtual`.
- **CVideoSys** (9 rows, engine/CVideoSys.h layout, no vptr): COM interfaces as `virtual __stdcall` vtables in IDispatch
  slot order. WaitForState = `do { GetState(10, &s); ++i; } while (state != s && i < 100)`; Pause/Stop = `unsigned char`
  results (`xor al,al`, no `xor eax,eax`), `State = 2/3`; GetLength/GetCurTime = `double t = 0.0` (the qword constant
  0x0122ED70, not `fldz`), `return (float)t` without narrowing; IsFinished inlines Stop (defined ABOVE it in the same TU,
  its `State == 0` guard folds to `test eax,eax`) under `GetProgress() >= 1.0f` (`test ah,1 ; jne`; `!(p < 1.0f)` gives
  `test ah,5 ; jnp`); SetVolume = `if (v < 0.001f) v = 0.001f; put_Volume((long)(log(v) * 500.0f))` with `#pragma
  intrinsic(log)` (`fldln2 ; fxch ; fyl2x`, __ftol2); ~CVideoSys = `if (p) { delete p; p = 0; }` + `CoUninitialize()`
  (dllimport) + the inlined `~CMovieSubtitles` (CWideString member dtor, list `Clear()` then `if (head) free(head)`);
  the constructor's init list is split around the Subtitles member constructor (+0x18/+0x1c before, +0x5c.. after) with
  the interface pointers, PlayMode = 1, State, RenderVideo as BODY assignments and `CoInitialize(0)` last. Retail
  CMovieSubtitles is 0x2c bytes (PDB 0x34): members after +0x30 sit 8 bytes below the header.
- **NDisplayView::CView3DThirdPerson** (0069e940): the whole body is the inline CView3DBase constructor (vtable
  0x0125C7E0, zero init list, body `m_pCameraManager = GetDisplayEngine()->m_2c` where GetDisplayEngine is a `__thiscall`
  member -- ecx is still `this`, so no reload), then this class's vtable.
- **CViewBase::CalculateDisplayDistance** (00681980): `m_distance >= 0.0f` (`test ah,1`), the thing pointer in a NAMED
  local (edi) and the opinion manager chain in a second local before the call (the chained form pushes before the call).
- **NGameText::CDataBank::LookupValue** (009cc410): the wave-24p map-find ternary `(it == End() || key < it->key) ? End()
  : it` in an inline Find taking the key by REFERENCE (a by-value Find parameter is copied back into its own slot),
  then `if (it != End()) return it->value; return 0;` (the `== ; return 0` polarity swaps the tails).
- **NUISystem::CViewport** (0053fcd0 AddFakeChild, 0054ea00 ctor): landed CParticleEffect (0x00544010) base model --
  three vtables after the CChangingStateComponent base; CEnginePrimitiveHandle member (out-of-line CBase + vtable
  0x0122F598 + null), 12-byte vector member, plain word, body call; `push_back(const T&)` takes the argument slot's
  address and `child->SetParent(this)` is slot 90.
- **CVertexBufferWin32** (00a63200 Uninitialise, 00a63240 DoLock): `CSystemManager* system = GFGetSystemManager();`
  then the `system->PDisplayManager->PRenderManager->Notify(m_pBuffer, 1.0f)` chain (the call in a local, the two
  derefs after the pushes); DoLock(flags, offset, size) with `void* data = 0` homed in the dead `size` slot and
  `Lock(offset, size, &data, flags)` = D3D vertex-buffer slot 11.
- **Quest-script constructors** (00d2f6b0 CVtable_012cc60c, 00ec53c0 CVtable_012f17dc, 00df5c90 CV_SwordInTheStoneScript):
  landed 0x00CE1A00 model -- CScriptBase (0x00CB8110) under a `novtable` CScriptObject base holding the data/interface
  pair (+0x44 = arg1, +0x40 = arg2) stored before the vtable. Member handles (CScriptGameResourceObjectScriptedThingBase:
  0x0099A380 + vtable 0x0127094C; CScriptThing: 0x0099A2D0 + 0x01238C8C) need their two counted-pointer words in an
  EMBEDDED {Data, Info} member object with an inline constructor; plain `PImp_Data(0), PImp_Info(0)` initialisers let the
  lane hoist the `xor` (and even the stores) above the member's vtable store. Likewise the three zeroed words of 00df5c90
  are an embedded state struct. The timer at +0x6c is `g_pGameScriptInterface->RegisterTimer()` (slot 87) in the body.
- **CVtable_012efee8 helper ctor** (00eb2000): base `CVtable_012c3224(const CScriptThing& thing, void* iface)` called
  with `(thing, owner->m_pInterface)` -- the interface argument is evaluated first (right to left), so its load precedes
  the thing reference; `m_pInterface` and `m_pOwner` are plain initialisers (stored before their class's vtable), the
  index at +0x18 is a body assignment (stored after the last member constructor); the name sits 8 bytes after the handle.
- **CV_SwordInTheStoneScript::Init** (00eb6a00): `m_things.resize(2, CScriptThing()); m_objects.resize(2, T());` --
  explicit temporaries reuse the constructor's returned pointer (`push eax`), share one 16-byte frame slot and are
  destroyed right after each resize.
- **Script watchers** (00ed57e0, 00edbc30): `while (!IsActiveThreadTerminating()) { ...; m_pInterface->YieldFrame(); }`
  gives the rotated loop (entry test, aligned head, bottom `je`); an inner `if (IsActiveThreadTerminating()) return;`
  jumps to the shared exit; the 0.2f wait is `push 3E4CCCCDh` to slot 376.
- **ResetCrowdSwordTimer** (00ed72f0): `long seconds = rand() % 6 + 10;` BEFORE the global's virtual call (otherwise the
  lane hoists the global and its vtable above the rand call).
- **GetQuestIndex** (00ed7c70): a by-value CCharString parameter (user copy constructor, destroyed by the callee on each
  exit) compared with `!(entry.m_name != name)` (0x0099E900 is operator!=, `!(a == b)` with the sete tail); `if (i == 12)
  { if (IsActiveThreadTerminating()) return -1; i = -1; } return i;` keeps the two -1 exits.
- **HasHeroEnflamedEntity** (00eb07d0): implicit conversion temporary `IsEnflamedBy(3, "SCRIPT_NAME_HERO")` (re-addressed
  with `lea`, destroyed at the end of the full expression); the result in bl and `if (r) return !T(); T(); return 0;`
  -- the `if (T()) return false; return r;` form is folded to `not ; and` by the lane.

## Parked for Fable 5.1 (4)
- 00ac7940 CVertexPairContraction::RemapIndex DIFFER(76v75): lane `mov eax,[ecx+4] ; mov esi,[ecx+8] ; sub esi,eax` (m_pFirst register-cached for the entry size test) and `cmp [eax+edx*8],esi` on the first field with the second loaded into ebx; retail `mov eax,[ecx+8] ; sub eax,[ecx+4]`, loop head `mov eax,[ecx+4] ; mov ebx,[eax+edx*8] ; cmp ebx,esi ; lea eax,[eax+edx*8]` then `cmp [eax+4],esi` | tried: `m_pFirst + i` pointer, `*(Begin() + i)` reference (best), `volatile` m_pFirst (79), STLport operator[] per access (79) | next: lane gap: loop-invariant vector-first load scheduled before the size test; try `for (SIndexPair* p = Begin(); p != Begin() + Size(); ...)` or an index loop with Size() in a `long` local
- 00ad9470 CVertexGroupBlock::RemoveLast DIFFER(84v84): key `sub eax,4` for retail `add eax,-4`; hidden-result pointer `lea ecx ; push ecx` for retail `lea eax ; push eax`; after equal_range the lane reads `second` into edx and reserves BOTH iterator slots (`push ecx ; mov eax,esp ; push ecx ; mov [eax],edx`) before filling, retail reads `first` into edx, builds the `second` slot right after its `push ecx`, then the `first` slot | tried: key by const& (copied into the frame, 92), key by value, `&m_pLast[-1]` | next: lane gap: by-value 4-byte iterator pair argument build order; try erase taking the iterators by `const CNodeIterator&` plus an inline wrapper, or a pair type without user constructors
- 00b93eb0 CEngineSceneGrid::CEngineSceneGrid DIFFER(80v80): register colouring only -- `mov eax,1 ; shl eax,cl` / `lea edx,[esi+14h]` where retail has `mov edx,1 ; shl edx,cl` / `lea eax,[esi+14h]`; every other instruction and its order agrees | tried: init-list `m_side(1 << shift)` + `resize(m_side * m_side, 0)` (re-reads the member), `long`/`unsigned long` local side with the body store (best), vector(n, value) init-list constructor (76) | next: lane gap: register choice; try `m_cells.resize(side * side, 0)` with `side` declared before the init list via a helper, or `#pragma optimize("s", on)`
- 00de19f0 CVictimsFather::Init DIFFER(94v94): the lane hoists `lea edi,[esi+8]` (the &m_father of the two later pushes) between the by-value temporary's two vtable stores and copies the CScriptThing through it (`mov ecx,[edi+4] ; mov ecx,[edi+8]`); retail reads `[esi+0Ch]` / `[esi+10h]` and forms the lea right before the second call | tried: `&m_father` for both later calls, `CScriptThing&` parameters, a `CScriptThing* father = &m_father` local declared after the first call | next: lane gap: address hoisting across an inlined copy constructor; try an inline accessor returning the copy by value for the first call, or the two callbacks through a `__forceinline` helper taking `this`

## Skipped
- none (no row holds a second function body).

## Label corrections
- 00aab810 is CActiveSoundInfo::CActiveSoundInfo (vtable 0x0129E674), 00b0f310 CParticleUpdateParamDesc (0x0125FEE4),
  00bdaf80 CShaderConstantRange (0x0129A04C), 00b29720 CEngineLayerRenderer (0x012A0F04), 00b98bc0
  CEngineSubPrimitiveRepeatedStaticMesh (0x012A3E94) -- all carry the propagated "CVertexShaderData::CVertexShaderData".
- 0098c3d0 is CPixelShader's scalar deleting destructor (vtable 0x0129A054), not CVertexShader; 009c1ae0 and 00a630d0 are
  SCALAR (`??_G`) deleting destructors, not `vector_deleting_destructor'.
- 00b93eb0 is CEngineSceneGrid::CEngineSceneGrid (vtable 0x012A3E58), not CV_SingingStonesScript.
- 00d2f6b0 (vtable 0x012CC60C), 00ec53c0 (0x012F17DC) and 00eb2000 (0x012EFEE8, a 3-argument helper over base 0x012C3224)
  all carry "CV_MurderTwistFinalScript::CV_MurderTwistFinalScript" (five manifest rows): named CVtable_<va>.
- 009cc410 "CVectorMap::LookupValue" is a member of the engine text bank NGameText::CDataBank (the map at +0xd0).
- 0069e940 is NDisplayView::CView3DThirdPerson (vtables 0x0125C7E0 base / 0x0125C7F8); 0053fcd0 / 0054ea00 are
  NUISystem::CViewport (vtables 0x01249A5C/38/30).
- Callee labels: 0x0099E900 is CCharString::operator!= (not less<>::operator()), 0x005D8676 / 0x005D8561 are a tree's
  equal_range / erase(first, last) (not CompareDataOffset / CTCCarryable_RemoveAndDestroy), 0x009BDBA0 is the
  CMovieSubtitles constructor, 0x009BE060 a list clear, 0x006E7B40 / 0x007E72A0 the CScriptThing /
  CScriptGameResourceObjectScriptedThingBase default constructors (not CCarriedReadableDef), 0x004AA840 / 0x007E74D0 their
  destructors, 0x00D34AC0 / 0x00CD3CCB vector resize(n, const T&), 0x00A0AA70 a `ret 8` render-manager stub.
