# OUTLINE 0x00B4D370 void CEngineShadowRenderer::UpdateShadowSpace(const CEngineCamera& camera)  (7176 b, ret 4)

## Frame facts
- `sub esp,3D0h` first; then push ebx (0x00B4D37B), ebp (0x00B4D38B), esi, edi (0x00B4D38C/0x00B4D397) scheduled into the first block.
  No ebp frame, no SEH, no /GS. Final esp-relative offsets below (after all 4 pushes). camera = [esp+3E4h]. this = ebx all along.
- Epilogue 0x00B4EF6B: pop edi, esi, ebp, ebx; add esp,3D0h; ret 4. No cold blocks after the epilogue; no jump tables.

## Frame map (final esp offsets; slots are shared by disjoint lifetimes)
| range | contents |
|---|---|
| 0x10..0x30 | float scratch: 0x10 1/det, later yMax (-1e30 init), later n; 0x14 u / inverse-E41 / misc; 0x18 j counter, f/n, xMin(1e30); 0x1C; 0x20 uv weight / p.Z*0 CSE / candidate; 0x30 i counter (int, fild), later xMax (-1e30) |
| 0x24..0x2C | C3DVector p (grid point being built; AccurateNormalise this @0x00B4DA64) |
| 0x34..0x63 | CMatrix3x4 inverse (SetInverse of the view copy; E13/E23/E33/E41 never stored) |
| 0x34..0x73 | then CMatrix4x4 rot (light-space 2D rotation / perspective; Initialise(16 floats)) |
| 0x74, 0x78..0x80 | float scratch (1-u; yMax 2nd pass; TransformPoint staging x/y/z) |
| 0x84..0x88 | C2DVector dir (later the y-axis vector of the non-perspective branch) |
| 0x8C..0xBB | CMatrix3x4 view = CameraViewMatrix (rep movsd 12 from this+0x27C) |
| 0x8C..0xC7 | then C3DVector frustum[5] = inverse * {(0,0,0),(-M,-M,M),(M,-M,M),(-M,M,M),(M,M,M)} |
| 0x8C..0xCB | then CMatrix4x4 PostMultiply `result` temporaries / identity temp (rep movsd 16 back) |
| 0xCC..0xF0 | float scratch (x*0.0 CSE products of the four corners, sums) |
| 0xF4..0xFC | C3DVector light_vector |
| 0x100..0x12F | CMatrix3x4 local_to_base = CalcLocalWorldToBaseShadowSpace(...) (hidden-return target) |
| 0x100..0x13F | then last PostMultiply `result` (after local_to_base is dead) |
| 0x144..0x154, 0x298..0x2A4 | float scratch |
| 0x158..0x28F | C3DVector light_points[26] (light/base space) |
| 0x2A8..0x3DF | C3DVector points[26]: [0] = frustum[0] (camera), [1 + i*5 + j] = 5x5 far-plane grid |

