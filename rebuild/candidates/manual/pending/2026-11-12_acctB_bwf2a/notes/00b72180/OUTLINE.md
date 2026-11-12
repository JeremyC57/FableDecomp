# OUTLINE 0x00B72180 bool CEngineWaterRenderer::CreateDynamicTextures(bool render_water_bump, bool render_sea_bump, bool render_ice_bump)

## Frame facts
- `push ebp ; mov ebp,esp ; and esp,-8 ; sub esp,0A4h ; push ebx ; push esi ; push edi` (aligned frame; args via ebp+8/+0C/+10).
- No EH frame, no /GS. Saved order ebx, esi, edi; epilogues `pop edi ; pop esi ; pop ebx ; mov esp,ebp ; pop ebp ; ret 0Ch`.
- Two returns: `return false` block 0xB72992..0xB729F7 (frames destroyed in it), `return true` 0xB736E9..0xB73750.

## Frame map (esp after the three pushes)
| slot | local | type / ctor-dtor |
|---|---|---|
| +0x0F | ok | bool (`&=` of the five GetFrame results) |
| +0x10 | this | spilled ecx |
| +0x14,+0x18 | int->float temps | (unsigned)->float fix-ups of the viewport |
| +0x1C | new CFrameData* temp (expansion 1 only), later `bool old_vs` (+0x1C) and `bool old_ps` (+0x1D) | |
| +0x30 | CGraphicFrame water_frame  (env->WaterBumpMap  +0xD8) | declared 1st; ctor = 0 store at 0xB721CD |
| +0x24 | CGraphicFrame water_frame2 (env->WaterBumpMap2 +0xDC) | declared 2nd |
| +0x2C | CGraphicFrame sea_frame    (env->SeaBumpMap    +0xE0) | declared 3rd |
| +0x28 | CGraphicFrame sea_frame2   (env->SeaBumpMap2   +0xE4) | declared 4th (kept in esi at the failure test) |
| +0x20 | CGraphicFrame ice_frame    (env->IceBumpMap    +0xC8) | declared 5th |
|  | (zero stores 0x30,0x24,0x2C,0x28,0x20 = declaration order; both destructor sequences run 0x20,0x28,0x2C,0x24,0x30) | dtor: `if (FrameData && --FrameData->ReferenceCount == 0) delete FrameData` -> call ??_G 0x0041BDF0 (push 1) |
| +0x54..+0x73 | CRenderTarget render_target | inline default ctor: CBaseClass() 0x0099A2D0, vptr 0x0129C800, Target CSurface +0x58 {vptr 0x0122F84C, 0, 0}, ZBuffer CSurface +0x68 {vptr, 0, 0}; dtor: ~CSurface(+0x68), ~CSurface(+0x58) (vptr store + Uninitialise 0x009F2E20), ~CBaseClass 0x0099A2E0 |
| +0x34..+0x43 | CSurface surface | declared AFTER render_target (constructed at 0xB72D45, destroyed first); dtor vptr store + Uninitialise |
| +0x44 | CSurface temp (env-map GetSurfaceLevel return), later C2DBoxF in the 3 loops (+0x44..+0x50) | |
| +0x78 | CSurface temp (loops' GetSurfaceLevel return) | dtor right after `surface = temp` |
| +0x88,+0xA0 / +0x90,+0xA8 | C2DExtentsI temps (GetDimensions hidden returns) | |
| +0x90..+0x9C | C2DBoxF (env-map viewport) | |

## Pseudo-C++ in retail order
```cpp
bool CEngineWaterRenderer::CreateDynamicTextures(bool render_water_bump, bool render_sea_bump, bool render_ice_bump)
{
    // 0xB7218C  bank = GPResourceManager->PeekGraphicBank(0)  (GraphicBanks[0] +0x954, kept in edi for the five expansions)
    const CEnvironmentDef* env = g_pEngineEnvironmentDef;            // 0x01436E24, re-read before each expansion
    CGraphicFrame water_frame, water_frame2, sea_frame, sea_frame2, ice_frame;   // 0xB721CD..0xB721DD zero stores
    // Each line below = PeekGraphicInfoFrame(id,0) (two PeekGraphic size checks: `idx<size ? &list[idx] : 0`, then
    // `(idx<size ? p : 0)->Frames`; null info frame -> esi = 0, no null test before ->Data) + GetFrame(frame, 0, 0) const expanded:
    //   if (Data) { if (Data->Frame.FrameData) { TouchedFrame = GraphicInfo->Bank->CurrentFrame; ManagedTexture.TouchTexture();
    //       Data->TouchResource(); if (TouchedMipmapLevel > 0) TouchedMipmapLevel = 0;
    //       if (0 < CurrentMipmapLevel) { Data->BeginLoading(0,0); out = Data->Frame; return true; } out = Data->Frame; return true; }
    //     Data->BeginLoading(0,0); return false; }
    //   Data = new CFrameData(this) (op new 0x44, ctor 0x009FE380, old Data released through vtbl+4 and nulled); Data->BeginLoading(0,0); return false;
    // (flags 0 folds away the &8 / &4 / &1 / &0x10 tests; `mipmap <= ...` tests fold to true)
    bool ok =  bank->PeekGraphicInfoFrame(env->WaterBumpMap,  0)->GetFrame(water_frame, 0, 0);   // 0xB72197..0xB72347 (ok in [esp+0F] directly)
    ok     &= bank->PeekGraphicInfoFrame(env->SeaBumpMap,    0)->GetFrame(sea_frame,   0, 0);   // 0xB7234C..0xB724D5, and at 0xB724E7
    ok     &= bank->PeekGraphicInfoFrame(env->IceBumpMap,    0)->GetFrame(ice_frame,   0, 0);   // 0xB724D7..0xB72668, and at 0xB7266E
    ok     &= bank->PeekGraphicInfoFrame(env->WaterBumpMap2, 0)->GetFrame(water_frame2,0, 0);   // 0xB7267A..0xB727xx
    ok     &= bank->PeekGraphicInfoFrame(env->SeaBumpMap2,   0)->GetFrame(sea_frame2,  0, 0);   // ..0xB72988 (the last result is `test cl,al`)
    if (!ok)                                                        // 0xB7298A `test cl,al ; jne 0xB729FA`
        return false;                                               // 0xB72992..0xB729F7: 5 frame dtors, xor al,al
    // 0xB729FA
    CGPUScratchBufferTextureManager& scratch = GPResourceManager->ScratchBufferTextureManager;   // +0x3190, esi
    EnvironmentMap = scratch.StartTextureUse(EnvironmentMapHandle);   // 0x00F36EE0 (landed as StartSurfaceUse)
    WaterBumpMap   = scratch.StartTextureUse(WaterBumpMapHandle);
    SeaBumpMap     = scratch.StartTextureUse(SeaBumpMapHandle);
    IceBumpMap     = scratch.StartTextureUse(IceBumpMapHandle);
    CRenderStateCaptureBlock capture;                                // 0xB72A5A inline ctor (StateManager 0x013CB508, +0x2808/+0x2814)
    bool old_vs, old_ps;
    CShaderRenderManager::Get().SetVertexShadersEnable(true, &old_vs);   // 0xB72A88 `cmp al,1 ; je` -> EnableVertexShaders 0x00987FE0
    CShaderRenderManager::Get().SetPixelShadersEnable(true, &old_ps);    // 0xB72A9F -> EnablePixelShaders 0x00988110
    // 0xB72AB6 five SetRenderState(state, 0) on GPRenderManager: +0x2868, +0x28CC, +0x29E4, +0x28B8, +0x2944 (resolve setter names)
    // 0xB72CBB
    GPRenderManager->SelectVertexBuffer(0, &ForegroundTextureVertexBuffers);   // +0x34; SetStreamSource vtbl+0x190
    GPRenderManager->SelectIndexBuffer(&ForegroundTextureIndexBuffers, 0);     // +0x20; PSelectedIndexBuffer=, SetIndices vtbl+0x1A0
    CRenderTarget render_target;                                     // 0xB72D0F
    CSurface surface;                                                // 0xB72D45
    if (Settings.EnableReflectionGeneration && HighendWaterEnabled) { // 0xB72D18 (+0x28F), 0xB72D57 (+0x274); else -> 0xB72E62
        surface = EnvironmentMap->GetSurfaceLevel(0);                // 0x009F9E00 into temp +0x44, operator= 0x009F2E70, ~temp
        SetRenderTargetToSurface(render_target, surface);            // inline: AttachTarget 0xA10E60, GPDisplayManager->SetRenderTarget 0x9BF160,
                                                                     //   SetVWindow(C2DBoxF(0,0,GFAXToVX((float)surface.GetWidth()),GFAYToVY((float)surface.GetHeight())))
                                                                     //   (height GetDimensions first, GFAYToVY called first)
        RenderEnvironmentMap();                                      // 0xB72E49 -> 0x00B70A90
        render_target.DetachTarget();                                // 0xB72E52 0x00A10E10
        EnvironmentMap->GenerateMipmapsWithGPU();                    // 0xB72E5D 0x009FAF70
    }
    // 0xB72E62 four UV-mode setters, value D3DTextureUVModeTable[0] (0x01396DC8), re-read each time:
    GPRenderManager->SetTextureUVModeU(0, WRAP); /* +0x2EE4 */  GPRenderManager->SetTextureUVModeV(0, WRAP); /* +0x2F84 */
    GPRenderManager->SetTextureUVModeU(1, WRAP); /* +0x2EF8 */  GPRenderManager->SetTextureUVModeV(1, WRAP); /* +0x2F98 */
    GPShaderRenderManager->ApplyVertexShader(BumpmapVertexShader, false);   // 0xB73016 (+0x178)
    GPShaderRenderManager->ApplyPixelShader(BumpmapPixelShader);            // 0xB7302A (+0x180)
    if (render_water_bump) {                                         // 0xB7303C; else -> 0xB7323C
        GPRenderManager->AttachTextureToStage(0, water_frame.GetTexture());   // +0x3D00 compare, SetTexture vtbl+0x104, TextureStagesInUse>=1
        GPRenderManager->AttachTextureToStage(1, water_frame2.GetTexture());  // +0x3D04, >=2
        for (long level = 0; level < WaterBumpMap->GetNoLevels(); ++level) {   // 0xB730E5 (test + jle), loop head 0xB730FA, tail 0xB73229 `jl`
            if (WaterBumpMap->GetDimensions(level).X < 32 || WaterBumpMap->GetDimensions(level).Y < 32) break;   // `jb 0xB7323C` x2
            surface = WaterBumpMap->GetSurfaceLevel(level);          // temp +0x78
            SetRenderTargetToSurface(render_target, surface);        // box at +0x44
            RenderBumpMap((EWaterArrayIndexAliases)0);               // 0xB7321A push 0
            render_target.DetachTarget();
        }
    }
    if (render_sea_bump) {                                           // 0xB7323C [ebp+0C]; else -> 0xB73448
        AttachTextureToStage(0, sea_frame.GetTexture()); AttachTextureToStage(1, sea_frame2.GetTexture());
        for (level < SeaBumpMap->GetNoLevels()) { same 32x32 break; surface = SeaBumpMap->GetSurfaceLevel(level);
            SetRenderTargetToSurface(...); RenderBumpMap((EWaterArrayIndexAliases)1); /*0xB73422 push 1*/ render_target.DetachTarget(); }   // loop head 0xB73300
    }
    if (render_ice_bump) {                                           // 0xB73448 [ebp+10]; else -> 0xB735FA
        AttachTextureToStage(0, ice_frame.GetTexture());             // stage 0 only
        for (level < IceBumpMap->GetNoLevels()) { same; surface = IceBumpMap->GetSurfaceLevel(level);
            SetRenderTargetToSurface(...); RenderIceBumpMap(); /*0xB735D8*/ render_target.DetachTarget(); }   // loop head 0xB734C0 (aligned by `jmp`+lea pad)
    }
    // 0xB735FA
    GPRenderManager->SelectVertexBuffer(0, 0);                       // SetStreamSource(0,0,0,0)
    GPRenderManager->SelectIndexBuffer(0, 0);                        // PSelectedIndexBuffer = 0, SetIndices(0)
    GPDisplayManager->SetRenderTargetAsBackScreen();                 // 0xB7363C
    render_target.DetachTarget();                                    // 0xB73645 (or an inline Uninitialise = DetachTarget+DetachZBuffer)
    render_target.DetachZBuffer();                                   // 0xB7364E 0x00A10E20
    GPCamera->RefreshD3DTransforms();                                // 0xB73659 (0x01436EA0)
    surface.Uninitialise();                                          // 0xB73662 (explicit; no vptr store)
    // scope end: ~surface (0xB73674), ~render_target (+0x68, +0x58, ~CBaseClass 0xB73697)
    CShaderRenderManager::Get().SetVertexShadersEnable(old_vs, 0);   // 0xB7369C cmp al,[0x013BC800] ; je ; test al -> Enable/Disable
    CShaderRenderManager::Get().SetPixelShadersEnable(old_ps, 0);    // 0xB736BD
    // ~capture -> StateManager->RestoreCaptureBlock() 0xB736DE
    // ~ice_frame, ~sea_frame2, ~sea_frame, ~water_frame2, ~water_frame (0xB736E9..0xB73743)
    return true;                                                     // 0xB73748
}
```
## Scope hint from destruction order
The surface/render_target dtors run BEFORE the shader restores, which run before the capture block dtor, which runs before
the frame dtors -> nest: frames (function scope) { capture { old_vs/old_ps set; ... { render_target; surface; ... } restores } }.
A simple way: put render_target+surface in an inner block ending after `surface.Uninitialise()`, then the two Set*ShadersEnable(old, 0)
calls, all inside a block that owns `capture`. Alternatively the capture block may be a scoped helper. Check the 2048 RenderClouds model.
