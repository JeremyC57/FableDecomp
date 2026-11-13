# OUTLINE 0x00A059E0 CRenderStateManager::SetDefaultRenderStates (retail order)

## Frame
- Prologue (interleaved with block 1): `mov edx,[ecx+2A10h] ; sub esp,18h ; push ebx ; push ebp ; lea eax,[ecx+2A0Ch] ; mov ebx,1 ;
  push esi ; xor ebp,ebp ; cmp edx,ebx ; push edi ; je`. Saved order ebx, ebp, esi, edi. No EH, no /GS, no frame pointer.
- Epilogue 0xA078EE pop edi/esi/ebp/ebx ; add esp,18h ; ret. ecx = this the whole way.
- Stack (offsets after the 4 pushes):
  [esp+10h] long stage (loop counter home; init `mov [esp+10h],ebp`=0 at 0xA06951; inc/cmp 8/store/jl at 0xA07228)
  [esp+14h] float 0.0f = SetTextureMipmapLODBias arg (stored once before loop, 0xA06955)
  [esp+18h] float temp: every float setter outside the loop (`mov [esp+18h],imm ; mov edi,[esp+18h]`); in the loop = BumpEnv arg a (m00)
  [esp+1Ch] BumpEnv arg b (-> Mat10), [esp+20h] arg c (-> Mat01), [esp+24h] arg d (-> Mat11); all 0.0f, stored before the loop
  in order [14h],[24h],[20h],[1Ch],[18h] (0xA06955..0xA06975) = args materialised right to left, hoisted out of the loop.
- Registers: ebx = 1, ebp = 0 (before loop), eax = &state, edi = value (or edx when value comes from a stack slot).

## Expansion template (each block = header SetRenderState + AddStateToUpdateList)
```
if (state.DesiredState != v) { if (!(state.BookmarkMask & CurrentBookmarkMask)) { state.BookmarkMask |= CurrentBookmarkMask;
  StateStack[StateStackSize].State=&state; StateStack[StateStackSize].Value=state.DesiredState; ++StateStackSize; }
  state.DesiredState=v; if(!state.DirtyListFlag){state.DirtyListFlag=true; StateUpdateList[StateUpdateListSize]=&state; ++StateUpdateListSize;} }
```
Bookmark-test register form tag (lane gap, not controllable): A=`mov esi,[ecx+2814h] ; mov edx,[eax+8]`, B=`mov edx,[ecx+2814h] ;
mov esi,[eax+8]`, C=`mov esi,[eax+8] ; mov edx,[ecx+2814h]`, D=`mov edx,[eax+8] ; mov esi,[ecx+2814h]`.
Join `je` target of each block = the next block's address (except the shared-global cases noted: retail duplicates the next
block's `mov edx,[table]` on the fall-through path and jumps past it).