## Block-by-block (retail order)
```
// 0x00B4D370
C3DVector light_vector = g_pEngineLightingManager->PeekDiffuseVector();        // [0x01436E9C]+0x30..0x38 -> [esp+F4h]
light_vector.AccurateNormalise();                                               // call 0x00A14440 (ecx=&[esp+F4h])
SceneData.ShadowLightVector = light_vector;                                     // this+0x104 (stores interleaved with arg pushes)
CMatrix3x4 local_to_base = CalcLocalWorldToBaseShadowSpace(light_vector, MaxShadowDistance * 2.0f, 512.0f);  // `fadd st,st`; call 0x00B4B8B0
// 0x00B4D3F6
CMatrix3x4 view = CameraViewMatrix;                                            // rep movsd 12 -> [esp+8Ch]
CMatrix3x4 inverse; inverse.SetInverse(view);                                   // 0x00B4D40A..0x00B4D5C1, CMatrix3x4.h SetInverse (FABLE_RETAIL_CMATRIX3X4); 1/det -> [esp+10h]
// 0x00B4D4E7: M = MaxShadowDistance loaded as int bits into eax/ecx/edx (the +M stores) and `fld; fchs` (the -M stores 0x00B4D607)
C3DVector frustum[5] = { (0,0,0), (-M,-M,M), (M,-M,M), (-M,M,M), (M,M,M) };     // frustum[1..4] stored to [esp+98h..C4h]
for k in 0..4: inverse.TransformPoint(frustum[k]);   // in place, fully unrolled 0x00B4D5C1..0x00B4D8C4; VC factors x*0+y*0+z*0 into (x+y+z)*0 for k=0
points[0] = frustum[0];                              // 0x00B4D7CD..0x00B4D81E ([esp+2A8h..2B0h])
// grid: 0x00B4D7ED [esp+30h]=0 (i), ebp=&points[1]; outer head 0x00B4D8C6, inner head 0x00B4D8F0
for (long i = 0; i <= 4; ++i) {                       // `cmp eax,4 ; jle` 0x00B4DAED (i in [esp+30h], fild)
    float u = i * 0.25f;  float one_minus_u = 1.0f - u;            // [esp+14h], [esp+74h]
    for (long j = 0; j <= 4; ++j) {                   // edi, mirrored in [esp+18h] for fild; `cmp edi,4 ; jle` 0x00B4DAE2
        float v = j * 0.25f;
        w_uv = u*v; w_u1v = u*(1-v); w_1uv = (1-u)*v; w_1u1v = (1-u)*(1-v);
        inv = 1.0f / (w_1uv + w_1u1v + w_u1v + w_uv);  each w *= inv;          // fdivr [1.0] 0x00B4D938
        C3DVector p = frustum[1]*w_uv + frustum[2]*w_u1v + frustum[3]*w_1uv + frustum[4]*w_1u1v;  // exact term order: 0x00B4D95E..0x00B4DA92
        p.AccurateNormalise();                        // call 0x00A14440 at 0x00B4DA96 (ecx=&[esp+24h])
        p *= MaxShadowDistance;                       // fld [ebx+2BCh] once, three fmul/fstp into p, copied to points[]
        points[1 + i*5 + j] = p;                      // esi walks +0xC, ebp +0x3C per row
    }
}
// 0x00B4DAFA: light_points[k] = points[k] transformed by local_to_base, k = 0..25 (13 iterations x 2, eax += 18h, cmp 138h)
for (k = 0; k < 26; ++k) { light_points[k] = points[k]; local_to_base.TransformPoint(light_points[k]); }
//   X row: E31*z, E21*y, E11*x, +E41 (CMatrix3x4.h FABLE_RETAIL_CMATRIX3X4_TRANSFORMPOINT_ZYX_SUMS form); Y: E12*x, E32*z, E22*y; Z: E23*y, E13*x, E33*z
// 0x00B4DCCA
C2DVector dir(light_points[13].X - light_points[0].X, light_points[13].Y - light_points[0].Y);   // [esp+84h]
dir.AccurateNormalise();                              // call 0x00A14510
CMatrix4x4 rot; rot.Initialise(dir.Y, dir.X, 0,0,  -dir.X, dir.Y, 0,0,  0,0,1,0,  0,0,0,1);   // [esp+34h..70h], 0x00B4DD02..0x00B4DDF2
//   (x_axis = (dir.Y,-dir.X) also kept in [esp+18h]/[esp+1Ch]: Initialise(x.X, y.X, 0,0, x.Y, y.Y, 0,0, ...) with y = dir)
// 0x00B4DD1C: y range of the four far corners light_points[1], [5], [21], [25] (= grid (0,0),(0,4),(4,0),(4,4))
float y_min = 1e30f, y_max = -1e30f;                  // y_min in st, y_max [esp+10h] (imm F149F2CA)
for each corner c: { C3DVector q = c; rot.PerspectiveTransformPoint(q);   // only q.Y live; w = X*0+Y*0+Z*0+1
                     if (q.Y < y_min) y_min = q.Y; if (q.Y > y_max) y_max = q.Y; }    // `test ah,41h` / `test ah,5` pairs
//   blocks at 0x00B4DD1C (c=[1]), 0x00B4DE56 ([5]), 0x00B4DEF7 ([21]), 0x00B4DF98 ([25]); x*0.0 products kept in frame and REUSED below
// 0x00B4E033
float near_y;
if (y_min >= 0.0f) near_y = y_min;                    // jp 0x00B4E0E6
else if (y_max > 0.0f) near_y = 0.0f;                 // 0x00B4E057 -> jmp 0x00B4E0E6
else { rot.PostScale(C3DVector(-1.0f, -1.0f, 1.0f)); near_y = -y_max; }   // 0x00B4E062: -0.0f stores, fmul [-1.0]
// 0x00B4E0E6  (cl = PerspectiveShadowsEnabled kept for 0x00B4E654)
float t;
if (PerspectiveShadowsEnabled
    && (t = (near_y - PerspectiveInterpStart) / (PerspectiveInterpEnd - PerspectiveInterpStart),
        GFLimit(t, 0, 1), t > 0.0f)) {                // 0x00B4E10A..0x00B4E13D jump-threaded GFLimit<float,int,int> + `if (t > 0)`
    // 0x00B4E143 perspective branch
    float f = ForegroundBufferSpan / MaxShadowDistance;
    float n = f / ((1.0f - t) * f + t - f);           // [esp+10h]
    float limit = f / n;                              // [esp+18h]
    float scale = 1.0f;                               // st, `fld [1.0]` 0x00B4E16D
    for each corner c in {[1],[5],[21],[25]}:         // 0x00B4E16D, 0x00B4E23D, 0x00B4E301, 0x00B4E3C5
        { C3DVector q = c; rot.PerspectiveTransformPoint(q);              // q.X -> [esp+24h], q.Y in st
          float r = 1.0f / (n + q.Y);  float px = q.X * r, py = q.Y * r;    // py -> [esp+D0h]
          if (fabs(px) > limit) { float z = py / fabs(px) * limit;          // fdivr [D0h]; fmul [18h]
                                  float cand = n * z / (1.0f - z) / q.Y;    // -> [esp+20h]
                                  if (scale > cand) scale = cand; } }      // first corner compares the constant 1.0
    for (k = 0; k < 26; ++k) light_points[k] *= scale;                     // 0x00B4E486: 13 x 6 floats
    SceneData.ShadowFadeEnd = scale * MaxShadowDistance;                   // this+0x100, 0x00B4E4DE
    CMatrix4x4 perspective; perspective.Initialise(n+1,0,0,0,  0,n+1,0,1,  0,0,n+1,0,  0,0,0,n);  // n+1 computed once (`fadd [1.0]`)
    rot.PostMultiply(perspective);                    // 0x00B4E4F0 loop (edx=4), result temp [esp+8Ch], rep movsd 16 -> [esp+34h]
    // jmp 0x00B4E7CA with st0 = rot.E11 (pre-load for the 0.5 scale)
} else {
    // 0x00B4E587 non-perspective / t <= 0 branch
    C2DVector a(light_points[15].X - light_points[11].X, light_points[15].Y - light_points[11].Y);   // grid (2,4)-(2,0)
    C2DVector b(light_points[23].X - light_points[3].X,  light_points[23].Y - light_points[3].Y);    // grid (4,2)-(0,2)
    C2DVector x_axis, y_axis;                         // x_axis [esp+18h], y_axis = dir slot [esp+84h]
    if (a.GetSquaredMagnitude() > b.GetSquaredMagnitude()) { x_axis = a; y_axis = C2DVector(-a.Y, a.X); }  // fcompp 0x00B4E605
    else { y_axis = b; x_axis = C2DVector(b.Y, -b.X); }                                                   // 0x00B4E630
    rot.Initialise(x_axis.X, y_axis.X, 0,0,  x_axis.Y, y_axis.Y, 0,0,  0,0,1,0,  0,0,0,1);              // 0x00B4E654
    if (PerspectiveShadowsEnabled) {                  // `test cl,cl` 0x00B4E654 / je 0x00B4E7BE
        // 0x00B4E6DE
        float len  = sqrt(|frustum[2] - frustum[1]|^2);           // fsqrt inline (sqrt intrinsic or C3DVector::GetMagnitude)
        float half = 0.5f * len;  float inv_max = 1.0f / MaxShadowDistance;   // [esp+78h]
        float r    = sqrt(MaxShadowDistance*MaxShadowDistance + half*half);
        SceneData.ShadowFadeEnd = ForegroundBufferSpan / (MaxShadowDistance / r * MaxShadowDistance * inv_max * len) * MaxShadowDistance * 2;  // op order 0x00B4E75A..0x00B4E778 (`fadd st,st`)
        float s = SceneData.ShadowFadeEnd * inv_max;
        for (k = 0; k < 26; ++k) light_points[k] *= s;            // 0x00B4E784, jmp 0x00B4E7CA
    } else
        SceneData.ShadowFadeEnd = MaxShadowDistance;              // 0x00B4E7BE (int move)
}
// 0x00B4E7CA
if (SceneData.ShadowFadeStart < 0.0f) SceneData.ShadowFadeStart = 0.0f;              // (GFLimit<float,int,double>-like)
else if (SceneData.ShadowFadeStart > SceneData.ShadowFadeEnd * 0.9) SceneData.ShadowFadeStart = SceneData.ShadowFadeEnd * 0.9;  // DOUBLE 0.9 @0x012A23B8
// 0x00B4E80E
rot.PostScale(C3DVector(0.5f, 0.5f, 0.5f));          // 12 fmul [0.5] (st0 = E11 already loaded)
rot.PostTranslate(C3DVector(0.5f, 0.5f, 0.5f));      // 0x00B4E8B2..0x00B4E95A
// 0x00B4E95C: x/y range of all 26 light points through rot (13 iterations x 2, ecx += 18h)
float x_min = 1e30f /*[esp+18h], imm 7149F2CA @0x00B4E814*/, x_max = -1e30f /*[esp+30h]*/, y_min2 = 1e30f /*st*/, y_max2 = -1e30f /*[esp+74h]*/;
for (k = 0; k < 26; ++k) { C3DVector q = light_points[k]; rot.PerspectiveTransformPoint(q);   // w via `fld [1.0]; fdiv st1`
                           x_max = max(x_max,q.X) (fst), x_min = min, y_max2 = max(q.Y), y_min2 = min; }
// 0x00B4EB23
rot.PostTranslate(C3DVector(-x_min, -y_min2, 0.0f)); // E?3 += E?4*0.0 kept, int copies of E13/E23 at 0x00B4EB5B/0x00B4EB91
rot.PostScale(C3DVector(1.0f / (x_max - x_min), 1.0f / (y_max2 - y_min2), 1.0f));   // `*1` folds to mov-copies (0x00B4EC5D)
// 0x00B4EC67: identity into a temp, then copied (WorldToShadowBufferMatrix = CMatrix4x4(identity-enum))
SceneData.WorldToShadowBufferMatrix = CMatrix4x4(<EMatrix34Init identity>);   // for j<4 { M[0][j]=(j==0)?1:0; ..M[3][j] } -> [esp+8Ch], rep movsd -> this+0xC
SceneData.WorldToShadowBufferMatrix.PostTranslate(-SceneData.CameraPos);     // 0x00B4ED04: three fchs of this+0x110..0x118, rows fully unrolled; eax = this+0xC kept
SceneData.WorldToShadowBufferMatrix.PostMultiply(CMatrix4x4(local_to_base)); // 0x00B4EDD0 loop (esi=4), 4x4-from-3x4 temp fully scalar-replaced (0,0,0,1 column -> (m0+m2+m1)*0.0+m3)
                                                                             //   result [esp+8Ch], rep movsd -> this+0xC (0x00B4EE85)
SceneData.WorldToShadowBufferMatrix.PostMultiply(rot);                       // 0x00B4EEA4 loop, result [esp+100h], rep movsd -> this+0xC (0x00B4EF41)
BuildDerivedShadowMatriciesFromWorldToShadowMatrix();                        // call 0x00B4BA60
CalcClipVolume(camera);                                                      // call 0x00B4BCC0
```

## Notes for the author
- Every helper above must be expanded (no calls except the five): FORCEINLINE CMatrix4x4 Initialise(16)/Initialise(3x4)/ctors/PostMultiply/
  PostScale/PostTranslate/PerspectiveTransformPoint/SetIdentity, or a `// Flags: /d2inlT<N>` like r3004 (/d2inlT314).
- PerspectiveTransformPoint(C3DVector& p): x=(X*E11+Y*E21+Z*E31+E41)*iw, y likewise col 2, z col 3, iw = 1/(X*E14+Y*E24+Z*E34+E44)
  (FableWin 241 b / 87 insns -- check with fwsize). In the corner passes VC propagates rot's constant 0/1 entries and CSEs p.Z*0.
- Corner index set {1,5,21,25} and the 26-point loops: VC unrolled the 26-loops x2; the 4-corner passes are fully expanded (4 blocks) --
  either 4 explicit blocks or a 2x2 loop that VC unrolls; measure.
- Uncertain (verify on the asm): exact weight/term order of the grid sum (0x00B4D95E..0x00B4DA92) and of the non-perspective ShadowFadeEnd
  expression; whether `view` is a named copy (the rep movsd proves a copy exists).
