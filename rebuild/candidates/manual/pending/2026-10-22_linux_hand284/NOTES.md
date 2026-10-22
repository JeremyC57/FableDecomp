# Hand chunk 284 (CDefinitionManager .. CEngineInternalPrimitiveAnimatedMesh): 21 of 40 byte-exact

All 21 grade RELOCATION_MATCH on RTM cl 13.10.3077 (no QFE row). Every one has a behaviour test that passes under g++ and VC7.1.

## What matched (21)
- **CDefinitionManager**
  - 009aef60 CreateSymbolsFromPathList: scan each path spec, lower-case and sort the names, read each file, strip comments, then parse it for symbols; finalise both symbol tables. Shape: a non-template 12-byte STLport `CWideStringVector` with an inline dtor (destroy loop, then `free`) and a placement-new push_back. `_M_insert_overflow` 0x412330 is a same-TU noinline stand-in that reads Start/Finish before writing, which keeps _M_start/_M_finish in edi/esi. Two more pieces are needed: the chained `ConvertToCharString().ToLower().ToWideString()`, and a qualified `file.CDiskFileWin32::Read(...)` for the direct call.
  - 009af480 DoCompilePathList: the same shape. The function-name string is declared inside the loop after `fileName`, and there is no finalise tail.
  - 009b05f0 CheckDependencies: builds the .cks path, looks up checksums into a vector map, sorts it and CRCs the values. Shape:
    - Add and push_back are `__forceinline` (otherwise Add goes out of line).
    - The insert-grow 0x9B71B0 is a same-TU stand-in.
    - The CRC table loop runs over a local copy of each value.
    - An `__int64` member in the checksum file gives the `and esp,-8` frame.
- **CDragon / CDrinkingHobbe / CDyingVillager / CEM_FlashLightPoint**
  - 00d25a00, 00d9bf60, 00d7f460, 00e76920 Main: generated script bodies with `if (IsActiveThreadTerminating()) return;` after every statement. A CScriptThing copy that retail inlines needs the out-of-line copy declared for the predicate copy only. By-value CScriptThing results need the dtor declared on the class itself.
- **CDrawAbilityButtons**
  - 0064db2b DrawAbilityList: /Os via `optimize("y",off)` plus `("s",on)`. It uses the GetInterface out-param idiom, and the drawn counter is initialised as `firstRow - 1`.
- **CDrawAbilityButtonsPC**
  - 00658020 DrawExpressionButton: one nested-if body. The def-pointer operator= and dtor are `__forceinline`, and the release is written `if (--count == 0)`.
  - 00657d90 DrawExpressionList: matched by advisor A3. The slot-offset fill is a same-TU stand-in that reads Start/Finish/End first, and `long row = firstRow;` is declared before `drawn`.
- **CDrawPerceivers**
  - 0064499c Draw: `optimize("s",on)` alone gives the biased frame. It also uses an STLport-style min by const reference and an inline GetFont accessor.
- **CDrawScriptScreenText**
  - 0064560d Draw: /Os; the inline CCharString== is `__forceinline`; the iterator object has a user copy ctor and is read through a `SScriptLine&` local.
- **CEditBox**
  - 0053fe50 AddCharacter: retail members sit 0x40 below Ego_r.pdb; CWideString operator+ overloads are `__fastcall` with the hidden result first.
  - 00540980 UpdateDisplayText: advisor B4 found the double-sum X order `C2DVector((double)home.X + offset, home.Y)`.
- **CEngineScreenEffectColourFilter** (catalog row CEngineBillboardGenerator)
  - 00b5a1d0 ctor: some member ctors are inline and others out of line; the grey weights are an assigned CRGBFloatColour temporary.
- **CEngineCamera**
  - 00b314e0 Process: the desc ctor stores StateValid first, and the FOV conversion constants are named so VC cannot fold them.
  - 00b308e0 ClipWorldSpaceBoundingBox: plane tests with a user-copy-ctor C3DVector.
- **CEngineClothSimulationAnimatedMesh**
  - 00b82100 RunSimulation: the first failure exit releases inline and the empty-primitive exit calls the out-of-line dtor. The simulation and the program are named before the call.
- **CEngineInternalPrimitive2DSprite**
  - 00bad8a0 Update: the colour goes through a named CRGBColour local; WrapTexture is `start.IsNonZero() || end.IsNonZero()`.
