# OUTLINE 0x00B73760 CEngineWaterRenderer::CEngineWaterRenderer(const CEngineInit& init)

## Frame
- `sub esp,0Ch ; push ebx ; push ebp ; push esi ; push edi` (saved order ebx, ebp, esi, edi), no ebp frame, no SEH, no /GS,
  epilogue `pop edi ; mov eax,esi ; pop esi ; pop ebp ; pop ebx ; add esp,0Ch ; ret 4` at 0xB751E3.
- Registers: esi = this; ebx = 0 for the whole body (from 0xB7378E); ebp = 1.0f (0x3F800000) during the vertex-buffer ctors, then
  &IceForegroundShadowedVertexShader (+0x138, set at 0xB738A3 while zeroing it, reused at 0xB73E99..), then &+0x144 (0xB7412A),
  then scratch; edi = &ForegroundTextureVertexBuffers / &SeaBackgroundVertexBuffer, then &IceForegroundVertexShader (+0x12C,
  0xB73895, reused by the first push_back group), later the constants 8 / 0x0129B3D0 / 0x0129A054.
- Stack map (offsets at body level, no pushes outstanding):
  - [esp+10h] compiler scratch: the returned CVertexShader* (eax) for the first two assignments only.
  - [esp+14h..1Bh] the by-value return temporary (CVertexShader or CPixelShader: vptr +14h, Data +18h), ONE slot shared by all 41
    statements; dtor = inline ~CVertexShader/~CPixelShader (vptr store 0x0129B3D0/0x0129A054, Release) + ~CShaderBase (0x0129A030 store).
  - [esp+20h] = the `init` parameter slot; after InitFlags is read (0xB73ABB) it is reused for the CCharString name temporary
    (ctor 0x0099EBF0 with `push -1 ; push str ; lea ecx,[esp+28h]`, dtor 0x0099EAE0 `lea ecx,[esp+20h]`). The push_back tag
    `const __false_type&` also gets &[esp+20h] (empty object, no store).

## Member-initialisation phase (implicit, declaration order) 0xB73767..0xB73AB6
```
0xB73769  CEngineComponent::CEngineComponent()            call 0x00B59710 ; vptr = 0x012A3364
0xB73777  ForegroundIndexBuffers        CIndexBuffer()    call 0x00AC1EC0  (+0x0C)
0xB7377F  ForegroundTextureIndexBuffers CIndexBuffer()    call 0x00AC1EC0  (+0x20)
0xB73784  ForegroundTextureVertexBuffers CVertexBufferWin32() INLINE (+0x34): call CBaseClassNonCopyable 0x0099A2F0; Length=Type=0;
          CompressionParams 1,1,1,1,0,0,0,0; vptr 0x0129BA20; PVertexBuffer=0; Stride=0 (byte)
0xB737BC  SeaBackgroundIndexBuffer      CIndexBuffer()    call 0x00AC1EC0  (+0x68)
0xB737C7  SeaBackgroundVertexBuffer     CVertexBufferWin32() INLINE (+0x7C), same as above
0xB73801..0xB7388A  CVertexShader() 0x00A62FB0 / CPixelShader() 0x00A5EC00 for +0xBC .. +0x124 (declaration order; 0xFC is a PixelShader)
0xB73895  IceForegroundVertexShader / IceForegroundShadowedVertexShader (CArray: 3 zero dwords each, via edi / ebp)
0xB738B2  IceForegroundColbuffShadowedVertexShader (+0x144..+0x14C zeros)
0xB738CA..0xB73974  +0x150 .. +0x1D0 shader ctors (call each)
0xB7397F..0xB73A33  ten CArray render lists +0x1FC..+0x26C (30 zero stores, one per dword in order; last +0x270 store at 0xB73A33)
0xB73A39  LoadedOrLoadingSeaBodyID CCharString() 0x0099E4B0 ; 0xB73A44 RequestedSeaBodyID same
0xB73A4F  Settings  CEngineWaterSettings() 0x00BE9920 (+0x280)
0xB73A54  Tesselator (+0x564): zeros +0x568/+0x56C/+0x570 (base BridgeIndexBuffer), vptr 0x012A3334, zeros +0x578/+0x57C/+0x580
0xB73A82  SeaTesselator (+0x584): zeros +0x588/+0x58C/+0x590, vptr 0x012A334C, zeros +0x598/+0x59C/+0x5A0
0xB73AB6  StaticMapSeaEntryPrefix CCharString() 0x0099E4B0 (+0x5AC)
0xB73ABF  LoadingSeaEntry / LoadingSea CCountedPointer(): zeros +0x5B0/+0x5B4/+0x5B8/+0x5BC
```
(WaterGenerator, StaticMapBankFile, WaterBumpFactor, the bools, +0xB0..+0xB8 and the texture members get NO initializer here.)

