# OUTLINE 0x00B89C30 CEngineScreenEffectOutlineGlow::ApplyOutline()  (retail order; addresses are block starts)

## Frame facts
- `sub esp,94h ; push ebp ; push esi ; mov esi,ecx` ; early-out ; `push ebx ; push edi` (0x00B89C56/57). No ebp frame, no SEH, no /GS.
- Exits: normal 0x00B8B4EF `pop edi ; pop ebx` (interleaved in the shader-enable dtor), then 0x00B8B52D/0x00B8B546 `pop esi ; pop ebp ;
  add esp,94h ; ret`. Early return 0x00B89C45 `je 0x00B8B546` (skips capture-block restore).
- esp0 = entry-0xA4 (after all four pushes). Frame map (offsets from esp0; xdis shows [esp+N] with N = offset + pushes in flight):
| esp0+ | local | type | ctor / dtor |
|---|---|---|---|
| 0x10 | this spill | CEngineScreenEffectOutlineGlow* | stored 0x00B89C41 |
| 0x14 | i (loop) ; later `const CTexture* tex` for the final draw (slot reused, 0x00B8B4D9) | long / const CTexture* | i=0 at 0x00B8A8AF |
| 0x18,0x19 | shaders (CScopedShadersEnable: OldVertexEnable, OldPixelEnable) | 2 bools | ctor 0x00B89C77..0x00B89CA0, dtor 0x00B8B4E5..0x00B8B536 |
| 0x1C..0x40 | pass-0 `target` and pass-A `target` | CRenderTarget (vptr 0x0129C800 +0x1C, RenderToSurface +0x20, ZBufferSurface +0x30) | CBaseClass() 0x00B8A70D / 0x00B8A945, dtor = ~CSurface(+0x30), ~CSurface(+0x20), ~CBaseClass(+0x1C) |
| 0x40..0x64 | GetRenderTarget() temporary (start) ; pass-B `target` | CRenderTarget | temp: ctor call 0x00A10DC0 at 0x00B89CB6, dtor 0x00B89CDB..0x00B89CFE ; pass B: CBaseClass() 0x00B8AB3E, dtor 0x00B8AC95..0x00B8ACB3 |
| 0x64..0x74 | `const CTexture* textures[4]` pass 0 and pass A | | 4 stores of the same pointer |
| 0x74..0x84 | GetSurfaceLevel temp (pass 0) ; textures[4] (pass B) | CSurface / const CTexture*[4] | pass-0 temp dtor 0x00B8A74A |
| 0x84..0x94 | GetSurfaceLevel temp (pass A) | CSurface | dtor 0x00B8A989 |
| 0x94..0xA4 | GetSurfaceLevel temp (pass B) | CSurface | dtor 0x00B8AB7E |
Capture block (CRenderStateScopedCaptureBlock) has no stack footprint.
Scopes implied: pass 0 shares slots with pass A -> disjoint scopes; pass A and pass B do NOT share slots although targetA is destroyed
before pass B starts (see note after the loop). The start temp (0x40) is a full-expression temporary.