- **CEngineInternalPrimitive2DViewport**
  - 00be1e70 Render: the working description lives in its own block, the stores follow source order, and the unused dimensions result is a named local.
- **CEngineInternalPrimitiveAnimatedMesh**
  - 00bcebe0 GetBoundingBoxTransform: the sphere is read twice through the by-value GetBoundingSphere().

## Parked for Fable 5.1 (18)
- 00badb40 CEngineInternalPrimitive2DClockSprite::Update DIFFER(764v764): eax/edx phase swap on the Colour2 (+0x40) and InterpolationSet (+0x4) copies, 4 lines | tried: colour/colour2/interp/flags locals, bitfield flags, Filter forms | next: named-local order for the two copies as in 00bad8a0, or lane gap: register phase
- 00b25d80 CEngine::Uninitialise DIFFER(653v653): delete loops load the popped component via eax + mov ecx,eax where retail loads straight into ecx (start edx, finish reloaded) | tried: index/pointer loops, back()/pop_back forms | next: STLport pop_back with a `T& back = *(finish-1)` local, or lane gap: register allocation
- 00b393d0 CEngineInternalAttachedPrimitive::GetChildInverseTransformStaticMesh DIFFER(529v521): inline 3x3 inverse cofactor fld/fmul operand order, determinant term order, extra frame spill (sub esp,0xc v 8) | tried: source-order and named-member forms | next: lane gap: x87 operand canonicalisation
- 00bcf040 CEngineInternalPrimitiveAnimatedMesh::IsInsideFrustum DIFFER(658v658): row-1 c.X term is `fld st(3); fmul [E11]` in retail v `fld [E11]; fmul st(4)` | tried: term order, operand swap, by-value / member-operator forms | next: lane gap: x87 operand choice (advisor asked, no lever)
- 00b39080 CEngineInternalAttachedPrimitive::GetParentTransformAnimatedMesh DIFFER(846v846): one `lea ecx,[esp+0x74]` for the out-of-line stats release scheduled one scale store early | tried: member scale writes, forceinline ScaleRotation, operand swap, scoping, direct-init | next: lane gap: scheduling
- 00b80ce0 CEngineClothSimulationMeshBase::GetDeltaTransform DIFFER(687v682): inline 3x3 inverse cofactor operand canonicalisation + c0*inv frame spill (sub esp,0x34 v 0x30) | tried: all-swapped source (identical), named members | next: lane gap: x87 operand canonicalisation
- 00b31160 CEngineCamera::ProcessDebugScript DIFFER(891v886): in-loop index recompute uses eax/edx in swapped roles (+5 bytes), int-copy register rotation, two Normalise `lea ecx` scheduled early | tried: loop inversion, `!(t < 1)`, element pointer locals | next: `*(begin()+i)` index form for the recompute
- 00540320 CEditBox::KeyPressed DIFFER(755v755): three vtable loads in eax v edx, and `sub edi,5` v `add edi,-5` | tried: unsigned/signed, `+ -5`, `-= 5`, `- 5u`, parenthesised forms | next: lane gap: register/encoding choice
- 00b30610 CEngineCamera::ClipBox DIFFER(710v710): VC7.1 CSE-pairs the 24 corner sums differently from retail's (x + t) + (y + z), and one dot-product operand order | tried: 13 association forms per component, explicit per-corner trees, out-param v by-value transform | next: lane gap: reassociation
- 0064ae80 CDrawAbilityButtons::DrawAbilityButton DIFFER(812v812): pos.Y x87 reassociation, plus the &abilities[index] lea placed before the AddRef copy | tried: split statements, float temp, double-sum lever, noinline GFAPositionToScreen stand-in (advisor A3 parked) | next: lane gap: x87 reassociation
- 006495a8 CDrawScreenFade::Draw DIFFER(937v938): frame 0x90 v 0xe4; retail keeps two sprite objects and separate black/fade boxes, while VC shares them across sibling blocks | tried: named per-block locals, scoping | next: lane gap: frame-slot sharing
- 0064e978 CDrawReceivedItems::Draw DIFFER(910v910): frame layout 0x13c v 0x158, ebx kept 0 from the prologue, by-value colour built in a frame temp | tried: member-wise lerp statements, forceinline colour copy | next: lane gap: frame layout
- 00646f67 CDrawExperience::Draw DIFFER(824v843): CRGBColour copy ctor out of line (forceinline gives 837v843), frame 0x14c v 0x154, faded colour built through a byte temp | tried: forceinline / implicit copy ctor, y-off+s-on v s-only frames, field-wise colour | next: colour built field-wise into a named byte struct then copied
- 0064a639 CDrawGuildSeal::Draw DIFFER(911v911): frame 0x80 v 0x78; retail shares hero/map/name and key/seal slots and hoists the loop's zero UV stores after the guard | tried: forceinline HasGuildSeal (fixed length), scoped blocks, named v temporary strings | next: lane gap: frame-slot sharing
- 00435190 CDisplayEngine::InitEngine DIFFER(783v774): ENGINE def block puts the name temp at [esp+4] and the def at [esp+8]; the release decrements on the loaded pointer where VC reloads it | tried: Release() member, local data pointer, `!--` / split forms, nested-if fail block | next: declare the def pointer after the name temporary in an inner scope
- 009bf7e0 CDisplayManager::CreateDevice DIFFER(824v846): retail builds CPixelFormat temporaries in the dead params argument slot (0x18 v 0x28 frame) | tried: one full reconstruction | next: lane gap: locals in dead argument slots
- 009bfb30 CDisplayManager::ResetDevice DIFFER(828v830): same dead-argument-slot temporaries (and the thrown empty object) | tried: reconstruction from the CreateDevice model | next: lane gap: locals in dead argument slots
- 009af7a0 CDefinitionManager::SaveBinaryDefinitions DIFFER(1007v1007): register allocation; retail has this in ebx, the stream in ebp and edi = 0 across the clears, while VC swaps ebx/ebp and caches &CompressedOffsets in edi | tried: inline STLport erase/copy (memmove-return form fixes the erase), same-TU InsertGrow stand-ins, 0x2c persist context, separate CRC global | next: lane gap: register allocation

