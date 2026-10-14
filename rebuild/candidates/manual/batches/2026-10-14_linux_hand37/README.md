# Batch `2026-10-14_linux_hand37` — coordinator hand-authoring chunk 37 (CEngineScreenEffectOutlineGlow .. CEngineTreeFilerBase): 33 of 40 landed

Chunk 37 of the class-labelled 17–96-byte pool, authored by the coordinator with the Wine lane: another run of
destructors and releases (entry-list owners, buffer pairs / triples, a chunked-file chunk, a triple of `__stdcall`-freed
resources, a worker thread's interlocked handle / critical sections / events, texture-pool releases of the outline glow
and radial blur renderers, the shadow renderer's default-pool release, three non-polymorphic deleting destructors whose
polymorphic object at +0 is a MEMBER), the outline glow's Render, the deed owner's clear, the CEngineSimulationBase /
CHeroAbilityDef-style / CEngineStateBlocks (ten 4-byte state-block members) / CTCBase / CEngineSubPrimitiveBase /
CEngineSubPrimitiveText / static- and animated-mesh sub-primitive / CSoundEmitterLH::CActiveSound constructors, the
block-file manager's set lookups (ReleaseRequest, IsBlockLoaded, GetBlockData) and SetBankFileEntry (an inlined
CCountedPointer operator= over a by-value argument), CEngineSubPrimitiveRenderer's RenderList / AddSubPrimitive, the
surface composition manager's mipmap adjust, and CEngineTreeFilerBase::DeleteTree.

Label corrections: the eleven "FinishWithDynamicTextures" rows are unrelated destructors / releases (named by address or
effect: an entry-list buffer, a chunked-file chunk's buffers, a buffer triple, the outline glow's texture release, an
entry-list owner, a `__stdcall`-freed resource triple of vtable 0x012B3290, a worker thread, a deed owner, the radial
blur's texture release); the seven "CEngineSubPrimitiveText `scalar deleting destructor'" rows are deleting destructors of
seven different classes (a wide-string pair owner, a hover-wander AI group with its base as a member, a CModeIntangible
owner, a buffer-and-string record, a mesh base with an edge strip; 00bc7a80 is the real one, parked); 00b80900's chunk
label is right; 0045604d "CEngineStateBlocks" is CHeroAbilityDef (parked); 00686800 "CEngineSubPrimitive2DClockSprite"
is CTCBase (vtable 0x0125BE0C); 00aab8b0 "CEngineSubPrimitiveAnimatedMesh" is CSoundEmitterLH::CActiveSound; 00b9cb10 is
CEngineSubPrimitiveStaticMesh; 005812ae "GetBlockData" is a game-module tree lookup (named by address); 00b842d0 /
00b84990 are `__stdcall` statics; 00bc9b30 FindNode is a `__stdcall` static (parked).

Lessons (also in GOTCHAS, wave 24g): a destructor body with no vtable store whose "base" destructor is called on `this`
and whose other member is destroyed first is a NON-POLYMORPHIC class holding the polymorphic object as its first member
(VC7.1 then emits the non-virtual `??_G` on `delete`); a size-optimised TU emits an empty class destructor out of line
unless it is `__forceinline`; a global read through an inline member (`g->Next()`) lands the global in ecx; a `do/while`
chain walk has no entry null test; `__stdcall`-freed resources leave no `add esp,4`; a mixed `add esp,4` / `pop ecx`
destructor needs no pragma games (the lane produces it naturally); the mesh sub-primitives' members are stored before the
derived vtable because they belong to the (novtable) base constructor; a counted-pointer by-value argument's
destructor drops its dead field clears.

Parked (evidence in the scratch NOTES): 0045604d (lea scheduling), 00b84990 (callee-saved scratch registers around a
byte test), 00bc9b30 / 00be0420 (register choice), 00bf3180 (`add esp,4` placement), 00b8fb60 / 00bc7a80 (deleting
destructors without a vtable store; a pooled delete called with `this` in ecx).
