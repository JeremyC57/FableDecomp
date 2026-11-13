# OUTLINE 0x00BA2350 CEnginePrimitiveManagerStaticMeshesBase::SetupRenderModeShadersAndConstants (retail order)

## Frame
- `sub esp,0B0h; push ebx,ebp,esi,edi` (ESP frame, no SEH, no cookie). `ret 1Ch`. Offsets below are post-push esp (as dis_all prints them when no args are pending).
- Args: +C4 pass, +C8 material, +CC sub, +D0 stages, +D4 mipmap_level, +D8 shader_flags, +DC degenerate.
- Locals: +10 bool tinted (and, in other cases, the bool& out of Get*Texture) | +14 this | +18 CRGBColour tint / int temp / then `lights` (parent->RenderInfo.Lights) from 0xBA25F4 |
  +1C..+2B float temps, then C4DVector theme colour copy (cases 7/10/13/17) or C4DVector(1,1,1,1) (pass 1) | +2C int/float temp, later long outline arg (pass 10) |
  +30 CGraphicDataBank* bank (also CRGBColour 0xFFFFFFFF in pass-1) | +34 unsigned long flags (0x109 default) | +38 bool flag7 | +3C/+40/+44 uchar ftol results |
  +48 long render_mode (later MeshEffect in pass 10) | +4C float alpha | +50..+5C C4DVector for PS constant 2 / C3DVector (mode 15) | +60 shadow params (mode 10) |
  +6C C3DVector (mode 17) | +78 params (mode 12) | +84 C3DVector (mode 18) | +90 C3DVector (mode 16) | +9C params (mode 9) | +A8 params (mode 13) | +B4 params (mode 11).

