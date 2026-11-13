# OUTLINE 0x00BCFF10 CEnginePrimitiveManagerAnimatedMeshesBase::CEnginePrimitiveManagerAnimatedMeshesBase(CArray<CEngineScene*>&)

## Frame facts (python3 $S/w17/xdis.py 00bcff10 0x1fdc; $W/dis_all.txt)
- __thiscall (ecx=this -> esi), one stack arg (CArray<CEngineScene*>& scenes), `ret 4` at 0x00BD1ED9, returns this (`mov eax,esi`).
- Prologue: `mov eax,[esp+4]` (arg read FIRST) ; `sub esp,0Ch` ; push ebx, ebp, esi, edi ; `push eax` ; `mov esi,ecx` ; call base.
  No frame pointer (no `push ebp; mov ebp,esp`) -> plain /O2 frame-pointer omission. NO SEH frame (no fs:0, no EH state stores)
  -> TU compiled WITHOUT /EHsc (same as the landed siblings 0x00BC14E0 "Flags: none", 0x00B99D90, 0x00BD1EF0 "no EH frame").
  No /GS cookie. Epilogue: pop edi, esi, ebp, ebx ; add esp,0Ch ; ret 4.
- Frame map (E = esp after the 4 pushes; offsets as written when no call args are pushed):
  - [E+0x10] 4 bytes: CCharString temporary (the literal -> const CCharString& conversion), ctor 0x0099EBF0(text,-1), dtor 0x0099EAE0.
  - [E+0x14] 8 bytes: CVertexShader by-value return temporary of GetVertexShader (vptr [E+0x14], Data [E+0x18]); destroyed INLINE:
    vptr=0x0129B3D0, Data->Release() expanded, vptr=0x0129A030 (~CShaderBase inline, empty).
  - [E+0x20] = the incoming arg slot (dead after the base ctor): reused to spill `dp3_shader_resource` (= VShaderResources[4]);
    read as [esp+0x24] / [esp+0x28] depending on pushes.
- Registers: esi=this; ebx = loop counter (4) during member construction, then `shader_resource` (VShaderResources[3]) from 0x00BD01AC
  to 0x00BD1D93, then the constant 0x0129B3D0 (CVertexShader vtable) from 0x00BD1DC1; ebp = &VertexShaderPalSkinOutline (this+0x2A8)
  from 0x00BD0105 to 0x00BD059E (used by the outline assignment as [ebp+4]), then the constant 0x0129A030 (CShaderBase vtable) from
  0x00BD05CB. edi = array walk pointer, then GetVertexShader's returned pointer.

