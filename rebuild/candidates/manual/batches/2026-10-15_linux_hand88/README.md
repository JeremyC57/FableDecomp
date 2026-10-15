# Hand chunk 88 (CTCForcePushed .. CTCGraphicAppearanceStaticMesh): 37 of 40 byte-exact

All 37 src/ rows grade MATCH (1) or RELOCATION_MATCH (36) with q.py (grades.txt); all 37 tests print PASS under g++
(tall.sh) and under the lane's VC7.1 cl.exe (tall.py). Three rows parked in differ/. No row skipped.
Two authoring sessions: the first agent did 28 matches + 3 parks; the second finished 004bff60 and the last 8 rows.

## What matched (37)
- **CTCForcePushed** -- 00798540 vfunc_2: alive player-creature owner (`Dead` bit clear, type byte 1) gets
  SetSuspendAnimation(false, 0). Owner read once; VC keeps the constant 1 in al for both the bit test and the compare.
- **CTCGamePlayer** -- 004d8fdd ctor: CTCBase, `Game(0)`, the GameTable intelligent pointer; `#pragma optimize("s")`
  (`push [esp+0Ch]` pass-through, `and [esi+0Ch],0`). 005edc90 GetGame: GameTable.GetPItem() called twice (null test, then the
  Dead bit), nested ifs share the `xor eax,eax` tail.
- **CTCGatherPointBase** -- 007ffe40 vfunc_1 (persist): Transfer("MaxOccupation", MaxOccupation, 0) with the zero default in
  the `push ecx` slot, then the derived transfer (slot 30).
- **CTCGatherPointRadialArea** -- 007ffd60 vfunc_28: strict 2D distance test against GatherRadius. Owner and radius read into
  named locals before the GetPosition call; `dx*dx + dy*dy` as one sum squares dy first.
- **CTCGossip** (label) -- 004d8c27 CTCGossip ctor and 004d966f (really CTCFlourishTarget ctor): optimize("s"), member
  intelligent pointer then bool initialisers in declaration order.
- **CTCGraphicAppearance** (15) -- shared local model in gen/ (no engine header; retail 0x5c layout from Ego_r.pdb):
  - 004bc620 UpdateRenderedLastFrameFlag: walk Primitives only when PrimitivesDrawn; two exits `or 40h` / `and 0BFh`.
  - 004bcfe0 RefreshPrimitiveAnimationAndPos: a CEnginePrimitiveAnimation temporary in a 0x64 frame passed to manager slot 24.
  - 004bd420 GetAllMeshes: `reserve(4)`, then each component's slot 2 with the vector held in ebx.
  - 004bd690 SetAttachedPrimitiveGraphics: flags as an `unsigned short` local widened at the push (`movzx eax,si`).
  - 004be490 PrepareForRenderIfNeeded: inline `unsigned long` reasons getter; the latch bit copy reuses the flag register.
  - 004bf920 RemovePrimitive: compare each node's value ADDRESS; inlined STLport erase with `next` read first.
  - 004bf9e0 SetAsChanged / 004bfce0 CatchMessage (SetAsChanged(8) inlined): reasons in bl; `head->next == head` empty test.
  - 004bfa20 SetSelfIlluminating / 004bfb00 SetByPassColorFilter: bit-field merge from a bool; signed counter transitions
    share one notification tail (cross-jumped).
  - 004bff60 ClearAllMeshEffects: STLport `_M_increment` inlined; `cur = begin` declared FIRST, then `it.m_node = cur`, loop on
    `cur` with `cur = it` at the bottom (needed 4 grades -- see LESSONS).
  - 004bffd0 SetActivePersistenceFlags, 004c00b0 SetTintColour (map operator[] with the colour read first), 004c0000
    UnregisterComponent (ClearAllPrimitives inline + list remove with an rvalue copy), 004c11d0 GetFocalPosIfLoaded
    (`test al,al ; setne al`).
- **CTCGraphicAppearance label, really other classes** (propagated "GetAnims" label):
  - 00a3f8b0 CGameMapInterface::GetHeightAt(float, float) (vtable 0x0129D288 slot 0): null map -> pool 0.0f; coordinates
    truncated with __ftol2 in source order into an aggregate (no 2-arg ctor: it would convert y first).
  - 00ca9ae0 / 00ca9b10 / 00ca9b40 video player (class named after the landed Pause @ 0x00CA9A90): SetEvent/ResetEvent on +0xd0
    then a COM call (control slot 6 / stream slot 15 / 16); plain C++, the stream member's double read is VC's own.
  - 00caec80 CAdviseHolder_00caec80::Unadvise: a `__stdcall` virtual overriding a slot of the SECOND base (+0x0c), giving
    `mov esi,[esp+8]`, `[esi+4]` member reads and `lea edx,[esi-0Ch]` for `this` passed as the full object. Plain MATCH.
