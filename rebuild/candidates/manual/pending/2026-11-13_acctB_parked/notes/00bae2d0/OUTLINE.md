# OUTLINE 0x00BAE2D0 CEnginePrimitiveManager2DSprites::RenderSubPrimitive(CEngineSubPrimitiveBase& subPrimitive, ERenderPassType pass)

## Frame facts
- `sub esp,34h ; mov eax,[13CB508h] ; push ebx ; push ebp ; push esi ; ... push edi (0xBAE30D)`; no ebp frame, no SEH, no /GS.
- `ret 8` twice (0xBAF9BB, 0xBAF9D1). `pass` unused.
- Frame map (offsets from esp after all four pushes; args at +48h sub, +4Ch pass):
  - +10h  dword scratch: unsigned->float temp for every fild (colour_scale, Alpha, colour bytes) AND the CRGBColour 0xFFFFFFFF built
          bytewise (+12h,+11h,+10h,+13h order) in the SetTextureFactor else-path.
  - +14h  `this` copy (stored 0xBAE30E); in the clock branch later reused as fild scratch (0xBAEDED) and as the shader-manager spill
          (0xBAF0F4). Sprite branch reloads this from it at 0xBAF94C (Indices +2Ch).
  - +18h  CScopedShadersEnable.OldVertexEnable (bool), +19h OldPixelEnable.
  - +1Ch, +20h  `const CTexture* textures[2]` (Texture / AlphaMask).
  - +24h..+33h  C4DVector temp for every SetPixelShaderConstant (X +24h, Y +28h, Z +2Ch, W +30h).
  - +3Ch, +40h  float spills of the CRGBFloatColour Z/W (fstp then copied to +2Ch/+30h).
- Objects with dtors: CRenderStateScopedCaptureBlock capture (first), CScopedShadersEnable shaders(true,true) (second). Destroyed in
  reverse: shaders (vertex then pixel compare/call), then RestoreCaptureBlock. No EH states (all inline, no unwinding emitted).
- Register roles: ebp = this until 0xBAE55F (`add ebp,0Ch` = &VertexShader2DSprite); ebx = &alphaMask->(+0Ch view) i.e.
  &viewportManager->AlphaMaskInfo from 0xBAE35A; from 0xBAE7A9 ebp = &subPrimitive. edi = 1 for the bool states.

