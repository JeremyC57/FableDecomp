# OUTLINE 0x00B9AC30 CEnginePrimitiveManagerRepeatedStaticMeshes::RenderSubPrimitive(CEngineSubPrimitiveBase& sub, ERenderPassType pass)

## Frame facts
- Prologue: `sub esp,0C4h ; mov eax,[__security_cookie 0x0139C8A8] ; push ebx ; push ebp ; push esi ; mov [esp+0CCh],eax`
  (cookie -> final slot 0xD0) ; ... `push edi` at 0x00B9AC5A. No ebp frame, no SEH, `ret 8`.
- Epilogue 0x00B9BC9C: `mov ecx,[esp+0D0h] ; pop edi ; pop esi ; pop ebp ; pop ebx ; call __security_check_cookie ; add esp,0C4h ; ret 8`.
  Early returns (clip list empty 0x00B9AD06, texture not ready 0x00B9B2DC / 0x00B9B323) jump straight here.
- No EH states (Flags: none). /GS cookie yes.

## Frame map (final esp offsets; slots are reused -- c2 packing)
| slot | holds (in order of use) |
|---|---|
| 0x10 | `float fade` (0x00B9AC6A) -> clip-sum loop index i -> nx pointer (normals X) -> batch loop: matrices-minus-offsets delta -> 0x10..0x12 bytes: CEnginePrimitiveThemeChannelDesc() temp {0,0,0} of RestoreDefaultPrimitiveParameters |
| 0x14 | copy loop: j*16 byte offset -> `CScopedVertexShaderConstantLayout layout(4)` (OldIndex, stored 0x00B9B270, read 0x00B9BC82) |
| 0x18 | `primitive` (sub.GetParent()) |
| 0x1C | clip countdown -> `const CTexture* texture` (0x00B9B1C7 / 0x00B9B2CA) -> batch: nx-minus-ny delta |
| 0x20 | clip cursor (wind loop) / dest scratch -> batch: `-batch_size` |
| 0x24 | `long count` (sum of clip counts / ObjectCount) -> batch: remaining |
| 0x28 | inner source index j -> batch: batch_size*16 |
| 0x2C | dest matrix offset (wind loop) / clip cursor (no-wind loop) -> batch: nz-minus-ny delta |
| 0x30 | ny base (VectorNormals + aligned) -> stipple stage (1/2) -> batch: n (this batch) |
| 0x34 | nx base (VectorNormals) -> `material` (0x00B9B0C8) -> batch: batch_size |
| 0x38 | ny pointer (selected) -> batch: ny cursor |
| 0x3C | nz-minus-ny delta -> batch: offsets cursor |
| 0x40 | nx-minus-ny delta -> `mesh_primitive` (sub MeshPrimitive, 0x00B9B079) |
| 0x44 | nz base -> &GPShaderRenderManager->D3DDevice (draw) |
| 0x48 | nz pointer (selected) -> GPShaderRenderManager copy (draw) |
| 0x4C | offsets pointer (C4DVector*) |
| 0x50 | wind pointer (`const unsigned char*`, 0 initially, 0x00B9ACB1) |
| 0x54 | matrices pointer (C4DVector*) |
| 0x58 | `CClipListElement* clip_end` (address taken -> ClipSubSections*(.., clip_end)) |
| 0x5C | `unsigned long occlusion_handle` (address taken) |
| 0x60..0x6B | `CColorBufferDepthPackParameters params = {3, 5, 0}` (0x00B9B946..56); 0x68 also the camera BR.X int spill (0x00B9B35F) |
| 0x70..0xAF | `CClipListElement clip_list[32]` (2 B each: StartIndex +0, Count +1) |
| 0xB0..0xCF | `unsigned char wind_delays[32]` |
| 0xD0 | /GS cookie |

