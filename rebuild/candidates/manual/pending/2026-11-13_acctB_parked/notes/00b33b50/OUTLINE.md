# OUTLINE 0x00B33B50 CEnginePrimitiveRenderer::CEnginePrimitiveRenderer()  (0x00B33B50..0x00B35DEA, 8858 bytes)

## Frame
- Prologue: `push ebp; mov ebp,esp; and esp,-8; sub esp,18h; push ebx; push ebp; push esi; xor ebx,ebx; push edi; mov esi,ecx;
  mov [esp+14h],ebx` (ebx=0 kept to the end; ebp re-pushed as a GPR). No fs:0 / EH states, no /GS. Epilogue at 0x00B35DDA:
  `pop edi; mov [0x01436E78],esi; mov eax,esi; pop esi; pop ebp; pop ebx; mov esp,ebp; pop ebp; ret`. No cold blocks.
- Frame map (offsets from esp after the 4 pushes; 0x18 bytes = [esp+10h..27h]):
  - [esp+13h] 1 byte: the `__true_type` tag passed by address to vector<CEngineScene*>::_M_insert_overflow (`lea edx,[esp+1Bh]` after 2 pushes).
  - [esp+14h] dword: VC7.1 conditional-temporary flags (bit0 MainScene CCharString, bit1 RepMeshScene CCharString).
  - [esp+18h] 8 bytes: MainScene `new` result (CEngineScene*, push_back arg) / RepMeshScene CCharString / every
    `GetPixelShader` CPixelShader return temporary (vptr +18h, Data +1Ch).
  - [esp+20h] 8 bytes: MainScene CCharString / RepMeshScene `new` result / every shader-name CCharString / the
    CCountedPointer<CEnginePrimitiveManager> argument temporary (Data +20h, Info +24h) / "EnableSprites", "EnableFlareSprites" strings.
- Registers: esi=this, ebx=0, edi=&SceneList (+0x338) from 0x00B33CA9 to 0x00B3411E, ebp = `new` result / &WeatherMaskManager
  (0x00B34110..0x00B345E2) / 1 (0x00B34641..) / vtable 0x0129A030 (0x00B3478E..0x00B35680) / &PixelShaderDiffuseEnvBumpFogDepthBufferShadow
  (+0x134, from 0x00B3570E block to 0x00B35C5C).