## Skipped
- 0055a5d0 CDraggable::OnLeftClicked: a multi-function row (6 int3-separated bodies up to 0x55a890). The first piece, OnLeftClicked (82 bytes), matched in probe/dg3.cpp.

## Label corrections
- 0x00B5A1D0 "CEngineBillboardGenerator::CEngineBillboardGenerator" is the CEngineScreenEffectColourFilter ctor (vtable 0x012A25CC RTTI).
- 0x00A0BE90 / 0x00A0BE80 / 0x00A0BEA0 "CMap/CWorld DrawGet*" are the camera horizontal-FOV / FOV-flags / vertical-FOV getters; 0x00A14440 "GetScaled" is C3DVector::Normalise.
- 0x00AE0600 "GetDefaultVal<CPhysicalPrimitiveInit>" is the CParticleSimulation ctor; 0x00A36EC0 "_Dest_val<...COptimisedPrimitive>" is the counted-pointer dtor body (byte-identical COMDAT).
- 0x00B26340 "CreateInternalPrimitive" is CreateEngine (new 0x178 + ctor); 0x009A4EC0 "AddChildPrimitive" is a global-address getter; 0x00436FB0 "CGameJoystickManager::Get" is the engine-list singleton getter; 0x006404D0 is list push_back.
- 0x0041A100 / 1C0 / 1E0 / 200 "GetAutoSaveFileName" are __fastcall directory getters (Graphics, Mesh, Shader, EngineCache); 0x00430096 "GetDef<CEnvironmentDef>" is used for the ENGINE def; 0x009BEBB0 is the depth-format chooser.
- 0x0099E480 "GetDefaultVal<CAnimationSet>" is CCharString::ToWideString; 0x0099AED0 "CCharString" is the CWideString default ctor; 0x0099B510 "~CCharString" is ~CWideString; 0x0099B720 is the CWideString copy ctor; 0x0099EAE0 "_Cons_val..." is ~CCharString.
- 0x009AF1E0 is CDefinitionManager::CompileString; 0x009E5C30 "ProcessButtonBReleased" is the symbol-table finalise; 0x00A60D40 / 0x00A60FB0 are checksum-file Open / GetChecksum; 0x00A60CE0 "~CPatchTesselationEdgeStrip" is the checksum-file dtor.
- 0x0099A300 "CBase" is the CBase dtor; 0x009BAA50 "CPersistContext" is the CPersistContext ctor; 0x009D4CE0 is the CDefStringTable save of 0x013CA828.
