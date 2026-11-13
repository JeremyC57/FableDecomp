# OUTLINE 0x00B9ECE0 CEnginePrimitiveManagerStaticMeshesBase::CEnginePrimitiveManagerStaticMeshesBase(CArray<CEngineScene*>& scenes)

## Frame
- No ebp frame, no SEH, no /GS. `sub esp,10h` then push ebx, esi, edi (ebp pushed late at 0xB9ED22, popped 0xBA0F22). `ret 4`.
- esi = this all along. edi = scratch (bank1 resource for the very first call; the returned temp's address after each call).
- ebx: loop counter for the member-array ctors -> then &VertexShaderOutline (+0x2D0, set 0xB9EF10, used in its assignment at
  0xB9F472) -> then 0 (`xor ebx,ebx` 0xB9F4EE) used as the zero register for every later null test/`cmp` and for OverridePrimitive=0.
- ebp: &VertexShadersStatic (+0x78) during the ctor loops (and its [0] assignment `[ebp+4]` 0xB9EF9C), then bank1 resource
  (`mov ebp,[esp+14h]` 0xB9F001) for the rest.
- Locals (locals = esp+0x10 with 4 regs saved):
  - local+0x0  CCharString temporary (4 bytes) for blocks 0..54 (`lea ecx,[esp+18h]` after the two pushes).
  - local+0x4  bank1 `const CShaderResource*` spill (written 0xB9EF82, read once into ebp 0xB9F001).
  - local+0x8  CVertexShader by-value result temporary (vptr +8, Data +0xC); the CPixelShader result in the last block too.
  - ARG slot (esp+0x24): after the base ctor the `scenes` parameter is dead; the compiler stores the bank2 resource pointer there
    (0xB9EF75 `mov [esp+28h],ecx` with one push), and from block 55 (0xBA0A91) on, the CCharString temporaries live there.
- No EH states (no /EHsc).

## Pseudo-C++ (retail order)
```cpp
namespace NEngineInternal { extern CEngineResourceManager* GPResourceManager; extern CDisplayManager* GPDisplayManager; }

CEnginePrimitiveManagerStaticMeshesBase::CEnginePrimitiveManagerStaticMeshesBase(CArray<CEngineScene*>& scenes)
    : CEnginePrimitiveManagerMeshBase(scenes)          // 0xB9ECED call 0x00BEBC30; vptr 0x012A50DC stored 0xB9ECF2
    // member ctors in declaration order, implicit:
    // 0xB9ECF8 VertexShadersStaticSpotlight[4]  (+0x38) loop ebx=4, edi+=8, call 0x00A62FB0
    // 0xB9ED0D StippleAlpha +0x58 ; 0xB9ED22 push ebp ; 0xB9ED23 Static +0x78 (edi=ebp=this+0x78) ; +0x98 Env ; +0xB8 EnvBump ;
    // +0xD8 DepthBufferShadow ; +0xF8 ; +0x118 ; +0x138 ; +0x158 ; +0x178 ; +0x198 ; +0x1B8 (all `mov ebx,4` loops)
    // 0xB9EE2D six singles +0x1D8,+0x1E0,+0x1E8,+0x1F0,+0x1F8,+0x200 (direct calls)
    // 0xB9EE6F loops +0x208,+0x228,+0x248,+0x268,+0x288,+0x2A8
    // 0xB9EF05 CPixelShader +0x2C8 (call 0x00A5EC00)
    // 0xB9EF10 +0x2D0 (lea ebx,[esi+2D0h] kept in ebx), +0x2D8, +0x2E0, +0x2E8 (call 0x00A62FB0)
{
    // 0xB9EF3E
    PointLightShaderIndex[0] = 0; PointLightShaderIndex[1] = 1; PointLightShaderIndex[2] = 1;
    PointLightShaderIndex[3] = 2; PointLightShaderIndex[4] = 2; PointLightShaderIndex[5] = 3;

    // 0xB9EF62: ONE load of GPResourceManager, both banks read from it (eax) BEFORE the first CCharString is built.
    // Retail loads +0x29D0 (bank 2 -> ecx -> arg slot) first, then +0x29C8 (bank 1 -> edi -> local+4). Try bank1 declared first;
    // swap if the two loads come out reversed.
    const CShaderResource* static_shaders = NEngineInternal::GPResourceManager->GetVShaderResource((EShaderBankNo)1);
    const CShaderResource* bump_shaders   = NEngineInternal::GPResourceManager->GetVShaderResource((EShaderBankNo)2);

    // Each statement below is ONE block of 0x76..0x7E bytes:
    //   push -1 ; push lit ; lea ecx,str ; call 0x0099EBF0            CCharString tmp(lit)
    //   push &str ; push &result ; mov ecx,res ; call 0x00A5D5F0      res->GetVertexShader(tmp)  (hidden result = local+8)
    //   mov ecx,[member.Data] ; test/cmp ; mov edi,eax ; je ; dec [ecx+10h] ; jne ; vcall [ecx]->slot0(1)    CVertexShader::operator=
    //   mov eax,[edi+4] ; mov [member.Data],eax ; inc [eax+10h]
    //   ~CVertexShader(result): vptr 0x0129B3D0, Release(); vptr 0x0129A030 (~CShaderBase)
    //   lea ecx,str ; call 0x0099EAE0                                 ~CCharString
    // Block start address = the `push -1`.
    VertexShadersStatic[0]                           = static_shaders->GetVertexShader("VSHADER_STATIC_DIRLIGHT_FOG");   // +0x78   // block @00b9ef73
    VertexShadersStatic[1]                           = static_shaders->GetVertexShader("VSHADER_STATIC_DIRLIGHT_2POINTLIGHTS_FOG");   // +0x80   // block @00b9eff1
    VertexShadersStatic[2]                           = static_shaders->GetVertexShader("VSHADER_STATIC_DIRLIGHT_4POINTLIGHTS_FOG");   // +0x88   // block @00b9f071
    VertexShadersStatic[3]                           = static_shaders->GetVertexShader("VSHADER_STATIC_DIRLIGHT_5POINTLIGHTS_FOG");   // +0x90   // block @00b9f0ed
    VertexShadersStaticEnv[0]                        = static_shaders->GetVertexShader("VSHADER_STATIC_ENV_DIRLIGHT_FOG");   // +0x98   // block @00b9f169
    VertexShadersStaticEnv[1]                        = static_shaders->GetVertexShader("VSHADER_STATIC_ENV_DIRLIGHT_2POINTLIGHTS_FOG");   // +0xa0   // block @00b9f1e5
    VertexShadersStaticEnv[2]                        = static_shaders->GetVertexShader("VSHADER_STATIC_ENV_DIRLIGHT_4POINTLIGHTS_FOG");   // +0xa8   // block @00b9f261
    VertexShadersStaticEnv[3]                        = static_shaders->GetVertexShader("VSHADER_STATIC_ENV_DIRLIGHT_5POINTLIGHTS_FOG");   // +0xb0   // block @00b9f2dd
    VertexShaders2DStatic                            = static_shaders->GetVertexShader("VSHADER_2D_STATIC_DIRLIGHT");   // +0x1d8   // block @00b9f359
    VertexShaders2DStaticEnv                         = static_shaders->GetVertexShader("VSHADER_2D_STATIC_ENV_DIRLIGHT");   // +0x1e0   // block @00b9f3d5
    VertexShaderOutline                              = static_shaders->GetVertexShader("VSHADER_STATIC_OUTLINE");   // +0x2d0   // block @00b9f451
    VertexShaderOutlineSmoothFill                    = static_shaders->GetVertexShader("VSHADER_STATIC_OUTLINE_SMOOTH_FILL");   // +0x2e0   // block @00b9f4c7
    VertexShaderDepthShadowBufferRender              = static_shaders->GetVertexShader("VSHADER_STATIC_SHADOW_BUFFER_RENDER");   // +0x1f8   // block @00b9f545
    VertexShadersStaticDepthBufferShadow[0]          = static_shaders->GetVertexShader("VSHADER_STATIC_DIRLIGHT_FOG_SHADOW");   // +0xd8   // block @00b9f5c1
    VertexShadersStaticDepthBufferShadow[1]          = static_shaders->GetVertexShader("VSHADER_STATIC_DIRLIGHT_2POINTLIGHTS_FOG_SHADOW");   // +0xe0   // block @00b9f63d
    VertexShadersStaticDepthBufferShadow[2]          = static_shaders->GetVertexShader("VSHADER_STATIC_DIRLIGHT_4POINTLIGHTS_FOG_SHADOW");   // +0xe8   // block @00b9f6b9
    VertexShadersStaticDepthBufferShadow[3]          = static_shaders->GetVertexShader("VSHADER_STATIC_DIRLIGHT_5POINTLIGHTS_FOG_SHADOW");   // +0xf0   // block @00b9f735
    VertexShadersStaticSpotDepthBufferShadow[0]      = static_shaders->GetVertexShader("VSHADER_STATIC_DIRLIGHT_FOG_SPOT_SHADOW");   // +0x158   // block @00b9f7b1
    VertexShadersStaticSpotDepthBufferShadow[1]      = static_shaders->GetVertexShader("VSHADER_STATIC_DIRLIGHT_2POINTLIGHTS_FOG_SPOT_SHADOW");   // +0x160   // block @00b9f82d
    VertexShadersStaticSpotDepthBufferShadow[2]      = static_shaders->GetVertexShader("VSHADER_STATIC_DIRLIGHT_4POINTLIGHTS_FOG_SPOT_SHADOW");   // +0x168   // block @00b9f8a9
    VertexShadersStaticSpotDepthBufferShadow[3]      = static_shaders->GetVertexShader("VSHADER_STATIC_DIRLIGHT_5POINTLIGHTS_FOG_SPOT_SHADOW");   // +0x170   // block @00b9f925
    VertexShadersStaticEnvDepthBufferShadow[0]       = static_shaders->GetVertexShader("VSHADER_STATIC_ENV_DIRLIGHT_FOG_SHADOW");   // +0xf8   // block @00b9f9a1
    VertexShadersStaticEnvDepthBufferShadow[1]       = static_shaders->GetVertexShader("VSHADER_STATIC_ENV_DIRLIGHT_2POINTLIGHTS_FOG_SHADOW");   // +0x100   // block @00b9fa1d
    VertexShadersStaticEnvDepthBufferShadow[2]       = static_shaders->GetVertexShader("VSHADER_STATIC_ENV_DIRLIGHT_4POINTLIGHTS_FOG_SHADOW");   // +0x108   // block @00b9fa99
    VertexShadersStaticEnvDepthBufferShadow[3]       = static_shaders->GetVertexShader("VSHADER_STATIC_ENV_DIRLIGHT_5POINTLIGHTS_FOG_SHADOW");   // +0x110   // block @00b9fb15
    VertexShadersStaticEnvSpotDepthBufferShadow[0]   = static_shaders->GetVertexShader("VSHADER_STATIC_ENV_DIRLIGHT_FOG_SPOT_SHADOW");   // +0x178   // block @00b9fb91
    VertexShadersStaticEnvSpotDepthBufferShadow[1]   = static_shaders->GetVertexShader("VSHADER_STATIC_ENV_DIRLIGHT_2POINTLIGHTS_FOG_SPOT_SHADOW");   // +0x180   // block @00b9fc0d
    VertexShadersStaticEnvSpotDepthBufferShadow[2]   = static_shaders->GetVertexShader("VSHADER_STATIC_ENV_DIRLIGHT_4POINTLIGHTS_FOG_SPOT_SHADOW");   // +0x188   // block @00b9fc89
    VertexShadersStaticEnvSpotDepthBufferShadow[3]   = static_shaders->GetVertexShader("VSHADER_STATIC_ENV_DIRLIGHT_5POINTLIGHTS_FOG_SPOT_SHADOW");   // +0x190   // block @00b9fd05
    VertexShadersStaticDP3BumpDepthBufferShadow[0]   = bump_shaders->GetVertexShader("VSHADER_STATIC_DP3_BUMP_DIRLIGHT_FOG_SHADOW");   // +0x138   // block @00b9fd81
    VertexShadersStaticDP3BumpDepthBufferShadow[1]   = bump_shaders->GetVertexShader("VSHADER_STATIC_DP3_BUMP_DIRLIGHT_2POINTLIGHTS_FOG_SHADOW");   // +0x140   // block @00b9fdff
    VertexShadersStaticDP3BumpDepthBufferShadow[2]   = bump_shaders->GetVertexShader("VSHADER_STATIC_DP3_BUMP_DIRLIGHT_4POINTLIGHTS_FOG_SHADOW");   // +0x148   // block @00b9fe7d
    VertexShadersStaticDP3BumpDepthBufferShadow[3]   = bump_shaders->GetVertexShader("VSHADER_STATIC_DP3_BUMP_DIRLIGHT_5POINTLIGHTS_FOG_SHADOW");   // +0x150   // block @00b9fefb
    VertexShaderDP3BumpOutline                       = bump_shaders->GetVertexShader("VSHADER_STATIC_DP3_BUMP_OUTLINE");   // +0x2d8   // block @00b9ff79
    VertexShaders2DStaticDP3Bump                     = bump_shaders->GetVertexShader("VSHADER_2D_STATIC_DP3_BUMP_DIRLIGHT");   // +0x1f0   // block @00b9fff7
    VertexShaderDP3BumpOutlineSmoothFill             = bump_shaders->GetVertexShader("VSHADER_STATIC_DP3_BUMP_OUTLINE_SMOOTH_FILL");   // +0x2e8   // block @00ba0075
    VertexShadersStaticStippleAlpha[0]               = static_shaders->GetVertexShader("VSHADER_STATIC_DIRLIGHT_FOG_STIPPLE_ALPHA");   // +0x58   // block @00ba00f3
    VertexShadersStaticStippleAlpha[1]               = static_shaders->GetVertexShader("VSHADER_STATIC_DIRLIGHT_2POINTLIGHTS_FOG_STIPPLE_ALPHA");   // +0x60   // block @00ba0169
    VertexShadersStaticStippleAlpha[2]               = static_shaders->GetVertexShader("VSHADER_STATIC_DIRLIGHT_4POINTLIGHTS_FOG_STIPPLE_ALPHA");   // +0x68   // block @00ba01df
    VertexShadersStaticStippleAlpha[3]               = static_shaders->GetVertexShader("VSHADER_STATIC_DIRLIGHT_5POINTLIGHTS_FOG_STIPPLE_ALPHA");   // +0x70   // block @00ba0255
    VertexShadersStaticSpotlight[0]                  = static_shaders->GetVertexShader("VSHADER_STATIC_DIRLIGHT_SPOT_LIGHT");   // +0x38   // block @00ba02cb
    VertexShadersStaticSpotlight[1]                  = static_shaders->GetVertexShader("VSHADER_STATIC_DIRLIGHT_2POINTLIGHTS_SPOT_LIGHT");   // +0x40   // block @00ba0341
    VertexShadersStaticSpotlight[2]                  = static_shaders->GetVertexShader("VSHADER_STATIC_DIRLIGHT_4POINTLIGHTS_SPOT_LIGHT");   // +0x48   // block @00ba03b7
    VertexShadersStaticSpotlight[3]                  = static_shaders->GetVertexShader("VSHADER_STATIC_DIRLIGHT_5POINTLIGHTS_SPOT_LIGHT");   // +0x50   // block @00ba042d

    // 0xBA04A3: mov ecx,[0x01436E1C] ; call 0x009BEE00 ; cmp eax,578h ; jb 0xBA0EED   (unsigned compare: ps_1_4 = 1400)
    if (NEngineInternal::GPDisplayManager->GetPixelShaderVersion() >= 1400)
    {
        VertexShadersStaticColourBufferShadow[0]         = static_shaders->GetVertexShader("VSHADER_STATIC_DIRLIGHT_FOG_COLBUFF_SHADOW");   // +0x208   // block @00ba04b9
        VertexShadersStaticColourBufferShadow[1]         = static_shaders->GetVertexShader("VSHADER_STATIC_DIRLIGHT_2POINTLIGHTS_FOG_COLBUFF_SHADOW");   // +0x210   // block @00ba0535
        VertexShadersStaticColourBufferShadow[2]         = static_shaders->GetVertexShader("VSHADER_STATIC_DIRLIGHT_4POINTLIGHTS_FOG_COLBUFF_SHADOW");   // +0x218   // block @00ba05b1
        VertexShadersStaticColourBufferShadow[3]         = static_shaders->GetVertexShader("VSHADER_STATIC_DIRLIGHT_5POINTLIGHTS_FOG_COLBUFF_SHADOW");   // +0x220   // block @00ba062d
        VertexShadersStaticEnvColourBufferShadow[0]      = static_shaders->GetVertexShader("VSHADER_STATIC_ENV_DIRLIGHT_FOG_COLBUFF_SHADOW");   // +0x228   // block @00ba06a9
        VertexShadersStaticEnvColourBufferShadow[1]      = static_shaders->GetVertexShader("VSHADER_STATIC_ENV_DIRLIGHT_2POINTLIGHTS_FOG_COLBUFF_SHADOW");   // +0x230   // block @00ba0725
        VertexShadersStaticEnvColourBufferShadow[2]      = static_shaders->GetVertexShader("VSHADER_STATIC_ENV_DIRLIGHT_4POINTLIGHTS_FOG_COLBUFF_SHADOW");   // +0x238   // block @00ba07a1
        VertexShadersStaticEnvColourBufferShadow[3]      = static_shaders->GetVertexShader("VSHADER_STATIC_ENV_DIRLIGHT_5POINTLIGHTS_FOG_COLBUFF_SHADOW");   // +0x240   // block @00ba081d
        VertexShadersStaticDP3BumpColourBufferShadow[0]  = bump_shaders->GetVertexShader("VSHADER_STATIC_DP3_BUMP_DIRLIGHT_FOG_COLBUFF_SHADOW");   // +0x268   // block @00ba0899
        VertexShadersStaticDP3BumpColourBufferShadow[1]  = bump_shaders->GetVertexShader("VSHADER_STATIC_DP3_BUMP_DIRLIGHT_2POINTLIGHTS_FOG_COLBUFF_SHADOW");   // +0x270   // block @00ba0917
        VertexShadersStaticDP3BumpColourBufferShadow[2]  = bump_shaders->GetVertexShader("VSHADER_STATIC_DP3_BUMP_DIRLIGHT_4POINTLIGHTS_FOG_COLBUFF_SHADOW");   // +0x278   // block @00ba0995
        VertexShadersStaticDP3BumpColourBufferShadow[3]  = bump_shaders->GetVertexShader("VSHADER_STATIC_DP3_BUMP_DIRLIGHT_5POINTLIGHTS_FOG_COLBUFF_SHADOW");   // +0x280   // block @00ba0a13
        // from here (0xBA0A91) the CCharString temporaries sit in the dead arg slot (bump_shaders' last use was the block before)
        VertexShadersStaticSpotColourBufferShadow[0]     = static_shaders->GetVertexShader("VSHADER_STATIC_DIRLIGHT_FOG_SPOT_COLBUFF_SHADOW");   // +0x288   // block @00ba0a91
        VertexShadersStaticSpotColourBufferShadow[1]     = static_shaders->GetVertexShader("VSHADER_STATIC_DIRLIGHT_2POINTLIGHTS_FOG_SPOT_COLBUFF_SHADOW");   // +0x290   // block @00ba0b0d
        VertexShadersStaticSpotColourBufferShadow[2]     = static_shaders->GetVertexShader("VSHADER_STATIC_DIRLIGHT_4POINTLIGHTS_FOG_SPOT_COLBUFF_SHADOW");   // +0x298   // block @00ba0b89
        VertexShadersStaticSpotColourBufferShadow[3]     = static_shaders->GetVertexShader("VSHADER_STATIC_DIRLIGHT_5POINTLIGHTS_FOG_SPOT_COLBUFF_SHADOW");   // +0x2a0   // block @00ba0c05
        VertexShadersStaticEnvSpotColourBufferShadow[0]  = static_shaders->GetVertexShader("VSHADER_STATIC_ENV_DIRLIGHT_FOG_SPOT_COLBUFF_SHADOW");   // +0x2a8   // block @00ba0c81
        VertexShadersStaticEnvSpotColourBufferShadow[1]  = static_shaders->GetVertexShader("VSHADER_STATIC_ENV_DIRLIGHT_2POINTLIGHTS_FOG_SPOT_COLBUFF_SHADOW");   // +0x2b0   // block @00ba0cfd
        VertexShadersStaticEnvSpotColourBufferShadow[2]  = static_shaders->GetVertexShader("VSHADER_STATIC_ENV_DIRLIGHT_4POINTLIGHTS_FOG_SPOT_COLBUFF_SHADOW");   // +0x2b8   // block @00ba0d79
        VertexShadersStaticEnvSpotColourBufferShadow[3]  = static_shaders->GetVertexShader("VSHADER_STATIC_ENV_DIRLIGHT_5POINTLIGHTS_FOG_SPOT_COLBUFF_SHADOW");   // +0x2c0   // block @00ba0df5
        VertexShaderColourShadowBufferRender             = static_shaders->GetVertexShader("VSHADER_STATIC_COLBUFF_SHADOW_BUFFER_RENDER");   // +0x200   // block @00ba0e71
    }
    // 0xBA0EED (jb target): string built FIRST, then the global re-read inline (`mov ecx,[0x1436E98] ; mov ecx,[ecx+2A98h]`),
    // call 0x00A5D720; CPixelShader::operator= (count at +8, `mov [esi+2CCh],ebx` nulls Data after the release), temp
    // ~CPixelShader (vptr 0x0129A054, Release at +8) then ~CShaderBase (0x0129A030), ~CCharString. `pop ebp` at 0xBA0F22.
    PixelShaderDiffuseFogStippleAlpha                = NEngineInternal::GPResourceManager->GetPShaderResource()->GetPixelShader("PSHADER_TEXTURE_DIFFUSE_FOG_STIPPLE_ALPHA");   // +0x2c8   // block @00ba0eed

    // 0xBA0F7A tail (retail store order):
    VertexShaderShadowBufferRender = &VertexShaderDepthShadowBufferRender;      // +0x308 = this+0x1F8
    VertexShadersStaticShadow      = VertexShadersStaticDepthBufferShadow;      // +0x2F0 = this+0xD8
    VertexShadersStaticSpotShadow  = VertexShadersStaticSpotDepthBufferShadow;  // +0x300 = this+0x158
    OverridePrimitive = 0;                                                      // +0x30C (ebx)
    RenderOnlyNMeshes = -1;                                                     // +0x310
}   // 0xBA0FAF mov eax,esi ; pop esi ; pop ebx ; add esp,10h ; ret 4
```
No jump tables, no cold blocks after the epilogue. The only branch besides the inline null/refcount tests is the jb at 0xBA04B3.

## Expected residues / levers
- Order of the two bank loads (declaration order of the locals); whether the compiler keeps bank1 in a spill + ebp exactly like
  retail (ebp chosen once the +0x78 base is no longer needed).
- `test ecx,ecx` (first 11 blocks) vs `cmp ecx,ebx` (after `xor ebx,ebx` at 0xB9F4EE) is pure register allocation; it follows
  if ebx = &VertexShaderOutline is CSE'd from the member-ctor phase (it is a compiler choice, no source lever expected).
- bank2 blocks alternate `mov ecx,[esp+28h]` placement (before/after the second push) -- scheduling, not source.
- If GetVShaderResource is spelled through the header inline (VShaderResources[bank].Get()) the loads are `[eax+29C8h]`/`[eax+29D0h]`.
