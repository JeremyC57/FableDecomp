# OUTLINE 0x00BB3E80 CEngineClothRenderer::RenderClothPrimitive

## Frame
- `push ebp; mov ebp,esp; sub esp,48h`; push ebx, esi (interleaved into the capture ctor), push edi (0xBB3EB5). No EH state, no /GS.
- Two epilogues (`pop edi/esi/ebx; mov esp,ebp; pop ebp; ret 20h`) at 0xBB4502 and 0xBB5033: block-layout copies of the
  ~CScopedShadersEnable(pixel half) + ~CRenderStateScopedCaptureBlock tail.

## Frame map (ebp-relative)
| slot | holds | type / ctor-dtor |
|---|---|---|
| -0x01 | occlusion_test (init false at 0xBB3ECF, true at 0xBB3FAA) | bool |
| -0x08 | this | CEngineClothRenderer* (ecx spill) |
| -0x0C..-0x09 | CRGBColour temp of GetRGBColour (B -0xC, G -0xB, R -0xA, A -9), later `material` (0xBB4290) | CRGBColour / const C3DMeshMaterial* |
| -0x10 | occlusion handle (init 0 at 0xBB3ED3; -1 when the 0x2000 array is full) | unsigned long |
| -0x14 / -0x13 | CScopedShadersEnable {OldVertexEnable, OldPixelEnable} | ctor 0xBB3FAE, dtor 0xBB4FFD |
| -0x18 | primitive.Scale copy (float), then 0.5f constant of GFFloatToLongFloor | float |
| -0x1C | AddPixelCountTest by-value handle copy (&value for push_back) / int temps of the 1/255 filds / later render_info->Lights (0xBB427A) | unsigned long / const CEngineLocalLightArray* |
| -0x20 | mipmap ratio float | float |
| -0x24 | floor-compare float temp, then `texture` (const CTexture*, address passed to RenderPolygons) | |
| -0x28 | &program = cloth_primitive.ParticleProgram.Data (0xBB3E9C) | const CParticleProgram* |
| -0x34..-0x2C | colour G,B,A floats (SetTextureFactor) / camera-minus-centre X,Y / CColorBufferDepthPackParameters {4,5,0} (twice) | |
| -0x48..-0x3C | C4DVector for SetPixelShaderConstant(2) / sphere copy (-0x40 Center.Z, -0x3C Radius) / -0x48 world X | |
| +0x0C | (param slot reuse) world X float, then distance; +0x0F preloading bool (init 0 at 0xBB4439); later TwoSided byte (0xBB4E98) | |
| +0x10 | (param slot reuse) world Y float, int width, 1.0f const, then sub.RenderFlags copy (0xBB46C4); +0x0F also alpha_ref byte (0xBB46C7) | |
| +0x18 | (param slot reuse) render-manager pointer in FlushRender; +0x18..+0x1A CEnginePrimitiveThemeChannelDesc() temp in RestoreDefaultPrimitiveParameters | |
| +0x1B | SScalarTag temp passed to _M_insert_overflow | |