## Body
```
void CEngineScreenEffectOutlineGlow::ApplyOutline()
{
    static float BlurOffsetU = 1.0f / 128.0f;          // 0x0139AB54 (descriptive name)  -- function-local, non-const
    static float BlurOffsetV = 1.0f / 128.0f;          // 0x0139AB58 (descriptive name)
    static unsigned short QuadIndices[6] = { 0, 1, 3, 1, 2, 3 };   // 0x0139AB5C
                                                        // (order in .data = declaration order; verify the statics land at these addresses' order)
0x00B89C3A  if (!OutlineTexture) return;                // cmp [esi+0Ch],0 ; je 0x00B8B546
0x00B89C4B  CRenderStateScopedCaptureBlock capture;     // inline StartCaptureBlock on [0x013CB508]: StateStack[StateStackSize].p = &BookmarkDummyState(+0x3A24);
                                                        //   CurrentBookmarkMask(+0x2814) <<= 1 (shl edx,1); StateStackSize++   (engine/CRenderStateCaptureBlock.h)
0x00B89C77  CScopedShadersEnable shaders(true, true);   // al=[0x013BC800] ; cmp al,1 ; save ; jne -> EnableVertexShaders 0x00987FE0 ;
                                                        // 0x00B89C8E same for PixelShadersEnabled 0x013BC801 -> EnablePixelShaders 0x00988110
0x00B89CA5  OldRenderTarget = NEngineInternal::GPDisplayManager->GetRenderTarget();
            //   inline GetRenderTarget: CRenderTarget tmp(DM->RenderTargetSurface(+0xC), DM->RenderTargetDepthBuffer(+0x1C)) -> call 0x00A10DC0 (pushes +0x1C then +0xC)
            //   inline CRenderTarget::operator=: OldRenderTarget.RenderToSurface(+0x150) = tmp.RenderToSurface  -> call 0x009F2E70
            //                                    OldRenderTarget.ZBufferSurface(+0x160)  = tmp.ZBufferSurface   -> call 0x009F2E70
            //   ~tmp: [esp0+0x54]=0x0122F84C, Uninitialise 0x009F2E20 ; [esp0+0x44]=vtbl, Uninitialise ; ~CBaseClass 0x0099A2E0 on esp0+0x40
            //   (ebx = 0x0122F84C loaded at 0x00B89CDB and kept for every later ~CSurface)
0x00B89D03  OldViewport = NEngineInternal::GPDisplayManager->GetViewport();   // 4 dword copy from DM+0x1F4 to this+0x170 (esi += 0x170)
            // ---- render states (all GPRenderManager inline setters; eax reloaded from 0x01436E18 after each) ----
0x00B89D2B  SetAlphaBlendEnable(false);       // +0x28B8, value ebp=0
0x00B89D97  SetAlphaTestEnable(false);        // +0x28CC
0x00B89DFE  SetZBufferEnable(false);          // +0x2868
0x00B89E65  SetPolyCullMode(POLY_CULL_NONE);  // +0x2890 <- [0x01396FB8]
0x00B89ED2  SetDitherEnable(false);           // +0x2A0C
0x00B89F39  SetStencilEnable(false);          // +0x2B88  (STENCIL_INLINE)
0x00B89FA0  SetTextureFilterModeMag(0, TEXTURE_FILTER_POINT);   // +0x3164 <- [0x01396E58] (table re-read after every store)
0x00B8A015  SetTextureFilterModeMin(0, TEXTURE_FILTER_POINT);   // +0x30C4
0x00B8A084  SetTextureFilterModeMag(1, ..POINT);  // +0x3178
0x00B8A0F3  SetTextureFilterModeMin(1, ..POINT);  // +0x30D8
0x00B8A162  SetTextureFilterModeMag(2, ..POINT);  // +0x318C
0x00B8A1D1  SetTextureFilterModeMin(2, ..POINT);  // +0x30EC
0x00B8A240  SetTextureFilterModeMag(3, ..POINT);  // +0x31A0
0x00B8A2AF  SetTextureFilterModeMin(3, ..POINT);  // +0x3100
0x00B8A318  SetTextureUVModeU(0, TEXTURE_UV_CLAMP); // +0x2EE4 <- [0x01396DD0]
0x00B8A38D  SetTextureUVModeV(0, ..CLAMP);  // +0x2F84
0x00B8A3FC  SetTextureUVModeU(1, ..CLAMP);  // +0x2EF8
0x00B8A46B  SetTextureUVModeV(1, ..CLAMP);  // +0x2F98
0x00B8A4DA  SetTextureUVModeU(2, ..CLAMP);  // +0x2F0C
0x00B8A549  SetTextureUVModeV(2, ..CLAMP);  // +0x2FAC
0x00B8A5B8  SetTextureUVModeU(3, ..CLAMP);  // +0x2F20
0x00B8A627  SetTextureUVModeV(3, ..CLAMP);  // +0x2FC0 (last block uses edx/esi swapped)
            // (the 8 filter + 8 UV calls are straight-line; a per-stage loop would interleave Mag/Min/U/V -- write them out, or 2 loops? VC7.1 does
            //  not unroll: write 16 explicit calls)
            // ---- pass 0: outline -> Blur1 ----
0x00B8A68B  GPShaderRenderManager->ApplyVertexShader(OutlineToBlurVertexShader /*+0x180*/, false);   // 0x00988020, edi=0
0x00B8A6A4  GPShaderRenderManager->ApplyPixelShader(OutlineToBlurPixelShader /*+0x188*/);           // 0x00988140
0x00B8A6B6  GPShaderRenderManager->SetUserVertexShaderConstant(0, 0.0f, 0.0f, 0.0f, 0.0f);          // 0x00989B00 (zeros pushed as edi)
0x00B8A6C6  ...SetUserVertexShaderConstant(1, 1.0f/256, 0.0f, 0.0f, 0.0f);    // push 3B800000h = x
0x00B8A6DB  ...SetUserVertexShaderConstant(2, 0.0f, 1.0f/256, 0.0f, 0.0f);
0x00B8A6F0  ...SetUserVertexShaderConstant(3, 1.0f/256, 1.0f/256, 0.0f, 0.0f);
0x00B8A709  {
              CRenderTarget target;                       // esp0+0x1C: CBaseClass() 0x0099A2D0, then inline vptr/surface stores (interleaved with the next pushes)
              target.AttachTarget(Blur1Texture /*+0x10*/->GetSurfaceLevel(0));   // 0x009F9E00 -> temp esp0+0x74 ; 0x00A10E60 ; ~temp 0x00B8A74A
0x00B8A757    NEngineInternal::GPDisplayManager->SetRenderTarget(target);          // 0x009BF160
0x00B8A767    NEngineInternal::GPShaderRenderManager->PrepareForRender(0);       // inline: tests 4(SetVSConstants 0x0098B930), 1, 2, 4, 0x20000, 0x400, WorldMatrixIsIdentity, 0x80
0x00B8A81C    const CTexture* textures[4] = { OutlineTexture, OutlineTexture, OutlineTexture, OutlineTexture };   // esp0+0x64 (+0xC loaded once)
              NEngineInternal::GPRenderManager->DrawAsIndexedTris(textures, 4, OutlineToBlurVerts /*+0x2C*/, sizeof(CTVertexColTex1) /*0x18*/, 4, QuadIndices, 2);  // 0x00A0AEA0
0x00B8A85E  }                                                  // ~target: ~CSurface(esp0+0x30), ~CSurface(esp0+0x20), ~CBaseClass(esp0+0x1C)
            // ---- blur ping-pong ----
0x00B8A881  GPShaderRenderManager->ApplyVertexShader(BlurToBlurVertexShader /*+0x190*/, false);  // push 0 immediate here
0x00B8A895  GPShaderRenderManager->ApplyPixelShader(BlurToBlurPixelShader /*+0x198*/);
0x00B8A8A7  for (long i = 0; i < BlurStepCount /*+0x1D8*/; ++i)     // i in esp0+0x14; test/jle 0x00B8ACD4; ebp = this+0x8C hoisted (0x00B8A8BD)
            {
0x00B8A8C5    ...SetUserVertexShaderConstant(0, 0,0,0,0);
0x00B8A8D7    ...SetUserVertexShaderConstant(1, -BlurOffsetU, -BlurOffsetV, 0, 0);   // y = -[AB58] computed first, then x = -[AB54]
0x00B8A900    ...SetUserVertexShaderConstant(2,  BlurOffsetU, -BlurOffsetV, 0, 0);
0x00B8A927    ...SetUserVertexShaderConstant(3,  0.0f,         BlurOffsetV, 0, 0);    // `push ecx` reserves the y slot
0x00B8A941    CRenderTarget targetA;                                                  // esp0+0x1C
              targetA.AttachTarget(Blur2Texture /*+0x14*/->GetSurfaceLevel(0));       // temp esp0+0x84
0x00B8A99C    GPDisplayManager->SetRenderTarget(targetA);
0x00B8A9AC    GPShaderRenderManager->PrepareForRender(0);                              // (ebx clobbered by SetVSConstants count; reloaded 0x0122F84C at 0x00B8A9F9)
0x00B8AA5C    const CTexture* texturesA[4] = { Blur1Texture x4 };                     // esp0+0x64
              GPRenderManager->DrawAsIndexedTris(texturesA, 4, BlurToBlurVerts /*ebp*/, 0x18, 4, QuadIndices, 2);   // here 4s are `push 4` immediates
0x00B8AA9D    ~targetA (esp0+0x30, +0x20, +0x1C)  -- destroyed here, before pass B starts
0x00B8AAC0    ...SetUserVertexShaderConstant(0, 0,0,0,0);
0x00B8AAD2    ...SetUserVertexShaderConstant(1, -BlurOffsetU,  BlurOffsetV, 0, 0);
0x00B8AAF9    ...SetUserVertexShaderConstant(2,  BlurOffsetU,  BlurOffsetV, 0, 0);
0x00B8AB1E    ...SetUserVertexShaderConstant(3,  0.0f,        -BlurOffsetV, 0, 0);
0x00B8AB3A    CRenderTarget targetB;                                                  // esp0+0x40
              targetB.AttachTarget(Blur1Texture /*+0x10*/->GetSurfaceLevel(0));       // temp esp0+0x94
0x00B8AB91    GPDisplayManager->SetRenderTarget(targetB);
0x00B8ABA1    GPShaderRenderManager->PrepareForRender(0);
0x00B8AC51    const CTexture* texturesB[4] = { Blur2Texture x4 };                     // esp0+0x74
              GPRenderManager->DrawAsIndexedTris(texturesB, 4, BlurToBlurVerts, 0x18, 4, QuadIndices, 2);
0x00B8AC95    ~targetB (esp0+0x54, +0x44, +0x40)
0x00B8ACB8  }                                      // inc i ; cmp i, this->BlurStepCount (re-read) ; jl 0x00B8A8C5 ; fallthrough `mov ebp,4` 0x00B8ACCF
            // FACTS: targetA is destroyed BEFORE pass B's constants (0x00B8AA9D), so A and B are separate scopes ({ A } { B } inside the
            // loop body, or A's objects die at a block end); yet A (0x1C/0x64/0x84) and B (0x40/0x74/0x94) do not share slots, while pass 0
            // (0x1C/0x64/0x74) does share with A. First probe: `{ ...A... } { ...B... }` inside the loop and pass 0 in its own block; if VC7.1
            // overlays A/B, try declaring A's and B's arrays/targets so that their lifetimes overlap the other block, or B's texture array outside.
            // ---- composite to back buffer ----
0x00B8ACD4  NEngineInternal::GPDisplayManager->SetRenderTargetAsBackScreen();   // 0x009BF220 (ebx = this reloaded first)
0x00B8ACE3  g_pEngineCamera->RefreshD3DTransforms();                           // 0x00B2FC50 on [0x01436EA0]
0x00B8ACEE  OldRenderTarget.Uninitialise();                                    // 0x00A10E30, ecx = this+0x14C
0x00B8ACF9  GPShaderRenderManager->ApplyVertexShader(BlurToBackBufferVertexShader /*+0x1A0*/, false);
0x00B8AD0D  GPShaderRenderManager->ApplyPixelShader(BlurToBackBufferPixelShader /*+0x1A8*/);
0x00B8AD1F  SetTextureFilterModeMag(0, TEXTURE_FILTER_LINEAR);   // +0x3164 <- [0x01396E5C]
0x00B8AD97  SetTextureFilterModeMin(0, TEXTURE_FILTER_LINEAR);   // +0x30C4
0x00B8ADFE  SetAlphaBlendEnable(true);                           // +0x28B8, edi=1
0x00B8AE72  SetAlphaTestEnable(true);                            // +0x28CC
0x00B8AED9  SetAlphaTestFunc(ALPHA_TEST_GREATER_EQUAL);          // +0x28A4 <- [0x01396DF0]
0x00B8AF46  SetAlphaRef(1);                                      // +0x28E0, `mov [edx+4],1` immediate
0x00B8AFB2  SetSourceAlphaBlendMode(ALPHA_BLEND_SRC_ALPHA);      // +0x291C <- [0x01396F78]
0x00B8B01F  SetDestAlphaBlendMode(ALPHA_BLEND_INV_SRC_ALPHA);    // +0x2930 <- [0x01396F7C]
0x00B8B08C  SetZBufferEnable(false);                             // +0x2868, immediate 0
0x00B8B0F7  SetPolyCullMode(POLY_CULL_NONE);                     // +0x2890 <- [0x01396FB8]
0x00B8B164  SetDitherEnable(true);                               // +0x2A0C
0x00B8B1D8  SetStencilEnable(true);                              // +0x2B88
0x00B8B23F  SetStencilFunc(STENCIL_FUNC_EQUAL);                  // +0x2BD8 <- [0x01396E20]
0x00B8B2AC  SetStencilOpPass(STENCIL_OP_MODE_ZERO);              // +0x2BC4 <- [0x01396F3C]  (OUTLINE_INLINE)
0x00B8B319  SetStencilMask(4);                                   // +0x2C00, value ebp=4
0x00B8B380  SetStencilRef(4);                                    // +0x2BEC, value ebp=4
0x00B8B3E7  GPShaderRenderManager->PrepareForRender(0);          // eax=GPRenderManager re-loaded after each Update* call
0x00B8B4BF  const CTexture* tex = Blur1Texture /*+0x10*/;        // esp0+0x14 (reuses i's slot)
            NEngineInternal::GPRenderManager->DrawAsIndexedTris(&tex, 1, BlurToBackBufferVerts /*+0xEC*/, 0x18, 4 /*ebp*/, QuadIndices, 2);
0x00B8B4E5  // ~shaders: if (old != current) old ? Enable : Disable (vertex: 0x00987FE0 / 0x009880E0 ; pixel: 0x00988110 / 0x00988190)
0x00B8B522  // ~capture: RestoreCaptureBlock 0x00A05840 on [0x013CB508]   (tail-duplicated at 0x00B8B53B)
}
```

## Cold / tail blocks
- 0x00B8B536 `call DisablePixelShaders` then falls into the duplicated restore+epilogue 0x00B8B53B..0x00B8B54E (also the early-return target
  0x00B8B546). No jump tables. No other cold code.