## Block-by-block (retail order)
```
CEnginePrimitiveManagerAnimatedMeshesBase::CEnginePrimitiveManagerAnimatedMeshesBase(CArray<CEngineScene*>& scenes)
    : CEnginePrimitiveManagerMeshBase(scenes)            // 0x00BCFF1E call 0x00BEBC30 (landed as (void*) -- PDB says CArray<CEngineScene*>&)
{                                                        // 0x00BCFF23 mov [esi],0x012A78DC (own vftable, RTTI CEnginePrimitiveManagerAnimatedMeshesBase)
    // implicit member construction, declaration order, CVertexShader() = 0x00A62FB0 out of line, each [4] array is an
    // inline `mov ebx,4 / mov ecx,edi / call / add edi,8 / dec ebx / jne` loop (no EH -> no `eh vector constructor iterator`):
    //   0x00BCFF29 +0x038 VertexShadersPalSkin[4]          0x00BCFF3E +0x058 VertexShadersPalSkinEnv[4]
    //   0x00BCFF53 +0x078 VertexShadersPalSkinEnvBump[4]   (loop head aligned by `jmp 0x00BCFF60 ; lea ecx,[ecx]`)
    //   0x00BCFF6D +0x098 ..DepthBufferShadow[4]  0x00BCFF85 +0x0B8 ..EnvDepthBufferShadow[4]  0x00BCFF9D +0x0D8 ..EnvBumpDepthBufferShadow[4]
    //   0x00BCFFB5 +0x0F8 ..DP3BumpDepthBufferShadow[4] 0x00BCFFCD +0x118 ..SpotDepthBufferShadow[4] 0x00BCFFE5 +0x138 ..EnvSpotDepthBufferShadow[4]
    //   0x00BCFFFD +0x158 ..EnvBumpSpotDepthBufferShadow[4] 0x00BD0015 +0x178 ..DP3BumpSpotDepthBufferShadow[4]
    //   0x00BD002D +0x198 VertexShadersPalSkinDepthShadowBufferRender  0x00BD0038 +0x1A0 VertexShadersPalSkinColourShadowBufferRender (single calls)
    //   0x00BD0043 +0x1A8 ..ColourBufferShadow[4] (`mov edi,edi` align) 0x00BD005D +0x1C8 ..EnvColourBufferShadow[4] 0x00BD0075 +0x1E8 ..EnvBumpColourBufferShadow[4]
    //   0x00BD008D +0x208 ..DP3BumpColourBufferShadow[4] 0x00BD00A5 +0x228 ..SpotColourBufferShadow[4] 0x00BD00BD +0x248 ..EnvSpotColourBufferShadow[4]
    //   0x00BD00D5 +0x268 ..EnvBumpSpotColourBufferShadow[4] 0x00BD00ED +0x288 ..DP3BumpSpotColourBufferShadow[4]
    //   0x00BD0105 +0x2A8 VertexShaderPalSkinOutline (via ebp) +0x2B0 ..DP3BumpOutline +0x2B8 ..OutlineSmoothFill +0x2C0 ..DP3BumpOutlineSmoothFill
    //   +0x2C8 VertexShaders2DPalSkin +0x2D0 ..2DPalSkinEnv +0x2D8 ..2DPalSkinEnvBump +0x2E0 ..2DPalSkinDP3Bump (single calls to 0x00BD015A)
    //   (pointers +0x2E8..+0x308 have no ctor)  0x00BD015F +0x30C VertexShadersPalSkinSpotlight[4] (`lea ebx,[ebx]` align)
    //   float +0x32C and CPreTransposedBoneMatrix BoneMatrixBuffer[18] +0x330: no construction code (POD).

    // 0x00BD017D: eax=1 -> [+0x24],[+0x28]; eax=2; [+0x20]=0; [+0x2C],[+0x30]=eax; [+0x34]=3
    PointLightShaderIndex[0] = 0;
    PointLightShaderIndex[1] = 1;
    PointLightShaderIndex[2] = 1;
    PointLightShaderIndex[3] = 2;
    PointLightShaderIndex[4] = 2;
    PointLightShaderIndex[5] = 3;

    // 0x00BD01A1: eax=[0x01436E98]; ecx=[eax+0x29E0] (bank 4, spilled to the arg slot at 0x00BD01B4); ebx=[eax+0x29D8] (bank 3).
    // Loaded ONCE and kept across 61 calls -> they are source locals (the compiler cannot CSE a global load across calls).
    const CShaderResource* shader_resource     = g_pResourceManager->GetVShaderResource((EShaderBankNo)3);   // +0x29D8
    const CShaderResource* dp3_shader_resource = g_pResourceManager->GetVShaderResource((EShaderBankNo)4);   // +0x29E0
    //   (declaration order vs load order: retail loads bank 4 first; try both orders.)

    // Every statement below = one 0x76/0x78-byte block:
    //   push -1; push <literal>; lea ecx,[esp+18h]; call 0x0099EBF0 (CCharString temp at E+0x10)
    //   push &temp_str; push &ret_tmp(E+0x14); mov ecx,<resource>; call 0x00A5D5F0 (CShaderResource::GetVertexShader(const CCharString&) const)
    //   CVertexShader::operator= inline: ecx=member.Data; test; mov edi,eax; je; dec [ecx+10h]; jne; call [vtbl+0](1); Data=[edi+4]; inc [Data+10h]
    //   ~CVertexShader(ret_tmp) inline: eax=[E+0x18]; test; mov [E+0x14],0x0129B3D0; je; Release() `mov edi,[eax+10h]; mov ecx,eax;
    //     add eax,10h; dec edi; mov edx,edi; test edx,edx; mov [eax],edi; jne; call [vtbl+0](1)`; mov [E+0x14],0x0129A030 (imm, later ebp)
    //   lea ecx,[esp+10h]; call 0x0099EAE0 (~CCharString)
    VertexShadersPalSkin[0] = shader_resource->GetVertexShader("VSHADER_PALSKIN_DIRLIGHT_FOG");   // 00bd01b2 +0x038
    VertexShadersPalSkin[1] = shader_resource->GetVertexShader("VSHADER_PALSKIN_DIRLIGHT_2POINTLIGHTS_FOG");   // 00bd022c +0x040
    VertexShadersPalSkin[2] = shader_resource->GetVertexShader("VSHADER_PALSKIN_DIRLIGHT_4POINTLIGHTS_FOG");   // 00bd02a2 +0x048
    VertexShadersPalSkin[3] = shader_resource->GetVertexShader("VSHADER_PALSKIN_DIRLIGHT_5POINTLIGHTS_FOG");   // 00bd0318 +0x050
    VertexShadersPalSkinEnv[0] = shader_resource->GetVertexShader("VSHADER_PALSKIN_ENV_DIRLIGHT_FOG");   // 00bd038e +0x058
    VertexShadersPalSkinEnv[1] = shader_resource->GetVertexShader("VSHADER_PALSKIN_ENV_DIRLIGHT_2POINTLIGHTS_FOG");   // 00bd0404 +0x060
    VertexShadersPalSkinEnv[2] = shader_resource->GetVertexShader("VSHADER_PALSKIN_ENV_DIRLIGHT_4POINTLIGHTS_FOG");   // 00bd047a +0x068
    VertexShadersPalSkinEnv[3] = shader_resource->GetVertexShader("VSHADER_PALSKIN_ENV_DIRLIGHT_5POINTLIGHTS_FOG");   // 00bd04f0 +0x070
    VertexShaderPalSkinOutline = shader_resource->GetVertexShader("VSHADER_PALSKIN_OUTLINE");   // 00bd0566 +0x2a8
    VertexShaderPalSkinOutlineSmoothFill = shader_resource->GetVertexShader("VSHADER_PALSKIN_OUTLINE_SMOOTH_FILL");   // 00bd05dd +0x2b8
    VertexShadersPalSkinDepthBufferShadow[0] = shader_resource->GetVertexShader("VSHADER_PALSKIN_DIRLIGHT_FOG_SHADOW");   // 00bd0655 +0x098
    VertexShadersPalSkinDepthBufferShadow[1] = shader_resource->GetVertexShader("VSHADER_PALSKIN_DIRLIGHT_2POINTLIGHTS_FOG_SHADOW");   // 00bd06cd +0x0a0
    VertexShadersPalSkinDepthBufferShadow[2] = shader_resource->GetVertexShader("VSHADER_PALSKIN_DIRLIGHT_4POINTLIGHTS_FOG_SHADOW");   // 00bd0745 +0x0a8
    VertexShadersPalSkinDepthBufferShadow[3] = shader_resource->GetVertexShader("VSHADER_PALSKIN_DIRLIGHT_5POINTLIGHTS_FOG_SHADOW");   // 00bd07bd +0x0b0
    VertexShadersPalSkinSpotDepthBufferShadow[0] = shader_resource->GetVertexShader("VSHADER_PALSKIN_DIRLIGHT_FOG_SPOT_SHADOW");   // 00bd0835 +0x118
    VertexShadersPalSkinSpotDepthBufferShadow[1] = shader_resource->GetVertexShader("VSHADER_PALSKIN_DIRLIGHT_2POINTLIGHTS_FOG_SPOT_SHADOW");   // 00bd08ad +0x120
    VertexShadersPalSkinSpotDepthBufferShadow[2] = shader_resource->GetVertexShader("VSHADER_PALSKIN_DIRLIGHT_4POINTLIGHTS_FOG_SPOT_SHADOW");   // 00bd0925 +0x128
    VertexShadersPalSkinSpotDepthBufferShadow[3] = shader_resource->GetVertexShader("VSHADER_PALSKIN_DIRLIGHT_5POINTLIGHTS_FOG_SPOT_SHADOW");   // 00bd099d +0x130
    VertexShadersPalSkinDepthShadowBufferRender = shader_resource->GetVertexShader("VSHADER_PALSKIN_SHADOW_BUFFER_RENDER");   // 00bd0a15 +0x198
    VertexShadersPalSkinEnvDepthBufferShadow[0] = shader_resource->GetVertexShader("VSHADER_PALSKIN_ENV_DIRLIGHT_FOG_SHADOW");   // 00bd0a8d +0x0b8
    VertexShadersPalSkinEnvDepthBufferShadow[1] = shader_resource->GetVertexShader("VSHADER_PALSKIN_ENV_DIRLIGHT_2POINTLIGHTS_FOG_SHADOW");   // 00bd0b05 +0x0c0
    VertexShadersPalSkinEnvDepthBufferShadow[2] = shader_resource->GetVertexShader("VSHADER_PALSKIN_ENV_DIRLIGHT_4POINTLIGHTS_FOG_SHADOW");   // 00bd0b7d +0x0c8
    VertexShadersPalSkinEnvDepthBufferShadow[3] = shader_resource->GetVertexShader("VSHADER_PALSKIN_ENV_DIRLIGHT_5POINTLIGHTS_FOG_SHADOW");   // 00bd0bf5 +0x0d0
    VertexShadersPalSkinEnvSpotDepthBufferShadow[0] = shader_resource->GetVertexShader("VSHADER_PALSKIN_ENV_DIRLIGHT_FOG_SPOT_SHADOW");   // 00bd0c6d +0x138
    VertexShadersPalSkinEnvSpotDepthBufferShadow[1] = shader_resource->GetVertexShader("VSHADER_PALSKIN_ENV_DIRLIGHT_2POINTLIGHTS_FOG_SPOT_SHADOW");   // 00bd0ce5 +0x140
    VertexShadersPalSkinEnvSpotDepthBufferShadow[2] = shader_resource->GetVertexShader("VSHADER_PALSKIN_ENV_DIRLIGHT_4POINTLIGHTS_FOG_SPOT_SHADOW");   // 00bd0d5d +0x148
    VertexShadersPalSkinEnvSpotDepthBufferShadow[3] = shader_resource->GetVertexShader("VSHADER_PALSKIN_ENV_DIRLIGHT_5POINTLIGHTS_FOG_SPOT_SHADOW");   // 00bd0dd5 +0x150
    VertexShadersPalSkinDP3BumpDepthBufferShadow[0] = dp3_shader_resource->GetVertexShader("VSHADER_PALSKIN_DP3_BUMP_DIRLIGHT_FOG_SHADOW");   // 00bd0e4d +0x0f8
    VertexShadersPalSkinDP3BumpDepthBufferShadow[1] = dp3_shader_resource->GetVertexShader("VSHADER_PALSKIN_DP3_BUMP_DIRLIGHT_2POINTLIGHTS_FOG_SHADOW");   // 00bd0ec7 +0x100
    VertexShadersPalSkinDP3BumpDepthBufferShadow[2] = dp3_shader_resource->GetVertexShader("VSHADER_PALSKIN_DP3_BUMP_DIRLIGHT_4POINTLIGHTS_FOG_SHADOW");   // 00bd0f41 +0x108
    VertexShadersPalSkinDP3BumpDepthBufferShadow[3] = dp3_shader_resource->GetVertexShader("VSHADER_PALSKIN_DP3_BUMP_DIRLIGHT_5POINTLIGHTS_FOG_SHADOW");   // 00bd0fbb +0x110
    VertexShaderPalSkinDP3BumpOutline = dp3_shader_resource->GetVertexShader("VSHADER_PALSKIN_DP3_BUMP_OUTLINE");   // 00bd1035 +0x2b0
    VertexShaderPalSkinDP3BumpOutlineSmoothFill = dp3_shader_resource->GetVertexShader("VSHADER_PALSKIN_DP3_BUMP_OUTLINE_SMOOTH_FILL");   // 00bd10af +0x2c0
    if (NEngineInternal::GPDisplayManager->GetPixelShaderVersion() >= 1400) {   // 0x00BD1129: mov ecx,[0x01436E1C]; call 0x009BEE00; cmp eax,578h; jb 0x00BD1CFF
        VertexShadersPalSkinSpotlight[0] = shader_resource->GetVertexShader("VSHADER_RIGIDSKIN_DIRLIGHT_SPOT_LIGHT");   // 00bd113f +0x30c
        VertexShadersPalSkinSpotlight[1] = shader_resource->GetVertexShader("VSHADER_RIGIDSKIN_DIRLIGHT_2POINTLIGHTS_SPOT_LIGHT");   // 00bd11b7 +0x314
        VertexShadersPalSkinSpotlight[2] = shader_resource->GetVertexShader("VSHADER_RIGIDSKIN_DIRLIGHT_4POINTLIGHTS_SPOT_LIGHT");   // 00bd122f +0x31c
        VertexShadersPalSkinSpotlight[3] = shader_resource->GetVertexShader("VSHADER_RIGIDSKIN_DIRLIGHT_5POINTLIGHTS_SPOT_LIGHT");   // 00bd12a7 +0x324
        VertexShadersPalSkinColourShadowBufferRender = shader_resource->GetVertexShader("VSHADER_PALSKIN_COLBUFF_SHADOW_BUFFER_RENDER");   // 00bd131f +0x1a0
        VertexShadersPalSkinColourBufferShadow[0] = shader_resource->GetVertexShader("VSHADER_PALSKIN_DIRLIGHT_FOG_COLBUFF_SHADOW");   // 00bd1397 +0x1a8
        VertexShadersPalSkinColourBufferShadow[1] = shader_resource->GetVertexShader("VSHADER_PALSKIN_DIRLIGHT_2POINTLIGHTS_FOG_COLBUFF_SHADOW");   // 00bd140f +0x1b0
        VertexShadersPalSkinColourBufferShadow[2] = shader_resource->GetVertexShader("VSHADER_PALSKIN_DIRLIGHT_4POINTLIGHTS_FOG_COLBUFF_SHADOW");   // 00bd1487 +0x1b8
        VertexShadersPalSkinColourBufferShadow[3] = shader_resource->GetVertexShader("VSHADER_PALSKIN_DIRLIGHT_5POINTLIGHTS_FOG_COLBUFF_SHADOW");   // 00bd14ff +0x1c0
        VertexShadersPalSkinSpotColourBufferShadow[0] = shader_resource->GetVertexShader("VSHADER_PALSKIN_DIRLIGHT_FOG_SPOT_COLBUFF_SHADOW");   // 00bd1577 +0x228
        VertexShadersPalSkinSpotColourBufferShadow[1] = shader_resource->GetVertexShader("VSHADER_PALSKIN_DIRLIGHT_2POINTLIGHTS_FOG_SPOT_COLBUFF_SHADOW");   // 00bd15ef +0x230
        VertexShadersPalSkinSpotColourBufferShadow[2] = shader_resource->GetVertexShader("VSHADER_PALSKIN_DIRLIGHT_4POINTLIGHTS_FOG_SPOT_COLBUFF_SHADOW");   // 00bd1667 +0x238
        VertexShadersPalSkinSpotColourBufferShadow[3] = shader_resource->GetVertexShader("VSHADER_PALSKIN_DIRLIGHT_5POINTLIGHTS_FOG_SPOT_COLBUFF_SHADOW");   // 00bd16df +0x240
        VertexShadersPalSkinEnvColourBufferShadow[0] = shader_resource->GetVertexShader("VSHADER_PALSKIN_ENV_DIRLIGHT_FOG_COLBUFF_SHADOW");   // 00bd1757 +0x1c8
        VertexShadersPalSkinEnvColourBufferShadow[1] = shader_resource->GetVertexShader("VSHADER_PALSKIN_ENV_DIRLIGHT_2POINTLIGHTS_FOG_COLBUFF_SHADOW");   // 00bd17cf +0x1d0
        VertexShadersPalSkinEnvColourBufferShadow[2] = shader_resource->GetVertexShader("VSHADER_PALSKIN_ENV_DIRLIGHT_4POINTLIGHTS_FOG_COLBUFF_SHADOW");   // 00bd1847 +0x1d8
        VertexShadersPalSkinEnvColourBufferShadow[3] = shader_resource->GetVertexShader("VSHADER_PALSKIN_ENV_DIRLIGHT_5POINTLIGHTS_FOG_COLBUFF_SHADOW");   // 00bd18bf +0x1e0
        VertexShadersPalSkinEnvSpotColourBufferShadow[0] = shader_resource->GetVertexShader("VSHADER_PALSKIN_ENV_DIRLIGHT_FOG_SPOT_COLBUFF_SHADOW");   // 00bd1937 +0x248
        VertexShadersPalSkinEnvSpotColourBufferShadow[1] = shader_resource->GetVertexShader("VSHADER_PALSKIN_ENV_DIRLIGHT_2POINTLIGHTS_FOG_SPOT_COLBUFF_SHADOW");   // 00bd19af +0x250
        VertexShadersPalSkinEnvSpotColourBufferShadow[2] = shader_resource->GetVertexShader("VSHADER_PALSKIN_ENV_DIRLIGHT_4POINTLIGHTS_FOG_SPOT_COLBUFF_SHADOW");   // 00bd1a27 +0x258
        VertexShadersPalSkinEnvSpotColourBufferShadow[3] = shader_resource->GetVertexShader("VSHADER_PALSKIN_ENV_DIRLIGHT_5POINTLIGHTS_FOG_SPOT_COLBUFF_SHADOW");   // 00bd1a9f +0x260
        VertexShadersPalSkinDP3BumpColourBufferShadow[0] = dp3_shader_resource->GetVertexShader("VSHADER_PALSKIN_DP3_BUMP_DIRLIGHT_FOG_COLBUFF_SHADOW");   // 00bd1b17 +0x208
        VertexShadersPalSkinDP3BumpColourBufferShadow[1] = dp3_shader_resource->GetVertexShader("VSHADER_PALSKIN_DP3_BUMP_DIRLIGHT_2POINTLIGHTS_FOG_COLBUFF_SHADOW");   // 00bd1b91 +0x210
        VertexShadersPalSkinDP3BumpColourBufferShadow[2] = dp3_shader_resource->GetVertexShader("VSHADER_PALSKIN_DP3_BUMP_DIRLIGHT_4POINTLIGHTS_FOG_COLBUFF_SHADOW");   // 00bd1c0b +0x218
        VertexShadersPalSkinDP3BumpColourBufferShadow[3] = dp3_shader_resource->GetVertexShader("VSHADER_PALSKIN_DP3_BUMP_DIRLIGHT_5POINTLIGHTS_FOG_COLBUFF_SHADOW");   // 00bd1c85 +0x220
    }   // 0x00BD1CFF = jb target
    VertexShaders2DPalSkin = shader_resource->GetVertexShader("VSHADER_PALSKIN_2D_DIRLIGHT");   // 00bd1cff +0x2c8
    VertexShaders2DPalSkinEnv = shader_resource->GetVertexShader("VSHADER_PALSKIN_2D_ENV_DIRLIGHT");   // 00bd1d77 +0x2d0
    VertexShaders2DPalSkinDP3Bump = dp3_shader_resource->GetVertexShader("VSHADER_PALSKIN_2D_DP3_BUMP_DIRLIGHT");   // 00bd1df0 +0x2e0

    // 0x00BD1E66 tail (retail store order: +2EC,+2FC,+2E8,+2F0,+300,+2F4,+304,+2F8 -- each `lea eax,[esi+X]; mov [esi+Y],eax`;
    // NB the landed FrameStart 0x00BCEA60 stores in a different order (2E8,2EC,2FC,2F0,...), so this ctor's source order differs:
    // start from the retail store order below):
    VertexShadersPalSkinShadow             = VertexShadersPalSkinDepthBufferShadow;           // +0x2EC = this+0x098
    VertexShadersPalSkinSpotShadow         = VertexShadersPalSkinSpotDepthBufferShadow;       // +0x2FC = this+0x118
    VertexShadersPalSkinShadowBufferRender = &VertexShadersPalSkinDepthShadowBufferRender;    // +0x2E8 = this+0x198
    VertexShadersPalSkinEnvShadow          = VertexShadersPalSkinEnvDepthBufferShadow;        // +0x2F0 = this+0x0B8
    VertexShadersPalSkinEnvSpotShadow      = VertexShadersPalSkinEnvSpotDepthBufferShadow;    // +0x300 = this+0x138
    VertexShadersPalSkinEnvBumpShadow      = VertexShadersPalSkinEnvBumpDepthBufferShadow;    // +0x2F4 = this+0x0D8
    VertexShadersPalSkinEnvBumpSpotShadow  = VertexShadersPalSkinEnvBumpSpotDepthBufferShadow;// +0x304 = this+0x158
    VertexShadersPalSkinDP3BumpShadow      = VertexShadersPalSkinDP3BumpDepthBufferShadow;    // +0x2F8 = this+0x0F8 (pop edi interleaved)
    // VertexShadersPalSkinDP3BumpSpotShadow (+0x308) is NOT assigned.
    TextureOverlayFadeDistandeFactor = 2.0f;                                                  // 0x00BD1EC7 mov [esi+32Ch],40000000h
}                                                                                             // 0x00BD1ED1 mov eax,esi ... ret 4
```
- Not assigned by retail (leave alone): VertexShadersPalSkinEnvBump[4] (+0x78), the EnvBump*/DP3BumpSpot* DepthBuffer and ColourBuffer
  arrays (+0x158, +0x178, +0x1E8, +0x268, +0x288), VertexShaders2DPalSkinEnvBump (+0x2D8), DP3BumpSpotShadow (+0x308).
- No cold blocks, no jump tables. Only conditional: the >= 1400 test (jb 0x00BD1CFF). 61 GetVertexShader statements
  (11 of them on dp3_shader_resource: exactly the DP3_BUMP names; 50 on shader_resource).
