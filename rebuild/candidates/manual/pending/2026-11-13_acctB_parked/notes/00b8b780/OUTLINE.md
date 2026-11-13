# OUTLINE 0x00B8B780 void CEngineBillboardGenerator::RenderBillboard(const C3DMesh2* mesh, CEngineBillboard* billboard, float a3, float a4, long alphaRef)

## Frame facts
- `push ebp ; lea ebp,[esp-64h] ; sub esp,1A0h` (biased ebp frame, see DOSSIER s.1), cookie `[0139C8A8h] -> [ebp+60h]`,
  `push ebx ; push esi` at 0x00B8B7B0, `push edi` at 0x00B8B7DB; epilogue pops edi, esi, ebx, check_cookie, `ret 14h`.
- No EH states (no /EHsc). Function-scope objects in construction order: capture block (no storage), CRenderTarget target
  [ebp-80h], CScopedShadersEnable shaders [ebp+4Ch] (declared late, after FlushRender). Destroyed at the end in reverse.

## Frame map (ebp-relative; slots are shared by the compiler)
| range | local | type / ctor-dtor |
|---|---|---|
| -13Ch..-0CDh | verts[4] | CTVertexRHWColTex1 (X,Y,Z,RHW,DiffuseColour,U1,V1; 0x1C each); DiffuseColour never written |
| -0CCh..-0BDh | GetSurfaceLevel(level) temp (loop 2) | CSurface, inline dtor 0x00B8C17A/0x00B8C184 |
| -0BCh..-08Dh | identity | CMatrix3x4 (SetIdentity) for SetViewTransform |
| -8Ch | GFFloatToLongFloor 1.0f constant | float |
| -88h..-81h | GetDimensions(0) result | C2DExtentsI (only X read); -84h also the floor block's `result+1.0` temp and, in loop 2, `const CTexture* tex` for DrawAsIndexedTris |
| -80h..-5Dh | target | CRenderTarget (vptr 0x0129C800 -80h, RenderToSurface -7Ch, ZBufferSurface -6Ch) |
| -5Ch..-2Dh | world | CMatrix3x4 (result, SetWorldTransform arg) |
| -2Ch..+03h | copy | CMatrix3x4 copy of world (multiply operand) |
| +04h..+33h | view | CMatrix3x4 copy of g_pEngineCamera->NoScaleViewTransform |
| +34h..+43h | sphere | C3DBoundingSphere (Center +34h, Radius +40h); +3Ch..+43h reused as GetDimensions(level) result in loop 2; +40h reused as the floor block 0.5f |
| +44h | this, then unsigned long i (primitive index) | |
| +48h | (long)FrameWidth -> float mip value (asm input); later CShaderRenderManager* spill | |
| +4Ch | CRGBColour(0,0,0,0) for ClearRenderTarget; long mip (asm result); CRenderManagerCore* spill (FlushRender); CScopedShadersEnable (+4Ch Old vertex, +4Dh Old pixel) | |
| +50h..+5Fh | GetSurfaceLevel(0) temp CSurface; C4DVector(1,1,1,1) temp | |
| +54h..+5Fh | -Center temp (54h,58h); GetTranslation()*Radius temp (54h..5Ch); C3DTextureStages stages (54h Diffuse=0, 58h Bump=1, 5Ch UseBlank=false); unsigned short indices[6] (loop 2) | |
| +60h | /GS cookie | |
| +6Ch arg mesh (loop 2: long level), +70h arg billboard (after 0x00B8C135: float -1.0f LOD bias), +74h a3, +78h a4, +7Ch alphaRef (loop 1: i*0xA0 byte offset; loop 2: texture width) | | |

