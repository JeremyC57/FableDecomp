# OUTLINE 0x00B3CB30 CEngineResourceManager::CEngineResourceManager(const CEngineInit& init, const CEngineMemoryInit& memory_init)

## Frame
- `sub esp,0C8h ; push ebx,ebp,esi,edi` (saved order ebx, ebp, esi, edi); no ebp frame, no EH, no cookie; `ret 8`; eax = this.
- E = esp after pushes. Args: init [E+0xDC], memory_init [E+0xE0]. Frame 0xC8 bytes:
| slot | holds (lifetime) |
|---|---|
| E+0x10 | bool temp for AllocatePools (0x00B3CD27); CWideString L"pc\\" (0x00B3CD64..CDB0); CCharString temp names (const char*,-1) for PS consts/MBANK_ENGINE; CWideString L"editmesh.bbb"; loop counter (10) of vector fill; CRGBColour (E+0x10..0x13) late |
| E+0x14 | CWideString result1 (ShaderResourcePath + L"pc\\"); CCharString results of operator+(const char*, suffix); `textureCount` (0x00B3D83D) ; then CPixelFormat A8R8G8B8 (0x00B3DADB); then C2DExtentsI(1,1) at E+0x14..0x1B |
| E+0x1F | dummy byte (__false_type tag / allocator temp) passed by address to _M_insert_overflow and __destroy_aux |
| E+0x20 | CCharString `suffix` (default-ctor 0x00B3CE99, never assigned, dtor last 0x00B3E02F) |
| E+0x24..0x33 | C4DVector constant value (PS consts); CCountedPointer<CShaderResource> temp from CreateResource (E+0x24/0x28); `textureCount/16` (E+0x24); CTexture temp (E+0x24/0x28); CSurface temp (E+0x24, vptr 0x0122F84C) |
| E+0x34..0x3F | CArray<CTextureManagerPoolInfo> pools (start/finish/eos) |
| E+0x40..0x53 | CTextureManagerPoolInfo temp {Width,Height,PixelFormat,UseMipmaps,Count} |
| E+0x54 | CPixelFormat dxt1 ; E+0x58 CPixelFormat dxt3 |
| E+0x5C | CWideString result2 (early) / ConvertDirectoryAndFilenameToPath result (editor) / spill of MainGraphicBank PixelFormats[6] |
| E+0x60 | CPixelFormat alphaFormat (= -1, then GetBestAlphaTextureFormat) |
| E+0x64 | CWideString unused local (default-ctor 0x00B3D723 .. dtor 0x00B3DF05) |
| E+0x68 | CVertexShaderInputStatic staticBump (0x1C) |
| E+0x84 | CVertexShaderInputAnimated animated |
| E+0xA0 | CVertexShaderInputAnimated animatedBump |
| E+0xBC | CVertexShaderInputStatic staticInput |
Local declaration/construction order: suffix, staticInput(BC), animated(84), staticBump(68), animatedBump(A0), unused CWideString(64), pools(34);
destruction in reverse at 0x00B3DEF4: pools, CWideString 64, A0, 68, 84, BC, suffix.

