# Hand chunk 133 (CDrawTargetInfo .. CEngineInternalPrimitive2DStaticMesh): 33 of 40 byte-exact

All 33 graded MATCH / RELOCATION_MATCH (grades.txt) and PASS under g++ (tall.sh) and the lane's VC7.1 (tall.py).
Many chunk labels are propagated names; the corrected identities are listed under "Label corrections" and carried in
each source's line 1 (module/name) with a `// Leaf:` line wherever the symbol differs from the chunk name.

## What matched (33)
- **NPlayerGui::CDrawTargetInfo** -- 00644526 OnEventAttack: restart the attack/block alpha fades (GUI-def duration, 0.0f, opaque black). Size TU
  (`#pragma optimize("s", on)`); the by-value CRGBColour is built in its argument slot only with a USER copy constructor, and its
  `+2,+1,+0,+3` stores are body assignments R,G,B,A over a B,G,R,A layout (an initialiser list stores in declaration order).
- **CDriveManager** -- 009e4ae0 DriveExists: `_wchdir("X:\")`, restore the working directory, 1 / 3. `/GS` (security cookie); the success
  path must be the `if` body (`if (_wchdir(root) == 0) {...; return 1;} return 3;`), the early-return form inverts the blocks.
- **NUISystem::CDynamicList** -- 00540e50 / 00540ec0 ScrollSelectionUp/Down: STLport `*(begin()+n)` element access, members 0x30 below
  the PDB (first try).
- **NUISystem::CFrontEndButton** (chunk: CDynamicList) -- 0054e0b0 constructor: CClickable base, three vtables, two STLport vectors with
  `_STLP_alloc_proxy` end-of-storage members, the list sentinel linked through a LOCAL before it is stored, then Initialize().
- **CEM_Rhodri** -- 00e76540 Init: four flags + six game-interface calls; the CBriarRose::Init model (implicit CScriptThing copy in the
  argument area) matched first try.
- **CActiveQuest** (chunk: CEditFractal) -- 004b9b90 destructor: empty out-of-line `~CActiveQuest()`, the members' inline destructors
  (CCountedPointer release re-reading the block, STLport vector free, CCharString) in reverse order.
- **CSoundBankRecord** (descriptive; chunk: CEditFractal) -- 00992510 destructor: four strings, two counted pointers (same shape).
- **C3DAnimationInfo::CAnimationData** (chunk: CEditWorld) -- 00a26860 destructor: vtable store, `AnimInfo = 0` body, two counted pointers,
  the inlined STLport map destructor (clear() + header free) and the base destructor tail jump.
- **SGI heap algorithms over pair<unsigned __int64, CPersistInfo>** (chunk: CEditPrimitiveStats) -- 00cba750 `__pop_heap`, 00cbbeb0
  `__partial_sort`: `__fastcall` free templates (the /Gr algorithm TUs), the pair's implicit copy/assignment, the inline
  CKeyPairCompareLess as an unsigned 64-bit `<` (landed 0x00A70280 model; first try).
- **CMusicManager** (chunk: CEngine) -- 006be300 constructor: CState/CDefPointer/CVectorMap member constructors, `{ CCharString name("MAIN_SOUND_SETUP", -1);
  Get().GetDef(name, PSoundDef); }` in its own scope, RegisterMusic(), `Paused = false` after it (first try).
- **HighestSetBit** (chunk: CEngine::GetScratchZBufferD24S8) -- 0099fdb0: `__fastcall` floor(log2); the parameter itself is incremented
  (`++value`, a separate `n = value + 1` gives `lea` and a late esi save).
- **CEngine** -- 00b24030 / 00b240c0 World/PausedWorldFrameUpdate (component loop re-reading end(), `(float)unsigned`, `1.0f / x` into a
  double), 00b25c00 RenderColorFilteredQuad (inlined CRenderStateManager capture-block push through a `long&` to StateStackSize),
  00b26010 OnPreDeviceReset (STLport clear() whose trivial copy must be `if (first == last) return result; ...memmove(...) + n` --
  the `n ? ... : result` form tests `je` on the difference). CEngine.h carries PDB offsets 8+ bytes too high: modelled locally.
