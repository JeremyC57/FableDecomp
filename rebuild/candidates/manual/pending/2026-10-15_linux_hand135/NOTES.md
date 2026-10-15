# Hand chunk 135 (CEngineLandscapeMemoryManager .. CEnginePrimitiveManager2DStaticMeshes): 28 of 40 byte-exact

All 28 sources in `src/` grade MATCH / RELOCATION_MATCH (`grades.txt`), and every behaviour test prints PASS under g++
(`tall.sh`) and the lane's VC7.1 cl.exe (`tall.py`). 12 rows are parked in `differ/` with evidence; no row was skipped.

## What matched (28)
- **CEngineLandscapeMemoryManager** -- 00bda470 AllocateTexture: FlushCycle, then `new CTexture` whose class `operator new`
  is the landscape allocator body (create-on-first-use, lock, InternalAllocate, unlock) inlined; the pool argument is
  `~usage & 1`. Shape lesson: the zero stores of the inline constructor sink below the seven argument pushes.
- **CEngineLandscapePatch** -- 00bdd3e0 vector deleting destructor (`??_E`, slot 0): the `xor edx,edx` before each free is
  the explicit 0 of an inline usual `operator delete(void*)` / `delete[]` that forwards to the class's `__fastcall
  (void*, long)` placement forms (a `(void*, size_t)` usual form passes `mov edx,48h`; a lone `(void*, long)` is
  placement-only and VC falls back to global delete). 00bf3860 ClipPatch: four frustum planes, per-axis corner pick
  `n > 0 ? min : max`, dot summed z, y, x. 00bf5d30 `??_G`: the landed destructor body inlined; both destructors must be
  `__forceinline` (else `??1` stays out of line), class `operator delete` frees through the landscape manager.
- **CEngineLandscapeRenderer** -- 00b671e0 CalcForegroundFadeFactors: an inline `Set(x, y, z, w)` (arguments right to left,
  w computed first; a `C4DVector(..)` temporary assigned whole goes through the frame and VC rewrites the shared divisor
  as a reciprocal multiply). 00b67480 SetupCommonStates: identity matrix = descending zero fill then m[8], m[4], m[0].
- **CEngineLightingManager** -- 00b44ce0 ResetAlreadyFoundFlags (16x16 grid at +0xEC, inner loop strength-reduced to a
  0x40 stride); 00b44eb0 CreateInternalPrimitive (two `if (type == N) return new X;`); 00b46890 EnableFog (the tracked
  render-state set inline: `__forceinline`, the mask test written `(m_captureMask & state.m_captureMask) == 0` with
  `|=`); 00b46b90 GetLightsInSphereFast (`r + r <= limit` = `fadd st,st ; fcomp ; test ah,41h ; jp`); 00b47280
  SetGlobalLightingParameters (first theme channel colours + normalised direction); 00b47310 SetupShaderSpotLight (copy of
  the renderer's 0x240-byte scene data; the scale is a named float local homed in the frame).
- **CEngineLocalDetailGenerator** -- 00b599c0 LoadNewScene: two walks of the engine's loaded-map list through a pointer
  local; the Update argument is `(double)1.0e30f` (pool value 1.0000000150474662e+30).
- **CEngineLocalLightElement** -- 00b44e30 constructor: WorldPos copied straight into `[eax+N]` needs C3DVector's
  memberwise copy constructor (the POD copy borrows esi through `mov edx,eax`).
- **CEngineParticleSimulationRenderData** -- 00bb63a0 constructor: the CVertexBufferWin32[2] array is an inline counted
  loop; PVertexBuffer/Stride must be cleared in the DERIVED (CVertexBufferWin32) body to land after its vtable store.
- **CEnginePrimitive2DText (label)** -- 004c3000 is really `CEngineMeshEffectOutlineSmooth::CEngineMeshEffectOutlineSmooth`
  (base vtable 0x01239848 then derived 0x01239854); 00655048 is the 2D text copy constructor (`/Os`: the 12-byte header
  as a POD record copies with three `movsd`; the colour copy constructor and its Set must be `__forceinline`). The colour
  copy that loads all four bytes before any store is `CRGBColour(const CRGBColour& o) { Set(o.R, o.G, o.B, o.A); }`.
  0075ef00 is really `CKeyCamera::operator=` (callers are the CKeyCamera vector helpers; PDB 56 bytes) and 00c49100 the
  operator= of a 0x40-byte libpk CStaticQueue record (`SQueuedSoundEvent`): memberwise copies written in the body.
- **CEnginePrimitive2DViewportManager** -- 00be1ba0 Render2DPrimitive: the two stores before the sub-primitive's vtable
  belong to a `__declspec(novtable)` intermediate base; the chain passed to the renderer is the THIRD argument
  (`[esp+20h]`), not the second as the manifest says.
- **CEnginePrimitiveAnimatingMesh (label)** -- 006db7e0 is really `CTCHeroExperience::~CTCHeroExperience` (its `??_G`
  @ 0x006DB7C0 is slot 0 of vtable 0x0125F6C4): `__declspec(novtable)`, retail vectors 12 bytes / lists 4 bytes, def
  releases `if (m_p) { m_p->Release(); m_p = 0; }`, tail `jmp` to the out-of-line ~CTCBase.
- **CEnginePrimitiveLine** -- 004c3090 constructor: memberwise C3DVector copy constructor for the end points, the colour
  through the four-value Set.
- **CEnginePrimitiveManager2DParticleGroup** -- 00be1a00 CreateInternalPrimitive: group + 2D group + STLport list
  constructors all `__forceinline` (VC keeps a constructor of that size out of line).
- **CEnginePrimitiveManager2DSprites** -- 00bacfd0 CreateInternalPrimitive (two-case `switch`); 00bb1900 / 00bc7b20 (byte
  twins) destructor: four CVertexShader/CPixelShader wrappers whose release goes through a LOCAL copy of the object
  pointer (`dec [ecx+N] ; jne`), `delete` on the known-non-null object, novtable manager over ~CBase.
- **CEnginePrimitiveManager2DStaticMeshes** -- 00b9e8c0 CreateInternalPrimitive: early `type != 0x24` return; the
  init stores before the vtable belong to a `__declspec(novtable)` intermediate base, all constructors `__forceinline`.
  00ba77b0 RenderSubPrimitive: optional non-virtual base call with pass 1, then the landed base body inlined.

## Parked for Fable 5.1 (12)
- 00bf38f0 CEngineLandscapePatch::Clear DIFFER(126v132): `mov eax,[esi+14h] ; test eax,eax ; je ; mov ecx,eax ; test ecx,ecx ; je` -- retail keeps a second null test on the +0x14 delete, ours folds it | tried: plain `delete`, inline template DeleteObject(p) with/without its own `if (p)`, `#pragma optimize("s")` | next: an inline member `void Destroy() { if (this) delete this; }` (VC may not propagate non-null into an inlined `this`), or a scoped-pointer member with an inline Reset
- 007624e0 NShape::CShapeManager::CreateShape DIFFER(167v160): retail `mov ecx,[esi] ; inc ecx ; mov eax,ecx ; dec eax ; mov [esi],ecx ; mov [esi],eax` (add-ref + release folded) and `this` in edi / zero in esi; ours `inc [eax]` then reloads the address-taken local | tried: local-pointer release, `count = count + 1` add-ref, /Oa | next: lane gap: alias knowledge of an address-taken local across a store through the block pointer
- 00b59c70 CEngineLocalDetailGenerator::RegisterObjectCollectionType DIFFER(162v159): retail `mov [esp+10h],edi` + `lea ecx,[esp+18h] ; lea edx,[esp+1Ch]` (value and tag share the dead argument slot, no frame); ours adds a `push ecx` word | tried: lvalue, static_cast, inline by-value accessor (rvalue), by-value push_back parameter | next: lane gap: value temporary sharing the dead argument slot with the tag
- 009eaf80 CConsole::AddCommandLineToHistory DIFFER(105v107): retail re-reads the list head (`mov eax,[edi]`) for pop_back after the size walk and pushes the line before building the iterator in its slot; ours reuses the head register and interleaves | tried: prev/next locals, iterator copy constructor (fixed the in-slot build), erase(--end()) via iterator objects, SGI-distance size() | next: size() through an out-of-line-style iterator distance with `end()` re-read, or lane gap (CSE of the head load)
- 00b41db0 CEngineMap::SetMapVisibility DIFFER(147v147): retail pushes ebx/ebp/esi/edi in the prologue, ours shrink-wraps `push ebp ; push edi` into the insert arm | tried: STLport insert/erase inline (instructions otherwise identical) | next: lane gap: shrink-wrapping
- 005bc1b0 CEnginePrimitive2DMorphedAnimatingMesh::CEnginePrimitive2DMorphedAnimatingMesh DIFFER(102v102): retail `mov edx,[ebp+34h]` hoisted to the top, stores +2,+3 then +1,+0 from dh/dl; ours loads the B/G dword late into ecx | tried: out-of-line copy (93v102), `__forceinline` B,G,R,A init list, body R,A,G,B, inline Set(a, r, g, b) | next: Set argument orders (r, g, b, a) / (b, g, r, a) under `y`-off + `s`, or lane gap (byte-copy scheduling)
- 00493ac2 CEnginePrimitive2DSprite::CEnginePrimitive2DSprite DIFFER(153v153): `lea ecx,[esi+0Ch]` after the type/+4 stores and `and dword ptr [esi+40h],0` before `mov eax,[ebp+40h]` in retail | tried: non-polymorphic header base, flattened header + body R,G,B,A colour (fixed the colour), trailing members as body assignments | next: scheduling-only, lane gap
- 00543910 CEnginePrimitive2DText::CEnginePrimitive2DText DIFFER(114v114): `mov [esi+0Ch],edx` (position X) one pair later in retail | tried: plain C2DVector copy, memberwise copy constructor, C2DVector(float, float) init list (fixed the x87 Y), body assignments | next: scheduling-only, lane gap
- 00b3a110 CEnginePrimitiveAttachmentManager::FrameStart DIFFER(99v99): retail `lea eax,[esp+1Ch]` (empty tag over the LIVE params slot, `sub esp,0Ch`), ours `sub esp,10h ; lea eax,[esp+0Fh]` | tried: the inline CArray push_back of the engine camera pointer | next: lane gap: empty tag over a live parameter slot (GOTCHAS 24l)
- 00bc5a20 CEnginePrimitiveChangerManager::RenderPrimitiveToShadowBuffer DIFFER(113v116): retail dead `lea eax,[edx+eax*8]` beside the slot load (index eax / primitive ecx), ours folds it (primitive eax / index ecx) | tried: early returns (shrink-wrapped), single-exit nested ifs (fixed the tail), handle reference local, STLport iterator object | next: lane gap: RTM vector-slot address materialisation
- 0041c500 CEnginePrimitiveHandle::`scalar deleting destructor' DIFFER(112v114): retail `mov [esi],vtbl ; mov eax,[esi+4] ; test eax,eax ; ... ; mov ecx,eax` (the retail out-of-line Clear @ 0x0041BE70 has the same head), ours loads Data straight into ecx and sinks the vtable store | tried: member test, pointer local, pointee-side inline NotifyIfLastHandle, static_cast to a derived primitive | next: Clear as an out-of-line member defined above under `auto_inline(off)` is NOT the shape (it is inlined); try the release/notify through CIVCountedPointer inline accessors (`Data->`, `Data.GetRefCount()`)
- 00bb1640 CEnginePrimitiveManager2DSprites::GetSupportedTypes DIFFER(122v122): retail `mov dword ptr [esp+0Ch],23h` (second constant in the dead argument slot, shared with the tag), ours reuses the first constant's `push ecx` word | tried: literal temporaries, two named locals (128v122) | next: lane gap: value temporary sharing the dead argument slot (as 00b59c70)

## Skipped
None (no row spans two functions; 00bb1900 and 00bc7b20 are byte-identical twins and are authored separately).

## Label corrections
- 007624e0 "CEngineLightingManager::CreateInternalPrimitive" -> NShape::CShapeManager::CreateShape (callees are the NShape
  CBasicShape / CTrack constructors and the CCountedPointer<CShape> Reset).
- 009eaf80 "CEngineManager::Cleanup" -> CConsole::AddCommandLineToHistory (named by the landed SendMessageToUser @ 0x009EB260).
- 004c3000 "CEnginePrimitive2DText::CEnginePrimitive2DText" -> CEngineMeshEffectOutlineSmooth::CEngineMeshEffectOutlineSmooth (vtables 0x01239848 / 0x01239854).
- 0075ef00 "CEnginePrimitive2DText::operator=" -> CKeyCamera::operator= (callers: _Copy_backward @ 0x0075F0E0 and the insert @ 0x0075FAE0, 0x38-byte stride).
- 00c49100 "CEnginePrimitive2DText::operator=" -> operator= of a 0x40-byte libpk CStaticQueue element (only caller 0x00C49010, "PKCStaticQueue.h").
- 006db7e0 "~CEnginePrimitiveAnimatingMesh" -> CTCHeroExperience::~CTCHeroExperience (`??_G` @ 0x006DB7C0 = slot 0 of 0x0125F6C4).
- 00bf5d30 "~CEngineLandscapePatch" is the scalar deleting destructor `??_G` (the plain destructor is 0x00BF3A40).
- Callee labels: 0x00BF36A0 / 0x00BF36E0 "operator_new" are the patch's `operator delete` / `delete[]`; 0x009F2000 / 0x009F2010
  "_Dest_val<..CFlash>" are GFEnterAllocatorLock / GFLeaveAllocatorLock; 0x00A14440 "C3DVector::GetScaled" is
  the in-place Normalise; 0x005BBDF1 "CExpressionSet::Add" and 0x00514400 "std_map_insert_CCharString" are copy constructors of a
  vector / map; 0x00B84720 "Engine_AddClockVertex" is the primitive renderer's sub-primitive queue call; 0x006DA9A0
  "SortTreeRecursively" and 0x0053C965 "CActiveFile::OnReadFinished" are list destructors.