## Pseudo-C++ in retail order
```cpp
void CEnginePrimitiveManagerRepeatedStaticMeshes::RenderSubPrimitive(CEngineSubPrimitiveBase& sub, ERenderPassType pass)
{
    // 0x00B9AC45
    CEngineInternalPrimitiveRepeatedStaticMesh* primitive = ((CEngineSubPrimitiveRepeatedStaticMesh&)sub).GetParent();   // [arg+0xC] -> [esp+18]
    const float fade = g_pEnginePrimitiveRenderer->FadeDistanceScale(+0x88) * primitive->FadeDistance.MaxDrawDistance(+0x18); // fld/fmul/fstp [esp+10]
    for (long i = 0; i < 4; ++i)                                   // 0x00B9AC70 loop (eax 0xBF0..0xC00), mov edi,edi pad
        SIMDVariableBlock->FadeDistance[i] = fade;                 // re-reads this->SIMDVariableBlock each iteration
    // 0x00B9AC87
    long aligned = (primitive->ObjectCount + 3) & ~3;
    const float* nx = primitive->VectorNormals;                    // ebx  [34]
    const float* ny = nx + aligned;                                // ebp  [30]
    const float* nz = ny + aligned;                                // edx  [44]
    const unsigned char* wind = 0;                                 // [50] = 0
    long count; const float *sel_nx,*sel_ny,*sel_nz; const C4DVector *offsets,*matrices;
    if (primitive->SubSections == 0) {                             // je 0x00B9B041
        count = primitive->ObjectCount; sel_nz = nz;               // 0x00B9B041 [24],[48]
        goto use_primitive_arrays;                                 // falls into 0x00B9B049
    }
    // 0x00B9ACBF
    CClipListElement clip_list[32]; CClipListElement* clip_end = clip_list;
    if (g_SSEAvailable)                                            // byte 0x013D2880
        ClipSubSectionsSIMD(primitive->SubSections, 0, clip_end);  // call 0x00B98E10
    else
        ClipSubSections(primitive->SubSections, 0, fade, false, clip_end);   // call 0x00B99710
    // 0x00B9ACF8
    long clip_count = clip_end - clip_list;                        // sub ; sar 1
    if (clip_count == 0) return;                                   // je 0x00B9BC9C
    count = 0;
    for (long i = 0; i < clip_count; ++i) count += clip_list[i].Count;   // 0x00B9AD20 (index kept in [10])
    if (count == primitive->ObjectCount) {                         // 0x00B9AD38
        sel_nz = nz;                                               // [48] = [44]
use_primitive_arrays:                                              // 0x00B9B049
        sel_nx = nx; sel_ny = ny;                                  // [10], [38]
        matrices = primitive->ObjectMatricies;                     // [54] (+0x90)
        offsets  = primitive->ObjectOffsets;                       // [4C] (+0x94)
        wind     = primitive->WindDelayArray;                      // [50] (+0x9C)
    } else {
        // 0x00B9AD4A: compact the visible objects into the SIMD block
        CSIMDVariableBlock* block = SIMDVariableBlock;
        sel_nx = block->NormalX; sel_ny = block->NormalY; sel_nz = block->NormalZ;   // [10],[38],[48]
        matrices = block->ObjectMatricies; offsets = block->ObjectOffsets;           // [54],[4C]
        long n = 0;                                                // eax
        if (primitive->WindDelayArray) {                           // 0x00B9AD77 (je 0x00B9AEF8)
            unsigned char wind_delays[32]; wind = wind_delays;     // [50] = esp+0xB0
            for (k over clip_list[0..clip_count)) {                // 0x00B9ADB0, countdown [1C], cursor [20]
                for (long j = clip->StartIndex; j < clip->StartIndex + clip->Count; ++j, ++n) {   // 0x00B9ADF3, bound re-read
                    SIMDVariableBlock->NormalX[n] = nx[j];
                    SIMDVariableBlock->NormalY[n] = ny[j];
                    SIMDVariableBlock->NormalZ[n] = nz[j];
                    SIMDVariableBlock->ObjectMatricies[n] = primitive->ObjectMatricies[j];   // 4 dword copy
                    SIMDVariableBlock->ObjectOffsets[n]   = primitive->ObjectOffsets[j];
                    wind_delays[n] = primitive->WindDelayArray[j];
                }
            }                                                      // jmp 0x00B9B06F
        } else {                                                   // 0x00B9AEF8: same loop, no wind copy
            for (...) for (...) { NormalX/Y/Z, ObjectMatricies, ObjectOffsets as above }   // 0x00B9AF10 / 0x00B9AF53
        }
    }
    // 0x00B9B06F
    C3DPrimitive2* mesh_primitive = ((CEngineSubPrimitiveMeshBase&)sub).MeshPrimitive;     // [40]
    const C3DMeshMaterial* material = primitive->RenderInfo.Mesh(+0xA8)->GetMaterial(mesh_primitive->MaterialIndex);  // [34], *0x30
    const CGraphicDataBank* bank = primitive->MeshInfo->PeekTextureBank();                // +0x50 -> +4 -> +0x3D4
    // 0x00B9B0B0: GraphicList.size() via /44 (0x2E8BA2E9), twice
    CGraphicInfoFrame* frame = bank->GetGraphicInfo(material->DiffuseMapBankIndex)
                             ? bank->GetGraphicInfo(material->DiffuseMapBankIndex)->Frames : 0;   // edi; = PeekGraphicInfoFrame(index, 0)
    // inline bool CGraphicInfoFrame::GetTexture(const CTexture** out, 0, 0) (FableWin 470 B), flags/mip folded:
    const CTexture* texture;
    {   // 0x00B9B102
        if (!frame->Data) {                                         // je 0x00B9B2E1 (cold, after the main body)
            frame->Data = new CFrameData(frame);                    // push 44h; call 0x00BFEA1A; ctor 0x009FE380 (null-checked new)
                                                                    // CIVCountedPointer assign: old: --refcount(+4), 0 -> vtbl slot 1 call; store
            frame->Data->BeginLoading(0, 0);                        // 0x009FDE80
            return;                                                 // jmp 0x00B9BC9C
        }
        if (!frame->Data->Frame.FrameData) {                        // je 0x00B9B2D3 (cold)
            frame->Data->BeginLoading(0, 0); return;
        }
        frame->Data->TouchedFrame = frame->GraphicInfo->Bank->CurrentFrame;   // +0x3C = (+0xC)->(+0x20)->+0x194
        frame->Data->ManagedTexture.Touch();     // res=[Data+0x24]; if (res && res->ResourceList(+8) && res->LastUsedFrame(+0x18) != list->CurrentFrame(+0x2C))
                                                 //   { res->ListRemove(); res->ListInsert(list->Head.PrevResource(+0x10), &list->Head(+4)); res->LastUsedFrame = list->CurrentFrame; }
        frame->Data->Touch();                    // same on the CFrameData's own CResource (ebx=[edi+8]) 0x00B9B165
        if ((unsigned)frame->Data->TouchedMipmapLevel > 0) frame->Data->TouchedMipmapLevel = 0;   // +0x3A, test/jbe
        if ((unsigned)frame->Data->CurrentMipmapLevel > 0) {        // +0x38 (jbe 0x00B9B2C4)
            frame->Data->BeginLoading(0, 0);                        // 0x00B9B1B9
            texture = frame->Data->Frame.FrameData->PTexture.Data;  // [[[edi+8]+1C]+8] -> [1C]
        } else
            texture = frame->Data->Frame.FrameData->PTexture.Data;  // 0x00B9B2C4, jmp back 0x00B9B1CB
    }   // => source most likely: `const CTexture* texture; if (!frame->GetTexture(&texture, 0, 0)) return;`
    // 0x00B9B1CB
    CRenderStateScopedCaptureBlock capture;                         // StartCaptureBlock on CRenderStateCaptureBlock::StateManager (0x013CB508)
    NEngineInternal::GPRenderManager->SetSpecularEnable(true);      // +0x29E4 desired 1 (0x00B9B1F9)
    CScopedVertexShaderConstantLayout layout(4);                    // 0x00B9B264: old = singleton.CurrentVSConstantLayoutIndex (0x013BC478) -> [14]; SetVertexShaderConstantLayout(4)
    // 0x00B9B279  (push 0 = the bool of ApplyVertexShader, shared by both arms)
    if (sub.RenderFlags & 8) {                                      // stipple alpha
        long stipple_stage;                                         // ebx -> [30]
        if (sub.RenderFlags & 0x200) {                              // shadowed
            GPShaderRenderManager->ApplyVertexShader(VertexShaderShadowedStippleAlpha, false);   // +0x8C
            GPShaderRenderManager->ApplyPixelShader(PixelShaderShadowedStippleAlpha);            // +0x88
            stipple_stage = 2;                                      // 0x00B9B2BA, jmp 0x00B9B34B
        } else {                                                    // 0x00B9B328
            GPShaderRenderManager->ApplyVertexShader(&VertexShaderStippleAlpha, false);          // this+0x38
            GPShaderRenderManager->ApplyPixelShader(&PixelShaderStippleAlpha);                   // this+0x30
            stipple_stage = 1;
        }
        // 0x00B9B34B camera window (g_pEngineCamera 0x01436EA0, CameraDesc.Window +0x14: TL.X a, TL.Y b, BR.X c, BR.Y d)
        GPShaderRenderManager->SetUserVertexShaderConstant(3,
            (c - a) * 0.015625f,                 // [0x0125B24C]
            (d - b) * -0.015625f,                // [0x012A41B0]
            a * 0.03125f + 0.015625,             // [0x0129DD98] float * then + DOUBLE [0x012A2788]
            b * 0.03125f + 0.015625);            // args evaluated right to left (w first); c spilled via int to [68]
        GPRenderManager->SetTextureFilterModeMin(stipple_stage, TEXTURE_FILTER_POINT);   // +0x30C4+20*st, table 0x01396E58[0]
        GPRenderManager->SetTextureFilterModeMag(stipple_stage, TEXTURE_FILTER_POINT);   // +0x3164
        GPRenderManager->SetTextureUVModeU(stipple_stage, TEXTURE_UV_WRAP);              // +0x2EE4, table 0x01396DC8[0]
        GPRenderManager->SetTextureUVModeV(stipple_stage, TEXTURE_UV_WRAP);              // +0x2F84
        GPRenderManager->AttachTextureToStage(stipple_stage, &g_pResourceManager->StipleFadeTexture);   // +0x2AAC; inline: `p ? p->PD3DTexture : 0`, SetTexture on change, TextureStagesInUse max
        // jmp 0x00B9B60C
    } else {                                                        // 0x00B9B5D4
        if (sub.RenderFlags & 0x200) {
            GPShaderRenderManager->ApplyVertexShader(VertexShaderShadowed, false);   // +0x84
            GPShaderRenderManager->ApplyPixelShader(PixelShaderShadowed);            // +0x80 (tail 0x00B9B601 shared)
        } else {
            GPShaderRenderManager->ApplyVertexShader(&VertexShader, false);          // this+0x28
            GPShaderRenderManager->ApplyPixelShader(&PixelShader);                   // this+0x20
        }
    }
    // 0x00B9B60C
    NEngineInternal::GPLightingManager->SetEnableFadedShadowFactor(<!(sub.RenderFlags & 0x200), forced false unless ShadowMode(+0x4694) == 1>);
        // retail: al = !((flags>>9)&1); if (ShadowMode != 1) al = 0;  -- fit the spelling (e.g. `ShadowMode == 1 ? !(flags & 0x200) : false`)
    NEngineInternal::GPLightingManager->UpdateGlobalLightingStates();          // 0x00B46C80
    if (primitive->WindDelayArray)                                             // 0x00B9B653
        UpdateBoneMatrixForWindSheer(*primitive, matrices, wind, count);       // 0x00B990B0 (non-const C4DVector*)
    GPRenderManager->AttachTextureToStage(0, texture);                         // 0x00B9B678
    if (material->TwoSided)                                                    // 0x00B9B6C5 (+0x28)
        GPRenderManager->SetPolyCullMode(POLY_CULL_NONE);                      // table 0x01396FB0[2] = 0x01396FB8
    GPRenderManager->SetAlphaTestEnable(true);                                 // 0x00B9B73B (+0x28CC)
    GPRenderManager->SetAlphaRef(primitive->AlphaRef);                         // 0x00B9B7A7 movzx +0x28
    GPShaderRenderManager->PrepareForRender(0x204);                            // 0x00B9B811..0x00B9B8D5 (world-space fog + camera pos; flags&1 == 0)
    GPShaderRenderManager->SelectVertexBuffer(*mesh_primitive->VertexBuffer);  // 0x00B9B8DA: D3DDevice->SetStreamSource(0, PVertexBuffer(+4), 0, Stride(+0x10 byte))
    unsigned long occlusion_handle;
    PrepareOcclusionTest(*primitive, pass, occlusion_handle);                  // 0x00B9B913 (base CEnginePrimitiveManagerOcclusionBase)
    if (occlusion_handle != (unsigned long)-1) g_pOcclusionManager->StartOcclusionTest();   // 0x00B8D090
    if (sub.RenderFlags & 0x200) {                                             // 0x00B9B92A
        CEngineShadowRenderer::CColorBufferDepthPackParameters params;         // {3, 5, 0} in member order
        params.DepthToColourLookupTextureStage = 3; params.LowByteCarryPixelShaderConstant = 5; params.LowRepeatFactorUserVertexShaderConstant = 0;
        g_pShadowRenderer->SetupShadowFuncRenderStates(1, params);             // 0x00B50510
    }
    // 0x00B9B963 batch loop (top-tested at 0x00B9B9BC, back-jump 0x00B9BB60)
    const long batch_size = g_pResourceManager->PeekVSConstantLayoutRepeatedMesh().ObjectMatricies.Count;   // +0x3034 -> [34]
    for (long done = 0 /*pointers advanced by batch_size*/; ; remaining -= batch_size) {
        long n = remaining < batch_size ? remaining : batch_size;  // cmp ebx,ecx; jl -> [30]
        if (n <= 0) break;                                          // jle 0x00B9BB65
        ProcessLightingSW(*primitive, sel_nx + done, sel_ny + done, sel_nz + done, offsets + done, n,
                          (sub.RenderFlags & 0x200) != 0);          // 0x00B99300; bool from shr 9 / and 0xFFFFFF01
        UploadShaderConstants(*primitive, offsets + done, matrices + done, n);   // 0x00B99670
        GPShaderRenderManager->PrepareForRender(0);                 // 0x00B9BA14..0x00B9BABC (no fog/camera bits)
        GPShaderRenderManager->DrawBuffersIndexed(0, mesh_primitive->VertexCount * n, *mesh_primitive->IndexBuffer,
                                                  0, mesh_primitive->TriangleCount * n, PT_NULL);
            // 0x00B9BAC1: ebx=IB(+0x34), edi=Triangles(+0x5C), ebp=Vertices(+0x58) read first; SetIndices(IB->PD3DIndexBuffer +4);
            // RenderManager(+0x1D0)->RealiseRenderState(); type = IB->PrimitiveType(+0xC); imuls; DrawIndexedPrimitive(type,0,0,v*n,0,t*n)
    }   // pointer form: ny cursor, deltas nx-ny / nz-ny / matrices-offsets; remaining += -batch_size; offsets += batch_size
    // 0x00B9BB65
    if (occlusion_handle != (unsigned long)-1) g_pOcclusionManager->EndOcclusionTest(occlusion_handle);   // 0x00B8D0A0
    NEngineInternal::GPRenderManager->FlushRender();               // 0x00B9BB7A: SetStreamSource(0,0,0,0); SetIndices(0); clear textures < TextureStagesInUse; = 0
    NEngineInternal::GPLightingManager->RestoreDefaultPrimitiveParameters();   // 0x00B9BBEF..0x00B9BC7B (theme-channel temp at [10])
}   // ~layout: 0x00B9BC82 SetVertexShaderConstantLayout(old) on 0x013BC470 ; ~capture: 0x00B9BC91 RestoreCaptureBlock 0x00A05840
```

## Cold blocks (after the main path, before the epilogue)
- 0x00B9B2C4 texture read for CurrentMipmapLevel == 0 (jumps back to 0x00B9B1CB).
- 0x00B9B2D3 FrameData-null exit: BeginLoading(0,0) -> epilogue.
- 0x00B9B2E1 Data-null exit: new CFrameData + counted-pointer assign + BeginLoading(0,0) -> epilogue.
- 0x00B9B328 stipple non-shadowed shader arm (after the shadowed arm), 0x00B9B5D4 the non-stipple shader block (after the stipple block).
- No jump tables.

## Open fitting points for the author
- The Faded-shadow bool spelling (compute-then-zero shape); the loop forms (count sum with index in memory; copy loops' countdown
  over clip entries; batch loop pointer strength reduction); `count` / pointer locals' declaration order (slot reuse above).
- `GetParent()` must be the CEngineSubPrimitiveRepeatedStaticMesh one (FableWin COMDAT after the row).
- Globals: run vland RELOC_IDENTITY (0x01436E98 / 0x01436E9C / 0x01436E5C / 0x01436E60 have several landed spellings).