## Body (retail order)
```
CEnginePrimitiveRenderer::CEnginePrimitiveRenderer()
    // 0x00B33B65 call 0x00B36CE0 CEnginePrimitiveRendererSharedData()   (base 1, ecx=this)
    // 0x00B33B72 call 0x00B59710 CEngineComponent()  (base 2, ecx=this+0x268)
    // 0x00B33B77 [this+0x268]=0x012A1348 ; [this]=0x012A1380   (vptrs)
    // 0x00B33B83..0x00B33CB6 member default ctors, declaration order, all `mov [esi+X],ebx`:
    //   PrimitiveManagers (+0x274/278/27c vector zeros), 23 CCountedPointer() (Data,Info pairs +0x280..+0x334),
    //   SceneList (+0x338/33c/340 via edi = esi+0x338)
{
    // 0x00B33CB4  new(0x5c) -> ebp ; null -> 0x00B33CFE (xor eax,eax)
    //   0x00B33CC7 CCharString tmp("MainScene") at [esp+20h], flags=1, call 0x00B94A80
    SceneList.push_back(new CEngineScene(128.0f, 6, 0.7f, 6.0f, CCharString("MainScene")));
    //   0x00B33D00 inline push_back: if (finish != eos) { if (finish) *finish = p; ++finish; } else (0x00B33D18)
    //   _M_insert_overflow 0x00B389D0 (finish, &p, &tag, 1, true) ; 0x00B33D2E if (flags&1) { flags&=~1; ~CCharString 0x0099EAE0 }
    // 0x00B33D49 same with 1.2f (0x3F99999A), "RepMeshScene" (CCharString at [esp+18h], flags|=2), value at [esp+20h];
    //   0x00B33DC3 if (flags&2) ~CCharString (no clear)
    SceneList.push_back(new CEngineScene(128.0f, 6, 1.2f, 6.0f, CCharString("RepMeshScene")));

    // Each: push size ; call new ; test ; push edi(&SceneList) ; ctor ; else eax=0 ; push eax ; lea ecx,[esi+M] ; call Reset
    RepeatedZSpriteManager.Reset(new CEnginePrimitiveManagerRepeatedZSprites(SceneList));            // 0x00B33DD3 0xc4  ctor 0x00BABB50 Reset 0x00B37680 (+0x298)
    StaticMeshManager.Reset(new CEnginePrimitiveManagerStaticMeshes(SceneList));                     // 0x00B33DFC 0x318 ctor 0x00BA1A70 Reset 0x00B37470 (+0x280)
    AnimatedMeshManager.Reset(new CEnginePrimitiveManagerAnimatedMeshes(SceneList));                 // 0x00B33E25 0x690 ctor 0x00BD1EF0 Reset 0x00B37520 (+0x288)
    RepeatedStaticMeshManager.Reset(new CEnginePrimitiveManagerRepeatedStaticMeshes(SceneList));     // 0x00B33E4E 0x94  ctor 0x00B99D90 Reset 0x00B375D0 (+0x290)
    SpriteManager.Reset(new CEnginePrimitiveManagerSprites(SceneList));                              // 0x00B33E77 0xc   ctor 0x00BACF70 Reset 0x00B37730 (+0x2a0)
    FlareSpriteManager.Reset(new CEnginePrimitiveManagerFlareSprites(SceneList));                    // 0x00B33E9D 0x24  ctor 0x00BCE070 Reset 0x00B377E0 (+0x2a8)
    WeaponTrailManager.Reset(new CEnginePrimitiveManagerWeaponTrails(SceneList));                    // 0x00B33EC3 0x30  ctor 0x00BCC5F0 Reset 0x00B37890 (+0x2b0)
    SpriteTrailManager.Reset(new CEnginePrimitiveManagerSpriteTrails(SceneList));                    // 0x00B33EE9 0x30  ctor 0x00BCA770 Reset 0x00B37940 (+0x2b8)
    LineManager.Reset(new CEnginePrimitiveManagerLines(SceneList));                                  // 0x00B33F0F 0x14  ctor 0x00BC7E90 Reset 0x00B379F0 (+0x2c0)
    TextManager.Reset(new CEnginePrimitiveManagerText(SceneList));                                   // 0x00B33F35 0x2c  ctor 0x00BC6B50 Reset 0x00B37AA0 (+0x2c8)
    OcclusionGroupManager.Reset(new CEnginePrimitiveOcclusionGroupManager(SceneList));               // 0x00B33F5B 0x20  INLINE ctor: new->ebp,
        //   push edi; call 0x00B90270 CEnginePrimitiveManagerOcclusionBase(CArray&); mov [ebp],0x012A1308 ; Reset 0x00B37B90 (+0x2d8)
    PrimitiveGroupManager.Reset(new CEnginePrimitiveGroupManager(SceneList));                        // 0x00B33F8A 0x20  ctor 0x00BC61F0 Reset 0x00B37C40 (+0x2e0)
    PrimitiveChangerManager.Reset(new CEnginePrimitiveChangerManager(SceneList));                    // 0x00B33FB0 0xc   ctor 0x00BC5570 Reset 0x00B37CF0 (+0x2e8)
    ParticleLineGroupManager.Reset(new CEnginePrimitiveManagerParticleLineGroup(SceneList));         // 0x00B33FD6 0x260 ctor 0x00BC4D90 Reset 0x00B37E50 (+0x2f8)
    ParticleMeshGroupManager.Reset(new CEnginePrimitiveManagerParticleMeshGroup(SceneList));         // 0x00B33FFF 0x2dc ctor 0x00BC34D0 Reset 0x00B37F00 (+0x300)
    ParticleSpriteGroupManager.Reset(new CEnginePrimitiveManagerParticleSpriteGroup(SceneList));     // 0x00B34028 0x328 ctor 0x00BC14E0 Reset 0x00B37FB0 (+0x308)
    ParticleSpriteTrailGroupManager.Reset(new CEnginePrimitiveManagerParticleSpriteTrailGroup(SceneList, SpriteTrailManager));
        // 0x00B34051 0x40: lea ecx,[esi+2B8h]; push ecx; push edi; ctor 0x00BBEAF0 ; Reset 0x00B38060 (+0x310)
    ParticleGroupManager.Reset(new CEnginePrimitiveManagerParticleGroup(SceneList));                 // 0x00B3407E 0x28  ctor 0x00BBBF40 Reset 0x00B38110 (+0x318)
    DecalManager.Reset(new CEnginePrimitiveManagerDecal(SceneList));                                 // 0x00B340A4 0x124 ctor 0x00BBA480 Reset 0x00B381C0 (+0x320)
    DecalGroupManager.Reset(new CEnginePrimitiveManagerParticleDecalGroup(SceneList));               // 0x00B340CD 0x124 ctor 0x00BB6E00 Reset 0x00B38270 (+0x328)
    WeatherMaskManager.Reset(new CEnginePrimitiveManagerWeatherMask(SceneList));                     // 0x00B340F6 0xc   ctor 0x00BB6A30;
        // 0x00B34110 lea ebp,[esi+330h]; Reset 0x00B38320 (ebp reused at 0x00B345E2)

    // 21 x (block = 0x3D bytes): tmp(Data,Info) copied to [esp+20h], if(Info) ++RefCount, push &tmp, call 0x00B33AE0,
    //   if (Info && --RefCount==0) { ecx=Info->Data; call Info->DeleteFunc; operator delete(Info) }  (no zero stores)
    AddPrimitiveManager(RepeatedZSpriteManager);          // 0x00B3411E (Info +0x29c)
    AddPrimitiveManager(StaticMeshManager);               // 0x00B3415B
    AddPrimitiveManager(AnimatedMeshManager);             // 0x00B34198
    AddPrimitiveManager(RepeatedStaticMeshManager);       // 0x00B341D5
    AddPrimitiveManager(SpriteManager);                   // 0x00B34212
    AddPrimitiveManager(FlareSpriteManager);              // 0x00B3424F
    AddPrimitiveManager(WeaponTrailManager);              // 0x00B3428C
    AddPrimitiveManager(SpriteTrailManager);              // 0x00B342C9
    AddPrimitiveManager(LineManager);                     // 0x00B34306
    AddPrimitiveManager(TextManager);                     // 0x00B34343
    AddPrimitiveManager(OcclusionGroupManager);           // 0x00B34380
    AddPrimitiveManager(PrimitiveGroupManager);           // 0x00B343BD
    AddPrimitiveManager(PrimitiveChangerManager);         // 0x00B343FA
    AddPrimitiveManager(ParticleLineGroupManager);        // 0x00B34437
    AddPrimitiveManager(ParticleMeshGroupManager);        // 0x00B34474
    AddPrimitiveManager(ParticleSpriteGroupManager);      // 0x00B344B1
    AddPrimitiveManager(ParticleSpriteTrailGroupManager); // 0x00B344EE
    AddPrimitiveManager(ParticleGroupManager);            // 0x00B3452B
    AddPrimitiveManager(DecalManager);                    // 0x00B34568
    AddPrimitiveManager(DecalGroupManager);               // 0x00B345A5
    AddPrimitiveManager(WeatherMaskManager);              // 0x00B345E2 (via ebp)
    // (implicit conversion CCountedPointer<Derived> -> CCountedPointer<CEnginePrimitiveManager>: the header's template copy ctor)

    ClothRenderer.Reset(new CEngineClothRenderer());      // 0x00B34619 new(0xf0), ctor 0x00BB5040 (no args), Reset 0x00B37DA0 (+0x2f0)

    // 0x00B34641 ebp=1; store order as retail (source order = this order is the safe first guess):
    BlockOnLoad = false;                       // +0x04
    OcclusionEnable = false;                   // +0xbd
    OcclusionHierarchyEnable = false;          // +0xbe
    OcclusionHierarchyEnableConsole = false;   // +0xbf
    CurrentOcclusionLayer = (EEngineOcclusionLayer)-1;   // +0x78
    BumpMappingEnable = false;                 // +0xbc
    DynamicVBUsage = 0;                        // +0xc4
    DynamicVBPool = (EVertexBufferPool)1;      // +0xc8 (ebp)
    DynamicVBLockFlags = 0;                    // +0xcc
    DynamicIBUsage = 0;                        // +0xd0
    DynamicIBPool = (EIndexBufferPool)2;       // +0xd4
    DynamicIBLockFlags = 0;                    // +0xd8
    GlobalPrimitiveInitFlags = 0;              // +0xc0
    NEngineInternal::GPShaderRenderManager->SetSoftwareVertexShaderEnable(false);    // 0x00B34696 call 0x009886F0
    NEngineInternal::GPShaderRenderManager->SetSoftwareFixedFunctionEnable(false);   // 0x00B346A2 call 0x00988700
    if (NEngineInternal::GPDisplayManager->GetVertexShaderVersion() < 1100) {        // 0x00B346AE call 0x009BEDE0; cmp 44Ch; jae 0x00B346FB
        NEngineInternal::GPShaderRenderManager->SetSoftwareVertexShaderEnable(true); // 0x00B346C0 (push ebp=1)
        DynamicVBUsage |= 0x10;                // D3DUSAGE_SOFTWAREPROCESSING; loads c4,d0 then c0, stores c4,d0,c0
        DynamicIBUsage |= 0x10;
        GlobalPrimitiveInitFlags |= 1;
    }
    // 0x00B346FB ecx = GPResourceManager->MeshBanks[0] (+0x10); if (ecx) SetDP3BumpEnable(BumpMappingEnable) 0x00A23B60 (movzx-style xor edx,edx; mov dl)
    if (NEngineInternal::GPResourceManager->MeshBanks[0])
        NEngineInternal::GPResourceManager->MeshBanks[0]->SetDP3BumpEnable(BumpMappingEnable);   // exact accessor spelling unproven

    // 39 shader blocks (0x88 bytes each; 0x8C for the ones storing vtable 0x0129A030 as an immediate). Each:
    //   CCharString(name,-1) at [esp+20h]; ecx = [[0x01436E98]+0x2A98]; GetPixelShader 0x00A5D720(&ret[esp+18h], &str);
    //   member = ret (CPixelShader::operator=, header shape); ~CPixelShader(ret) inline; ~CCharString 0x0099EAE0
    //   X = NEngineInternal::GPResourceManager->GetPShaderResource()->GetPixelShader("NAME");
    PixelShaderDiffuseOnly                         = ..("PSHADER_DIFFUSE_ONLY");                       // 0x00B34716 +0xdc
    PixelShaderDiffuseColourTextureDiffuseAlpha    = ..("PSHADER_DIFFUSE_COLOUR_TEXTURE_DIFFUSE_ALPHA"); // 0x00B347A0 +0xe4
    PixelShaderTextureOnly                         = ..("PSHADER_TEXTURE_ONLY");                       // 0x00B34828 +0xec
    PixelShaderDiffuse                             = ..("PSHADER_TEXTURE_DIFFUSE");                    // 0x00B348B0 +0xf4
    PixelShaderDiffuseFog                          = ..("PSHADER_TEXTURE_DIFFUSE_FOG");                // 0x00B34938 +0xfc
    PixelShaderDiffuseEnvFog                       = ..("PSHADER_TEXTURE_DIFFUSE_ENV_FOG");            // 0x00B349C0 +0x104
    PixelShaderDiffuseEnvBumpFog                   = ..("PSHADER_TEXTURE_DIFFUSE_ENV_BUMP_FOG");       // 0x00B34A48 +0x10c
    PixelShader2DDiffuseOnly                       = ..("PSHADER_2D_DIFFUSE_ONLY");                    // 0x00B34AD0 +0x19c
    PixelShader2DTextureOnly                       = ..("PSHADER_2D_TEXTURE_ONLY");                    // 0x00B34B58 +0x1a4
    PixelShader2DDiffuse                           = ..("PSHADER_2D_TEXTURE_DIFFUSE");                 // 0x00B34BE0 +0x1ac
    PixelShader2DDiffuseEnv                        = ..("PSHADER_2D_TEXTURE_DIFFUSE_ENV");             // 0x00B34C68 +0x1b4
    PixelShader2DDiffuseEnvBump                    = ..("PSHADER_2D_TEXTURE_DIFFUSE_ENV_BUMP");        // 0x00B34CF0 +0x1bc
    PixelShader2DDiffuseOnlyAdditiveAlpha          = ..("PSHADER_2D_DIFFUSE_ONLY_ADDITIVE_ALPHA");     // 0x00B34D78 +0x1cc
    PixelShader2DTextureOnlyAdditiveAlpha          = ..("PSHADER_2D_TEXTURE_ONLY_ADDITIVE_ALPHA");     // 0x00B34E00 +0x1d4
    PixelShader2DDiffuseAdditiveAlpha              = ..("PSHADER_2D_TEXTURE_DIFFUSE_ADDITIVE_ALPHA");  // 0x00B34E88 +0x1dc
    PixelShader2DDiffuseEnvAdditiveAlpha           = ..("PSHADER_2D_TEXTURE_DIFFUSE_ENV_ADDITIVE_ALPHA");      // 0x00B34F10 +0x1e4
    PixelShader2DDiffuseEnvBumpAdditiveAlpha       = ..("PSHADER_2D_TEXTURE_DIFFUSE_ENV_BUMP_ADDITIVE_ALPHA"); // 0x00B34F98 +0x1ec
    PixelShaderOutlineSmoothFill                   = ..("PSHADER_OUTLINE_SMOOTH_FILL");                // 0x00B35020 +0x1fc
    PixelShaderOutlineSmoothFillAlpha              = ..("PSHADER_OUTLINE_SMOOTH_FILL_ALPHA");          // 0x00B350A8 +0x204
    PixelShaderOutlineSmoothFillDP3                = ..("PSHADER_OUTLINE_SMOOTH_FILL_DP3");            // 0x00B35130 +0x20c
    PixelShaderOutlineSmoothFillDP3Alpha           = ..("PSHADER_OUTLINE_SMOOTH_FILL_DP3_ALPHA");      // 0x00B351B8 +0x214
    PixelShaderDepthShadowBufferRender             = ..("PSHADER_SHADOW_BUFFER_RENDER");               // 0x00B35240 +0x14c
    PixelShaderDepthShadowBufferRenderAlpha        = ..("PSHADER_SHADOW_BUFFER_RENDER_ALPHA");         // 0x00B352C8 +0x154
    PixelShaderColourShadowBufferRender            = ..("PSHADER_COLBUFF_SHADOW_BUFFER_RENDER");       // 0x00B35350 +0x15c
    PixelShaderColourShadowBufferRenderAlpha       = ..("PSHADER_COLBUFF_SHADOW_BUFFER_RENDER_ALPHA"); // 0x00B353D8 +0x164
    PixelShaderDiffuseFogDepthBufferShadow         = ..("PSHADER_TEXTURE_DIFFUSE_FOG_SHADOW");         // 0x00B35460 +0x11c
    PixelShaderDiffuseFogDepthBufferShadowOutputFactor = ..("PSHADER_TEXTURE_DIFFUSE_FOG_SHADOW_OUTPUT_FACTOR");      // 0x00B354E8 +0x124
    PixelShaderDiffuseDP3BumpFogDepthBufferShadow  = ..("PSHADER_TEXTURE_DIFFUSE_DP3_BUMP_FOG_SHADOW");               // 0x00B35570 +0x13c
    PixelShaderDiffuseDP3BumpFogDepthBufferShadowOutputFactor = ..("PSHADER_TEXTURE_DIFFUSE_DP3_BUMP_FOG_SHADOW_OUTPUT_FACTOR"); // 0x00B355F8 +0x144
    PixelShaderDiffuseEnvFogDepthBufferShadow      = ..("PSHADER_TEXTURE_DIFFUSE_ENV_FOG_SHADOW");                    // 0x00B35680 +0x12c
    PixelShaderDiffuseEnvBumpFogDepthBufferShadow  = ..("PSHADER_TEXTURE_DIFFUSE_ENV_BUMP_FOG_SHADOW");               // 0x00B3570C +0x134 (lea ebp,[esi+134h] kept)
    PixelShader2DDiffuseDP3Bump                    = ..("PSHADER_2D_TEXTURE_DIFFUSE_DP3_BUMP");                       // 0x00B35798 +0x1c4
    PixelShader2DDiffuseDP3BumpAdditiveAlpha       = ..("PSHADER_2D_TEXTURE_DIFFUSE_DP3_BUMP_ADDITIVE_ALPHA");        // 0x00B35824 +0x1f4
    if (NEngineInternal::GPDisplayManager->GetPixelShaderVersion() >= 1400) {   // 0x00B358B0 call 0x009BEE00; cmp 578h; jb 0x00B35C0E
        PixelShaderDiffuseFogColourBufferShadow              = ..("PSHADER_TEXTURE_DIFFUSE_FOG_COLBUFF_SHADOW");                    // 0x00B358C6 +0x16c
        PixelShaderDiffuseFogColourBufferShadowOutputFactor  = ..("PSHADER_TEXTURE_DIFFUSE_FOG_COLBUFF_SHADOW_OUTPUT_FACTOR");      // 0x00B35952 +0x174
        PixelShaderDiffuseDP3BumpFogColourBufferShadow       = ..("PSHADER_TEXTURE_DIFFUSE_DP3_BUMP_FOG_COLBUFF_SHADOW");           // 0x00B359DE +0x18c
        PixelShaderDiffuseDP3BumpFogColourBufferShadowOutputFactor = ..("PSHADER_TEXTURE_DIFFUSE_DP3_BUMP_FOG_COLBUFF_SHADOW_OUTPUT_FACTOR"); // 0x00B35A6A +0x194
        PixelShaderDiffuseEnvFogColourBufferShadow           = ..("PSHADER_TEXTURE_DIFFUSE_ENV_FOG_COLBUFF_SHADOW");                // 0x00B35AF6 +0x17c
        PixelShaderDiffuseEnvBumpFogColourBufferShadow       = ..("PSHADER_TEXTURE_DIFFUSE_ENV_BUMP_FOG_COLBUFF_SHADOW");           // 0x00B35B82 +0x184
    }
    // (PixelShaderDiffuseDP3BumpFog +0x114 is never assigned.)
    // 0x00B35C0E pointer members (retail stores 21c,220,224,228,22c,234,230(ebp),238 -- scheduler order)
    PixelShaderShadowBufferRender                = &PixelShaderDepthShadowBufferRender;              // +0x21c = +0x14c
    PixelShaderShadowBufferRenderAlpha           = &PixelShaderDepthShadowBufferRenderAlpha;         // +0x220 = +0x154
    PixelShaderDiffuseFogShadow                  = &PixelShaderDiffuseFogDepthBufferShadow;          // +0x224 = +0x11c
    PixelShaderDiffuseFogShadowOutputFactor      = &PixelShaderDiffuseFogDepthBufferShadowOutputFactor; // +0x228 = +0x124
    PixelShaderDiffuseEnvFogShadow               = &PixelShaderDiffuseEnvFogDepthBufferShadow;       // +0x22c = +0x12c
    PixelShaderDiffuseEnvBumpFogShadow           = &PixelShaderDiffuseEnvBumpFogDepthBufferShadow;   // +0x230 = +0x134
    PixelShaderDiffuseDP3BumpFogShadow           = &PixelShaderDiffuseDP3BumpFogDepthBufferShadow;   // +0x234 = +0x13c
    PixelShaderDiffuseDP3BumpFogShadowOutputFactor = &PixelShaderDiffuseDP3BumpFogDepthBufferShadowOutputFactor; // +0x238 = +0x144

    NEngineInternal::GPSubPrimitiveRenderer->SetMaxSortDistance(10000.0f);   // 0x00B35C68 call 0x00B84290 (0x461C4000)

    // 0x00B35C78 retail store order (eax=640.0f, then eax=1.0f, ecx=FLT_MAX 0x7F7FFFFF):
    LODVirtualScreenSize = 640.0f;                 // +0xa4
    LODVirtualScreenSizeFOVAdjusted = 640.0f;      // +0xa8
    DrawDistanceFactor = 1.0f;                     // +0x7c
    DrawDistanceMinimum = 0.0f;                    // +0x80 (ebx)
    DrawDistanceMaximum = FLT_MAX;                 // +0x84
    MaximumZSpriteAsMeshDistance = FLT_MAX;        // +0x94
    DrawDistanceInvalid = false;                   // +0x98
    RepeatedMeshDrawDistanceFactor = 1.0f;         // +0x88
    ZSpriteDrawDistanceFactor = 1.0f;              // +0x8c
    MinimumZSpriteAsMeshDistance = 0.0f;           // +0x90
    LODErrorTolerance = g_pEngineDef_01436e20->LODErrorTolerance;                     // +0x9c <- [[0x01436E20]+0x3c]
    CharacterLODErrorTolerance = g_pEngineDef_01436e20->CharacterLODErrorTolerance;   // +0xa0 <- [[0x01436E20]+0x40] (global reloaded)
    MipmapQuality = 1.0f;                          // +0xac
    MipmapSizeMultiplier = 0.0f;                   // +0xb0
    PreloadMipmapTolerance = 0;                    // +0xb4
    PreloadMipmapMaxSize = 256;                    // +0xb8
    BoundingSphereViewEnabled = false;             // +0x240
    BoundingBoxViewEnabled = false;                // +0x241
    ShowMeshInfo = 0;                              // +0x244
    ShowPreloadColours = false;                    // +0x248
    ShowVisibilityInfo = 0;                        // +0x24c
    ShowSilhouettes = 0;                           // +0x250
    ShowSilhouettesConsole = 0;                    // +0x254
    BoneViewEnabled = false;                       // +0x25b
    BoundingRadiusFix = 0.0f;                      // +0x23c
    MaxPreloadRadius = 15.0f;                      // +0x344 (0x41700000)
    MinPreloadRadius = 5.0f;                       // +0x348 (0x40A00000)
    MaxPreloadMipmapTolerance = 4.0f;              // +0x34c (0x40800000)
    VisibilityRatiosUpdate = false;                // +0x258
    VisibilityRatiosReceiveUpdate = false;         // +0x259
    VisibilityRatiosValid = false;                 // +0x25a
    Show2DBoundingRect = false;                    // +0x25c
    Show2DBoundingSphere = false;                  // +0x25d
    ShowUser2DBoundingSphere = false;              // +0x25e
    Show2DRotationPos = false;                     // +0x25f
    HighlightMirroredTexturesEnabled = false;      // +0x262
    LODForced = -1;                                // +0x264
    // (the "EnableSprites" CCharString ctor at 0x00B35CD8 is interleaved into these stores by the scheduler)

    SpriteManager->AddEnableConsoleVariable("EnableSprites");             // 0x00B35CD8 ecx=[esi+2A0h] (CCountedPointer operator->), call 0x00BACD80
    FlareSpriteManager->AddEnableConsoleVariable("EnableFlareSprites");   // 0x00B35DB1 ecx=[esi+2A8h], call 0x00BACD80

    g_pPrimitiveRenderer = this;                   // 0x00B35DDB [0x01436E78] = esi
}
```
Inline expansions and where they already match:
- CPixelShader::operator= / ~CPixelShader / ~CShaderBase: engine/CPixelShader.h (matched in 0x00BC14E0, 0x00B36EC0).
- CCountedPointer template copy-ctor + dtor (temp for AddPrimitiveManager), default ctor zeros, Reset (called): engine/CCountedPointer.h.
- vector<CEngineScene*>::push_back: stlport/fable_containers.h:479 (FABLE_RETAIL_CARRAY for CArray).
- new + null test + ctor + `xor eax,eax` else: plain `new X(args)`.
No jump tables.