- **CTCGraphicAppearanceAnimatedMesh** -- 004de839 (really CTCAnimatingObject ctor): optimize("y",off)+("s",on) frame-pointer
  TU, empty allocator temporary homed in the dead owner slot. 0077b6b0 (catalog StaticMesh vector-deleting dtor; really
  AnimatedMesh `scalar deleting destructor'): `__forceinline virtual ~` + non-virtual component dtor + `new` factory.
- **CTCGraphicAppearanceMultipleStaticMeshes** -- 007e14c0 ctor (+RegisterComponent). 00c2a6d0 (label OnCreate; really a
  third-party registry `ReleaseByKey(unsigned long key)`: virtual Find(&key) at slot 17 then ReleaseEntry). 00caddc0 (label
  ctor; really CAppearance_00caddc0::Rebuild, the callee of landed CAppearanceOwner_00ca4990::Rebuild): critical-section-guarded
  reset of four counters and a dirty word, returns 0.
- **CTCGraphicAppearanceReplaceableMesh** -- 007a66a0 ctor (+RegisterComponent). 004e748a (really CTCCarrying ctor, optimize
  y-off + s). 007495d0 (really CTCStealth::~CTCStealth: `__declspec(novtable)`, vector range destroy + free, def release).
- **CTCGraphicAppearanceSprite** -- 0077c3e0 ctor (float zeroes via the integer zero register, 1.0f sunk below the vtable
  stores). 0077c490 SetRotationAngle(float): SetAsChanged(4) on the owner's appearance, store the per-frame amount, random
  start angle; the random generator fetched into a local in its own statement keeps `push 1.0f` after the getter calls.

## Parked for Fable 5.1 (3)
- 008fa280 CTCGift::IsGiftOfType DIFFER(104v83): after GetSubDef the lane keeps the sub-def pointer in eax (retail ecx), so each inlined release is `mov ecx,eax ; add eax,4 ; dec ; test ; mov [eax]` instead of `dec [ecx+4]`, and ebx is pushed at entry (retail only on the success path) | tried: refcount in the class, local-copy release, the landed two-base CDefBase model, reversed polarity | next: a CDefPointer-style RAII local whose destructor releases (scope end instead of explicit releases), or lane gap: register allocation
- 004bc030 CTCGraphicAppearance::Copy DIFFER(94v91): first bit-field copy -- retail `mov al,[src+59h] ; mov dl,[ecx+59h] ; xor al,dl ; and al,1 ; xor dl,al` loads the destination byte once, the lane reloads it for the final xor | tried: plain bit-field copy, `bool : 1` bit-fields, a getter returning the bit, an explicit `(flags & ~1) | (other & 1)` merge | next: copy the whole flag byte through a union/struct-of-bits member, or lane gap: bit-field merge codegen
- 004bd3c0 CTCGraphicAppearance::IsDrawingAfterWater DIFFER(93v91): after the interface lookup retail loads the component into ecx and the holder (+0x10) into eax (`mov ecx,[eax+4] ; mov eax,[ecx+10h]`); the lane swaps them and adds `mov ecx,eax` | tried: the out-parameter GetInterface template (plain and `__forceinline`), HasTC + pointer-returning FindTC, an inline holder test member | next: lane gap: register allocation (scheduling-only, 2 bytes)

## Skipped
None (no row holds two functions).

## Label corrections
- 004d966f CTCGossip::CTCGossip -> CTCFlourishTarget::CTCFlourishTarget.
- 004de839 CTCGraphicAppearanceAnimatedMesh ctor -> CTCAnimatingObject::CTCAnimatingObject.
- 004e748a CTCGraphicAppearanceReplaceableMesh ctor -> CTCCarrying::CTCCarrying.
- 0077b6b0 CTCGraphicAppearanceStaticMesh `vector deleting destructor' -> CTCGraphicAppearanceAnimatedMesh `scalar deleting destructor'.
- 007495d0 ~CTCGraphicAppearanceReplaceableMesh -> CTCStealth::~CTCStealth.
- 008fa280 CTCGift::IsFriendly -> CTCGift::IsGiftOfType.
- 00a3f8b0 CTCGraphicAppearance::GetAnims -> CGameMapInterface slot 0 (vtable 0x0129D288; named GetHeightAt here).
- 00ca9ae0 / 00ca9b10 / 00ca9b40 CTCGraphicAppearance::GetAnims -> video-player methods next to Pause @ 0x00CA9A90 (vtable cells
  0x012BF7C0, 0x012BF7F8, 0x012BF7FC).
- 00caec80 CTCGraphicAppearance::GetAnims -> third-party COM-style Unadvise (vtable cell 0x012BF8C4).
- 00c2a6d0 CTCGraphicAppearanceMultipleStaticMeshes::OnCreate -> third-party registry ReleaseByKey (only caller 0x00C14A19).
- 00caddc0 CTCGraphicAppearanceMultipleStaticMeshes ctor -> CAppearance_00caddc0::Rebuild (callers 0x00CA499A / 0x00CA49CA).
- Callees: 0x00686800 "CEngineSubPrimitive2DClockSprite" is CTCBase::CTCBase; 0x00686830 "~CChunkedFileChunk" is
  CTCBase::~CTCBase; 0x004BD390 "DoubleLinkedListInsertNode" is CTCGraphicAppearance::RegisterComponent; 0x004978A0
  "GFFloatRandom" (`__fastcall`) is a `__thiscall` FloatRandom(float max) on the random seed; 0x0049D810
  "CVertexBufferWin32::DoSizeof" is a display-engine getter returning `&p->+0x1613c` (the random generator); 0x00C2A5C0
  "CTCParticleAttacher::HandleMessage" is a third-party entry release.
