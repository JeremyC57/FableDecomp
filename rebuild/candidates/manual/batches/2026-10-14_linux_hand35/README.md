# Batch `2026-10-14_linux_hand35` — coordinator hand-authoring chunk 35 (CEnginePrimitiveManagerDecal .. CEngineResourceManager): 34 of 40 landed

Chunk 35 of the class-labelled 17–96-byte pool, authored by the coordinator with the Wine lane: eight more
CreateInternalPrimitive allocators (lines, sprites, flare sprites, repeated Z sprites, sprite trails, weapon trails,
weather masks: a type check, non-throwing new, the inlined constructor with its plain words before the vtable and its
member objects after), four primitive-manager destructors sharing one CVertexShader model (the CVertexShader /
CShaderBase vtables, the shared shader's intrusive count, the counted occlusion data, the dynamic 3D resource
manager; all `novtable` abstract bases with a tail-jump ~CBase), the decal / static-mesh RenderSubPrimitive bodies,
DrawVisibilityTestBox, the repeated-static-mesh FrameStart (buffer pointer selection) and GetSupportedTypes, the
repeated-Z-sprite FrameEnd (vector resize to nothing), the CTCHeroSpecialMovement, CEnginePrimitiveOcclusionManager,
CEnginePrimitiveTypeInfo (five-argument) and static-mesh placement constructors, CTCCreatureModeManager::ClearModes,
two CCharString-forwarding setters, CEnginePrimitiveRenderer's ResetVisibilityRatios / RenderNonsortedPrimitives /
AddStaticMesh (a by-value result forwarded through the hidden pointer), a vector resize with inlined erase / insert
selection, the simulation-info RunSimulation loop, two GetIndexBufferUsage copies, and CEngineResourceManager's
DefragmentBuffers / ChangeScene / Render.

Label corrections: 00bc9a00 "vector deleting destructor" and 00bcbea0 / 00bcd7c0 "scalar deleting destructor" have no
flag test: they are the plain destructors of CEnginePrimitiveManagerLines and of the trails managers' shared base
(named by address, two copies); 007e08e0 "CEnginePrimitiveManagerParticleGroup" is CTCHeroSpecialMovement's
constructor (vtable 0x0126FE3C); 0059b49b "CEnginePrimitiveRenderer::AddStaticMesh" is a game-side constructor taking
three 16-byte structs (named by address); 0073ae70 / 00a0b6e0 / 00a3e800 "ClearAllChildPrimitives" are
CTCCreatureModeManager::ClearModes and two CCharString-forwarding setters on game objects (named by address);
00ac41d0 "FindSimulation" is a vector Resize; 00cac260 "CEnginePrimitiveTypeInfo" is a five-argument record
constructor of the 0x00CA module (named by address); 0x00C1BD20 "CDateAndTime" is a two-float pair constructor;
0x00C20390 / 0x00B59710 "CNavigationLayer" are propagated base-constructor labels; 0x00B8D090 / 0x00B8D0A0 / 0x00BD9B50
are empty `ret` bodies; 00ba1140 is a 28-byte prefix of a larger function (bad family-scan row, skipped).

Lessons (also in GOTCHAS, wave 24e): a counted member released through `dec [p+10h] ; jne ; call [vt]` is a local
pointer copy (`--p->count == 0` on the member re-reads it); an engine manager destructor with no vtable store of its
own is `__declspec(novtable)`; constructor words stored before the vtable are plain init-list members, words after it
are member objects with inline constructors; an array zeroed with `rep stosd` is `memset` in the body; a by-value
result forwarded to another by-value-returning call needs no local; a frame-pointer game constructor is
`optimize("y",off)` + `("s",on)`; a store scheduled after a local's destructor needs the local in an inner scope.

Parked (evidence in the scratch NOTES): 00b81e00 / 00b2fa90 / 00b8faa0 (zero-register `xor` hoisted above the counted
base's store), 00b812c0 (index/first register allocation), 00b2f9f0 (the empty-range inlined erase's register copy),
00ba1140 (bad oracle row).