## Body (retail order)
```
// 0xBAE2D0
CRenderStateScopedCaptureBlock capture;         // StateStack[n++].State=&BookmarkDummyState(+3A24h); CurrentBookmarkMask<<=1
CScopedShadersEnable shaders(true, true);       // 0xBAE306 VS (cmp [13BC800h],1 ; call EnableVertexShaders 0x987FE0)
                                                // 0xBAE327 PS (cmp [13BC801h],1 ; call EnablePixelShaders 0x988110)
const C2DAlphaMaskInfo* alpha_mask = GetCurrent2DAlphaMaskInfo();   // 0xBAE33E [1436E84h]->+94h (+0Ch) -- BEFORE the states
// 0xBAE349.. GPRenderManager (0x1436E18) inline SetRenderState x5:
GPRenderManager->SetZBufferEnable(false);       // 0xBAE34E state +2868h, value 0
GPRenderManager->SetZBufferWriteEnable(false);  // 0xBAE3BC +2854h, 0
GPRenderManager->SetAlphaBlendEnable(true);     // 0xBAE427 +28B8h, edi=1
GPRenderManager->SetAlphaTestEnable(true);      // 0xBAE48E +28CCh, 1
GPRenderManager->SetAlphaRef(1);                // 0xBAE4F5 +28E0h, 1
ApplyVertexShader(VertexShader2DSprite);        // 0xBAE557 GPShaderRenderManager->ApplyVertexShader(this+0Ch, false) call 0x988020
if (!alpha_mask->FilterMask)                    // 0xBAE568 [ebx+29h]; jne 0xBAE6BE
    DisableTextureFiltering(1);                 // 0xBAE573 Mag[1]=POINT(+3178h), 0xBAE5ED Min[1]=POINT(+30D8h),
                                                // 0xBAE654 MipMap[1]=DISABLE(+2E58h, table 1396E6Ch)
GPRenderManager->SetTextureUVModeU(1, TEXTURE_UV_CLAMP);   // 0xBAE6C3 +2EF8h (table 1396DD0h)
GPRenderManager->SetTextureUVModeV(1, TEXTURE_UV_CLAMP);   // 0xBAE736 +2F98h
GPShaderRenderManager->SetUserVertexShaderConstant(0, alpha_mask->AlphaMaskPositionFactors);  // 0xBAE798 push &mask+8, push 0; call 0x989AB0
if (subPrimitive.GetParentBase()->GetType() == ENGINE_PRIMITIVE_2D_CLOCK_SPRITE) {   // 0xBAE7A9 [[sub+0Ch]+8]==23h; jne 0xBAF1BD
    CEngineSubPrimitive2DClockSprite& clock = static_cast<...&>(subPrimitive);
    // 0xBAE7BA WrapTexture +135h: true -> 0xBAE7D7 U[0]=WRAP(1396DC8h) then V uses esi=WRAP; false -> 0xBAE83D U[0]=CLAMP; V at 0xBAE8A2
    SetTextureUVModeU(0, wrap ? WRAP : CLAMP); SetTextureUVModeV(0, same);   // V tail shared at 0xBAE8A2 (+2F84h)
    if (!clock.Filter) DisableTextureFiltering(0);   // 0xBAE909 [+134h]; jne 0xBAEA60; Mag[0] 0xBAE917, Min[0] 0xBAE98A, MipMap[0] 0xBAE9F1
    unsigned long colour_scale;
    SetupAlphaBlendingStatesFromBlendMode(clock.BlendMode, clock.PeekAlpha(), colour_scale);   // 0xBAEA60
        // ecx=BlendMode-3: ==0 -> 0xBAEC0B (ADDITIVE: Src ONE 1396F6Ch, Dest ONE 0xBAEC73) -> eax=Alpha (0xBAECD5)
        // dec ==0 -> 0xBAEB56 (ADDSMOOTH: Src ONE, Dest INV_SRC_COLOR 1396F74h) -> jmp 0xBAECB7 (shared AddStateToUpdateList tail) -> 0xBAECD5
        // default 0xBAEA82: Src SRC_ALPHA 1396F78h, Dest INV_SRC_ALPHA 1396F7Ch (0xBAEAE4) -> eax=0xFF, jmp 0xBAECD7
        // Alpha = movzx [sub+8] into ebx at 0xBAEA69 (read once, before the switch)
    Setup2DDiffuseShading(clock.Texture /*array -> always true: lea esi,[ebp+120h]; test esi,esi*/, colour_scale, alpha, BlendMode);
        // 0xBAECD7 s=scale/255 (fst X,Y,fstp Z), a=Alpha/255 (W) -> PSConstTable[2] (+3CCh +20h), UpdateFlags|=2 (edi=2)
        // 0xBAED61 renderer=[1436E78h]: textured ? (mode==2 ? +1ACh : +1DCh) : (mode==2 ? +19Ch : +1CCh); 0xBAED8A ApplyPixelShader 0x988140
    ApplyPixelShader(clock.BlendMode == ENGINE_PRIMITIVE_BLEND_ALPHA ? PixelShader2DClockSprite : PixelShader2DClockSpriteAdditive);
        // 0xBAED96 this from [esp+14h] +1Ch / +24h; call 0x988140
    const CTexture* textures[2]; textures[1] = clock.AlphaMask; textures[0] = clock.Texture[0];   // 0xBAEDB6 (stores +20h then +1Ch)
    long first = clock.IndexOffset / 3;                 // 0xBAEDC2 movzx; imul 55555556h -> ebp (signed /3 of int)
    SetPixelShaderConstant(5, C4DVector(CRGBFloatColour(clock.Colour)));   // 0xBAEDD4 bytes +12Ah(R),+129h(G),+128h(B),+12Bh(A) *1/255 -> +3CCh+50h, |=2 (0xBAEE7E `or [eax+3D8h],edi`)
    SetTextureFactor(CRGBFloatColour(1,1,1,1));         // 0xBAEE84 if PixelShadersEnabled(+391h): const 2=(1,1,1,1) |=2, jmp 0xBAEF5D
                                                        // else 0xBAEEE0 TextureFactor(+29D0h)=0xFFFFFFFF (bytes into [esp+10h])
    GPShaderRenderManager->PrepareForRender(0);         // 0xBAEF5D/0xBAEF63: &4 SetVSConstants 0x98B930, &1 0x988A20, &2 0x989BF0,
                                                        // &4 0x989760, &20000h 0x9897C0, &400h 0x98A760, !+1E8h 0x988290, &80h(jns) 0x988A50
    GPRenderManager->DrawAsIndexedTris(textures, 2, clock.Vertices, 0x20, 7, clock.Indices, first);   // 0xBAF008 call 0xA0AEA0
    textures[0] = clock.Texture[1];                     // 0xBAF03C (+124h) -> [esp+1Ch]
    SetPixelShaderConstant(5, C4DVector(CRGBFloatColour(clock.Colour2)));  // 0xBAF02E bytes +12Eh,+12Dh,+12Ch,+12Fh; `or [..+3D8h],2`
    GPShaderRenderManager->PrepareForRender(0);         // 0xBAF0DE (spill mgr in [esp+14h])
    GPRenderManager->DrawAsIndexedTris(textures, 2, clock.Vertices, 0x20, 7, &clock.Indices[clock.IndexOffset], 6 - first);
        // 0xBAF191 args pushed, then jmp 0xBAF963 (call tail-merged with the sprite branch)
} else {
    CEngineSubPrimitive2DSprite& sprite = static_cast<...&>(subPrimitive);   // 0xBAF1BD
    SetTextureUVModeU/V(0, sprite.WrapTexture ? WRAP : CLAMP);   // [+A1h]; WRAP path 0xBAF1DE (V 0xBAF245, jmp 0xBAF33F tail), CLAMP 0xBAF294 (V 0xBAF2FB)
    if (!sprite.Filter) DisableTextureFiltering(0);     // 0xBAF362 [+A0h]; Mag 0xBAF370, Min 0xBAF3E3, MipMap 0xBAF44A
    SetupAlphaBlendingStatesFromBlendMode(sprite.BlendMode, sprite.PeekAlpha(), colour_scale);  // 0xBAF4B9; ADDITIVE 0xBAF61F,
        // ADDSMOOTH 0xBAF5B6 (jmp 0xBAF688 shared Dest), default 0xBAF4E1 -> eax=0FFh jmp 0xBAF6EE; eax=Alpha at 0xBAF6EC
    Setup2DDiffuseShading(sprite.Texture != 0, colour_scale, alpha, sprite.BlendMode);   // 0xBAF6EE (`test ecx,ecx ; setne cl`), ApplyPixelShader 0xBAF7B1
    SetTextureFactor(CRGBFloatColour(1,1,1,1));         // 0xBAF7B6 (else 0xBAF812)
    const CTexture* textures[2] = { sprite.Texture, sprite.AlphaMask };   // 0xBAF895 (loads +9Ch then +98h; stores +1Ch then +20h)
    GPShaderRenderManager->PrepareForRender(0);         // 0xBAF8AF
    GPRenderManager->DrawAsIndexedTris(textures, 2, sprite.Vertices, 0x20, 4, Indices /*this+2Ch*/, 2);   // 0xBAF94C push ebx(=2)...
}
// 0xBAF963 mov ecx,[1436E18h] ; call 0xA0AEA0 (shared call)
// 0xBAF96E ~CScopedShadersEnable: if (old VS != [13BC800h]) old ? Enable(0x987FE0) : Disable(0x9880E0)   (pops precede: edi,esi,ebp,ebx)
//           if (old PS != [13BC801h]) old ? Enable 0x988110 -> RestoreCaptureBlock 0xA05840 ; ret 8 (0xBAF9BB)
//                                        : Disable 0x988190 (0xBAF9BE) -> RestoreCaptureBlock ; ret 8 (0xBAF9D1)
```
Notes for the author
- Each Set* is the inline CRenderStateManager::SetRenderState (DesiredState compare, BookmarkMask & CurrentBookmarkMask, StateStack
  push, DirtyListFlag / StateUpdateList). The listing's raw idiom is stripped in $W/ann.txt; register / or-operand order of the
  bookmark test varies per block (see agent2059 NOTES: spelling through the same helpers fixed it).
- The clock branch's "always true" textured test is a retail quirk (address of the Texture array tested); pass `clock.Texture`
  (array decays) to a `bool` parameter of the inlined Setup2DDiffuseShading -- do not "fix" it to Texture[0].
- Two different scheduling forms of the Source/Dest blend sets between branches (clock: `cmp [edx+4],esi`; sprite: DesiredState
  preloaded into ebp) -- expected from one helper inlined twice.
- No cold blocks, no jump tables (blend switch compiled as `sub 3 / dec`).