## Body (DS = DeviceState, offsets relative to DS = this+0x2818)
```
void CRenderStateManager::SetDefaultRenderStates()
{
  SetDitherEnable(true);                              // 0xA059E0 DS+0x1F4 v=ebx(1)        A
  SetZBufferEnable(true);                             // 0xA05A4D DS+0x050 1               A
  SetZBufferWriteEnable(true);                        // 0xA05AAC DS+0x03C 1               A
  SetZBufferTestFunc(ZBUFFER_FUNC_LESS_EQUAL);        // 0xA05B0B DS+0x064 [0x1396E04]=Z[3] A
  SetPolyFillMode(POLY_FILL_SOLID);                   // 0xA05B72 DS+0x014 [0x1396FA4]=[0]  A
  SetPolyShadeMode(POLY_SHADE_GOURAUD);               // 0xA05BD9 DS+0x000 [0x1396FA0]=[1]  A
  SetAlphaTestEnable(false);                          // 0xA05C40 DS+0x0B4 ebp(0)          A
  SetAlphaRef(0x80);                                  // 0xA05C9F DS+0x0C8 edi=80h         B
  SetAlphaTestFunc(ALPHA_TEST_LESS);                  // 0xA05D03 DS+0x08C [0x1396DDC]=[1]  B
  SetSourceAlphaBlendMode(ALPHA_BLEND_ONE);           // 0xA05D6A DS+0x104 [0x1396F6C]=[1]  B
  SetDestAlphaBlendMode(ALPHA_BLEND_ZERO);            // 0xA05DD1 DS+0x118 [0x1396F68]=[0]  B
  SetPolyCullMode(POLY_CULL_CCW);                     // 0xA05E38 DS+0x078 [0x1396FB0]=[0]  B
  SetAlphaBlendEnable(false);                         // 0xA05E9F DS+0x0A0 0               B
  SetSpecularEnable(false);                           // 0xA05EFE DS+0x1CC 0               B
  SetTextureFactor(0xFF000000);                       // 0xA05F5D DS+0x1B8 edi imm         B
  SetTextureFactor(0xFFFFFFFF);                       // 0xA05FC1 same eax (no lea), `or edi,-1`  C
  SetClippingEnable(true);                            // 0xA0601A DS+0x208 1               C
  SetFogEnable(false);                                // 0xA06079 DS+0x12C 0               C
  SetFogColour(0x00FFFFFF);                           // 0xA060D8 DS+0x140 edi imm         C
  SetFogMode(FOG_MODE_NONE);                          // 0xA0613C DS+0x190 [0x1396F58]=[0]  C (je -> 0xA061A9; reload at 0xA061A3)
  SetFogVertexMode(FOG_MODE_NONE);                    // 0xA061A9 DS+0x1A4 [0x1396F58]      C
  SetFogDensity(0.0f);                                // 0xA0620A DS+0x17C [esp+18h]=0      C
  SetFogStart(0.1f);                                  // 0xA06275 DS+0x154 0x3DCCCCCD       C
  SetFogEnd(0.9f);                                    // 0xA062E0 DS+0x168 0x3F666666       C
  SetVertexBlending(VERTEX_BLEND_DISABLE);            // 0xA0634B DS+0x0DC [0x1396FBC]=[0]  C
  SetColourWriteEnable(true);  /* or (true,true,true,true) -> 0xF */ // 0xA063B2 DS+0x028 edi=0Fh  C
  SetDepthOffset(0.0f);                               // 0xA06416 DS+0x348 [esp+18h]=0      C
  SetSlopeScaledDepthOffset(0.0f);                    // 0xA06481 DS+0x35C [esp+18h]=0      C
  SetPointSpriteEnable(false);                        // 0xA064EC DS+0x280 0               C
  SetPointScaleEnable(false);                         // 0xA0654B DS+0x294 0               C
  SetPointSize(64.0f);                                // 0xA065AA DS+0x2A8 0x42800000       C
  SetPointSizeMin(1.0f);                              // 0xA06615 DS+0x2BC 0x3F800000       C
  SetPointSizeMax(64.0f);                             // 0xA06680 DS+0x2D0 0x42800000       C
  SetPointScaleConstantFactor(1.0f);                  // 0xA066EB DS+0x2E4 0x3F800000       C
  SetPointScaleDistanceFactor(0.0f);                  // 0xA06756 DS+0x2F8 0               C
  SetPointScaleDistanceSqrFactor(0.0f);               // 0xA067C1 DS+0x30C 0               C
  SetBlendOp(BLEND_OP_ADD);                           // 0xA0682C DS+0x424 [0x1396ED0]=[0]  C
  SetLightingEnable(false);                           // 0xA06893 DS+0x21C 0               C
  SetNormalizeNormals(false);                         // 0xA068F2 DS+0x230 0               C
  // 0xA06951 loop setup (stores above), 0xA0697D `lea ecx,[ecx]` pad, loop top 0xA06980 (16-aligned):
  // eax=stage ; edi=[table] ; lea eax,[eax+eax*4] ; lea ebp,[ecx+eax*4]  (ebp = this + stage*0x14; lea eax,[ebp+off])
  for (long stage = 0; stage < 8; ++stage) {
    SetTextureOperation(stage, TEXTURE_OP_DISABLE);          // 0xA06980 DS+0x9EC [0x1396E70]=[0]  C
    SetTextureAlphaOperation(stage, TEXTURE_OP_DISABLE);     // 0xA069EF DS+0xA8C [0x1396E70]      C
    SetTextureArgument1(stage, TEXTURE_ARG_TEXTURE);         // 0xA06A54 DS+0xB2C [0x1396EF0]=[2]  C
    SetTextureArgument2(stage, TEXTURE_ARG_DIFFUSE);         // 0xA06AB9 DS+0xBCC [0x1396EEC]=[1]  C
    SetTextureAlphaArgument1(stage, TEXTURE_ARG_TEXTURE);    // 0xA06B1E DS+0xC6C [2]              C
    SetTextureAlphaArgument2(stage, TEXTURE_ARG_DIFFUSE);    // 0xA06B83 DS+0xD0C [1]              C
    SetTextureFilterModeMin(stage, TEXTURE_FILTER_LINEAR);   // 0xA06BE8 DS+0x8AC [0x1396E5C]=[1]  C
    SetTextureFilterModeMag(stage, TEXTURE_FILTER_LINEAR);   // 0xA06C4D DS+0x94C [1]              C
    SetTextureUVModeU(stage, TEXTURE_UV_CLAMP);              // 0xA06CB2 DS+0x6CC [0x1396DD0]=[2]  C
    SetTextureUVModeV(stage, TEXTURE_UV_CLAMP);              // 0xA06D17 DS+0x76C [2]              C
    SetTextureUVModeW(stage, TEXTURE_UV_CLAMP);              // 0xA06D7C DS+0x80C [2]              C
    SetTextureMipmapLODBias(stage, 0.0f);                    // 0xA06DE1 DS+0x58C edx=[esp+14h] (loaded twice)  C
    SetTextureMipMapMode(stage, TEXTURE_MIPMAP_LINEAR);      // 0xA06E48 DS+0x62C [0x1396E68]=[1]  C
    SetTextureCoordIndex(stage, stage);                      // 0xA06EAD DS+0xDAC edx=[esp+10h]    C
    SetTextureTransformMode(stage, TEXTURE_TRANSFORM_DIABLED); // 0xA06F14 DS+0xE4C [0x1396FCC]=[0] C
    SetTextureBumpEnvMatrix(stage, 0.0f, 0.0f, 0.0f, 0.0f);  // 0xA06F79 Mat00 DS+0xF8C [esp+18h] B; 0xA06FE0 Mat10 DS+0x10CC
                                                             // [esp+1Ch] B; 0xA07043 Mat01 DS+0x102C [esp+20h] B; 0xA070A6 Mat11 DS+0x116C [esp+24h] B
    SetTextureCoordWrapUVW(stage, false, false, false);      // 0xA07109 DS+0x44C `xor edi,edi`    B
    SetTextureMaxAnisotropy(stage, 1);                       // 0xA0716A DS+0x4EC ebx(1)           C
    SetTextureBorderColour(stage, 0);                        // 0xA071C9 DS+0xEEC edi (still 0)    B
  }                                                          // 0xA07228 mov eax,[esp+10h]; inc; cmp 8; store; jl 0xA06980
  SetTextureOperation(0, TEXTURE_OP_MODULATE);               // 0xA0723A DS+0x9EC [0x1396E7C]=[3]  B (je -> 0xA072A7)
  SetTextureAlphaOperation(0, TEXTURE_OP_MODULATE);          // 0xA072A7 DS+0xA8C (reload 0xA072A1) B
  SetTextureArgument1(0, TEXTURE_ARG_TEXTURE);               // 0xA07308 DS+0xB2C [0x1396EF0]      B
  SetTextureArgument2(0, TEXTURE_ARG_DIFFUSE);               // 0xA0736D DS+0xBCC [0x1396EEC]      B
  SetTextureAlphaArgument1(0, TEXTURE_ARG_TEXTURE);          // 0xA073D2 DS+0xC6C                  B
  SetTextureAlphaArgument2(0, TEXTURE_ARG_DIFFUSE);          // 0xA07437 DS+0xD0C                  B
  SetStencilEnable(false);                                   // 0xA0749C DS+0x370 `xor ebp,ebp`    A
  SetStencilOpFail(STENCIL_OP_MODE_KEEP);                    // 0xA074FD DS+0x384 [0x1396F38]=[0]  A (je->0xA0756A, reload 0xA07564)
  SetStencilOpZFail(STENCIL_OP_MODE_KEEP);                   // 0xA0756A DS+0x398                  A (je->0xA075D1, reload 0xA075CB)
  SetStencilOpPass(STENCIL_OP_MODE_KEEP);                    // 0xA075D1 DS+0x3AC                  A
  SetStencilFunc(STENCIL_FUNC_ALWAYS);                       // 0xA07632 DS+0x3C0 [0x1396E34]=[7]  D
  SetStencilRef(0);                                          // 0xA07699 DS+0x3D4 ebp              D
  SetStencilMask(0xFFFFFFFF);                                // 0xA076F8 DS+0x3E8 `or edi,-1`      D
  SetStencilWriteMask(0xFFFFFFFF);                           // 0xA0775A DS+0x3FC edi reused       D
  SetAmbientMaterialColourSource(MATERIAL_COLOUR_SOURCE_MATERIAL);  // 0xA077B9 DS+0x244 [0x1396FF4]=[0] C (je->0xA07826, reload 0xA07820)
  SetDiffuseMaterialColourSource(MATERIAL_COLOUR_SOURCE_MATERIAL);  // 0xA07826 DS+0x258   C (reload 0xA07887)
  SetSpecularMaterialColourSource(MATERIAL_COLOUR_SOURCE_MATERIAL); // 0xA0788D DS+0x26C   C
}                                                            // 0xA078EE epilogue
```
- No cold blocks, no jump tables, no calls. 77 SetRenderState expansions (TextureFactor twice, BumpEnvMatrix = 4).
- Helper texts to define in the row TU (retail out-of-line oracles, check each with xdis): SetTextureFactor 0xA09050, SetFogDensity
  0xA08E70, SetFogMode 0xA08F50 / SetFogVertexMode 0xA08FD0 (D3DFogTableMode[mode]), SetVertexBlending 0xA08D10
  (D3DVertexBlendModeTable[mode]), SetDepthOffset 0xA09CA0, SetSlopeScaledDepthOffset 0xA09D10, SetPoint* 0xA09340..0xA09670,
  SetNormalizeNormals 0xA09140 (`enable?1:0`), Set*MaterialColourSource 0xA091C0.. (D3DMaterialColourSourceTable[src]),
  SetTextureCoordWrapUVW 0xA09760, SetTextureBorderColour 0xA09A00, SetTextureBumpEnvMatrix 0xA09A80 (a->00, b->10, c->01, d->11),
  SetStencilOpFail/ZFail (no copy; same as OpPass through D3DStencilOperationModeTable), plus Stencil/ColourWrite via opt-ins.
