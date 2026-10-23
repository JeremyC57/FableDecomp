# Hand chunk 317 (CEnginePrimitiveRenderUtil .. CExpression_PicklockScript): 13 of 40 byte-exact

## What matched (13)
- **CEnginePrimitiveRenderer** 00b33520 ~CEnginePrimitiveRenderer: deletes every scene in SceneList, then clears the list and the global render lists. Shape: STLport `clear()` written as the CopyTrivial erase-to-end.
- **CEngineScreenEffectDisplacementRenderer** 00b871b0 BeginDisplacementStencilRendering: opens the render-state capture block and sets the stencil states. Shape: OpenCaptureBlock is inline (`CaptureStack[CaptureStackCount++].Block = CaptureBlock; CaptureMask <<= 1`), and so are the core SetState calls.
- **CEngineStateBlock{Solid,Solid2X,DiffuseEnv2X,Alpha2XTFactor}** 00b56b00, 00b56f50, 00b592c0, 00b586a0 Apply: set the 10 or 17 tracked render states. Shape: one inlined SetState per state (capture mask, then the dirty list), generated from the state table.
- **CEngineWeatherRenderer** 00b54310 RenderMist: binds the mist volume texture on stages 0-3, sets the VB/IB and constants 0..9, then draws. Shapes:
  - the mist-params global 0x1436E9C is read into a named local (`const CMistParamsView* mist = g_pMistParams;`), which gives retail's fld order for `MistHeight * mist->Scale`;
  - SetIndices is if/else, not a ternary;
  - the volume texture holds its device pointer at +4.
- **CEngineWaterRenderer** (4 rows):
  - 00b6e3e0 RenderIceBumpMap: two bump passes. Shape: a local layout view whose STLport CArrays are 12 bytes (PDB 16), so everything after +0x12c shifts by -0xc.
  - 00b75ca0 RenderIceBackground: shapes are `IceBumpScale * 0.5f` with 0x122F59C as a literal (an extern swaps the fld/fmul order), the bone resize plus four SetBoneTransform calls, and PrepareForRender(0x604).
  - 00b76bc0 RenderBackground: the textures are interleaved with the sampler SetStates. Shapes:
    - `CRGBFloatColour colour = lowEndColour * *ambient;` comes before the PS5 constant, then `C4DVector(colour)` (frame 0x54);
    - the +0x28b8 block needs the "StateMaskFirst" SetState spelling.
  - 00b77e20 RenderSeaNonReflectiveForeground matched on the first grade. Shapes:
    - the swell constant is written as whole if/else calls;
    - the fresnel factor uses an inline Clamp;
    - PS7 is a C4DVector local filled member by member.
- **CExpression_DigScript** 00eea7c0 Main is the dig expression quest, modelled on the landed CExpressionWaitScript_Main 00eea460. Shapes:
  - `bool found = spot.IsAlive() && Is2DDistanceBetweenThingsUnder(...); if (found) {...}`. An if/else that assigns true/false places the `found = false` store inline, 8 bytes off.
  - The discarded GetPos/GetAngleXY virtual calls stay.
  - The movie object's ctor and dtor are out of line.
- **CExpression_PicklockScript** 00eeb7b0 Main is the picklock mini-game quest; it matched on the first grade. Shapes:
  - The first lock-alive test is CScriptThing::IsAlive expanded inline (`PImp_Data && PImp_Data->IsThingAlive()` through a `__forceinline` helper). The second is the direct 0x4AB130 call.
  - The movie handle sits in an inner block, so its out-of-line dtor 0x6E7B80 runs before DeactivateQuestLater.
  - Every exit is written in full.