- **CEnginePrimitiveRenderer** (chunk: CEngine) -- 00b321b0 CollectPrimitiveStats: enumerator in the frame, index loop over SceneList.
- **CEnginePrimitiveRenderer2D** (chunk: CEngine) -- 00b4a5f0 Set2DLighting: three POD colour assignments, LightVector then Normalise().
- **CShadowClipVolume** (chunk: CEngineCameraDesc) -- 00b36c50 copy constructor: the `mov ebx,8 ; dec ebx` element loop only comes from
  the IMPLICIT copy constructor (CPlane non-trivial through C3DVector's user copy constructor), emitted out of line for a placement-new
  copy site (`// Leaf: ??0CShadowClipVolume`); a body loop is unrolled by the lane, arrays cannot sit in an initialiser list.
- **CEngineCameraDesc** -- 00b4af50 copy constructor: C2DBoxF and C3DVector need user member-wise copy constructors (dword moves through
  one register); the POD CMatrix3x4 stays a `rep movsd` block.
- **CEngineClothSimulation*** -- 00b80ad0 ConvertVectorToLocal and 00b81230 CalcAndSetRunState: the FP term order only reproduces
  statement by statement (`x = a; x += b; x += c;`) -- one three-term expression is re-associated into a canonical order; 00b80fe0
  GetWindVector: ConvertVectorToLocal defined ABOVE under `#pragma auto_inline(off)` (keeps the result pointer in edx), ONE vector local
  reused for the position and the scaled wind, the wind read through the returned temporary (`GetWindAt(v).ScaleInto(v, 100.0f)`);
  00b81d60 ClearParticleSims: STLport push_back (placement copy + _M_insert_overflow with the `__false_type` tag) first try.
- **CKsObject** (descriptive; chunk: CEngineDebug::Draw3DSphere) -- 00c5e050 SetProperty: KSPROPERTY on the stack +
  `DeviceIoControl(h, IOCTL_KS_PROPERTY, ...)` through a dllimport; `!= 0` into a bool gives `neg ; sbb ; neg`.
- **CEngineInit** -- 0042d830 constructor (retail has eight pointers before the strings, PDB nine). **CRegion** (chunk: CEngineInit) --
  006bc410 constructor: STLport `_STLP_alloc_proxy` vectors fix the `lea ecx` placement; map head node built in place.
- **CEngineInlineTableBuilder** -- 00be2a80 constructor: 256-entry log2 byte table; the helper must be `__forceinline`.
- **CEngineInternalAttachedPrimitive** -- 00b38db0 / 00b38e30 RedoMatrixConversion (`m[i] *= literal`), 00b3a910 constructor (handles =
  root ctor + vtable + null; attachment info `Flags(0)` + two strings).
- **CCombatManager** (chunk: CEngineInternalPrimitive2DStaticMesh) -- 006eb9f0 HasSequenceAgainstAttacked (descriptive): two inline
  GetCombatantInfo(uid) lookups sharing one by-value uid slot; `return x >= 0 ? true : false;` for the bare `setge al`.

## Parked for Fable 5.1 (6)
- 00c17400 _global::DeleteTrackNodeCommand DIFFER(138v155): `push esi ; mov esi,[esp+24h] ; call 0x00C16CE0` -- the callee reads esi as an input | tried: one draft | next: lane gap: LTCG custom register argument (whole-program codegen)
- 00c26cd0 CMailboxQueue_00c26cd0::PostMessage DIFFER(115v115): `lea edx,[esp+1Ch] ; push edx` (lane ecx) and `push 1 ; lea ecx,[esi+18h] ; push eax` (lane lea after both pushes) | tried: push_back wrapper, insert(end(), 1, x) at the call site | next: lane gap: register/scheduling
- 00c27130 CMailboxQueue_00c27130::PostMessage DIFFER(115v115): same as 00c26cd0 (twin, other insert instantiation) | tried: same two forms | next: lane gap: register/scheduling
- 00af12a0 CEmitterData::BuildEmitterTemplate DIFFER(144v144): `mov ecx,[edi] ; lea eax,[esp+8] ; push eax` (lane: `lea ecx` + push first) and the count block reloaded into ecx (lane edx) | tried: `EmitterTemplate->ReadBinary(stream)`, object read into a local first | next: lane gap: register allocation
- 00b1aeb0 NParticleEngine::CPSCLight::~CPSCLight DIFFER(119v121): `mov ecx,[edi+5Ch] ; test ecx,ecx` hoisted above `lea esi,[edi+58h] ; mov [esi],vtable` (retail: after the store, through eax, then `mov ecx,eax`) | tried: plain member, volatile member + locals, __forceinline + volatile, __forceinline + volatile re-read helper | next: lane gap: store-then-load order (the 24k "volatile does not pin it" gap)
- 008859f0 CEnvironmentSpellEffectInterface::GetRenderFrameInterpolatedScreenFilterParameters DIFFER(112v119): middle copy read through the returned eax with `add eax,10h` for Tint and the named local placed below the temporary (retail: fixed frame offsets, temporary at the frame bottom) | tried: copy-init, POD assignment, user copy ctor + out-param, user operator= + implicit copy | next: operator= copying Tint as four floats, or a named temporary `CScreenFilterParams interpolated = ...; params = interpolated;`

## Skipped
- 00f36ca0 (chunk: CDynamicObjectHeapBase::RefCountedRelease): the 144-byte row holds a 0x11-byte body, int3 padding, then three more
  functions (a list walk at 0x00F36CC0, a deleting destructor at 0x00F36CF0, a two-word constructor at 0x00F36D10).

## Label corrections
- 0054e0b0 CDynamicList::CDynamicList -> NUISystem::CFrontEndButton::CFrontEndButton (vtables 0x01249554/30/28 in rtti-vtables.tsv)
- 004b9b90 CEditFractal::~CEditFractal -> CActiveQuest::~CActiveQuest (0x34-byte allocation in CQuestManager::ActivateQuests, ctor 0x004B0310)
- 00992510 CEditFractal::~CEditFractal -> sound-bank record destructor (element of the CASoundManager vector, vtable 0x0129A574)
- 00a26860 CEditWorld::~CEditWorld -> C3DAnimationInfo::CAnimationData::~CAnimationData (vtable 0x0129CE50 slot 0 calls it)
- 00cba750 CEditPrimitiveStats::CEditPrimitiveStats -> __pop_heap<pair<unsigned __int64, NScript::CRuntimeScriptedEntityPersist::CPersistInfo>>
- 00cbbeb0 CEditPrimitiveStats::CopySort -> __partial_sort of the same instantiation
- 006be300 CEngine::CEngine -> CMusicManager::CMusicManager (only caller CWorld::PostInit)
- 0099fdb0 CEngine::GetScratchZBufferD24S8 -> `__fastcall` integer floor(log2) helper (inlined into 0x00BE2A80)
- 00b321b0 CEngine::CollectPrimitiveStats -> CEnginePrimitiveRenderer::CollectPrimitiveStats (SceneList +0x338)
- 00b4a5f0 CEngine::Set2DLighting -> CEnginePrimitiveRenderer2D::Set2DLighting
- 00b36c50 CEngineCameraDesc::CEngineCameraDesc -> CShadowClipVolume copy constructor (0x98 bytes)
- 00c5e050 CEngineDebug::Draw3DSphere -> kernel-streaming property setter (DeviceIoControl IOCTL_KS_PROPERTY)
- 006bc410 CEngineInit::CEngineInit -> CRegion::CRegion (callers in CWorldMap)
- 00b1aeb0 ~CEngineInternalAttachedPrimitiveInfo -> NParticleEngine::CPSCLight::~CPSCLight (vtable 0x012A0624)
- 006eb9f0 CEngineInternalPrimitive2DStaticMesh::GetBoundingSphereWorldSpace -> CCombatManager method (Combatants map, GetBestSequenceIndexAttackerOnDefender)
- 008859f0 CEngineInternalPrimitive2DStaticMesh::GetObjectTransform -> CEnvironmentSpellEffectInterface::GetRenderFrameInterpolatedScreenFilterParameters
- 00c26cd0 / 00c27130 CEditControlCentre::CreateAndSaveNewLevel -> mailbox-queue post (one argument, `ret 4`)
- 00c17400 CEditControlCentre::DeleteSelectedTrackNode -> a `__fastcall` with two stack arguments (ret 8) in the script module
- callee labels: 0x00434840 "CEngine::StartNewScene" is CFadeInFadeOutBase's start (writes the fade record); 0x00A14440 "C3DVector::GetScaled" is the in-place
  Normalise; 0x00882FE0 "InterpolateRadialBlurParams" interpolates CScreenFilterParams (thiscall, hidden result); 0x00997F80 "CInputProcessMain_GetCameraPath"
  returns the working directory; 0x006F0250 "RedblackTree_FindLE" is the Combatants map find.