## Body (retail order)
```
// 0xBB3E80
const CParticleProgram& program = *cloth_primitive.ParticleProgram;          // [eax+4] -> [ebp-28h], hoisted into the capture ctor
CRenderStateScopedCaptureBlock capture_block;                                 // StartCaptureBlock inline 0xBB3E8F..0xBB3EBD
bool occlusion_test = false; unsigned long occlusion_handle = 0;              // 0xBB3ECF / 0xBB3ED3
EEngineOcclusionLayer layer = GetCurrentOcclusionLayer();                     // g_shared+0x78 ; cmp -1 -> 0xBB3FAE
if (layer != -1 && primitive.OcclusionInfo[layer] (+0x48+layer*4) != 0 && GetOcclusionEnable() (+0xBD)) {   // 0xBB3EE0..0xBB3EF7
    occlusion_handle = g_pOcclusionManager->AllocateOccluderHandle();         // 0xBB3EFD: EntriesSize<0x2000 ? {Entries[i]={0,0}; ++; i} : -1
    primitive.GetOcclusionInfo(layer).AddPixelCountTest(occlusion_handle);    // 0xBB3F3B: if (CurrentFrame != TestFrame) OcclusionTestHandles.clear()
                                                                              //   (cmp eax,eax dead memmove arm 0xBB3F61); push_back (0xBB3F6E; overflow
                                                                              //   call 0x00464ACE at 0xBB3F97 with (finish,&value,&tag,1,true)); TestFrame = CurrentFrame (0xBB3F9C)
    occlusion_test = true;                                                    // 0xBB3FAA
}
CScopedShadersEnable shaders(true, true);                                     // 0xBB3FAE (EnableVertexShaders 0xBB3FBF), 0xBB3FC4 (EnablePixelShaders 0xBB3FD5)
GPRenderManager->SetSpecularEnable(true);                                     // 0xBB3FDA (+0x29E4)
GPRenderManager->SetFogEnable(true);                                          // 0xBB404D (+0x2944)
CEnginePrimitiveRenderUtil::SetTextureFactor(CRGBFloatColour(CRGBColour(primitive.TintColour.R, .G, .B, sub_primitive.Alpha)));   // 0xBB40B5
    // bytes read +0x5E,+0x5D,+0x5C then sub+8; each *1/255 (0x01231724). 0xBB40FC: if GPShaderRenderManager->GetPixelShadersEnabled() (+0x391)
    //   -> SetPixelShaderConstant(2, C4DVector(R,G,B,A)) (copy to [+0x3CC]+0x20, |= 2 at +0x3D8), jmp 0xBB420A
    // else 0xBB415C: GetRGBColour (4x __ftol2 of *255 at 0xBB4162/72/83/9D, R first) -> GPRenderManager->SetTextureFactor(IntValue) (+0x29D0)
const CEngineRenderInfoMeshBase* render_info = primitive.PeekRenderInfoMeshBase();   // 0xBB4211 vtbl+0x74
GPLightingManager->SetCurrentEnvironmentThemeChannel(primitive.ThemeChannelDesc);    // 0xBB4214 (+0xE0..E2 / |=0xF at +0xE4)
GPLightingManager->UpdateGlobalLightingStates();                                     // 0xBB426D call 0x00B46C80
const CEngineLocalLightArray& lights = *render_info->Lights;                          // 0xBB427A [ebp-1Ch]
const C3DMeshMaterial* material = render_info->Mesh->GetMaterial(cloth_primitive.MaterialIndex);  // 0xBB4280 (*0x30 + [+0x8C]) -> [ebp-0Ch]
const CGraphicDataBank* bank = primitive.MeshInfo->PeekTextureBank();                 // 0xBB4293 (+0x50 -> +4 -> +0x3D4) -> esi
CVectorMap<...>* composite_surfaces = 0;                                              // xor edi,edi
CEngineInternalPrimitiveMorphInfo* morph = primitive.GetMorphInfo();                  // 0xBB42A3 vtbl+0x80
if (morph) composite_surfaces = &morph->CompositeSurfaces;                            // lea edi,[eax+10h]
unsigned long mipmap = CEnginePrimitiveManagerMeshBase::GetAppropriateMipmap(         // inline 0xBB42B0..0xBB4425 (agent2060 body; scale = primitive.Scale
        g_pEngineCamera->PeekCameraPos(), transform, mesh_primitive, primitive.Scale, material, bank);   //   first read +0x58 -> [ebp-18h]; mipmap in edx)
    //  sphere copy, transform.TransformPoint (0xBB42C8..0xBB4325), camera +0x54/58/5C minus, sqrt (X,Z,Y order 0xBB4344..0xBB4358),
    //  - Radius*Scale, * AverageTextureStretch (+0x1C); fcomp DELTA_NOTIONAL_ZERO 0xBB436A (`test ah,41h ; jne`)
    //  0xBB437B: DiffuseMapBankIndex (+0x10) `jle` (signed long); PeekGraphic bounds check (0x2C stride, 0xBB4389..0xBB43B9 -> 0)
    //  0xBB43BB: width word +6; ratio = width / (MipmapSizeMultiplier/distance*scale) -> [ebp-20h]
    //  0xBB43EA..0xBB4411 GFFloatToLongFloor (Retail-asm fistp site 0xBB43F0); GFLimit(1,255) 0xBB4407; g_highestSetBitTable[(uchar)] 0xBB4425
bool preloading = false;                                                              // 0xBB4439 [ebp+0Fh]
const CTexture* texture = CEnginePrimitiveManagerMeshBase::GetDiffuseTexture(material, composite_surfaces, mipmap, preloading, bank, false);   // 0xBB443D -> [ebp-24h]
if (preloading) {                                                                     // 0xBB444A je 0xBB450B
    GPRenderManager->FlushRender();                                                   // 0xBB4450..0xBB44CC inline, then jmp 0xBB4F75
    GPLightingManager->RestoreDefaultPrimitiveParameters(); return;                   //   (tail shared with the normal path at 0xBB4F75)
}
// 0xBB450B: pass dispatch (edi = pass)
if (pass == 8) {                         // RENDER_PASS_OUTLINE_SMOOTH_STENCIL
    SetupOutlineSmoothStencilStates(*primitive.MeshEffect (+0x68), primitive.Scale (+0x58));   // 0xBB4520 call 0x00BEFD90; jmp 0xBB461A
    -> common A
} else if (pass == 10) {                 // 0xBB452A OUTLINE_SMOOTH_FILL
    GPShaderRenderManager->ApplyVertexShader(TriVertexShaderOutlineSmoothFill (+0x5C), false);   // 0xBB4545
    if (material->DiffuseMapBankIndex != 0 && material->Transparent (+0x29)) {                   // 0xBB454A..0xBB455B
        GPShaderRenderManager->ApplyPixelShader(shared->PixelShaderOutlineSmoothFillAlpha (+0x204)); // 0xBB456E
        SetupOutlineSmoothFillStates(effect, scale, true, 1);                                     // 0xBB457E
    } else {
        ApplyPixelShader(shared->PixelShaderOutlineSmoothFill (+0x1FC));                          // 0xBB459B
        SetupOutlineSmoothFillStates(effect, scale, false, 1);                                    // 0xBB45AB
    }                                                                                             // jmp 0xBB4E8D
} else if (pass == 9) {                  // 0xBB45B5 OUTLINE_SMOOTH_ALPHA_STENCIL_MASK
    ApplyVertexShader(TriVertexShaderOutline (+0x54), false);                                    // 0xBB45CC
    SetupOutlineSmoothAlphaStencilMaskStates(effect);                                             // 0xBB45D4
    ApplyPixelShader(shared->PixelShaderDiffuseColourTextureDiffuseAlpha (+0xE4));               // via 0xBB4E81
} else if (pass == 12) {                 // 0xBB45EA OUTLINE_STENCIL
    SetupOutlineStencilStates(effect, scale);                                                     // 0xBB45FC call 0x00BF0740 -> common A
} else if (pass == 13) {                 // 0xBB4603 OUTLINE_FILL
    SetupOutlineFillStates(effect, scale);                                                        // 0xBB4615 call 0x00BF0B20 -> common A
    // common A 0xBB461A: ApplyVertexShader(TriVertexShaderOutline (+0x54), false); ApplyPixelShader(shared->PixelShaderDiffuseOnly (+0xDC)) via 0xBB4E81
    // (common A is the merged tail of three arms: the source likely repeats the two calls in each arm of 8/12/13)
} else {                                 // 0xBB463C default
    unsigned long flags = sub_primitive.RenderFlags;
    if (flags & 0x80) SetupStencilMask(primitive.MeshEffect, flags, pass);                         // 0xBB4651 (`test al,al ; jns`)
    const CVertexShader* shaders;  unsigned long n = lights.GetCount();                           // 0xBB465B
    if (flags & 0x200) { if (flags & 0x10) shaders = TriVertexShadersSpotShadow (+0x44); else shaders = TriVertexShadersShadow (+0x40); }
    else shaders = TriVertexShaders.begin() (+0x04);                                              // each arm clamps n to 5 (`cmp eax,5 ; jb` UNSIGNED)
    ApplyVertexShader(shaders[min(n,5)], false);                                                  // 0xBB46A4
    SetupAlphaBlendingStatesFromRenderFlags(flags, pass, sub_primitive.Alpha, sub_primitive.Parent->AlphaRef (+0x28));  // 0xBB46A9..0xBB4D11 inline
        // pass==1 (0xBB46CA): ZWrite 1 (+0x2854), AlphaBlend 0 (+0x28B8), AlphaTest 1 (+0x28CC), AlphaRef alpha_ref (+0x28E0), ColourWrite 0 (+0x2840, tail merged 0xBB4CD5)
        // else 0xBB48AE: flags&2 ? {AlphaTest 1; AlphaRef (alpha_ref*alpha)>>8 (imul/sar 8)} : {AlphaTest 0; AlphaRef 0}
        //   0xBB4A42 flags&1 ? {ZWrite 0; AlphaBlend 1; SrcBlend SRC_ALPHA (0x01396F78); DestBlend INV_SRC_ALPHA (0x01396F7C); AlphaTest 1}
        //                    : 0xBB4C53 {ZWrite 1; AlphaBlend 0}
    GPLightingManager->SetSelfIlluminationAmbient(material->SelfIllumination (+0x24));            // 0xBB4D17
    GPLightingManager->UpdateGlobalLightingStates();                                              // 0xBB4D3E
    bool tinted = (primitive.TintColour.IntValue & 0xFFFFFF) != 0xFFFFFF;                         // 0xBB4D43 (setne cl, computed before the flag tests)
    if (flags & 0x200) {
        if (flags & 0x10) {                                                                       // 0xBB4D65
            ApplyPixelShader(tinted ? *shared->PixelShaderDiffuseFogShadowOutputFactor (+0x228) : *shared->PixelShaderDiffuseFogShadow (+0x224));   // 0xBB4D8E
            CEngineShadowRenderer::CColorBufferDepthPackParameters params = {4, 5, 0};            // [ebp-34h]
            g_pShadowRenderer->SetupShadowFuncRenderStates(1, transform, params);                  // 0xBB4DB8
            GPLightingManager->SetupShaderSpotLight(transform);                                    // 0xBB4DC4
        } else {                                                                                  // 0xBB4DCB
            ApplyPixelShader(tinted ? ... : ...);                                                 // 0xBB4DF1
            params = {4,5,0}; SetupShadowFuncRenderStates(1, transform, params);                  // 0xBB4E1B
        }
        GPLightingManager->SetEnableFadedShadowFactor(GPLightingManager->ShadowMode == 1);       // 0xBB4E20
    } else {                                                                                      // 0xBB4E49
        GPLightingManager->SetEnableFadedShadowFactor(ShadowMode == 1);
        ApplyPixelShader(shared->PixelShaderDiffuse (+0xF4));                                     // 0xBB4E75 -> 0xBB4E81 call 0x00988140
    }
}
// 0xBB4E8D common
bool two_sided = material->TwoSided (+0x28);                                                     // -> [ebp+0Ch]
if (occlusion_test) g_pOcclusionManager->StartOcclusionTest();                                    // 0xBB4EA3
RenderPolygons(simulation, program, transform, lights, &texture, 1, false, two_sided, sub_primitive.RenderFlags,
               true, true, 0x109, pass, primitive);                                               // 0xBB4EDF call 0x00BB2540
if (occlusion_test) g_pOcclusionManager->EndOcclusionTest(occlusion_handle);                      // 0xBB4EF2
GPRenderManager->FlushRender();                                                                   // 0xBB4EF7..0xBB4F6F (render manager kept in [ebp+18h]; `mov edi,edi` pad 0xBB4F3E)
GPLightingManager->RestoreDefaultPrimitiveParameters();                                           // 0xBB4F75..0xBB4FF6
// ~CScopedShadersEnable 0xBB4FFD (vertex half; jumps back to 0xBB44D1/0xBB44D6 for the pixel half), ~capture block -> RestoreCaptureBlock
//   0xBB44FD / 0xBB502E, epilogues 0xBB4502 / 0xBB5033.
```

## Notes for the author
- Statement order before the shaders scope matters: retail emits the capture ctor, the occlusion block, THEN CScopedShadersEnable
  (the enable calls follow the occlusion code at 0xBB3FAE).
- The preloading early-out runs FlushRender then RestoreDefaultPrimitiveParameters (opposite of agent2060's early returns).
- Cold/merged blocks: 0xBB44D1..0xBB4508 (pixel-shader dtor half + epilogue 1) sits in the middle (after the preloading
  FlushRender); 0xBB5023..0xBB5039 epilogue 2 at the end. No jump tables (the pass dispatch is a cmp chain 8,10,9,12,13).
- `push 0` ApplyVertexShader second arg = force false; RenderPolygons constants 0x109 / 1 / 1 / 0 / count 1 (see DOSSIER sec. 3 arg map).
