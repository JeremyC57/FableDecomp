# Chunk 727 (C3DAnimationManager / CEngineShadowRenderer / CEngineWeatherRenderer / CEngineSkyRenderer)

Result: 1 matched (0x00B625E0 CEngineSkyRenderer ctor, RELOCATION_MATCH, behaviour PASS under vland), 3 parked near misses
(differ/), 10 not attempted (budget; see STATUS.tsv for the blocker of each). Overlay: hdr/ = chunk 723 round 2 overlay + the
changes below. Regrade: `FABLE_HDR_OVERLAY=$D/hdr LANDVERIFY_WORK=$D/lv python3 -u $T/vland.py payload.json $S/batch/chunk727_oracle.tsv`.

## Matched
- 0x00B625E0 `??0CEngineSkyRenderer@@QAE@ABVCEngineInit@@@Z` (src/00b625e0.cpp, test/00b625e0.cpp). Levers:
  1. CEngineComponent() declared out of line (0x00B59710) in the FABLE_RETAIL_CENGINECOMPONENT branch: retail calls it.
  2. Cloud vectors as a nested loop (`for i<2 { for j<2 History[i][j] = C2DVector(0,0); CloudVector[i] = C2DVector(0,0); }`):
     inner unrolls, outer stays a 2-trip loop over two stack zero temporaries (retail). A flat body unrolls fully (dl 930).
  3. GetDef<CSkyDef>(const CCharString&, long, CDefPointer<const T>*) body in the TU (declared in the CDefinitionManager retail
     branch, defined in the source like landed 0x004084B0 does for the (long) overload); it inlines.
  4. The SetDynamicCast inside GetDef: retail re-reads the def temporary after releasing the old PSkyDef. engine/CDefPointer.h's
     SetDynamicCast caches it (Assign(other.Object)) -> dl 48, tail only. Re-reading in the header (differ/CDefPointer_reread_option.h)
     matches this row but regresses landed 0x0047A9F0 CHeroLogBook::AddEntry (1149v1105: VC7.1 then inlines its GetDef<T>(long),
     retail calls 0x0042B312; `#pragma auto_inline(off)` around that body does not stop it). The matched source writes the call as
     the copy assignment `*def = reinterpret_cast<const CDefPointer<const T>&>(base)` (the header's copy operator= re-reads).
     COORDINATOR DECISION: FableWin.pdb proves GetDef<CSkyDef> calls SetDynamicCast<const CDefClassBase>; if the cast spelling is
     not acceptable, park the row with differ/00b625e0_setdynamiccast.cpp (dl 48) until a SetDynamicCast form satisfies both rows.
  Cloud settings come from CEnvironmentDef (Ego_r - 4: CloudSpeedMultiplier +0x118 .. CloudHeightOffset +0x128) through the
  established label g_pEngineEnvironmentDef 0x01436E24. Retail CEngineInit InitFlags is +0x60 (no DebugString; as landed 0x0042E204).

## Parked (differ/)
- 0x00B4BCC0 CalcClipVolume, dl 27 (336 grades). Scheduling window: reset stores / first plane's lea and pushes ~3 slots early.
  Inert: /G5 /G6 /GB (/G7 worse), loop vs unrolled transforms, Reset placement, empty CMatrix4x4/CPlane ctors, T spellings.
- 0x00B4C550 CalcSpotLightClipVolume, dl 20 (39 grades). Same window residue (6 slots), one `inc edx`.
- 0x00A9DC20 ApplyPartialAnimationAlpha, dl 23 / 709v706 (43 grades). x87 reassociation of the inlined slerp dot (ours Z,Y,X,W,
  retail Y,X,Z,W) and the lerp tail operand order. Register convention solved (`static void __stdcall` + same-TU caller).

## Overlay changes (hdr/, all default=same in hdrcheck2)
- NEW engine/CPlane.h, engine/CMatrix4x4.h (SetInverse 0x00A5CB00, inline PerspectiveTransformPoint), engine/CShadowClipVolume.h
  (Reset / AddPlane / SetSphere), engine/CShadowRendererSceneData.h (Ego_r 0x240 layout).
- engine/CEngineShadowRenderer.h retail branch: SceneData at +0x0C; CalcClipVolume / CalcSpotLightClipVolume / UpdateShadowSpace.
- engine/CEngineCamera.h retail branch: NoScaleViewTransform .. FrustrumPlanes[4] (+0x114..+0x200), PeekCameraPos /
  PeekFrustrumPlane / PeekLookVector inline.
- engine/C3DBoundingSphere.h: `(const C3DVector&, float)` ctor under FABLE_RETAIL_C3DBOUNDINGSPHERE.
- engine/C4DVector.h retail branch: operator+ / operator- / operator*(float) / Interpolate.
- engine/CEngineComponent.h retail branch: `CEngineComponent(); // 0x00B59710` (out of line).
- engine/CEngineSkyRenderer.h: new FABLE_RETAIL_CENGINESKYRENDERER branch (typed members at the Ego_r offsets, ctor, dtor,
  BuildBuffers); default branch unchanged.
- engine/CDefinitionManager.h retail branch: GetDef(const CCharString&, long, CDefPointer<const T>*) and GetPDefFromClassIndex.
- NOT in the overlay: engine/CDefPointer.h (the re-read option is differ/CDefPointer_reread_option.h, see above).

## Siblings (SIBLINGS.tsv)
293 landed rows (every SetDynamicCast user plus every includer of a changed header / macro) regraded with the final overlay.
Pre-existing failures that are NOT from this chunk: 0x00B3BE10 BuildAlphaStippleTexture SRC_FAIL also with the bare 723 round 2
overlay (its CEngineResourceManager.h retail branch). 0x0047A9F0 AddEntry matches with the final overlay (it only broke with the
CDefPointer re-read option).

## Gotchas (one line each)
- A same-TU `static` that retail calls with register args and callee-pop: `static void __stdcall` + a same-TU caller (so it is emitted) reproduces VC7.1's custom convention without LTCG (0x00A9DC20).
- x87 sum order: a loop of an inlined transform keeps one reassociation order for every clone; four explicit calls rotate it (0x00B4BCC0).
- Parenthesising an x87 sum pins its order but changes the IR count, which moves VC7.1's scheduling window: unparenthesised + loop was the better trade (dl 128 -> 27).
- CDefPointer SetDynamicCast: retail re-reads the source after Release in an inlined GetDef(name, index) (0x00B625E0) while the header caches it; re-reading in the header makes landed 0x0047A9F0 inline GetDef<T>(long).
- Zeroing a C2DVector[2][2] + C2DVector[2] pair: retail's 2-trip loop over stack zero temporaries is a nested loop (inner unrolled); a flat body unrolls into register stores.
