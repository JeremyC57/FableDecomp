# OUTLINE 0x00B5FAB0 CEngineScreenEffectGlowRenderer::CaptureBackBuffer()  (void, __thiscall, private, `ret`)

## Frame facts
- Prologue `sub esp,0C0h ; mov eax,[013CB508h] ; push ebx ; push ebp ; push esi ; push edi` (load of StateManager scheduled
  between sub and pushes); epilogue `pop edi ; pop esi ; pop ebp ; pop ebx ; add esp,0C0h ; ret` at 0x00B60F5B.
- No EBP frame, no SEH/EH states (no /EHsc), no /GS cookie. ebx = 0 throughout, esi = this (until `add esi,13Ch` at 0x00B60EF0
  in the final pass -- `this` dead after that), edi/ebp scratch.

## Frame map (esp-relative, no pushes pending; locals 0x10..0xCF)
| slot | local | type | touched by |
|---|---|---|---|
| 0x10 / 0x11 | shaders.OldVertexEnable / OldPixelEnable | CScopedShadersEnable (pass 1 only) | ctor 0xB5FB93, dtor 0xB602B7 |
| 0x10 (float) | blurOffsetY (init 1/128, *=1.5 twice per loop iter) | float | 0xB605FD, 0xB60976, 0xB60CA2 (slot reused after pass 1) |
| 0x14 | backBufferTexture (CTexture& from CopyBackBufferToTexture), later PrepareForRender spill of GPShaderRenderManager; loop counter `i` | | 0xB6001B; 0xB60605 (i=0), 0xB60C88 |
| 0x18..0x24 | C4DVector threshold (pass 1) / motionBlur splat (pass1) / 0.33f constant (pass 2, loop-A) | C4DVector | |
| 0x28 (float) | blurOffsetX (init 1/128) | float | 0xB605F5, 0xB60962, 0xB60C8C |
| 0x2C..0x33 | CPixelShader glowShader (vptr 0x129A054, Data) in pass 1; float temps later | CPixelShader | copy 0xB5FBEF, dtor 0xB60291 |
| 0x34 | PrepareForRender spill (loop-A/B) | | |
| 0x38..0x44 | textures[4] (pass 2) / 0.33f C4DVector (loop-B) / Intensity C4DVector (final) | | |
| 0x48..0x6B | CRenderTarget target (passes 1, 2, loop-A): +0x48 vptr 0x129C800, RenderToSurface 0x4C, ZBufferSurface 0x5C | CRenderTarget | CBaseClass() 0x99A2D0, ~ -> 0x9F2E20 x2 + 0x99A2E0 |
| 0x6C..0x7B | temp CSurface from GetSurfaceLevel (passes 1, 2) / textures[4] (loop-B, final) | | |
| 0x7C..0x9F | CRenderTarget target (loop-B, final): CSurfaces at 0x80, 0x90 | CRenderTarget | |
| 0xA0..0xAF | temp CSurface (loop-B, final) | CSurface | |
| 0xB0..0xBC | textures[4] (loop-A) | const CTexture*[4] | |
| 0xC0..0xCF | temp CSurface (loop-A) | CSurface | |

Statics: `static float BlurOffsetScale = 1.5f;` 0x0139A6B0, `static long NoBlurPasses = 2;` 0x0139A6B4,
`static unsigned short QuadIndices[6] = {0,1,2,0,2,3};` 0x0139A6B8 (names descriptive; order = address order).

## Body in retail order (G = NEngineInternal::GPRenderManager 0x01436E18, SRM = GPShaderRenderManager 0x01436E14,
## DM = display manager 0x01436E1C)