## Block by block (retail order)
```
// 0x00B8B780 prologue, cookie. [ebp+44h] = this.
CRenderStateScopedCaptureBlock capture;                       // 0x00B8B7AB..7DF inline StartCaptureBlock on [013CB508]
                                                              //   (sphere copy from mesh+10h..1Ch is interleaved here: 0x00B8B793..7B8)
CRenderTarget target;                                         // 0x00B8B7DC call CBaseClass() 0x0099A2D0; 0x00B8B7F9.. vptr 0129C800,
                                                              //   both CSurface vptrs 0122F84C, +4/+8 zeroed
const CSurface* zbuffer = billboard->ZBuffer;                 // 0x00B8B7EA (ebx = billboard)
if (!zbuffer)                                                 // 0x00B8B7F2 jne 0x00B8B831
    zbuffer = g_pEngine->GetScratchZBufferD24S8(billboard->Texture->GetDimensions(0).X);   // 0x00B8B81E GetDimensions, 0x00B8B82C [01436EA4]
target.AttachZBuffer(*zbuffer);                               // 0x00B8B835 call 0x00A10E90
target.AttachTarget(billboard->Texture->GetSurfaceLevel(0));  // 0x00B8B842 0x009F9E00 (temp +50h), 0x00B8B84B 0x00A10E60, temp dtor 0x00B8B850..856
NEngineInternal::GPDisplayManager->SetRenderTarget(target);   // 0x00B8B865 0x009BF160
NEngineInternal::GPRenderManager->ClearRenderTarget(CRGBColour(0,0,0,0), 0);   // 0x00B8B886 0x009D8CF0 (stores R,G,B,A = +4Eh,+4Dh,+4Ch,+4Fh)
CMatrix3x4 identity; identity.SetIdentity();                  // 0x00B8B898..906
NEngineInternal::GPShaderRenderManager->SetViewTransform(identity);              // 0x00B8B910 0x00988350
NEngineInternal::GPShaderRenderManager->SetProjectionTransform(1,0,0,0, 0,1,0,0, 0,0,1,0, 0,0,0,1);   // 0x00B8B947 0x00988490

C3DBoundingSphere sphere = mesh->GetBoundingSphere();         // copied at 0x00B8B793..7B8 (eax=mesh)
float inv = 1.0f / sphere.Radius;                             // 0x00B8B94C fld [0122DED8] ; fdiv [ebp+40h]
CMatrix3x4 view = g_pEngineCamera->NoScaleViewTransform;      // 0x00B8B955..969 rep movsd from [01436EA0]+114h
C3DVector negCentre = -sphere.Center;                         // 0x00B8B96B.. X->[+54h], Y->[+58h], Z stays on st
// world = translate(-centre) scaled by inv: stores E11=inv, E12..E33 off-diagonal = inv*0.0f (one fmul [0122DEDC], fst x6),
//   E22=inv, E33=inv, E41=-cx*inv, E42=-cy*inv, E43=-cz*inv (0x00B8B980..9C4). The `inv*0.0f` (not a 0.0f store) and the
//   bare `inv` diagonal fit `world.SetTranslate(negCentre); world.PostScale(inv);` (FableWin ?SetTranslate@CMatrix3x4@@QAEXABVC3DVector@@@Z,
//   ?PostScale@CMatrix3x4@@QAEXM@Z, both inline; VC folds 1.0f*x, cannot fold 0.0f*x) -- verify with fwsize.
CMatrix3x4 copy(world);                                       // 0x00B8B993/99B/9A0 + 0x00B8B9C7 rep movsd -5Ch -> -2Ch
// world = copy * view (3x3 of view; view's translation NOT added): 0x00B8B9CB..0x00B8BAF3. Exact retail terms
//   (first product's load order):
//   E11 = v.E11*c.E11 + v.E31*c.E13 + v.E21*c.E12      (`fld v ; fmul c`; k order 0,2,1 -- row 1 only)
//   E12 = v.E32*c.E13 + v.E22*c.E12 + v.E12*c.E11      (k order 2,1,0)
//   E13 = v.E33*c.E13 + v.E23*c.E12 + v.E13*c.E11     (kept on st, not stored -> scaled below)
//   E21 = c.E23*v.E31 + c.E22*v.E21 + c.E21*v.E11     (`fld c ; fmul v` from row 2 on)
//   E22 = c.E23*v.E32 + c.E22*v.E22 + c.E21*v.E12
//   E23 = c.E23*v.E33 + c.E22*v.E23 + c.E21*v.E13     (kept on st)
//   E31/E32/E33 with c.E33,c.E32,c.E31; E41/E42/E43 with c.E43,c.E42,c.E41 (k = 2,1,0), no + v.E4x.
//   Candidate: FableWin ?SetMultiplyNoTranslation@CMatrix3x4@@QAEXABV1@0@Z (this = a*b without b's translation), world.SetMultiplyNoTranslation(copy, view);
//   compare the SetMultiply note in engine/CMatrix3x4.h (row-1 vs row-2..4 operand order flips with a copy).
world.E13 *= 0.5f; world.E23 *= 0.5f; world.E33 *= 0.5f; world.E43 = world.E43 * 0.5f + 0.5f;   // 0x00B8BAF6..B25 (0122F59C)
billboard->Scale = sphere.Radius * 2.0f;                      // 0x00B8BB28 fadd st,st -> [ebx+14h]
billboard->Offset = world.GetTranslation() * sphere.Radius;   // 0x00B8BB30..B59 temp [+54h..5Ch] then 3 dword copies (lea eax,[ebx+8] hoisted to 0x00B8BA74)
billboard->Offset.X = -billboard->Offset.X;                   // 0x00B8BB5C
billboard->Offset.Z *= -2.0f;                                 // 0x00B8BB65 [01246050]
NEngineInternal::GPShaderRenderManager->SetWorldTransform(world);  // 0x00B8BB78 0x009881F0
GPRenderManager->SetAlphaTestEnable(true);                    // 0x00B8BB7D +28CCh (edi=1)
GPRenderManager->SetAlphaBlendEnable(false);                  // 0x00B8BBEE +28B8h (ebx=0)
GPRenderManager->SetAlphaRef(alphaRef);                       // 0x00B8BC57 +28E0h
GPRenderManager->SetAlphaTestFunc(ALPHA_TEST_GREATER);        // 0x00B8BCC1 +28A4h <- [01396DE8]
GPRenderManager->SetFogEnable(false);                         // 0x00B8BD2E +2944h
GPShaderRenderManager->SetPixelShaderConstant(2, C4DVector(1.0f,1.0f,1.0f,1.0f));   // 0x00B8BD90..BD4 (PSConstTable+20h, UpdateFlags|=2)
GPShaderRenderManager->ApplyVertexShader(VertexShader, false);    // 0x00B8BDE6 0x00988020 (this+10h)
GPShaderRenderManager->ApplyPixelShader(PixelShader);             // 0x00B8BDF5 0x00988140 (this+18h)

for (unsigned long i = 0; i < mesh->PrimitiveCount; ++i) {    // test 0x00B8BDFA (jbe 0x00B8C06A), head 0x00B8BE13, bottom 0x00B8C046..064 jb
    C3DPrimitive2& prim = mesh->Primitives[i];                // edi; byte offset kept in [ebp+7Ch] (+=0A0h)
    unsigned long graphic = mesh->GetMaterial(prim.MaterialIndex)->DiffuseMapBankIndex;   // 0x00B8BE25..31 (index*30h, +10h)
    if (graphic == 0) continue;                               // 0x00B8BE37 je 0x00B8C046
    const CGraphicDataBank* bank = NEngineInternal::GPResourceManager->PeekGraphicBank(0);
    long width = bank->PeekGraphicInfoFrame(graphic, 0)->GraphicInfo->Header.FrameWidth;   // 0x00B8BE3D..BEA3 (no null check of the frame;
                                                              //   0x00B8BE9E xor eax,eax then deref)
    float mip = (float)width * prim.AverageTextureStretch * a3 / (a4 * GetMipmapSizeMultiplier());   // 0x00B8BEB7..BED5 -> [ebp+48h]
    long level = GFFloatToLongFloor(mip);                     // 0x00B8BED8..BEFB asm (0.5f [ebp+40h] stored 0x00B8BEB0, 1.0f [ebp-8Ch] 0x00B8BEBA)
    if (level < 1) level = 1; else if (level > 255) level = 255;   // 0x00B8BF01..F14 (in eax)
    C3DTextureStages stages(0, 1, false);                     // 0x00B8BF2B..F36 ([+5Ch]=0 byte, [+54h]=0, [+58h]=1)
    if (!CEnginePrimitiveManagerStaticMeshesBase::SetupStaticBlocksNoMorph(prim, *mesh, stages,
            GFGetTopBitInByte((unsigned char)level),          // 0x00B8BF1F movzx ; movzx [0143B990+edx]
            NEngineInternal::GPResourceManager->PeekGraphicBank(0), false))   // 0x00B8BF4E 0x00BA48F0 (ecx=prim, edx=mesh)
        continue;                                             // 0x00B8BF55
    GPShaderRenderManager->SelectVertexBuffer(*prim.VertexBuffer);   // 0x00B8BF5B..F7A D3DDevice(+1D4h)->SetStreamSource(0, vb->PVertexBuffer, 0, vb->Stride)
    GPShaderRenderManager->PrepareForRender(0x11);            // 0x00B8BF80..C029 (manager kept in ebx, &UpdateFlags in esi)
    CEnginePrimitiveManagerStaticMeshesBase::SetupFirstTextures(prim, stages);              // 0x00B8C033 0x00B9D670
    CEnginePrimitiveManagerStaticMeshesBase::RenderMeshPrimitiveBlocks(prim, stages, false, false);   // 0x00B8C041 0x00BA1110
}
GPRenderManager->FlushRender();                               // 0x00B8C06A..C0E0 (SetStreamSource(0,0,0,0) +190h, SetIndices(0) +1A0h,
                                                              //   SetTexture loop +104h over TextureStagesInUse, then = 0)
target.DetachTarget();                                        // 0x00B8C0EA 0x00A10E10
CScopedShadersEnable shaders(false, false);                   // 0x00B8C0EF..C116
for (long level = 1; level < billboard->Texture->GetNoLevels(); ++level) {   // pre-test 0x00B8C127/12F jle 0x00B8C639; body 0x00B8C140
                                                              //   (pad lea esp,[esp] 0x00B8C13C); bottom 0x00B8C623..633 jl
    // 0x00B8C135: [ebp+70h] = -1.0f hoisted (the LOD-bias float local)
    long width = billboard->Texture->GetDimensions(level).X;  // 0x00B8C14A -> [ebp+7Ch]
    target.DetachTarget();                                    // 0x00B8C157
    target.AttachTarget(billboard->Texture->GetSurfaceLevel(level));   // 0x00B8C166, 0x00B8C16F, temp dtor 0x00B8C174..184
    NEngineInternal::GPDisplayManager->SetRenderTarget(target);        // 0x00B8C193
    CTVertexRHWColTex1 verts[4];                              // 0x00B8C198..C227: X/Y = {0,0},{w,0},{0,w},{w,w} (w = (float)width, fild),
                                                              //   U/V = {0,0},{1,0},{0,1},{1,1}
    float halfTexel = 0.5f / (float)width;                    // 0x00B8C1EF fdivr [0122F59C]
    for (long v = 0; v < 4; ++v) { verts[v].RHW = 1.0f; verts[v].Z = 0.0f; verts[v].U1 += halfTexel; verts[v].V1 += halfTexel; }   // 0x00B8C231..C25E
                                                              //   (pointer at &U1, count-down ecx=4, RHW stored before Z)
    GPRenderManager->SetZBufferEnable(false);                 // 0x00B8C260 +2868h
    unsigned short indices[6] = { 0, 1, 2, 1, 3, 2 };         // 0x00B8C2CE..C2F3 ([+54h]..[+5Eh])
    GPRenderManager->SetVertexFormat(0x144);                  // 0x00B8C2F9 0x00A0AA30 (XYZRHW|DIFFUSE|TEX1)
    GPRenderManager->SetTextureArgument1(0, TEXTURE_ARG_TEXTURE);          // 0x00B8C2FE +3344h <- [01396EF0]
    GPRenderManager->SetTextureOperation(0, TEXTURE_OP_SELECT_ARG1);       // 0x00B8C372 +3204h <- [01396E74]
    GPRenderManager->SetTextureAlphaArgument1(0, TEXTURE_ARG_TEXTURE);     // 0x00B8C3E0 +3484h
    GPRenderManager->SetTextureAlphaOperation(0, TEXTURE_OP_SELECT_ARG1);  // 0x00B8C44E +32A4h
    GPRenderManager->SetAlphaBlendEnable(false);              // 0x00B8C4BC +28B8h
    GPRenderManager->SetAlphaTestEnable(false);               // 0x00B8C526 +28CCh
    GPRenderManager->SetTextureMipmapLODBias(0, -1.0f);       // 0x00B8C58E +2DA4h (value from [ebp+70h])
    const CTexture* tex = billboard->Texture;                 // 0x00B8C5FC/608 -> [ebp-84h]
    GPRenderManager->DrawAsIndexedTris(&tex, 1, verts, sizeof(CTVertexRHWColTex1), 4, indices, 2);   // 0x00B8C61E 0x00A0AEA0
}
NEngineInternal::GPDisplayManager->SetRenderTargetAsBackScreen();   // 0x00B8C63F 0x009BF220
g_pEngineCamera->RefreshD3DTransforms();                      // 0x00B8C64A 0x00B2FC50
// end of scope: ~CScopedShadersEnable 0x00B8C64F..68A (vertex then pixel: cmp old,[013BC800] ; je ; old ? Enable : Disable),
// ~CRenderTarget 0x00B8C68F..6AD (ZBufferSurface then RenderToSurface: vptr 0122F84C + Uninitialise; then ~CBaseClass 0x0099A2E0),
// ~CRenderStateScopedCaptureBlock 0x00B8C6B2..6B8 RestoreCaptureBlock 0x00A05840; cookie check; ret 14h.
```
No jump tables; no cold blocks after the epilogue (every block is in line).

## Render-state setter block shape (11 copies)
`mov reg,[GPRenderManager] ; mov esi,[reg+off+4] (Desired) ; lea edx,[reg+off] ; cmp esi,value ; je skip ; [bookmark test/or + captured push] ;
mov [edx+4],value ; if (!InList) { InList=1 ; UpdateList[count++]=&state }` = engine/CRenderStateManager.h SetRenderState. The
bookmark `test a,b` operand order varies per block (header lane-gap note); here blocks 1,2 test `eax,esi`, 3,4 `esi,eax`, loop-2 blocks
1..4 `esi,ecx`, 5..9 `ecx,esi`.