## Body (retail order)
```
// --- member/base construction (init list, member order) ---
0B3CB3C  CEngineComponent::CEngineComponent()                 // 0x00B59710
         CAsyncLoadingProcessCallback inline ctor: [+0xC]=0x012A1AF8; then vptrs [+0]=0x012A1B3C, [+0xC]=0x012A1B34
0B3CB58  EditorMeshBank  CMeshDataBank()  0x00A27030 ; 0B3CB63 EngineMeshBank CMeshDataBank()
0B3CB68  edi=0; 5 CCountedPointer default ctors +0x8E0..+0x904 = 0 (Data,Info)
0B3CBAC  AnimationManager C3DAnimationManager() 0x00AA0F60
         (MeshBanks/MainMeshBank/GraphicBanks/MainGraphicBank/EngineFont: no init)
0B3CBB1  for 27: VShaderBanks[i] CShaderDataBank() 0x00A62020 (ebx ptr += 0x128, ebp count 0x1B, loop 0B3CBC0)
0B3CBD0  ShaderBankPixel CShaderDataBank()
0B3CBDB  for 27: VShaderResources[i] = {0,0} (loop 0B3CBE6); ShaderResourcePixel {0,0} 0B3CBF1
0B3CBFD  StipleFadeTexture CTexture() {0,0}; HWInverseStipplePatterns / DitherTextureList CArray() {0,0,0}
0B3CC33  EngineCachePath, WriteableEngineCachePath CWideString() 0x0099AED0
0B3CC43  ZeroRGBATexture, SatRGBATexture CTexture() {0,0}
0B3CC63  VSConstantLayoutBasic/Lights/LightsBones/RepeatedMesh/Particles ctors (0xBDBB70/BDB400/BDB260/BDB080/BDAFF0)
0B3CC9A  GPUScratchBufferTextureManager() 0x00F37520
0B3CC9F  ExternalTexturesAllocatedFromEnginePools: STLport map ctor INLINE: [+0x31D4]=0 ; hdr=malloc(0x1C) 0x00BFEA0E;
         [+0x31D4]=hdr; [+0x31D8]=0 (count); hdr->color(byte)=0; hdr->parent=0; hdr->left=hdr; hdr->right=hdr
// --- body ---
0B3CCAC  MaxLoadingTime = 0.001;              // fld [0x0126F430] ... fstp [+0x2AA0] at 0B3CCE3 (scheduled)
0B3CCE9  EngineFont = init.Font;  MainGraphicBank = init.MainGraphicBank;  MainMeshBank = init.MeshBank;
0B3CD01  if (MainMeshBank && MainMeshBank->GetTextureBank() == NULL)      // [ecx+0x3D4]
0B3CD13      MainMeshBank->SetTextureBank(init.MainGraphicBank /*edx reused*/);   // 0x00A23B50
0B3CD18  AllocatePools(MainMeshBank != NULL, memory_init);                 // 0x00B3B240 (bool via setne->byte E+0x10->dword push)
0B3CD38  EngineCachePath = init.EngineCacheResourcePath;                   // 0x0099B7D0
0B3CD47  WriteableEngineCachePath = init.WriteableEngineCacheResourcePath;
0B3CD56  if (GBankFileManager.IsUsingRetailBanks())                         // byte [0x013CA7B0]
0B3CD5F      GBankFileManager.OpenRetailBank(init.ShaderResourcePath + CWideString(L"pc\\") + L"shaders.big", false);
             // CWideString(L"pc\\") 0x0099B6B0 @E+0x10 ; op+(W,W) 0x0099BE70 -> E+0x14 ; op+(W,PBG) 0x0099BF30 -> E+0x5C ; OpenRetailBank 0x009A8840;
             // dtors E+0x5C, E+0x14, E+0x10 (0x0099B510). The false is pushed FIRST (0B3CD6D) before the operands.
0B3CDB5  GPShaderRenderManager->RegisterVertexShaderConstantLayout(1, &VSConstantLayoutBasic);   // 0x0098B230
         ... (2,&VSConstantLayoutLights) (3,&...LightsBones) (4,&...RepeatedMesh) (5,&...Particles)
0B3CE17  if (init.InitFlags & 0x20) {
0B3CE1D      EditorMeshBank.OpenFileReadWrite(CAFile::ConvertDirectoryAndFilenameToPath(init.MeshResourcePath, CWideString(L"editmesh.bbb")), 1, 0x200);
             // vptr loaded+spilled before the call (0B3CE2B); virtual slot 4 (+0x10); result E+0x5C, name E+0x10; dtors 5C then 10
0B3CE68      EditorMeshBank.SetTextureBank(MainGraphicBank);          // reads [+0x958]
0B3CE76      EditorMeshBank.SetClothMemoryPool(ClothMemoryPool);      // by-value copy: sub esp,8; Data, Info; if(Info) ++*Info
         }
0B3CE99  CCharString suffix;                                          // 0x0099E4B0 @E+0x20
0B3CEA2  { C4DVector v; CCharString("PSCONST_ZERO") ; v=(0,0,0,0); GPShaderRenderManager->SetPixelShaderConstant(name, v); ~name }  // 0x0098B390
         Order / values (strings 0x012A1E98.. ; float imm stores even for 0.0f):
         PSCONST_ZERO (0,0,0,0) | PSCONST_ONE (1,1,1,1) | PSCONST_HALF (.5,.5,.5,.5) | PSCONST_1_0_0_0 | PSCONST_0_1_0_0 | PSCONST_0_0_1_0 |
         PSCONST_0_0_0_1 | PSCONST_SHADOW_FADE_COLOUR (.5x4) | PSCONST_MAX_FOG_ALPHA (.5x4, string 0x0129A0EC) | PSCONST_TFACTOR (1,1,1,1)
         each: push -1; push str; CCharString(const char*,long) @E+0x10; push &v(E+0x24); push &name; ecx=GPShaderRenderManager (load position varies); call; ~CCharString
0B3D1AE  CVertexShaderInputStatic staticInput(0,false)  @E+0xBC: base ctor 0x00AB3B10, vptr 0x0129DFB8, Initialise(0,false) 0x00A90560
0B3D1D3  CVertexShaderInputAnimated animated(0,false)    @E+0x84: base ctor, vptr 0x0129DFB0 (ebp holds it), Initialise(0,false) 0x00A90630
0B3D1F9  SetupVertexShaderResource((EShaderBankNo)1, "SHADERS_STATIC" + suffix, staticInput);   // 0x00B3B6D0; op+ 0x0099F690 -> E+0x14; ~ after
0B3D226  SetupVertexShaderResource((EShaderBankNo)3, "SHADERS_PALSKIN" + suffix, animated);
0B3D254  SetupVertexShaderResource(bank, "NAME" + suffix, decl, vtype)   // 0x00B3B5D0, one block each, in this order:
         (0,  SHADERS_STATIC,               0x0129D6B0, (EVertexType)0x112)
         (12, SHADERS_LANDSCAPE_FOREGROUND, 0x0129D980, 0)   (11, SHADERS_LANDSCAPE_BACKGROUND, 0x0129D958, 0)
         (13, SHADERS_POS_COL_TEX1,         0x0129D6D0, 0)   (7,  SHADERS_WATER_FOREGROUND,     0x0129D9A8, 0)
         (8,  SHADERS_WATER_BACKGROUND,     0x0129D9E8, 0)   (9,  SHADERS_SEA_BACKGROUND,       0x0129DA18, 0)
         (5,  SHADERS_SKY,                  0x0129D6D0, 0)   (6,  SHADERS_SKY_SCREEN_SPACE,     0x0129D708 g_Declaration2D, 0)
         (14, SHADERS_REPEATED_MESH,        0x0129D8C0, 0)   (16, SHADERS_POINT_SPRITE1,        0x0129D6D0, 0)
         (24, SHADERS_PARTICLE_SPRITE_TRAIL,0x0129D6D0, 0)   (23, SHADERS_MESH_GROUP,           0x0129D8C0, 0)
         (21, SHADER_SPRITE_GROUP (sic),    0x0129D940, 0)   (22, SHADERS_DECAL_GROUP,          0x0129D920, 0)
         (17, SHADERS_ZSPRITE,              0x0129D8E8, 0)   (26, SHADERS_TEXT,                 0x0129DA28, 0)
         (20, SHADERS_VERTEX_POS,           0x0129D748, 0)   (10, SHADERS_WEATHER,              0x0129D900, 0)
         (25, SHADERS_DEBUGGING,            0x0129D788, 0)
         push order: vtype(edi), decl, &suffix, [edx=literal, ecx=&tmp E+0x14, call op+], push eax, push bank, ecx=esi
0B3D5C7  CVertexShaderInputStatic staticBump(2,false) @E+0x68 ; 0B3D5E4 CVertexShaderInputAnimated animatedBump(2,false) @E+0xA0 (vptr from ebp)
0B3D606  SetupVertexShaderResource((EShaderBankNo)2, "SHADERS_STATIC_BUMP" + suffix, staticBump);
0B3D631  SetupVertexShaderResource((EShaderBankNo)4, "SHADERS_PALSKIN_BUMP" + suffix, animatedBump);
0B3D65F  ShaderBankPixel.OpenReadOnly("PIXEL_SHADERS" + suffix, 0x48);           // vptr loaded first (ebp), virtual slot 1, ~tmp
0B3D68D  ShaderResourcePixel = GPShaderRenderManager->CreateResource();          // 0x0098B860 ret E+0x24; inline CCountedPointer::operator=
         (0B3D69D..0B3D6E9: if (old.Info != new.Info) { if (old.Info && --*old.Info==0) { Info->DeleteFunc(Info->Data); operator delete(Info) 0x00BFE9BC } Data=..; Info=..; if (Info) ++*Info })
         then ~temp (0B3D6EC..0B3D70E same release body)
0B3D711  ShaderResourcePixel->OpenShaderBank(&ShaderBankPixel);                 // 0x00A5D7C0, ecx = Data
0B3D723  CWideString unused;                                                     // E+0x64 default ctor, never used
0B3D72C  for (i = 0; i < 3; ++i) MeshBanks[i] = NULL;   // pointer loop &MeshBanks[0] .. &MainMeshBank
0B3D73F  if (MainMeshBank) {
0B3D747      EngineMeshBank.OpenReadOnly(CCharString("MBANK_ENGINE"), 0x48);      // vslot 1; ebp=&EngineMeshBank
0B3D778      MeshBanks[0] = MainMeshBank; MeshBanks[1] = &EditorMeshBank; MeshBanks[2] = &EngineMeshBank;
0B3D787      MainMeshBank->SetMeshDataMemoryPool(MeshDataMemoryPool, true);       // 0x00A24150 (push ebx=1 first)
0B3D7AC      EngineMeshBank.SetMeshDataMemoryPool(MeshDataMemoryPool, false);
         }
0B3D7D0  CPixelFormat dxt1; dxt1.Initialise(D3DFMT_DXT1 0x31545844) @E+0x54 ; CPixelFormat dxt3; dxt3.Initialise(D3DFMT_DXT3 0x33545844) @E+0x58
         (no -1 store: the formats are declared without the CPixelFormat() ctor writing, or the store is dead -- check CPixelFormat.h CPIXELFORMAT_INIT)
0B3D7EC  long textureSize = CTextureManager::CalculateTextureSize(0x200, 0x200, true, dxt1);   // ebp
0B3D7FE  long hiresKB = memory_init.HiresTextureMemory;
0B3D80A  if (videoOptions /*0x01436EB0*/) hiresKB = GPEngine->PeekVideoOptionsDef(videoOptions->TextureDetail /*+0x24*/).HiresTextureMemory /*+0x3C*/;  // 0x00B244F0
0B3D826  long textureCount = (hiresKB << 10) / textureSize;    // E+0x14 ; textureCount / 16 -> E+0x24 (signed: cdq/and 0Fh/add/sar 4)
0B3D82C  CArray<CTextureManagerPoolInfo> pools(10);   // inline: start/finish/eos=0; malloc(200); eos=start+200; fill 10 copies of
         CTextureManagerPoolInfo() (only PixelFormat=-1 defined; other fields copied from the uninitialised temp E+0x40) with
         placement-new null test; loop counter in E+0x10 (0B3D869..0B3D8A5); finish = end
0B3D8A7  pools[0] = CTextureManagerPoolInfo(32,32,dxt1,true,128);  pools[1] = (32,32,dxt3,true,64);
         pools[2] = (64,64,dxt1,true,64);  pools[3] = (64,64,dxt3,true,64);  pools[4] = (128,128,dxt1,true,64);
         pools[5] = (128,128,dxt3,true,64); pools[6] = (256,256,dxt1,true,128); pools[7] = (256,256,dxt3,true,24);
         pools[8] = (512,512,dxt1,true,textureCount); pools[9] = (512,512,dxt3,true,0);
         (each: bool via byte store into temp E+0x4C then dword copy of +0xC; index reloads `mov ebp,[esp+34h]; add ebp,i*20`)
0B3DA59  pools.push_back(CTextureManagerPoolInfo(32,32, MainGraphicBank->PixelFormats[7] /*+0x2A4*/, true, 64));
         (PixelFormats[6] /*+0x2A0*/ read at the same time into E+0x5C; ebp=&MainGraphicBank)
         push_back fast path: if (finish != eos) { if (finish) *finish = x; ++finish } else _M_insert_overflow(finish, x, __false_type(), 1, true) 0x00B3EA70
0B3DAD9  CPixelFormat argb; argb.Initialise(D3DFMT_A8R8G8B8 0x15) @E+0x14;
         pools.push_back(CTextureManagerPoolInfo(512,512,argb,false,1));
0B3DB51  pools.push_back(CTextureManagerPoolInfo(512,512, PixelFormats[6] (E+0x5C), true, textureCount/16));
0B3DBB5  if (!MainGraphicBank->TextureManager->Initialised) MainGraphicBank->TextureManager->Initialise(pools);  // [+0x280] ->[+4]; 0x00A6AAA0
0B3DBCF  if (MainMeshBank) {
             MainMeshBank->SetAnimationMemoryPool(AnimationMemoryPool, true);   // 0x00A24050
             MainMeshBank->SetClothMemoryPool(ClothMemoryPool);                 // 0x00A240E0
             MainMeshBank->SetLoadingMemoryPool(LoadingMemoryPool);             // 0x00B3E820 (CBankFileAsync)
             MainMeshBank->SetLoadingMemoryFailureHandler(LoadingMemoryFailureHandler);  // 0x009D66A0
         }
0B3DC69  MainGraphicBank->SetLoadingMemoryPool(LoadingMemoryPool);             // ecx=[ebp] (CGraphicDataBank is a CBankFileAsync)
0B3DC8D  MainGraphicBank->SetLoadingMemoryFailureHandler(LoadingMemoryFailureHandler);
0B3DCB1  if (MeshDataMemoryPool) EditorMeshBank.SetMeshDataMemoryPool(MeshDataMemoryPool, false);
0B3DCDC  if (AnimationMemoryPool) EditorMeshBank.SetAnimationMemoryPool(AnimationMemoryPool, false);
0B3DD05  if (ClothMemoryPool) EditorMeshBank.SetClothMemoryPool(ClothMemoryPool);
0B3DD2F  if (LoadingMemoryPool) EditorMeshBank.SetLoadingMemoryPool(LoadingMemoryPool);
0B3DD57  if (LoadingMemoryFailureHandler) EditorMeshBank.SetLoadingMemoryFailureHandler(LoadingMemoryFailureHandler);
         (test = Data != 0, inline CCountedPointer validity test)
0B3DD81  for (i = 0; i < 1; ++i) GraphicBanks[i] = NULL;  GraphicBanks[0] = MainGraphicBank;   // pointer loop to &MainGraphicBank
0B3DD9E  SetupAlphaStippleTexture((init.InitFlags >> 2) & 1);       // `shr edx,2 ; and dl,bl(1)` ; 0x00B3C5D0
0B3DDB5  CPixelFormat alphaFormat;  /* -1 store at 0B3DDC2 */  GPDisplayManager->GetBestAlphaTextureFormat(32, alphaFormat);  // 0x009BE6C0
0B3DDCF  ZeroRGBATexture = CTexture(C2DExtentsI(1,1), 1, alphaFormat, 0, (ESurfacePool)1);   // CTEXTURE_SIZED inline; 0x009FA280; operator= 0x009FA1C0; ~CTexture -> Uninitialise 0x009F9F70
0B3DE1C  ZeroRGBATexture.GetSurfaceLevel(0).Clear(CRGBColour(0,0,0,0));   // colour pushed first; 0x009F9E00 ret E+0x24; Clear 0x009F3FF0; ~CSurface: vptr 0x0122F84C, Uninitialise 0x009F2E20
0B3DE5B  SatRGBATexture = CTexture(C2DExtentsI(1,1), 1, alphaFormat, 0, (ESurfacePool)1);   // ebx=&SatRGBATexture
0B3DEA8  SatRGBATexture.GetSurfaceLevel(0).Clear(CRGBColour(255,255,255,255));  // `mov al,0FFh` x4
0B3DEE0  GPUScratchBufferTextureManager.AddScratchPool(0);         // 0x00F370C0
0B3DEEC  CAsyncLoadingMemoryPool::AddLoadingCallback(this);       // ecx = this+0xC (static fastcall) 0x00A14990
// --- scope exit, 0B3DEF4: ~pools: if (start) free(start) 0x00BFEA14 ; ~unused CWideString (E+0x64)
//     ~animatedBump(A0), ~staticBump(68), ~animated(84), ~staticInput(BC): each = if(v2.start) free; __destroy_aux(v1.start, v1.finish, tag E+0x1F) 0x00A5E900 (fastcall ecx=first, edx=last); if(v1.start) free; ~CBaseClass 0x0099A2E0
//     ~suffix 0x0099EAE0 (E+0x20)
0B3E038  pop edi ; mov eax,esi ; pop esi ; pop ebp ; pop ebx ; add esp,0C8h ; ret 8
```
No jump tables; no cold blocks after the epilogue (the function ends at 0x00B3E046 incl. padding).