## Body (retail order)
```cpp
// 0xB73ABB  (mov eax,[esp+20h] ; mov cl,[eax+60h] ; test cl,cl ; jns 0xB73AEE)
if (init.InitFlags & 0x80) {           // see DOSSIER 7 for the spelling
    HighendWaterSupported = true;      // 0xB73ADE  +0x275
    HighendWaterEnabled   = true;      // 0xB73AE5  +0x274 ; jmp 0xB73AFA
} else {
    HighendWaterSupported = false;     // 0xB73AEE
    HighendWaterEnabled   = false;     // 0xB73AF4
}
// each statement: CCharString temp ctor, global reload, Get*Shader(&tmp, name), member op= or push_back, temp dtors.
// Vn = g_pResourceManager->GetVShaderResource((EShaderBankNo)n)->GetVertexShader("...")   (0x00A5D5F0)
// P  = g_pResourceManager->GetPShaderResource()->GetPixelShader("...")                    (0x00A5D720)
0xB73AFA ForegroundVertexShader       = V7("VSHADER_WATER_FOREGROUND");          // +0xEC  (op= through [esp+10h])
0xB73B84 BackgroundVertexShader       = V8("VSHADER_WATER_BACKGROUND");          // +0x104
0xB73C0D IceForegroundVertexShader.push_back(V7("VSHADER_ICE_FOREGROUND"));      // edi=+0x12C; overflow call 0xB73C69
0xB73CA4 IceForegroundVertexShader.push_back(V7("VSHADER_ICE_FOREGROUND_1_LIGHTS"));
0xB73D3C IceForegroundVertexShader.push_back(V7("VSHADER_ICE_FOREGROUND_2_LIGHTS"));
0xB73DD4 IceForegroundVertexShader.push_back(V7("VSHADER_ICE_FOREGROUND_3_LIGHTS"));
0xB73E6E IceForegroundShadowedVertexShader.push_back(V7("VSHADER_ICE_FOREGROUND_SHADOWED"));   // ebp=+0x138
0xB73F14 IceForegroundShadowedVertexShader.push_back(V7("VSHADER_ICE_FOREGROUND_SHADOWED_1_LIGHTS"));
0xB73FAB IceForegroundShadowedVertexShader.push_back(V7("VSHADER_ICE_FOREGROUND_SHADOWED_2_LIGHTS"));
0xB74042 IceForegroundShadowedVertexShader.push_back(V7("VSHADER_ICE_FOREGROUND_SHADOWED_3_LIGHTS"));
0xB740DB if (g_pDisplayManager_01436e1c->GetPixelShaderVersion() >= 1400) {   // call 0x009BEE00 ; cmp eax,578h ; jb 0xB7435D
0xB740F1   IceForegroundColbuffShadowedVertexShader.push_back(V7("VSHADER_ICE_FOREGROUND_COLBUFF_SHADOWED"));   // ebp=+0x144
0xB74196   ...push_back(V7("VSHADER_ICE_FOREGROUND_COLBUFF_SHADOWED_1_LIGHTS"));
0xB7422D   ...push_back(V7("VSHADER_ICE_FOREGROUND_COLBUFF_SHADOWED_2_LIGHTS"));
0xB742C4   ...push_back(V7("VSHADER_ICE_FOREGROUND_COLBUFF_SHADOWED_3_LIGHTS"));
         }
0xB7435D IceBackgroundVertexShader              = V8 ("VSHADER_ICE_BACKGROUND");               // +0x150
0xB743E4 SeaBackgroundVertexShader              = V9 ("VSHADER_SEA_BACKGROUND");               // +0xBC  (+0x2A08)
0xB74466 SeaForegroundReflectiveVertexShader    = V7 ("VSHADER_SEA_FOREGROUND_REFLECTIVE");    // +0xCC
0xB744E8 SeaForegroundNonReflectiveVertexShader = V7 ("VSHADER_SEA_FOREGROUND_NON_REFLECTIVE");// +0xDC
0xB7456A BumpmapVertexShader                    = V13("VSHADER_WATER_BUMPMAP");                // +0x178 (+0x2A28)
0xB745EC IceBumpmapVertexShader                 = V13("VSHADER_ICE_BUMPMAP");                  // +0x188
0xB7466E BumpmapRainVertexShader                = V13("VSHADER_WATER_BUMPMAP_RAIN");           // +0x198
0xB746F0 EnvironmentMapVertexShader             = V13("VSHADER_WATER_ENVIRONMENT_MAP");        // +0x1A8
0xB74772 SkyMapVertexShader                     = V13("VSHADER_WATER_SKY_MAP");                // +0x1B8
0xB747F4 ForegroundPixelShader                  = P("PSHADER_WATER_FOREGROUND");               // +0xF4
0xB74881 BackgroundPixelShader                  = P("PSHADER_WATER_BACKGROUND");               // +0x10C
0xB74909 IceForegroundPixelShader               = P("PSHADER_ICE_FOREGROUND");                 // +0x158
0xB74991 IceForegroundShadowedPixelShader       = P("PSHADER_ICE_FOREGROUND_SHADOWED");        // +0x160
0xB74A19 RefractionForegroundPixelShader        = P("PSHADER_WATER_REFRACTION_FOREGROUND");    // +0xFC
0xB74AA1 if (g_pDisplayManager_01436e1c->GetPixelShaderVersion() >= 1400)   // jb 0xB74B3F
0xB74AB7   IceForegroundColbuffShadowedPixelShader = P("PSHADER_ICE_FOREGROUND_COLBUFF_SHADOWED"); // +0x168
0xB74B3F SeaBackgroundLowEndVertexShader        = V9 ("VSHADER_SEA_BACKGROUND_LOW_END");       // +0x114
0xB74BC5 WaterBackgroundLowEndVertexShader      = V8 ("VSHADER_WATER_BACKGROUND_LOW_END");     // +0x11C
0xB74C4B WaterLowEndPixelShader                 = P("PSHADER_WATER_LOW_END");                  // +0x124
0xB74CD3 IceBackgroundPixelShader               = P("PSHADER_ICE_BACKGROUND");                 // +0x170
0xB74D5B SeaBackgroundPixelShader               = P("PSHADER_SEA_BACKGROUND");                 // +0xC4
0xB74DE3 SeaForegroundReflectivePixelShader     = P("PSHADER_SEA_FOREGROUND_REFLECTIVE");      // +0xD4
0xB74E6B SeaForegroundNonReflectivePixelShader  = P("PSHADER_SEA_FOREGROUND_NON_REFLECTIVE");  // +0xE4
0xB74EF3 BumpmapPixelShader                     = P("PSHADER_WATER_BUMPMAP");                  // +0x180
0xB74F7B IceBumpmapPixelShader                  = P("PSHADER_ICE_BUMPMAP");                    // +0x190
0xB75003 BumpmapRainPixelShader                 = P("PSHADER_WATER_BUMPMAP_RAIN");             // +0x1A0
0xB7508B EnvironmentMapPixelShader              = P("PSHADER_WATER_ENVIRONMENT_MAP");          // +0x1B0
0xB75113 SkyMapPixelShader                      = P("PSHADER_WATER_SKY_MAP");                  // +0x1C0
// (EnvMapOverlay*Shader +0x1C8/+0x1D0 are NOT looked up.)
0xB7519D DynamicTexturePageHandle = 0;   // +0x1D8
         EnvironmentMap = 0;             // +0x1E0
         SeaBumpMap = 0;                 // +0x1E8
         WaterBumpMap = 0;               // +0x1F0
         IceBumpMap = 0;                 // +0x1F8
         HaveBackgroundSea = false;      // +0x276 (byte)
         WaterGenerator = 0;             // +0x5A4
         StaticMapBankFile = 0;          // +0x5A8
         WaterBumpFactor = 1.5f;         // +0x5C0 = 0x3FC00000
0xB751D7 InitialiseDynamicTextures();    // call 0x00B6D510
0xB751DE BuildConstantBuffers();         // call 0x00B6EDF0
```
## Inline expansions per statement (all proven in the landed 0xBC14E0 with the shared headers)
- Assignment (vertex): `mov ecx,[esi+M+4] ; cmp ecx,ebx ; je ; dec [ecx+10h] ; jne ; call [vptr] (push 1)` then `Data = tmp.Data ;
  inc [Data+10h]` = CVertexShader::operator= (engine/CVertexShader.h). Pixel: same with +8 and an extra `mov [esi+M+4],ebx`
  inside the if = CPixelShader::operator= (engine/CPixelShader.h).
- push_back: `mov ecx,[arr+4] ; cmp ecx,[arr+8] ; je overflow ; if (ecx) { copy-construct } ; [arr+4] += 8 ; else push 1, push 1,
  push &tag, push &tmp, push finish ; call 0x00B6C120` = CArray::push_back -> STLport vector::push_back (fable_containers.h 479).
- temp dtor: `mov eax,[esp+18h] ; cmp eax,ebx ; mov [esp+14h],vptr ; je ; Release ; ... mov [esp+14h],0129A030h ; call ~CCharString`.