## Body
```
bool SetupRenderModeShadersAndConstants(ERenderPassType pass, const C3DMeshMaterial* material, CEngineSubPrimitiveStaticMesh& sub,
                                        C3DTextureStages& stages, long mipmap_level, unsigned long* shader_flags, bool* degenerate)
{
  // 0xBA2360
  CEngineInternalPrimitiveStaticMesh* parent = sub.GetParent();                 // esi
  bool flag7 = (parent->EnginePrimitiveFlags >> 7) & 1;                          // [38]
  const CGraphicDataBank* bank = parent->MeshInfo->(+4)->(+0x3D4);               // [30]  (C3DMeshInfo -> ... -> graphic bank; resolve via C3DMeshInfo.h)
  CRGBColour tint = parent->TintColour;                                          // bytes +5C..+5F
  bool tinted = (tint & 0xFFFFFF) != 0xFFFFFF;                                    // [10] (setne)
  C3DPrimitive2* mesh_primitive = sub.MeshPrimitive;                             // edi (+0x18)
  unsigned long flags = 0x109;                                                   // [34]
  if (pass == RENDER_PASS_COMBINED) {                                            // cmp 2; jne 0xBA40AA
    long render_mode = sub.RenderMode;                                           // [48]
    unsigned char alpha = sub.Alpha;
    if (sub.RenderFlags & 8) {                                                   // 0xBA23E3
      render_mode = 5;                                                            // stipple alpha
      if (!material->Transparent) {                                              // +0x29
        long a = StippleAlphaSolidFactor * alpha;                                // +0x314, imul
        if (a >= 255) alpha = 255; else alpha = (uchar)a;                        // `cmp eax,0FFh; jl; or al,0FFh`
      }
    }
    // 0xBA2414: CEnginePrimitiveRenderUtil::SetTextureFactor(CRGBFloatColour(tint.R/255, G/255, B/255, alpha/255))  -- expanded:
    //   if (GPShaderRenderManager->PixelShadersEnabled) SetPixelShaderConstant(2, C4DVector(...))   (PSConstTable+0x20, UpdateFlags|=2; 0xBA2475..0xBA24BE)
    //   else GPRenderManager->SetTextureFactor(colour.GetRGBColour())   (4 x fmul 255.0 + __ftol2, 0xBA24C3..0xBA258D; SetRenderState +0x29D0)
    //   Same text as DisplayMirroredTextures 0x00BA1B00 (copy). Byte order of tint->float: [5C]->R? check: fild order cl(+5C),dl(+5D),+5E,+5F(alpha slot gets `alpha`).
    // 0xBA258D
    g_pEngineLightingManager->SetCurrentEnvironmentThemeChannel(parent->ThemeChannelDesc);   // +0x6C vs +0xE0..E2, |=0xF
    CEngineLocalLightArray* lights = parent->RenderInfo.Lights;                  // [18]  (+0xA0)
    stages.DiffuseTextureStage = 0 is written at the start of most cases (eax = &stages)
    switch (render_mode) {                                                       // 0xBA25F8 dec/jmp table, NO range check -> default: __assume(0)
    case 1:  // 0xBA31DE
      ApplyVertexShader(VertexShadersStatic[PointLightShaderIndex[0]], true); ApplyPixelShader(g_pPrimitiveRenderer->PixelShaderDiffuseOnly /*+0xDC*/); break;   // -> 0xBA405E
    case 2:  // 0xBA2600
      stages.DiffuseTextureStage = 0;
      ApplyVertexShader(VertexShadersStatic[PointLightShaderIndex[min(lights->Count,5)]], true); ApplyPixelShader(renderer +0xF4 PixelShaderDiffuse);
      lighting->SetEnableFadedShadowFactor(lighting->ShadowMode == 1); break;   // je 0xBA4064 when unchanged
    case 3:  // 0xBA2677
      stages.Diffuse = 0; VS VertexShadersStaticSpotlight[idx] (+0x38); PS +0xF4; lighting->SetupShaderSpotLight(parent->ObjectTransform /*+0x70*/); break;
    case 4:  // 0xBA26CF
      stages.Diffuse = 0; VS Static[idx]; PS renderer+0xFC; FadedShadow(ShadowMode==1); break;          // tail-merged into 0xBA2C56
    case 5:  // 0xBA270C  stipple alpha
      stages.Diffuse = 0; VS VertexShadersStaticStippleAlpha[idx] (+0x58); PS this->PixelShaderDiffuseFogStippleAlpha (+0x2C8);
      GPShaderRenderManager->SetUserVertexShaderConstant(0, x, y, z, w);  // 0x989B00; from g_pEngineCamera +0x14,+0x18,+0x1C,+0x20:
         //   a=cam[+0x20]; x=(a+cam14)*K1+D; y=(cam1C+cam18?)*K1+D ... K1=[0x125B24C] float, D=[0x12A2788] double, K2=[0x12A41B0]; push order w,z,y,x
         //   (read 0xBA274A..0xBA27B3 carefully: fadd st(2)/st(3), fsub st(1))
      rm->SetTextureFilterModeMin(1, TEXTURE_FILTER_POINT); SetTextureFilterModeMag(1, POINT);   // +0x30D8, +0x3178 (table 0x1396E58 [0])
      rm->SetTextureUVModeU(1, TEXTURE_UV_WRAP); SetTextureUVModeV(1, WRAP);                     // +0x2EF8, +0x2F98
      rm->AttachTextureToStage(1, &resource_mgr->(+0x2AAC CTexture));                             // 0x1436E98; inline null test of the address
      rm->SetTextureTransformMode(1, TEXTURE_TRANSFORM_PROJECTED);                                // +0x3678, table[5]
      lighting->SetEnableFadedShadowFactor(ShadowMode == 1); break;                              // 0xBA2A3C -> 0xBA2CC8
    case 6:  // 0xBA2A4E  env
      stages.Diffuse = 0; rm->AttachTextureToStage(1, &resource_mgr->(+0x2AD4));
      { bool b; rm->AttachTextureToStage(2, GetReflectionTexture(material, mipmap_level, b, bank, flag7)); }
      flags = 0x129; VS VertexShadersStaticEnv[idx] (+0x98); PS renderer+0x104; FadedShadow; break;   // -> 0xBA2C57/0xBA2C62
    case 7:  // 0xBA2B4F  env + theme
      stages.Diffuse = 0; { bool b; AttachTextureToStage(1, GetReflectionTexture(material, mipmap_level, b, bank, flag7)); }
      SetPixelShaderConstant(0, lighting->ThemeChannels[lighting->CurrentThemeChannel.ChannelIndex0].Colour /*+0x30, C4DVector*/);  // UpdateFlags |= 2
      flags = 0x129; VS Env[idx]; PS +0x104; FadedShadow; break;
    case 8:  // 0xBA2C71
      stages.Diffuse = 0; stages.Bump = 1; VS Static[idx]; PS +0xFC; FadedShadow; break;
    case 9:  // 0xBA2CEC  shadow
      stages.Diffuse = 0; VS VertexShadersStaticShadow[idx] (ptr +0x2F0); PS tinted ? *renderer+0x228 : *renderer+0x224;
      { CColorBufferDepthPackParameters p = {4,5,0}; g_pEngineShadowRenderer->SetupShadowFuncRenderStates(1, parent->ObjectTransform, p); }  // [9C]
      flags = 0x10B; break;
    case 10: // 0xBA2D92  env shadow
      stages.Diffuse=0; refl -> stage 1; theme colour -> PS const 0; flags=0x12B; VS EnvShadow (+0x2F4); PS *renderer+0x22C;
      SetupShadowFuncRenderStates(2, ObjectTransform, {4,5,0} [60]); break;      // shares 0xBA2FB6 with case 11
    case 11: // 0xBA2ECD  DP3 bump shadow
      stages.Diffuse=0; stages.Bump=1; AttachTextureToStage(2, &resource_mgr->(+0x2AD4)); VS DP3BumpShadow (+0x2FC); PS tinted ? *+0x238 : *+0x234;
      flags = 0x10B; SetupShadowFuncRenderStates(3, ObjectTransform, {4,5,0} [B4]); break;
    case 12: // 0xBA2FCB  spot shadow
      stages.Diffuse=0; VS SpotShadow (+0x300); PS tinted ? *+0x228 : *+0x224; SetupShadowFuncRenderStates(1, ObjectTransform, {4,5,0} [78]);
      lighting->SetupShaderSpotLight(ObjectTransform); flags = 0x10B; break;
    case 13: // 0xBA307B  env spot shadow
      stages.Diffuse=0; refl -> stage 1; theme colour -> PS const 0; flags=0x12B; VS EnvSpotShadow (+0x304); PS *+0x22C;
      SetupShadowFuncRenderStates(2, ObjectTransform, {4,5,0} [A8]); SetupShaderSpotLight(ObjectTransform); break;
    // case 14: none (table entry 0)
    case 15: // 0xBA3CF9  2D static (mask on stage 0)
      info = g_pEnginePrimitiveTypeInfo->(+0x94) + 0xC  (struct: +0 bank, +4 id, +8..+10 C3DVector, +0x14 float w, +0x1D bool "no filtering" (= base+0x29));
      { bool b; AttachTextureToStage(0, Get2DAlphaMaskTexture(info->id, 0, b, info->bank, flag7)); }
      if (!info->flag) { SetTextureFilterModeMag(0, POINT); SetTextureFilterModeMin(0, POINT); SetTextureMipMapMode(0, TEXTURE_MIPMAP_DISABLE); }   // +0x3164,+0x30C4,+0x2E44
      SetTextureUVModeU(0, CLAMP); SetTextureUVModeV(0, CLAMP);                  // +0x2EE4, +0x2F84 (table[2])
      ApplyVertexShader(VertexShaders2DStatic /*+0x1D8*/, true); ApplyPixelShader(renderer+0x19C);
      SetUserVertexShaderConstant(0, C3DVector(info+8..+0x10), info->w);  // [50]; -> 0xBA3FE8 shared
      rm->SetFogEnable(false); break;                                            // +0x2944, 0xBA3FF5..0xBA405E
    case 16: // 0xBA3214  2D, mask on stage 1
      stages.Diffuse=0; mask -> stage 1; filter Mag/Min/MipMap(1) unless flag; UV U/V(1) CLAMP; VS 2DStatic (+0x1D8); PS renderer+0x1AC;
      SetUserVertexShaderConstant(0, C3DVector [90], w); SetFogEnable(false); break;    // joins 0xBA4013
    case 17: // 0xBA3555  2D env
      stages.Diffuse=0; mask -> stage 1; { bool b; AttachTextureToStage(2, GetReflectionTexture(material, mipmap_level, b, bank, flag7)); }
      filter(1) unless flag; UV(1) CLAMP; theme colour -> PS const 0; flags = 0x129; VS 2DStaticEnv (+0x1E0); PS +0x1B4;
      SetUserVertexShaderConstant(0, C3DVector [6C], w); SetFogEnable(false); break;    // own copy 0xBA3923..0xBA3995
    case 18: // 0xBA399A  2D DP3 bump
      stages.Diffuse=0; stages.Bump=1; mask = Get2DAlphaMaskTexture(...); AttachTextureToStage(2, mask); AttachTextureToStage(3, mask);
      filter(2) unless flag (+0x318C Mag, +0x30EC Min, +0x2E6C MipMap); UV(2) CLAMP (+0x2F0C, +0x2FAC); VS 2DStaticDP3Bump (+0x1F0); PS +0x1C4;
      SetUserVertexShaderConstant(0, C3DVector [84], w); SetFogEnable(false); break;    // jmp 0xBA3FE8
    default: __assume(0);
    }
    // 0xBA405E (ecx reload) / 0xBA4064
    g_pEngineLightingManager->SetupShaderPointLights(*lights);
    g_pEngineLightingManager->SetSelfIlluminationAmbient(material->SelfIllumination /*+0x24*/);   // +0xE8, |=2
    g_pEngineLightingManager->UpdateGlobalLightingStates();
  }                                                                               // jmp 0xBA46A2 (ebp = &stages)
  else if (pass == RENDER_PASS_Z_WRITE) {                                         // 0xBA40AA
    CEnginePrimitiveRenderUtil::SetTextureFactor(CRGBFloatColour(1,1,1,1));       // PS const 2 = (1,1,1,1) | TextureFactor 0xFFFFFFFF (folded)
    if (material->Transparent) { stages.Diffuse = 0; if (mesh_primitive bit1) stages.Bump = 1; }     // 0xBA419F
    else { bool bump = bit1; rm->AttachTextureToStage(0, 0); stages.UseBlankTextures = true; stages.Diffuse = 0; if (bump) stages.Bump = 1; }
    ApplyVertexShader(VertexShadersStatic[PointLightShaderIndex[0]], true); ApplyPixelShader(renderer+0xFC);
  }
  else if (pass == RENDER_PASS_OUTLINE_SMOOTH_STENCIL) {                          // 8, 0xBA4250
    SetupOutlineSmoothStencilStates(*parent->MeshEffect, parent->Scale);           // 0xBEFD90, ecx=this
    bool bump = bit1; AttachTextureToStage(0, 0); stages.UseBlankTextures = true; stages.Diffuse = 0; if (bump) stages.Bump = 1;
    ApplyVertexShader(bit1 ? VertexShaderDP3BumpOutline /*+0x2D8*/ : VertexShaderOutline /*+0x2D0*/, true); ApplyPixelShader(renderer+0xDC);
    *degenerate = true;
  }
  else if (pass == RENDER_PASS_OUTLINE_SMOOTH_FILL) {                             // 10, 0xBA431D
    effect = parent->MeshEffect;                                                  // [48]
    ApplyVertexShader(bit1 ? +0x2E8 : +0x2E0, true); *degenerate = true;
    GPScreenEffectOutlineGlow->SetupDepthBufferTransform(parent->ObjectTransform);   // 0xB89070
    long n = bump ? 2 : 1;                                                       // setne/inc, [2C]
    if (material->DiffuseMapBankIndex && material->Transparent) {                // +0x10, +0x29
      stages.Diffuse = 0; if (bit1) stages.Bump = 1;
      ApplyPixelShader(bit1 ? renderer+0x214 : renderer+0x204);
      SetupOutlineSmoothFillStates(*effect, parent->Scale, true, n);             // 0xBF2240
    } else {
      AttachTextureToStage(0, 0); stages.UseBlankTextures = true; stages.Diffuse = 0; if (bump) stages.Bump = 1;
      ApplyPixelShader(bit1 ? renderer+0x20C : renderer+0x1FC);
      SetupOutlineSmoothFillStates(*effect, parent->Scale, false, n);
    }
  }
  else if (pass == RENDER_PASS_OUTLINE_SMOOTH_ALPHA_STENCIL_MASK) {               // 9, 0xBA44AC
    ApplyVertexShader(bit1 ? +0x2D8 : +0x2D0, true); *degenerate = true;
    if (material->DiffuseMapBankIndex && material->Transparent) { stages.Diffuse = 0; if (bit1) stages.Bump = 1; ApplyPixelShader(renderer+0xE4); }
    else { AttachTextureToStage(0,0); UseBlankTextures = true; Diffuse = 0; if (bump) Bump = 1; ApplyPixelShader(renderer+0xDC); }
    SetupOutlineSmoothAlphaStencilMaskStates(*parent->MeshEffect);               // 0xBF01E0 (tail-duplicated in both arms)
  }
  else {                                                                          // 0xBA45CF
    if (pass == RENDER_PASS_OUTLINE_STENCIL) SetupOutlineStencilStates(*parent->MeshEffect, parent->Scale);   // 0xBF0740
    else SetupOutlineFillStates(*parent->MeshEffect, parent->Scale);              // 0xBF0B20 placed cold at 0xBA4663, jmp back 0xBA45E7
    AttachTextureToStage(0,0); UseBlankTextures = true; Diffuse = 0; if (bump) Bump = 1;
    ApplyVertexShader(bit1 ? +0x2D8 : +0x2D0, true); ApplyPixelShader(renderer+0xDC); *degenerate = true;
  }
  // 0xBA46A2 common tail (ebp = &stages)
  if (stages.DiffuseTextureStage >= 0) { rm->SetTextureUVModeU(stages.Diffuse, WRAP); rm->SetTextureUVModeV(stages.Diffuse, WRAP); }   // +0x2EE4/+0x2F84 + 0x14*s
  if (stages.BumpTextureStage >= 0)    { rm->SetTextureUVModeU(stages.Bump, WRAP);    rm->SetTextureUVModeV(stages.Bump, WRAP); }
  *shader_flags = flags;                                                          // 0xBA488A
  return true;
}
```
- "AttachTextureToStage(0,0)" = AttachD3DTextureToStage(0, 0) shape (`test ecx,ecx` on CurrentlySelectedTextures[0], then high-water 1).
- The 4 C4DVector copies of theme colour go through a local (16-byte copy to [1C..28]) before PSConstTable store -> pass a named C4DVector local / const ref the way the inline takes it.
- `bump` (bl) is read BEFORE AttachTextureToStage in the pass-1/8/9/else paths, but `bit1` is re-read from mesh_primitive later for shader choice: two separate reads (inline accessor called twice).
