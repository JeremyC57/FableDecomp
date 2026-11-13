# OUTLINE 0x00B5CA70 CEngineScreenEffectRadialBlurRenderer::CaptureBackBuffer()

## Frame facts
- `sub esp,88h` then push ebx, ebp, esi, edi (after the sub; `mov eax,[13CB508h]` and `mov esi,ecx` are scheduled between).
  No ebp frame, no SEH, no /GS. Plain `ret`. ebx = 0 from 0x00B5CAF6 on.
- Offsets below are relative to esp AFTER the four pushes ("body"); add the live push depth when reading the listing.
  Locals 0x10..0x97.

## Frame map (body offsets)
| slot | holds | ctor / dtor |
|---|---|---|
| 0x94 | CScopedVertexShaderConstantLayout.OldIndex (= [0x013BC478]) | whole function; restored at 0x00B5E0BC |
| (none) | CRenderStateScopedCaptureBlock (empty; StartCaptureBlock inline at top) | ~ = RestoreCaptureBlock @0x00B5E0CE (last) |
| 0x44..0x63 | CRenderTarget target (vptr 0x0129C800 @0x44, RenderToSurface @0x48 {vptr 0x0122F84C, PD3DSurface 0x4C=0, AllocationSource 0x50=0}, ZBufferSurface @0x58 {vptr, 0x5C=0, 0x60=0}) | one per pass (4 scopes, same slot): CBaseClass() 0x0099A2D0 first, then inline field init; dtor = ZBufferSurface (vptr store + Uninitialise) @0x58, RenderToSurface @0x48, ~CBaseClass 0x0099A2E0 @0x44 |
| 0x7C..0x8B | pass A: CSurface temporary of GetSurfaceLevel(0) (dtor right after AttachTarget); passes B-D: `const CTexture* textures[4]` (0x7C,0x80,0x84,0x88) | |
| 0x14..0x23 | passes B-D: GetSurfaceLevel temporary CSurface @0x14; all passes: C4DVector temporaries for SetPixelShaderConstant (0x14,0x18,0x1C,0x20) | |
| 0x24,0x25 | pass A only: CScopedShadersEnable {OldVertexEnable, OldPixelEnable} | ctor @0x00B5CB52, dtor @0x00B5D06F |
| 0x30 | pass A: 1/Range, then `CTexture& backBuffer` (CopyBackBufferToTexture result), then &UpdateFlags spill; pass B: angle0 copy | |
| 0x3C | `float twist = CurrentParameters.Twist` (loaded 0x00B5D0D9, live through passes B-D) | |
| 0x10,0x24,0x28,0x2C,0x34,0x38,0x40,0x68..0x78,0x8C,0x90 | pass B-D float temporaries (scale terms, offsets, angles, pow results), 0x10 = shader-manager spill in PrepareForRender, 0x90 = &Blur256ToBlur256Verts (CSE'd, used by pass D after esi is overwritten) | |

## Block-by-block

### Prologue / whole-function scope (0x00B5CA70 - 0x00B5CAE7)
```cpp
static unsigned short QuadIndices[6] = { 0, 1, 2, 0, 2, 3 };        // 0x0139A538
CRenderStateScopedCaptureBlock capture;                              // 0x00B5CA76-0x00B5CAA0 inline StartCaptureBlock
CScopedVertexShaderConstantLayout layout(2);                         // 0x00B5CAA6 old -> [0x94]; call 0x0098B5E0(2) @0x00B5CABD
CGPUScratchBufferTextureManager& pool = GPResourceManager->GetGPUScratchBufferTextureManager();  // [0x01436E98]+0x3190 in edi
BlurTexture256_1 = pool.StartTextureUse(BlurTextureHandle256_1);    // 0x00F36EE0 @0x00B5CAD4, stored +0x0C
BlurTexture256_2 = pool.StartTextureUse(BlurTextureHandle256_2);    // @0x00B5CAE2, stored +0x10
```
### Pass A: back buffer -> BlurTexture256_1, threshold (0x00B5CAE7 - 0x00B5D0D9)
```cpp
{
  CRenderTarget target;                                              // 0x00B5CAEE CBaseClass(), inline init 0x00B5CB06..
  target.AttachTarget(BlurTexture256_1->GetSurfaceLevel(0));         // GetSurfaceLevel 0x009F9E00 @0x00B5CB26 (tmp @0x7C),
                                                                     // AttachTarget 0x00A10E60 @0x00B5CB30, tmp dtor @0x00B5CB3D
  NEngineInternal::GPDisplayManager->SetRenderTarget(target);       // 0x009BF160 @0x00B5CB4D
  CScopedShadersEnable shaders(true, true);                         // 0x00B5CB52..0x00B5CB7B (Enable* only if != 1)
  GPShaderRenderManager->ApplyVertexShader(ScreenCopyVertexShader, false);   // +0x20, 0x00988020 @0x00B5CB8B
  GPShaderRenderManager->ApplyPixelShader(ScreenCopyPixelShader);           // +0x30, 0x00988140 @0x00B5CB9A
  // 0x00B5CB9F: fld 1.0f is loaded here but used at 0x00B5CCA0 (scheduling of 1.0f / Range)
  GPShaderRenderManager->SetPixelShaderConstant(5, C4DVector(CurrentParameters.ThresholdR, ThresholdG, ThresholdB, 0.3f));
        // temp 0x14..0x20 (X=+0x198, Y=+0x19C, Z=+0x1A0 loaded eax-first; W = 0x3E99999A); copy to PSConstTable.begin()+5 (+0x50); UpdateFlags |= 2 (ebp=2)
  GPShaderRenderManager->SetPixelShaderConstant(6, C4DVector(1.0f, 1.0f, 1.0f, 1.0f));   // +0x60, stored through the temp
  GPShaderRenderManager->SetPixelShaderConstant(7, C4DVector(0.0f, 0.0f, 0.0f, 0.0f));   // +0x70 (3rd |= is `mov/or/mov` form)
  float invRange = 1.0f / CurrentParameters.Range;                   // fdiv [+0x1B8] @0x00B5CCA0 -> [0x30]
  GPShaderRenderManager->SetUserVertexShaderConstant(0, AreaCenter.X * 2.0f - 1.0f, 1.0f - AreaCenter.Y * 2.0f, invRange, invRange);
        // `fadd st,st` = *2 ; x: fld [+0x178] fadd st,st fsub 1.0 ; y: fld [+0x17C] fadd st,st fsubr 1.0 ; call @0x00B5CCDD
  GPShaderRenderManager->SetUserVertexShaderConstant(1, 0.5f, 0.0f, 0.0f, 0.0f);         // @0x00B5CCF2
  GPRenderManager->SetZBufferEnable(false);                          // +0x2868 (state value 0) 0x00B5CCF7
  GPRenderManager->SetPolyCullMode(POLY_CULL_NONE);                  // +0x2890 <- [0x01396FB8] 0x00B5CD61
  GPRenderManager->SetAlphaBlendEnable(false);                       // +0x28B8 0x00B5CDCC
  CTexture& backBuffer = GPDisplayManager->CopyBackBufferToTexture(); // 0x009BF5E0 @0x00B5CE32 -> [0x30]
  GPRenderManager->SetTextureUVModeU(0, TEXTURE_UV_CLAMP);           // +0x2EE4 <- [0x01396DD0] 0x00B5CE3D
  GPRenderManager->SetTextureUVModeV(0, TEXTURE_UV_CLAMP);           // +0x2F84 0x00B5CEAE
  GPRenderManager->AttachTextureToStage(0, &backBuffer);             // inline 0x00B5CF14: null test, [+0x3D00] compare, dev->SetTexture (+0x104), TextureStagesInUse max 1
  GPShaderRenderManager->PrepareForRender(0);                        // inline 0x00B5CF58..0x00B5D00F (SetVSConstants 0x0098B930, UpdatePixelShader, UpdatePixelShaderConstants, UpdateAmbient, UpdateFogColour, UpdateLightGlobals, ResetWorldTransform if !WorldMatrixIsIdentity, UpdateCombinedProjectionTransform)
  GPRenderManager->DrawAsIndexedTris(BackBufferToTextureVerts, sizeof(CTVertexColTex1), 4, QuadIndices, 2);  // 0x00A0AF40 @0x00B5D024 (+0x50)
  GPRenderManager->AttachTextureToStage(0, 0);                       // inline 0x00B5D029 (no null test: constant 0)
  GPDisplayManager->SetRenderTargetAsBackScreen();                   // 0x009BF220 @0x00B5D06A
}   // ~shaders 0x00B5D06F..0x00B5D0AC (Enable if old==1 else Disable, only when changed); ~target 0x00B5D0B1..0x00B5D0D4
```
### Pass B: BlurTexture256_1 -> BlurTexture256_2, taps k = 0..3 (step 1) (0x00B5D0D9 - 0x00B5D60F)
```cpp
float twist = CurrentParameters.Twist;                               // [0x3C], load at 0x00B5D0D9 BEFORE the state set
GPRenderManager->SetAlphaBlendEnable(false);                         // 0x00B5D0DF..0x00B5D141 (outside the target scope)
{
  CRenderTarget target;                                              // 0x00B5D145
  target.AttachTarget(BlurTexture256_2->GetSurfaceLevel(0));         // tmp @0x14 ; GetSurfaceLevel @0x00B5D178 ; AttachTarget @0x00B5D182 ; tmp dtor @0x00B5D18F
  GPDisplayManager->SetRenderTarget(target);                         // @0x00B5D19F
  GPShaderRenderManager->ApplyVertexShader(RadialBlurVertexShader, false);   // +0x28 (address kept in ebp for pass C/D)
  GPShaderRenderManager->ApplyPixelShader(RadialBlurPixelShader);           // +0x38
  GPShaderRenderManager->SetUserVertexShaderConstant(0, 0.5f, 0.5f, 0.0f, 0.0f);   // @0x00B5D1D6
  float l1 = 1.0f - CurrentParameters.Length * 0.00390625f;   // float const 0x01238C64 -> [0x28]
  float l2 = 1.0f - Length * 0.0078125f;                      // 0x012A1F34 -> [0x40]
  float l3 = 1.0f - Length * 0.01171875f;                     // 0x012A27B0 -> [0x2C]
  // dx = (BlurCenter.X - 0.5f) * Length  (fsub float 0.5 0x0122F59C, fmul [+0x1B0]); then three DOUBLE-constant products:
  float dx1 = dx * 0.00390625 /*0x012A27A8 dbl*/; [0x68]   dx2 = dx * 0.0078125 /*0x012A27A0*/; [0x6C]   dx3 = dx * 0.01171875 /*0x012A2798*/; [0x70]
  // dy = (BlurCenter.Y - 0.5f) * Length ; dy1 [0x34], dy2 [0x74], dy3 [0x78] with the same doubles
  float a0 = twist * 0.0f;    // float 0x0122DEDC; `fst [0x30] ; fstp [0x24]` (a0 is CSE'd and reused by passes C and D)
  float a2 = twist + twist;   // `fadd st,st` -> [0x10] (stored between the pushes of the next call)
  float a3 = twist * 3.0f;    // 0x0122DED4 -> [0x38]
  GPShaderRenderManager->SetUserVertexShaderConstant(0, 0.5f, 0.5f, 0.5f, 0.5f);   // @0x00B5D2CA
  SetUserVertexShaderConstant(1, cos(a0), sin(a0), 0.0f, 0.0f);                    // @0x00B5D2EF (fsin/fcos intrinsics)
  SetUserVertexShaderConstant(2, cos(twist) * l1, sin(twist) * l1, dx1, dy1);      // @0x00B5D328 (a1 = twist itself, [0x3C])
  SetUserVertexShaderConstant(3, cos(a2) * l2, sin(a2) * l2, dx2, dy2);            // @0x00B5D35D
  SetUserVertexShaderConstant(4, cos(a3) * l3, sin(a3) * l3, dx3, dy3);            // @0x00B5D392
        // each: sin computed first (y), then w,z pushed, then x = cos; `push edi(2)` for index 2 (edi = 2 reused for |= 2)
  float p0 = pow(CurrentParameters.IntensityR, 0.0);   // _CIpow, double 0x0122ED70 -> [0x10]
  float p1 = pow(IntensityR, 1.0);                      // 0x01237F00 -> [0x28]
  float p2 = pow(IntensityR, 2.0);                      // 0x01244480 -> [0x24]
  float p3 = pow(IntensityR, 3.0);                      // 0x012368F0 (left in st0)
  SetPixelShaderConstant(5, C4DVector(p0, p0, p0, p0)); // +0x50 ; 6: p1 +0x60 ; 7: p2 +0x70 (mov/or/mov form) ; 8: p3 +0x80 (p3 via fst x3 / fstp)
  const CTexture* textures[4] = { BlurTexture256_1, BlurTexture256_1, BlurTexture256_1, BlurTexture256_1 };   // 0x00B5D4D6 [0x7C..0x88]
  GPShaderRenderManager->PrepareForRender(0);           // inline 0x00B5D4FE..0x00B5D5B5 (manager pointer spilled at [0x10])
  GPRenderManager->DrawAsIndexedTris(textures, 4, Blur256ToBlur256Verts, sizeof(CTVertexColTex1), 4, QuadIndices, 2);   // 0x00A0AEA0 @0x00B5D5D7 (+0xB0)
  GPDisplayManager->SetRenderTargetAsBackScreen();      // @0x00B5D5E2
}   // ~target 0x00B5D5E7..0x00B5D60A
```
### Pass C: BlurTexture256_2 -> BlurTexture256_1, step 4 (0x00B5D60F - 0x00B5DB6B)
Same as pass B except:
- SetAlphaBlendEnable(false) 0x00B5D60F; target @0x00B5D677; source of GetSurfaceLevel = BlurTexture256_1 (+0x0C) (render INTO _1).
- Two index-0 calls first: `SetUserVertexShaderConstant(0, 0.498046875f (0x3EFF0000), 0.498046875f, 0, 0)` @0x00B5D705, then
  `(0, 0.5f, 0.5f, 0, 0)` @0x00B5D71D; later `(0, 0.5f x4)` @0x00B5D81C.
- l: 1 - Length * {0.015625f 0x0125B24C, 0.03125f 0x0129DD98, 0.046875f 0x012A2790}; dx/dy doubles {0.015625 0x012A2788,
  0.03125 0x0129C7F0, 0.046875 0x012A2780}; angles: a0 (reused), twist * {4.0f 0x0123088C, 8.0f 0x0125916C, 12.0f 0x01230884};
  pow exponents {0.0 0x0122ED70, 4.0 0x012724C8, 8.0 0x012A2778, 12.0 0x012A2770}.
- textures[4] = BlurTexture256_2 x4; DrawAsIndexedTris @0x00B5DB33 (Blur256ToBlur256Verts address also spilled to [0x90]);
  SetRenderTargetAsBackScreen @0x00B5DB3E; ~target to 0x00B5DB66. Calls: SVC @D705 D71D D81C D841 D87A D8AF D8E7; pow D8F8..D937.
### Pass D: BlurTexture256_1 -> BlurTexture256_2, step 16 (0x00B5DB6B - 0x00B5E0B1)
Same as C except: target surface from BlurTexture256_2 (+0x10) (0x00B5DBDD); l: Length * {0.0625f 0x0122DC98, 0.125f 0x0123095C,
0.1875f 0x012A2768}; dx/dy doubles {0.0625 0x012A2230, 0.125 0x01238018, 0.1875 0x012A2760}; angles twist * {16.0f 0x01238164,
32.0f 0x01230894, 48.0f 0x0125A6B4}; pow exponents {0.0, 16.0 0x012A2758, 32.0 0x012A2750, 48.0 0x012A2748};
textures = BlurTexture256_1 x4 (`mov esi,[esi+0Ch]` -- this is dead after it, so esi is reused; the vertex pointer comes from
[0x90]); DrawAsIndexedTris @0x00B5E07E; SetRenderTargetAsBackScreen @0x00B5E089; ~target 0x00B5E08E..0x00B5E0AC.
Calls: SVC @DC61 DC79 DD78 DD9D DDD9 DE0E DE43; pow DE54..DE93.
### Tail (0x00B5E0B1 - 0x00B5E0E3)
```cpp
GPDisplayManager->SetRenderTargetAsBackScreen();       // 0x00B5E0B7, OUTSIDE pass D's scope (after ~target)
}   // ~layout: SetVertexShaderConstantLayout([0x94]) 0x00B5E0C9 ; ~capture: RestoreCaptureBlock 0x00B5E0D4 ; ret
```
No cold blocks, no jump tables.

## Open points for the author
- Exact scalar spelling of the B-D taps (float vs double literals as listed; `twist + twist` vs `twist * 2.0f` -- the listing shows
  `fadd st,st`, which VC7.1 emits for `* 2.0f`); whether k=0 terms are literal `twist * 0.0f`; operand order of l/dx expressions.
- Use fwsize with FW_EXTRA=/EHsc against FableWin 6243 b / 1514 i (the 4 passes written out in source; no helper).
