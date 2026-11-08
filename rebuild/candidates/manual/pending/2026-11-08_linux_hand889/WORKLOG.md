# Chunk 889 worklog

## Extents (xdis, last ret + 0xCC padding, no inner int3)
- 0x00BD5C90 ends 0x00BD6807 (`ret 4` at 0x00BD6804) = 2935 bytes, one function.
- 0x00BCA950 ends 0x00BCB7F5 (`ret 8` at 0x00BCB7F2) = 3749 bytes, one function.

## 0x00BD5C90 CEnginePrimitiveManagerAnimatedMeshesBase::RenderPrimitiveToShadowBuffer
Not a byte twin of 0x00BA6240: same skeleton (scoped shaders, capture block, scene-data ClipSphere, mipmap inline), but the
transform comes from the virtual CalcInterpolatedObjectTransform (slot 33) into a frame CMatrix3x4, the bounding radius is
(|centre| + radius) * Scale + BoundingRadiusFix (renderer 0x01436E80 +0x23C), SetupMesh(distance, radius*Scale) 0x00BCF2E0,
RenderInfo.ObjectTransform = transform, morph hidden materials for types 11 / 38, technique block before the loop, per
primitive GetDiffuseTexture 0x00BA9360 + AttachTextureToStage(stage), PrepareForRender(0x401), RenderPrimitiveBlocksIndexBuffer
0x00BCFB00 with GetBoundingTransformations 0x00BD2D90.
- a1 first draft (twin models): 2869v2935, 619 lines.
- a3 C4DVector temp / a5 named constant in its own block: constant packs with the mipmap sphere (retail +0x30).
- a6 morph branches spelled per type without a shared morph reference (no tail join): 2931v2935.
- a6s1 `float distance = CalcDistanceFromCamera(centre); float mesh_radius = ...; SetupMesh(distance, mesh_radius)`: retail order
  (offset vector, radius*Scale stored to [ebp+44h], then sqrt).
- a7 radius from by-value GetBoundingSphere() copies: R loaded first (retail); magnitude product order still Z*Z first.
- a8 `const bool transparent = material->Transparent;` (bl in retail): 152 lines, frame 0x2D4 and EVERY slot = retail.
- Transform x87 order: retail rows E1k*x, E3k*z, E2k*y with x/y/z kept on the x87 stack. 30+ probe forms (inline with
  reference vector, named xx/yy/zz copies, in-body sums, by-value vector, all 12 term orders): by-value vector + `E1k*x + E3k*z +
  E2k*y` (qq3) gives the retail term order in all rows, but the first product is `fld st(2) ; fmul [E11]` instead of
  `fld [E11] ; fmul st(3)`.

## Row 1 update (22:35)
- ty6 (88) -> cc1 (86): constant block with `constant.Y = MinWClamp` assigned BEFORE `constant.X = 128.0f`
  gets the early `mov edx,[ecx+0x25c]`; only the X/Y store order is now swapped. Local float temp / ctor
  forms (cc3-cc6) go through x87 fld/fstp (worse). Best so far: probe/best1.cpp (= cc1).
- Moving to row 2 now; come back to row 1 residues after.

## 0x00BCA950 CEnginePrimitiveManagerSpriteTrails::RenderSpriteTrail (23:00-23:08)
- Built from t2a: models for the pool, tree filer and sprite trail; inline SelectVertexBuffer / DrawBuffersNonIndexed / FlushRender.
  Progression: t2a -> t2j (Min form) -> t2s/t2t (colour Initialise + separate doubling) -> t31 (camera reassign) = 3749v3749, 1031 lines.
- Loop forms: t34 (n>0, 3733), t35 (n!=0, 3737), t40 (n beside i, 3748), t45 (n>=1, 3733), t44 (tangent ref, 1096 lines). None put n in ebx.
- t36 / t46 `if (id > 0)` in ReturnResource: retail `test ; jbe`; 205 register+frame-blind lines (t31: 212). Parked as differ (t46).
- t38 / t39 (no segments variable): c2 folds `(points-1)*16+2` to `points*16-14`, where retail has `lea ; shl ; add 2`, so keep the variable.
- t47..t50 direction / position term orders and tangent references: no gain.
- Mistake: t38..t50 were first graded two at a time on the same address. q.py shares a work dir, so the paired results were identical. All were re-graded one at a time; the numbers above are the sequential ones.
- c2prio (t46): points prio 89, allowed ebx only, split (esi #196); trail_size prio -5 gets ebx #171; n = temp 279 prio 146.
- Row 1 parked as best1 (cc1), with /d2inlS100 confirmed needed (best1_noinl: 2659v2935).
