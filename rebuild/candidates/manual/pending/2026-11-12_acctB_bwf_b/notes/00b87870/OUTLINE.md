# OUTLINE 0x00B87870 void CEngineScreenEffectDisplacementRenderer::ApplyDisplacement()  (private, __thiscall, plain ret)

## Frame
- `sub esp,24h; push ebx; push ebp; push esi; push edi` (no ebp frame, no SEH, no /GS). Locals relative to esp after the 4 pushes:
  - [esp+10h] `this` spill (CEngineScreenEffectDisplacementRenderer*)
  - [esp+14h] bool OldVertexEnable, [esp+15h] bool OldPixelEnable  = CScopedShadersEnable shaders(true,true)
  - [esp+18h],[1Ch],[20h],[24h] the four by-value float params of the inline SetTextureBumpEnvMatrix (homes reused for each stage);
    [24h] also receives the fstp of `v` at 0x00B88518
  - [esp+28h],[2Ch],[30h] `const CTexture* textures[3]`
  - CRenderStateScopedCaptureBlock capture: empty, no slot.
- ebx = 1 and ebp = 0 held all function long.
- Epilogue: inline ~CScopedShadersEnable then ~CRenderStateScopedCaptureBlock (RestoreCaptureBlock), two tail copies.

## Body (retail order)
```
static unsigned short QuadIndices[6] = { 0, 1, 3, 1, 2, 3 };       // 0x0139AAFC
// 0x0139AB08: static bool (init true) of this TU -> name descriptively, e.g. UseStencil  (layout: after QuadIndices, before Show's)

0x00B87870  CRenderStateScopedCaptureBlock capture;              // inline StartCaptureBlock on [0x013CB508]: StateStack[n++].State = &BookmarkDummyState(+0x3A24); CurrentBookmarkMask <<= 1
0x00B878A3  CScopedShadersEnable shaders(true, true);              // al=[0x013BC800]; save [esp+14h]; if (al!=1) call EnableVertexShaders 0x00987FE0 (ecx=0x013BC470)
0x00B878C3                                                        // al=[0x013BC801]; save [esp+15h]; if (al!=1) call EnablePixelShaders 0x00988110
0x00B878DA  GPShaderRenderManager->ApplyVertexShader(DisplacementVertexShader /*+0x134*/, false);   // call 0x00988020 (push ebp=0 first)
0x00B878EF  GPShaderRenderManager->ApplyPixelShader(DisplacementPixelShader /*+0x13C*/);           // call 0x00988140
            // each Set* below = inline SetRenderState: cmp [state+4],value; je next; bookmark; store DesiredState; AddStateToUpdateList
0x00B87901  GPRenderManager->SetAlphaBlendEnable(true);           // +0x28B8 <- 1
0x00B8796C  GPRenderManager->SetAlphaTestEnable(true);            // +0x28CC <- 1
0x00B879D2  GPRenderManager->SetAlphaTestFunc(ALPHA_TEST_GREATER_EQUAL);  // +0x28A4 <- [0x01396DF0]
0x00B87A3E  GPRenderManager->SetAlphaRef(1);                      // +0x28E0 <- 1
0x00B87AA4  GPRenderManager->SetSourceAlphaBlendMode(ALPHA_BLEND_SRC_ALPHA);     // +0x291C <- [0x01396F78]
0x00B87B10  GPRenderManager->SetDestAlphaBlendMode(ALPHA_BLEND_INV_SRC_ALPHA);   // +0x2930 <- [0x01396F7C]
0x00B87B7C  if (UseStencil /*byte [0x0139AB08]*/) {               // je 0x00B87E6C
0x00B87B8A      SetStencilEnable(true);                           // +0x2B88 <- 1
0x00B87BF0      SetStencilFunc(STENCIL_FUNC_EQUAL);               // +0x2BD8 <- [0x01396E20]
0x00B87C5C      SetStencilOpFail(STENCIL_OP_MODE_KEEP);           // +0x2B9C <- [0x01396F38]   (needs new inline opt-in)
0x00B87CC3      SetStencilOpZFail(STENCIL_OP_MODE_KEEP);          // +0x2BB0 <- [0x01396F38]   (needs new inline opt-in; the skip path of the
                                                                  //   previous block jumps to 0x00B87CCE past the table reload: esi kept)
0x00B87D34      SetStencilOpPass(STENCIL_OP_MODE_ZERO);           // +0x2BC4 <- [0x01396F3C]   (OUTLINE_INLINE)
0x00B87DA0      SetStencilRef(1);                                 // +0x2BEC <- 1
0x00B87E06      SetStencilMask(1);                                // +0x2C00 <- 1
            }
0x00B87E6C  GPRenderManager->SetZBufferEnable(false);             // +0x2868 <- 0 (ebp)
0x00B87ED2  GPRenderManager->SetPolyCullMode(POLY_CULL_NONE);     // +0x2890 <- [0x01396FB8]
0x00B87F3E  GPRenderManager->SetDitherEnable(false);              // +0x2A0C <- 0
0x00B87FA4  GPRenderManager->SetTextureBumpEnvMatrix(0, DisplaceAmount, 0.0f, 0.0f, DisplaceAmount);
              //  homes: [18h]=amt,[24h]=amt,[20h]=0,[1Ch]=0 ; Mat00[0] +0x37A4 <-[18h] ; 0x00B88032 Mat10[0] +0x38E4 <-[1Ch] ;
              //  0x00B8809A Mat01[0] +0x3844 <-[20h] ; 0x00B88102 Mat11[0] +0x3984 <-[24h]
0x00B8816A  GPRenderManager->SetTextureBumpEnvMatrix(1, DisplaceAmount, 0.0f, 0.0f, DisplaceAmount);
              //  Mat00[1] +0x37B8 <-[24h] ; 0x00B881F8 Mat10[1] +0x38F8 <-[20h] ; 0x00B88260 Mat01[1] +0x3858 <-[1Ch] ; 0x00B882C8 Mat11[1] +0x3998 <-[18h]
0x00B88330  GPRenderManager->SetTextureBumpEnvMatrix(2, DisplaceAmount, 0.0f, 0.0f, DisplaceAmount);
              //  Mat00[2] +0x37CC ; 0x00B883B9 Mat10[2] +0x390C ; 0x00B8841C Mat01[2] +0x386C ; 0x00B8847F Mat11[2] +0x39AC
              //  (stage-2 blocks keep the core in ecx and the mask in esi: no global reloads)
0x00B884E2  GPShaderRenderManager->SetUserVertexShaderConstant(0, DisplaceAmount, 0.0f, 0.0f, DisplaceAmount);   // call 0x00989B00
0x00B884FE  GPShaderRenderManager->SetUserVertexShaderConstant(1, v, v, 0.0f, 0.0f);                            // call 0x00989B00 at 0x00B88523
              //  v = DisplaceAmount * -0.5f - 0.001953125f   (fld [this+E8h]; fmul [0x01238798]; fsub [0x012A3AF4]; one fstp, pushed twice)
0x00B88528  SetTextureFilterModeMag(0, TEXTURE_FILTER_LINEAR);    // +0x3164 <- [0x01396E5C]
0x00B885A1  SetTextureFilterModeMin(0, TEXTURE_FILTER_LINEAR);    // +0x30C4
0x00B8860F  SetTextureFilterModeMag(1, ..LINEAR);                 // +0x3178
0x00B8867D  SetTextureFilterModeMin(1, ..LINEAR);                 // +0x30D8
0x00B886EB  SetTextureFilterModeMag(2, ..LINEAR);                 // +0x318C
0x00B88759  SetTextureFilterModeMin(2, ..LINEAR);                 // +0x30EC
0x00B887C1  SetTextureUVModeU(0, TEXTURE_UV_WRAP);                // +0x2EE4 <- [0x01396DC8]
0x00B88835  SetTextureUVModeV(0, TEXTURE_UV_WRAP);                // +0x2F84
0x00B888A3  SetTextureUVModeU(1, TEXTURE_UV_WRAP);                // +0x2EF8
0x00B88911  SetTextureUVModeV(1, TEXTURE_UV_WRAP);                // +0x2F98
0x00B88979  SetTextureUVModeU(2, TEXTURE_UV_CLAMP);               // +0x2F0C <- [0x01396DD0]
0x00B889ED  SetTextureUVModeV(2, TEXTURE_UV_CLAMP);               // +0x2FAC
0x00B88A55  GPShaderRenderManager->PrepareForRender(0);           // inline (CShaderRenderManager.h:164): SetVSConstants 0x00B88A9E,
              //  UpdatePixelShader 0x00B88AAE, UpdatePixelShaderConstants 0x00B88ABF, UpdateAmbient 0x00B88AD0, UpdateFogColour 0x00B88AE4,
              //  UpdateLightGlobals 0x00B88AF7, ResetWorldTransform 0x00B88B0D (if !WorldMatrixIsIdentity), UpdateCombinedProjectionTransform 0x00B88B1E
0x00B88B28  const CTexture* textures[3] = { DisplacementTexture /*+0x10 -> [28h]*/, BackBufferCopyTexture /*+0x0C -> [2Ch]*/, BackBufferCopyTexture /*[30h]*/ };
            GPRenderManager->DrawAsIndexedTris(textures, 3, TextureToBackBufferVerts /*+0x80*/, sizeof(CTVertexColTex1) /*0x18*/, 4, QuadIndices, 2);  // call 0x00A0AEA0
0x00B88B5E  // ~shaders: if (Old!=cur) Enable/DisableVertexShaders (0x00987FE0 / 0x009880E0); pops interleaved here
0x00B88B83  //           if (OldPixel!=cur) EnablePixelShaders 0x00B88B98 -> RestoreCaptureBlock 0x00B88BA3 -> ret 0x00B88BAB
0x00B88BAC  //           cold copy: DisablePixelShaders -> 0x00B88BB1 RestoreCaptureBlock 0x00B88BB7 -> ret 0x00B88BBF
```
No jump tables. No loops (every stage is written out; VC7.1 does not unroll).
Pointers: GPRenderManager = NEngineInternal::GPRenderManager (0x01436E18), GPShaderRenderManager = NEngineInternal::GPShaderRenderManager (0x01436E14).