```cpp
void CEngineScreenEffectGlowRenderer::CaptureBackBuffer()
{
    // 0xB5FAB6: inline StartCaptureBlock on StateManager 0x013CB508 (StateStack[n++].State=&BookmarkDummyState(+0x3A24); mask<<=1)
    CRenderStateScopedCaptureBlock capture;
    // 0xB5FAEA: edi = &GPResourceManager->GPUScratchBufferTextureManager (+0x3190), computed once
    CGPUScratchBufferTextureManager& scratch = GPResourceManager->GetGPUScratchBufferTextureManager();
    GlowTexture256   = scratch.StartTextureUse(GlowTexture256Handle);     // 0xB5FAFC call 0xF36EE0
    GlowTexture64    = scratch.StartTextureUse(GlowTexture64Handle);      // 0xB5FB0A
    GlowTexture64Alt = scratch.StartTextureUse(GlowTexture64AltHandle);   // 0xB5FB18
    GlowTexture16    = scratch.StartTextureUse(GlowTexture16Handle);      // 0xB5FB26
    {   // ---- pass 1: back buffer -> GlowTexture256 (threshold) ----
        CRenderTarget target;                                   // 0xB5FB32 CBaseClass() + inline vptr/CSurface ctors (frame 0x48)
        target.AttachTarget(GlowTexture256->GetSurfaceLevel(0)); // 0xB5FB67 GetSurfaceLevel (temp 0x6C), 0xB5FB71 AttachTarget, 0xB5FB7E ~temp
        DM->SetRenderTarget(target);                            // 0xB5FB8E
        CScopedShadersEnable shaders(true, true);               // 0xB5FB93..0xB5FBBC (Enable VS 0x987FE0 / PS 0x988110 if not already)
        SRM->ApplyVertexShader(VertexShaderCopy, false);        // 0xB5FBCC (+0x34)
        CPixelShader glowShader(ThresholdR < 1.0f ? PixelShaderGlowNeg : PixelShaderGlow); // 0xB5FBD1: fcomp 1.0, `lea eax,[esi+54h] ; jnp` ; copy ctor (AddReference if Data)
        SRM->ApplyPixelShader(glowShader);                      // 0xB5FC0B
        C4DVector threshold;  threshold.W = 0.3f;               // 0xB5FC1C store 0x3E99999A to 0x24 BEFORE the branch
        if (ThresholdR < 1.0f) {                                // 0xB5FC10 fcomp ; test ah,5 ; jp 0xB5FC4B
            threshold.X = ThresholdR; threshold.Y = ThresholdG; threshold.Z = ThresholdB;          // int moves 0xB5FC2B
        } else {                                                // 0xB5FC4B
            threshold.X = ThresholdR - 1.0f; threshold.Y = ThresholdG - 1.0f; threshold.Z = ThresholdB - 1.0f;
        }
        SRM->SetPixelShaderConstant(5, threshold);              // 0xB5FC7B (+0x3CC table +0x50; UpdateFlags |= 2 via `mov ecx,2 ; or`)
        C4DVector motionBlur(MotionBlur, MotionBlur, MotionBlur, MotionBlur);   // 0xB5FCB7 (slot 0x18 reused; ecx=2 reused for the or)
        SRM->SetPixelShaderConstant(8, motionBlur);             // 0xB5FCD3 (+0x80)
        G->SetZBufferEnable(false);                             // 0xB5FD01 (+0x2868)
        G->SetPolyCullMode(POLY_CULL_NONE);                     // 0xB5FD6D (+0x2890 <- [0x1396FB8])
        if (MotionBlur > 0.0f && MotionBlur < 1.0f) {           // 0xB5FDD9 test ah,41h jne 0xB5FFB0 ; 0xB5FDF0 test ah,5 jp 0xB5FFB0
            G->SetAlphaBlendEnable(true);                       // 0xB5FE07 (+0x28B8, ebp=1)
            G->SetAlphaTestEnable(false);                       // 0xB5FE72 (+0x28CC)
            G->SetSourceAlphaBlendMode(ALPHA_BLEND_SRC_ALPHA);  // 0xB5FED8 (+0x291C <- [0x1396F78])
            G->SetDestAlphaBlendMode(ALPHA_BLEND_INV_SRC_ALPHA);// 0xB5FF44 (+0x2930 <- [0x1396F7C]); tail `jmp 0xB60010`
        } else
            G->SetAlphaBlendEnable(false);                      // 0xB5FFB0
        CTexture& backBuffer = DM->CopyBackBufferToTexture();   // 0xB60010 (kept in 0x14)
        BackBufferToTextureVerts[0].U1 = 0.0f; BackBufferToTextureVerts[0].V1 = 1.0f;   // 0xB60024 stores in this order:
        BackBufferToTextureVerts[1].U1 = 1.0f; BackBufferToTextureVerts[1].V1 = 1.0f;   //  +0x8C=0, +0x90=1, +0xA4=1, +0xA8=1,
        BackBufferToTextureVerts[2].U1 = 1.0f; BackBufferToTextureVerts[2].V1 = 0.0f;   //  +0xBC=1, +0xC0=0, +0xD4=0, +0xD8=0
        BackBufferToTextureVerts[3].U1 = 0.0f; BackBufferToTextureVerts[3].V1 = 0.0f;   //  (1.0f via eax=0x3F800000, 0 via ebx)
        G->SetTextureUVModeU(0, TEXTURE_UV_CLAMP);              // 0xB60054 (+0x2EE4 <- [0x1396DD0])
        G->SetTextureUVModeV(0, TEXTURE_UV_CLAMP);              // 0xB600CA (+0x2F88)
        G->AttachTextureToStage(0, &backBuffer);                // 0xB60132 (tex? tex->PD3DTexture:0 ; +0x3D00 ; SetTexture vtbl+0x104 ; +0x3CB8 >= 1)
        SRM->PrepareForRender(0);                               // 0xB6017A..0xB6022C
        G->DrawAsIndexedTris(BackBufferToTextureVerts, sizeof(CTVertexColTex1), 4, QuadIndices, 2);   // 0xB60246 call 0xA0AF40
        G->AttachTextureToStage(0, 0);                          // 0xB6024B
        DM->SetRenderTargetAsBackScreen();                      // 0xB6028C
    }   // 0xB60291 ~glowShader (rc dec, vfunc0(1) if 0; vptr 0x129A030) ; 0xB602B7 ~shaders (restore VS 0x987FE0/0x9880E0, PS 0x988110/0x988190) ;
        // 0xB60301 ~target: ZBufferSurface(0x5C) {vptr 0x122F84C; Uninitialise}, RenderToSurface(0x4C) same, ~CBaseClass 0x99A2E0(0x48)
    G->SetAlphaBlendEnable(false);                              // 0xB60329
    {   // ---- pass 2: GlowTexture256 -> GlowTexture64 (blur, 1/128 taps) ----
        CRenderTarget target;                                   // 0xB6038C (0x48)
        target.AttachTarget(GlowTexture64->GetSurfaceLevel(0)); // 0xB603C3 / 0xB603CD / ~temp 0xB603DA
        DM->SetRenderTarget(target);                            // 0xB603EA
        SRM->ApplyVertexShader(VertexShaderBlur, false);        // 0xB603FA (+0x3C)
        SRM->ApplyPixelShader(PixelShaderBlur);                 // 0xB60409 (+0x5C)  (no CPixelShader copy here)
        SRM->SetPixelShaderConstant(5, C4DVector(0.33f, 0.33f, 0.33f, 0.33f));   // 0xB6040E (via slot 0x18)
        SRM->SetUserVertexShaderConstant(0, 0.0f, 0.0f, 1.0f, 1.0f);              // 0xB60479
        SRM->SetUserVertexShaderConstant(1, 0.0f, 1.0f/128, 1.0f, 1.0f);         // 0xB60496
        SRM->SetUserVertexShaderConstant(2, 1.0f/128, 0.0f, 1.0f, 1.0f);         // 0xB604B3
        SRM->SetUserVertexShaderConstant(3, 1.0f/128, 1.0f/128, 1.0f, 1.0f);     // 0xB604D4
        const CTexture* textures[4] = { GlowTexture256, GlowTexture256, GlowTexture256, GlowTexture256 }; // 0xB604D9 (0x38..0x44)
        SRM->PrepareForRender(0);                               // 0xB604F8..0xB60593
        G->DrawAsIndexedTris(textures, 4, Glow256ToGlow64Verts, sizeof(CTVertexColTex1), 4, QuadIndices, 2); // 0xB605B7 call 0xA0AEA0
        DM->SetRenderTargetAsBackScreen();                      // 0xB605C2
    }   // 0xB605C7 ~target (0x5C, 0x4C, ~CBaseClass 0x48)
    float offsetX = 1.0f/128;   // 0x28   (0xB605F5)
    float offsetY = 1.0f/128;   // 0x10   (0xB605FD)
    // 0xB605EF: for (long i = 0; i < NoBlurPasses; ++i)  -- `cmp [0x139A6B4],ebx ; jle 0xB60CAC` ; i in 0x14
    // 0xB6060F..0xB60647: two C4DVector(0.33f) constants stored ONCE (0x18..0x24 and 0x38..0x44) -- loop-invariant
    for (long i = 0; i < NoBlurPasses; ++i) {                   // body 0xB60650 (after a 1-byte nop)
        G->SetAlphaBlendEnable(false);                          // 0xB60650
        {   // ---- loop A: GlowTexture64 -> GlowTexture64Alt ----
            CRenderTarget target;                               // 0xB606B3 (0x48)
            target.AttachTarget(GlowTexture64Alt->GetSurfaceLevel(0));   // temp CSurface at 0xC0 (0xB606ED / 0xB606F7 / 0xB6070A)
            DM->SetRenderTarget(target);                        // 0xB6071A
            SRM->ApplyVertexShader(VertexShaderBlur, false);    // 0xB6072A
            SRM->ApplyPixelShader(PixelShaderBlur);             // 0xB60739
            // 0xB6073E: tmp(0x2C) = -1/128 - offsetY   (fld [0x12A28E4] ; fsub [0x10])
            SRM->SetPixelShaderConstant(5, blurColourA /*0x18, 0.33f*/);           // 0xB60744
            SRM->SetUserVertexShaderConstant(0, -1.0f/128, -1.0f/128, 1.0f, 1.0f); // 0xB6079F
            SRM->SetUserVertexShaderConstant(1, -1.0f/128 - offsetX, -1.0f/128 - offsetY, 1.0f, 1.0f); // 0xB607C9
            SRM->SetUserVertexShaderConstant(2, offsetX - 1.0f/128, -1.0f/128 - offsetY, 1.0f, 1.0f);  // 0xB607EF (fsub [0x12A1F34])
            SRM->SetUserVertexShaderConstant(3, -1.0f/128, offsetY - 1.0f/128, 1.0f, 1.0f);           // 0xB60819
            const CTexture* textures[4] = { GlowTexture64 x4 }; // 0xB6081E (0xB0..0xBC)
            SRM->PrepareForRender(0);                           // 0xB60849 (this copy spills to 0x34/0x2C, `ja` form differs)
            G->DrawAsIndexedTris(textures, 4, Glow64ToGlow16Verts, sizeof(CTVertexColTex1), 4, QuadIndices, 2); // 0xB60912
            DM->SetRenderTargetAsBackScreen();                  // 0xB6091D
        }   // 0xB60922 ~target
        offsetX *= BlurOffsetScale;                             // 0xB6094A (fmul [0x139A6B0])
        offsetY *= BlurOffsetScale;                             // 0xB6096C  (interleaved with the next setter)
        G->SetAlphaBlendEnable(false);                          // 0xB60954..0xB609CA
        {   // ---- loop B: GlowTexture64Alt -> GlowTexture64 ----
            CRenderTarget target;                               // 0xB609CA (0x7C)
            target.AttachTarget(GlowTexture64->GetSurfaceLevel(0));      // temp 0xA0
            DM->SetRenderTarget(target);                        // 0xB60A49
            SRM->ApplyVertexShader(VertexShaderBlur, false);    // 0xB60A59
            SRM->ApplyPixelShader(PixelShaderBlur);             // 0xB60A68
            // 0xB60A6D: tmp(0x2C) = offsetY - 1/128
            SRM->SetPixelShaderConstant(5, blurColourB /*0x38, 0.33f*/);           // 0xB60A71
            SRM->SetUserVertexShaderConstant(0, -1.0f/128, -1.0f/128, 1.0f, 1.0f); // 0xB60ACE
            SRM->SetUserVertexShaderConstant(1, -1.0f/128, -1.0f/128 - offsetY, 1.0f, 1.0f); // 0xB60AF8
            SRM->SetUserVertexShaderConstant(2, -1.0f/128 - offsetX, offsetY - 1.0f/128, 1.0f, 1.0f); // 0xB60B22
            SRM->SetUserVertexShaderConstant(3, offsetX - 1.0f/128, offsetY - 1.0f/128, 1.0f, 1.0f);  // 0xB60B48
            const CTexture* textures[4] = { GlowTexture64Alt x4 };  // 0xB60B4D (0x6C..0x78)
            SRM->PrepareForRender(0);                           // 0xB60B6C
            G->DrawAsIndexedTris(textures, 4, Glow64ToGlow16Verts, sizeof(CTVertexColTex1), 4, QuadIndices, 2); // 0xB60C35
            DM->SetRenderTargetAsBackScreen();                  // 0xB60C40
        }   // 0xB60C45 ~target (0x90, 0x80, ~CBaseClass 0x7C)
        offsetX *= BlurOffsetScale;                             // 0xB60C79
        offsetY *= BlurOffsetScale;                             // 0xB60C90 (inc i interleaved; `cmp eax,[0x139A6B4] ; jl 0xB60650`)
    }
    {   // ---- final: GlowTexture64 -> GlowTexture16 (intensity, 1/32 taps) ----   0xB60CAC
        CRenderTarget target;                                   // 0x7C
        target.AttachTarget(GlowTexture16->GetSurfaceLevel(0)); // temp 0xA0
        DM->SetRenderTarget(target);                            // 0xB60D2B
        SRM->ApplyVertexShader(VertexShaderBlur, false);        // 0xB60D3B
        SRM->ApplyPixelShader(PixelShaderBlur);                 // 0xB60D4A
        SRM->SetPixelShaderConstant(5, C4DVector(IntensityR, IntensityG, IntensityB, 0.3f));   // 0xB60D4F (slot 0x38; loads B,R,G order)
        SRM->SetUserVertexShaderConstant(0, 0.0f, 0.0f, 1.0f, 1.0f);              // 0xB60DC0
        SRM->SetUserVertexShaderConstant(1, 0.0f, 1.0f/32, 1.0f, 1.0f);          // 0xB60DDD
        SRM->SetUserVertexShaderConstant(2, 1.0f/32, 0.0f, 1.0f, 1.0f);          // 0xB60DFA
        SRM->SetUserVertexShaderConstant(3, 1.0f/32, 1.0f/32, 1.0f, 1.0f);       // 0xB60E1B
        const CTexture* textures[4] = { GlowTexture64 x4 };     // 0xB60E20 (0x6C)
        SRM->PrepareForRender(0);                               // 0xB60E3F
        G->DrawAsIndexedTris(textures, 4, Glow64ToGlow16Verts, sizeof(CTVertexColTex1), 4, QuadIndices, 2); // 0xB60F01 (`add esi,13Ch`)
    }   // 0xB60F06 ~target -- NOTE: before SetRenderTargetAsBackScreen here
    DM->SetRenderTargetAsBackScreen();                          // 0xB60F40
    g_pEngineCamera->RefreshD3DTransforms();                    // 0xB60F4B (0x01436EA0)
}   // 0xB60F56 ~capture: StateManager->RestoreCaptureBlock() 0xA05840 ; epilogue
```

## Notes
- No jump tables, no cold blocks after the epilogue. Branch targets listed inline (all forward except the loop back-edge 0xB60CA6->0xB60650).
- Each inline SetRenderState expands to the 0x14-byte CStateInfo pattern (value compare, bookmark mask test/or, StateStack push,
  queued flag + StateUpdateList push); identical to RenderColorFilteredQuad's (shared CRenderStateManager.h).
- PrepareForRender(0) instance 1 keeps the manager in edi, instances 2/5 in ebp with a 0x14 spill, instances 3/4 spill the
  CArray pointer to 0x34/0x2C (register-pressure differences, not source differences).
- Vertex constant tap values: pass 2 uses 0 / +1/128; loop uses -1/128 combined with the growing offsets; final uses 0 / +1/32.