## Parked for Fable 5.1 (6)
- 00bf2240 CEnginePrimitiveRenderUtil::SetupOutlineSmoothFillStates DIFFER(1595v1589): the colour copy lands in the distance argument slot (retail: the effect slot); B is kept in bl where retail keeps R; one or/test commutation at +0x28cc; C4DVector tail copy register rotation | tried: named colour local, by-field copies, StateMaskFirst/MaskLocal SetState, argument-slot reuse | next: write the colour as a CRGBColour user-copy-ctor temporary built in the effect slot
- 00b5e0f0 CEngineScreenEffectRadialBlurRenderer::ApplyRadialBlur DIFFER(1547v1547): the +0x2a0c SetState block emits `test eax,esi / or esi,eax` where retail has `test esi,eax / or eax,esi` | tried: mask-local helper, Set member, k-sweep, 118 per-site variants | next: an R&D look at the commutative-operand order rule for inlined SetState
- 00b60f70 CEngineScreenEffectGlowRenderer::ApplyGlow DIFFER(1834v1823): ours keeps constant 0 in ebp and spills this (frame 0x20); retail keeps this in ebp and writes 0 as immediates (frame 0x1c) | tried: the landed ShowGlowTexture model, zero-constant locals, splitting the blocks | next: cut real uses of the 0 constant (R&D register-priority rule)
- 00bfd7c0 CEngineWaterBackgroundSubPatch::Load DIFFER(1203v1231): this/stream register swap (ours this=ebp, stream=ebx); the vertex-block size stays in memory where retail keeps it in esi across ReadBlock | tried: named size local, decompress loop forms, stand-in ReadBlock | next: add real uses of the stream pointer to raise its priority
- 00b6e890 CEngineWaterRenderer::RenderBumpMap DIFFER(1371v1371): the last inlined SetState(+0x28b8,0) loads the core mask into esi and the state mask into edx (retail: core edx, state esi) | tried: 15 MaskLocal subsets, test/or swaps, StateMaskFirst variants (right registers, wrong load order), an offset alias, a named core local | next: a SetState variant that loads the state mask first but tests `core & state`
- 00b6fa40 CEngineWaterRenderer::RenderSeaMidground DIFFER(2019v2019): three sampler SetState blocks (+0x2ef8, +0x2f98, +0x2f0c <- [0x1396DD0]) emit `test esi,eax / or eax,esi` where retail has `test eax,esi / or esi,eax` | tried: a 3^6 plain/MaskLocal/swapped sweep (best leaves 3 sites), a [type][stage] sampler-array helper (worse) | next: as 00b6e890; both are the same commutation family

## Skipped
21 rows were decoded only and never compiled. They are low-yield x87-heavy or very large shapes, or were not reached within the budget. STATUS.tsv has a one-line reason for each:
- 00bf18b0 AddGeneratedObjects (222 x87)
- 00b3c5d0 SetupAlphaStippleTexture (file streams, 64-bit)
- 00b891b0 OutlineGlow ctor (49 calls)
- 00b4ef80 UpdateSpotLightShadowSpace (234 x87)
- 00b4fc40 SetupShadowFuncRenderStates
- 00b63c00 RenderSun and 00b640e0 RenderMoon (fast inverse-sqrt table, __ftol2)
- 00b65a20 RenderOuterSky
- 00b665e0 GetEnvironmentMap (fptan)
- 00b2b180, 00b2cb40, 00b2d330 (surface composition)
- 00be9920 WaterSettings ctor (fcos)
- 00bea110 UpdateConstants (150 x87)
- 00b51570 BuildBuffers
- 00b54890 RenderRain and 00b54e30 RenderSnow (C3DVector vectors in nested x87 loops)
- 0051f480 and 005248b0 (EntityRuntimePersistence string parser and zlib chain)
- 006b42f0 CEnvironment::RenderUpdate (151 x87)
- 00c604e0 (CEnvironmentSpellEffectInterface ctor)

## Label corrections
- 0x006E7B60 is labelled CCarriedReadableDef. It is the script movie-object (CScriptGameResourceObjectMovieBase) ctor; 0x006E7B80 is its dtor.
- 0x004AA840 is labelled ~C3DClothPrimitive. It is ~CScriptThing.
- 0x007E74D0 is labelled ~CScriptGameResourceObjectMovieBase. It is the scripted-thing (CScriptGameResourceObjectScriptedThingBase) dtor.
- 0x00CBE371 Is2DDistanceBetweenThingsUnder is `__fastcall(const CScriptThing&, const CScriptThing&, float)`.
- 0x00485F3A "deque::insert" is really vector<unsigned char>::insert.
- 0x00BDA0D0 / 0x00BDA090 free the VB/IB; 0x00AC20A0 / 0x00AC20B0 are VB Lock/Unlock.
- CScriptThing virtual slot 21 (0x004AAB30) is MsgIsHitBy(const CCharString&), and slot 75 is IsAlive 0x004AB130.
- 0x00C604E0 is probably not the CEnvironmentSpellEffectInterface ctor: it builds a 0x4f0-byte object with vtable 0x12B9340.
- The landed Render_00b33010 misnames globals 0x1436E3C and 0x1436E38.
